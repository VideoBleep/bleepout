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
    
    void calculateFromVectors(ofVec3f start, ofVec3f through);
    
    void tick();
    
    float getRadius() const { return _r; }
    void setRadius(float r) { _r = r; }
    
    float getSpeed() const { return _speed; }
    void setSpeed(float s) { _speed = s; }
    
    const ofVec3f& getPosition() const { return _position; }
    
    void reflect(const ofVec3f& planeNormal);
    
    void output(std::ostream& os) const;

    const ofVec3f& u() const { return _u; }
    const ofVec3f& v() const { return _v; }
    const ofVec3f& w() const { return _w; }
    
protected:
    float _r;
    
    ofVec3f _u;
    ofVec3f _v;
    ofVec3f _w;
    float _t;
    float _speed;
    
    ofVec3f _position;
};

#endif /* defined(__bleepout__OrbitalTrajectory__) */
