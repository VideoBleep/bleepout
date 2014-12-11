//
//  OrbitalTrajectory.cpp
//  bleepout
//
//  Created by Dewb on 12/9/14.
//
//

#include "OrbitalTrajectory.h"


OrbitalTrajectory::OrbitalTrajectory(float radius, ofVec3f u, ofVec3f v, float speed) {
    _u = u.normalized();
    _v = v.normalized();
    _w = _u.getCrossed(v.normalized()).getCrossed(_u).normalized();
    _speed = speed;
    _t = 0;
    _r = radius;
    _position = _u * radius;
    _squishFactor = 1.0;
}
    
void OrbitalTrajectory::tick() {
    _t += _speed;
    _position = _r * (_u * cos(_t) + _w * sin(_t));
    _position.y *= _squishFactor;
}

void OrbitalTrajectory::reflect(const ofVec3f& planeNormal) {
    _u = getPosition().normalized();
    _w.y *= -1;
    _t = 0;
    //_u.normalize();
    //_w.normalize();
}