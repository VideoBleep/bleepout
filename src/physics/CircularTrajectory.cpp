//
//  CircularTrajectory.cpp
//  bleepout
//
//  Created by Dewb on 12/16/14.
//
//

#include "CircularTrajectory.h"
#include "PhysicsUtil.h"

CircularTrajectory::CircularTrajectory()
: _r(0)
, _phi(0)
, _circleRadius(0)
, _height(0)
{
}

CircularTrajectory::CircularTrajectory(float domeRadius, float elevation, float heading, float speed) {
    _speed = speed;
    _r = domeRadius;
    initWithElevationHeading(elevation, heading);
}

void CircularTrajectory::initWithElevationHeading(float elevation, float heading) {
    _position = sphericalToCartesian(_r, elevation, heading);
    _phi = heading * PI / 180.0;
    _height = _position.y;
    _circleRadius = sqrt(_position.x * _position.x + _position.z * _position.z);
    _t = 0;
    _lastPosition.x = cos(_phi - _speed);
    _lastPosition.y = _height;
    _lastPosition.z = sin(_phi - _speed);
}

void CircularTrajectory::setPosition(const ofVec3f& position) {
    float e, h;
    cartesianToSpherical(position, &e, &h);
    initWithElevationHeading(e, h);
}

float CircularTrajectory::getRotation() const {
    return 360 - ((_phi + _t) * 180.0 / PI);
}

void CircularTrajectory::tick() {
    _lastPosition = _position;
    _t += _speed;
    _position.x = _circleRadius * cos(_phi + _t);
    _position.z = _circleRadius * sin(_phi + _t);
}

void CircularTrajectory::reflect(const ofVec3f& planeNormal) {
    _speed = -_speed;
}