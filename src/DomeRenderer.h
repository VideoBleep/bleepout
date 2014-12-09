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
    virtual void draw(RoundState& round) override;
protected:
    virtual void drawBrick(RoundState& round, Brick& brick) override;
    virtual void drawPaddle(RoundState& round, Paddle& paddle) override;
    virtual void drawBall(RoundState& round, Ball& ball) override;
};

#endif /* defined(__bleepout__DomeRenderer__) */
