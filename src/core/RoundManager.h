//
//  RoundManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__RoundController__
#define __bleepout__RoundController__

#include <ofMain.h>
#include <list>
#include "PlayerManager.h"
#include "BleepoutConfig.h"
#include "GameState.h"
#include "SpaceController.h"
#include "LogicController.h"
#include "GameEvents.h"
#include "Timing.h"
#include "Animations.h"

class RendererBase;

class RoundController
{
public:
  RoundController(RoundConfig config,
                  std::list<ofPtr<Player> > players,
                  PlayerManager& playerManager);
  
  ~RoundController();
  
  void setup();
  void draw();
  void update();
  
  ofEvent<RoundStateEventArgs> roundEndedEvent;
  
  RoundState& state() { return _state; }
  const RoundState& state() const { return _state; }
  
  void dumpToLog(ofLogLevel level) const;
  void dumpConfig(ofLogLevel level) const;
  
  void keyPressed(int key);
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y );
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button);
  
  void setPaddlePosition(GameObjectId playerId, float xPercent);
  
  const RoundConfig& config() const { return _config; }
  
  void addAnimation(ofPtr<AnimationObject> animation);
  void addTimedAction(ofPtr<TimedAction> action);

private:
  void onPlayerYawPitchRoll(PlayerYawPitchRollEventArgs& e);
  void onRoundEnded(RoundStateEventArgs& e);
  void onModifierAppeared(ModifierEventArgs& e);
  void onModifierApplied(ModifierEventArgs& e);
  
  float _startTime;
  PlayerManager& _playerManager;
  RoundConfig _config;
  RoundState _state;
  ofPtr<RendererBase> _renderer;
  ofPtr<SpaceController> _spaceController;
  ofPtr<LogicController> _logicController;
  TimedActionSet _timedActions;
  ofPtr<AnimationManager> _animationManager;
};

#endif /* defined(__bleepout__RoundController__) */
