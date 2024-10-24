#ifndef COLLIDABLE_H
#define COLLIDABLE_H

class bullet;
class world_tile;

class collidable
{
public:
    collidable();

    enum types {BULLET_TYPE,TILE_TYPE,PLAYER_TYPE,ZOMBIE_TYPE};

    bool alive = true;
    types type;

    virtual void take_damage(bullet* bullet);
    virtual void take_damage();
    virtual void die();

};

#endif // COLLIDABLE_H
