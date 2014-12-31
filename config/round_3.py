from object_specs import *
import random

def generate():
  config = {
    "name": "Round3foo",
    "ballRadius": 8,
    "modifierSize": (9, 16, 9),
    "rules": {
      "ballsRespawn": True
    },
    "objects": []
  }
  objects = config["objects"]

  for i in range(5):
    objects.append(createBall(30, i * (360 / 5)))

  widePaddle = createModifier(name= "widePaddle",
                              type= "PaddleWidth",
                              amount= 1.5,
                              duration= 5,
                              color= [0, 0, 255])
  narrowPaddle = createModifier(name= "narrowPaddle",
                                type= "PaddleWidth",
                                amount= .5,
                                duration= 5,
                                color= [255, 0, 0],
                                beneficial= False)

  superCharge = createModifier(name= "supercharge",
                              type= "SuperchargedBall",
                              duration= 20,
                              color= [255, 69, 0])

  objects += [widePaddle, narrowPaddle, superCharge]


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
                     modifier= {"name": widePaddle["name"], "chance": .2}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=55,
                     elevationSpacing=1.5,
                     count=12,
                     headingSpacing=4,
                     modifier= {"name": superCharge["name"], "chance": .1}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=65,
                     elevationSpacing=1.5,
                     count=3,
                     headingSpacing=8,
                     modifier= {"name": narrowPaddle["name"], "chance": .2}),
    createBrickQuads(color1=quadsColor1,
                     color2=quadsColor2,
                     size=quadsSize,
                     elevation=75,
                     elevationSpacing=1.5,
                     count=3,
                     headingSpacing=12),
    createBrickRing(elevation=30,
                    count=24,
                    color=[0, 255, 255],
                    size=[7, 5, 17])
  ]

  objects += [
    createWallRing(elevation=65,
                   phase=60,
                   count=3,
                   size=[8, 3, 28]),
    createWallRing(elevation=75,
                   phase=60,
                   count=3,
                   size=[8, 3, 26]),
    createWallRing(elevation=60,
                   phase=360/24,
                   count=12,
                   size=[8, 3, 25]),
    createWallRing(elevation=55,
                   phase=360/24,
                   count=12,
                   size=[8, 3, 20]),
    createWallRing(elevation=50,
                   phase=360/24,
                   count=12,
                   size=[8, 3, 15]),
    createWallRing(elevation=45,
                   phase=360/24,
                   count=12,
                   size=[8, 3, 10])
  ]

  # moving lower walls
  objects += [
    createWallRing(elevation=35,
                   count=6,
                   size=[8, 3, 25],
                   speed=0.21)
  ]


  # create the floor exit wall
  objects.append(createFloor(True))

  # messages
  objects += [
    createMessage(text="STAGE # START",
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
                  lineWidth=1.2,
                  color=[0, 255, 0, 127]),
    createRingSet(spin=createValuePulser(0, 0.4, 5, [0,0,0]),
                  spread=createValuePulser(0, 0.5, 40, [0,0,0]),
                  spreadOffset=[60,60,60],
                  count=60,
                  radiusScale=2.3,
                  lineWidth=1.4,
                  color=[0, 255, 63, 127]),
    createRingSet(spin=createValuePulser(0, 0.2, 10, [0.2,4.2,0.2]),
                  spread=createValuePulser(0.01, 0.16, 15, [2.1,6,0.1]),
                  spreadOffset=[60,10,20],
                  count=100,
                  radiusScale=5,
                  lineWidth=.9,
                  color=[0, 127, 255, 95])
  ]


  return config

