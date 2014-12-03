//
//  BleepoutConfig.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__BleepoutConfig__
#define __bleepout__BleepoutConfig__

#include <string>

class BleepoutConfig {
public:
  static const BleepoutConfig& getInstance();
  
  void loadFile(const std::string& path);
  void saveFile(const std::string& path);
  
  int fps() const { return _fps; }
private:
  int _fps;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
