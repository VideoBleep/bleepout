//
//  PhysicsObject.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "PhysicsObject.h"
#include "OrbitalTrajectory.h"
#include "PhysicsWorld.h"

bool BoundingBox::testCollision(const BoundingBox& a, const BoundingBox& b) {
    bool x = std::fabs(a.center.x - b.center.x) <= (a.halfwidths.x + b.halfwidths.x);
    bool y = std::fabs(a.center.y - b.center.y) <= (a.halfwidths.y + b.halfwidths.y);
    bool z = std::fabs(a.center.z - b.center.z) <= (a.halfwidths.z + b.halfwidths.z);
    return x && y && z;
}

PhysicsObject::PhysicsObject(CollisionShape shape)
: collisionShape(shape)
, world(NULL)
{
    
}

void PhysicsObject::setPosition(const ofVec3f& newPosition) {
    position = newPosition;
    updateBoundingBox();
}

void PhysicsObject::setSize(const ofVec3f& newSize) {
    size = newSize;
    updateBoundingBox();
}

void PhysicsObject::setVelocity(const ofVec3f& v) {
    velocity = v;
}

void PhysicsObject::updateBoundingBox() {
    boundingBox.center = position;
    boundingBox.halfwidths = size / 2.0;
    if (world) {
        world->updateCollisionObject(this);
    }
}