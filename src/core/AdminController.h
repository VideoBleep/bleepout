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

struct AdminUIControls;
class BleepoutApp;
class EmptyEventArgs;
class RoundStateEventArgs;

class AdminController {
public:
  AdminController(BleepoutParameters& appParams);
  ~AdminController();
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void attachTo(BleepoutApp& app);
  void detachFrom(BleepoutApp& app);
private:
  void onUIEvent(ofxUIEventArgs& e);
  void onRoundStarted(RoundStateEventArgs& e);
  void onRoundEnded(EmptyEventArgs& e);
  
  BleepoutParameters& _appParams;
  BleepoutConfig& _appConfig;
  ofxUICanvas* _gui;
  AdminUIControls* _controls;
};

#endif /* defined(__bleepout__AdminController__) */
