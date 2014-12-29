//
//  Geometry.h
//  bleepout
//
//  Created by Dewb on 12/18/14.
//
//

#ifndef __bleepout__Geometry__
#define __bleepout__Geometry__

#include <vector>
#include <ofTypes.h>
#include <ofMesh.h>

class PointList {
public:
  std::vector<ofVec3f> points;
  void addPoint(const ofVec3f& pt) { points.push_back(pt); }
  void addPoint(float x, float y, float z) { points.push_back(ofVec3f(x, y, z)); }
};

class Path : public PointList {
public:
};

class FaceLoop : public PointList {
public:
};

class GenMesh {
public:
  virtual void generate(bool merge = true) = 0;
  
  ofPtr<ofMesh> mesh;
  ofPtr<ofMesh> outline;
};

class Sweep : public GenMesh {
public:
  virtual void generate(bool merge = true) override;
  
  FaceLoop startFace;
  Path path;
};

template<typename T>
T lerp(const T& v1, const T& v2, float s) {
  return v1 + (v2 - v1) * s;
}

#endif /* defined(__bleepout__Geometry__) */
