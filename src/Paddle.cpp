//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"

Paddle::Paddle(Player* player)
: GameObject(GAME_OBJECT_PADDLE), _player(player), _constraint(NULL) {
  ofLogVerbose() << "Create Paddle";
}

Paddle::~Paddle() {
  ofLogVerbose() << "Destroy Paddle";
}

void Paddle::output(std::ostream &os) const {
  ofVec3f pos = getPosition();
  os << "Paddle{id:" << id() << ", pos:" << pos << "}";
  if (_player)
    os << ", player:" << _player->id();
  else
    os << ", player: NONE";
  os << "}";
}

void Paddle::initConstraint(ofxBulletWorldRigid& world) {
    
    if (_constraint != NULL) {
        return;
    }
    
    btTransform tr;
    tr.setIdentity();
    tr.setOrigin(btVector3(0, 0, 0));
    
    _constraint = new btGeneric6DofConstraint(*getRigidBody(), tr, false);
    _constraint->setLinearLowerLimit(btVector3(0,0,0));
    _constraint->setLinearUpperLimit(btVector3(0,0,0));
    _constraint->setAngularLowerLimit(btVector3(0,0,0));
    _constraint->setAngularUpperLimit(btVector3(0,0,0));
    
    world.world->addConstraint(_constraint);
    
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
    _constraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
    
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
    _constraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
}


void Paddle::constrainTo(const ofVec3f& v) {
   	if (_constraint) {
        _constraint->getFrameOffsetA().setOrigin(btVector3(v.x, v.y, v.z));
    }
}