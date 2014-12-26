//
//  AudioManager.h
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#ifndef __bleepout__AudioManager__
#define __bleepout__AudioManager__

#include <ofSoundPlayer.h>

class RoundController;
class BrickDestroyedEventArgs;
class BleepoutParameters;

class AudioManager {
public:
  AudioManager(const BleepoutParameters& appParams);
  void setup();
  void update();
  void attachTo(RoundController& roundController);
  void detachFrom(RoundController& roundController);
private:
  void onBrickDestroyed(BrickDestroyedEventArgs& e);
  
  ofSoundPlayer _brickDestroyedSound;
  const BleepoutParameters& _appParams;
};

#endif /* defined(__bleepout__AudioManager__) */
