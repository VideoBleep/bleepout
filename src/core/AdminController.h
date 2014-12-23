//
//  AdminController.h
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#ifndef __bleepout__AdminController__
#define __bleepout__AdminController__

#include "ofxUI.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

struct AdminUIControls;

class AdminController {
public:
  AdminController(BleepoutParameters& appParams);
  ~AdminController();
  void setup();
  void update();
  void draw();
private:
  void onUIEvent(ofxUIEventArgs& e);
  
  BleepoutParameters& _appParams;
  BleepoutConfig& _appConfig;
  ofxUICanvas* _gui;
  AdminUIControls* _controls;
};

#endif /* defined(__bleepout__AdminController__) */
