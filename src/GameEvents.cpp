//
//  GameEvents.cpp
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#include "GameEvents.h"
#include <ofLog.h>

template<typename EventType>
static void logEvent(ofLogLevel level, const char* name, const EventType& e) {
  ofLog log(level);
  log << "EVENT{" << name << "}: ";
  std::ostringstream sstr;
  e.output(sstr);
  log << sstr.str();
}

#define EVENT_LOGGER_CALLBACK(EventName, ArgsType) \
  void on##EventName(ArgsType& e) { \
    logEvent(_level, #EventName, e);\
  }

class CollisionEventLogger {
public:
  CollisionEventLogger(ofLogLevel level) : _level(level) { }

  EVENT_LOGGER_CALLBACK(BallHitPaddle, BallHitPaddleEventArgs);
  EVENT_LOGGER_CALLBACK(BallHitBrick, BallHitBrickEventArgs);
  EVENT_LOGGER_CALLBACK(BallHitWall, BallHitWallEventArgs);
  EVENT_LOGGER_CALLBACK(BallHitBall, BallHitBallEventArgs);
private:
  ofLogLevel _level;
};

void CollisionEventSource::enableLogging(ofLogLevel level) {
  disableLogging();
  _logger.reset(new CollisionEventLogger(level));
  attachListener(*_logger);
}

void CollisionEventSource::disableLogging() {
  if (_logger)
    detachListener(*_logger);
  _logger.reset();
}

class RoundStateEventLogger {
public:
  RoundStateEventLogger(ofLogLevel level) : _level(level) { }
  
  EVENT_LOGGER_CALLBACK(BallOwnerChanged, BallOwnerChangedEventArgs);
  EVENT_LOGGER_CALLBACK(BrickDestroyed, BrickDestroyedEventArgs);
  EVENT_LOGGER_CALLBACK(PlayerScoreChanged, PlayerEventArgs);
  EVENT_LOGGER_CALLBACK(BallDestroyed, BallEventArgs);
  EVENT_LOGGER_CALLBACK(BallRespawned, BallEventArgs);
  EVENT_LOGGER_CALLBACK(PlayerLost, PlayerEventArgs);
  EVENT_LOGGER_CALLBACK(PlayerLivesChanged, PlayerEventArgs);
  EVENT_LOGGER_CALLBACK(RoundEnded, EmptyEventArgs);
private:
  ofLogLevel _level;
};

void RoundStateEventSource::enableLogging(ofLogLevel level) {
  disableLogging();
  _logger.reset(new RoundStateEventLogger(level));
  attachListener(*_logger);
}

void RoundStateEventSource::disableLogging() {
  if (_logger)
    detachListener(*_logger);
  _logger.reset();
}


