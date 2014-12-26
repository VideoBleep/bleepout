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
#include "RendererExtras.h"
#include "Geometry.h"

class RoundConfig;

class DomeRenderer : public RendererBase {
public:
  DomeRenderer(RoundState& state,
               const RoundConfig& config,
               const BleepoutParameters& appParams);
  virtual ~DomeRenderer() {}
  virtual void setup() override;
  virtual void update() override;
  virtual void draw() override;
  
  virtual void mousePressed(int x, int y, int button);
  virtual void mouseReleased(int x, int y, int button);
  virtual void mouseDragged(int x, int y, int button);
  
protected:
  virtual void drawBrick(Brick& brick) override;
  virtual void drawPaddle(Paddle& paddle) override;
  virtual void drawWall(Wall& wall) override;
  virtual void drawBall(Ball& ball) override;
  virtual void drawModifier(Modifier& modifier) override;
  
  void drawGenMesh(const GenMesh& gm, ofMaterial& faceColor, const ofColor& edgeColor, float lineWidth);
  
  ofxTurntableCam _cam;
  
  RendererExtras _extras;
  
  vector<ofLight> lights;
  ofMaterial wallMaterial;
};

#endif /* defined(__bleepout__DomeRenderer__) */
