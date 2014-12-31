//
//  RendererUtil.h
//  bleepout
//
//  Created by tekt on 12/20/14.
//
//

#ifndef __bleepout__RendererUtil__
#define __bleepout__RendererUtil__

#include <ofTypes.h>
#include "PhysicsObject.h"
#include "PhysicsUtil.h"
#ifdef RADOME
#include "radomePlugin.h"
#else
class LineWidthAdjuster {
public:
  static void setLineWidth(float f) { ofSetLineWidth(f); }
  static float factor;
};
#endif

void drawBoxObject(PhysicsObject& object, ofColor edgeColor, ofColor fillColor, ofMaterial* pMat = NULL, float lineWidth = 1.5, bool alphaBlending = false);

void drawText(const std::string& text, ofColor color, ofTrueTypeFont& font, float size, float radius, float elevation, float heading);

void drawTetrahedron(const ofVec3f& origin, float radius);

#endif /* defined(__bleepout__RendererUtil__) */
