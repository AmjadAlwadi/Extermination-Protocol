#ifndef MENU_H
#define MENU_H


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_state_enum.h"

class World;

class menu
{
public:
    menu();
    menu(sf::RenderWindow* window);
    menu(sf::RenderWindow* window,sf::Vector2i size, std::string atlas_path);

    ~menu();

    struct callback_info {
        void (*func)(sf::RenderWindow*); // Function pointer
    };

    struct button{

        sf::Text* sfml_text;
        sf::Color color;
        sf::Color on_hover_color;
        struct callback_info cal;

    };


    struct textbox {

        sf::RectangleShape rect;
        sf::Text sfml_text;
        std::string place_holder_text;
        sf::Color color;
        sf::Color on_focus_color;

    };

    static sf::Clock click_clock;

    static bool loaded;

    static sf::RenderWindow* window;

    static sf::Font font;
    static sf::Font cursed_font;
    static sf::Font zombie_font;

    static game_state current_game_state;


    static int loading_bar_bullets;
    static std::vector<sf::Sprite*> bullets;
    static sf::Texture bullet_texture;

    static sf::Sprite magazine_bottom;
    static sf::Sprite magazine_top;

    static sf::Texture magazine_top_texture;
    static sf::Texture magazine_bottom_texture;

    static sf::Sprite loading_screen_background;
    static sf::Texture loading_screen_background_texture;

    static sf::Text game_title;

    static float current_difficulty_level;

    static void change_view_to_menu();

    static void setup_loading_screen();
    static void step_loading_screen();

    struct menu::textbox* textbox_in_focus = nullptr;


    sf::Texture menu_atlas;
    sf::RenderTexture render_texture;

    std::vector<struct button*> buttons;
    std::vector<struct menu::textbox*> text_boxes;

    std::vector<sf::Texture*> textures;
    std::vector<sf::Sprite*> sprites;
    std::vector<sf::Text*> texts;

    sf::SoundBuffer* music_sound_buffer;
    sf::Sound music_sound;

    void setup(sf::Vector2i size);

    void load_atlas(std::string atlas_path);
    void set_background(std::string path);
    void create_decoration(sf::Vector2f window_position,sf::Vector2f scale, sf::Vector2i texture_position, sf::Vector2i size);
    void create_button(sf::Vector2f window_position, std::string text, sf::Color color ,sf::Color on_hover_color, void (*callback)(sf::RenderWindow*));
    void check_callback(sf::Vector2i mouse_pos);
    void add_sprite(sf::Sprite* sprite);
    sf::Text* add_text(sf::Color color, sf::Vector2f window_position, std::string string);
    sf::Text* add_textbox(sf::Vector2f window_position, sf::Color color, sf::Color on_focus_color, std::string placeholder_text);

    void draw();
    void update();

    void add_music(std::string music_name, float sound_level, bool loop_enabled);

    void start_music();
    void stop_music();

    void disable();



};

#endif // MENU_H
