//
//  RendererBase.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__RendererBase__
#define __bleepout__RendererBase__

#include "RoundManager.h"

class RendererBase {
public:
  virtual void draw(const RoundController& round) = 0;
};

#endif /* defined(__bleepout__RendererBase__) */
