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
  
  BrickSpec& setElevation(float e) { elevation = e; return *this; }
  BrickSpec& setHeading(float h) { heading = h; return *this; }
  BrickSpec& setValue(int v) { value = v; return *this; }
  BrickSpec& setColor(ofColor c) { color = c; return *this; }
  BrickSpec& setLives(int l) { lives = l; return *this; }
  BrickSpec& setSpeed(float s) { speed = s; return *this; }
  BrickSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  BrickSpec& setModifier(std::string m) { modifierName = m; return *this; }
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
  
  BrickRingSpec() : value(1), lives(1), phase(0), speed(0), stopHeading(-1) { }
  BrickRingSpec(float e, ofColor c, int n, int v, int l, float p = 0, float s = 0, float stop = -1)
  : elevation(e), count(n), phase(p), color(c), value(v), lives(l), speed(s), stopHeading(-1) { }
  BrickRingSpec& setElevation(float e) { elevation = e; return *this; }
  BrickRingSpec& setCount(int c) { count = c; return *this; }
  BrickRingSpec& setValue(int v) { value = v; return *this; }
  BrickRingSpec& setColor(ofColor c) { color = c; return *this; }
  BrickRingSpec& setLives(int l) { lives = l; return *this; }
  BrickRingSpec& setSpeed(float s) { speed = s; return *this; }
  BrickRingSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  BrickRingSpec& setPhase(float p) { phase = p; return *this; }
};

struct WallSpec {
  float elevation;
  float heading;
  ofVec3f size;
  bool isExit;
  float speed;
  float stopHeading;
  
  WallSpec() : speed(0), stopHeading(-1), isExit(false) { }
  WallSpec(float e, float h, ofVec3f s, bool exit = false, float sp = 0, float stop = -1)
  : elevation(e), heading(h), size(s), isExit(exit), speed(sp), stopHeading(stop) { }
  WallSpec& setElevation(float e) { elevation = e; return *this; }
  WallSpec& setHeading(float h) { heading = h; return *this; }
  WallSpec& setSize(ofVec3f s) { size = s; return *this; }
  WallSpec& setIsExit(bool e) { isExit = e; return *this; }
  WallSpec& setSpeed(float s) { speed = s; return *this; }
  WallSpec& setStopHeading(float s) { stopHeading = s; return *this; }
};

struct WallRingSpec {
  float elevation;
  ofVec3f size;
  bool isExit;
  int count;
  float phase;
  float speed;
  float stopHeading;
  WallRingSpec() : speed(0), count(1), isExit(false), phase(0), stopHeading(-1) { }
  WallRingSpec& setElevation(float e) { elevation = e; return *this; }
  WallRingSpec& setCount(int c) { count = c; return *this; }
  WallRingSpec& setSize(ofVec3f s) { size = s; return *this; }
  WallRingSpec& setIsExit(bool e) { isExit = e; return *this; }
  WallRingSpec& setSpeed(float s) { speed = s; return *this; }
  WallRingSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  WallRingSpec& setPhase(float p) { phase = p; return *this; }
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
  
  CurvedWallSpec() : speed(0), stopHeading(-1), isExit(false) { }
  CurvedWallSpec(float e1, float h1, float e2, float h2, float w, bool exit = false, float sp = 0, float stop = -1)
  : elevation1(e1), heading1(h1), elevation2(e2), heading2(h2), width(w), isExit(exit), speed(sp), stopHeading(stop) { }
  CurvedWallSpec& setEnd1(float e, float h) { elevation1 = e; heading1 = h; return *this; }
  CurvedWallSpec& setEnd2(float e, float h) { elevation2 = e; heading2 = h; return *this; }
  CurvedWallSpec& setWidth(float w) { width = w; return *this; }
  CurvedWallSpec& setIsExit(float e) { isExit = e; return *this; }
  CurvedWallSpec& setSpeed(float s) { speed = s; return *this; }
  CurvedWallSpec& setStopHeading(float s) { stopHeading = s; return *this; }
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
  std::string name;
  float amount;
  float duration;
  ofColor color;
  
  ModifierSpec() : type(MODIFIER_NONE) { }
  ModifierSpec(std::string n, ModifierType t)
  : type(t), name(n), amount(0), duration(0) { }
  ModifierSpec& setAmount(float amt) { amount = amt; return *this; }
  ModifierSpec& setDuration(float dur) { duration = dur; return *this; }
  ModifierSpec& setColor(ofColor c) { color = c; return *this; }
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
  MessageSpec(std::string txt, ofColor c)
  : text(txt), color(c), trails(0) { }
  MessageSpec& setSize(float s) { size = s; return *this; }
  MessageSpec& setTrails(int t) { trails = t; return *this; }
  MessageSpec& setTiming(float del, float dur) {
    delay = del;
    duration = dur;
    return *this;
  }
};

template<typename T>
struct ValuePulserSpec {
  float minRate;
  float maxRate;
  float changeInterval;
  T startValue;
  ValuePulserSpec() { }
  ValuePulserSpec(float min, float max, float interval, T start)
  : minRate(min), maxRate(max), changeInterval(interval), startValue(start) { }
};

struct RingSetSpec {
  ValuePulserSpec<ofVec3f> spin;
  ValuePulserSpec<ofVec3f> spread;
  ofVec3f spreadOffset;
  int count;
  float radiusScale;
  float lineWidth;
  ofColor color;
  
  RingSetSpec& setSpin(ValuePulserSpec<ofVec3f> pulser) {
    spin = pulser;
    return *this;
  }
  RingSetSpec& setSpread(ValuePulserSpec<ofVec3f> pulser, ofVec3f offset) {
    spread = pulser;
    spreadOffset = offset;
    return *this;
  }
  RingSetSpec& setCount(int c) { count = c; return *this; }
  RingSetSpec& setRadiusScale(float scale) { radiusScale = scale; return *this;  }
  RingSetSpec& setLineWidth(float width) { lineWidth = width; return *this; }
  RingSetSpec& setColor(ofColor col) { color = col; return *this; }
};

#endif /* defined(__bleepout__ObjectSpecs__) */
