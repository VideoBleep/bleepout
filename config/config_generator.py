import json

def build_color(color):
  if not color:
    return None
  d = dict(r = color[0], g = color[1], b = color[2])
  if len(color) == 4:
    d['a'] = color[3]
  else:
    d['a'] = 255
  return d

def build_vec3f(vec):
  if not vec:
    return None
  return dict(x = vec[0], y = vec[1], z = vec[2])

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
             size = build_vec3f(self.size),
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
             size = build_vec3f(self.size),
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
  def __init__(self, type = None, name = None, amount = 0, duration = 0, color = None):
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

def generate_all():
  pass


