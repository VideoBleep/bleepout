from object_specs import *
import random

def generate():
  config = {
    "name": "Round2omg",
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

  superCharge = createModifier(name= "supercharge",
                              type= "SuperchargedBall",
                              duration= 20,
                              color= [255, 69, 0])

  objects += [superCharge]

  cols = 18
  rows = 14
  for col in range(cols):
    s = col / float(cols)
    objects.append(createCurvedBrickCol(elevation1= 35,
                                        heading1= s * 360 + (5 if (col % 2) else -5),
                                        elevation2= 75,
                                        heading2= s * 360 + 3 + (rows - 1),
                                        count= rows,
                                        color1= [0, 255, 255],
                                        color2= [0, 255, 0],
                                        size= [5, 2, 10],
                                        stripe1= {},
                                        stripe2= {
                                          "lives": 2,
                                          "modifier": { "name": superCharge["name"], "chance": .3}
                                        },
                                        stripe3= {}))

  # create bottom moving walls
  objects += [
    createWallRing(elevation=25,
                   count=5,
                   speed=0.6,
                   size=[10, 10, 30]),
    createWallRing(elevation=30,
                   count=5,
                   speed=1.5,
                   size=[10, 10, 30])
  ]

  # create the floor exit wall
  objects.append(createFloor(True))

  # messages
  objects += [
    createMessage(text="STAGE # START",
                  color=[0, 255, 255],
                  size=25,
                  delay=1,
                  duration=3)
  ]
  config["startDelay"] = 5

  # background ring sets
  objects += [
    createRingSet(spin=createValuePulser(0, 0.01, 10, [0,0,30]),
                  spread=createValuePulser(0.2, 0.5, 10, [0.3, 10, 0.3]),
                  spreadOffset=[50, 90, 0],
                  count=80,
                  radiusScale=2.3,
                  lineWidth=1.3,
                  color=[0, 0, 255, 127]),
    createRingSet(spin=createValuePulser(0, 0.3, 8, [0,20,0]),
                  spread=createValuePulser(0.2, 0.7, 8, [10, 0.3, 20]),
                  spreadOffset=[50, 90, 0],
                  count=80,
                  radiusScale=4.3,
                  lineWidth=1.3,
                  color=[0, 255, 127, 127])
  ]


  return config

