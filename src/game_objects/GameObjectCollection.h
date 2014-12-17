//
//  GameObjectCollection.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef bleepout_GameObjectCollection_h
#define bleepout_GameObjectCollection_h

#include <list>
#include <ofTypes.h>
#include <iostream>
#include <string>
#include "Common.h"
#include "GameObject.h"

template<typename T>
class GameObjectCollection : public std::list<ofPtr<T> > {
private:
  typedef std::list<ofPtr<T> >  ImplType;

public:  
  ofPtr<T> getById(GameObjectId id) {
    for (auto& obj : *this) {
      if (obj && obj->id() == id)
        return obj;
    }
    return ofPtr<T>();
  }

  void dumpToLog(const std::string& label, ofLogLevel level) {
    ofLog(level) << label << "(size:" << ImplType::size() << ")";
    for (auto& obj : *this) {
      GameObject& o = *obj;
      ofLog(level) << "   " << (o);
    }
  }
  
  inline GameObjectType objectType() const {
    return GameObjectTypeTraits<T>::typeId;
  }
  
  int pruneDeadObjects() {
    int count = 0;
    for (auto iter = ImplType::begin();
         iter != ImplType::end();
         iter++) {
      ofPtr<T>& obj = *iter;
      if (obj && !obj->alive()) {
        obj.reset();
      }
      if (!obj) {
        iter = ImplType::erase(iter);
        count++;
      }
    }
    return count;
  }
};

#endif
