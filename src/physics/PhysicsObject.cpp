//
//  PhysicsObject.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "PhysicsObject.h"
#include "PhysicsWorld.h"
#include "PhysicsUtil.h"
#include "GameObject.h"

bool BoundingBox::testCollision(const BoundingBox& a, const BoundingBox& b, CollisionManifold* m) {
  float nx = a.center.x - b.center.x;
  float ny = a.center.y - b.center.y;
  float nz = a.center.z - b.center.z;
  float ex = a.halfwidths.x + b.halfwidths.x;
  float ey = a.halfwidths.y + b.halfwidths.y;
  float ez = a.halfwidths.z + b.halfwidths.z;
  bool collision = std::fabs(nx) <= ex && std::fabs(ny) <= ey && std::fabs(nz) <= ez;
  
  if (m && collision) {
    float ox = ex - std::fabs(nx);
    float oy = ey - std::fabs(ny);
    float oz = ez - std::fabs(nz);
    if (ox < oy) {
      if (ox < oz) {
        m->penetration = ox;
        m->normal = ofVec3f(nx < 0 ? -1 : 1, 0, 0);
      } else {
        m->penetration = oz;
        m->normal = ofVec3f(0, 0, nz < 0 ? -1 : 1);
      }
    } else {
      if (oy < oz) {
        m->penetration = oy;
        m->normal = ofVec3f(0, ny < 0 ? -1 : 1, 0);
      } else {
        m->penetration = oz;
        m->normal = ofVec3f(0, 0, nz < 0 ? -1 : 1);
      }
    }
  }
  
  return collision;
}


PhysicsObject::PhysicsObject(CollisionShape shape)
: collisionShape(shape)
, world(NULL)
, isCollidable(false)
, sizeDirty(false)
{
  
}

const ofVec3f& PhysicsObject::getPosition() const {
  if (isDynamic()) {
    return trajectory->getPosition();
  } else {
    return staticPosition;
  }
}

void PhysicsObject::setPosition(const ofVec3f& newPosition) {
  if (isDynamic()) {
    trajectory->setPosition(newPosition);
  } else {
    staticPosition = newPosition;
  }
  updateBoundingBox();
}

void PhysicsObject::setPositionCylindrical(float r, float heading, float z) {
  staticRotation = 360 - heading;
  setPosition(cylindricalToCartesian(r, heading, z));
}

void PhysicsObject::setPositionSpherical(float r, float elevation, float heading) {
  staticRotation = 360 - heading;
  setPosition(sphericalToCartesian(r, elevation, heading));
}

float PhysicsObject::getRotation() const {
  if (isDynamic()) {
    return trajectory->getRotation();
  } else {
    return staticRotation;
  }
}

void PhysicsObject::setRotation(float heading) {
  staticRotation = heading;
  updateBoundingBox();
}

void PhysicsObject::setSize(const ofVec3f& newSize) {
  size = newSize;
  sizeDirty = true;
  updateBoundingBox();
}

ofVec3f PhysicsObject::getVelocity() const {
  if (isDynamic()) {
    return trajectory->getInstantaneousVelocity();
  } else {
    return ofVec3f(0, 0, 0);
  }
}

void PhysicsObject::tick() {
  if (isDynamic()) {
    trajectory->tick();
    updateBoundingBox();
  }
  if (world) {
    bool alive = thisGameObject->alive();
    if (alive && !isCollidable) {
      world->addObject(this);
      isCollidable = true;
    } else if (!alive && isCollidable) {
      world->removeObject(this);
      isCollidable = false;
    }
  }
}

void PhysicsObject::updateBoundingBox() {
  boundingBox.center = getPosition();
  
  float rot = getRotation();
  if (rot != 0 && collisionShape != CollisionSphere) {
    float phi = rot * PI/180.0;
    float c = cos(phi);
    float s = sin(phi);
    float hx = size.x / 2.0;
    float hz = size.z / 2.0;
    boundingBox.halfwidths.x = max(abs(hx * c + hz * s), abs(hx * c - hz * s));
    boundingBox.halfwidths.y = size.y / 2.0;
    boundingBox.halfwidths.z = max(abs(hx * s - hz * c), abs(hx * s + hz * c));
  } else {
    boundingBox.halfwidths = size / 2.0;
  }
  
  if (world) {
    world->updateCollisionObject(this);
  }
}

void PhysicsObject::setTrajectory(Trajectory* t) {
  trajectory.reset(t);
  if (world) {
    world->updateCollisionObject(this);
  }
}