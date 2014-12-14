//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"


void RendererBase::draw(RoundState &state, RoundConfig& config) {
    for (auto& obj : state.bricks()) {
        if (!obj->alive()) {
            continue;
        }
        drawBrick(state, *obj.get());
    }
    for (auto& obj : state.paddles()) {
        if (!obj->alive()) {
            continue;
        }
        drawPaddle(state, *obj.get());
    }
    for (auto& obj : state.balls()) {
        if (!obj->alive()) {
            continue;
        }
        drawBall(state, *obj.get());
    }
    for (auto& obj : state.walls()) {
        if (!obj->alive()) {
            continue;
        }
        drawWall(state, *obj.get());
    }
}