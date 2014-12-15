//
//  RendererUtil.h
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#ifndef __bleepout__RendererUtil__
#define __bleepout__RendererUtil__

#include <ofTypes.h>
#include <string>

class ofTrueTypeFont;

void drawText(std::string text, ofColor color, ofTrueTypeFont& font, float size, float radius, float elevation, float heading);

#endif /* defined(__bleepout__RendererUtil__) */
