def build_color(color):
  if not color:
    return None
  d = dict(r = color[0], g = color[1], b = color[2])
  if len(color) == 4:
    d['a'] = color[3]
  else:
    d['a'] = 255
  return d

class BrickSpec:
  def __init__(self, elevation = 0, heading = 0, size = None, value = 1,
               lives = 1, color = None, speed = 0,
               stopHeading = -1, modifier = None):
    self.elevation = elevation
    self.heading = heading
    self.size = size if size else (7.0, 5.0, 17.0)
    self.value = value
    self.lives = lives
    self.color = color if color else (0, 255, 0, 255)
    self.speed = speed
    self.stopHeading = stopHeading
    self.modifier = modifier

  def build(self):
    d = dict(elevation = self.elevation,
             heading = self.heading,
             size = self.size,
             value = self.value,
             lives = self.lives,
             color = build_color(self.color))
    if self.speed and self.speed != 0:
      d['speed'] = self.speed
    if self.stopHeading:
      d['stopHeading'] = self.stopHeading
    if self.modifier:
      d['modifier'] = self.modifier
    return d

class BrickRingSpec:
  def __init__(self, elevation = 0, count = 1, phase = 0, size = None,
               value = 1, lives = 1, color = None, speed = 0,
               stopHeading = -1):
    self.elevation = elevation
    self.count = count
    self.phase = phase
    self.size = size if size else (7.0, 5.0, 17.0)
    self.value = value
    self.lives = lives
    self.color = color if color else (0, 255, 0, 255)
    self.speed = speed
    self.stopHeading = stopHeading

  def build(self):
    d = dict(elevation = self.elevation,
             count = self.count,
             size = self.size,
             value = self.value,
             lives = self.lives,
             color = build_color(self.color))
    if self.phase:
      d['phase'] = self.phase
    if self.speed:
      d['speed'] = self.speed
    if self.stopHeading:
      d['stopHeading'] = self.stopHeading
    return d

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
             color = build_color(self.color),
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
             color = build_color(self.color))
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
