#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2d/box2d.h>



class contact_listener : public b2ContactListener
{
public:
    contact_listener();

    virtual void BeginContact(b2Contact *contact);
};

#endif // CONTACT_LISTENER_H
