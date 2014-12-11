//
//  OrbitalTrajectory.cpp
//  bleepout
//
//  Created by Dewb on 12/9/14.
//
//

#include "OrbitalTrajectory.h"


OrbitalTrajectory::OrbitalTrajectory(float radius, ofVec3f start, ofVec3f through, float speed) {
    _speed = speed;
    _r = radius;
    calculateFromVectors(start, through);
}

void OrbitalTrajectory::calculateFromVectors(ofVec3f start, ofVec3f through) {
    _u = start.normalized();
    _v = through.normalized();
    _w = _u.getCrossed(_v).getCrossed(_u).normalized();
    _t = 0;
    _position = _u * _r;
}
    
void OrbitalTrajectory::tick() {
    _t += _speed;
    _position = _r * (_u * cos(_t) + _w * sin(_t));
}

void OrbitalTrajectory::reflect(const ofVec3f& planeNormal) {
    ofVec3f orbitNormal = _u.crossed(_v);
    orbitNormal.y *= -1;
    ofVec3f start = getPosition();
    ofVec3f through = _v.crossed(orbitNormal);
    _speed *= -1;
    calculateFromVectors(start, through);
}