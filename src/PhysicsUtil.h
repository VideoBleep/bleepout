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

ofVec3f cylindricalToCartesian(float theta, float r, float z);
ofVec3f sphericalToCartesian(float theta, float phi, float r);

#endif /* defined(__bleepout__PhysicsUtil__) */
