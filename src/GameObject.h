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

struct GameObjectKey {
  const GameObjectType type;
  const GameObjectId id;
  GameObjectKey(GameObjectType t, GameObjectId i) : type(t), id(i) {}
};

class GameObject {
public:
  GameObject(GameObjectType t);
  
  GameObjectType type() const { return _key.type; }
  GameObjectId id() const { return _key.id; }
  const GameObjectKey& key() const { return _key; }
  GameObjectKey& key() { return _key; }
  bool alive() const { return _alive; }
  
  void kill() { _alive = false; }
  void revive() { _alive = true; }
private:
  GameObjectKey _key;
  bool _alive;
};

#endif /* defined(__bleepout__GameObject__) */
