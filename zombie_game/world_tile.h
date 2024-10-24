#ifndef WORLD_TILE_H
#define WORLD_TILE_H

#include "collidable.h"
#include <Box2d/box2d.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class world_tile : public collidable
{
public:
    world_tile(sf::RenderWindow* window, b2World* world, int shadow_direction, sf::Vector2u texture_pos);

    ~world_tile();

    static float tile_width;
    static float tile_height;

    b2Body* tile_body;

    sf::RenderWindow* window;
    b2World *b2_world;

    static sf::Texture* shadow_texture;
    sf::Sprite shadow_sprite;

    float shadow_direction = 0;
    sf::Vector2f delta_shadow_pos;



    void setup_tilebody(sf::FloatRect tile);

    static void setup_shadow_sprite(std::string path_to_tileset);

    void draw_shadow();

    void update_shadow_position();

};

#endif // WORLD_TILE_H
