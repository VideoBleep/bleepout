//
//  Trajectory.h
//  bleepout
//
//  Created by Dewb on 12/16/14.
//
//

#ifndef __bleepout__Trajectory__
#define __bleepout__Trajectory__

#include <ofTypes.h>

class Trajectory {
public:
  Trajectory();
  
  virtual float getSpeed() const { return _speed; }
  virtual void setSpeed(float s) { _speed = s; }
  
  virtual const ofVec3f& getPosition() const { return _position; }
  virtual void setPosition(const ofVec3f& position) = 0;
  
  virtual float getRotation() const { return 0; }
  
  virtual float getTime() const { return _t; }
  virtual void setTime(float t) { _t = t; }
  
  virtual void reflect(const ofVec3f& planeNormal, float trueHitFactor = 0.0) = 0;
  
  virtual void output(std::ostream& os) const = 0;
  
  virtual ofVec3f getInstantaneousVelocity() const { return _position - _lastPosition; }
  
protected:
  friend class PhysicsObject;
  
  virtual void tick() = 0;
  
  float _t;
  float _speed;
  
  ofVec3f _position;
  ofVec3f _lastPosition;
};


#endif /* defined(__bleepout__Trajectory__) */
