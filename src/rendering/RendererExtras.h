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

class RoundConfig;
class RoundState;
class BleepoutParameters;
class RendererExtrasImpl;

class RendererExtras : public RoundComponent {
public:
  RendererExtras(RoundState& state,
                 const RoundConfig& config,
                 const BleepoutParameters& appParams);
  void setup() override;
  void update() override;
  void draw() override;
  void keyPressed(int key);
private:
  ofPtr<RendererExtrasImpl> _impl;
};

#endif /* defined(__bleepout__RendererExtras__) */
