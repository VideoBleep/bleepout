//
//  Brick.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Brick__
#define __bleepout__Brick__

#include "GameObject.h"
#include "PhysicsObject.h"
#include <ofMain.h>

class RoundConfig;
class BrickSpec;

class Brick : public GameObject, public PhysicsObject {
public:
  Brick(const RoundConfig* config = NULL, const BrickSpec* spec = NULL);
  
  int value() const { return _value; }
  void setValue(int value) { _value = value; }
  
  void output(std::ostream& os) const override;
  
private:
  int _value;
};

template<>
struct GameObjectTypeTraits<Brick> {
  static const GameObjectType typeId = GAME_OBJECT_BRICK;
  static const char typeName[];
};

#endif /* defined(__bleepout__Brick__) */
