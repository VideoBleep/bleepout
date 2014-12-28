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
#include <vector>
#include "Common.h"

class RoundConfig;

template<typename T>
struct SpecGroupBuilder {
  virtual void buildSpecs(const RoundConfig& config,
                          std::vector<T>* specs) const = 0;
};

template<typename Group, typename T>
void buildAllSpecs(const RoundConfig& config,
                   const std::vector<Group>& groups,
                   std::vector<T>* specs) {
  for (const auto& group : groups) {
    group.buildSpecs(config, specs);
  }
}

struct BrickSpec {
  float elevation;
  float heading;
  ofVec3f size;
  int value;
  int lives;
  ofColor color;
  float speed;
  float stopHeading;
  std::string modifierName;
  
  BrickSpec() : speed(0), stopHeading(-1), modifierName(), size(7.0f, 5.0f, 17.0f) { }
  BrickSpec& copyFrom(const BrickSpec& other) {
    elevation = other.elevation;
    heading = other.heading;
    size = other.size;
    value = other.value;
    lives = other.lives;
    color = other.color;
    speed = other.speed;
    stopHeading = other.stopHeading;
    modifierName = other.modifierName;
    return *this;
  }
  
  BrickSpec& setElevation(float e) { elevation = e; return *this; }
  BrickSpec& setHeading(float h) { heading = h; return *this; }
  BrickSpec& setSize(ofVec3f s) { size = s; return *this; }
  BrickSpec& setValue(int v) { value = v; return *this; }
  BrickSpec& setColor(ofColor c) { color = c; return *this; }
  BrickSpec& setLives(int l) { lives = l; return *this; }
  BrickSpec& setSpeed(float s) { speed = s; return *this; }
  BrickSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  BrickSpec& setModifier(std::string m) { modifierName = m; return *this; }
};

struct BrickRingSpec : public SpecGroupBuilder<BrickSpec> {
  float elevation;
  ofVec3f size;
  ofColor color;
  int value;
  int lives;
  int count;
  float phase;
  float speed;
  float stopHeading;
  std::string modifierName;
  float modifierChance;
  
  BrickRingSpec() : value(1), lives(1), phase(0), speed(0), stopHeading(-1), size(7.0f, 5.0f, 17.0f) { }
  BrickRingSpec& copyFrom(const BrickRingSpec& other) {
    elevation = other.elevation;
    size = other.size;
    color = other.color;
    value = other.value;
    lives = other.lives;
    count = other.count;
    phase = other.phase;
    speed = other.speed;
    stopHeading = other.stopHeading;
    modifierName = other.modifierName;
    modifierChance = other.modifierChance;
    return *this;
  }
  BrickRingSpec& setElevation(float e) { elevation = e; return *this; }
  BrickRingSpec& setSize(ofVec3f s) { size = s; return *this; }
  BrickRingSpec& setCount(int c) { count = c; return *this; }
  BrickRingSpec& setValue(int v) { value = v; return *this; }
  BrickRingSpec& setColor(ofColor c) { color = c; return *this; }
  BrickRingSpec& setLives(int l) { lives = l; return *this; }
  BrickRingSpec& setSpeed(float s) { speed = s; return *this; }
  BrickRingSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  BrickRingSpec& setPhase(float p) { phase = p; return *this; }
  BrickRingSpec& setModifier(std::string mod, float chance) {
    modifierName = mod;
    modifierChance = chance;
    return *this;
  }
  void buildSpecs(const RoundConfig& config,
                  std::vector<BrickSpec>* specs) const override;
};

struct BrickQuadsSpec : public SpecGroupBuilder<BrickSpec> {
  ofColor color1;
  ofColor color2;
  float elevation;
  int count;
  float elevationSpacing;
  float headingSpacing;
  ofVec3f size;
  float speed;
  float stopHeading;
  std::string modifierName;
  float modifierChance;
  
  BrickQuadsSpec() : speed(0), size(7, 5, 17) { }
  BrickQuadsSpec& copyFrom(const BrickQuadsSpec& other) {
    color1 = other.color1;
    color2 = other.color2;
    elevation = other.elevation;
    count = other.count;
    elevationSpacing = other.elevationSpacing;
    headingSpacing = other.headingSpacing;
    size = other.size;
    speed = other.speed;
    stopHeading = other.stopHeading;
    modifierName = other.modifierName;
    modifierChance = other.modifierChance;
    return *this;
  }
  BrickQuadsSpec& setColor(ofColor c1, ofColor c2) {
    color1 = c1;
    color2 = c2;
    return *this;
  }
  BrickQuadsSpec& setElevation(float e, float spacing) {
    elevation = e;
    elevationSpacing = spacing;
    return *this;
  }
  BrickQuadsSpec& setHeadingSpacing(float spacing) {
    headingSpacing = spacing;
    return *this;
  }
  BrickQuadsSpec& setSize(ofVec3f s) {
    size = s;
    return *this;
  }
  BrickQuadsSpec& setSpeed(float s) {
    speed = s;
    return *this;
  }
  BrickQuadsSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  BrickQuadsSpec& setCount(int c) {
    count = c;
    return *this;
  }
  BrickQuadsSpec& setModifier(std::string mod, float chance) {
    modifierName = mod;
    modifierChance = chance;
    return *this;
  }
  void buildSpecs(const RoundConfig& config,
                  std::vector<BrickSpec>* specs) const override;
};

struct WallSpec {
  float elevation;
  float heading;
  ofVec3f size;
  bool isExit;
  float speed;
  float stopHeading;
  bool visible;
  
  WallSpec() : speed(0), stopHeading(-1), isExit(false), visible(true) { }
  WallSpec& copyFrom(const WallSpec& other) {
    elevation = other.elevation;
    heading = other.heading;
    size = other.size;
    isExit = other.isExit;
    speed = other.speed;
    stopHeading = other.stopHeading;
    visible = other.visible;
    return *this;
  }
  WallSpec& setElevation(float e) { elevation = e; return *this; }
  WallSpec& setHeading(float h) { heading = h; return *this; }
  WallSpec& setSize(ofVec3f s) { size = s; return *this; }
  WallSpec& setIsExit(bool e) { isExit = e; return *this; }
  WallSpec& setVisible(bool v) { visible = v; return *this; }
  WallSpec& setSpeed(float s) { speed = s; return *this; }
  WallSpec& setStopHeading(float s) { stopHeading = s; return *this; }
};

struct WallRingSpec : public SpecGroupBuilder<WallSpec> {
  float elevation;
  ofVec3f size;
  bool isExit;
  bool visible;
  int count;
  float phase;
  float speed;
  float stopHeading;
  WallRingSpec() : speed(0), count(1), isExit(false), phase(0), stopHeading(-1), visible(true) { }
  WallRingSpec& copyFrom(const WallRingSpec& other) {
    elevation = other.elevation;
    size = other.size;
    isExit = other.isExit;
    visible = other.visible;
    count = other.count;
    phase = other.phase;
    speed = other.speed;
    stopHeading = other.stopHeading;
    return *this;
  }
  WallRingSpec& setElevation(float e) { elevation = e; return *this; }
  WallRingSpec& setCount(int c) { count = c; return *this; }
  WallRingSpec& setSize(ofVec3f s) { size = s; return *this; }
  WallRingSpec& setIsExit(bool e) { isExit = e; return *this; }
  WallRingSpec& setVisible(bool v) { visible = v; return *this; }
  WallRingSpec& setSpeed(float s) { speed = s; return *this; }
  WallRingSpec& setStopHeading(float s) { stopHeading = s; return *this; }
  WallRingSpec& setPhase(float p) { phase = p; return *this; }
  void buildSpecs(const RoundConfig& config,
                  std::vector<WallSpec>* specs) const override;
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
