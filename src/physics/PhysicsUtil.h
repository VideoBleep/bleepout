//
//  PhysicsUtil.h
//  bleepout
//
//  Created by Dewb on 12/11/14.
//
//

#ifndef __bleepout__PhysicsUtil__
#define __bleepout__PhysicsUtil__

#include <ofMain.h>

ofVec3f cylindricalToCartesian(float r, float heading, float z);
ofVec3f sphericalToCartesian(float r, float elevation, float heading);

void cartesianToSpherical(const ofVec3f& xyz, float *pElevation, float* pHeading, float* pRadius = NULL);

#endif /* defined(__bleepout__PhysicsUtil__) */
