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
, trajectory(NULL)
{
    
}

void PhysicsObject::setPosition(const ofVec3f& newPosition) {
    position = newPosition;
    updateBoundingBox();
}

void PhysicsObject::setPositionCylindrical(float theta, float r, float z) {
    position.x = r * cos(theta);
    position.y = z;
    position.z = r * sin(theta);
    rotation = 2 * PI - theta;
    updateBoundingBox();
}

void PhysicsObject::setPositionSpherical(float theta, float phi, float r) {
    float r_sin_theta = r * sin(theta);
    position.x = r_sin_theta * cos(phi),
    position.y = r_sin_theta * sin(phi),
    position.z = r * cos(theta);
    rotation = 2 * PI - theta;
    updateBoundingBox();
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

void PhysicsObject::updateBoundingBox() {
    boundingBox.center = position;
    
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