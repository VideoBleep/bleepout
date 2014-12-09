//
//  PhysicsObject.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "PhysicsObject.h"

const ofVec3f& PhysicsObject::getPosition() const {
    return position;
}

void PhysicsObject::setPosition(const ofVec3f& newPosition) {
    position = newPosition;
    updateBoundingBox();
}

const ofVec3f& PhysicsObject::getDimensions() const {
    return dimensions;
}

void PhysicsObject::setDimensions(const ofVec3f& newDimensions) {
    dimensions = newDimensions;
    updateBoundingBox();
}

const ofVec3f& PhysicsObject::getVelocity() const {
    return velocity;
}

void PhysicsObject::setVelocity(const ofVec3f& v) {
    velocity = v;
}

const BoundingBox& PhysicsObject::getBoundingBox() const {
    return boundingBox;
}

void PhysicsObject::updateBoundingBox() {
    boundingBox.min = position - dimensions / 2.0;
    boundingBox.max = position + dimensions / 2.0;
}