#ifndef VFX_H
#define VFX_H

#include <SFML/Graphics.hpp>

class vfx
{
public:
    vfx();

    sf::RenderWindow* window;
    enum vfx_type {BLOOD,BLOOD_STATIC,GUTS,DIRT};
    vfx_type type;

    float animation_time_in_seconds;

    static bool textures_loaded;

    static std::map<vfx_type,std::string> paths;

    static std::map<vfx_type,sf::Texture*> texture;

    static std::map<vfx_type,sf::Vector2u> frame_size;

    static std::map<vfx_type, int > texture_frame_limit;



    sf::Sprite sprite;

    bool random_rotation_enabled = false;
    int random_rotation = 0;

    sf::Vector2u texture_size = sf::Vector2u(0,0);

    int animation_frame = 0;
    sf::Clock animation_clock;

    bool alive = true;


    vfx(sf::RenderWindow* window, vfx_type type, float animation_time_in_seconds, bool random_rotation_enabled);
    vfx(sf::RenderWindow *window, vfx_type type, float animation_time_in_seconds, bool random_rotation_enabled, sf::Vector2f position, sf::Vector2f scale);

    ~vfx();

    void animate();

    void load_textures();

    void draw();


    sf::Vector2u frame_to_texture_rect(int frame_index);



};

#endif // VFX_H
