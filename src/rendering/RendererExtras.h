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

class RendererExtras {
public:
  void setup(const RoundConfig& config);
  void update(RoundState& state);
  void draw(RoundState& state);
  void keyPressed(int key);
private:
  ofPtr<RendererExtrasImpl> _impl;
};

#endif /* defined(__bleepout__RendererExtras__) */
