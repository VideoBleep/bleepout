//
//  OrbitalTrajectory.cpp
//  bleepout
//
//  Created by Dewb on 12/9/14.
//
//

#include "OrbitalTrajectory.h"
#include "PhysicsUtil.h"

OrbitalTrajectory::OrbitalTrajectory()
: _r(1.0)
, _speed(0.02)
, _t(0)
{
}

OrbitalTrajectory::OrbitalTrajectory(float radius, ofVec3f start, ofVec3f through, float speed) {
    _speed = speed;
    _r = radius;
    initWithTwoPoints(start, through);
}

void OrbitalTrajectory::initWithTwoPoints(ofVec3f start, ofVec3f through) {
    _u = start.normalized();
    _v = through.normalized();
    _w = _u.getCrossed(_v).getCrossed(_u).normalized();
    _t = 0;
    _position = _u * _r;
    _lastPosition = _r * (_u * cos(-_speed) + _w * sin(-_speed));
}

void OrbitalTrajectory::initWithTwoPoints(float elevation1, float heading1, float elevation2, float heading2) {
    initWithTwoPoints(sphericalToCartesian(_r, elevation1, heading1), sphericalToCartesian(_r, elevation2, heading2));
}

void OrbitalTrajectory::setPosition(const ofVec3f& position) {
    initWithTwoPoints(position, _v);
}
    
void OrbitalTrajectory::tick() {
    _lastPosition = _position;
    _t += _speed;
    _position = _r * (_u * cos(_t) + _w * sin(_t));
}

void OrbitalTrajectory::reflect(const ofVec3f& planeNormal) {
    // solution 3
    // reflect the orbit plane and calculate a new V from the reflected plane.
    // not perfect, but close.
    // sometimes trajectory ring is correct but direction is reversed so the ball seems to go through the paddle.

//    ofVec3f orbitNormal = _u.getCrossed(_v);
//    float d = orbitNormal.dot(planeNormal);
//    orbitNormal -= 2 * d * planeNormal;
//    
//    ofVec3f newv = (_v * cos(_t) + _w * sin(_t)); // using this instead of _v seems to reduce number of reversed cases
//
//    ofVec3f start = getPosition();
//    ofVec3f through = newv.crossed(orbitNormal);
//    initWithTwoPoints(start, through);

    // solution 4
    // just reflect V
    // faster, still often reversed but more predictably so, but likely not correct for non-horizontal bounces
    
//    float d = _v.dot(planeNormal);
//    initWithTwoPoints(getPosition(), _v - 2 * d * planeNormal);

    // solution 5
    // reflect instantaneous velocity
    
    ofVec3f vel = getInstantaneousVelocity();
    float d = vel.dot(planeNormal);
    ofVec3f reflectedVelocity = vel - 2 * d * planeNormal;
    initWithTwoPoints(_position, _position + reflectedVelocity);
}