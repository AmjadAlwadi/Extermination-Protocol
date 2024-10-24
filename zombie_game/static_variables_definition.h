#include "bullet.h"
#include "menu.h"




#ifndef STATIC_VARIABLES_DEFINITION_H
#define STATIC_VARIABLES_DEFINITION_H

// bullet class
std::map<weapons,sf::Vector2f> bullet::correction_vector;
std::map<int,sf::Texture> bullet::texture;
bool bullet::textures_loaded = false;
sf::Vector2i bullet::texture_frame_size = sf::Vector2i(16,16);
std::map<weapons,sf::Vector3i> bullet::weapon_coords;
std::map<weapons,sf::Vector2f> bullet::scales;




// menu class
sf::Clock menu::click_clock;
bool menu::loaded = false;
sf::RenderWindow* menu::window;
sf::Font menu::font;
sf::Font menu::cursed_font;
sf::Font menu::zombie_font;
game_state menu::current_game_state = game_state::MAIN_MENU;
int menu::loading_bar_bullets = 0;
std::vector<sf::Sprite*> menu::bullets;
sf::Texture menu::bullet_texture;
sf::Sprite menu::magazine_bottom;
sf::Sprite menu::magazine_top;
sf::Texture menu::magazine_top_texture;
sf::Texture menu::magazine_bottom_texture;
sf::Sprite menu::loading_screen_background;
sf::Texture menu::loading_screen_background_texture;
sf::Text menu::game_title;
float menu::current_difficulty_level = 0.f;




// player class
sf::Vector2f Player::hitbox_size;
bool Player::textures_loaded = false;
std::map<weapons,std::map<player_state, sf::Texture*>> Player::texture;
std::map<weapons,std::map<player_state, sf::Texture*>> Player::shadow_texture;
std::map<feet, sf::Texture*> Player::feet_texture;
sf::SoundBuffer* Player::steps_sound_buffer;
std::map<weapons,sf::SoundBuffer*> Player::shoot_sound_buffer;
std::map<weapons,sf::SoundBuffer*> Player::reload_sound_buffer;
std::map<weapons,float> Player::sound_levels_shoot;
float Player::steps_sound_level;
std::map<player_state, int> Player::texture_frame_limit;
std::map<feet, int> Player::feet_texture_frame_limit;
std::map<weapons, std::map<player_state, sf::Vector2i >> Player::texture_frame_size;
std::map<feet, sf::Vector2i> Player::feet_texture_frame_size;
std::map<weapons,std::map<player_state, sf::Vector2i >> Player::pivots;
std::map<weapons,int> Player::magazine_size;
std::map<weapons,float> Player::firerate;
std::map<weapons,float> Player::reload_time;
sf::Texture* Player::status_bar_texture;
sf::Text Player::round_bar;
int Player::round_counter = 1;




// terrain object
std::map<terrain_object::terrain_type,std::string> terrain_object::paths = {std::make_pair(PIXELATED_TREE,ASSETS_PATH "custom objects/pixelated_tree.png")};
bool terrain_object::textures_loaded = false;
std::map<terrain_object::terrain_type,sf::Texture*> terrain_object::texture;
std::map<terrain_object::terrain_type,sf::Texture*> terrain_object::shadow_texture;
std::map<terrain_object::terrain_type,sf::Vector2u> terrain_object::frame_size  = {std::make_pair(PIXELATED_TREE,sf::Vector2u(500,500))};
std::map<terrain_object::terrain_type, int > terrain_object::texture_frame_limit;




// vfx class
bool vfx::textures_loaded;
std::map<vfx::vfx_type,std::string> vfx::paths = {std::make_pair(BLOOD,ASSETS_PATH"vfx/blood_5.png"),std::make_pair(BLOOD_STATIC,ASSETS_PATH"vfx/blood.png"),
                                                  std::make_pair(GUTS,ASSETS_PATH"vfx/guts.png"),std::make_pair(DIRT,ASSETS_PATH"vfx/dirt.png")};
std::map<vfx::vfx_type,sf::Texture*> vfx::texture;
std::map<vfx::vfx_type,sf::Vector2u> vfx::frame_size = {std::make_pair(BLOOD,sf::Vector2u(100,100)),std::make_pair(BLOOD_STATIC,sf::Vector2u(70,76)),
                                                        std::make_pair(GUTS,sf::Vector2u(70,76)), std::make_pair(DIRT,sf::Vector2u(32,32))};
std::map<vfx::vfx_type, int > vfx::texture_frame_limit;




// zombie class
bool Zombie::textures_loaded = false;
std::map<zombie_state, sf::Texture*> Zombie::texture;
std::map<zombie_state,sf::Texture*> Zombie::shadow_texture;
std::map<zombie_state, int > Zombie::texture_frame_limit;
std::map<zombie_state, sf::Vector2i> Zombie::texture_frame_size;
std::map<zombie_state, sf::Vector2i> Zombie::pivots;
std::map<zombie_state,sf::SoundBuffer*> Zombie::zombie_sound_buffer;
std::map<zombie_state,float> Zombie::sound_levels;
std::map<zombie_state,bool> Zombie::sound_loop_enabled;



// world class
float World::world_bound_x;
float World::world_bound_y;
std::vector<world_tile*> World::tiles;
bool World::friendly_fire = false;



// world tile
float world_tile::tile_width;
float world_tile::tile_height;
sf::Texture* world_tile::shadow_texture;


#endif // STATIC_VARIABLES_DEFINITION_H
