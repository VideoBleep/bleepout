//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutApp.h"
#include <ofxXmlSettings.h>
#include <json.h> // it's included as part of ofxLibwebsockets
#include <fstream>

BleepoutConfig::BleepoutConfig()
: _fps(30),
_logLevel(OF_LOG_NOTICE),
_vsync(true) {
  
}

void BleepoutConfig::loadFile(const std::string& path) {
  ofxXmlSettings settings;
  if (!path.empty())
    settings.load(path);
  _fps = settings.getValue("settings:fps", _fps);
  _logLevel = (ofLogLevel)settings.getValue("settings:logLevel", _logLevel);
  _vsync = settings.getValue("settings:vsync", _vsync);
  //...
}

void BleepoutConfig::saveFile(const std::string& path) const {
  ofxXmlSettings settings;
  settings.setValue("settings:fps", _fps);
  settings.setValue("settings:logLevel", (int)_logLevel);
  settings.setValue("settings:vsync", _vsync);
  settings.save(path);
}

static bool readJsonVal(const Json::Value& obj, const char* property, float* result) {
  const Json::Value& val = obj[property];
  if (!obj.isNumeric()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected number): " << val.toStyledString();
    return false;
  }
  *result = obj.asFloat();
  return true;
}

static bool readJsonVal(const Json::Value& obj, const char* property, int* result) {
  const Json::Value& val = obj[property];
  if (!obj.isNumeric()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected number): " << val.toStyledString();
    return false;
  }
  *result = obj.asInt();
  return true;
}

static bool readJsonVal(const Json::Value& obj, const char* property, bool* result) {
  const Json::Value& val = obj[property];
  if (!obj.isBool()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected boolean): " << val.toStyledString();
    return false;
  }
  *result = val.asBool();
  return true;
}

template<typename TEnum>
static bool readJsonEnumVal(const Json::Value& obj, const char* property, TEnum* result) {
  int valueTemp;
  if(!readJsonVal(obj, property, &valueTemp))
    return false;
  *result = (TEnum)valueTemp;
  return true;
}

static bool readJsonVal(const Json::Value& obj, const char* property, ofVec2f* result) {
  const Json::Value& val = obj[property];
  if (!obj.isObject()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected object): " << val.toStyledString();
    return false;
  }
  ofVec2f temp;
  if (!readJsonVal(val, "x", &(temp.x)) ||
      !readJsonVal(val, "y", &(temp.y))) {
    return false;
  }
  *result = temp;
  return true;
}

static bool readJsonVal(const Json::Value& obj, const char* property, ofVec3f* result) {
  const Json::Value& val = obj[property];
  if (!obj.isObject()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected object): " << val.toStyledString();
    return false;
  }
  ofVec3f temp;
  if (!readJsonVal(val, "x", &(temp.x)) ||
      !readJsonVal(val, "y", &(temp.y)) ||
      !readJsonVal(val, "z", &(temp.z))) {
    return false;
  }
  *result = temp;
  return true;
}

static bool readJsonVal(const Json::Value& obj, const char* property, PhysicsOptions* result) {
  const Json::Value& val = obj[property];
  if (!obj.isObject()) {
    ofLogError() << "invalid type for property \"" << property << "\" (expected object): " << val.toStyledString();
    return false;
  }
  PhysicsOptions temp;
  if (!readJsonVal(val, "density", &temp.density) ||
      !readJsonVal(val, "bounce", &temp.bounce) ||
      !readJsonVal(val, "friction", &temp.friction))
    return false;
  *result = temp;
  return true;
}

static Json::Value toJsonObj(const ofVec2f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  return obj;
}

static Json::Value toJsonObj(const ofVec3f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  obj["z"] = val.z;
  return obj;
}

static Json::Value toJsonObj(const PhysicsOptions& val) {
  Json::Value obj(Json::objectValue);
  obj["density"] = val.density;
  obj["bounce"] = val.bounce;
  obj["friction"] = val.friction;
  return obj;
}

static bool readJsonObjectFile(std::string path, Json::Value* obj) {
  path = ofToDataPath(path);
  std::ifstream fis(path.c_str());
  Json::Reader reader;
  if (!reader.parse(fis, *obj)) {
    ofLogError() << "error loading json from: " << path << ": " << reader.getFormattedErrorMessages();
    return false;
  }
  if (!obj->isObject()) {
    ofLogError() << "config root is not object: " << obj->toStyledString();
    return false;
  }
}

static void writeJsonFile(std::string path, const Json::Value& obj) {
  path = ofToDataPath(path);
  std::ofstream fos(path.c_str());
  Json::StyledStreamWriter writer;
  writer.write(fos, obj);
}

void BleepoutConfig::loadJsonFile(std::string path) {
  Json::Value root;
  if (!readJsonObjectFile(path, &root))
    return;
  readJsonVal(root, "fps", &_fps);
  readJsonEnumVal(root, "logLevel", &_logLevel);
  readJsonVal(root, "vsync", &_vsync);
}

void BleepoutConfig::saveJsonFile(std::string path) const {
  Json::Value root(Json::objectValue);
  root["fps"] = _fps;
  root["logLevel"] = (int)_logLevel;
  root["vsync"] = _vsync;
  writeJsonFile(path, root);
}

static void readPhysics(ofxXmlSettings& settings,
                        std::string prefix,
                        PhysicsOptions& vals) {
  vals.density = settings.getValue(prefix + "Density", vals.density);
  vals.bounce = settings.getValue(prefix + "Bounce", vals.bounce);
  vals.friction = settings.getValue(prefix + "Friction", vals.friction);
}

static void writePhysics(ofxXmlSettings& settings,
                         std::string prefix,
                         const PhysicsOptions& options) {
  settings.setValue(prefix + "Density", options.density);
  settings.setValue(prefix + "Bounce", options.bounce);
  settings.setValue(prefix + "Friction", options.friction);
}

static void readVec2f(ofxXmlSettings& settings, std::string prefix, ofVec2f& vals) {
  vals.x = settings.getValue(prefix + "X", vals.x);
  vals.y = settings.getValue(prefix + "Y", vals.y);
}

static void readVec3f(ofxXmlSettings& settings, std::string prefix, ofVec3f& vals) {
  vals.x = settings.getValue(prefix + "X", vals.x);
  vals.y = settings.getValue(prefix + "Y", vals.y);
  vals.z = settings.getValue(prefix + "Z", vals.z);
}

static void writeVec2f(ofxXmlSettings& settings, std::string prefix, ofVec2f vals) {
  settings.setValue(prefix + "X", vals.x);
  settings.setValue(prefix + "Y", vals.y);
}

static void writeVec3f(ofxXmlSettings& settings, std::string prefix, ofVec3f vals) {
  settings.setValue(prefix + "X", vals.x);
  settings.setValue(prefix + "Y", vals.y);
  settings.setValue(prefix + "Z", vals.z);
}

RoundConfig::RoundConfig(const BleepoutConfig& appConfig)
: _brickSize(20.0f, 5.0f, 5.0f),
_brickGap(1.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_ballPhysics(0.0f, 1.0f, 0.0f),
_paddlePhysics(0.0f, 0.0f, 0.9f),
_ballInitialVelocity(0.0f, 30.5f, 0.0f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_appConfig(appConfig) { }

void RoundConfig::loadFile(const std::string &path) {
  ofxXmlSettings settings;
  if (!path.empty())
    settings.load(path);
  readVec3f(settings, "settings:brickSize", _brickSize);
  _brickGap = settings.getValue("settings:brickGap", _brickGap);
  readVec3f(settings, "settings:paddleSize", _paddleSize);
  _ballRadius = settings.getValue("settings:ballRadius", _ballRadius);
  readPhysics(settings, "settings:ball", _ballPhysics);
  readPhysics(settings, "settings:paddle", _paddlePhysics);
  readVec3f(settings, "settings:ballInitialVelocity", _ballInitialVelocity);
  //...
}

void RoundConfig::saveFile(const std::string &path) const {
  ofxXmlSettings settings;
  writeVec3f(settings, "settings:brickSize", _brickSize);
  settings.setValue("settings:brickGap", _brickGap);
  writeVec3f(settings, "settings:paddleSize", _paddleSize);
  settings.setValue("settings:ballRadius", _ballRadius);
  writePhysics(settings, "settings:ball", _ballPhysics);
  writePhysics(settings, "settings:paddle", _paddlePhysics);
  writeVec3f(settings, "settings:ballInitialVelocity", _ballInitialVelocity);
  
  //...
  settings.save(path);
}

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value root;
  if (!readJsonObjectFile(path, &root))
    return;
  readJsonVal(root, "brickSize", &_brickSize);
  readJsonVal(root, "brickGap", &_brickGap);
  readJsonVal(root, "paddleSize", &_paddleSize);
  readJsonVal(root, "ballRadius", &_ballRadius);
  readJsonVal(root, "ballPhysics", &_ballPhysics);
  readJsonVal(root, "paddlePhysics", &_paddlePhysics);
  readJsonVal(root, "ballInitialVelocity", &_ballInitialVelocity);
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value root;
  root["brickSize"] = toJsonObj(_brickSize);
  root["brickGap"] = _brickGap;
  root["paddleSize"] = toJsonObj(_paddleSize);
  root["ballRadius"] = _ballRadius;
  root["ballPhysics"] = toJsonObj(_ballPhysics);
  root["paddlePhysics"] = toJsonObj(_paddlePhysics);
  root["ballInitialVelocity"] = toJsonObj(_ballInitialVelocity);
  writeJsonFile(path, root);
}

