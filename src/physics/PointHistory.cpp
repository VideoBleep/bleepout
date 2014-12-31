//
//  PointHistory.cpp
//  bleepout
//
//  Created by Dewb on 12/16/14.
//
//

#include "PointHistory.h"

void PointHistory::emitPoints() {
  int last = (_caret - 1) % _size;
  for (int i = last; i >= 0; i--) {
    glVertex3f(_buffer[i].x, _buffer[i].y, _buffer[i].z);
  }
  if (_filled) {
    for (int i = _size - 1; i > last; i--) {
      glVertex3f(_buffer[i].x, _buffer[i].y, _buffer[i].z);
    }
  }
}

void emitHalfQuad(ofVec3f point, ofVec3f nextPoint, float width, float zoffset) {
  ofVec3f d = nextPoint - point;
  ofVec3f z = point.normalized() * zoffset;
  d.cross(point).normalize().scale(width);
  ofVec3f a = point + d + z;
  ofVec3f b = point - d + z;
  glVertex3f(a.x, a.y, a.z);
  glVertex3f(b.x, b.y, b.z);
}

void PointHistory::emitQuadStrip(float width, float zoffset) {
  float hw = width * 0.5;
  int last = (_caret - 1) % _size;
  if (last > 0) {
    for (int i = last; i >= 1; i--) {
      emitHalfQuad(_buffer[i], _buffer[i-1], hw, zoffset);
    }
  }
  if (_filled) {
    for (int i = _size - 1; i > last; i--) {
      emitHalfQuad(_buffer[i], _buffer[i-1], hw, zoffset);
    }
  }
}