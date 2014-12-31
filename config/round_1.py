from object_specs import *
import random

def generate():
  config = {
    "name": "Round1",
    "paddleSize": (16, 8, 40),
    "ballRadius": 8,
    "modifierSize": (9, 16, 9),
    "rules": {
      "ballsRespawn": True
    },
    "objects": []
  }
  objects = config["objects"]

  for i in range(5):
    objects.append(createBall(30, random.uniform(0, 360)))

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

  objects += [widePaddle, narrowPaddle]

  rows = 12
  cols = 10
  for row in range(rows):
    for col in range(cols):
      s = col / (cols * 1.0)
      objects.append(createCurvedBrickCol(elevation1= 30,
                                          heading1= s * 360 + (5 if (col % 2) else -5),
                                          elevation2= 60,
                                          heading2= s * 360 + 20 + (5 if (col % 2) else -5),
                                          count= rows,
                                          color1= [s * 255, 0, 255],
                                          color2= [s * 255, 255, 0],
                                          size= [7, 5, 17],
                                          stripe1= {},
                                          stripe2= {
                                            "lives": 2,
                                            "modifier": { "name": widePaddle["name"], "chance": .3}
                                          },
                                          stripe3= {
                                            "modifier": { "name": narrowPaddle["name"], "chance": .2}
                                          }))

  for i in range(6):
    objects.append(createCurvedWall(elevation1=30,
                                    heading1=i * 60 + 15,
                                    elevation2=68 if i % 2 else 62,
                                    heading2=i * 60 + 45,
                                    width=10))

  # rotating top walls
  objects += [
    createWall(elevation=67,
               heading=5,
               size=[10, 10, 30],
               speed=0.02,
               stopHeading=80),
    createWall(elevation=67,
               heading=125,
               size=[10, 10, 30],
               speed=0.02,
               stopHeading=200),
    createWall(elevation=67,
               heading=245,
               size=[10, 10, 30],
               speed=0.02,
               stopHeading=320)
  ]

  # rotating top brick rings
  objects += [
    createBrickRing(elevation=72,
                    color=[0, 0, 0],
                    count=12,
                    value=2,
                    lives=2,
                    speed=0.02,
                    size=[7, 5, 17]),
    createBrickRing(elevation=76,
                    color=[0, 0, 0],
                    count=10,
                    speed=-0.02,
                    size=[7, 5, 17]),
    createBrickRing(elevation=80,
                    color=[0, 0, 0],
                    count=8,
                    value=2,
                    speed=0.02,
                    size=[7, 5, 17])
  ]

  # create the floor exit wall
  objects.append(createFloor(True))

  # messages
  objects += [
    createMessage(text="VideoBleep\n presents",
                  size=12,
                  duration=3,
                  color=[255, 255, 255]),
    createMessage(text="BLEEPOUT",
                  size=50,
                  color=[0, 120, 240],
                  trails=4,
                  delay=3,
                  duration=4.5),
    createMessage(text="STAGE 1 START",
                  color=[0, 255, 0],
                  size=25,
                  delay=7.5,
                  duration=2.5)
  ]
  config["startDelay"] = 10

  # background ring sets
  objects += [
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0,0,0]),
                  spread=createValuePulser(0, 0.1, 10, [20, 20, 20]),
                  spreadOffset=[20, 20, 20],
                  count=30,
                  radiusScale=1.95,
                  lineWidth=1.2,
                  color=[0, 0, 255, 63]),
    createRingSet(spin=createValuePulser(0, 0.4, 5, [0, 0, 0]),
                  spread=createValuePulser(0, 0.5, 40, [0, 0, 0]),
                  spreadOffset=[60, 60, 60],
                  radiusScale=2.3,
                  lineWidth=1.4,
                  color=[0, 255, 255, 63]),
    createRingSet(spin=createValuePulser(0, 0.2, 5, [0, 0, 0]),
                  spread=createValuePulser(0.01, 0.16, 10, [0, 0, 0]),
                  spreadOffset=[60, 60, 60],
                  count=50,
                  radiusScale=2,
                  lineWidth=0.9,
                  color=[127, 172, 255, 63])
  ]


  return config

