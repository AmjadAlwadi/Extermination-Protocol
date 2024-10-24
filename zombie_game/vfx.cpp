#include "vfx.h"
#include "iostream"
#include "constants.h"
#include "world.h"


vfx::vfx(sf::RenderWindow *window, vfx_type type, float animation_time_in_seconds, bool random_rotation_enabled)
{
    this->window = window;
    this->type = type;
    this->animation_time_in_seconds = animation_time_in_seconds;


    // apply random rotation
    if(random_rotation_enabled)
        random_rotation = World::get_random_number(359);


    if(!textures_loaded){
        load_textures();
        textures_loaded = true;
    }

    texture_size = texture[type]->getSize();

    sprite.setTexture(*texture[type]);
    sprite.setTextureRect(sf::IntRect(0,0,frame_size[type].x,frame_size[type].y));

    sprite.setOrigin(frame_size[type].x/2.f, frame_size[type].y/2.f);
    sprite.setRotation(random_rotation);

}




vfx::vfx(sf::RenderWindow *window, vfx_type type, float animation_time_in_seconds,bool random_rotation_enabled, sf::Vector2f position, sf::Vector2f scale) : vfx(window,type,animation_time_in_seconds,random_rotation_enabled)
{
    sprite.setScale(scale);
    sprite.setPosition(position);
}




vfx::~vfx()
{

}



sf::Vector2u vfx::frame_to_texture_rect(int frame_index){

    return sf::Vector2u((frame_index * frame_size[type].x) % texture_size.x,((frame_index * frame_size[type].x)/texture_size.x) * frame_size[type].y);

}




void vfx::load_textures()
{
    // initialize textures on the heap
    for (const auto& t1 : paths) {

        texture.insert(std::make_pair(t1.first,new sf::Texture()));
    }


    // load the textuers
    for (const auto& t1 : paths) {

        if (!texture[t1.first]->loadFromFile(t1.second)) {
            std::cerr << "Failed to load grass texture" << std::endl;
            return;
        }

        texture[t1.first]->setSmooth(true);
    }


    texture_frame_limit[vfx_type::BLOOD] = 29;
    texture_frame_limit[vfx_type::BLOOD_STATIC] = 1;
    texture_frame_limit[vfx_type::GUTS] = 1;
    texture_frame_limit[vfx_type::DIRT] = 20;

}





void vfx::draw()
{
    window->draw(sprite);
}




void vfx::animate()
{

    // animate player
    if (animation_clock.getElapsedTime().asSeconds() > animation_time_in_seconds * FRAME_TIME * (24.f/texture_frame_limit[type])){

        sf::Vector2u texture_pos = frame_to_texture_rect(animation_frame);

        sprite.setTextureRect(sf::IntRect(texture_pos.x,texture_pos.y,frame_size[type].x,frame_size[type].y));
        sprite.setOrigin(frame_size[type].x/2.f, frame_size[type].y/2.f);

        animation_frame++;
        animation_clock.restart();


        // vfx fades away when it reaches the end of animation
        if(animation_frame == texture_frame_limit[type])
            alive = false;

    }

    draw();
}


