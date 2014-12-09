//
//  PhysicsObject.h
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#ifndef __bleepout__PhysicsObject__
#define __bleepout__PhysicsObject__

#include <ofMain.h>


struct BoundingBox {
    ofVec3f min;
    ofVec3f max;
};

class PhysicsObject {
public:
    PhysicsObject() {}
    
    const ofVec3f& getPosition() const;
    void setPosition(const ofVec3f& newPosition);
    
    const ofVec3f& getDimensions() const;
    void setDimensions(const ofVec3f& newDimensions);
    
    const BoundingBox& getBoundingBox() const;

    const ofVec3f& getVelocity() const;
    void setVelocity(const ofVec3f& v);
    
    virtual void update() {}
    
protected:
    void updateBoundingBox();
    
    ofVec3f position;
    ofVec3f dimensions;
    BoundingBox boundingBox;
    
    ofVec3f velocity;
};

#endif /* defined(__bleepout__PhysicsObject__) */
