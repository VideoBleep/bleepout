//
//  JsonUtil.h
//  bleepout
//
//  Created by tekt on 12/11/14.
//
//

#ifndef __bleepout__JsonUtil__
#define __bleepout__JsonUtil__

#include <json.h> // it's included as part of ofxLibwebsockets
#include <ofMain.h>
#include <vector>
#include <map>
#include "ObjectSpecs.h"
#include "Common.h"

bool assertType(const Json::Value& val, Json::ValueType type);

template<typename T>
bool readJsonVal(const Json::Value& obj, T* result);

template<typename TEnum>
bool readJsonEnumVal(const Json::Value& val, TEnum* result) {
  int temp;
  if(!readJsonVal(val, &temp))
    return false;
  *result = (TEnum)temp;
  return true;
}

template<typename T>
bool readJsonArr(const Json::Value& arr, std::vector<T>* result) {
  if (!assertType(arr, Json::arrayValue))
    return false;
  std::vector<T> tempVals;
  for (Json::ArrayIndex i = 0; i < arr.size(); i++) {
    T temp;
    if (!readJsonVal(arr[i], &temp))
      return false;
    tempVals.push_back(temp);
  }
  result->clear();
  result->swap(tempVals);
  return true;
}

template<typename T>
bool readJsonVal(const Json::Value& val,
                 std::map<std::string, T>* result) {
  if (val.isNull())
    return true;
  if (!assertType(val, Json::objectValue))
    return false;
  auto end = val.end();
  for (auto i = val.begin(); i != end; i++) {
    T temp;
    if (!readJsonVal(*i, &temp))
      return false;
    result->insert(std::make_pair(i.key().asString(), temp));
  }
  return true;
}

template<typename T>
Json::Value toJsonVal(const T& val);

template<typename T>
Json::Value toJsonArr(const std::vector<T>& vals) {
  Json::Value arr(Json::arrayValue);
  arr.resize(vals.size());
  for (int i = 0; i < vals.size(); i++) {
    arr[i] = toJsonVal(vals[i]);
  }
  return arr;
}

template<typename T>
Json::Value toJsonObj(const std::map<std::string, T>& map) {
  Json::Value obj(Json::objectValue);
  for (const auto& entry : map) {
    obj[entry.first] = toJsonVal(entry.second);
  }
  return obj;
}

template<typename T>
Json::Value valuesToJsonObj(const std::map<std::string, T>& map) {
  Json::Value arr(Json::arrayValue);
  int i = 0;
  for (const auto& entry : map) {
    arr[i] = toJsonVal(entry.second);
    i++;
  }
  return arr;
}

void writeJsonFile(std::string path, const Json::Value& obj);

class JsonLoader {
public:
  JsonLoader() : _logLevel(OF_LOG_WARNING) { }
  
  template<typename T>
  void readVal(const Json::Value& val, T* result,
               const T& defaultVal) const;
  
  template<typename T>
  void readVal(const Json::Value& obj, T* result) const {
    readVal(obj, result, *result);
  }
  template<typename T>
  void readMap(const Json::Value& obj,
               std::map<std::string, T>* result) const {
    result->clear();
    if (!assertType(obj, Json::objectValue))
      return;
    auto end = obj.end();
    for (auto i = obj.begin(); i != end; i++) {
      T temp;
      if (readVal(*i, &temp))
        result->insert(std::make_pair(i.key().asString(), temp));
    }
  }
  
  template<typename T>
  void readArr(const Json::Value& arr, std::vector<T>* result) const {
    result->clear();
    if (!assertType(arr, Json::arrayValue))
      return;
    for (int i = 0; i < arr.size(); i++) {
      T temp;
      readVal(arr[i], &temp);
      result->push_back(temp);
    }
  }
  
  bool readFile(std::string path, Json::Value* result) const;
  
  bool assertType(const Json::Value& val,
                  Json::ValueType type) const;
private:
  ofLogLevel _logLevel;
};

#endif /* defined(__bleepout__JsonUtil__) */
