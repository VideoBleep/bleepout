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

void PhysicsObject::setPositionCylindrical(float r, float phi, float z) {
    rotation = 2 * PI - phi;
    setPosition(cylindricalToCartesian(r, phi, z));
}

void PhysicsObject::setPositionSpherical(float r, float theta, float phi) {
    rotation = 2 * PI - phi;
    setPosition(sphericalToCartesian(r, theta, phi));
}

void PhysicsObject::setRotation(float phi) {
    rotation = phi;
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
    if (isDynamic()) {
        trajectory->tick();
        updateBoundingBox();
    }
}

void PhysicsObject::updateBoundingBox() {
    boundingBox.center = getPosition();
    
    if (rotation != 0 && collisionShape != CollisionSphere) {
        float c = cos(rotation);
        float s = sin(rotation);
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