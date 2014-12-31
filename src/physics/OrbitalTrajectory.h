//
//  OrbitalTrajectory.h
//  bleepout
//
//  Created by Dewb on 12/9/14.
//
//

#ifndef __bleepout__OrbitalTrajectory__
#define __bleepout__OrbitalTrajectory__

#include "Trajectory.h"
#include "PointHistory.h"


class OrbitalTrajectory : public Trajectory {
public:
  OrbitalTrajectory();
  virtual ~OrbitalTrajectory() {}
  OrbitalTrajectory(float radius, ofVec3f start, ofVec3f through, float speed = 0.02);
  
  void initWithTwoPoints(ofVec3f start, ofVec3f through);
  void initWithTwoPoints(float elevation1, float heading1, float elevation2, float heading2);
  
  float getRadius() const { return _r; }
  void setRadius(float r) { _r = r; }
  
  virtual void setPosition(const ofVec3f& position) override;
  
  virtual void reflect(const ofVec3f& planeNormal, float trueHitFactor = 0.0) override;
  
  virtual void output(std::ostream& os) const override;
  
  const ofVec3f& u() const { return _u; }
  const ofVec3f& v() const { return _v; }
  const ofVec3f& w() const { return _w; }
  
  PointHistory history;
  
protected:
  virtual void tick(float delta) override;
  
  float _r;
  
  ofVec3f _u;
  ofVec3f _v;
  ofVec3f _w;
};

#endif /* defined(__bleepout__OrbitalTrajectory__) */
