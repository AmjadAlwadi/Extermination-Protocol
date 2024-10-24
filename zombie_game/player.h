#ifndef PLAYER_H
#define PLAYER_H


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Box2d/box2d.h>
#include <SFML/Audio.hpp>

#include "player_state_enum.h"
#include "weapons_enum.h"
#include "feet_enum.h"
#include "constants.h"
#include "collidable.h"
#include "packet.h"
#include "vfx.h"

class World;

class bullet;

class Player : public collidable
{
public:
    // 0 for server

    int id = 0;
    int hp = 100;   // Health points

    std::map<weapons,int> kills;
    std::map<weapons,int> level;

    World* world;
    b2Body* player_body;

    bool server = false;
    bool local_player = false;

    bool joystick_player = false;
    int joystick_id = 0;
    
    sf::IpAddress ip;

    sf::Vector2f direction;     // Directional velocities
    float da;                   // Directional angle
    float shadow_direction = 0;


    static sf::Vector2f hitbox_size;

    static bool textures_loaded;

    static std::map<weapons,std::map<player_state, sf::Texture*>> texture;
    static std::map<weapons,std::map<player_state, sf::Texture*>> shadow_texture;
    static std::map<feet, sf::Texture*> feet_texture;

    static sf::SoundBuffer* steps_sound_buffer;
    static std::map<weapons,sf::SoundBuffer*> shoot_sound_buffer;
    static std::map<weapons,sf::SoundBuffer*> reload_sound_buffer;

    static std::map<weapons,float> sound_levels_shoot;
    static float steps_sound_level;

    static std::map<player_state, int> texture_frame_limit;
    static std::map<feet, int> feet_texture_frame_limit;

    static std::map<weapons, std::map<player_state, sf::Vector2i >> texture_frame_size;
    static std::map<feet, sf::Vector2i> feet_texture_frame_size;

    static std::map<weapons,std::map<player_state, sf::Vector2i >> pivots;


    static std::map<weapons,int> magazine_size;

    static std::map<weapons,float> firerate;
    static std::map<weapons,float> reload_time;

    static sf::Texture* status_bar_texture;

    static sf::Text round_bar;
    static int round_counter;


    sf::Text survived_time;
    sf::Text kills_level_bar;


    player_state current_player_state;
    weapons current_equipped_weapon;
    feet current_feet_type;

    sf::RenderWindow* window;

    sf::Sprite shadow_sprite;
    sf::Sprite sprite;
    sf::Sprite feet_sprite;

    sf::Texture blank_texture;

    sf::Sprite health_bar_left;
    sf::Sprite health_bar_right;
    sf::Sprite health_bar_left_blank;
    sf::Sprite health_bar_right_blank;
    sf::Sprite health_bar_center_dot;
    sf::Sprite health_bar_back_plate;
    sf::Text health_counter;

    sf::Text magazine_bar;
    std::map<weapons,int> magazine_load;


    sf::Clock animation_clock;
    sf::Clock feet_animation_clock;
    sf::Clock joystick_click_clock;

    std::vector<bullet*> bullets;

    std::vector<vfx*> vfx_effects;


    sf::Sound steps_sound;
    sf::Sound shoot_sound;
    sf::Sound reload_sound;

    sf::Clock send_clock;


    int animation_frame = 0;
    int feet_animation_frame = 0;
    bool animation_finished = false;

    sf::Vector2f delta_shadow_pos;

    sf::Vector2f target_pos = sf::Vector2f(0.f, 0.f);

    bool shoot_flag = false;

    bool state_has_changed = false;





    // Constructor to initialize the player's attributes
    Player(sf::RenderWindow *window, World *world, sf::Vector2f spawn_location, bool joystick_player,int joystick_id, sf::IpAddress ip, bool local_player, bool server);

    ~Player();

    void setup_values();

    void setup_shadow_sprite();

    void update(sf::Time elapsed_time);

    void animate(sf::Time elapsed_time);

    void draw();

    void draw_shadow();

    void draw_vfx();

    void move(sf::Time elapsed_time);

    void rotate_player();

    void set_position(float x, float y);

    void load_textures();

    void load_sound();

    void setup_status_bar();

    void shoot_bullet();

    void draw_status_bar();

    void update_status_bar();

    void reload_gun();

    void restart_level();

    void level_up();

    void change_level(int pervious_level);

    void next_weapon();

    void previous_weapon();

    virtual void take_damage();
    virtual void take_damage(bullet* bullet);
    virtual void die();

    bool change_player_state(player_state state);
    bool change_feet_state(feet state);
    bool change_weapon(weapons weapon);

    packet pack_to_packet();
    void apply_data_from_packet(packet& packet);

    packet pack_bullet_to_packet();
    void apply_bullet_data_from_packet(packet& packet);

    void send_information();
    void send_bullet_information();

    sf::Vector2i frame_to_texture_rect(int frame_index);
    sf::Vector2i frame_to_feet_texture_rect(int frame_index);
    sf::Vector2i index_to_texture_rect(int frame_index, sf::Vector2i frame_size, sf::Vector2u texture_size);

};

#endif // PLAYER_H
