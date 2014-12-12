//
//  JsonUtil.cpp
//  bleepout
//
//  Created by tekt on 12/11/14.
//
//

#include "JsonUtil.h"

#include <fstream>

bool readJsonFile(std::string path, Json::Value* obj) {
  path = ofToDataPath(path);
  std::ifstream fis(path.c_str());
  Json::Reader reader;
  if (!reader.parse(fis, *obj)) {
    ofLogError() << "error loading json from: " << path
                 << ": " << reader.getFormattedErrorMessages();
    return false;
  }
  if (!obj->isObject()) {
    ofLogError() << "invalid type for root (expected object): "
                 << obj->toStyledString();
    return false;
  }
}

void writeJsonFile(std::string path, const Json::Value& obj) {
  path = ofToDataPath(path);
  std::ofstream fos(path.c_str());
  Json::StyledStreamWriter writer;
  writer.write(fos, obj);
}

static const Json::Value* getNumeric(const Json::Value& obj, const char* property) {
  if (!obj.isMember(property)) {
    ofLogError() << "property not found '" << property << "'";
    return NULL;
  }
  const Json::Value& val = obj[property];
  if (!val.isNumeric()) {
    ofLogError() << "invalid type for '" << property << "' "
                 << "(expected number): " << val.toStyledString();
  }
  return &val;
}

static const Json::Value* getTypedValue(const Json::Value& obj, const char* property, Json::ValueType type) {
  if (!obj.isMember(property)) {
    ofLogError() << "property not found '" << property << "'";
    return NULL;
  }
  const Json::Value& val = obj[property];
  if (val.type() != type) {
    ofLogError() << "invalid type for '" << property << "' "
                 << "(expected " << type << "): " << val.toStyledString();
  }
  return &val;
}

bool readJsonVal(const Json::Value& obj, const char* property, float* result) {
  const Json::Value* val = getNumeric(obj, property);
  if (val == NULL)
    return false;
  *result = val->asFloat();
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, int* result) {
  const Json::Value* val = getNumeric(obj, property);
  if (val == NULL)
    return false;
  *result = val->asInt();
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, unsigned char* result) {
  const Json::Value* val = getNumeric(obj, property);
  if (val == NULL)
    return false;
  *result = (unsigned char)(val->asInt());
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, bool* result) {
  const Json::Value* val = getTypedValue(obj, property, Json::booleanValue);
  if (val == NULL)
    return false;
  *result = val->asBool();
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, ofVec2f* result) {
  const Json::Value* val = getTypedValue(obj, property, Json::objectValue);
  if (val == NULL)
    return false;
  ofVec2f temp;
  if (!readJsonVal(*val, "x", &(temp.x)) ||
      !readJsonVal(*val, "y", &(temp.y))) {
    return false;
  }
  *result = temp;
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, ofVec3f* result) {
  const Json::Value* val = getTypedValue(obj, property, Json::objectValue);
  if (val == NULL)
    return false;
  ofVec3f temp;
  if (!readJsonVal(*val, "x", &(temp.x)) ||
      !readJsonVal(*val, "y", &(temp.y)) ||
      !readJsonVal(*val, "z", &(temp.z))) {
    return false;
  }
  *result = temp;
  return true;
}

static bool readJsonValImpl(const Json::Value& val, const char* description, ofColor* result) {
  if (!val.isObject()) {
    ofLogError() << "invalid type for '" << description << "' "
                 << "(expected object): " << val.toStyledString();
    return false;
  }
  ofColor temp;
  if (!readJsonVal(val, "r", &temp.r) ||
      !readJsonVal(val, "g", &temp.g) ||
      !readJsonVal(val, "b", &temp.b)) {
    return false;
  }
  *result = temp;
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, ofColor* result) {
  const Json::Value* val = getTypedValue(obj, property, Json::objectValue);
  if (val == NULL)
    return false;
  return readJsonValImpl(val, property, result);
}

bool readJsonVal(const Json::Value& obj, const char* property, std::vector<ofColor>* result) {
  const Json::Value* arr = getTypedValue(obj, property, Json::arrayValue);
  std::vector<ofColor> tempVals;
  for (Json::ArrayIndex i = 0; i < arr->size(); i++) {
    ofColor tempVal;
    if (!readJsonValImpl((*arr)[i], property, &tempVal)) {
      return false;
    }
    tempVals.push_back(tempVal);
  }
  result->clear();
  result->swap(tempVals);
  return true;
}

bool readJsonVal(const Json::Value& obj, const char* property, ValueSpecifier* result) {
  const Json::Value& val = obj["property"];
  if (val.isNull()) {
    ofLogError() << "property not found '" << property << "'";
    return false;
  }
  if (val.isNumeric()) {
    *result = ValueSpecifier::createConstant(val.asFloat());
    return true;
  }
  Json::Value min;
  Json::Value max;
  Json::Value isRandom(Json::nullValue);
  if (val.isArray()) {
    if (val.size() != 2) {
      ofLogError() << "Invalid number of values for property '" << property << "'";
      return false;
    }
    min = val[0];
    max = val[1];
  } else if (val.isObject()) {
    min = val["min"];
    max = val["max"];
    isRandom = val["random"];
    if (!isRandom.isNull() || !isRandom.isBool()) {
      ofLogError() << "Invalid type for property '" << property << ".random' "
      << "(expected bool or null): " << isRandom.toStyledString();
      return false;
    }
  } else {
    ofLogError() << "Invalid type for property '" << property << "' "
      << "(expected number or array or object): " << val.toStyledString();
    return false;
  }
  if (!min.isNumeric()) {
    ofLogError() << "Invalid type for property '" << property << "'[0] "
    << "(expected number): " << min.toStyledString();
    return false;
  }
  if (!max.isNumeric()) {
    ofLogError() << "Invalid type for property '" << property << "'[0] "
    << "(expected number): " << max.toStyledString();
    return false;
  }
  if (isRandom.asBool()) {
    *result = ValueSpecifier::createRandom(min.asFloat(), max.asFloat());
  } else {
    *result = ValueSpecifier::createRange(min.asFloat(), max.asFloat());
  }
  return true;
}

Json::Value toJsonObj(const ofVec2f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  return obj;
}

Json::Value toJsonObj(const ofVec3f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  obj["z"] = val.z;
  return obj;
}

Json::Value toJsonObj(const ofColor& val) {
  Json::Value obj(Json::objectValue);
  obj["r"] = val.r;
  obj["g"] = val.g;
  obj["b"] = val.b;
  return obj;
}

Json::Value toJsonObj(const ValueSpecifier& val) {
  if (val.isConstant())
    return Json::Value((double)val.minValue());
  if (val.isRange()) {
    Json::Value arr(Json::arrayValue);
    arr.resize(2);
    arr[0] = Json::Value(val.minValue());
    arr[1] = Json::Value(val.maxValue());
    return arr;
  }
  if (val.isRandom()) {
    Json::Value obj(Json::objectValue);
    obj["random"] = true;
    obj["min"] = val.minValue();
    obj["max"] = val.maxValue();
    return obj;
  }
  return Json::Value(Json::nullValue);
}

Json::Value toJsonArr(const std::vector<ofColor>& vals) {
  Json::Value arr(Json::arrayValue);
  arr.resize(vals.size());
  for (Json::ArrayIndex i = 0; i < vals.size(); i++) {
    arr[i] = toJsonObj(vals[i]);
  }
  return arr;
}