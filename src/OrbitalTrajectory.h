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

class OrbitalTrajectory {
public:
    OrbitalTrajectory();
    OrbitalTrajectory(float radius, ofVec3f u, ofVec3f v, float speed = 0.02);
    
    void tick();
    
    float getRadius() const { return _r; }
    void setRadius(float r) { _r = r; }
    
    float getSpeed() const { return _speed; }
    void setSpeed(float s) { _speed = s; }
    
    void setSquishFactor(float s) { _squishFactor = s; }
    
    ofVec3f getPosition() const { return _position; }
    
protected:
    float _r;
    float _squishFactor;
    
    ofVec3f _u;
    ofVec3f _w;
    float _t;
    float _speed;
    
    ofVec3f _position;
};

#endif /* defined(__bleepout__OrbitalTrajectory__) */
