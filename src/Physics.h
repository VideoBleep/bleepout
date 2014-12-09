//
//  Physics.h
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#ifndef __bleepout__Physics__
#define __bleepout__Physics__

#include <ofEvents.h>
#include "GameObject.h"

class CollisionArgs : public ofEventArgs {
public:
    GameObject* a;
    GameObject* b;
};

class Physics {
public:
    void update();
    void notifyCollision(GameObject* a, GameObject* b);
    
    static ofEvent<CollisionArgs> collisionEvent;
};

#endif /* defined(__bleepout__Physics__) */
