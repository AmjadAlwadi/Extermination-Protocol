#include "menu.h"
#include "constants.h"

#include <iostream>
#include <SFML/Window.hpp>

menu::menu() {}

menu::menu(sf::RenderWindow *window)
{

}



menu::menu(sf::RenderWindow *window, sf::Vector2i size, std::string atlas_path)  : menu(window)
{
    setup(size);
    load_atlas(atlas_path);
}



menu::~menu()
{
    for(auto button : buttons){
        delete button;
    }

    for(auto sprite : sprites){
        delete sprite;
    }

    for(auto tex : textures){
        delete tex;
    }

    for(auto bul : bullets){
        delete bul;
    }

    for (auto textbox : text_boxes) {
        delete textbox;
    }


    delete music_sound_buffer;

}



void menu::setup(sf::Vector2i size)
{
    if (!render_texture.create(size.x,size.y))
    {
        std::cerr << "Failed to setup render_texture" << std::endl;
    }
}




void menu::load_atlas(std::string atlas_path)
{
    if (!menu_atlas.loadFromFile(atlas_path)) {
        std::cerr << "Failed to load menu atlas" << std::endl;
        return;
    }
}




void menu::set_background(std::string path)
{

    sf::Texture *background_texture = new sf::Texture();
    sf::Sprite *background = new sf::Sprite();

    sprites.push_back(background);
    textures.push_back(background_texture);


    if (!background_texture->loadFromFile(path)) {
        std::cerr << "Failed to load background" << std::endl;
        return;
    }


    background_texture->setSmooth(true);

    sf::Image shadow_image = background_texture->copyToImage();

    for(int i = 0; i < background_texture->getSize().x;i++){

        for(int j = 0; j < background_texture->getSize().y;j++){

            shadow_image.setPixel(i,j,sf::Color(shadow_image.getPixel(i,j).r,shadow_image.getPixel(i,j).g,shadow_image.getPixel(i,j).b, BACKGROUND_ALPHA));

        }
    }

    background_texture->update(shadow_image);
    background_texture->setSmooth(true);


    background->setTexture(*background_texture,true);
    background->setOrigin(0,0);
    background->setPosition(0,0);

    background->setTextureRect(sf::IntRect(0,0,window->getSize().x,window->getSize().y));

    render_texture.draw(*background);

}






void menu::create_decoration(sf::Vector2f window_position,sf::Vector2f scale, sf::Vector2i texture_position, sf::Vector2i size)
{
    sf::Sprite *deco = new sf::Sprite();

    sprites.push_back(deco);

    deco->setTexture(menu_atlas,true);
    deco->setTextureRect(sf::IntRect(texture_position - (size/2),size));

    deco->setScale(scale);

    deco->setOrigin(size.x/2.f,size.y/2.f);
    deco->setPosition(window_position);

    render_texture.draw(*deco);
}




void menu::create_button(sf::Vector2f window_position, std::string text, sf::Color color,sf::Color on_hover_color, void (*callback)(sf::RenderWindow*))
{
    sf::Text *sfml_text = new sf::Text();

    sfml_text->setFont(font);
    sfml_text->setString(text);
    sfml_text->setCharacterSize(40);
    sfml_text->setFillColor(color);
    sfml_text->setStyle(sf::Text::Italic || sf::Text::Bold);


    sfml_text->setOrigin(sfml_text->getLocalBounds().getSize().x/2.f,sfml_text->getLocalBounds().getSize().y/1.5f);
    sfml_text->setPosition(window_position);

    render_texture.draw(*sfml_text);

    callback_info cal;
    cal.func = callback;

    struct button *new_button = new struct button;
    new_button->sfml_text = sfml_text;
    new_button->color = color;
    new_button->on_hover_color = on_hover_color;
    new_button->cal = cal;

    buttons.push_back(new_button);

}

extern std::string typed_text;


void menu::check_callback(sf::Vector2i mouse_pos)
{

    for(auto button : buttons){

        if(button->sfml_text->getGlobalBounds().contains(sf::Vector2f(mouse_pos))){

            button->sfml_text->setFillColor(button->on_hover_color);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && click_clock.getElapsedTime().asSeconds() > 0.3f){
                button->cal.func(this->window);
                click_clock.restart();
            }

        }else{

            button->sfml_text->setFillColor(button->color);

        }
    }

    

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && click_clock.getElapsedTime().asSeconds() > 0.3f) {

        textbox_in_focus = nullptr;

        for (auto textbox : text_boxes) {

            if (textbox->rect.getGlobalBounds().contains(sf::Vector2f(mouse_pos))) {
                
                if (textbox_in_focus != textbox) {
                    textbox_in_focus = textbox;

                    if (textbox_in_focus->sfml_text.getString() == textbox_in_focus->place_holder_text)
                        textbox_in_focus->sfml_text.setString("");

                    typed_text = textbox_in_focus->sfml_text.getString();
                }
            
            }

        }

        click_clock.restart();

    }


    for (auto textbox : text_boxes) {

        textbox->sfml_text.setFillColor(textbox->color);

        if (textbox->sfml_text.getString().isEmpty() && textbox != textbox_in_focus) {
            textbox->sfml_text.setString(textbox->place_holder_text);
        }

    }


}




void menu::add_sprite(sf::Sprite *sprite)
{
    sprites.push_back(sprite);
}


sf::Text* menu::add_text(sf::Color color,sf::Vector2f window_position,std::string string)
{
    sf::Text* text = new sf::Text();

    text->setFont(font);
    text->setCharacterSize(40);
    text->setFillColor(color);
    text->setStyle(sf::Text::Italic || sf::Text::Bold);
    text->setString(string);

    text->setOrigin(text->getLocalBounds().getSize().x / 2.f, text->getLocalBounds().getSize().y / 2.f);
    text->setPosition(window_position);

    texts.push_back(text);

    return text;
}



sf::Text* menu::add_textbox(sf::Vector2f window_position, sf::Color color, sf::Color on_focus_color, std::string placeholder_text)
{
    struct menu::textbox *t = new struct menu::textbox;

    t->place_holder_text = placeholder_text;
    t->color = color;
    t->on_focus_color = on_focus_color;

    t->sfml_text.setFont(font);
    t->sfml_text.setCharacterSize(40);
    t->sfml_text.setFillColor(color);
    t->sfml_text.setStyle(sf::Text::Italic || sf::Text::Bold);
    t->sfml_text.setString(placeholder_text);

    t->sfml_text.setOrigin(t->sfml_text.getLocalBounds().getSize().x / 2.f, t->sfml_text.getLocalBounds().getSize().y / 2.f);
    t->sfml_text.setPosition(window_position);

    t->rect = sf::RectangleShape();
    t->rect.setFillColor(sf::Color(0, 0, 0, 120));
    t->rect.setSize(sf::Vector2(t->sfml_text.getGlobalBounds().getSize().x*1.f, t->sfml_text.getGlobalBounds().getSize().y * 1.3f));
    t->rect.setPosition(t->sfml_text.getGlobalBounds().left, t->sfml_text.getGlobalBounds().top - t->rect.getGlobalBounds().height + t->sfml_text.getGlobalBounds().height);

    text_boxes.push_back(t);

    render_texture.draw(t->rect);
    render_texture.draw(t->sfml_text);


    return &t->sfml_text;

}





void menu::draw()
{

    render_texture.display();

    // get the target texture (where the stuff has been drawn)
    const sf::Texture& texture = render_texture.getTexture();

    // draw it to the window
    sf::Sprite sprite;
    sprite.setTexture(texture,true);
    sprite.setPosition(window->mapPixelToCoords(sf::Vector2i(0,0)));

    window->draw(sprite);
}




void menu::update()
{
    render_texture.clear(sf::Color::Black);

    // write
    if (textbox_in_focus != nullptr) {
        textbox_in_focus->sfml_text.setString(typed_text);
    }

    check_callback(sf::Mouse::getPosition());

    if (textbox_in_focus != nullptr) {
        textbox_in_focus->sfml_text.setFillColor(textbox_in_focus->on_focus_color);
    }

    for(auto sprite : sprites){

        render_texture.draw(*sprite);
    }

    for(auto button : buttons){

        render_texture.draw(*button->sfml_text);
    }

    for(auto textbox : text_boxes){

        render_texture.draw(textbox->rect);
        render_texture.draw(textbox->sfml_text);
    }

    for (auto text : texts) {
        render_texture.draw(*text);
    }

    draw();

}



void menu::add_music(std::string music_name, float sound_level, bool loop_enabled)
{
    music_sound_buffer = new sf::SoundBuffer();

    std::string directory_path = ASSETS_PATH"sounds/";

    std::string filePath = directory_path + music_name;
    if (!music_sound_buffer->loadFromFile(filePath)){
        std::cerr << "Failed to load sound from: " << filePath << std::endl;
        return;
    }

    music_sound.setBuffer(*music_sound_buffer);
    music_sound.setLoop(loop_enabled);
    music_sound.setVolume(sound_level);

}



void menu::start_music()
{
    if(music_sound.getStatus() != sf::Sound::Playing)
        music_sound.play();
}


void menu::stop_music()
{
    if(music_sound.getStatus() == sf::Sound::Playing)
        music_sound.stop();
}


void menu::disable()
{
    textbox_in_focus = nullptr;
}





void menu::change_view_to_menu()
{
    sf::View view = window->getView();

    float ratio = window->getSize().y/(float)window->getSize().x;

    float view_width = 1920.f;
    float view_height = ratio * view_width;

    view.setCenter(view_width/2.f,view_height/2.f);
    view.setSize(view_width,view_height);

    window->setView(view);
}





void menu::setup_loading_screen(){

    click_clock.restart();

    if (!font.loadFromFile(ASSETS_PATH"ui/fonts/Arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    if (!cursed_font.loadFromFile(ASSETS_PATH"ui/fonts/Exquisite Corpse.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }


    if (!zombie_font.loadFromFile(ASSETS_PATH"ui/fonts/Zombie_Holocaust.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }


    // loading screen
    if(!bullet_texture.loadFromFile(ASSETS_PATH"ui/loading screen/Bullet.png")){
        return;
    }

    if(!magazine_bottom_texture.loadFromFile(ASSETS_PATH"ui/loading screen/Magazine/Bottom.png")){
        return;
    }

    if(!magazine_top_texture.loadFromFile(ASSETS_PATH"ui/loading screen/Magazine/Top.png")){
        return;
    }

    if(!loading_screen_background_texture.loadFromFile(ASSETS_PATH"ui/menus/scary-zombie-hand-ground.jpg")){
        return;
    }


    sf::Image t1 = loading_screen_background_texture.copyToImage();

    for(int i = 0; i < loading_screen_background_texture.getSize().x;i++){

        for(int j = 0; j < loading_screen_background_texture.getSize().y;j++){

            t1.setPixel(i,j,sf::Color(t1.getPixel(i,j).r,t1.getPixel(i,j).g,t1.getPixel(i,j).b, BACKGROUND_ALPHA));

        }
    }

    loading_screen_background_texture.update(t1);
    loading_screen_background_texture.setSmooth(true);
    loading_screen_background.setTexture(loading_screen_background_texture);

    loading_screen_background.setPosition(0,0);


    sf::Image t2 = magazine_bottom_texture.copyToImage();

    for(int i = 0; i < magazine_bottom_texture.getSize().x;i++){

        for(int j = 0; j < magazine_bottom_texture.getSize().y;j++){

            t2.setPixel(i,j,sf::Color(t2.getPixel(i,j).r,t2.getPixel(i,j).g,t2.getPixel(i,j).b, 90));

        }
    }

    magazine_bottom_texture.update(t2);
    magazine_bottom_texture.setSmooth(true);
    magazine_bottom.setTexture(magazine_bottom_texture);

    magazine_bottom.setOrigin(magazine_bottom.getLocalBounds().getSize().x/2.f,magazine_bottom.getLocalBounds().getSize().y/2.f);
    magazine_bottom.setPosition(sf::Vector2f(window->getSize().x/2,window->getSize().y/2.f));


    magazine_top_texture.setSmooth(true);
    magazine_top.setTexture(magazine_top_texture);

    magazine_top.setOrigin(magazine_bottom.getLocalBounds().getSize().x/2.f,magazine_bottom.getLocalBounds().getSize().y/2.f);
    magazine_top.setPosition(magazine_bottom.getPosition().x - 4,magazine_bottom.getPosition().y - 4);

    bullet_texture.setSmooth(true);


    game_title.setFont(zombie_font);
    game_title.setString(GAME_TITLE);
    game_title.setCharacterSize(80);
    game_title.setFillColor(sf::Color(112,121,164,255));

    game_title.setOrigin(game_title.getLocalBounds().getSize().x/2.f,game_title.getLocalBounds().getSize().y/1.5f);
    game_title.setPosition(magazine_bottom.getPosition() - sf::Vector2f(0,300));

}





void menu::step_loading_screen(){

    window->clear();

    sf::Sprite* bul = new sf::Sprite();
    bullets.push_back(bul);

    bul->setTexture(bullet_texture);
    bul->setPosition(20 + magazine_bottom.getGlobalBounds().left + loading_bar_bullets * 44,magazine_bottom.getGlobalBounds().top + 10);

    window->draw(loading_screen_background);
    window->draw(magazine_bottom);

    for(auto bullet : bullets){
        window->draw(*bullet);
    }

    window->draw(magazine_top);
    window->draw(game_title);

    window->display();

    loading_bar_bullets++;
}
