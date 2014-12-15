//
//  RendererExtras.h
//  bleepout
//
//  Created by tekt on 12/13/14.
//
//

#ifndef __bleepout__RendererExtras__
#define __bleepout__RendererExtras__

#include "BleepoutConfig.h"
#include "GameState.h"
#include <ofTypes.h>

class RendererExtrasImpl;
class RoundStateEventSource;

class RendererExtras {
public:
  void setup(const RoundConfig& config, RoundStateEventSource& eventSource);
  void update();
  void draw(RoundState& state, const RoundConfig& config);
  void keyPressed(int key);
private:
  ofPtr<RendererExtrasImpl> _impl;
};

#endif /* defined(__bleepout__RendererExtras__) */
