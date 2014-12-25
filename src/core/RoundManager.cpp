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

static const char roundControllerEventSource[] = "RoundController";

RoundController::RoundController(RoundConfig config,
                                 BleepoutParameters& appParams,
                                 std::list<ofPtr<Player> > players,
                                 PlayerManager& playerManager)
: _config(config)
, _appParams(appParams)
, _state(_config, players)
, _playerManager(playerManager)
, _timedActions(true)
, EventSource() {
}

const char* RoundController::eventSourceName() const {
  return roundControllerEventSource;
}

RoundController::~RoundController() {
  ofRemoveListener(_playerManager.playerYawPitchRollEvent, this, &RoundController::onPlayerYawPitchRoll);
  ofRemoveListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  ofRemoveListener(_logicController->tryEndRoundEvent, this, &RoundController::onTryEndRound);
  _logicController->detachFrom(*_spaceController);
  _renderer->detachFrom(*_logicController);
  _animationManager->detachFrom(*_logicController);
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
  _spaceController.reset(new SpaceController(_state, _config, _appParams));
  _logicController.reset(new LogicController(_state, _config, _appParams));
  _animationManager.reset(new AnimationManager(*this));
  _spaceController->setup();
  _logicController->setup();
  ofAddListener(_logicController->tryEndRoundEvent, this, &RoundController::onTryEndRound);
  ofAddListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  
  // for ease of debugging, disable exits initially
  for (auto& wall : _state.walls()) {
    if (wall->isExit()) {
      wall->kill();
    }
  }
  
  _animationManager->attachTo(*_logicController);
  _logicController->attachTo(*_spaceController);
  
  _renderer.reset(new DomeRenderer(_state, _config, _appParams));
  _renderer->setup();
  _renderer->attachTo(*_logicController);
  
  for (auto& msg : _config.startMessages()) {
    _animationManager->addMessage(msg);
  }
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
  if (_paused && !_appParams.paused) {
//    float footime = ofGetElapsedTimef() - _startTime;
//    float diff = footime - _state.time;
//    _startTime += diff;
    _startTime = ofGetElapsedTimef() - _state.time;
    _paused = false;
  }
  _paused = _appParams.paused;
  if (_paused)
    return;
  _state.time = ofGetElapsedTimef() - _startTime;
  
  if (_state.time >= _config.startDelay()) {
    if (_state.paddles().size() == 0) {
      ofLogNotice() << "Initial Paddle Create";
      _spaceController->addInitialPaddles();
    }
  }
  
  while (_appParams.ballsToAdd) {
    _appParams.ballsToAdd--;
    // add a new ball
    _spaceController->addBall(BallSpec(30, ofRandom(360)));
  }
  if (_spaceController)
    _spaceController->update();
  if (_logicController)
    _logicController->update();
  _timedActions.update(_state);
  if (_renderer)
    _renderer->update();
  if (_cullDeadObjectsPulser.update(_state.time)) {
    removeDeadPhysicalObjects(_state.balls(), *_spaceController);
    removeDeadPhysicalObjects(_state.bricks(), *_spaceController);
    removeDeadPhysicalObjects(_state.modifiers(), *_spaceController);
    removeDeadPhysicalObjects(_state.paddles(), *_spaceController);
  }
}

void RoundController::onModifierAppeared(ModifierEventArgs& e) {
  if (!e.target() || e.target()->type() != GAME_OBJECT_BRICK) {
    ofLogError() << "Invalid spawn source for modifier";
    return;
  }
  _spaceController->setUpModifier(*e.modifier(), static_cast<Brick&>(*e.target()));
}

void RoundController::onRoundEnded(RoundStateEventArgs &e) {
  _timedActions.clear();
}

void RoundController::onTryEndRound(EndRoundEventArgs &e) {
  notifyTryEndRound(e);
}

bool RoundController::notifyTryEndRound(EndRoundEventArgs &e) {
  ofNotifyEvent(tryEndRoundEvent, e);
  logEvent("TryEndRound", e);
  return e.handled();
}

void RoundController::addAnimation(ofPtr<AnimationObject> animation) {
  _state.addAnimation(animation);
  auto updater = animation->createUpdaterAction(_state);
  addTimedAction(ofPtr<TimedAction>(updater));
}

void RoundController::addTimedAction(ofPtr<TimedAction> action) {
  _timedActions.add(action);
}

void RoundController::keyPressed(int key) {
  if (!ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    if (key == 'q') {
      EndRoundEventArgs e;
      notifyTryEndRound(e);
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
  
  paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 360 * e.yaw(), _config.domeMargin());
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
  
  paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 360 * xPercent, _config.domeMargin());
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
  ofLog(level) << _config.toJsonVal().toStyledString();
  ofLog(level) << "--  END round config--";
}
