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

ofVec3f cylindricalToCartesian(float heading, float r, float z);
ofVec3f sphericalToCartesian(float elevation, float heading, float r);

#endif /* defined(__bleepout__PhysicsUtil__) */
