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
  void loadFile(const std::string& path);
  void saveFile(const std::string& path);
};

#endif /* defined(__bleepout__BleepoutConfig__) */
