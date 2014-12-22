//
//  Wall.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__Wall__
#define __bleepout__Wall__

#include "GameObject.h"
#include "PhysicsObject.h"

class RoundConfig;
struct WallSpec;

class Wall : public GameObject, public PhysicsObject {
public:
  Wall(const RoundConfig& config, const WallSpec& spec);
  
  bool isExit() const { return _isExit; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }

  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "wall"; }
private:
  bool _isExit;
  bool _visible;
};

template<>
struct GameObjectTypeTraits<Wall> {
  static const GameObjectType typeId = GAME_OBJECT_WALL;
  static const char typeName[];
};


#endif /* defined(__bleepout__Wall__) */
