 #include "contact_listener.h"
#include "collidable.h"
#include "bullet.h"

contact_listener::contact_listener() {}

void contact_listener::BeginContact(b2Contact *contact)
{
    auto world_object_a = reinterpret_cast<collidable*>(contact->GetFixtureA()->GetUserData().pointer);
    auto world_object_b = reinterpret_cast<collidable*>(contact->GetFixtureB()->GetUserData().pointer);


    if(world_object_a->type == collidable::BULLET_TYPE){

        // player's bullet hits himself for whatever reason
        if(world_object_b->type == collidable::PLAYER_TYPE && reinterpret_cast<bullet*>(world_object_a)->owner == reinterpret_cast<Player*>(world_object_b)){
            return;
        }


        switch (world_object_b->type) {
        
        // bullet dies
        case collidable::TILE_TYPE:
            world_object_a->alive = false;
            break;

        case collidable::PLAYER_TYPE:
            if (World::friendly_fire) 
                world_object_b->take_damage(reinterpret_cast<bullet*>(world_object_a));

            // if it's handgun's bullet then it goes through every zombie otherwise it disappears
            if (reinterpret_cast<bullet*>(world_object_a)->weapon != weapons::handgun) {
                world_object_a->die();
            }
            
            break;
        
        case collidable::ZOMBIE_TYPE:
            world_object_b->take_damage(reinterpret_cast<bullet*>(world_object_a));

            // if it's handgun's bullet then it goes through every zombie otherwise it disappears
            if (reinterpret_cast<bullet*>(world_object_a)->weapon != weapons::handgun) {
                world_object_a->die();
            }

            break;

        case collidable::BULLET_TYPE:
            break;

        default:
            break;

        }



    }else if(world_object_b->type == collidable::BULLET_TYPE){

        // player's bullet hits himself for whatever reason
        if (world_object_a->type == collidable::PLAYER_TYPE && reinterpret_cast<bullet*>(world_object_b)->owner == reinterpret_cast<Player*>(world_object_a)) {
            return;
        }


        switch (world_object_a->type) {

            // bullet dies
        case collidable::TILE_TYPE:
            world_object_b->alive = false;
            break;

        case collidable::PLAYER_TYPE:
            if (World::friendly_fire)
                world_object_a->take_damage(reinterpret_cast<bullet*>(world_object_b));

            // if it's handgun's bullet then it goes through every zombie otherwise it disappears
            if (reinterpret_cast<bullet*>(world_object_b)->weapon != weapons::handgun) {
                world_object_b->die();
            }
            break;

        case collidable::ZOMBIE_TYPE:

            world_object_a->take_damage(reinterpret_cast<bullet*>(world_object_b));

            world_object_b->die();


            break;

        case collidable::BULLET_TYPE:
            break;

        default:
            break;

        }
        
    }


}
