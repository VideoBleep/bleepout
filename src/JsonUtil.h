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
#include "BleepoutConfig.h"
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
Json::Value toJsonObj(const T& val);

bool readJsonFile(std::string path, Json::Value* obj);
void writeJsonFile(std::string path, const Json::Value& obj);

#endif /* defined(__bleepout__JsonUtil__) */
