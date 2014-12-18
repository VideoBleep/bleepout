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
#include "TurntableCam.h"
#include <ofTrueTypeFont.h>
#include "RendererExtras.h"
#include "Geometry.h"

class DomeRenderer : public RendererBase {
public:
    virtual ~DomeRenderer() {}
    virtual void setup(RoundController& roundController) override;
    virtual void update() override;
    virtual void draw(RoundState& round, RoundConfig& config) override;
    
    virtual void keyPressed(int key);
    virtual void mousePressed(int x, int y, int button);
    virtual void mouseReleased(int x, int y, int button);
    virtual void mouseDragged(int x, int y, int button);

protected:
    virtual void drawBrick(RoundState& round, Brick& brick) override;
    virtual void drawPaddle(RoundState& round, Paddle& paddle) override;
    virtual void drawWall(RoundState& round, Wall& wall) override;
    virtual void drawBall(RoundState& round, Ball& ball) override;
    
    void drawGenMesh(const GenMesh& gm, const ofColor& faceColor, const ofColor& edgeColor, float lineWidth);
    
    ofxTurntableCam _cam;
    bool _debugGraphics;
    bool _drawTrajectories;
    bool _drawLasers;
    bool _drawCometTails;
    
    ofTrueTypeFont _font;
    RendererExtras _extras;
};

#endif /* defined(__bleepout__DomeRenderer__) */
