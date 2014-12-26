//
//  PhysicsUtil.cpp
//  bleepout
//
//  Created by Dewb on 12/11/14.
//
//

#include "PhysicsUtil.h"


ofVec3f cylindricalToCartesian(float r, float heading, float z) {
  ofVec3f p;
  float phi = heading * PI/180.0;
  p.x = r * cos(phi);
  p.y = z;
  p.z = r * sin(phi);
  return p;
}

ofVec3f sphericalToCartesian(float r, float elevation, float heading) {
  ofVec3f p;
  float theta = PI/2.0 - elevation * PI/180.0;
  float phi = heading * PI/180.0;
  float r_sin_theta = r * sin(theta);
  p.x = r_sin_theta * cos(phi);
  p.y = r * cos(theta);
  p.z = r_sin_theta * sin(phi);
  return p;
}

void cartesianToSpherical(const ofVec3f& xyz, float *pElevation, float* pHeading, float* pRadius) {
  if (!pElevation || !pHeading) {
    return;
  }
  
  float r = xyz.length();
  float phi = fmod(atan2(xyz.z, xyz.x) + 2 * PI, 2 * PI);
  float theta = acos(xyz.y / r);
  *pHeading = phi * 180/PI;
  *pElevation = (PI/2.0 - theta) * 180/PI;
  
  if (pRadius) {
    *pRadius = r;
  }
}
