cmake_minimum_required(VERSION 3.5)

project(Extermination_Protocol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the external library as a subdirectory
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include/tmxlite")

# Include the external library as a subdirectory
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include//Box2d")


# Find SFML. If it's not in a standard path, set SFML_DIR to the cmake folder of your SFML installation
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/SFML/lib/cmake/SFML")
find_package(SFML COMPONENTS audio graphics window system REQUIRED)


add_executable(Extermination_Protocol main.cpp
    player.h player.cpp
    player_state_enum.h
    world.h world.cpp
    weapons_enum.h
    feet_enum.h
    bullet.h bullet.cpp
    zombie.h zombie.cpp
    zombie_state_enum.h
    constants.h
    contact_listener.h contact_listener.cpp
    collidable.h collidable.cpp
    world_tile.h world_tile.cpp
    terrain_object.h terrain_object.cpp
    menu.h menu.cpp
    game_state_enum.h
    packet_type.h
    packet.h packet.cpp
    vfx.h vfx.cpp
    static_variables_definition.h
    SFMLOrthogonalLayer.hpp
)



# Link SFML to your executable. Adjust the components based on your needs
target_link_libraries(Extermination_Protocol PRIVATE box2d tmxlite sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

#target_compile_definitions(Extermination_Protocol PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_compile_definitions(Extermination_Protocol PUBLIC ASSETS_PATH="./assets/")


include(GNUInstallDirs)
install(TARGETS Extermination_Protocol
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


