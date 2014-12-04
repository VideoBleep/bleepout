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
#include "Common.h"
#include "GameObject.h"

template<typename T>
class GameObjectCollection {
public:
  typedef std::vector<ofPtr<T> > vector_type;
  
  int size() const { return _list.size(); }
  
  ofPtr<T>& operator[](int i) { return _list[i]; }
  const ofPtr<T>& operator[](int i) const { return _list[i]; }
  
  ofPtr<T> getById(GameObjectId id) {
    auto index = _idIndexLookup.at(id);
    return _list.at(index);
  }
  const T& getById(GameObjectId id) const {
    auto index = _idIndexLookup.at(id);
    return *(_list.at(index));
  }
  
  void add(ofPtr<T> obj) {
    _list.push_back(obj);
    _idIndexLookup[obj->id()] = _list.size() - 1;
  }
  
  void removeById(GameObjectId id) {
    auto index = _idIndexLookup.at(id);
    _idIndexLookup.erase(id);
    _list.erase(_list.begin() + index);
  }
  
  inline const vector_type& list() const { return _list; }
  
  inline typename vector_type::iterator begin() { return _list.begin(); }
  inline typename vector_type::const_iterator begin() const { return _list.begin(); }

  inline typename vector_type::iterator end() { return _list.end(); }
  inline typename vector_type::const_iterator end() const { return _list.end(); }
  
private:
  std::vector<ofPtr<T> > _list;
  std::map<GameObjectId, int > _idIndexLookup;
};

#endif
