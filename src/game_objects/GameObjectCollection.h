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
  typedef std::list<ofPtr<T> >  CollectionType;
public:
  
  ofPtr<T> getById(GameObjectId id) {
    for (auto& obj : *this) {
      if (obj && obj->id() == id)
        return obj;
    }
    return ofPtr<T>();
  }

  void dumpToLog(const std::string& label, ofLogLevel level) {
    ofLog(level) << label << "(size:" << CollectionType::size() << ")";
    for (auto& obj : *this) {
      GameObject& o = *obj;
      ofLog(level) << "   " << (o);
    }
  }
  
  inline GameObjectType objectType() const {
    return GameObjectTypeTraits<T>::typeId;
  }
  
private:
  void push_back(ofPtr<T>& obj) {
    CollectionType::push_back(obj);
  }
  
  friend class RoundState;
  
};

#endif
