//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"

namespace {
    void drawBoxObject(PhysicsObject& object, ofColor edgeColor, ofColor fillColor) {
        ofPushMatrix();
        ofPushStyle();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofVec3f dims = object.getSize();
        ofTranslate(object.getPosition());
        ofRotateY(object.getRotation());
        ofNoFill();
        ofSetColor(edgeColor);
        ofDrawBox(ofVec3f::zero(), dims.x + 0.1, dims.y + 0.1, dims.z + 0.1);
        ofFill();
        ofSetColor(fillColor);
        ofDrawBox(ofVec3f::zero(), dims.x, dims.y, dims.z);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    template<typename T>
    void drawBoundingBoxes(GameObjectCollection<T> objects) {
        ofPushStyle();
        for (auto const &obj : objects) {
            ofSetColor(255, 0, 0);
            ofNoFill();
            BoundingBox bbox = obj->getBoundingBox();
            ofDrawBox(bbox.center.x, bbox.center.y, bbox.center.z,
                      bbox.halfwidths.x * 2, bbox.halfwidths.y * 2, bbox.halfwidths.z * 2);
        }
        ofPopStyle();
    }
    
    template<typename T>
    void drawTrajectories(GameObjectCollection<T> objects) {
        ofPushStyle();
        for (auto const &obj : objects) {
            auto t = obj->getTrajectory();
            if (t) {
                float r = t->getRadius();
                
                ofPushMatrix();
                ofVec3f normal(0, 0, -1);
                ofVec3f orbitNormal = t->u().crossed(t->w()).normalized();
                ofVec3f axis = normal.crossed(orbitNormal);
                float angle = acos(normal.dot(orbitNormal));
                ofRotate(angle * 180/PI, axis.x, axis.y, axis.z);
                ofSetColor(255, 0, 0);
                ofNoFill();
                ofCircle(0, 0, 0, r);
                ofPopMatrix();
                
                ofFill();
                ofDrawArrow(ofVec3f::zero(), t->u() * r, 2);
                ofLine(ofVec3f::zero(), t->v() * r);
                ofColor(128, 0, 0);
                ofDrawArrow(ofVec3f::zero(), t->u().crossed(t->v()) * r/4.0, 2);
            }
        }
        ofPopStyle();
    }
}


void DomeRenderer::setup() {
    _cam.setTarget(ofVec3f(0.0, 25.0, 0.0));
    _cam.setRotation(0.66, 0.5);
    _cam.setupPerspective(false);
    ofEnableDepthTest();
    ofSetCircleResolution(64);
    _debugGraphics = false;
}

void DomeRenderer::draw(RoundState &state, RoundConfig& config) {

    ofSetColor(80, 80, 80);
    ofDrawBitmapString("command + mouse to rotate camera\ncommand + d for physics debugging info", 10, 20);
    
    _cam.setDistance(config.domeRadius() * 2);
    _cam.begin();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(80, 80, 110);
    ofNoFill();
    ofRotateX(90);
    ofCircle(0, 0, 0, config.domeRadius());

    ofPopStyle();
    ofPopMatrix();
    
    RendererBase::draw(state, config);
    
    if (_debugGraphics) {
        drawBoundingBoxes(state.balls());
        drawBoundingBoxes(state.paddles());
        drawBoundingBoxes(state.bricks());
        drawBoundingBoxes(state.walls());
        drawTrajectories(state.balls());
    }
    
    _cam.end();
}

void DomeRenderer::keyPressed(int key) {
    if (key == 'd') {
        _debugGraphics = !_debugGraphics;
    }
}

void DomeRenderer::mousePressed(int x, int y, int button) {
    _cam.mousePressed(x, y, button);
}

void DomeRenderer::mouseReleased(int x, int y, int button) {
    _cam.mouseReleased(x, y, button);
}

void DomeRenderer::mouseDragged(int x, int y, int button) {
    _cam.mouseDragged(x, y, button);
}

void DomeRenderer::drawBrick(RoundState& round, Brick &brick) {
    drawBoxObject(brick, ofColor(255, 255, 255), brick.getColor());
}

void DomeRenderer::drawPaddle(RoundState& round, Paddle &paddle) {
    drawBoxObject(paddle, ofColor(0, 0, 0), ofColor(0, 200, 60));
}

void DomeRenderer::drawWall(RoundState& round, Wall &wall) {
    drawBoxObject(wall, ofColor(180, 180, 180), ofColor(100, 100, 100));
}

void DomeRenderer::drawBall(RoundState& round, Ball &ball) {
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofTranslate(ball.getPosition());
    ofRotateX(360 * ball.getTrajectory()->getTime());
    ofRotateY(45);
    ofCircle(ofVec3f::zero(), ball.getSize().x / 2.0 + 0.1);
    ofFill();
    ofSetColor(255, 255, 255);
    ofDrawSphere(ofVec3f::zero(), ball.getSize().x / 2.0);

    ofPopStyle();
    ofPopMatrix();
}