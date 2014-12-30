
defaultBrickSize = [7, 5, 17]

def createBrickSpec(elevation = 30,
                    heading = 0,
                    speed = 0,
                    stopHeading = -1,
                    lives = 1,
                    value = 1,
                    modifier = None,
                    color = None,
                    size = None):
  return dict(_type = "Brick",
              elevation = elevation,
              heading = heading,
              size = size if size else defaultBrickSize,
              value = value,
              lives = lives,
              color = color if color else [255, 0, 255],
              speed = speed,
              stopHeading = stopHeading,
              modifier = modifier)

def createBrickRing(elevation = 0, size = None, color = None, value = 1, lives = 1,
                    count = 1, phase = 0, speed = 0, stopHeading = -1,
                    modifierName = "", modifierChance = 1):
  return dict(_type = "BrickRing",
              elevation = elevation,
              size = size if size else defaultBrickSize,
              color = color if color else [255, 0, 255],
              value = value,
              lives = lives,
              count = count,
              phase = phase,
              speed = speed,
              stopHeading = stopHeading,
              modifierName = modifierName,
              modifierChance = modifierChance)

def createBrickQuads(elevation = 0, color1 = None, color2 = None, count = 1,
                     elevationSpacing = 5, headingSpacing = 5, size = None,
                     speed = 0, stopHeading = -1, modifierName = "", modifierChance = 1):
  return dict(_type = "BrickQuads",
              elevation = elevation,
              size = size if size else defaultBrickSize,
              color1 = color1 if color1 else [255, 0, 255],
              color2 = color2 if color2 else [255, 255, 0],
              count = count,
              elevationSpacing = elevationSpacing,
              headingSpacing = headingSpacing,
              speed = speed,
              stopHeading = stopHeading,
              modifierName = modifierName,
              modifierChance = modifierChance)

class WallSpec:
  def __init__(self, elevation = 0, heading = 0, size = None,
               speed = 0, stopHeading = None, isExit = False, visible = True):
    self.elevation = elevation
    self.heading = heading
    self.size = size if size else (7.0, 5.0, 17.0)
    self.speed = speed
    self.stopHeading = stopHeading
    self.isExit = isExit
    self.visible = visible

  def build(self):
    d = dict(elevation = self.elevation,
             heading = self.heading,
             size = self.size,
             visible = self.visible,
             isExit = self.isExit)
    if self.speed and self.speed != 0:
      d['speed'] = self.speed
    if self.stopHeading:
      d['stopHeading'] = self.stopHeading
    return d

class WallRingSpec:
  def __init__(self, elevation = 0, count = 1, phase = 0, size = None,
               speed = 0, stopHeading = None, isExit = False, visible = True):
    self.elevation = elevation
    self.count = count
    self.phase = phase
    self.size = size if size else (7.0, 5.0, 17.0)
    self.speed = speed
    self.stopHeading = stopHeading
    self.isExit = isExit
    self.visible = visible

  def build(self):
    d = dict(elevation = self.elevation,
             count = self.count,
             size = self.size,
             visible = self.visible,
             isExit = self.isExit)
    if self.phase:
      d['phase'] = self.phase
    if self.speed:
      d['speed'] = self.speed
    if self.stopHeading:
      d['stopHeading'] = self.stopHeading
    return d

class CurvedWallSpec:
  def __init__(self, elevation1 = 0, heading1 = 0, elevation2 = 0, heading2 = 0,
               width = 5, speed = 0, stopHeading = None, isExit = False):
    self.elevation1 = elevation1
    self.heading1 = heading1
    self.elevation2 = elevation2
    self.heading2 = heading2
    self.width = width
    self.speed = speed
    self.stopHeading = stopHeading
    self.isExit = isExit

  def build(self):
    d = dict(elevation1 = self.elevation1,
             heading1 = self.heading1,
             elevation2 = self.elevation2,
             heading2 = self.heading2,
             width = self.width,
             isExit = self.isExit)
    if self.speed:
      d['speed'] = self.speed
    if self.stopHeading:
      d['stopHeading'] = self.stopHeading
    return d

class ModifierType:
  Unknown = 'None'
  ExtraLife = 'ExtraLife'
  PaddleWidth = 'PaddleWidth'
  LaserBall = 'LaserBall'

class ModifierSpec:
  def __init__(self, type, name, amount = 0, duration = 0, color = None):
    self.type = type
    self.name = name
    self.amount = amount
    self.duration = duration
    self.color = color if color else (255, 255, 0, 255)

  def build(self):
    d = dict(type = self.type,
             name = self.name,
             amount = self.amount,
             duration = self.duration,
             color = build_color(self.color))
    return d

class MessageSpec:
  def __init__(self, text = None, color = None, size = 10, trails = 0, delay = 0, duration = 10):
    self.text = text
    self.color = color if color else (255, 255, 255, 255)
    self.size = size
    self.trails = trails
    self.delay = delay
    self.duration = duration

  def build(self):
    d = dict(text = self.text,
             color = self.color,
             size = self.size,
             trails = self.trails,
             delay = self.delay,
             duration = self.duration)
    return d

class ValuePulserSpec:
  def __init__(self, minRate = 0, maxRate = 0.5, interval = 10, start = None):
    self.minRate = minRate
    self.maxRate = maxRate
    self.interval = interval
    self.start = start

  def build(self):
    d = dict(minRate = self.minRate,
             maxRate = self.maxRate,
             interval = self.interval,
             start = self.start)
    return d

class RingSetSpec:
  def __init__(self, spin = None, spread = None, spreadOffset = None, count = 10,
               radiusScale = 1.2, lineWidth = 1, color = None):
    self.spin = spin
    self.spread = spread
    self.spreadOffset = spreadOffset
    self.count = count
    self.radiusScale = radiusScale
    self.lineWidth = lineWidth
    self.color = color if color else (0, 255, 255, 255)

  def build(self):
    d = dict(spin = self.spin.build(),
             spread = self.spread.build(),
             spreadOffset = self.spreadOffset,
             count = self.count,
             radiusScale = self.radiusScale,
             lineWidth = self.lineWidth,
             color = self.color)
    return d

def build_all(objects):
  return [o.build() for o in objects]

class RoundConfig:
  def __init__(self, name):
    self.name = name
    self.startDelay = 0
    self.paddleSize = (16.0, 8.0, 40.0)
    self.ballRadius = 8
    self.brickFadeTime = 0.4
    self.modifierRadius = 9
    self.modifierFadeTime = 0.2
    self.domeRadius = 150
    self.domeMargin = 20
    self.balls = []
    self.bricks = []
    self.brickRings = []
    self.walls = []
    self.wallRings = []
    self.curvedWallSets = []
    self.modifierDefs = []
    self.startMessages = []
    self.ringSets = []

  def build(self):
    d = dict(name = self.name,
             startDelay = self.startDelay,
             paddleSize = self.paddleSize,
             ballRadius = self.ballRadius,
             brickFadeTime = self.brickFadeTime,
             modifierRadius = self.modifierRadius,
             modifierFadeTime = self.modifierFadeTime,
             domeRadius = self.domeRadius,
             domeMargin = self.domeMargin,
             balls = build_all(self.balls),
             bricks = build_all(self.bricks),
             brickRings = build_all(self.brickRings),
             walls = build_all(self.walls),
             wallRings = build_all(self.wallRings),
             curvedWallSets = build_all(self.curvedWallSets),
             modifierDefs = build_all(self.modifierDefs),
             startMessages = build_all(self.startMessages),
             ringSets = build_all(self.ringSets))
    return d

class LogLevel:
  Verbose = 'Verbose'
  Notice = 'Notice'
  Warning = 'Warning'
  Error = 'Error'
  FatalError = 'FatalError'
  Silent = 'Silent'

class BleepoutConfig:
  def __init__(self):
    self.fps = 30
    self.logLevel = LogLevel.Notice
    self.vsync = True
    self.syphonServerName = 'Composition'
    self.syphonAppName = 'Arena'
    self.roundConfigs = []

  def build(self):
    d = dict(fps = self.fps,
             logLevel = self.logLevel,
             vsync = self.vsync,
             syphonServerName = self.syphonServerName,
             syphonAppName = self.syphonAppName,
             roundConfigs = build_all(self.roundConfigs))
    return d
