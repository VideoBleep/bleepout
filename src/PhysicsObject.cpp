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
#include "PhysicsUtil.h"

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

const ofVec3f& PhysicsObject::getPosition() const {
    if (trajectory) {
        return trajectory->getPosition();
    } else {
        return position;
    }
}

void PhysicsObject::setPosition(const ofVec3f& newPosition) {
    if (trajectory) {
        trajectory->setPosition(newPosition);
    } else {
        position = newPosition;
    }
    updateBoundingBox();
}

void PhysicsObject::setPositionCylindrical(float r, float theta, float z) {
    rotation = 2 * PI - theta;
    setPosition(cylindricalToCartesian(r, theta, z));
}

void PhysicsObject::setPositionSpherical(float r, float theta, float phi) {
    rotation = 2 * PI - phi;
    setPosition(sphericalToCartesian(r, theta, phi));
}

void PhysicsObject::setRotation(float theta) {
    rotation = theta;
    updateBoundingBox();
}

void PhysicsObject::setSize(const ofVec3f& newSize) {
    size = newSize;
    updateBoundingBox();
}

void PhysicsObject::setVelocity(const ofVec3f& v) {
    velocity = v;
}

void PhysicsObject::tick() {
    if (trajectory) {
        trajectory->tick();
        updateBoundingBox();
    }
}

void PhysicsObject::updateBoundingBox() {
    boundingBox.center = getPosition();
    
    float c = cos(rotation);
    float s = sin(rotation);
    float hx = size.x / 2.0;
    float hz = size.z / 2.0;
        
    boundingBox.halfwidths.x = max(abs(hx * c + hz * s), abs(hx * c - hz * s));
    boundingBox.halfwidths.y = size.y / 2.0;
    boundingBox.halfwidths.z = max(abs(hx * s - hz * c), abs(hx * s + hz * c));
    
    if (world) {
        world->updateCollisionObject(this);
    }
}