//
//  PhysicsUtil.cpp
//  bleepout
//
//  Created by Dewb on 12/11/14.
//
//

#include "PhysicsUtil.h"


ofVec3f cylindricalToCartesian(float r, float theta, float z) {
    ofVec3f p;
    p.x = r * cos(theta);
    p.y = z;
    p.z = r * sin(theta);
    return p;
}

ofVec3f sphericalToCartesian(float r, float theta, float phi) {
    ofVec3f p;
    float r_sin_theta = r * sin(theta);
    p.x = r_sin_theta * cos(phi);
    p.y = r * cos(theta);
    p.z = r_sin_theta * sin(phi);
    return p;
}