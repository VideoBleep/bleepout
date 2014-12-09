//
//  Physics.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "Physics.h"

void Physics::notifyCollision(GameObject* a, GameObject* b) {
    static CollisionArgs args;
    args.a = a;
    args.b = b;
    ofNotifyEvent(collisionEvent, args, this);
}

ofEvent<CollisionArgs> Physics::collisionEvent;