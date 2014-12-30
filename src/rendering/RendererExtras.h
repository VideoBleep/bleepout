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
#include "RoundComponent.h"
#include <ofTypes.h>

class RendererExtrasImpl;

class RendererExtras : public RoundComponent {
public:
  RendererExtras(RoundState& state);
  void setup() override;
  void update() override;
  void draw() override;
private:
  ofPtr<RendererExtrasImpl> _impl;
};

#endif /* defined(__bleepout__RendererExtras__) */
