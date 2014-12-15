//
//  RendererUtil.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "RendererUtil.h"
#include "PhysicsUtil.h"
#include <ofMain.h>

void drawText(string text, ofColor color, ofTrueTypeFont& font, float size, float radius, float elevation, float heading) {
  ofPushMatrix();
  ofPushStyle();
  
  ofVec3f pos = sphericalToCartesian(radius, elevation, heading);
  ofTranslate(pos);
  ofRotateY(360 - heading - 90);
  ofRotateX(elevation);
  
  float scale = size/(font.getSize() * 1.0);
  ofScale(scale, scale);
  
  ofRectangle rect = font.getStringBoundingBox(text, 0, 0);
  ofTranslate(-rect.width/2, -rect.height/2, 0);
  
  ofSetColor(color);
  font.drawStringAsShapes(text, 0, 0);
  
  ofPopStyle();
  ofPopMatrix();
}
