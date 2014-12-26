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
  
  history.push(_position);
}

void OrbitalTrajectory::reflect(const ofVec3f& planeNormal, float trueHitFactor /* = 0.0 */) {
  ofVec3f vel = getInstantaneousVelocity();
  float d = vel.dot(planeNormal);
  ofVec3f reflectedVelocity = vel - 2 * d * planeNormal;
  if (trueHitFactor > 0) {
    ofVec3f trueReflection = vel.length() * -planeNormal;
    reflectedVelocity.interpolate(trueReflection, trueHitFactor);
  }
  initWithTwoPoints(_position, _position + reflectedVelocity);
}