//
//  ObjectSpecs.h
//  bleepout
//
//  Created by tekt on 12/17/14.
//
//

#ifndef __bleepout__ObjectSpecs__
#define __bleepout__ObjectSpecs__

#include <string>
#include <map>
#include "Common.h"

typedef std::map<std::string, std::string> StringMap;

struct BrickSpec {
  float elevation;
  float heading;
  int value;
  int lives;
  ofColor color;
  float speed;
  float stopHeading;
  std::string modifierName;
  
  BrickSpec() : speed(0), stopHeading(-1), modifierName() { }
  BrickSpec(float e, float h, ofColor c, int v,
            int l, float s = 0, float stop = -1, std::string mod = "")
  : elevation(e), heading(h), color(c), value(v)
  , lives(l), speed(s), stopHeading(stop), modifierName(mod) { }
};

struct BrickRingSpec {
  float elevation;
  ofColor color;
  int value;
  int lives;
  int count;
  float phase;
  float speed;
  float stopHeading;
  
  BrickRingSpec() : speed(0), stopHeading(-1) { }
  BrickRingSpec(float e, ofColor c, int n, int v, int l, float p = 0, float s = 0, float stop = -1)
  : elevation(e), count(n), phase(p), color(c), value(v), lives(l), speed(s), stopHeading(-1) { }
};

struct WallSpec {
  float elevation;
  float heading;
  ofVec3f size;
  bool isExit;
  float speed;
  float stopHeading;
  
  WallSpec() : speed(0), stopHeading(-1) { }
  WallSpec(float e, float h, ofVec3f s, bool exit = false, float sp = 0, float stop = -1)
  : elevation(e), heading(h), size(s), isExit(exit), speed(sp), stopHeading(stop) { }
};

struct CurvedWallSpec {
  float elevation1;
  float heading1;
  float elevation2;
  float heading2;
  float width;
  bool isExit;
  float speed;
  float stopHeading;
  
  CurvedWallSpec() : speed(0), stopHeading(-1) { }
  CurvedWallSpec(float e1, float h1, float e2, float h2, float w, bool exit = false, float sp = 0, float stop = -1)
  : elevation1(e1), heading1(h1), elevation2(e2), heading2(h2), width(w), isExit(exit), speed(sp), stopHeading(stop) { }
};

struct BallSpec {
  float elevation;
  float heading;
  
  BallSpec() { }
  BallSpec(float e, float h)
  : elevation(e), heading(h) { }
};

struct ModifierSpec {
  ModifierType type;
  StringMap properties;
  
  bool getProperty(const std::string& name, float* result) const {
    auto iter = properties.find(name);
    if (iter == properties.end())
      return false;
    *result = ofFromString<float>(iter->second);
    return true;
  }
  
  ModifierSpec() : type(MODIFIER_NONE), properties() { }
  ModifierSpec(ModifierType t) : type(t), properties() { }
  ModifierSpec(ModifierType t, StringMap p)
  : type(t), properties(p) { }
};

struct MessageSpec {
  std::string text;
  ofColor color;
  float size;
  int trails;
  float delay;
  float duration;
  
  MessageSpec() {}
  MessageSpec(std::string txt, ofColor c, float s, int trl, float del, float dur)
  : text(txt), color(c), size(s), trails(trl)
  , delay(del), duration(dur) { }
};

#endif /* defined(__bleepout__ObjectSpecs__) */
