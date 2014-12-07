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
#include "PlayerManager.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"

class BleepoutApp : public ofBaseApp {
public:
  static inline BleepoutApp* getApp() {
    return (BleepoutApp*)ofGetAppPtr();
  }
  
  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;
  void mouseMoved(int x, int y ) override;
  void mouseDragged(int x, int y, int button) override;
  const BleepoutConfig& config() const { return _config; }
private:
  BleepoutConfig _config;
  ofPtr<RoundController> _roundController;
};

#endif /* defined(__bleepout__BleepoutApp__) */
