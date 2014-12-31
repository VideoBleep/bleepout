
def createBrick(elevation = None,
                heading = None,
                speed = None,
                stopHeading = None,
                lives = None,
                value = None,
                modifier = None,
                color = None,
                size = None):
  return dict(_type = "Brick",
              elevation = elevation,
              heading = heading,
              size = size,
              value = value,
              lives = lives,
              color = color,
              speed = speed,
              stopHeading = stopHeading,
              modifier = modifier)

def createBrickRing(elevation = None,
                    phase = None,
                    speed = None,
                    stopHeading = None,
                    count = None,
                    lives = None,
                    value = None,
                    modifier = None,
                    color = None,
                    size = None):
  return dict(_type = "BrickRing",
              elevation = elevation,
              size = size,
              color = color,
              value = value,
              lives = lives,
              count = count,
              phase = phase,
              speed = speed,
              stopHeading = stopHeading,
              modifier = modifier)

def createBrickQuads(elevation = None,
                     elevationSpacing = None,
                     headingSpacing = None,
                     speed = None,
                     stopHeading = None,
                     count = None,
                     modifier = None,
                     color1 = None,
                     color2 = None,
                     size = None,
                     phase = None):
  return dict(_type = "BrickQuads",
              elevation = elevation,
              elevationSpacing = elevationSpacing,
              headingSpacing = headingSpacing,
              speed = speed,
              stopHeading = stopHeading,
              count = count,
              modifier = modifier,
              color1 = color1,
              color2 = color2,
              size = size,
              phase = phase)

def createWall(isExit = False,
               isFloor = False,
               visible = True,
               elevation = 30,
               heading = 0,
               speed = 0,
               stopHeading = -1,
               size = None):
  return dict(_type = "Wall",
              isExit = isExit,
              isFloor = isFloor,
              visible = visible,
              elevation = elevation,
              heading = heading,
              speed = speed,
              stopHeading = stopHeading,
              size = size)

def createFloor(isExit = False,
                elevation = -10):
  return createWall(isExit= isExit,
                    isFloor= True,
                    visible= False,
                    elevation= elevation)

def createWallRing(isExit = None,
                   visible = None,
                   elevation = None,
                   phase = None,
                   speed = None,
                   stopHeading = None,
                   count = None,
                   size = None):
  return dict(_type = "WallRing",
              isExit = isExit,
              visible = visible,
              elevation = elevation,
              phase = phase,
              speed = speed,
              stopHeading = stopHeading,
              count = count,
              size = size)

def createCurvedBrickCol(elevation1 = None,
                         elevation2 = None,
                         heading1 = None,
                         heading2 = None,
                         phase = None,
                         speed = None,
                         stopHeading = None,
                         count = None,
                         color1 = None,
                         color2 = None,
                         size = None,
                         stripe1 = None,
                         stripe2 = None,
                         stripe3 = None):
  return dict(_type = "CurvedBrickColumn",
              elevation1 = elevation1,
              elevation2 = elevation2,
              heading1 = heading1,
              heading2 = heading2,
              phase = phase,
              speed = speed,
              stopHeading = stopHeading,
              count = count,
              color1 = color1,
              color2 = color2,
              size = size,
              stripe1 = stripe1,
              stripe2 = stripe2,
              stripe3 = stripe3)


def createCurvedWall(isExit = False,
                     elevation1 = None,
                     elevation2 = None,
                     heading1 = None,
                     heading2 = None,
                     speed = None,
                     stopHeading = None,
                     width = None):
  return dict(_type = "CurvedWall",
              isExit = isExit,
              elevation1 = elevation1,
              elevation2 = elevation2,
              heading1 = heading1,
              heading2 = heading2,
              speed = speed,
              stopHeading = stopHeading,
              width = width)

def createBall(elevation = None,
               heading = None):
  return dict(_type = "Ball",
              elevation = elevation,
              heading = heading)

def createModifier(type = None,
                   name = None,
                   amount = None,
                   duration = None,
                   color = None,
                   beneficial = None):
  return dict(_type = "Modifier",
              type = type,
              name = name,
              amount = amount,
              duration = duration,
              color = color if color else [0, 255, 0],
              beneficial = beneficial)

def createMessage(text = None,
                  color = None,
                  size = None,
                  trails = None,
                  delay = None,
                  duration = None):
  return dict(_type = "Message",
              text = text,
              color = color if color else [255, 0, 255],
              size = size,
              trails = trails,
              delay = delay,
              duration = duration)

def createValuePulser(minRate = None,
                      maxRate = None,
                      changeInterval = None,
                      startValue = None):
  return dict(minRate = minRate,
              maxRate = maxRate,
              changeInterval = changeInterval,
              startValue = startValue)

def createRingSet(spin = None,
                  spread = None,
                  spreadOffset = None,
                  count = None,
                  radiusScale = None,
                  lineWidth = None,
                  color = None):
  return dict(_type= "RingSet",
              spin = spin,
              spread = spread,
              spreadOffset = spreadOffset,
              count = count,
              radiusScale = radiusScale,
              lineWidth = lineWidth,
              color = color)
