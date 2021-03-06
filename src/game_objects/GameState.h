//
//  GameState.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__GameState__
#define __bleepout__GameState__

#include <iostream>
#include <list>

#include "GameObject.h"
#include "Paddle.H"
#include "Brick.h"
#include "Ball.h"
#include "Player.h"
#include "Wall.h"
#include "Modifier.h"
#include "AnimationObject.h"
#include "GameObjectCollection.h"

class RoundState {
public:
  
  RoundState(std::list<ofPtr<Player> >& players);
  ~RoundState();
  
  void initialize(ofPtr<RoundConfig> config);
  
  const GameObjectCollection<Paddle>& paddles() const { return _paddles; }
  const GameObjectCollection<Ball>& balls() const { return _balls; }
  const GameObjectCollection<Brick>& bricks() const { return _bricks; }
  const GameObjectCollection<Wall>& walls() const { return _walls; }
  const GameObjectCollection<Player>& players() const { return _players; }
  const GameObjectCollection<Modifier>& modifiers() const { return _modifiers; }
  const GameObjectCollection<AnimationObject>& animations() const { return _animations; }
  
  GameObjectCollection<Paddle>& paddles() { return _paddles; }
  GameObjectCollection<Ball>& balls() { return _balls; }
  GameObjectCollection<Brick>& bricks() { return _bricks; }
  GameObjectCollection<Wall>& walls() { return _walls; }
  GameObjectCollection<Player>& players() { return _players; }
  GameObjectCollection<Modifier>& modifiers() { return _modifiers; }
  GameObjectCollection<AnimationObject>& animations() { return _animations; }
  
  const RoundConfig& config() const { return *_config; }
  
  bool hasConfig() const { return !!_config; }
  
  Paddle& addPaddle(Player* player);
  Brick& addBrick(const BrickSpec& brickSpec);
  Wall& addWall(const WallSpec& wallSpec);
  Ball& addBall(const BallSpec& ballSpec);
  void addModifier(ofPtr<Modifier> modifier);
  void addAnimation(ofPtr<AnimationObject> animation);
  
  void decrementLiveBricks() { _liveBricks--; }
  void decrementLiveBalls() { _liveBalls--; }
  
  int liveBricks() const { return _liveBricks; }
  int totalBricks() const { return _totalBricks; }
  
  int liveBalls() const { return _liveBalls; }
  int totalBalls() const { return _totalBalls; }
  
  void output(std::ostream& os) const;
  
  float time;
  float endTime;
  
  float remainingTime() const {
    if (endTime < 0)
      return -1;
    if (time >= endTime)
      return 0;
    return endTime - time;
  }
  
private:
  void addPlayer(ofPtr<Player> player);
  
  RoundState(const RoundState& other) : _config(other._config) {
    ofLogWarning() << "DUPLICATING ROUND STATE!!!";
  }
  ofPtr<RoundConfig> _config;
  GameObjectCollection<Paddle> _paddles;
  GameObjectCollection<Ball>   _balls;
  GameObjectCollection<Brick>  _bricks;
  GameObjectCollection<Wall>   _walls;
  GameObjectCollection<Player> _players;
  GameObjectCollection<Modifier> _modifiers;
  GameObjectCollection<AnimationObject> _animations;
  int _liveBricks;
  int _totalBricks;
  int _liveBalls;
  int _totalBalls;
};

#endif /* defined(__bleepout__GameState__) */
