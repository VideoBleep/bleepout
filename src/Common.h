//
//  Common.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef bleepout_Common_h
#define bleepout_Common_h

#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
public:
  explicit NotImplementedException(const std::string& description)
    : std::logic_error("not implemented: " + description) {}
  NotImplementedException()
    : std::logic_error("not implemented") {}
};


#endif
