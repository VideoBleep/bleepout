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
  BoundingBox() : center(ofVec3f::zero()), halfwidths(ofVec3f::zero()) {}
  BoundingBox(const BoundingBox& other) { center = other.center; halfwidths = other.halfwidths; }
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
  
  ofVec3f getVelocity() const;
  
  bool isDynamic() const { return trajectory != NULL; }
  virtual void tick();
  
  Trajectory* getTrajectory() { return trajectory.get(); }
  void setTrajectory(Trajectory* t);
  
  PhysicsWorld* getWorld() { return world; }
  
  virtual void output(std::ostream& os) const;
  
protected:
  void updateBoundingBox();
  
  ofVec3f staticPosition;
  float staticRotation;
  ofVec3f size;
  BoundingBox boundingBox;
  
  CollisionShape collisionShape;
  
  ofPtr<Trajectory> trajectory;
  PhysicsWorld* world;
  GameObject* thisGameObject;
  bool isCollidable;
  
  friend class GameObject;
  friend class PhysicsWorld;
  friend class PhysicsImpl;
  friend void outputPhysicsObjectFields(std::ostream& os, const PhysicsObject& obj);
};

#endif /* defined(__bleepout__PhysicsObject__) */
