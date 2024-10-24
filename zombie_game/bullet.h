#ifndef BULLET_H
#define BULLET_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "world.h"
#include "weapons_enum.h"
#include "constants.h"
#include "collidable.h"



class bullet : public collidable
{
public:
    bullet(sf::RenderWindow* window, World* world, sf::Vector2f origin, float bullet_angle, weapons weapon, int level, Player* owner);

    ~bullet();

    static std::map<weapons,sf::Vector2f> correction_vector;
    static std::map<int,sf::Texture> texture;
    static sf::Vector2i texture_frame_size;
    static std::map<weapons,sf::Vector3i> weapon_coords;
    static std::map<weapons,sf::Vector2f> scales;
    static bool textures_loaded;

    sf::Sprite sprite;

    sf::Clock animation_clock;
    int animation_frame = 0;


    sf::RenderWindow* window;
    sf::Vector2f origin;
    sf::Vector2f direction;

    Player* owner;

    weapons weapon;

    b2Body* bullet_body;
    World* world;

    int level = 0;
    int number_of_zombies_passed_through = 0;
    float distance_travelled = 0.f;
    float distance_travelled_limit;

    std::vector<vfx*> vfx_effects;

    static void load_textures();

    void animate(sf::Time elapsed_time);

    void update(sf::Time elapsed_time);

    void move(sf::Time elapsed_time);

    void draw();

    void draw_vfx();

    void fade();

    virtual void take_damage();
    virtual void die();



};

#endif // BULLET_H
