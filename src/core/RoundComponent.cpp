//
//  RoundComponent.cpp
//  bleepout
//
//  Created by tekt on 12/24/14.
//
//

#include "RoundComponent.h"
#include "BleepoutParameters.h"
#include "BleepoutConfig.h"
#include "GameState.h"

RoundComponent::RoundComponent(RoundState& state,
                               const RoundConfig& config)
: _state(state) , _config(config) { }
