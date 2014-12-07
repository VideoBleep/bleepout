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
//  int size() const { return _list.size(); }
  
//  ofPtr<T>& operator[](int i) { return _list[i]; }
//  const ofPtr<T>& operator[](int i) const { return _list[i]; }
  
  ofPtr<T> getById(GameObjectId id) {
//    auto index = _idIndexLookup.at(id);
//    return _list.at(index);
    // yes this is slow, but it's simple for now
    for (auto iter = VectorType::begin(); iter != VectorType::end(); ++iter) {
      if ((*iter)->id() == id)
        return *iter;
    }
    return ofPtr<T>();
  }
//  const T& getById(GameObjectId id) const {
//    auto index = _idIndexLookup.at(id);
//    return *(_list.at(index));
//  }
  
  void add(ofPtr<T> obj) {
    push_back(obj);
//    _idIndexLookup[obj->id()] = VectorType::size() - 1;
  }
  
//  inline const vector_type& list() const { return _list; }
  
  inline typename VectorType::iterator begin() { return VectorType::begin(); }
  inline typename VectorType::const_iterator begin() const { return VectorType::begin(); }

  inline typename VectorType::iterator end() { return VectorType::end(); }
  inline typename VectorType::const_iterator end() const { return VectorType::end(); }
  
  void dumpToLog(const std::string& label) {
    ofLogVerbose() << label << "(size:" << VectorType::size() << ")";
    for (auto& obj : *this) {
      GameObject& o = *obj;
      ofLogVerbose() << "   " << (o);
    }
  }
  
private:
//  std::vector<ofPtr<T> > _list;
//  std::map<GameObjectId, int > _idIndexLookup;
};

#endif
