from object_specs import *
import random

def generate():
  config = {
    "name": "Round4",
    "ballRadius": 8,
    "modifierSize": (9, 16, 9),
    "objects": []
  }
  objects = config["objects"]

  for i in range(5):
    objects.append(createBall(30, i * (360 / 5)))

  superCharge = createModifier(name= "supercharge",
                              type= "SuperchargedBall",
                              duration= 20,
                              color= [255, 69, 0])

  objects += [superCharge]


  quadsColor1 = [0, 255, 0]
  quadsColor2 = [0, 127, 127]
  quadsSize = [7, 5, 14]
  objects += [
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=45,
                     elevationSpacing=1.5,
                     count=12,
                     headingSpacing=4,
                     modifier={"name": superCharge["name"], "chance": .4}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=55,
                     elevationSpacing=1.5,
                     count=12,
                     headingSpacing=4,
                     modifier={"name": superCharge["name"], "chance": .4}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=65,
                     elevationSpacing=1.5,
                     count=3,
                     headingSpacing=8,
                     modifier={"name": superCharge["name"], "chance": .4}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=75,
                     elevationSpacing=1.5,
                     count=3,
                     headingSpacing=12,
                     modifier={"name": superCharge["name"], "chance": .4})
  ]

  objects += [
    createBrickRing(elevation=60,
                    count=24,
                    color=[0, 0, 192],
                    size=[7, 5, 8]),
    createBrickRing(elevation=60,
                    count=24,
                    color=[0, 192, 255],
                    size=[7, 5, 8],
                    value=2,
                    lives=2,
                    phase=8)
  ]

  objects += [
    createWallRing(elevation=56,
                   count=12,
                   size=[7, 5, 15],
                   speed=0.6)
  ]

  objects += [
    createBrickRing(elevation=52,
                    count=28,
                    speed=0.6,
                    size=[7, 5, 10],
                    color=[0, 0, 192]),
    createBrickRing(elevation=52,
                    count=28,
                    size=[7, 5, 10],
                    color=[0, 192, 255],
                    value=2,
                    lives=2,
                    phase=6)
  ]

  objects += [
    createWallRing(elevation=48,
                   count=24,
                   size=[7, 5, 12])
  ]

  objects += [
    createBrickRing(elevation=44,
                    count=12,
                    lives=2,
                    value=2,
                    color=[0, 255, 127],
                    speed=0.15,
                    modifier={"name":superCharge["name"], "chance": .5}),
    createBrickRing(elevation=42,
                    count=28,
                    color=[0, 0, 192],
                    size=[7, 5, 10]),
    createBrickRing(elevation=42,
                    count=28,
                    color=[0, 192, 255],
                    size=[7, 5, 10],
                    value=2,
                    lives=2,
                    phase=6)
  ]

  objects += [
    createWallRing(elevation=38,
                   count=6,
                   speed=1.5,
                   size=[7, 5, 8])
  ]

  objects += [
    createWallRing(elevation=36,
                   count=12,
                   size=[7, 5, 12]),
    createWallRing(elevation=34,
                   count=12,
                   size=[7, 5, 10]),
    createWallRing(elevation=32,
                   count=12,
                   size=[7, 5, 8]),
    createWallRing(elevation=28,
                   count=12,
                   size=[7, 5, 8]),
    createWallRing(elevation=26,
                   count=12,
                   size=[7, 5, 8]),
    createWallRing(elevation=22,
                   count=12,
                   size=[7, 5, 8])
  ]

  # create the floor exit wall
  objects.append(createFloor(True))

  # messages
  objects += [
    createMessage(text="STAGE 4 START",
                  color=[0, 255, 0],
                  size=25,
                  delay=1,
                  duration=3)
  ]
  config["startDelay"] = 5

  # background ring sets
  objects += [
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0,0,0]),
                  spread=createValuePulser(0, 0.1, 10, [20, 20, 20]),
                  spreadOffset=[20, 20, 20],
                  count=30,
                  radiusScale=1.95,
                  lineWidth=2.2,
                  color=[0, 255, 0, 95]),
    createRingSet(spin=createValuePulser(0, 0.4, 5, [0,0,0]),
                  spread=createValuePulser(0, 0.5, 40, [0,0,0]),
                  spreadOffset=[60,60,60],
                  count=60,
                  radiusScale=2.3,
                  lineWidth=1.4,
                  color=[0, 255, 63, 95]),
    createRingSet(spin=createValuePulser(0, 0.2, 10, [0.2,0.2,0.2]),
                  spread=createValuePulser(0.01, 0.16, 15, [0.1,0.1,0.1]),
                  spreadOffset=[60,60,60],
                  count=50,
                  radiusScale=2,
                  lineWidth=.9,
                  color=[0, 127, 255, 95])
  ]


  return config

