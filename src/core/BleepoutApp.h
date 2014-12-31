//
//  BleepoutApp.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__BleepoutApp__
#define __bleepout__BleepoutApp__

#include <ofMain.h>
#include <ofTypes.h>
#include "PlayerManager.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"
#include "Common.h"
#include "SetupController.h"
#include "GameEvents.h"
#include "AdminController.h"
#include "AudioManager.h"
#include "RoundResults.h"
#include "Timing.h"
#include "AnimationObject.h"
#include "GameObjectCollection.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphonClient.h>
#endif // ENABLE_SYPHON

#ifdef RADOME
#include "radomePlugin.h"
typedef radomePlugin AppBase;
#else
typedef ofBaseApp AppBase;
#endif

class AppAnimationManager;

class BleepoutApp : public AppBase, public EventSource {
public:
  BleepoutApp();
  ~BleepoutApp();
  
  ofEvent<RoundStateEventArgs> roundStartedEvent;
  ofEvent<RoundStateEventArgs> roundPlayEvent;
  ofEvent<RoundEndedEventArgs> roundEndedEvent;
  
  const RoundConfig* currentRoundConfig() const;
  
#ifndef RADOME
  // oF interface methods
  void setup() override;
  void update() override;
  void draw() override;
#else
  void setup();
  void update();
  void draw();
    
  virtual void initialize() override;
  virtual void renderScene(DomeInfo& dome);
  virtual void update(DomeInfo& dome) override;
#endif
  
  void addAnimation(ofPtr<AnimationObject> animation);
  void addTimedAction(ofPtr<TimedAction> action);
    
  void keyPressed(int key);
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y );
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button);
  
  const char* eventSourceName() const override { return "BleepoutApp"; }

private:
  void onTryStartRound(StartRoundEventArgs& e);
  void onRoundPlay(RoundStateEventArgs& e);
  void onRoundEnded(RoundEndedEventArgs& e);

  void notifyRoundStarted(RoundState& state);
  void notifyRoundPlay(RoundStateEventArgs& e);
  void notifyRoundEnded(RoundEndedEventArgs& e);
  void endRound();

  ofPtr<BleepoutConfig> _config;
  ofPtr<PlayerManager> _playerManager;
  ofPtr<PlayerController> _playerController;
  ofPtr<SetupController> _setupController;
  ofPtr<RoundController> _roundController;
  ofPtr<AdminController> _adminController;
  ofPtr<AudioManager> _audioManager;
  ofPtr<AppAnimationManager> _animationManager;
  TimedActionSet _timedActions;
  GameObjectCollection<AnimationObject> _animations;
#ifdef ENABLE_SYPHON
  ofxSyphonClient _syphonClient;
#endif // ENABLE_SYPHON
  bool _syphonEnabled;
};

#endif /* defined(__bleepout__BleepoutApp__) */
