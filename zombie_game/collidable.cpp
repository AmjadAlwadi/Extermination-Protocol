#include "collidable.h"

collidable::collidable() {}

void collidable::take_damage(bullet* bullet)
{
    this->alive = false;
}

void collidable::take_damage()
{
    this->alive = false;
}

void collidable::die()
{
    this->alive = false;
}