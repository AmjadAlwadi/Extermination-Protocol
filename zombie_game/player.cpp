#include "player.h"
#include "bullet.h"
#include "world.h"
#include "menu.h"


#include <iostream>
#include <cmath>
#include <thread>


void Player::setup_values()
{
    // loading pivots manually

    pivots[weapons::flashlight][player_state::idle] = sf::Vector2i(93,105);
    pivots[weapons::flashlight][player_state::melee] = sf::Vector2i(100,129);
    pivots[weapons::flashlight][player_state::moving] = sf::Vector2i(93,112);

    pivots[weapons::handgun][player_state::idle] = sf::Vector2i(96,120);
    pivots[weapons::handgun][player_state::melee] = sf::Vector2i(103,124);
    pivots[weapons::handgun][player_state::moving] = sf::Vector2i(102,119);
    pivots[weapons::handgun][player_state::reload] = sf::Vector2i(100,118);
    pivots[weapons::handgun][player_state::shoot] = sf::Vector2i(100,118);

    pivots[weapons::knife][player_state::idle] = sf::Vector2i(106,113);
    pivots[weapons::knife][player_state::melee] = sf::Vector2i(106,113);
    pivots[weapons::knife][player_state::moving] = sf::Vector2i(106,112);


    pivots[weapons::rifle][player_state::idle] = sf::Vector2i(95,120);
    pivots[weapons::rifle][player_state::melee] = sf::Vector2i(115,200);
    pivots[weapons::rifle][player_state::moving] = sf::Vector2i(95,119);
    pivots[weapons::rifle][player_state::reload] = sf::Vector2i(100,121);
    pivots[weapons::rifle][player_state::shoot] = sf::Vector2i(95,119);

    pivots[weapons::shotgun][player_state::idle] = sf::Vector2i(95,120);
    pivots[weapons::shotgun][player_state::melee] = sf::Vector2i(115,200);
    pivots[weapons::shotgun][player_state::moving] = sf::Vector2i(94,119);
    pivots[weapons::shotgun][player_state::reload] = sf::Vector2i(100,121);
    pivots[weapons::shotgun][player_state::shoot] = sf::Vector2i(94,119);



    feet_texture_frame_limit[feet::idle] = 1;
    feet_texture_frame_limit[feet::run] = 19;
    feet_texture_frame_limit[feet::strafe_left] = 19;
    feet_texture_frame_limit[feet::strafe_right] = 19;

    texture_frame_limit[player_state::idle]  = 19;
    texture_frame_limit[player_state::melee] = 14;
    texture_frame_limit[player_state::moving] = 19;
    texture_frame_limit[player_state::reload] = 14;
    texture_frame_limit[player_state::shoot] = 2;


    feet_texture_frame_size[feet::idle] = sf::Vector2i(132,155);
    feet_texture_frame_size[feet::run]  = sf::Vector2i(204,124);
    feet_texture_frame_size[feet::strafe_left] = sf::Vector2i(155,174);
    feet_texture_frame_size[feet::strafe_right] = sf::Vector2i(154,176);


    texture_frame_size[weapons::flashlight][player_state::idle] = sf::Vector2i(303,223);
    texture_frame_size[weapons::flashlight][player_state::melee] = sf::Vector2i(316,246);
    texture_frame_size[weapons::flashlight][player_state::moving] = sf::Vector2i(305,231);

    texture_frame_size[weapons::handgun][player_state::idle] = sf::Vector2i(253,216);
    texture_frame_size[weapons::handgun][player_state::melee] = sf::Vector2i(291,256);
    texture_frame_size[weapons::handgun][player_state::moving] = sf::Vector2i(258,220);
    texture_frame_size[weapons::handgun][player_state::reload] = sf::Vector2i(260,230);
    texture_frame_size[weapons::handgun][player_state::shoot] = sf::Vector2i(255,215);

    texture_frame_size[weapons::knife][player_state::idle] = sf::Vector2i(289,224);
    texture_frame_size[weapons::knife][player_state::melee] = sf::Vector2i(329,300);
    texture_frame_size[weapons::knife][player_state::moving] = sf::Vector2i(279,219);

    texture_frame_size[weapons::rifle][player_state::idle] = sf::Vector2i(313,207);
    texture_frame_size[weapons::rifle][player_state::melee] = sf::Vector2i(358,353);
    texture_frame_size[weapons::rifle][player_state::moving] = sf::Vector2i(313,206);
    texture_frame_size[weapons::rifle][player_state::reload] = sf::Vector2i(322,217);
    texture_frame_size[weapons::rifle][player_state::shoot] = sf::Vector2i(312,206);

    texture_frame_size[weapons::shotgun][player_state::idle] = sf::Vector2i(313,207);
    texture_frame_size[weapons::shotgun][player_state::melee] = sf::Vector2i(358,353);
    texture_frame_size[weapons::shotgun][player_state::moving] = sf::Vector2i(313,206);
    texture_frame_size[weapons::shotgun][player_state::reload] = sf::Vector2i(322,217);
    texture_frame_size[weapons::shotgun][player_state::shoot] = sf::Vector2i(312,206);

    // setting the firerate
    firerate[weapons::handgun] = 5.f;
    firerate[weapons::shotgun] =  8.f;
    firerate[weapons::rifle] = 1.4f;

    // setting the reload time
    reload_time[weapons::handgun] = 2.f;
    reload_time[weapons::shotgun] =  2.2f;
    reload_time[weapons::rifle] = 2.f;


    // setting the magazine size
    magazine_size[weapons::handgun] = 12.f;
    magazine_size[weapons::shotgun] =  5.f;
    magazine_size[weapons::rifle] = 30.f;



    // setting the sound levels
    sound_levels_shoot[weapons::handgun] = 5.f;
    sound_levels_shoot[weapons::shotgun] = 10.f;
    sound_levels_shoot[weapons::rifle] = 50.f;
    sound_levels_shoot[weapons::knife] = 80.f;

    steps_sound_level = 3.f;

}




sf::Vector2i Player::frame_to_texture_rect(int frame_index){

    return sf::Vector2i((frame_index * texture_frame_size[current_equipped_weapon][current_player_state].x) % texture[current_equipped_weapon][current_player_state]->getSize().x,((frame_index * texture_frame_size[current_equipped_weapon][current_player_state].x)/texture[current_equipped_weapon][current_player_state]->getSize().x) * texture_frame_size[current_equipped_weapon][current_player_state].y);

}

sf::Vector2i Player::frame_to_feet_texture_rect(int frame_index){

    return sf::Vector2i((frame_index * feet_texture_frame_size[current_feet_type].x) % feet_texture[current_feet_type]->getSize().x,((frame_index * feet_texture_frame_size[current_feet_type].x)/feet_texture[current_feet_type]->getSize().x) * feet_texture_frame_size[current_feet_type].y);

}






sf::Vector2i Player::index_to_texture_rect(int frame_index, sf::Vector2i frame_size, sf::Vector2u texture_size){

    return sf::Vector2i((frame_index * frame_size.x) % texture_size.x,((frame_index * frame_size.x)/texture_size.x) * frame_size.y);

}



void Player::setup_status_bar(){

    sf::Vector2i status_bar_frame_size = sf::Vector2i(960,182);
    sf::Vector2f scale = sf::Vector2f(PLAYER_SCALE,PLAYER_SCALE) * 4.f;

    status_bar_texture = new sf::Texture();

    if (!status_bar_texture->loadFromFile(ASSETS_PATH"ui/status bar/spritesheet.png")) {
        std::cerr << "Failed to load spritesheet" << std::endl;
        return;
    }



    sf::Image blank_image;
    blank_image.create(status_bar_frame_size.x/2,95,sf::Color(41,41,41));
    blank_texture.create(status_bar_frame_size.x/2,95);


    sf::Image status_bar_image = status_bar_texture->copyToImage();

    sf::Vector2i dimensions = index_to_texture_rect(1,status_bar_frame_size,status_bar_texture->getSize());


    for(int i = 0; i < status_bar_frame_size.x/2; i++){
        for(int j = 0; j < 95; j++){

            blank_image.setPixel(i,j,sf::Color(41,41,41,status_bar_image.getPixel(i + dimensions.x,j + dimensions.y).a));

        }
    }


    status_bar_texture->setSmooth(true);


    blank_texture.update(blank_image);
    blank_texture.setSmooth(true);


    health_bar_left_blank.setTexture(blank_texture,true);

    health_bar_back_plate.setTexture(*status_bar_texture);
    health_bar_center_dot.setTexture(*status_bar_texture);
    health_bar_left.setTexture(*status_bar_texture);
    health_bar_right.setTexture(*status_bar_texture);

    health_bar_back_plate.setTextureRect(sf::IntRect(index_to_texture_rect(3,status_bar_frame_size,status_bar_texture->getSize()),status_bar_frame_size));
    health_bar_right.setTextureRect(sf::IntRect(index_to_texture_rect(2,status_bar_frame_size,status_bar_texture->getSize()),status_bar_frame_size));
    health_bar_left.setTextureRect(sf::IntRect(index_to_texture_rect(1,status_bar_frame_size,status_bar_texture->getSize()),status_bar_frame_size));
    health_bar_center_dot.setTextureRect(sf::IntRect(index_to_texture_rect(0,status_bar_frame_size,status_bar_texture->getSize()),status_bar_frame_size));

    health_bar_back_plate.setScale(scale);
    health_bar_right.setScale(scale);
    health_bar_left.setScale(scale);
    health_bar_center_dot.setScale(scale);
    health_bar_left_blank.setScale(scale);

    // mirror it
    health_bar_right_blank = health_bar_left_blank;
    health_bar_right_blank.setScale(-scale.x,scale.y);

    health_bar_back_plate.setPosition((window->getView().getCenter().x) - health_bar_back_plate.getGlobalBounds().getSize().x/2.f , window->getView().getCenter().y + window->getView().getSize().y/2.f - health_bar_back_plate.getGlobalBounds().getSize().y);

    health_bar_right.setPosition(health_bar_back_plate.getPosition());
    health_bar_left.setPosition(health_bar_back_plate.getPosition());
    health_bar_center_dot.setPosition(health_bar_back_plate.getPosition());


    update_status_bar();


    health_counter.setFont(menu::font);


    health_counter.setString(std::to_string(this->hp));
    health_counter.setFillColor(sf::Color(160,0,0));
    health_counter.setCharacterSize(30);

    health_counter.setScale(scale);
    health_counter.setOrigin(health_counter.getLocalBounds().getSize().x/2.f,health_counter.getLocalBounds().getSize().y/1.5f);
    health_counter.setPosition(health_bar_center_dot.getPosition() + health_bar_center_dot.getGlobalBounds().getSize()/2.f);
    

    round_bar.setFont(menu::cursed_font);
    round_bar.setOrigin(round_bar.getLocalBounds().getSize().x / 2.f, round_bar.getLocalBounds().getSize().y);
    round_bar.setString(std::to_string(round_counter));
    round_bar.setFillColor(sf::Color(160,0,0));
    round_bar.setCharacterSize(40);
    round_bar.setScale(scale);

    magazine_bar.setFont(menu::font);
    magazine_bar.setOrigin(magazine_bar.getLocalBounds().getSize().x / 2.f, magazine_bar.getLocalBounds().getSize().y);

    if (current_equipped_weapon != weapons::knife) {
        magazine_bar.setString(std::to_string(magazine_load[current_equipped_weapon]) + "/" + std::to_string(magazine_size[current_equipped_weapon]));
    }
    else {
        magazine_bar.setString("");
    }

    magazine_bar.setFillColor(sf::Color(255,255,255));
    magazine_bar.setCharacterSize(50);
    magazine_bar.setScale(scale);


    survived_time.setFont(menu::font);

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << world->never_restarting_clock.getElapsedTime().asSeconds();
    survived_time.setString(stream.str() + " s");
    survived_time.setOrigin(survived_time.getLocalBounds().getSize().x / 2.f, 0);
    survived_time.setFillColor(sf::Color(255,255,255));
    survived_time.setCharacterSize(24);
    survived_time.setScale(scale);


    kills_level_bar.setFont(menu::font);
    kills_level_bar.setOrigin(0, 0);
    kills_level_bar.setString("Kills: " + std::to_string(kills[weapons::flashlight] + kills[weapons::handgun] + kills[weapons::knife] + kills[weapons::rifle] + kills[weapons::shotgun]) + "   Level: " + std::to_string(level[current_equipped_weapon]));
    kills_level_bar.setFillColor(sf::Color(255,255,255));
    kills_level_bar.setCharacterSize(24);
    kills_level_bar.setScale(scale);

}




void Player::draw_status_bar()
{
    window->draw(health_bar_back_plate);
    window->draw(health_bar_left);
    window->draw(health_bar_right);
    window->draw(health_bar_left_blank);
    window->draw(health_bar_right_blank);
    window->draw(health_bar_center_dot);
    window->draw(health_counter);
    window->draw(round_bar);
    window->draw(magazine_bar);
    window->draw(kills_level_bar);
    window->draw(survived_time);
}





// needs to be called when camera moves, hp changes, weapon changes or we enter a new round or for no optimization just every new frame
void Player::update_status_bar(){

    health_bar_left_blank.setPosition(health_bar_left.getPosition() + sf::Vector2f(health_bar_left.getScale().x * 224.f,0));
    health_bar_left_blank.setTextureRect(sf::IntRect(225,0,256.f - (hp * 256.f / 100.f),health_bar_left_blank.getTextureRect().getSize().y));

    health_bar_right_blank = health_bar_left_blank;
    health_bar_right_blank.setScale(-health_bar_left_blank.getScale().x,health_bar_left_blank.getScale().y);
    health_bar_right_blank.setPosition(health_bar_center_dot.getPosition().x + health_bar_center_dot.getGlobalBounds().getSize().x/2.f + health_bar_center_dot.getScale().x * 256.f,health_bar_center_dot.getPosition().y);


    health_counter.setString(std::to_string(this->hp));
    health_counter.setOrigin(health_counter.getLocalBounds().getSize().x/2.f,health_counter.getLocalBounds().getSize().y/1.5f);
    health_counter.setPosition(health_bar_center_dot.getPosition() + health_bar_center_dot.getGlobalBounds().getSize()/2.f);


    kills_level_bar.setString("Kills: " + std::to_string(kills[weapons::flashlight] + kills[weapons::handgun] + kills[weapons::knife] + kills[weapons::rifle] + kills[weapons::shotgun]) + "   Level: " + std::to_string(level[current_equipped_weapon]));
    kills_level_bar.setOrigin(0, 0);

    round_bar.setString(std::to_string(round_counter));
    round_bar.setOrigin(round_bar.getLocalBounds().getSize().x / 2.f, round_bar.getLocalBounds().getSize().y / 1.5f);


    if (current_equipped_weapon != weapons::knife) {
        magazine_bar.setString(std::to_string(magazine_load[current_equipped_weapon]) + "/" + std::to_string(magazine_size[current_equipped_weapon]));
    }
    else {
        magazine_bar.setString("");
    }

    magazine_bar.setOrigin(magazine_bar.getLocalBounds().getSize().x / 2.f, magazine_bar.getLocalBounds().getSize().y);


    round_bar.setPosition(window->getView().getCenter().x - window->getView().getSize().x / 2.f + round_bar.getGlobalBounds().width, window->getView().getCenter().y + window->getView().getSize().y / 2.f - round_bar.getGlobalBounds().height);
    magazine_bar.setPosition(window->getView().getCenter() + window->getView().getSize() / 2.f - sf::Vector2f(magazine_bar.getGlobalBounds().width, 2.2f * magazine_bar.getGlobalBounds().height));
    kills_level_bar.setPosition(window->getView().getCenter().x - window->getView().getSize().x / 2.f + 0.2f * kills_level_bar.getGlobalBounds().width, window->getView().getCenter().y - window->getView().getSize().y / 2.f + 0.2f * kills_level_bar.getGlobalBounds().height);
    survived_time.setPosition(window->getView().getCenter().x + window->getView().getSize().x / 2.f - 1.2f * survived_time.getGlobalBounds().width, window->getView().getCenter().y - window->getView().getSize().y / 2.f);

}



void Player::reload_gun()
{
    reload_sound.play();
}






void Player::restart_level()
{
    // setting the magazine load
    magazine_load[weapons::handgun] = magazine_size[weapons::handgun];
    magazine_load[weapons::shotgun] =  magazine_size[weapons::shotgun];
    magazine_load[weapons::rifle] = magazine_size[weapons::rifle];

    level[weapons::flashlight] = 1;
    level[weapons::handgun] = 1;
    level[weapons::knife] = 1;
    level[weapons::rifle] = 1;
    level[weapons::shotgun] = 1;
}







Player::Player(sf::RenderWindow *window, World *world, sf::Vector2f spawn_location, bool joystick_player, int joystick_id, sf::IpAddress ip, bool local_player, bool server){


    this->type = PLAYER_TYPE;
    this->window = window;
    this->world = world;
    this->ip = ip;
    this->local_player = local_player;
    this->server = server;
    this->joystick_player = joystick_player;
    this->joystick_id = joystick_id;
    this->target_pos = spawn_location;

    current_player_state = player_state::moving;
    current_equipped_weapon = weapons::handgun;
    current_feet_type = feet::run;

    // setup shadow
    shadow_direction = world->shadow_direction;
    delta_shadow_pos = sf::Vector2f(std::sin(shadow_direction * M_PI/180.f),std::cos(shadow_direction * M_PI/180.f)) * SHADOW_DISTANCE_FACTOR;


    if(!textures_loaded){
        load_textures();
        setup_values();
        setup_shadow_sprite();
        load_sound();
        textures_loaded = true;   
    }

    setup_status_bar();

    steps_sound.setBuffer(*steps_sound_buffer);
    steps_sound.setVolume(steps_sound_level);
    steps_sound.setPitch(1.f);

    sprite.setTexture(*texture[current_equipped_weapon][current_player_state],true);
    feet_sprite.setTexture(*feet_texture[current_feet_type],true);

    set_position(spawn_location.x,spawn_location.y);

    sprite.setScale(sf::Vector2f(PLAYER_SCALE,PLAYER_SCALE));
    feet_sprite.setScale(sf::Vector2f(PLAYER_SCALE,PLAYER_SCALE));
    shadow_sprite.setScale(sprite.getScale());


    // setting the origin based on the texture size
    sprite.setOrigin(pivots[current_equipped_weapon][current_player_state].x,pivots[current_equipped_weapon][current_player_state].y);
    shadow_sprite.setOrigin(pivots[current_equipped_weapon][current_player_state].x,pivots[current_equipped_weapon][current_player_state].y);
    feet_sprite.setOrigin(feet_sprite.getLocalBounds().width / 2, feet_sprite.getLocalBounds().height / 2);



    // setup hitbox
    hitbox_size  = sf::Vector2f(132,160);



    // account for scale
    sf::Vector2f scale = sprite.getScale();
    hitbox_size.x *= scale.x;
    hitbox_size.y *= scale.y;





    // setup body in box2d

    // body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(sprite.getPosition().x/PPM, (this->window->getSize().y - sprite.getPosition().y)/PPM);
    boxBodyDef.type = b2_dynamicBody;

    // shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(hitbox_size.x/(2.f*PPM), hitbox_size.y/(2.f*PPM));

    // fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.4f;
    fixtureDef.friction = 0.3f;
    fixtureDef.shape = &boxShape;

    b2FixtureUserData data;
    data.pointer = reinterpret_cast<uintptr_t>(this);
    fixtureDef.userData = data;



    // now we have a body for our Box object
    player_body = world->b2_world->CreateBody(&boxBodyDef);

    // lastly, assign the fixture
    player_body->CreateFixture(&fixtureDef);
        


    // setting the magazine load
    restart_level();

    change_player_state(player_state::idle);
    change_feet_state(feet::idle);
    change_weapon(weapons::knife);

}






Player::~Player()
{

    world->b2_world->DestroyBody(player_body);

}





bool Player::change_player_state(player_state state){


    if(state == player_state::reload && (magazine_load[current_equipped_weapon] == magazine_size[current_equipped_weapon] || current_equipped_weapon == weapons::knife || current_equipped_weapon == weapons::flashlight)){
        return false;
    }



    if(current_player_state != state && !(!animation_finished && current_player_state != player_state::idle && current_player_state != player_state::moving)){

        current_player_state = state;
        animation_frame = 0;
        animation_finished = false;

        sprite.setTexture(*texture[current_equipped_weapon][current_player_state],false);
        sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));

        shadow_sprite.setTexture(*shadow_texture[current_equipped_weapon][current_player_state],false);
        shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));

        // setting the origin based on the texture size
        sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));
        shadow_sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));

        state_has_changed = true;
        return true;
    }

    return false;

}







bool Player::change_feet_state(feet state){


    if(state != feet::idle && (feet_animation_frame == 1 + feet_texture_frame_limit[current_feet_type]/2 || feet_animation_frame == 1) && steps_sound.getStatus() != sf::Sound::Playing){
        steps_sound.play();
    }


    if(current_feet_type != state){

        current_feet_type = state;
        feet_animation_frame = 0;

        feet_sprite.setTexture(*feet_texture[current_feet_type],false);
        feet_sprite.setTextureRect(sf::IntRect(frame_to_feet_texture_rect(feet_animation_frame),feet_texture_frame_size[current_feet_type]));
        feet_sprite.setOrigin(sf::Vector2f(feet_texture_frame_size[current_feet_type]/2));

        state_has_changed = true;
        return true;
    }

    return false;

}







bool Player::change_weapon(weapons weapon){

    if(current_equipped_weapon != weapon && !(!animation_finished && current_player_state != player_state::idle && current_player_state != player_state::moving)){

        current_equipped_weapon = weapon;
        animation_frame = 0;
        animation_finished = false;

        sprite.setTexture(*texture[current_equipped_weapon][current_player_state],false);
        sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));

        shadow_sprite.setTexture(*shadow_texture[current_equipped_weapon][current_player_state],false);
        shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));

        // setting the origin based on the texture size
        sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));
        shadow_sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));

        shoot_sound.setBuffer(*shoot_sound_buffer[current_equipped_weapon]);
        shoot_sound.setVolume(sound_levels_shoot[current_equipped_weapon]);

        reload_sound.setBuffer(*reload_sound_buffer[current_equipped_weapon]);
        reload_sound.setVolume(5);


        state_has_changed = true;
        update_status_bar();
        return true;
    }

    return false;
}





packet Player::pack_to_packet()
{
    return packet(packet_type::PLAYER_DATA,this->ip,sprite.getPosition().x, sprite.getPosition().y, da, hp, current_player_state, current_equipped_weapon, current_feet_type);
}





void Player::apply_data_from_packet(packet& packet)
{

    this->target_pos = sf::Vector2f(packet.x, packet.y);
    this->da = packet.da;
    this->hp = packet.hp;
    change_player_state(packet.current_player_state);
    change_weapon(packet.current_equipped_weapon);
    change_feet_state(packet.current_feet_type);
}



packet Player::pack_bullet_to_packet()
{
    return packet(packet_type::BULLET_DATA, this->ip, sprite.getPosition().x, sprite.getPosition().y, da, level[current_equipped_weapon], current_player_state, current_equipped_weapon, current_feet_type);
}



void Player::apply_bullet_data_from_packet(packet& packet)
{

    this->target_pos = sf::Vector2f(packet.x, packet.y);
    this->da = packet.da;
    change_player_state(packet.current_player_state);
    change_weapon(packet.current_equipped_weapon);
    change_feet_state(packet.current_feet_type);
    level[current_equipped_weapon] = packet.hp;
    shoot_flag = true;

}






void Player::send_information()
{

    // sending information
    if (!world->coop_mode && state_has_changed) {
 
        if (!server) {

            // send data to server only
            packet data_packet = pack_to_packet();
            data_packet.send_packet(world->send_socket,world->server_ip);

        }


        else {

            // send data to everyone except myself
            packet data_packet = pack_to_packet();

            for (auto player : world->players) {

                if (!player->server)
                    data_packet.send_packet(world->send_socket, player->ip);
            }

        }

    }
}




void Player::send_bullet_information()
{

    // sending information
    if (!world->coop_mode) {

        if (!server) {

            // send data to server only
            packet data_packet = pack_bullet_to_packet();
            data_packet.send_packet(world->send_socket, world->server_ip);

        }

        else {

            // send data to everyone except myself
            packet data_packet = pack_bullet_to_packet();

            for (auto player : world->players) {

                if (!player->server)
                    data_packet.send_packet(world->send_socket, player->ip);
            }

        }

    }
}







void Player::update(sf::Time elapsed_time) {

    state_has_changed = false;

    if(local_player){

        direction.x = 0.0f; direction.y = 0.0f;

        if (joystick_player) {
            
            // input events
            if (sf::Joystick::getAxisPosition(joystick_id,sf::Joystick::Axis::PovY) > EPS) {

                direction.y = -SPEED;

                change_player_state(player_state::moving);

            }


            if (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Axis::PovY) < -EPS) {

                direction.y = SPEED;

                change_player_state(player_state::moving);

            }

            if (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Axis::PovX) < -EPS) {

                direction.x = -SPEED;

                change_player_state(player_state::moving);

            }

            if (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Axis::PovX) > EPS) {

                direction.x = SPEED;

                change_player_state(player_state::moving);

            }


            if (sf::Joystick::isButtonPressed(joystick_id,4)) {

                if (joystick_click_clock.getElapsedTime().asSeconds() > 0.5f) {
                    next_weapon();
                    joystick_click_clock.restart();
                }

                

            }

            if (sf::Joystick::isButtonPressed(joystick_id, 6)) {
                if (joystick_click_clock.getElapsedTime().asSeconds() > 0.5f) {
                    previous_weapon();
                    joystick_click_clock.restart();
                }
            }



            if (sf::Joystick::isButtonPressed(joystick_id, 1)) {

                if (!(current_equipped_weapon == weapons::flashlight || current_equipped_weapon == weapons::knife)) {

                    if (change_player_state(player_state::reload))
                        reload_gun();

                }

            }


            if (sf::Joystick::isButtonPressed(joystick_id, 7)) {

                if (current_equipped_weapon == weapons::knife) {
                    change_player_state(player_state::melee);
   
                }

                else if (current_equipped_weapon == weapons::flashlight) {

                    // turn on light or something

                }
                else if (change_player_state(player_state::shoot)) {

                    send_bullet_information();
                    shoot_bullet();

                }

            }


            if (sf::Joystick::isButtonPressed(joystick_id, 3)) {
                if (change_player_state(player_state::melee) && current_equipped_weapon == weapons::knife)
                    shoot_sound.play();

            }



            float x_direction = sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Axis::Z) / 100.f;  // x
            float y_direction = sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Axis::R) / 100.f;  // y


            if (!(x_direction > -EPS && x_direction < EPS && y_direction > -EPS && y_direction < EPS)) {
                da = atan2(y_direction, x_direction) * (180 / M_PI) + CORRECTION_ANGLE_FOR_BULLET; // Convert radians to degrees
            }



            // set idle
            if (direction.x == 0.0f && direction.y == 0.0f) {

                change_player_state(player_state::idle);
                change_feet_state(feet::idle);

            }

            else {

                // apply proper feet animation (strafing sidly)
                if (direction.x == SPEED && ((da < 90 + DELTA_STRAFING_ANGLE && da > 90 - DELTA_STRAFING_ANGLE) || (da > -90 - DELTA_STRAFING_ANGLE && da < -90 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_right);

                }
                else if (direction.x == -SPEED && ((da < 90 + DELTA_STRAFING_ANGLE && da > 90 - DELTA_STRAFING_ANGLE) || (da > -90 - DELTA_STRAFING_ANGLE && da < -90 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_left);

                }
                else if (direction.y == SPEED && ((da < DELTA_STRAFING_ANGLE && da > -DELTA_STRAFING_ANGLE) || (da > 180 - DELTA_STRAFING_ANGLE || da < -180 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_right);

                }
                else if (direction.y == -SPEED && ((da < DELTA_STRAFING_ANGLE && da > -DELTA_STRAFING_ANGLE) || (da > 180 - DELTA_STRAFING_ANGLE || da < -180 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_left);

                }
                else {
                    change_feet_state(feet::run);

                }

            }


            // Apply the diagonal factor if moving diagonally
            if (direction.x != 0 && direction.y != 0) {

                direction.x *= DIAGONALFACTOR;
                direction.y *= DIAGONALFACTOR;

            }
        
        
        
        }else {
            
            // input events
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

                direction.y = -SPEED;

                change_player_state(player_state::moving);

            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

                direction.y = SPEED;

                change_player_state(player_state::moving);

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

                direction.x = -SPEED;

                change_player_state(player_state::moving);

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

                direction.x = SPEED;

                change_player_state(player_state::moving);

            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {

                change_weapon(weapons::shotgun);


            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {

                change_weapon(weapons::rifle);


            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {

                change_weapon(weapons::handgun);


            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {

                change_weapon(weapons::knife);


            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {

                change_weapon(weapons::flashlight);

            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

                if (!(current_equipped_weapon == weapons::flashlight || current_equipped_weapon == weapons::knife)) {

                    if (change_player_state(player_state::reload))
                        reload_gun();

                }

            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                if (current_equipped_weapon == weapons::knife) {
                    change_player_state(player_state::melee);
                }

                else if (current_equipped_weapon == weapons::flashlight) {

                    // turn on light or something

                }

                else if (change_player_state(player_state::shoot)) {
                    
                    send_bullet_information();
                    shoot_bullet();  
                        
                }

            }


            if ((sf::Mouse::isButtonPressed(sf::Mouse::XButton1) || sf::Keyboard::isKeyPressed(sf::Keyboard::V))) {
                if (change_player_state(player_state::melee) && current_equipped_weapon == weapons::knife) {
                    shoot_bullet();
                    send_bullet_information();
                }
                    
            }



            sf::Vector2i mouse_position = sf::Mouse::getPosition(*window);

            sf::Vector2f world_mouse_position = window->mapPixelToCoords(mouse_position); // Convert to world coordinates

            sf::Vector2f player_center = sprite.getPosition();
            float delta_x = world_mouse_position.x - player_center.x;
            float delta_y = world_mouse_position.y - player_center.y;

            da = atan2(delta_y, delta_x) * (180 / M_PI) + CORRECTION_ANGLE_FOR_BULLET; // Convert radians to degrees


            if (da != sprite.getRotation()) {
                state_has_changed = true;
            }

            // set idle
            if (direction.x == 0.0f && direction.y == 0.0f) {

                change_player_state(player_state::idle);
                change_feet_state(feet::idle);

            }else {

                state_has_changed = true;

                // apply proper feet animation (strafing sidly)
                if (direction.x == SPEED && ((da < 90 + DELTA_STRAFING_ANGLE && da > 90 - DELTA_STRAFING_ANGLE) || (da > -90 - DELTA_STRAFING_ANGLE && da < -90 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_right);

                }
                else if (direction.x == -SPEED && ((da < 90 + DELTA_STRAFING_ANGLE && da > 90 - DELTA_STRAFING_ANGLE) || (da > -90 - DELTA_STRAFING_ANGLE && da < -90 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_left);

                }
                else if (direction.y == SPEED && ((da < DELTA_STRAFING_ANGLE && da > -DELTA_STRAFING_ANGLE) || (da > 180 - DELTA_STRAFING_ANGLE || da < -180 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_right);

                }
                else if (direction.y == -SPEED && ((da < DELTA_STRAFING_ANGLE && da > -DELTA_STRAFING_ANGLE) || (da > 180 - DELTA_STRAFING_ANGLE || da < -180 + DELTA_STRAFING_ANGLE))) {
                    change_feet_state(feet::strafe_left);

                }
                else {
                    change_feet_state(feet::run);

                }

            }




            // Apply the diagonal factor if moving diagonally
            if (direction.x != 0 && direction.y != 0) {

                direction.x *= DIAGONALFACTOR;
                direction.y *= DIAGONALFACTOR;

            }

        
        }



        // update survived time
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << world->never_restarting_clock.getElapsedTime().asSeconds();
        survived_time.setString(stream.str() + " s");
        survived_time.setOrigin(survived_time.getLocalBounds().getSize().x / 2.f, 0);



        if (send_clock.getElapsedTime().asSeconds() >  ONLINE_SEND_RATE) {
            send_information();
            send_clock.restart();
        }


    }


    // because box2d doesn't handle threads
    if (shoot_flag) {
        shoot_bullet();
        shoot_flag = false;
    }

 
    animate(elapsed_time);

}




void Player::level_up(){

    this->hp = 100;
    update_status_bar();

    magazine_size[weapons::flashlight] += 1;
    magazine_size[weapons::handgun] += 4; 
    //magazine_size[weapons::knife] +=  1; 
    magazine_size[weapons::rifle] += 5; 
    magazine_size[weapons::shotgun] += 1;

}




// when number of kills change
void Player::change_level(int pervious_level){

    // calculate level as a root function
    level[current_equipped_weapon] = (int) std::max(1.f,std::min(std::pow( (float) kills[current_equipped_weapon] - KILLS_FOR_FIRST_LEVEL - 1,1/(DIFFICULTY_LEVEL + menu::current_difficulty_level)),7.f));

    if(pervious_level != level[current_equipped_weapon]){
        level_up();
    }

}




void Player::next_weapon()
{

    int weap = static_cast<int>(current_equipped_weapon);

    if (weap == 3) {
        change_weapon(static_cast<weapons>(0));
    }
    else {
        change_weapon(static_cast<weapons>(weap + 1));
    }

    
}




void Player::previous_weapon()
{
    int weap = static_cast<int>(current_equipped_weapon);

    if (weap == 0) {
        change_weapon(static_cast<weapons>(3));
    }
    else {
        change_weapon(static_cast<weapons>(weap - 1));
    }

}





void Player::animate(sf::Time elapsed_time)
{

    float frame_time = FRAME_TIME;

    if(current_player_state == player_state::shoot){
        frame_time = firerate[current_equipped_weapon] * FRAME_TIME;
    }

    if(current_player_state == player_state::reload){
        frame_time = reload_time[current_equipped_weapon] * FRAME_TIME;
    }

    // animate player
    if (animation_clock.getElapsedTime().asSeconds() > frame_time){

        // reload the gun
        if(current_player_state == player_state::reload && animation_frame == texture_frame_limit[current_player_state]){
            magazine_load[current_equipped_weapon] = magazine_size[current_equipped_weapon];
        }


        if (current_equipped_weapon == weapons::knife && current_player_state == player_state::melee && animation_frame == 7){
            send_bullet_information();
            shoot_bullet();
        }

        if(animation_frame == texture_frame_limit[current_player_state]){

            animation_finished = true;
            animation_frame = 0;
        }


        if(magazine_load[current_equipped_weapon] == 0 && current_player_state == player_state::shoot && animation_finished){

            if(change_player_state(player_state::reload))
                reload_gun();
        }


        sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));
        shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_equipped_weapon][current_player_state]));

        // setting the origin based on the texture size
        sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));
        shadow_sprite.setOrigin(sf::Vector2f(pivots[current_equipped_weapon][current_player_state]));


        animation_frame++;
        animation_clock.restart();

    }



    // animate feet
    if (feet_animation_clock.getElapsedTime().asSeconds() > 1.3f*FRAME_TIME){


        if(feet_animation_frame >= feet_texture_frame_limit[current_feet_type]){

            feet_animation_frame = 0;

        }

        // setting the origin based on the texture size
        feet_sprite.setTextureRect(sf::IntRect(frame_to_feet_texture_rect(feet_animation_frame),feet_texture_frame_size[current_feet_type]));
        feet_sprite.setOrigin(sf::Vector2f(feet_texture_frame_size[current_feet_type]/2));


        feet_animation_frame++;
        feet_animation_clock.restart();

    }


    

    // physics
    rotate_player();
    move(elapsed_time);


    // draw
    draw();

}







void Player::draw()
{
    window->draw(feet_sprite);
    window->draw(sprite);
}




void Player::draw_shadow(){

    window->draw(shadow_sprite);
}



void Player::draw_vfx()
{
    for(auto effect : vfx_effects){

        effect->animate();

    }
}








void Player::move(sf::Time elapsed_time){

    float new_direction_x = 0.f;
    float new_direction_y = 0.f;

    if(current_equipped_weapon == weapons::knife && (direction.x != 0.f || direction.y != 0.f))
    {
        direction.x *= 1.14f;
        direction.y *= 1.14f;

    }



    if (local_player) {
    
        // make speed independent from frame rate
        new_direction_x = 100 * elapsed_time.asSeconds() * direction.x;
        new_direction_y = 100 * elapsed_time.asSeconds() * direction.y;


        // check for map boundary
        if(sprite.getPosition().x - 2.f * hitbox_size.x <= 0)
            new_direction_x = new_direction_x > 0 ? new_direction_x : 0;

        else if(sprite.getPosition().x + 2.f * hitbox_size.x >= world->world_bound_x)
            new_direction_x = new_direction_x < 0 ? new_direction_x : 0;

        if(sprite.getPosition().y - 2.f * hitbox_size.y <= 0)
            new_direction_y = new_direction_y > 0 ? new_direction_y : 0;

        else if(sprite.getPosition().y + 2.f * hitbox_size.y >= world->world_bound_y)
            new_direction_y = new_direction_y < 0 ? new_direction_y : 0;



        // check for camera view boundary if coop
        if (sprite.getPosition().x + 2.f * hitbox_size.x > window->getView().getCenter().x + window->getView().getSize().x / 2.f) 
            new_direction_x = new_direction_x < 0 ? new_direction_x : 0;

        if (sprite.getPosition().x - 2.f * hitbox_size.x < window->getView().getCenter().x - window->getView().getSize().x / 2.f)
            new_direction_x = new_direction_x > 0 ? new_direction_x : 0;

        if (sprite.getPosition().y - 2.f * hitbox_size.y < window->getView().getCenter().y - window->getView().getSize().y / 2.f)
            new_direction_y = new_direction_y > 0 ? new_direction_y : 0;

        if (sprite.getPosition().y + 2.f * hitbox_size.y > window->getView().getCenter().y + window->getView().getSize().y / 2.f)
            new_direction_y = new_direction_y < 0 ? new_direction_y : 0;

    }else {
        
        new_direction_x = target_pos.x - sprite.getPosition().x;
        new_direction_y = target_pos.y - sprite.getPosition().y;

    }



    // add dirt animation
    if(new_direction_x != 0.f || new_direction_y != 0.f){

        float y = std::sin((da + 90) * M_PI/180.f) * 2.5f;
        float x = std::cos((da + 90) * M_PI/180.f) * 2.5f;

        sf::Vector2f dirt_position = this->sprite.getPosition() + ( ( (float) World::get_random_number(200)/100.f * sf::Vector2f(x,y) ) -  sf::Vector2f(x,y) );


        // add vfx
        vfx_effects.push_back(new vfx(window,vfx::vfx_type::DIRT,1.f,true, dirt_position, 1.2f * this->sprite.getScale()));
    }



    // calculate position
    player_body->SetLinearVelocity(b2Vec2(new_direction_x,-new_direction_y));

    b2Vec2 new_pos = player_body->GetPosition();

    set_position(new_pos.x * PPM,this->window->getSize().y - (new_pos.y * PPM));

}






void Player::set_position(float x, float y)
{
    this->sprite.setPosition(x,y);
    this->feet_sprite.setPosition(x,y);
    this->shadow_sprite.setPosition(sf::Vector2f(x,y) + delta_shadow_pos);
}







// rotating the sprite towards mouse
void Player::rotate_player(){

    float old_angle = sprite.getRotation();

    player_body->SetAngularVelocity(-(old_angle - da) * DEG_PER_RAD_INVERSE);

    shadow_sprite.setRotation(da);
    sprite.setRotation(da);
    feet_sprite.setRotation(da);

}

















void Player::shoot_bullet()
{

    // reload if gun is empty
    if(magazine_load[current_equipped_weapon] == 0 && current_equipped_weapon != weapons::knife){
        if(change_player_state(player_state::reload))
            reload_gun();
        return;
    }


    if(current_equipped_weapon != weapons::shotgun){

        bullet* shot = new bullet(window,world,sprite.getPosition(),da,current_equipped_weapon,level[current_equipped_weapon],this);
        this->bullets.push_back(shot);

    }

    else {

        float correction_vector_length = 2.f;

        float corrected_angle = da + (atan2(100, 20) * (180 / M_PI));

        float y = std::sin(corrected_angle * M_PI/180.f) * correction_vector_length;
        float x = std::cos(corrected_angle * M_PI/180.f) * correction_vector_length;


        float corrected_angle1 = da + (atan2(-100, 20) * (180 / M_PI));

        float y1 = std::sin(corrected_angle1 * M_PI/180.f) * correction_vector_length;
        float x1 = std::cos(corrected_angle1 * M_PI/180.f) * correction_vector_length;


        bullet* shot1 = new bullet(window,world,sprite.getPosition(),da,current_equipped_weapon,level[current_equipped_weapon],this);
        bullet* shot2 = new bullet(window,world,sprite.getPosition() + sf::Vector2f(x,y),da + 30,current_equipped_weapon,level[current_equipped_weapon],this);
        bullet* shot3 = new bullet(window,world,sprite.getPosition() + sf::Vector2f(x1,y1),da - 30,current_equipped_weapon,level[current_equipped_weapon],this);

        this->bullets.push_back(shot1);
        this->bullets.push_back(shot2);
        this->bullets.push_back(shot3);

    }

    shoot_sound.play();


    magazine_load[current_equipped_weapon]--;
}







void Player::take_damage()
{

    this->hp -= PLAYER_DAMAGE_PER_HIT;

    state_has_changed = true;

    if(this == world->players[0])
        update_status_bar();

    vfx_effects.push_back(new vfx(window,vfx::vfx_type::BLOOD,0.4f,true,this->sprite.getPosition(),5.f * this->sprite.getScale()));
    vfx_effects.push_back(new vfx(window,vfx::vfx_type::BLOOD_STATIC,2.f,true,this->sprite.getPosition(),5.f * this->sprite.getScale()));

    if(hp == 100 - 2 * ZOMBIE_DAMAGE_PER_HIT)
        vfx_effects.push_back(new vfx(window,vfx::vfx_type::GUTS,1.f,true,this->sprite.getPosition(),0.7f * this->sprite.getScale()));

    if(hp <= 0.f){

        die();

    }

}





void Player::take_damage(bullet* bullet)
{
    take_damage();
}




extern void to_die_screen_menu(sf::RenderWindow* window);




void Player::die()
{
    this->alive = false;

    world->remove_dead();

    if (world->players.size() == 0) {
        to_die_screen_menu(window);
    }

}





void Player::setup_shadow_sprite()
{
    for (int weapon_index= static_cast<int>(weapons::handgun); weapon_index <= static_cast<int>(weapons::flashlight); weapon_index++) {

        weapons weapon = static_cast<weapons>(weapon_index);

        for (int state_index = static_cast<int>(player_state::idle); state_index <= static_cast<int>(player_state::moving); state_index++) {

            player_state state = static_cast<player_state>(state_index);

            if(texture[weapon][state] == nullptr)
                continue;

            shadow_texture[weapon][state] = new sf::Texture();

            shadow_texture[weapon][state]->create(texture[weapon][state]->getSize().x, texture[weapon][state]->getSize().y);

            sf::Image shadow_image = texture[weapon][state]->copyToImage();


            for(int i = 0; i < texture[weapon][state]->getSize().x;i++){

                for(int j = 0; j <texture[weapon][state]->getSize().y;j++){

                    shadow_image.setPixel(i,j,sf::Color(0,0,0,shadow_image.getPixel(i,j).a * SHADOW_STRENGTH));

                }

            }


            // Update the shadow texture with the modified image
            shadow_texture[weapon][state]->update(shadow_image);
        }
    }

    shadow_sprite.setTexture(*shadow_texture[current_equipped_weapon][current_player_state],false);

}













void Player::load_textures()
{


    std::string directory_path = ASSETS_PATH"characters/player spritesheets/";

    // load feet

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "feet/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        feet_texture[feet::idle] = current_texture;

    }



    // run
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "feet/run.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        feet_texture[feet::run] = current_texture;

    }


    // strafe left
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "feet/strafe_left.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        feet_texture[feet::strafe_left] = current_texture;

    }

    // strafe right
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "feet/strafe_right.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        feet_texture[feet::strafe_right] = current_texture;

    }




    // load feet









    // load body



    // handgun


    // move
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "handgun/move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::handgun][player_state::moving] = current_texture;

    }


    // reload
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "handgun/reload.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::handgun][player_state::reload] = current_texture;

    }

    // shoot
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "handgun/shoot.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::handgun][player_state::shoot] = current_texture;

    }

    // melee
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "handgun/melee.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::handgun][player_state::melee] = current_texture;

    }

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "handgun/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::handgun][player_state::idle] = current_texture;

    }



    // handgun








    // rifle




    // move
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "rifle/move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::rifle][player_state::moving] = current_texture;

    }

    // reload
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "rifle/reload.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::rifle][player_state::reload] = current_texture;

    }

    // shoot
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "rifle/shoot.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::rifle][player_state::shoot] = current_texture;

    }

    // melee
        {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "rifle/melee.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::rifle][player_state::melee] = current_texture;

    }

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "rifle/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::rifle][player_state::idle] = current_texture;

    }



    // rilfe






    // shotgun




    // move
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "shotgun/move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::shotgun][player_state::moving] = current_texture;

    }

    // reload
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "shotgun/reload.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return; // Skip this iteration if the texture failed to load
        }

        current_texture->setSmooth(true);

        texture[weapons::shotgun][player_state::reload] = current_texture;

    }

    // shoot
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "shotgun/shoot.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::shotgun][player_state::shoot] = current_texture;

    }

    // melee
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "shotgun/melee.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::shotgun][player_state::melee] = current_texture;

    }

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "shotgun/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::shotgun][player_state::idle] = current_texture;

    }



    // shotgun








    // knife




    // move
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "knife/move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::knife][player_state::moving] = current_texture;

    }




    // melee
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "knife/melee.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::knife][player_state::melee] = current_texture;

    }

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "knife/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::knife][player_state::idle] = current_texture;

    }



    // knife







    // flashlight




    // move
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "flashlight/move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::flashlight][player_state::moving] = current_texture;

    }




    // melee
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "flashlight/melee.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;
        }

        current_texture->setSmooth(true);

        texture[weapons::flashlight][player_state::melee] = current_texture;

    }

    // idle
    {
        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "flashlight/idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return; // Skip this iteration if the texture failed to load
        }

        current_texture->setSmooth(true);

        texture[weapons::flashlight][player_state::idle] = current_texture;

    }



    // flashlight


    // load body


}




void Player::load_sound()
{


    steps_sound_buffer = new sf::SoundBuffer();

    shoot_sound_buffer[weapons::flashlight] = new sf::SoundBuffer();
    shoot_sound_buffer[weapons::knife] = new sf::SoundBuffer();
    shoot_sound_buffer[weapons::rifle] = new sf::SoundBuffer();
    shoot_sound_buffer[weapons::shotgun] = new sf::SoundBuffer();
    shoot_sound_buffer[weapons::handgun] = new sf::SoundBuffer();

    reload_sound_buffer[weapons::flashlight] = new sf::SoundBuffer();
    reload_sound_buffer[weapons::knife] = new sf::SoundBuffer();
    reload_sound_buffer[weapons::rifle] = new sf::SoundBuffer();
    reload_sound_buffer[weapons::shotgun] = new sf::SoundBuffer();
    reload_sound_buffer[weapons::handgun] = new sf::SoundBuffer();



    std::string directory_path = ASSETS_PATH"sounds/";

    std::string filePath = directory_path + "step.wav";
    if (!steps_sound_buffer->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }


    filePath = directory_path + "handgun.wav";
    if (!shoot_sound_buffer[weapons::handgun]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

    filePath = directory_path + "shotgun.wav";
    if (!shoot_sound_buffer[weapons::shotgun]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

    filePath = directory_path + "rifle.flac";
    if (!shoot_sound_buffer[weapons::rifle]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }


    filePath = directory_path + "rifle-reload.flac";
    if (!reload_sound_buffer[weapons::rifle]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }



    filePath = directory_path + "shotgun-reload.flac";
    if (!reload_sound_buffer[weapons::shotgun]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }



    filePath = directory_path + "handgun-reload.flac";
    if (!reload_sound_buffer[weapons::handgun]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }


    filePath = directory_path + "knife.flac";
    if (!shoot_sound_buffer[weapons::knife]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

}


















