//
//  PointHistory.h
//  bleepout
//
//  Created by Dewb on 12/16/14.
//
//

#ifndef __bleepout__PointHistory__
#define __bleepout__PointHistory__

#include <ofTypes.h>

// could include boost::circular_buffer, or could just hack something up...

template<class T>
class SimpleRingBuffer {
public:
  SimpleRingBuffer(int size) {
    _size = size;
    _caret = 0;
    _buffer = (T*)malloc(size * sizeof(T));
    _filled = false;
  }
  
  ~SimpleRingBuffer() {
    free(_buffer);
  }
  
  void push(const T& item) {
    _buffer[_caret] = item;
    _caret = (_caret + 1) % _size;
    if (_caret == 0) { _filled = true; }
  }
  
protected:
  int _size;
  bool _filled;
  int _caret;
  T* _buffer;
};

class PointHistory : public SimpleRingBuffer<ofVec3f> {
public:
  PointHistory() : SimpleRingBuffer(50) {}
  void emitPoints();
  void emitQuadStrip(float width, float zoffset);
};

#endif /* defined(__bleepout__PointHistory__) */
