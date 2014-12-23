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

class AdminController {
public:
  AdminController(BleepoutConfig& appConfig);
  ~AdminController();
  void setup();
  void update();
  void draw();
private:
  void onUIEvent(ofxUIEventArgs& e);
  
  ofxUICanvas* _gui;
  BleepoutConfig& _appConfig;
};

#endif /* defined(__bleepout__AdminController__) */
