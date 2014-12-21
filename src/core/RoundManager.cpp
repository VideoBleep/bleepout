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

RoundController::RoundController(RoundConfig config,
                                 std::list<ofPtr<Player> > players,
                                 PlayerManager& playerManager)
: _config(config)
, _state(_config, players)
, _playerManager(playerManager)
, _timedActions(true) {
  ofAddListener(playerManager.playerYawPitchRollEvent, this, &RoundController::onPlayerYawPitchRoll);
}

RoundController::~RoundController() {
  ofRemoveListener(_playerManager.playerYawPitchRollEvent, this, &RoundController::onPlayerYawPitchRoll);
  ofRemoveListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  ofRemoveListener(_logicController->modifierAppliedEvent, this, &RoundController::onModifierApplied);
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
   
  _spaceController.reset(new SpaceController(_state, _config));
  _logicController.reset(new LogicController(_state, _config));
  _animationManager.reset(new AnimationManager(*this));
  _spaceController->setup();
  _logicController->setup();
  ofAddListener(_logicController->roundEndedEvent, this, &RoundController::onRoundEnded);
  ofAddListener(_logicController->modifierAppearedEvent, this, &RoundController::onModifierAppeared);
  ofAddListener(_logicController->modifierAppliedEvent, this, &RoundController::onModifierApplied);
    
  // for ease of debugging, disable exits initially
  for (auto& wall : _state.walls()) {
    if (wall->isExit()) {
      wall->kill();
    }
  }
  
  _animationManager->attachTo(*_logicController);
  _logicController->attachTo(*_spaceController);

  _renderer.reset(new DomeRenderer());
  _renderer->setup(_config);
  _renderer->attachTo(*_logicController);
  
  for (auto& msg : _config.startMessages()) {
    _animationManager->addMessage(msg);
  }
}

void RoundController::draw() {
  _renderer->draw(_state);
}

void RoundController::update() {
  _state.time = ofGetElapsedTimef() - _startTime;

  if (_state.time >= _config.startDelay()) {
    if (_state.paddles().size() == 0) {
      ofLogNotice() << "Initial Paddle Create";
      _spaceController->addInitialPaddles();
    }
  }

  _spaceController->update();
  _logicController->update();
  _timedActions.update(_state);
  _renderer->update();
}

void RoundController::onModifierAppeared(ModifierEventArgs& e) {
  if (!e.target() || e.target()->type() != GAME_OBJECT_BRICK) {
    ofLogError() << "Invalid spawn source for modifier";
    return;
  }
  _spaceController->setUpModifier(*e.modifier(), static_cast<Brick&>(*e.target()));
}

void RoundController::onModifierApplied(ModifierEventArgs &e) {
  _spaceController->removeModifier(*e.modifier());
}

void RoundController::onRoundEnded(RoundStateEventArgs &e) {
  ofNotifyEvent(roundEndedEvent, e);
}

void RoundController::addAnimation(ofPtr<AnimationObject> animation) {
  _state.addAnimation(animation);
  auto updater = animation->createUpdaterAction(_state.animations());
  addTimedAction(ofPtr<TimedAction>(updater));
}

void RoundController::addTimedAction(ofPtr<TimedAction> action) {
  _timedActions.add(action);
}

void RoundController::keyPressed(int key) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    _renderer->keyPressed(key);
  } else {
    if (key == 'q') {
      RoundStateEventArgs e(_state);
      ofNotifyEvent(roundEndedEvent, e);
      //....
    } else if (key == 'l') {
      dumpToLog(OF_LOG_NOTICE);
    } else if (key == 'r') {
      dumpConfig(OF_LOG_NOTICE);
    } else if (key == 'p') {
      if (_spaceController->loggingEnabled())
        _spaceController->disableLogging();
      else
        _spaceController->enableLogging(OF_LOG_NOTICE);
    } else if (key == 'o') {
      if (_logicController->loggingEnabled())
        _logicController->disableLogging();
      else
        _logicController->enableLogging(OF_LOG_NOTICE);
    } else if (key == 'e') {
        // toggle exits on and off
        for (auto& wall : _state.walls()) {
            if (wall->isExit()) {
                if (wall->alive()) {
                    wall->kill();
                } else {
                    wall->revive();
                }
            }
        }
    } else if (key == 'b') {
        // add a new ball
        _spaceController->addBall(BallSpec(30, ofRandom(360)));
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
