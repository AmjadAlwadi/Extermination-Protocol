#ifndef WORLD_H
#define WORLD_H

#include <SFML/Window.hpp>
#include <tmxlite/Map.hpp>
#include <random>
#include <Box2d/box2d.h>


#include "player.h"
#include "zombie.h"
#include "SFMLOrthogonalLayer.hpp"
#include "constants.h"
#include "world_tile.h"
#include "terrain_object.h"


class Player;



class World
{
public:
    World(sf::RenderWindow* window, b2World* b2_world);

    ~World();

    struct tile_information {
        sf::FloatRect tile_rect;
        sf::Vector2u tile_texture_pos;
        bool collidable;
    };

    sf::RenderWindow *window;

    bool coop_mode = true;
    bool server = false;

    int number_of_coop_players = 1;

    sf::IpAddress server_ip;

    sf::UdpSocket receive_socket;
    sf::UdpSocket send_socket;
    

    static bool friendly_fire;

    // PLAYER 0 IS ME
    std::vector<Player*> players;
    std::vector<Zombie*> zombies;

    b2World* b2_world;

    static std::vector<world_tile*> tiles;

    std::vector<MapLayer*> map_layers;

    std::vector<terrain_object*> terrain_objects;

    sf::Color background_color;

    // sound
    sf::SoundBuffer* wind_sound_buffer;
    sf::Sound wind_sound;

    sf::SoundBuffer* music_sound_buffer;
    sf::Sound music_sound;


    sf::Clock zombie_spawn_clock;
    sf::Clock never_restarting_clock;


    // animate shadow_direction
    int shadow_direction = -60;

    bool round_finished = false;
    int round_counter = 1;

    int zombies_spawned_number = 0;

    int number_of_tiles_x;
    int number_of_tiles_y;

    int tile_width;
    int tile_height;

    static float world_bound_x;
    static float world_bound_y;


    int collidable_layer_index;


    bool spawn_zombie_flag = false;
    sf::Vector2f new_zombie_spawn_position = sf::Vector2f();


    sf::Vector2f player_spawn_location = sf::Vector2f(0,0);

    void setup_map();

    void update(sf::Time elapsed_time);

    void load_map_sprites();

    void draw_map(sf::Time elapsed_time);

    void spawn_zombies(int number);

    void spawn_zombie(sf::Vector2f location);

    void remove_dead();

    void pause_music();

    void resume_music();

    void zombies_chase_players();

    void restart();

    void update_round();

    void enter_new_round();
    
    void move_camera();

    void setup_online_game();

    void receiver_thread();

    void send_zombie_information(sf::Vector2f spawn_location);

    struct tile_information tile_to_rect(int x, int y);

    static int get_random_number(int limit);

    static sf::Vector2f random_coords();

    static sf::Vector2f random_coords(sf::Vector2f position);

    static void switch_to_map(int id);

    

};

#endif // WORLD_H
