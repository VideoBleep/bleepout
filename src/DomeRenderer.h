//
//  DomeRenderer.h
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#ifndef __bleepout__DomeRenderer__
#define __bleepout__DomeRenderer__

#include "RendererBase.h"
#include "RoundManager.h"

class DomeRenderer : public RendererBase {
public:
    virtual ~DomeRenderer() {}
    virtual void init() override;
    virtual void draw(RoundController& round) override;
protected:
    virtual void drawBrick(RoundController& round, Brick& brick) override;
    virtual void drawPaddle(RoundController& round, Paddle& paddle) override;
    virtual void drawBall(RoundController& round, Ball& ball) override;
};

#endif /* defined(__bleepout__DomeRenderer__) */
