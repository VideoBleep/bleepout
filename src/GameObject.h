//
//  GameObject.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__GameObject__
#define __bleepout__GameObject__

enum GameObjectType {
  GAME_OBJECT_BRICK,
  GAME_OBJECT_PADDLE,
  GAME_OBJECT_BALL,
  GAME_OBJECT_PLAYER
};
typedef int GameObjectId;

class GameObject {
public:
  GameObject(GameObjectType t);
  
  GameObjectType type() const { return _type; }
  GameObjectId id() const { return _id; }
  bool alive() const { return _alive; }
  
  void kill() { _alive = false; }
  void revive() { _alive = true; }
private:
  const GameObjectType _type;
  const GameObjectId _id;
  bool _alive;
};

#endif /* defined(__bleepout__GameObject__) */
