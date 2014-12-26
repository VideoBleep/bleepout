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
#include <vector>
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
  
  std::vector<ofPtr<T> > extractDeadObjects() {
    std::vector<ofPtr<T> > deadObjects;
    for (auto iter = ImplType::begin();
         iter != ImplType::end();) {
      ofPtr<T>& obj = *iter;
      if (obj && obj->alive()) {
        iter++;
      } else {
        if (obj) {
          deadObjects.push_back(obj);
        }
        iter = ImplType::erase(iter);
      }
    }
    return deadObjects;
  }
  
  bool eraseObjectById(GameObjectId id) {
    for (auto iter = ImplType::begin();
         iter != ImplType::end();
         iter++) {
      ofPtr<T>& obj = *iter;
      if (obj && obj->id() == id) {
        obj.reset();
        ImplType::erase(iter);
        return true;
      }
    }
    return false;
  }
};

#endif
