//
//  AdminController.h
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#ifndef __bleepout__AdminController__
#define __bleepout__AdminController__

#include <ofxUI.h>
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"
#include "GameEvents.h"
#include <ofEvents.h>
#include <ofTypes.h>
#include <list>

struct AdminUIControls;
class BleepoutApp;
class SetupController;
class RoundController;
class RoundState;

class AdminController : public EventSource {
public:
  AdminController(BleepoutParameters& appParams,
                  SetupController& setupController);
  ~AdminController();
  
  ofEvent<StartRoundEventArgs> tryStartRoundEvent;
  ofEvent<EndRoundEventArgs> tryEndRoundEvent;
  
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void attachTo(BleepoutApp& app);
  void detachFrom(BleepoutApp& app);
  
  const char* eventSourceName() const override { return "AdminController"; }
private:
  void onUIEvent(ofxUIEventArgs& e);
  void onRoundStarted(RoundStateEventArgs& e);
  void onRoundEnded(RoundEndedEventArgs& e);
  bool tryStartRound();
  bool canStartRound();
  void tryEndRound();
  bool notifyTryStartRound(ofPtr<RoundConfig> config,
                           std::list<ofPtr<Player> > players);
  bool notifyTryEndRound();
  
  BleepoutParameters& _appParams;
  BleepoutConfig& _appConfig;
  SetupController& _setupController;
  ofxUICanvas* _gui;
  RoundState* _roundState;
  AdminUIControls* _controls;
};

#endif /* defined(__bleepout__AdminController__) */
