//
//  GameObjectCollection.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef bleepout_GameObjectCollection_h
#define bleepout_GameObjectCollection_h

#include <vector>
#include <map>
#include <ofTypes.h>
#include <iostream>
#include <string>
#include "Common.h"
#include "GameObject.h"

template<typename T>
class GameObjectCollection : public std::vector<ofPtr<T> > {
public:
  typedef std::vector<ofPtr<T> >  VectorType;
  
  ofPtr<T> getById(GameObjectId id) {
    for (auto iter = VectorType::begin(); iter != VectorType::end(); ++iter) {
      if ((*iter)->id() == id)
        return *iter;
    }
    return ofPtr<T>();
  }
  
  inline typename VectorType::iterator begin() { return VectorType::begin(); }
  inline typename VectorType::const_iterator begin() const { return VectorType::begin(); }
  
  inline typename VectorType::iterator end() { return VectorType::end(); }
  inline typename VectorType::const_iterator end() const { return VectorType::end(); }

  void dumpToLog(const std::string& label, ofLogLevel level) {
    ofLog(level) << label << "(size:" << VectorType::size() << ")";
    for (auto& obj : *this) {
      GameObject& o = *obj;
      ofLog(level) << "   " << (o);
    }
  }
};

#endif
