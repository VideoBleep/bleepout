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

bool readJsonVal(const Json::Value& obj, const char* property, float* result);
bool readJsonVal(const Json::Value& obj, const char* property, int* result);
bool readJsonVal(const Json::Value& obj, const char* property, unsigned char* result);
bool readJsonVal(const Json::Value& obj, const char* property, bool* result);
bool readJsonVal(const Json::Value& obj, const char* property, std::string* result);
bool readJsonVal(const Json::Value& obj, const char* property, ofVec2f* result);
bool readJsonVal(const Json::Value& obj, const char* property, ofVec3f* result);
bool readJsonVal(const Json::Value& obj, const char* property, ofColor* result);
bool readJsonVal(const Json::Value& obj, const char* property, std::vector<ofColor>* result);

template<typename TEnum>
bool readJsonEnumVal(const Json::Value& obj, const char* property, TEnum* result) {
  int valueTemp;
  if(!readJsonVal(obj, property, &valueTemp))
    return false;
  *result = (TEnum)valueTemp;
  return true;
}

Json::Value toJsonObj(const ofVec2f& val);
Json::Value toJsonObj(const ofVec3f& val);
Json::Value toJsonObj(const ofColor& val);
Json::Value toJsonArr(const std::vector<ofColor>& vals);

bool readJsonFile(std::string path, Json::Value* obj);
void writeJsonFile(std::string path, const Json::Value& obj);

#endif /* defined(__bleepout__JsonUtil__) */
