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

CircularTrajectory::CircularTrajectory(float domeRadius, float speed, float elevation, float startHeading, float stopHeading) {
    _speed = speed;
    _r = domeRadius;
    initWithElevationHeading(elevation, startHeading, stopHeading);
}

void CircularTrajectory::initWithElevationHeading(float elevation, float startHeading, float stopHeading) {
    _elevation = elevation;
    _startHeading = startHeading;
    _stopHeading = stopHeading;
    
    _position = sphericalToCartesian(_r, elevation, _startHeading);
    _phi = _startHeading * PI / 180.0;
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

    float heading = (_phi + _t) * 180.0 / PI;
    if (_stopHeading > 0 &&
        ((_startHeading < _stopHeading && (heading < _startHeading || heading > _stopHeading)) ||
         (_startHeading > _stopHeading && (heading > _startHeading || heading < _stopHeading)))) {
        initWithElevationHeading(_elevation, _stopHeading, _startHeading);
        return;
    }
    
    _lastPosition = _position;
    _t += _startHeading < _stopHeading ? _speed : -_speed;
    _position.x = _circleRadius * cos(_phi + _t);
    _position.z = _circleRadius * sin(_phi + _t);
    

}

void CircularTrajectory::reflect(const ofVec3f& planeNormal, float trueHitFactor /* = 0.0 */) {
    _speed = -_speed;
}