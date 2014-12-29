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

std::ostream& operator<<(std::ostream& os, const Json::ValueType& type);

template<typename T>
Json::Value toJsonVal(const T& val);

template<typename T>
Json::Value toJsonArr(const std::vector<T>& vals) {
  Json::Value arr(Json::arrayValue);
  for (int i = 0; i < vals.size(); i++) {
    arr[i] = toJsonVal(vals[i]);
  }
  return arr;
}

template<typename T>
void addToJsonArr(Json::Value* arr, const std::vector<T>& vals) {
  int size = arr->size();
  for (const auto& val : vals) {
    (*arr)[size] = toJsonVal(val);
    size++;
  }
}

template<typename T>
void addValuesToJsonArr(Json::Value* arr, const std::map<std::string, T>& map) {
  int size = arr->size();
  for (const auto& entry : map) {
    (*arr)[size] = toJsonVal(entry.second);
    size++;
  }
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

bool jsonValIsEmpty(const Json::Value& val);
bool jsonValIsEmptyOrDefault(const Json::Value& val,
                             const float defaultValue = 0);
bool jsonValIsEmptyOrDefault(const Json::Value& val,
                             const int defaultValue = 0);
bool jsonValIsEmptyOrDefault(const Json::Value& val,
                             const bool defaultValue = false);
bool jsonValIsEmptyOrDefault(const Json::Value& val,
                             const std::string& defaultValue);

void cleanJsonVal(Json::Value& val);

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
