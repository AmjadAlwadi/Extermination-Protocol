#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

#include "world.h"
#include "contact_listener.h"
#include "constants.h"
#include "menu.h"
#include "game_state_enum.h"
#include "static_variables_definition.h"


sf::Sprite game_cursor;
sf::Texture game_cursor_texture;

sf::Sprite menu_cursor;
sf::Texture menu_cursor_texture;

sf::Vector2i resolution = sf::Vector2i(1920,1080);

World* current_world;

sf::Text* server_ip_text;
sf::Text* joined_players_number;

// color and onhover_color
std::vector<sf::Color> main_menu_color_palette = {sf::Color(255,255,255),sf::Color(180,243,216)};
std::vector<sf::Color> pause_menu_color_palette = {sf::Color(176,176,176),sf::Color(116,91,89)};
std::vector<sf::Color> die_screen_menu_color_palette = {sf::Color(203,236,233),sf::Color(125,186,183)};


// all menus
menu *main_menu;
menu *server_or_client_select_menu;
menu* server_start_screen;
menu *coop_mode_select_menu;
menu *difficulty_select_menu;
menu *pause_menu;
menu *die_screen_menu;

std::vector<menu*> menus;

std::string typed_text;

bool volatile start_command = false;



void set_active_menu(menu* active_menu) {

    for (menu* m : menus) {
        if (m != active_menu) {
            m->disable();
        }
    }

}



// button functions
void exit_game(sf::RenderWindow* window){

    menu::click_clock.restart();
    window->close();

}



void server_start_game(sf::RenderWindow* window) {

    menu::click_clock.restart();
    start_command = true;

}



void to_online_difficulty_select_menu(sf::RenderWindow* window){

    set_active_menu(difficulty_select_menu);

    current_world->coop_mode = false;
    current_world->server = true;

    menu::current_game_state = game_state::DIFFICULTY_SELECT;
    menu::click_clock.restart();

}



void to_solo_coop_difficulty_select_menu(sf::RenderWindow* window) {

    set_active_menu(difficulty_select_menu);

    current_world->number_of_coop_players = 1;
    current_world->coop_mode = true;
    current_world->server = false;

    menu::current_game_state = game_state::DIFFICULTY_SELECT;
    menu::click_clock.restart();
}


void to_duo_coop_difficulty_select_menu(sf::RenderWindow* window) {

    set_active_menu(difficulty_select_menu);

    current_world->number_of_coop_players = 2;
    current_world->coop_mode = true;
    current_world->server = false;

    menu::current_game_state = game_state::DIFFICULTY_SELECT;
    menu::click_clock.restart();

}


void to_trio_coop_difficulty_select_menu(sf::RenderWindow* window) {

    set_active_menu(difficulty_select_menu);

    current_world->number_of_coop_players = 3;
    current_world->coop_mode = true;
    current_world->server = false;


    menu::current_game_state = game_state::DIFFICULTY_SELECT;
    menu::click_clock.restart();

}


void to_server_or_client_select_menu(sf::RenderWindow* window) {

    set_active_menu(server_or_client_select_menu);

    menu::current_game_state = game_state::SERVER_OR_CLIENT_SELECT_MENU;
    menu::click_clock.restart();

}


void to_coop_mode_select_menu(sf::RenderWindow* window) {

    set_active_menu(coop_mode_select_menu);

    menu::current_game_state = game_state::COOP_MODE_SELECT_MENU;
    menu::click_clock.restart();

}



void continue_game(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    pause_menu->stop_music();

    current_world->resume_music();

    menu::current_game_state = game_state::PLAYING;

}



void join_game(sf::RenderWindow* window) {

    set_active_menu(nullptr);

    menu::click_clock.restart();

    current_world->coop_mode = false;
    current_world->server = false;
    current_world->server_ip = sf::IpAddress(server_ip_text->getString());

    main_menu->stop_music();
    die_screen_menu->stop_music();


    menu::current_game_state = game_state::PLAYING;
    current_world->restart();

}





void to_main_menu(sf::RenderWindow* window){

    set_active_menu(main_menu);

    menu::click_clock.restart();

    current_world->number_of_coop_players = 1;
    current_world->coop_mode = true;
    current_world->server = false;

    die_screen_menu->stop_music();
    pause_menu->stop_music();

    menu::current_game_state = game_state::MAIN_MENU;
    main_menu->start_music();

}




extern void to_die_screen_menu(sf::RenderWindow* window){

    set_active_menu(die_screen_menu);

    menu::click_clock.restart();

    current_world->pause_music();

    menu::change_view_to_menu();

    menu::current_game_state = game_state::DIE_SCREEN;
    die_screen_menu->start_music();

}



extern void to_pause_menu(sf::RenderWindow* window){

    set_active_menu(pause_menu);

    menu::click_clock.restart();

    current_world->pause_music();

    menu::change_view_to_menu();

    menu::current_game_state = game_state::PAUSE_MENU;
    pause_menu->start_music();

}


void to_playing(sf::RenderWindow* window) {

    menu::current_game_state = game_state::PLAYING;

    for (auto menu : menus) {
        menu->stop_music();
    }

}




void difficulty_level_1(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 0.f;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }

}



void difficulty_level_2(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 0.2f;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }
}




void difficulty_level_3(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 0.4f;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }
}





void difficulty_level_4(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 0.6f;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }
}


void difficulty_level_5(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 0.8f;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }
}


void difficulty_level_6(sf::RenderWindow* window){

    set_active_menu(nullptr);

    menu::click_clock.restart();

    menu::current_difficulty_level = 1.f;
    current_world->friendly_fire = true;

    main_menu->stop_music();
    die_screen_menu->stop_music();

    if (current_world->coop_mode) {
        menu::current_game_state = game_state::PLAYING;
        current_world->restart();
    }
    else {
        menu::current_game_state = game_state::SERVER_START_SCREEN;
        // start online procedure on another thread
        std::thread restart_thread(&World::restart, current_world);
        restart_thread.detach();
    }
    
}




void setup_cursor(){

    //setup game_cursor
    std::string directory_path = ASSETS_PATH"ui/";

    std::string filePath = directory_path + "kenney_crosshairPack/Tilesheet/crosshairs_tilesheet_white.png";

    if (!game_cursor_texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

    sf::Vector2i game_cursor_size = sf::Vector2i(70,70);

    game_cursor.setTexture(game_cursor_texture,false);
    game_cursor.setTextureRect(sf::IntRect(sf::Vector2i(584,515) - game_cursor_size/2,game_cursor_size));
    game_cursor.setOrigin(game_cursor.getLocalBounds().width / 2, game_cursor.getLocalBounds().height / 2);
    game_cursor.setScale(sf::Vector2f(0.13f,0.13f));



    // setup menu_cursor
    filePath = directory_path + "menus/menu_cursor.png";

    if (!menu_cursor_texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return;
    }

    menu_cursor.setTexture(menu_cursor_texture,true);
    menu_cursor.setOrigin(0,0);
    menu_cursor.setScale(sf::Vector2f(1,1));

}





void show_cursor(sf::RenderWindow* window)
{

    sf::Vector2i mouse_position = sf::Mouse::getPosition(*window);
    sf::Vector2f world_mouse_position = window->mapPixelToCoords(mouse_position);

    if(menu::current_game_state == game_state::PLAYING){

        if (!current_world->players[0]->joystick_player) {
            game_cursor.setPosition(world_mouse_position.x, world_mouse_position.y);
            window->draw(game_cursor);
        }
        

    }else{

        menu_cursor.setPosition(world_mouse_position.x,world_mouse_position.y);
        window->draw(menu_cursor);

    }

    window->display();

}







int main()
{


    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;

    sf::RenderWindow window(sf::VideoMode(resolution.x,resolution.y), GAME_TITLE,sf::Style::Fullscreen,settings);

    window.setFramerateLimit(FRAME_RATE);
    window.setMouseCursorVisible(false);

    menu::window = &window;

    sf::Image* game_icon = new sf::Image();

    if (!game_icon->loadFromFile(ASSETS_PATH"ui/game icon.png"))
        std::cerr << "icon not found" << std::endl;


    window.setIcon(game_icon->getSize().x, game_icon->getSize().y, game_icon->getPixelsPtr());


    menu::setup_loading_screen();

    // initialization

    sf::Clock world_clock;

    setup_cursor();


    b2Vec2 gravity = b2Vec2(0.f, 0.f);

    b2World b2_world(gravity);
    
    current_world = new World(&window,&b2_world);
    
    contact_listener* contact_listener_instacne = new contact_listener();

    b2_world.SetContactListener(contact_listener_instacne);


   
    // main menu
    main_menu = new menu(&window,sf::Vector2i(window.getSize()),ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    main_menu->set_background(ASSETS_PATH"ui/menus/main_menu.png");

    main_menu->create_button(sf::Vector2f(window.getSize().x/2,350),"Play Online",main_menu_color_palette[0],main_menu_color_palette[1],&to_server_or_client_select_menu);
    main_menu->create_button(sf::Vector2f(window.getSize().x/2,550), "Play Co-op", main_menu_color_palette[0], main_menu_color_palette[1], &to_coop_mode_select_menu);
    main_menu->create_button(sf::Vector2f(window.getSize().x/2,750),"Quit Game",main_menu_color_palette[0],main_menu_color_palette[1],&exit_game);

    main_menu->add_music("darksummer.flac",10,true);
    
    menus.push_back(main_menu);

    menu::step_loading_screen();

    // server or client select menu
    server_or_client_select_menu = new menu(&window, sf::Vector2i(window.getSize()), ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    server_or_client_select_menu->set_background(ASSETS_PATH"ui/menus/main_menu.png");

    server_or_client_select_menu->create_button(sf::Vector2f(window.getSize().x / 2, 350), "Start as Server", main_menu_color_palette[0], main_menu_color_palette[1], &to_online_difficulty_select_menu);
    server_or_client_select_menu->create_button(sf::Vector2f(window.getSize().x / 2, 820), "Join a Game", main_menu_color_palette[0], main_menu_color_palette[1], &join_game);
    server_ip_text = server_or_client_select_menu->add_textbox(sf::Vector2f(window.getSize().x / 2, 750), sf::Color(main_menu_color_palette[0].r, main_menu_color_palette[0].g, main_menu_color_palette[0].b,50), main_menu_color_palette[1],"Server IP Address");

    server_or_client_select_menu->create_button(sf::Vector2f(150, 1000), "Back", main_menu_color_palette[0], main_menu_color_palette[1], &to_main_menu);

    menus.push_back(server_or_client_select_menu);



    // server or client select menu
    server_start_screen = new menu(&window, sf::Vector2i(window.getSize()), ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    server_start_screen->set_background(ASSETS_PATH"ui/menus/main_menu.png");

    server_start_screen->create_button(sf::Vector2f(window.getSize().x / 2, 350), "Start The Game", main_menu_color_palette[0], main_menu_color_palette[1], &server_start_game);
    joined_players_number = server_start_screen->add_text(main_menu_color_palette[0], sf::Vector2f(window.getSize().x / 2, 750), "Joined Players : 1");

    menus.push_back(server_start_screen);



    // coop player number select menu
    coop_mode_select_menu = new menu(&window, sf::Vector2i(window.getSize()), ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    coop_mode_select_menu->set_background(ASSETS_PATH"ui/menus/main_menu.png");

    coop_mode_select_menu->create_button(sf::Vector2f(window.getSize().x / 2, 350), "Solo", main_menu_color_palette[0], main_menu_color_palette[1], &to_solo_coop_difficulty_select_menu);
    coop_mode_select_menu->create_button(sf::Vector2f(window.getSize().x / 2, 450), "Duo", main_menu_color_palette[0], main_menu_color_palette[1], &to_duo_coop_difficulty_select_menu);
    coop_mode_select_menu->create_button(sf::Vector2f(window.getSize().x / 2, 550), "Trio", main_menu_color_palette[0], main_menu_color_palette[1], &to_trio_coop_difficulty_select_menu);
    
    coop_mode_select_menu->create_button(sf::Vector2f(150, 1000), "Back", main_menu_color_palette[0], main_menu_color_palette[1], &to_main_menu);

    menus.push_back(coop_mode_select_menu);

    // dificulty select menu
    difficulty_select_menu = new menu(&window,sf::Vector2i(window.getSize()),ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    difficulty_select_menu->set_background(ASSETS_PATH"ui/menus/main_menu.png");


    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,250),"Easy",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_1);
    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,350),"Medium",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_2);
    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,450),"Hard",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_3);
    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,550),"Nightmare",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_4);
    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,650),"Hell",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_5);
    difficulty_select_menu->create_button(sf::Vector2f(window.getSize().x/2,750),"Insane",main_menu_color_palette[0],main_menu_color_palette[1],&difficulty_level_6);
    
    difficulty_select_menu->create_button(sf::Vector2f(150,1000),"Back",main_menu_color_palette[0],main_menu_color_palette[1],&to_main_menu);

    menus.push_back(difficulty_select_menu);

    // game pause menu
    pause_menu = new menu(&window,sf::Vector2i(window.getSize()),ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    pause_menu->set_background(ASSETS_PATH"ui/menus/pause_menu.jpg");

    pause_menu->create_button(sf::Vector2f(window.getSize().x/2,350),"Continue",pause_menu_color_palette[0],pause_menu_color_palette[1],&continue_game);
    pause_menu->create_button(sf::Vector2f(window.getSize().x/2,500),"Back To Mainmenu",pause_menu_color_palette[0],pause_menu_color_palette[1],&to_main_menu);
    pause_menu->create_button(sf::Vector2f(window.getSize().x/2,650),"Quit Game",pause_menu_color_palette[0],pause_menu_color_palette[1],&exit_game);

    pause_menu->add_music("universfield_tension-music-box.flac",1,true);

    menus.push_back(pause_menu);

    menu::step_loading_screen();


    // game die menu
    die_screen_menu = new menu(&window,sf::Vector2i(window.getSize()),ASSETS_PATH"ui/spritesheets/Spritesheet_UI_Flat.png");
    die_screen_menu->set_background(ASSETS_PATH"ui/menus/spooky.jpg");
    die_screen_menu->create_button(sf::Vector2f(window.getSize().x/2,350),"New Game",die_screen_menu_color_palette[0],die_screen_menu_color_palette[1],&to_solo_coop_difficulty_select_menu);
    die_screen_menu->create_button(sf::Vector2f(window.getSize().x/2,650),"Back To Mainmenu",die_screen_menu_color_palette[0],die_screen_menu_color_palette[1],&to_main_menu);

    die_screen_menu->add_music("timouse1.flac",10,true);

    menus.push_back(die_screen_menu);

    sf::Time elapsed_time = world_clock.restart();
    float last_time = 0;

    main_menu->start_music();
    menu::current_game_state = game_state::MAIN_MENU;


    // GAMELOOP
    while (window.isOpen())
    {

        current_world->server_ip = sf::IpAddress(server_ip_text->getString());

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::TextEntered) {

                // Check for printable characters and backspace
                if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                    typed_text += static_cast<char>(event.text.unicode);
                }

                else if (event.text.unicode == 8 && !typed_text.empty()) { // Backspace
                    typed_text.pop_back();
                }

            }

        }
        

        switch(menu::current_game_state){

        case game_state::MAIN_MENU:

            main_menu->update();
            break;

        case game_state::SERVER_OR_CLIENT_SELECT_MENU:

            server_or_client_select_menu->update();
            break;


        case game_state::SERVER_START_SCREEN:

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menu::current_game_state = game_state::MAIN_MENU;

            server_start_screen->update();
            break;


        case game_state::DIFFICULTY_SELECT:

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menu::current_game_state = game_state::MAIN_MENU;

            difficulty_select_menu->update();
            break;


        case game_state::PLAYING:

            elapsed_time = world_clock.restart();
            current_world->update(elapsed_time);
            break;


        case game_state::PAUSE_MENU:

            pause_menu->update();
            break;


        case game_state::DIE_SCREEN:

            die_screen_menu->update();
            break;

        case game_state::COOP_MODE_SELECT_MENU:

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                menu::current_game_state = game_state::MAIN_MENU;

            coop_mode_select_menu->update();
            break;

        }

        show_cursor(&window);

    }


    // clean up resources
    delete main_menu;
    delete pause_menu;
    delete die_screen_menu;
    delete difficulty_select_menu;
    delete server_or_client_select_menu;
    delete coop_mode_select_menu;
    delete current_world;
    delete game_icon;

    return 0;
}

