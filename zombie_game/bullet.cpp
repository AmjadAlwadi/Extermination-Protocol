#include "bullet.h"

#include <cmath>
#include <iostream>

bullet::bullet(sf::RenderWindow* window,World* world, sf::Vector2f origin, float bullet_angle, weapons weapon, int level, Player* owner){

    this->type = BULLET_TYPE;
    this->window = window;
    this->weapon = weapon;
    this->world = world;
    this->level = level;
    this->owner = owner;

    // calculate vector
    this->direction.y = std::sin(bullet_angle * M_PI/180.f) * BULLET_SPEED;
    this->direction.x = std::cos(bullet_angle * M_PI/180.f) * BULLET_SPEED;


    // load textures and setup values
    if(!textures_loaded){

        load_textures();
        textures_loaded = true;

    }


    // calculate correction to weapon tip
    // normalize diff
    float correction_vector_length = std::sqrt(correction_vector[weapon].x * correction_vector[weapon].x + correction_vector[weapon].y * correction_vector[weapon].y);

    float corrected_angle = bullet_angle + (atan2(correction_vector[weapon].y, correction_vector[weapon].x) * (180 / M_PI));

    float y = std::sin(corrected_angle * M_PI/180.f) * correction_vector_length;
    float x = std::cos(corrected_angle * M_PI/180.f) * correction_vector_length;

    this->origin = origin + sf::Vector2f(x,y);
    // calculate correction to weapon tip


    sprite.setTexture(texture[level - 1],true);
    sprite.setTextureRect(sf::IntRect(weapon_coords[weapon].x - texture_frame_size.x/2.f,weapon_coords[weapon].y - texture_frame_size.y/2.f,texture_frame_size.x,texture_frame_size.y));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    sprite.setPosition(this->origin);

    // rotate bullet sprite to the direction
    sprite.setRotation(bullet_angle);

    // set scale
    sprite.setScale(scales.at(weapon));



    // body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(this->origin.x/PPM, (this->window->getSize().y - this->origin.y)/PPM);
    boxBodyDef.type = b2_dynamicBody;

    // shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(sprite.getGlobalBounds().width /2.f/PPM, sprite.getGlobalBounds().height /2.f/PPM);


    // fixture definition
    b2FixtureDef fixtureDef;
    // fixtureDef.density = 1.f;
    // fixtureDef.friction = 1.f;
    fixtureDef.shape = &boxShape;

    //if(weapon == weapons::handgun)
    fixtureDef.isSensor = true;

    b2FixtureUserData data;
    data.pointer = reinterpret_cast<uintptr_t>(this);
    fixtureDef.userData = data;


    fixtureDef.shape = &boxShape;


    // now we have a body for our Box object
    bullet_body = world->b2_world->CreateBody(&boxBodyDef);

    bullet_body->SetBullet(true);

    // lastly, assign the fixture
    bullet_body->CreateFixture(&fixtureDef);


    bullet_body->SetLinearVelocity(b2Vec2(direction.x, -direction.y));


    // move the bullet
    distance_travelled_limit =  CAMERA_WIDTH;

    if(weapon == weapons::shotgun){

        distance_travelled_limit =  (distance_travelled_limit/6.f);
        distance_travelled_limit +=  (level - 1) * distance_travelled_limit/3.f;

    }else if (weapon == weapons::knife) {

        distance_travelled_limit = (distance_travelled_limit / 84.f);
    }

}



bullet::~bullet()
{
    world->b2_world->DestroyBody(bullet_body);
}





void bullet::update(sf::Time elapsed_time)
{
    fade();
    animate(elapsed_time);
}






void bullet::animate(sf::Time elapsed_time)
{
    // animate
    if (animation_clock.getElapsedTime().asSeconds() > FRAME_TIME){

        sprite.setTextureRect(sf::IntRect(weapon_coords[weapon].x - texture_frame_size.x/2.f +((animation_frame % weapon_coords[weapon].z) * texture_frame_size.x),weapon_coords[weapon].y - texture_frame_size.y/2.f,texture_frame_size.x,texture_frame_size.y));
        animation_frame++;
        animation_clock.restart();

    }

    move(elapsed_time);

    draw();
    draw_vfx();
}





void bullet::draw(){

    window->draw(sprite);

}


void bullet::draw_vfx()
{
    for(auto effect : vfx_effects){

        effect->animate();

    }

}






void bullet::move(sf::Time elapsed_time)
{
    // increase distance
    distance_travelled += BULLET_SPEED * PPM / FRAME_RATE;

    b2Vec2 new_pos = bullet_body->GetPosition();
    this->sprite.setPosition(new_pos.x * PPM,this->window->getSize().y - (new_pos.y * PPM));

}





void bullet::fade()
{
    if(distance_travelled > distance_travelled_limit){

        die();

    }

}



void bullet::take_damage()
{

}




void bullet::die()
{
    if (weapon == weapons::handgun) {

        this->number_of_zombies_passed_through++;
        
        if (number_of_zombies_passed_through >= NUMBER_OF_ZOMBIES_PASSED_THROUG_FIRST_LEVEL + (NUMBER_OF_ZOMBIES_PASSED_THROUG_PER_LEVEL_INCREASE * level)) {
            this->alive = false;
        }

    }

    else {
    
        this->alive = false;
    
    }

}





void bullet::load_textures()
{

    // load texture atlas
    for(int i = 0;i < 7; i++){

        std::string directory_path = ASSETS_PATH"fire_bullet/";

        std::string filePath = directory_path + "All_Fire_Bullet_Pixel_16x16_0" + std::to_string(i) + ".png";

        if (!texture[i].loadFromFile(filePath)) {
            std::cerr << "Failed to load texture from: " << filePath << std::endl;
            return; // Skip this iteration if the texture failed to load
        }

    }



    // setup values

    int x;
    int y;
    int number_of_frames;

    x = 104;
    y = 313;
    number_of_frames = 4;

    weapon_coords[weapons::handgun] = sf::Vector3i(x,y,number_of_frames);

    x = 8;
    y = 313;
    number_of_frames = 5;

    weapon_coords[weapons::rifle] = sf::Vector3i(x,y,number_of_frames);

    x = 264;
    y = 120;
    number_of_frames = 4;

    weapon_coords[weapons::shotgun] = sf::Vector3i(x,y,number_of_frames);


    x = 605;
    y = 166;
    number_of_frames = 1;

    weapon_coords[weapons::knife] = sf::Vector3i(x, y, number_of_frames);



    scales[weapons::rifle] =  sf::Vector2f(PLAYER_SCALE,PLAYER_SCALE) * 4.f;
    scales[weapons::shotgun] = sf::Vector2f(PLAYER_SCALE * 5 ,PLAYER_SCALE * 30.f);
    scales[weapons::handgun] = sf::Vector2f(PLAYER_SCALE,PLAYER_SCALE) * 5.f;
    scales[weapons::knife] = sf::Vector2f(PLAYER_SCALE * 2, PLAYER_SCALE * 17.f);

    correction_vector[weapons::rifle] = sf::Vector2f((158.f) * PLAYER_SCALE,(32.f) * PLAYER_SCALE);
    correction_vector[weapons::handgun] = sf::Vector2f((200.f) * PLAYER_SCALE,(44.f) * PLAYER_SCALE);
    correction_vector[weapons::shotgun] = sf::Vector2f((290.f) * PLAYER_SCALE,(32.f) * PLAYER_SCALE);
    correction_vector[weapons::knife] = sf::Vector2f((40.f) * PLAYER_SCALE, (-16.f) * PLAYER_SCALE);

}







