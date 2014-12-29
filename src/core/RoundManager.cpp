//
//  RoundManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Common.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"
#include "DomeRenderer.h"
#include "Logging.h"
#include "AdminController.h"
#include "BleepoutParameters.h"
#include "JsonUtil.h"

RoundController::RoundController(RoundConfig& config,
                                 std::list<ofPtr<Player> > players,
                                 PlayerManager& playerManager)
: _config(config)
, _state(_config, players)
, _playerManager(playerManager)
, _timedActions(true)
, _ending(false)
, EventSource() {
}

RoundController::~RoundController() {
  ofRemoveListener(_playerManager.playerYawPitchRollEvent, this, &RoundController::onPlayerYawPitchRoll);
  ofRemoveListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  ofRemoveListener(_logicController->tryEndRoundEvent, this, &RoundController::onTryEndRound);
  ofRemoveListener(_logicController->trySpawnBallEvent, this, &RoundController::onTrySpawnBall);
  _logicController->detachFrom(*_spaceController);
  _renderer->detachFrom(*_logicController);
  _animationManager->detachFrom(*_logicController);
  _timedActions.clear();
  _logicController.reset();
  _renderer.reset();
  _spaceController.reset();
  _animationManager.reset();
}

void RoundController::setup() {
  _startTime = ofGetElapsedTimef();
  _state.time = 0;
  
  _cullDeadObjectsPulser = Pulser(5.0f);
  
  ofAddListener(_playerManager.playerYawPitchRollEvent, this, &RoundController::onPlayerYawPitchRoll);
  _spaceController.reset(new SpaceController(_state, _config));
  _logicController.reset(new LogicController(_state, _config));
  _animationManager.reset(new RoundAnimationManager(*this));
  _spaceController->setup();
  _logicController->setup();
  ofAddListener(_logicController->tryEndRoundEvent, this, &RoundController::onTryEndRound);
  ofAddListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  ofAddListener(_logicController->trySpawnBallEvent, this, &RoundController::onTrySpawnBall);
  
  _animationManager->attachTo(*_logicController);
  _logicController->attachTo(*_spaceController);
  
  _renderer.reset(new DomeRenderer(_state, _config));
  _renderer->setup();
  _renderer->attachTo(*_logicController);
  
  for (auto& msg : _config.startMessages()) {
    _animationManager->addMessage(msg);
  }
}

void RoundController::attachTo(AdminController &adminController) {
  ofAddListener(adminController.tryEndRoundEvent,
                this, &RoundController::onTryEndRound);
}

void RoundController::detachFrom(AdminController &adminController) {
  ofRemoveListener(adminController.tryEndRoundEvent,
                   this, &RoundController::onTryEndRound);
}

void RoundController::draw() {
  _renderer->draw();
}

template<typename T>
static void removeDeadPhysicalObjects(GameObjectCollection<T>& objects,
                                      SpaceController& spaceController) {
  for (auto& obj : objects.extractDeadObjects()) {
    spaceController.removeObject(*obj);
    obj.reset();
  }
}

void RoundController::update() {
  auto& appParams = BleepoutParameters::get();
  if (_paused && !appParams.paused) {
    _startTime = ofGetElapsedTimef() - _state.time;
    _paused = false;
  }
  _paused = appParams.paused;
  if (_paused)
    return;
  _state.time = ofGetElapsedTimef() - _startTime;
  
  if (_state.time >= _config.startDelay()) {
    if (_state.paddles().size() == 0) {
      ofLogNotice() << "Initial Paddle Create";
      _spaceController->addInitialPaddles();
    }
  }
  
  while (appParams.ballsToAdd) {
    appParams.ballsToAdd--;
    // add a new ball
    Ball& ball = _spaceController->addBall(BallSpec(30, ofRandom(360)));
    notifyBallSpawned(_state, &ball);
  }
  if (_spaceController)
    _spaceController->update();
  if (_logicController)
    _logicController->update();
  _timedActions.update(_state.time);
  if (_renderer)
    _renderer->update();
  if (_cullDeadObjectsPulser.update(_state.time)) {
    removeDeadPhysicalObjects(_state.balls(), *_spaceController);
    removeDeadPhysicalObjects(_state.bricks(), *_spaceController);
    removeDeadPhysicalObjects(_state.modifiers(), *_spaceController);
    removeDeadPhysicalObjects(_state.paddles(), *_spaceController);
  }
  
  if (_ending) {
    endRound();
  }
}

void RoundController::endRound() {
  auto results = buildRoundResults(_endReason);
  _ending = false;
  notifyRoundEnded(results);
}

void RoundController::onTryEndRound(EndRoundEventArgs &e) {
  _ending = true;
  _endReason = e.reason();
}

void RoundController::notifyRoundEnded(RoundResults &results) {
  RoundEndedEventArgs e(results);
  logEvent("RoundEnded", e);
  // after this notification is sent off, the roundcontroller ceases to exist!
  ofNotifyEvent(roundEndedEvent, e);
}

RoundResults RoundController::buildRoundResults(RoundEndReason reason) {
  RoundResults results(reason, _state);
  return results;
}

void RoundController::onTrySpawnBall(SpawnBallEventArgs &e) {
  Ball& ball = _spaceController->addBall(e.ballSpec());
  notifyBallSpawned(_state, &ball);
}

void RoundController::notifyBallSpawned(RoundState &state, Ball *ball) {
  BallStateEventArgs e(state, ball);
  logEvent("BallSpawned", e);
  ofNotifyEvent(ballSpawnedEvent, e);
}

void RoundController::onModifierAppeared(ModifierEventArgs& e) {
  if (!e.target() || e.target()->type() != GAME_OBJECT_BRICK) {
    ofLogError() << "Invalid spawn source for modifier";
    return;
  }
  _spaceController->setUpModifier(*e.modifier(), static_cast<Brick&>(*e.target()));
}

void RoundController::addAnimation(ofPtr<AnimationObject> animation) {
  _state.addAnimation(animation);
  auto updater = animation->createUpdaterAction(_state.time, _state.animations());
  addTimedAction(ofPtr<TimedAction>(updater));
}

void RoundController::addTimedAction(ofPtr<TimedAction> action) {
  _timedActions.add(action);
}

void RoundController::keyPressed(int key) {
  if (!ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    if (key == 'q') {
      EndRoundEventArgs e(END_ADMIN_OVERRIDE);
      onTryEndRound(e);
    } else if (key == 'l') {
      dumpToLog(OF_LOG_NOTICE);
    } else if (key == 'r') {
      dumpConfig(OF_LOG_NOTICE);
    } else if (key == 'o') {
      _logicController->toggleLogging(OF_LOG_NOTICE);
    }
  }
}

void RoundController::onPlayerYawPitchRoll(PlayerYawPitchRollEventArgs &e) {
  Paddle* paddle = e.player()->paddle();
  if (!paddle) {
    ofLogError() << "Unable to set paddle position for player: " << e.player()->id();
    return;
  }
  const auto& appParams = BleepoutParameters::get();
  
  paddle->setPositionCylindrical(appParams.domeRadius + appParams.domeMargin, 360 * e.yaw(), appParams.domeMargin);
}

void RoundController::setPaddlePosition(GameObjectId playerId, float xPercent) {
  ofPtr<Player> player = _state.players().getById(playerId);
  if (!player) {
    ofLogError() << "Player not found: " << playerId;
    return;
  }
  
  Paddle* paddle = player->paddle();
  if (!paddle) {
    ofLogError() << "Unable to set paddle position for player: " << playerId;
    return;
  }
  const auto& appParams = BleepoutParameters::get();
  
  paddle->setPositionCylindrical(appParams.domeRadius + appParams.domeMargin, 360 * xPercent, appParams.domeMargin);
}

void RoundController::mousePressed(int x, int y, int button) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    _renderer->mousePressed(x, y, button);
  } else {
    
  }
}

void RoundController::mouseMoved(int x, int y) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    _renderer->mouseMoved(x, y);
  } else if (!_state.players().empty()) {
    const ofPtr<Player>& player = _state.players().front();
    setPaddlePosition(player->id(), (float)x / ofGetWidth());
  }
}

void RoundController::mouseDragged(int x, int y, int button) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    _renderer->mouseDragged(x, y, button);
  } else {
    
  }
}

void RoundController::mouseReleased(int x, int y, int button) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    _renderer->mouseReleased(x, y, button);
  } else {
    
  }
}

void RoundController::dumpToLog(ofLogLevel level) const {
  ofLog(level) << "--BEGIN round state--";
  ofLog(level) << _state;
  ofLog(level) << "--  END round state--";
}

void RoundController::dumpConfig(ofLogLevel level) const {
  ofLog(level) << "--BEGIN round config--";
  ofLog(level) << toJsonVal(_config).toStyledString();
  ofLog(level) << "--  END round config--";
}
