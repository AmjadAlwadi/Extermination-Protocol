#include "world.h"
#include "bullet.h"
#include "player.h"
#include "menu.h"
#include "packet.h"
#include "constants.h"

#include <tmxlite/ImageLayer.hpp>
#include <thread>


World::World(sf::RenderWindow *window, b2World* b2_world)
{
    this->window = window;
    this->b2_world = b2_world;


    // load map
    setup_map();
    menu::step_loading_screen();

    bullet::load_textures();
    bullet::textures_loaded = true;

    packet::setup_udp_socket(receive_socket);

    menu::step_loading_screen();

    // for shaders
    never_restarting_clock.restart();

    wind_sound_buffer = new sf::SoundBuffer();

    // load wind sound
    std::string directory_path = ASSETS_PATH"sounds/";

    std::string filePath = directory_path + "wind.wav";
    if (!wind_sound_buffer->loadFromFile(filePath)){
        std::cerr << "Failed to load sound from: " << filePath << std::endl;
        return;
    }

    wind_sound.setBuffer(*wind_sound_buffer);
    wind_sound.setLoop(true);
    wind_sound.setVolume(0.4f);

    music_sound_buffer = new sf::SoundBuffer();

    filePath = directory_path + "timouse2.flac";

    if (!music_sound_buffer->loadFromFile(filePath)){
        std::cerr << "Failed to load sound from: " << filePath << std::endl;
        return;
    }

    music_sound.setBuffer(*music_sound_buffer);
    music_sound.setLoop(true);
    music_sound.setVolume(5);

    menu::step_loading_screen();

}








World::~World()
{
    for(auto player : players){

        delete player;
    }

    for(auto zomb : zombies){

        delete zomb;
    }



    for(auto map_layer : map_layers){

        delete map_layer;

    }



    for(auto tile : tiles){

        delete tile;
    }


    for(auto terrain_object : terrain_objects){

        delete terrain_object;
    }




    // delete player resources


    for (auto it = Player::feet_texture.begin(); it != Player::feet_texture.end(); ++it) {
        delete it->second;
    }



    for(const auto& t1 : Player::texture){

        for(const auto& t2 : t1.second){

            delete t2.second;

        }
    }


    for(const auto& t1 : Player::shadow_texture){

        for(const auto& t2 : t1.second){

            delete t2.second;

        }
    }



    for(const auto& t1 : Player::feet_texture){

        delete t1.second;

    }


    delete Player::status_bar_texture;


    for(auto t1 : Player::shoot_sound_buffer){
        delete t1.second;
    }

    delete Player::steps_sound_buffer;



    // delete zombie resources

    for(auto t1 : Zombie::texture){
        delete t1.second;
    }


    for(auto t1 : Zombie::shadow_texture){
        delete t1.second;
    }


    for(auto t1 : Zombie::zombie_sound_buffer){
        delete t1.second;
    }


    // delete terrain_object resources

    for(auto t1 : terrain_object::texture){

        delete t1.second;

    }

    for(auto t1 : terrain_object::shadow_texture){

        delete t1.second;

    }




    // delete vfx

    for(auto t1 : vfx::texture){

        delete t1.second;

    }



    // delete world sound data

    delete wind_sound_buffer;



    // delete loading screen resources

    for(auto t1 : menu::bullets){
        delete t1;
    }



    // world_tile
    delete world_tile::shadow_texture;

}




void World::receiver_thread()
{
    while (true) {

        if (menu::current_game_state != game_state::PLAYING || players.size() == 0)
            return;


        packet data_packet;
        sf::IpAddress sender_ip;

        if (data_packet.receive_packet(receive_socket, sender_ip)) {

            switch (data_packet.type) {
            
            case packet_type::BULLET_DATA:

                for (auto player : players) {
                    if (player->ip == data_packet.player_ip){
                        player->apply_bullet_data_from_packet(data_packet);
                    }
                }

                break;

            case packet_type::PLAYER_DATA:

                for (auto player : players) {
                    if (player->ip == data_packet.player_ip) {
                        player->apply_data_from_packet(data_packet);
                    }
                }

                break;

            case packet_type::ZOMBIE_DATA:

                spawn_zombie_flag = true;
                new_zombie_spawn_position.x = data_packet.x;
                new_zombie_spawn_position.y = data_packet.y;
                break;


            default:
                break;

            }

 
            if (server) {

                // do the forwarding to other players only if server
                for (auto player : players) {

                    if (!(player->ip == sender_ip || player->server))
                        data_packet.send_packet(send_socket, player->ip);

                }
            }

        }

    }
}




void World::send_zombie_information(sf::Vector2f spawn_location)
{

    // sending information
    if (!coop_mode) {

        if (players.size() == 0) {
            return;
        }

        else {

            // send data to everyone except myself
            packet data_packet;
            data_packet.type = packet_type::ZOMBIE_DATA;
            data_packet.x = spawn_location.x;
            data_packet.y = spawn_location.y;

            for (auto player : players) {

                if (!player->server)
                    data_packet.send_packet(send_socket, player->ip);
            }

        }

    }

}




void World::setup_map()
{

    std::vector<sf::Vector2f> small_trees;

    // load first map
    tmx::Map map;

    if(map.load(ASSETS_PATH"map/map.tmx"))
    {  
        int index = 0;

        const auto& layers = map.getLayers();

        for(const auto& layer : layers)
        {
            if(layer->getType() == tmx::Layer::Type::Object)
            {
                const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                const auto& objects = objectLayer.getObjects();

                if(layer->getName() == "custom objects"){

                    for(const auto& object : objects)
                    {
                        if(object.getClass() == "tree")
                            small_trees.push_back(sf::Vector2f(object.getPosition().x,object.getPosition().y));

                    }

                }else if(layer->getName() == "key locations"){

                    for(const auto& object : objects)
                    {
                        if(object.getName() == "spawn")
                            player_spawn_location = sf::Vector2f(object.getPosition().x,object.getPosition().y);

                    }

                }

            }

            else if(layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();

                for(auto prop : tileLayer.getProperties()){
                    if(prop.getName() == "collidable" && prop.getBoolValue()){
                        collidable_layer_index = index;
                    }
                }

            }

            index++;

        }


    }

    menu::step_loading_screen();

    // initialize values

    number_of_tiles_x = map.getTileCount().x;
    number_of_tiles_y = map.getTileCount().y;

    tile_width = map.getTileSize().x;
    tile_height = map.getTileSize().y;

    world_bound_x = map.getBounds().width;
    world_bound_y = map.getBounds().height;

    background_color = sf::Color(map.getBackgroundColour().r,map.getBackgroundColour().g,map.getBackgroundColour().b,map.getBackgroundColour().a);


    // load layers
    for(int i = 0; i < map.getLayers().size();i++){

        MapLayer* layer = new MapLayer(map, i);
        map_layers.push_back(layer);
    }


    menu::step_loading_screen();




    // get collidable tiles and turn them to world_tiles
    std::vector<tile_information> collidable_tiles_information;

    for(int x = 0; x < number_of_tiles_x;x++){
        for(int y = 0; y < number_of_tiles_y;y++){

            struct World::tile_information tile = tile_to_rect(x,y);

            if(!tile.collidable){
                continue;
            }

            tile.tile_texture_pos.x = map.getTilesets()[0].getTile(map_layers[collidable_layer_index]->getTile(x,y).ID)->imagePosition.x;
            tile.tile_texture_pos.y = map.getTilesets()[0].getTile(map_layers[collidable_layer_index]->getTile(x,y).ID)->imagePosition.y;

            collidable_tiles_information.push_back(tile);

        }
    }


   // setup shadow texture for all world tiles using the tileset
   world_tile::setup_shadow_sprite(map.getTilesets()[0].getImagePath());


    // tiles to world_tiles and to box2d tiles
    for(auto tile_info : collidable_tiles_information){

        world_tile* tile = new world_tile(window,b2_world,shadow_direction,tile_info.tile_texture_pos);

        tile->setup_tilebody(tile_info.tile_rect);

        tile->update_shadow_position();

        tiles.push_back(tile);

    }




    // setup terrain

    menu::step_loading_screen();

    for(auto small_tree : small_trees){

        float delta = (get_random_number(100)/100.f)/8.f;

        //terrain_object::terrain_type random_tree_type = static_cast<terrain_object::terrain_type>(get_random_number(3));

        terrain_objects.push_back(new terrain_object(window,terrain_object::PIXELATED_TREE,shadow_direction,small_tree,sf::Vector2f(TERRAIN_SCALE + delta,TERRAIN_SCALE + delta)));
    }


}











void World::draw_map(sf::Time elapsed_time)
{

    for(int i = 0; i< map_layers.size();i++){

        if(i == collidable_layer_index){

            for (auto player : this->players) {
                player->draw_shadow();
            }

            for (auto zombie : this->zombies) {
                zombie->draw_shadow();
            }

            for (auto player : this->players) {
                player->draw_vfx();
            }

            for (auto zombie : this->zombies) {
                zombie->draw_vfx();
            }

            // update players
            for(auto player : this->players){

                // update bullets
                for(auto bullet : player->bullets){
                    bullet->update(elapsed_time);
                }

                player->update(elapsed_time);

            }

            // update zombies
            for(auto zombie : this->zombies){
                zombie->update(elapsed_time);
            }

            // draw shadow of all tiles
            for(auto wt : tiles){
                wt->draw_shadow();
            }

        }


        // animate layers
        map_layers.at(i)->update(elapsed_time);
        window->draw(*map_layers.at(i));


        for(auto terrain_object : terrain_objects){
            terrain_object->animate();
        }


    }

    if (players.size() != 0) {
        players[0]->draw_status_bar();
    }
    

}






int World::get_random_number(int limit) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, limit);
    return distrib(gen);

}







void World::restart(){

    for(auto t1 : zombies){
        t1->alive = false;
    }

    for(auto t2 : players){
        t2->alive = false;
    }

    remove_dead();
    

    // setup player and zombies
    if (coop_mode) {

        players.push_back(new Player(window, this, player_spawn_location, false, 0, sf::IpAddress(), true, false));

        for (int i = 1; i < number_of_coop_players;i++) {
            players.push_back(new Player(window, this, player_spawn_location + (float)i * sf::Vector2f(30, 0), true, i - 1, sf::IpAddress(), true, false));
        }

        std::cout << "coop mode" << std::endl;

    }

    else {

        std::cout << "online mode" << std::endl;

        setup_online_game();

    }



    never_restarting_clock.restart();

    wind_sound.stop();
    wind_sound.play();

    music_sound.stop();
    music_sound.play();

    move_camera();

}




extern World* current_world;



sf::Vector2f World::random_coords(){

    bool collidable = true;
    float x = 0;
    float y = 0;

    while (collidable) {
        
        x = (float) get_random_number(world_bound_x);
        y = (float) get_random_number(world_bound_y);

        // test if there is a collidable tile there
        struct World::tile_information tile = current_world->tile_to_rect(x/(current_world->tile_width), y/(current_world->tile_height));
        collidable = tile.collidable;

  
    }

    return sf::Vector2f(x,y);

}


sf::Vector2f World::random_coords(sf::Vector2f position) {

    bool collidable = true;
    float x = 0;
    float y = 0;



    while (collidable) {     

        x = (float) std::max(std::min(position.x - CAMERA_WIDTH + get_random_number(2.f * CAMERA_WIDTH),world_bound_x),0.f);
        y = (float) std::max(std::min(position.y - CAMERA_WIDTH + get_random_number(2.f * CAMERA_WIDTH),world_bound_y),0.f);

        // test if there is a collidable tile there
        struct World::tile_information tile = current_world->tile_to_rect(x / (current_world->tile_width), y / (current_world->tile_height));
        collidable = tile.collidable;

    }

    return sf::Vector2f(x, y);

}



void World::switch_to_map(int id)
{
    std::cout << "switch to map" << std::endl;
}

extern void to_playing(sf::RenderWindow* window);
extern sf::Text* joined_players_number;
extern volatile bool start_command;



void World::setup_online_game()
{

    packet::setup_udp_socket(receive_socket);

    if (server) {

        // i join first
        players.push_back(new Player(window, this, random_coords(), false, 0, sf::IpAddress::getLocalAddress(), true, true));

        std::thread player_join_thread([&]() {

            while (true) {

                // player joins
                packet player_join_packet;
                sf::IpAddress sender_ip;

                if (!player_join_packet.receive_packet(receive_socket, sender_ip)) {
                    continue;
                }
                    

                if (player_join_packet.type == packet_type::PLAYER_JOIN) {
                    players.push_back(new Player(window, this, sf::Vector2f(player_join_packet.x, player_join_packet.y), true, 0, sender_ip, false, false));
                    joined_players_number->setString("Joined Players : " + std::to_string(players.size()));
                }
                    

                if (start_command)
                    return;
            }
        });

        player_join_thread.detach();

        // wait for start button to be pressed
        while (!start_command) {}
        std::cout << "start button pressed." << std::endl;
        start_command = false;

        // send all player's info to all other players
        for (auto player_1 : players) {

            // sending player_1's info to player_2

            packet player_info;
            player_info.type = packet_type::PLAYER_INFO;
            player_info.player_ip = player_1->ip;
            player_info.x = player_1->sprite.getPosition().x;
            player_info.y = player_1->sprite.getPosition().y;

            for (auto player_2 : players) {

                if (player_2->ip != player_1->ip && !player_2->server) {

                    player_info.send_packet(send_socket, player_2->ip);
                }

            }
        }

        // send difficulty level
        packet difficulty_level;
        difficulty_level.type = packet_type::SET_DIFFICULTY_LEVEL;
        difficulty_level.x = menu::current_difficulty_level;


        for (auto player : players) {

            if (!player->server)
                difficulty_level.send_packet(send_socket, player->ip);
        }


        // start the game
        packet start_game;
        start_game.type = packet_type::START_GAME;

        for (auto player : players) {

            if (!player->server)
                start_game.send_packet(send_socket, player->ip);
        }

        std::cout << "end of server procedure" << std::endl;

    }

    else {

        std::cout << "joining" << std::endl;

        sf::Vector2f spawn_point = random_coords();

        // join the server
        packet player_join_packet;
        player_join_packet.type = packet_type::PLAYER_JOIN;
        player_join_packet.x = spawn_point.x;
        player_join_packet.y = spawn_point.y;
        player_join_packet.send_packet(send_socket, server_ip);

        std::cout << "sent player join packet to server ip : " << server_ip.toString() << std::endl;
       
        // i join first
        players.push_back(new Player(window, this, spawn_point, false, 0, sf::IpAddress::getLocalAddress(), true, false));

        sf::Clock timeout_for_start_command;
        timeout_for_start_command.restart();

        volatile bool start_command_arrived = false;

        std::thread player_info_thread([&]() {

            bool server_joined = false;

            while (timeout_for_start_command.getElapsedTime().asSeconds() < START_GAME_DELAY) {

                packet player_info;
                sf::IpAddress sender_ip;
                

                if (!player_info.receive_packet(receive_socket, sender_ip))
                    continue;

                if (player_info.type == packet_type::PLAYER_INFO) {
                    
                    // server joins
                    if (player_info.player_ip.toInteger() == server_ip.toInteger()) {
                        server_joined = true;
                    }
                    else {
                        server_joined = false;
                    }

                    players.push_back(new Player(window, this, sf::Vector2f(player_info.x, player_info.y), false, 0, player_info.player_ip, false, server_joined));
                    std::cout << "received player info" << std::endl;

                }else if(player_info.type == packet_type::SET_DIFFICULTY_LEVEL){
                    menu::current_difficulty_level = player_info.x;

                    if (menu::current_difficulty_level == 1.f) {
                        current_world->friendly_fire = true;
                    }
                    std::cout << "received difficulty level " << std::to_string(player_info.x) <<std::endl;
                }

                else if (player_info.type == packet_type::START_GAME) {
                    std::cout << "received start command" << std::endl;
                    start_command_arrived = true;
                    return;
                }
            }


            start_command_arrived = true;

        });

        player_info_thread.detach();

        sf::Clock timeout;
        timeout.restart();

        while (!start_command_arrived && timeout.getElapsedTime().asSeconds() < 10.f) {}

        std::cout << "end of client procedure" << std::endl;

    }

    to_playing(window);


    // setup the receiver
    std::thread receiver(&World::receiver_thread, this);
    receiver.detach();

}






// spawn zombies on a random position on the map
void World::spawn_zombies(int number)
{

    for(int i=0;i<number;i++){

        Zombie* zombie = new Zombie(window,this,random_coords(), shadow_direction, (round_counter * menu::current_difficulty_level) + (100));
        zombie->change_state(zombie_state::moving);
        zombie->move_towards(get_random_number(359));
        zombies.push_back(zombie);
        zombies_spawned_number++;

    }

}


// spawn one zombie on location or if random then near location
void World::spawn_zombie(sf::Vector2f location)
{
    Zombie* zombie = new Zombie(window, this, location, shadow_direction, (round_counter * menu::current_difficulty_level) + (100));
    zombie->change_state(zombie_state::moving);
    zombie->move_towards(get_random_number(359));
    zombies.push_back(zombie);
    zombies_spawned_number++;
}






// like garbage collector
void World::remove_dead()
{

    bool dead_exist = true;

    while(dead_exist && !zombies.empty()){

        // delete zombie and call the die function
        for (auto it = zombies.begin(); it != zombies.end(); ++it) {

            dead_exist = true;

            if (!(*it)->alive) {
                delete *it;
                zombies.erase(it);
                break;
            }

            dead_exist = false;
        }

    }

    

    // delete the bullets
    for(auto player : players){

        dead_exist = true;

        while(dead_exist && !player->bullets.empty()){

            for (auto it = player->bullets.begin(); it != player->bullets.end(); ++it) {

                dead_exist = true;

                if (!(*it)->alive) {
                    delete *it;
                    player->bullets.erase(it);
                    break;
                }

                dead_exist = false;

            }

        }

    }

    dead_exist = true;

    while (dead_exist && !players.empty()) {

        // delete player and call the die function
        for (auto it = players.begin(); it != players.end(); ++it) {

            dead_exist = true;

            if (!(*it)->alive) {
                delete* it;
                players.erase(it);
                break;
            }

            dead_exist = false;

        }

    }



}









void World::update_round(){

    if(!round_finished){

        if (coop_mode || server) {

            // spawn zombies
            if (zombie_spawn_clock.getElapsedTime().asSeconds() > ZOMBIE_SPAWN_TIME * (1.f / std::sqrt(round_counter))) {

                if (zombies.size() < MAX_NUMBER_OF_ZOMBIES_ALIVE) {

                    sf::Vector2f random_spawn_seed = players[get_random_number(players.size() - 1)]->sprite.getPosition();
                    sf::Vector2f random_spawn_location = random_coords(random_spawn_seed);
                    send_zombie_information(random_spawn_location);
                    spawn_zombie(random_spawn_location);
                }

                zombie_spawn_clock.restart();

            }
        
        }
        
        else if (spawn_zombie_flag){
 
            spawn_zombie(new_zombie_spawn_position);
            spawn_zombie_flag = false;

        }


        if(zombies_spawned_number >= ZOMBIES_NUMBER_FIRST_ROUND + ZOMBIES_NUMBER_INCREASE_PER_ROUND){

            round_finished = true;
            round_counter++;
            zombies_spawned_number = 0;
        }



    }else{

        enter_new_round();

    }





    for(auto player : players){

        player->round_counter = this->round_counter;

    }

}





void World::enter_new_round()
{
    round_finished = false;

    players[0]->update_status_bar();

}



void World::move_camera()
{
    
    float ratio = window->getSize().y / (float)window->getSize().x;
    float view_width = CAMERA_WIDTH;
    float view_height = ratio * view_width;
    sf::Vector2f window_size(window->getSize());

    if (coop_mode) {

        sf::Vector2f center = players[0]->sprite.getPosition();

        for (auto player : players) {
            center = (center + player->sprite.getPosition()) / 2.f;
        }

        // calculate the new view position, ensuring it remains within window boundaries
        float new_view_x = std::max(view_width / 2.0f, std::min(center.x, this->world_bound_x - view_width / 2.0f));
        float new_view_y = std::max(view_height / 2.0f, std::min(center.y, this->world_bound_y - view_height / 2.0f));

        // optimization
        if (new_view_x - window->getView().getCenter().x == 0.f && new_view_y - window->getView().getCenter().y == 0.f) {
            return;
        }

        sf::View view = window->getView();

        view.setCenter(new_view_x, new_view_y);
        view.setSize(view_width, view_height);

        window->setView(view);


        players[0]->health_bar_back_plate.setPosition((window->getView().getCenter().x) - players[0]->health_bar_back_plate.getGlobalBounds().getSize().x / 2.f, window->getView().getCenter().y + window->getView().getSize().y / 2.f - players[0]->health_bar_back_plate.getGlobalBounds().getSize().y);
        players[0]->health_bar_right.setPosition(players[0]->health_bar_back_plate.getPosition());
        players[0]->health_bar_left.setPosition(players[0]->health_bar_back_plate.getPosition());
        players[0]->health_bar_center_dot.setPosition(players[0]->health_bar_back_plate.getPosition());

        players[0]->update_status_bar();

    }

    else {

        sf::Vector2f center = players[0]->sprite.getPosition();
        

        // calculate the new view position, ensuring it remains within window boundaries
        float new_view_x = std::max(view_width / 2.0f, std::min(center.x, this->world_bound_x - view_width / 2.0f));
        float new_view_y = std::max(view_height / 2.0f, std::min(center.y, this->world_bound_y - view_height / 2.0f));

        // optimization
        if (new_view_x - window->getView().getCenter().x == 0.f && new_view_y - window->getView().getCenter().y == 0.f) {
            return;
        }

        sf::View view = window->getView();

        view.setCenter(new_view_x, new_view_y);
        view.setSize(view_width, view_height);

        window->setView(view);


        players[0]->health_bar_back_plate.setPosition((window->getView().getCenter().x) - players[0]->health_bar_back_plate.getGlobalBounds().getSize().x / 2.f, window->getView().getCenter().y + window->getView().getSize().y / 2.f - players[0]->health_bar_back_plate.getGlobalBounds().getSize().y);
        players[0]->health_bar_right.setPosition(players[0]->health_bar_back_plate.getPosition());
        players[0]->health_bar_left.setPosition(players[0]->health_bar_back_plate.getPosition());
        players[0]->health_bar_center_dot.setPosition(players[0]->health_bar_back_plate.getPosition());

        players[0]->update_status_bar();
    
    }
    
}






extern void to_pause_menu(sf::RenderWindow* window);




void World::pause_music()
{
    wind_sound.pause();
    music_sound.pause();

    for(auto zomb : zombies){
        zomb->zombie_sound.pause();
    }

}




void World::resume_music(){

    if(wind_sound.getStatus() != sf::Sound::Playing){

        wind_sound.play();
        music_sound.play();

        for(auto zomb : zombies){
            zomb->zombie_sound.play();
        }
    }
}






void World::update(sf::Time elapsed_time)
{

    window->clear(background_color);

    move_camera();

    update_round();
        
    zombies_chase_players();

    draw_map(elapsed_time);

    b2_world->Step(1 / FRAME_RATE, 8, 3);

    remove_dead();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        to_pause_menu(window);
    }

}













// return the collidable tiles
struct World::tile_information World::tile_to_rect(int x,int y){

    struct tile_information tile;
    tile.collidable = true;

    if(map_layers[collidable_layer_index]->getTile(x,y).ID == 0){
        tile.collidable = false;
    }

    float left = x  * tile_width;
    float top = y  * tile_height;

    tile.tile_rect = sf::FloatRect(left,top,tile_width,tile_height);
    tile.tile_texture_pos = sf::Vector2u(0,0);

    return tile;
}


















// follow the closest player and attack
void World::zombies_chase_players(){


    if(players.empty())
        return;


    float distance;

    sf::Vector2f player_pos = sf::Vector2f(-1.f,-1.f);

    Player* closest_player = nullptr;


    for(auto zombie : zombies){

        distance = world_bound_x;

        for(auto player : players){

            sf::Vector2f zomb_to_player = player->sprite.getPosition() - zombie->sprite.getPosition();


            // find closest player
            float new_distance = std::sqrt(zomb_to_player.x * zomb_to_player.x + zomb_to_player.y * zomb_to_player.y);

            if(new_distance < distance){

                distance = new_distance;
                player_pos = player->sprite.getPosition();
                closest_player = player;
            }

        }


        sf::Vector2f zomb_to_closest_player = player_pos - zombie->sprite.getPosition();


        // change angle when smaller than hitboxsize
        if(distance >= 1.4f * zombie->hitbox_size.x){

            float closest_player_angle = atan2(zomb_to_closest_player.y, zomb_to_closest_player.x) * (DEG_PER_RAD);

            zombie->move_towards(closest_player_angle);

        }else{

            zombie->direction = sf::Vector2f(0.f,0.f);

        }




        // inside range
        if(distance < 1.4f * zombie->hitbox_size.x){

            zombie->change_state(zombie_state::attack);

            // should take damage now
            if(zombie->animation_frame == 5){

                if(zombie->can_give_damage){

                    closest_player->take_damage();
                    zombie->can_give_damage = false;

                }
            }
        }
    }



}
