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
}

void OrbitalTrajectory::initWithTwoPoints(float elevation1, float heading1, float elevation2, float heading2) {
    initWithTwoPoints(sphericalToCartesian(_r, elevation1, heading1), sphericalToCartesian(_r, elevation2, heading2));
}

void OrbitalTrajectory::setPosition(const ofVec3f& position) {
    initWithTwoPoints(position, _v);
}
    
void OrbitalTrajectory::tick() {
    _t += _speed;
    _position = _r * (_u * cos(_t) + _w * sin(_t));
}

void OrbitalTrajectory::reflect(const ofVec3f& planeNormal) {
    // mostly works for +y normal paddle bounces, ignoring planeNormal for now
    // sometimes trajectory is correct but ball motion is reversed so it seems to go through paddle.
    ofVec3f orbitNormal = _u.crossed(_v);
    orbitNormal.y *= -1;
    ofVec3f start = getPosition();
    ofVec3f through = _v.crossed(orbitNormal);
    initWithTwoPoints(start, through);
}