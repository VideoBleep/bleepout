//
//  PhysicsWorld.h
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#ifndef __bleepout__PhysicsWorld__
#define __bleepout__PhysicsWorld__

#include <ofMain.h>

class GameObject;
class PhysicsObject;

class CollisionArgs : public ofEventArgs {
public:
    GameObject* a;
    GameObject* b;
};

class PhysicsImpl;

class PhysicsWorld {
public:    
    void setup();
    
    void addObject(PhysicsObject* obj);
    void removeObject(PhysicsObject* obj);
    
    void updateCollisionObject(PhysicsObject* obj);
    
    void update();
    
    void notifyCollision(GameObject* a, GameObject* b);
    
    static ofEvent<CollisionArgs> collisionEvent;
    
protected:
    ofPtr<PhysicsImpl> _impl;
};

#endif /* defined(__bleepout__PhysicsWorld__) */
