#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <Box2d/box2d.h>


#include "constants.h"
#include "zombie_state_enum.h"
#include "collidable.h"
#include "vfx.h"


class World;

class Zombie : public collidable
{
public:

    Zombie(sf::RenderWindow *window, World* world, sf::Vector2f position, float shadow_direction,int starting_hp);

    ~Zombie();

    static bool textures_loaded;


    int hp = 100;   // Health points
    int level = 0;
    float da = 0.f;

    bool can_give_damage = true;

    sf::Vector2f direction = sf::Vector2f(0,0);

    static std::map<zombie_state, sf::Texture*> texture;
    static std::map<zombie_state,sf::Texture*> shadow_texture;

    static std::map<zombie_state, int > texture_frame_limit;
    static std::map<zombie_state, sf::Vector2i> texture_frame_size;
    static std::map<zombie_state, sf::Vector2i> pivots;

    static std::map<zombie_state,sf::SoundBuffer*> zombie_sound_buffer;

    static std::map<zombie_state,float> sound_levels;
    static std::map<zombie_state,bool> sound_loop_enabled;

    void setup_values();


    std::vector<vfx*> vfx_effects;


    sf::Sprite sprite;
    sf::Sprite shadow_sprite;

    float shadow_direction = 0;

    sf::RenderWindow* window;
    World* world;

    b2Body* zombie_body;


    sf::Clock animation_clock;

    zombie_state current_zombie_state;

    sf::Vector2f delta_shadow_pos;

    sf::Vector2f hitbox_size;



    sf::Sound zombie_sound;

    int animation_frame = 0;

    void change_state(zombie_state new_state);

    sf::Vector2i frame_to_texture_rect(int frame_index);

    void setup_shadow_sprite();

    void update(sf::Time elapsed_time);

    void animate(sf::Time elapsed_time);

    void draw();

    void draw_shadow();

    void draw_vfx();

    void move(sf::Time elapsed_time);

    void load_textures();

    void load_sounds();

    void rotate_zombie();

    void move_towards(float angle);

    void remove_faded_effects();





    virtual void die();

    virtual void take_damage(bullet* bullet);




};

#endif // ZOMBIE_H
