#include "world_tile.h"
#include "world.h"
#include "constants.h"

#include <iostream>


world_tile::world_tile(sf::RenderWindow* window, b2World *b2_world, int shadow_direction, sf::Vector2u texture_pos) {

    this->window = window;
    this->b2_world = b2_world;
    type = TILE_TYPE;
    this->shadow_direction = shadow_direction;

    shadow_sprite.setTexture(*shadow_texture);
    shadow_sprite.setTextureRect(sf::IntRect(texture_pos.x,texture_pos.y,tile_width,tile_height));
    shadow_sprite.setOrigin(tile_width/2.f, tile_height/2.f);
    shadow_sprite.setScale(1,1);


}

world_tile::~world_tile()
{
    b2_world->DestroyBody(this->tile_body);
}








void world_tile::setup_tilebody(sf::FloatRect tile)
{

    tile_width = tile.width;
    tile_height = tile.height;

    // Body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set((tile.getPosition().x + tile.width/2.f)/PPM, window->getSize().y/PPM - ((tile.getPosition().y + tile.height/2.f) / PPM));
    boxBodyDef.type = b2_staticBody;

    // Shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(tile.width /2.f/PPM, tile.height /2.f/PPM);


    // Now we have a body for our Box object
    tile_body = b2_world->CreateBody(&boxBodyDef);

    // Fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.5f;
    fixtureDef.shape = &boxShape;

    b2FixtureUserData data;
    data.pointer = reinterpret_cast<uintptr_t>(this);
    fixtureDef.userData = data;

    // Lastly, assign the fixture
    tile_body->CreateFixture(&fixtureDef);


}








void world_tile::setup_shadow_sprite(std::string path_to_tileset)
{
    sf::Texture tileset_texture;

    shadow_texture = new sf::Texture();

    if (!tileset_texture.loadFromFile(path_to_tileset)) {
        std::cerr << "Failed to load tileset from: " << path_to_tileset << std::endl;
        return;  // return if the texture failed to load
    }


    shadow_texture->create(tileset_texture.getSize().x,tileset_texture.getSize().y);

    sf::Image shadow_image = tileset_texture.copyToImage();

    for(int i = 0; i < tileset_texture.getSize().x;i++){

        for(int j = 0; j < tileset_texture.getSize().y;j++){

            shadow_image.setPixel(i,j,sf::Color(0,0,0,shadow_image.getPixel(i,j).a * TERRAIN_SHADOW_STRENGTH));

        }
    }

    shadow_texture->update(shadow_image);
    shadow_texture->setSmooth(true);

}


void world_tile::draw_shadow()
{
    window->draw(shadow_sprite);
}





void world_tile::update_shadow_position()
{
    delta_shadow_pos = sf::Vector2f(std::sin(shadow_direction * M_PI/180.f),std::cos(shadow_direction * M_PI/180.f)) * WORLD_TILE_SHADOW_DISTANCE_FACTOR;
    shadow_sprite.setPosition((tile_body->GetPosition().x * PPM) + delta_shadow_pos.x,(this->window->getSize().y - (tile_body->GetPosition().y * PPM)) + delta_shadow_pos.y);
}











