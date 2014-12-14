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
  void setup();
  void draw(RoundState state, RoundConfig config);
private:
  ofPtr<RendererExtrasImpl> _impl;
};

#endif /* defined(__bleepout__RendererExtras__) */
