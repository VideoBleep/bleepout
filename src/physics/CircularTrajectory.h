//
//  CircularTrajectory.h
//  bleepout
//
//  Created by Dewb on 12/16/14.
//
//

#ifndef __bleepout__CircularTrajectory__
#define __bleepout__CircularTrajectory__

#include <ofTypes.h>
#include <Trajectory.h>

class CircularTrajectory : public Trajectory {
public:
  CircularTrajectory();
  CircularTrajectory(float domeRadius, float speed, float elevation, float startHeading, float stopHeading = -1);
  
  void initWithElevationHeading(float elevation, float startHeading, float stopHeading = -1);
  
  float getRadius() const { return _r; }
  void setRadius(float r) { _r = r; }
  
  virtual float getRotation() const override;
  
  virtual void setPosition(const ofVec3f& position) override;
  
  virtual void reflect(const ofVec3f& planeNormal, float trueHitFactor = 0.0) override;
  
  virtual void output(std::ostream& os) const override;
  
  
protected:
  virtual void tick() override;
  
  float _r;
  float _elevation;
  float _startHeading;
  float _stopHeading;
  
  float _phi;
  float _circleRadius;
  float _height;
};


#endif /* defined(__bleepout__CircularTrajectory__) */
