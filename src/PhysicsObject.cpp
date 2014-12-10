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
    boundingBox.min = position - size / 2.0;
    boundingBox.max = position + size / 2.0;
    if (world) {
        world->updateCollisionObject(this);
    }
}