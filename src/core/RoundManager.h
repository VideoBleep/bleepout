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
#include "BleepoutParameters.h"
#include "GameState.h"
#include "SpaceController.h"
#include "LogicController.h"
#include "GameEvents.h"
#include "Timing.h"
#include "Animations.h"

class RendererBase;
class AdminController;

class RoundController : public EventSource
{
public:
  RoundController(RoundConfig& config,
                  BleepoutParameters& appParams,
                  std::list<ofPtr<Player> > players,
                  PlayerManager& playerManager);
  
  ~RoundController();
  
  void setup();
  void draw();
  void update();
  
  ofEvent<RoundStateEventArgs> roundQueueEvent;
  ofEvent<RoundStateEventArgs> roundPlayEvent;
  ofEvent<RoundEndedEventArgs> roundEndedEvent;
  ofEvent<BallStateEventArgs> ballSpawnedEvent;
  
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
  
  void attachTo(AdminController& adminController);
  void detachFrom(AdminController& adminController);
  
  const char* eventSourceName() const override { return "RoundController"; }
  
  LogicController& logicController() { return *_logicController; }
  SpaceController& spaceController() { return *_spaceController; }

private:
  void onPlayerYawPitchRoll(PlayerYawPitchRollEventArgs& e);
  
  void onRoundQueue(RoundStateEventArgs& e);
  void onRoundPlay(RoundStateEventArgs& e);
  
  void onTryEndRound(EndRoundEventArgs& e);
  
  void onModifierAppeared(ModifierEventArgs& e);
  void onCountdownTick(TimerEventArgs& e);
  void onTrySpawnBall(SpawnBallEventArgs& e);
  void notifyBallSpawned(RoundState& state, Ball* ball);
  
  void endRound();
  void notifyRoundEnded(RoundResults& results);
  RoundResults buildRoundResults(RoundEndReason reason);
  
  bool _paused;
  float _startTime;
  BleepoutParameters& _appParams;
  PlayerManager& _playerManager;
  RoundConfig& _config;
  RoundState _state;
  ofPtr<RendererBase> _renderer;
  ofPtr<SpaceController> _spaceController;
  ofPtr<LogicController> _logicController;
  TimedActionSet _timedActions;
  ofPtr<AnimationManager> _animationManager;
  Pulser _cullDeadObjectsPulser;
  bool _ending;
  RoundEndReason _endReason;
};

#endif /* defined(__bleepout__RoundController__) */
