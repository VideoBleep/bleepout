//
//  RoundComponent.h
//  bleepout
//
//  Created by tekt on 12/24/14.
//
//

#ifndef __bleepout__RoundComponent__
#define __bleepout__RoundComponent__

class RoundState;
class RoundConfig;

class RoundComponent {
public:
  RoundComponent(RoundState& state,
                 const RoundConfig& config);
  virtual ~RoundComponent() {}
  
  virtual void setup() {}
  virtual void update() {}
  virtual void draw() {}
protected:
  const RoundConfig& _config;
  RoundState& _state;
};

#endif /* defined(__bleepout__RoundComponent__) */
