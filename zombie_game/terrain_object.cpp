#include "terrain_object.h"
#include "iostream"
#include "constants.h"
#include <math.h>

terrain_object::terrain_object(sf::RenderWindow *window, terrain_type type, int shadow_direction)
{
    this->window = window;
    this->type = type;
    this->shadow_direction = shadow_direction;

    // apply random rotation
    double random_value = static_cast<double>(rand());
    float frame_time_factor = (random_value / (RAND_MAX + 1.0));

    random_rotation = (TERRAIN_RANDOM_ROTATION) - frame_time_factor* (TERRAIN_RANDOM_ROTATION * 2.f);

    if(!textures_loaded){
        load_textures();
        setup_shadow_sprite();
        textures_loaded = true;
    }

    texture_size = texture[type]->getSize();

    sprite.setTexture(*texture[type]);
    sprite.setTextureRect(sf::IntRect(0,0,frame_size[type].x,frame_size[type].y));

    shadow_sprite.setTexture(*shadow_texture[type]);
    shadow_sprite.setTextureRect(sf::IntRect(0,0,frame_size[type].x,frame_size[type].y));

    sprite.setOrigin(frame_size[type].x/2.f, frame_size[type].y/2.f);
    sprite.setRotation(random_rotation);

    shadow_sprite.setOrigin(frame_size[type].x/2.f, frame_size[type].y/2.f);
    shadow_sprite.setRotation(random_rotation);

    delta_shadow_pos = sf::Vector2f(std::sin(shadow_direction * M_PI/180.f),std::cos(shadow_direction * M_PI/180.f)) * SHADOW_DISTANCE_FACTOR * 8.f * sprite.getScale().x;

}




terrain_object::terrain_object(sf::RenderWindow *window, terrain_type type, int shadow_direction, sf::Vector2f position, sf::Vector2f scale) : terrain_object(window,type,shadow_direction)
{
    sprite.setPosition(position);
    shadow_sprite.setPosition(sprite.getPosition() + delta_shadow_pos);
    sprite.setScale(scale);
    shadow_sprite.setScale(sprite.getScale());

}




terrain_object::~terrain_object()
{

}



sf::Vector2u terrain_object::frame_to_texture_rect(int frame_index){

    return sf::Vector2u((frame_index * frame_size[type].x) % texture_size.x,((frame_index * frame_size[type].x)/texture_size.x) * frame_size[type].y);

}




void terrain_object::load_textures()
{
    // initialize textures on the heap
    for (const auto& t1 : paths) {

        texture.insert(std::make_pair(t1.first,new sf::Texture()));
        shadow_texture.insert(std::make_pair(t1.first,new sf::Texture()));

    }


    // load the textuers
    for (const auto& t1 : paths) {

        if (!texture[t1.first]->loadFromFile(t1.second)) {
            std::cerr << "Failed to load grass texture" << std::endl;
            return;
        }

        texture[t1.first]->setSmooth(true);
    }



    texture_frame_limit[terrain_type::PIXELATED_TREE] = 24;

}






void terrain_object::setup_shadow_sprite()
{

    for (int terrain_index = static_cast<int>(terrain_type::PIXELATED_TREE); terrain_index <= static_cast<int>(terrain_type::PIXELATED_TREE); terrain_index++) {

        terrain_type terrain = static_cast<terrain_type>(terrain_index);

        shadow_texture[terrain]->create(texture[terrain]->getSize().x,texture[terrain]->getSize().y);

        sf::Image shadow_image = texture[terrain]->copyToImage();

        for(int i = 0; i < texture[terrain]->getSize().x;i++){

            for(int j = 0; j < texture[terrain]->getSize().y;j++){

                shadow_image.setPixel(i,j,sf::Color(0,0,0,shadow_image.getPixel(i,j).a * TERRAIN_SHADOW_STRENGTH));

            }
        }

        shadow_texture[terrain]->update(shadow_image);
        shadow_texture[terrain]->setSmooth(true);

    }

}







void terrain_object::draw_shadow()
{
    window->draw(shadow_sprite);
}

void terrain_object::draw()
{
    window->draw(sprite);
}





void terrain_object::animate()
{

    // animate player
    if (animation_clock.getElapsedTime().asSeconds() > 2.f*FRAME_TIME){

        sf::Vector2u texture_pos = frame_to_texture_rect(animation_frame);

        sprite.setTextureRect(sf::IntRect(texture_pos.x,texture_pos.y,frame_size[type].x,frame_size[type].y));
        shadow_sprite.setTextureRect(sf::IntRect(texture_pos.x,texture_pos.y,frame_size[type].x,frame_size[type].y));

        animation_frame++;
        animation_clock.restart();



        // check for frame
        if(animation_frame == texture_frame_limit[type])
            animation_frame = 0;

    }

    draw_shadow();
    draw();
}


