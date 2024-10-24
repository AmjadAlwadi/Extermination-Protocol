#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GAME_TITLE "Extermination Protocol"

/*  Whispers in the Ruins
    Echoes from the Wasteland
    Left 4 Lunch
    Extermination Protocol
    The Last Remnant
*/

#define CAMERA_WIDTH 350.f

#define FRAME_RATE 165.f

#define CORRECTION_ANGLE_FOR_BULLET -0.4f // in degree to mouse cursor

#define ZOMBIES_NUMBER_FIRST_ROUND 30
#define ZOMBIES_NUMBER_INCREASE_PER_ROUND 30
#define ZOMBIES_SPAWN_RATE 1
#define ZOMBIE_SPAWN_TIME 2.f

#define SPEED 2.5f

#define PLAYER_DAMAGE_PER_HIT 5.f
#define ZOMBIE_DAMAGE_PER_HIT 20.f

#define BACKGROUND_ALPHA 120

#define SHADOW_STRENGTH 0.17f
#define TERRAIN_SHADOW_STRENGTH 0.07f

#define MAX_NUMBER_OF_ZOMBIES_ALIVE 200

#define SHADOW_DISTANCE_FACTOR 4.f
#define WORLD_TILE_SHADOW_DISTANCE_FACTOR 8.f

#define TERRAIN_RANDOM_ROTATION 40

#define DELTA_STRAFING_ANGLE 30

#define ZOMBIE_SPEED SPEED/1.3f

#define FRAME_TIME 0.041f

#define DIAGONALFACTOR  0.7071f

#define PORT 6753

#define EPS 0.01f

#define BULLET_SPEED SPEED*8.f

#define KILLS_FOR_FIRST_LEVEL 10

// difficulty as float point from 4 to 3
#define DIFFICULTY_LEVEL 3.f

#define NUMBER_OF_ZOMBIES_PASSED_THROUG_FIRST_LEVEL 1
#define NUMBER_OF_ZOMBIES_PASSED_THROUG_PER_LEVEL_INCREASE 2

#define PLAYER_SCALE 0.04f
#define ZOMBIE_SCALE PLAYER_SCALE * 1.05f
#define TERRAIN_SCALE PLAYER_SCALE * 3.f

#define ONLINE_SEND_RATE 0.01f  // in seconds <==> 100 times in a second
#define SERVER_PLAYERS_JOIN_TIME 3.f
#define START_GAME_DELAY 10.f


#define EPS 0.1f

// pixels per meter. Box2D uses metric units, so we need to define a conversion
#define PPM 40.0F

// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F
#define DEG_PER_RAD_INVERSE 0.0174532925f
#define M_PI 3.14159265358979323846


#endif // CONSTANTS_H
