#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include <SFML/Graphics.hpp>


class terrain_object
{
public:

    sf::RenderWindow* window;
    enum terrain_type {PIXELATED_TREE};
    terrain_type type;

    static std::map<terrain_type,std::string> paths;
    static bool textures_loaded;
    static std::map<terrain_type,sf::Texture*> texture;
    static std::map<terrain_type,sf::Texture*> shadow_texture;
    static std::map<terrain_type,sf::Vector2u> frame_size;
    static std::map<terrain_type, int > texture_frame_limit;

    sf::Sprite sprite;
    sf::Sprite shadow_sprite;

    int random_rotation = 0;
    float shadow_direction = 0;
    sf::Vector2f delta_shadow_pos;

    sf::Vector2u texture_size = sf::Vector2u(0,0);

    int animation_frame = 0;
    sf::Clock animation_clock;



    terrain_object(sf::RenderWindow* window, terrain_type type, int shadow_direction);
    terrain_object(sf::RenderWindow *window, terrain_type type, int shadow_direction, sf::Vector2f position, sf::Vector2f scale);

    ~terrain_object();


    void animate();

    void load_textures();

    void setup_shadow_sprite();

    void draw_shadow();

    void draw();

    sf::Vector2u frame_to_texture_rect(int frame_index);

};

#endif // TERRAIN_OBJECT_H
