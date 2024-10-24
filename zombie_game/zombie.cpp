#include <iostream>
#include <cmath>

#include "zombie.h"
#include "world.h"
#include "bullet.h"

Zombie::Zombie(sf::RenderWindow *window,World* world, sf::Vector2f position, float shadow_direction,int starting_hp) {

    this->type = ZOMBIE_TYPE;
    this->window = window;
    this->world = world;
    this->shadow_direction = shadow_direction;
    this->hp = starting_hp;

    delta_shadow_pos = sf::Vector2f(std::sin(shadow_direction * M_PI/180.f),std::cos(shadow_direction * M_PI/180.f)) * SHADOW_DISTANCE_FACTOR;
    current_zombie_state = zombie_state::idle;



    if(!textures_loaded){
        load_textures();
        setup_values();
        setup_shadow_sprite();
        load_sounds();
        textures_loaded = true;
    }

    sprite.setScale(sf::Vector2f(ZOMBIE_SCALE,ZOMBIE_SCALE));
    shadow_sprite.setScale(sprite.getScale());

    // setup hitbox
    hitbox_size  = sf::Vector2f(200,200);

    // account for scale
    sf::Vector2f scale = sprite.getScale();
    hitbox_size.x *= scale.x;
    hitbox_size.y *= scale.y;


    sprite.setTexture(*texture[current_zombie_state]);
    shadow_sprite.setTexture(*shadow_texture[current_zombie_state]);

    sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));
    shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));

    sprite.setOrigin(sf::Vector2f(pivots[current_zombie_state]));
    shadow_sprite.setOrigin(sf::Vector2f(pivots[current_zombie_state]));


    sprite.setPosition(position);



    // setup zombie sounds
    zombie_sound.setBuffer(*zombie_sound_buffer[current_zombie_state]);
    zombie_sound.setLoop(sound_loop_enabled[current_zombie_state]);
    zombie_sound.setVolume(sound_levels[current_zombie_state]);
    zombie_sound.setAttenuation(3);
    zombie_sound.setMinDistance(1.1f * hitbox_size.x);


    // setup sound position
    sf::Vector3f sound_pos = sf::Vector3f(sprite.getPosition().x - world->players[0]->sprite.getPosition().x, 0, sprite.getPosition().y - world->players[0]->sprite.getPosition().y);
    zombie_sound.setPosition(sound_pos);


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
    zombie_body = world->b2_world->CreateBody(&boxBodyDef);

    // lastly, we assign the fixture
    zombie_body->CreateFixture(&fixtureDef);

}




sf::Vector2i Zombie::frame_to_texture_rect(int frame_index){

    return sf::Vector2i((frame_index * texture_frame_size[current_zombie_state].x) % texture[current_zombie_state]->getSize().x,((frame_index * texture_frame_size[current_zombie_state].x)/texture[current_zombie_state]->getSize().x) * texture_frame_size[current_zombie_state].y);

}





Zombie::~Zombie()
{
    world->b2_world->DestroyBody(zombie_body);
    remove_faded_effects();
}




void Zombie::setup_values()
{
    pivots[zombie_state::attack] = sf::Vector2i(145 * sprite.getScale().x,155 * sprite.getScale().y);
    pivots[zombie_state::idle] = sf::Vector2i(108 * sprite.getScale().x,118 * sprite.getScale().y);
    pivots[zombie_state::moving] = sf::Vector2i(123 * sprite.getScale().x,173 * sprite.getScale().y);

    texture_frame_limit[zombie_state::idle] = 17;
    texture_frame_limit[zombie_state::moving] = 17;
    texture_frame_limit[zombie_state::attack] = 9;

    texture_frame_size[zombie_state::attack] = sf::Vector2i(318,294);
    texture_frame_size[zombie_state::idle] = sf::Vector2i(241,222);
    texture_frame_size[zombie_state::moving] = sf::Vector2i(288,311);


    // initialize textures on the heap
    shadow_texture.insert(std::make_pair(zombie_state::idle,new sf::Texture()));
    shadow_texture.insert(std::make_pair(zombie_state::attack,new sf::Texture()));
    shadow_texture.insert(std::make_pair(zombie_state::moving,new sf::Texture()));

    sound_levels[zombie_state::idle] = 20;
    sound_levels[zombie_state::moving] = 40;
    sound_levels[zombie_state::attack] = 100;

    sound_loop_enabled[zombie_state::idle] = true;
    sound_loop_enabled[zombie_state::moving] = true;
    sound_loop_enabled[zombie_state::attack] = false;



}







void Zombie::update(sf::Time elapsed_time) {

    remove_faded_effects();
    animate(elapsed_time);

}









void Zombie::animate(sf::Time elapsed_time)
{


    // animate
    if (animation_clock.getElapsedTime().asSeconds() > FRAME_TIME){


        if(animation_frame != 5)
            can_give_damage = true;


        // undo attack animation when done
        if(current_zombie_state == zombie_state::attack && animation_frame == texture_frame_limit[current_zombie_state])
            change_state(zombie_state::moving);



        if(animation_frame >= texture_frame_limit[current_zombie_state])
            animation_frame = 0;




        sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));
        shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));

        // setting the origin based on the texture size
        sprite.setOrigin(sf::Vector2f(pivots[current_zombie_state]));
        shadow_sprite.setOrigin(sf::Vector2f(pivots[current_zombie_state]));


        animation_frame++;
        animation_clock.restart();

    }

    move(elapsed_time);

    draw();

}







void Zombie::change_state(zombie_state new_state){

    if(new_state == current_zombie_state)
        return;

    current_zombie_state = new_state;
    animation_frame = 0;

    sprite.setTexture(*texture[current_zombie_state],false);
    sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));

    shadow_sprite.setTexture(*shadow_texture[current_zombie_state],false);
    shadow_sprite.setTextureRect(sf::IntRect(frame_to_texture_rect(animation_frame),texture_frame_size[current_zombie_state]));



    if(!(zombie_sound.getStatus() == sf::Sound::Playing && !zombie_sound.getLoop())){

        zombie_sound.setBuffer(*zombie_sound_buffer[current_zombie_state]);
        zombie_sound.setLoop(sound_loop_enabled[current_zombie_state]);
        zombie_sound.setVolume(sound_levels[current_zombie_state]);

        zombie_sound.play();
    }

}









void Zombie::draw()
{

    window->draw(sprite);
}



void Zombie::draw_shadow()
{
    window->draw(shadow_sprite);
}



void Zombie::draw_vfx()
{
    for(auto effect : vfx_effects){

        effect->animate();

    }
}









void Zombie::move(sf::Time elapsed_time){


    zombie_body->SetLinearVelocity(100 * elapsed_time.asSeconds() * b2Vec2(direction.x,-direction.y));


    // add dirt animation
    if(direction != sf::Vector2f(0.f,0.f)){

        float y = std::sin((da + 90) * M_PI/180.f) * 2.5f;
        float x = std::cos((da + 90) * M_PI/180.f) * 2.5f;

        sf::Vector2f dirt_position = this->sprite.getPosition() + ( ( (float) World::get_random_number(200)/100.f * sf::Vector2f(x,y) ) -  sf::Vector2f(x,y) );


        // add vfx
        vfx_effects.push_back(new vfx(window,vfx::vfx_type::DIRT,1.f,true, dirt_position, 1.2f * this->sprite.getScale()));
    }



    b2Vec2 new_pos = zombie_body->GetPosition();
    sf::Vector2f sf_new_pos = sf::Vector2f(new_pos.x * PPM ,this->window->getSize().y - (new_pos.y * PPM));

    sprite.setPosition(sf_new_pos);
    shadow_sprite.setPosition(sf_new_pos + delta_shadow_pos);

    sf::Vector3f sound_pos = sf::Vector3f(sf_new_pos.x - world->players[0]->sprite.getPosition().x,0, sf_new_pos.y - world->players[0]->sprite.getPosition().y);
    zombie_sound.setPosition(sound_pos);

}







// rotate the sprite
void Zombie::rotate_zombie(){

    float old_angle = sprite.getRotation();

    zombie_body->SetAngularVelocity(-(old_angle - da) * DEG_PER_RAD_INVERSE);

    shadow_sprite.setRotation(da);
    sprite.setRotation(da);

}








// rotate and move towards angle
void Zombie::move_towards(float angle)
{
    if(current_zombie_state == zombie_state::idle)
        return;


    da = angle;
    rotate_zombie();

    direction.y = std::sin(da * DEG_PER_RAD_INVERSE) * ZOMBIE_SPEED;
    direction.x = std::cos(da * DEG_PER_RAD_INVERSE) * ZOMBIE_SPEED;


}





void Zombie::remove_faded_effects()
{

    bool faded_exist = true;

    while(faded_exist && !vfx_effects.empty()){

        // delete zombie and call the die function
        for (auto it = vfx_effects.begin(); it != vfx_effects.end(); ++it) {

            faded_exist = true;

            if (!(*it)->alive) {
                delete *it;
                vfx_effects.erase(it);
                break;
            }

            faded_exist = false;
        }

    }

}




void Zombie::take_damage(bullet* bullet)
{

    hp -= ZOMBIE_DAMAGE_PER_HIT * (bullet->level);

    vfx_effects.push_back(new vfx(window,vfx::vfx_type::BLOOD_STATIC,2.f,true,this->sprite.getPosition(),5.f * this->sprite.getScale()));

    if(hp == 100 - 2 * ZOMBIE_DAMAGE_PER_HIT)
        vfx_effects.push_back(new vfx(window,vfx::vfx_type::GUTS,1.f,true,this->sprite.getPosition(),0.7f * this->sprite.getScale()));

    if(hp <= 0.0f){

        die();
        bullet->owner->kills[bullet->weapon]++;
        bullet->owner->change_level(bullet->level);

    }

}








void Zombie::die()
{

    alive = false;

}






void Zombie::setup_shadow_sprite()
{

    for (int state_index = static_cast<int>(zombie_state::idle); state_index <= static_cast<int>(zombie_state::attack); state_index++) {

        zombie_state state = static_cast<zombie_state>(state_index);

        shadow_texture[state]->create(texture[state]->getSize().x,texture[state]->getSize().y);

        sf::Image shadow_image = texture[state]->copyToImage();

        for(int i = 0; i < texture[state]->getSize().x;i++){

            for(int j = 0; j < texture[state]->getSize().y;j++){

                shadow_image.setPixel(i,j,sf::Color(0,0,0,shadow_image.getPixel(i,j).a * SHADOW_STRENGTH));

            }

        }

        shadow_texture[state]->update(shadow_image);
        shadow_texture[state]->setSmooth(true);

    }


}












void Zombie::load_textures()
{


    std::string directory_path = ASSETS_PATH"characters/zombie spritesheets/";

    // idle
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "idle.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;  // return if the texture failed to load
        }

        current_texture->setSmooth(true);
        texture[zombie_state::idle] = current_texture;

    }


    // attack
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "attack.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return;  // return if the texture failed to load
        }

        current_texture->setSmooth(true);
        texture[zombie_state::attack] = current_texture;

    }



    // moving
    {

        sf::Texture* current_texture = new sf::Texture();

        std::string filePath = directory_path + "move.png";

        if (!current_texture->loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return; // return if the texture failed to load
        }

        current_texture->setSmooth(true);
        texture[zombie_state::moving] = current_texture;

    }

}











void Zombie::load_sounds()
{

    zombie_sound_buffer[zombie_state::idle] = new sf::SoundBuffer();
    zombie_sound_buffer[zombie_state::attack] = new sf::SoundBuffer();
    zombie_sound_buffer[zombie_state::moving] = new sf::SoundBuffer();


    std::string directory_path = ASSETS_PATH"sounds/";

    std::string filePath = directory_path + "zombie-idle.flac";
    if (!zombie_sound_buffer[zombie_state::idle]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }


    filePath = directory_path + "zombie-attack.flac";
    if (!zombie_sound_buffer[zombie_state::attack]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

    filePath = directory_path + "zombie-move.flac";
    if (!zombie_sound_buffer[zombie_state::moving]->loadFromFile(filePath)){
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }


}
