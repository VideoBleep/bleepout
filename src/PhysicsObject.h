//
//  PhysicsObject.h
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#ifndef __bleepout__PhysicsObject__
#define __bleepout__PhysicsObject__

#include <ofMain.h>

class OrbitalTrajectory;
class PhysicsWorld;

struct BoundingBox {
    ofVec3f min;
    ofVec3f max;
};

enum CollisionShape {
    CollisionSphere,
    CollisionBox
};

class PhysicsObject {
public:
    PhysicsObject(CollisionShape shape = CollisionBox);
    
    const ofVec3f& getPosition() const { return position; }
    void setPosition(const ofVec3f& newPosition);
    
    const ofVec3f& getSize() const { return size; }
    void setSize(const ofVec3f& newSize);
    
    const BoundingBox& getBoundingBox() const { return boundingBox; }

    const ofVec3f& getVelocity() const { return velocity; }
    void setVelocity(const ofVec3f& v);
    
    bool isDynamic() const { return trajectory != NULL; }
    
    
protected:
    void updateBoundingBox();
    
    ofVec3f position;
    ofVec3f size;
    BoundingBox boundingBox;
    
    ofVec3f velocity;
    CollisionShape collisionShape;

    OrbitalTrajectory* trajectory;
    PhysicsWorld* world;
    
    friend class PhysicsWorld;
    friend class PhysicsImpl;
    friend void outputPhysicsObjectFields(std::ostream& os, const PhysicsObject& obj);
};

#endif /* defined(__bleepout__PhysicsObject__) */
