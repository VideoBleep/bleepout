//
//  RendererUtil.cpp
//  bleepout
//
//  Created by tekt on 12/20/14.
//
//

#include "RendererUtil.h"

void drawBoxObject(PhysicsObject& object, ofColor edgeColor, ofColor fillColor, ofMaterial* pMat /*= NULL*/, float lineWidth /*= 1.5*/, bool alphaBlending /*= false*/) {
  ofPushMatrix();
  ofPushStyle();
  
  if (alphaBlending)
    ofEnableAlphaBlending();
  else
    ofDisableAlphaBlending();
  
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofVec3f dims = object.getSize();
  ofTranslate(object.getPosition());
  ofRotateY(object.getRotation());
  
  // Edge lines
  ofNoFill();
  ofDisableLighting();
  ofSetLineWidth(lineWidth);
  ofSetColor(edgeColor);
  ofDrawBox(ofVec3f::zero(), dims.x + 0.1, dims.y + 0.1, dims.z + 0.1);
  
  // Faces
  ofEnableLighting();
  ofFill();
  if (pMat) {
    pMat->begin();
  } else {
    ofSetColor(fillColor);
  }
  ofDrawBox(ofVec3f::zero(), dims.x, dims.y, dims.z);
  if (pMat) {
    pMat->end();
  }
  
  ofPopStyle();
  ofPopMatrix();
}

void drawText(const std::string& text, ofColor color, ofTrueTypeFont& font, float size, float radius, float elevation, float heading) {
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

void drawTetrahedron(const ofVec3f& origin, float radius) {
  
  ofVec3f a(origin.x + radius, origin.y + radius, origin.z + radius);
  ofVec3f b(origin.x - radius, origin.y - radius, origin.z + radius);
  ofVec3f c(origin.x - radius, origin.y + radius, origin.z - radius);
  ofVec3f d(origin.x + radius, origin.y - radius, origin.z - radius);
  
  if (ofGetFill()) {
    glBegin(GL_TRIANGLES);
  } else {
    glBegin(GL_LINE_LOOP);
  }
  
  glVertex3fv(a.getPtr());
  glVertex3fv(b.getPtr());
  glVertex3fv(c.getPtr());

  if (!ofGetFill()) {
    glEnd();
    glBegin(GL_LINE_LOOP);
  }
  
  glVertex3fv(a.getPtr());
  glVertex3fv(c.getPtr());
  glVertex3fv(d.getPtr());

  if (!ofGetFill()) {
    glEnd();
    glBegin(GL_LINE_LOOP);
  }
  
  glVertex3fv(a.getPtr());
  glVertex3fv(d.getPtr());
  glVertex3fv(b.getPtr());

  if (!ofGetFill()) {
    glEnd();
    glBegin(GL_LINE_LOOP);
  }
  
  glVertex3fv(b.getPtr());
  glVertex3fv(d.getPtr());
  glVertex3fv(c.getPtr());

  glEnd();
}
