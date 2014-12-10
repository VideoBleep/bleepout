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

class Wall : public GameObject, public PhysicsObject {
public:
  Wall(bool isExit) : GameObject(GAME_OBJECT_WALL), _isExit(isExit) {}
  
  bool isExit() const { return _isExit; }
  
  static const char* typeName() { return "wall"; }
private:
  bool _isExit;
};


#endif /* defined(__bleepout__Wall__) */
