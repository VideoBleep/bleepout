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
#include <Trajectory.h>

class PhysicsWorld;
class GameObject;

struct CollisionManifold {
    ofVec3f normal;
    float penetration;
};

struct BoundingBox {
    ofVec3f center;
    ofVec3f halfwidths;
    static bool testCollision(const BoundingBox& a, const BoundingBox& b, CollisionManifold* manifold = NULL);
};

enum CollisionShape {
    CollisionSphere,
    CollisionBox
};

class PhysicsObject {
public:
    PhysicsObject(CollisionShape shape = CollisionBox);
    
    const ofVec3f& getPosition() const;
    void setPosition(const ofVec3f& newPosition);
    
    float getRotation() const;
    void setRotation(float heading);
    
    void setPositionCylindrical(float r, float heading, float z);
    void setPositionSpherical(float r, float elevation, float heading);
    
    const ofVec3f& getSize() const { return size; }
    void setSize(const ofVec3f& newSize);
    
    const BoundingBox& getBoundingBox() const { return boundingBox; }

    const ofVec3f& getVelocity() const { return velocity; }
    void setVelocity(const ofVec3f& v);
    
    bool isDynamic() const { return trajectory != NULL; }
    void tick();
    
    Trajectory* getTrajectory() { return trajectory.get(); }
    void setTrajectory(Trajectory* t) { trajectory.reset(t); }
    
protected:
    void updateBoundingBox();
    
    ofVec3f staticPosition;
    float staticRotation;
    ofVec3f size;
    BoundingBox boundingBox;
    
    ofVec3f velocity;
    CollisionShape collisionShape;

    ofPtr<Trajectory> trajectory;
    PhysicsWorld* world;
    GameObject* thisGameObject;
    
    friend class GameObject;
    friend class PhysicsWorld;
    friend class PhysicsImpl;
    friend void outputPhysicsObjectFields(std::ostream& os, const PhysicsObject& obj);
};

#endif /* defined(__bleepout__PhysicsObject__) */
