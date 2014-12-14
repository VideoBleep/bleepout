//
//  GameObject.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__GameObject__
#define __bleepout__GameObject__

#include <iostream>
#include <ofColor.h>

enum GameObjectType {
  GAME_OBJECT_OTHER,
  GAME_OBJECT_BRICK,
  GAME_OBJECT_PADDLE,
  GAME_OBJECT_BALL,
  GAME_OBJECT_PLAYER,
  GAME_OBJECT_WALL,
  GAME_OBJECT_ANIMATION
};
typedef int GameObjectId;

class GameObject {
public:
  GameObject(GameObjectType t);
  virtual ~GameObject() {}
  
  GameObjectType type() const { return _type; }
  GameObjectId id() const { return _id; }
  bool alive() const { return _alive; }
  
  void kill() { _alive = false; }
  void revive() { _alive = true; }
    
  virtual const ofColor& getColor() const { return _color; }
  virtual void setColor(const ofColor& color) { _color = color; }
  
  virtual void output(std::ostream& os) const = 0;
private:
  GameObjectId _id;
  GameObjectType _type;
  bool _alive;
protected:
  ofColor _color;
};

#endif /* defined(__bleepout__GameObject__) */
