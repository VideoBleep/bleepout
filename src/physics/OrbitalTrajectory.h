//
//  OrbitalTrajectory.h
//  bleepout
//
//  Created by Dewb on 12/9/14.
//
//

#ifndef __bleepout__OrbitalTrajectory__
#define __bleepout__OrbitalTrajectory__

#include <ofMain.h>
#include <iostream>

class OrbitalTrajectory {
public:
    OrbitalTrajectory();
    OrbitalTrajectory(float radius, ofVec3f start, ofVec3f through, float speed = 0.02);
    
    void initWithTwoPoints(ofVec3f start, ofVec3f through);
    void initWithTwoPoints(float elevation1, float heading1, float elevation2, float heading2);
    
    float getRadius() const { return _r; }
    void setRadius(float r) { _r = r; }
    
    float getSpeed() const { return _speed; }
    void setSpeed(float s) { _speed = s; }
    
    const ofVec3f& getPosition() const { return _position; }
    void setPosition(const ofVec3f& position);
    
    float getTime() const { return _t; }
    
    void reflect(const ofVec3f& planeNormal);
    
    void output(std::ostream& os) const;

    const ofVec3f& u() const { return _u; }
    const ofVec3f& v() const { return _v; }
    const ofVec3f& w() const { return _w; }
    
    ofVec3f getInstantaneousVelocity() const { return _position - _lastPosition; }
    
protected:
    friend class PhysicsObject;
    void tick();

    float _r;
    
    ofVec3f _u;
    ofVec3f _v;
    ofVec3f _w;
    float _t;
    float _speed;
    
    ofVec3f _position;
    ofVec3f _lastPosition;
};

#endif /* defined(__bleepout__OrbitalTrajectory__) */
