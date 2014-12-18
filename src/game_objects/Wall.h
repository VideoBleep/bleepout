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
  Wall(const RoundConfig* config = NULL, const WallSpec* spec = NULL);
  
  bool isExit() const { return _isExit; }
  
  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "wall"; }
private:
  bool _isExit;
};


#endif /* defined(__bleepout__Wall__) */
