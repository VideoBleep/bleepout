from object_specs import *

def generate():
  config = {
    "name": "Round5",
    "ballRadius": 8,
    "modifierSize": (9, 16, 9),
    "objects": []
  }
  objects = config["objects"]

  objects += [createBall(10, i * (360/5)) for i in range(5)]

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

  objects += [
    createBrickRing(elevation=68,
                    color=[0, 0, 255],
                    count=24,
                    value=2,
                    lives=2,
                    size=[7, 5, 10],
                    modifier={"name": superCharge["name"], "chance": 0.2}),
    createBrickRing(elevation=64,
                    color=[0, 63, 255],
                    count=48,
                    value=2,
                    lives=2,
                    size=[7, 5, 8],
                    modifier={"name": narrowPaddle["name"], "chance": 0.4}),
    createBrickRing(elevation=56,
                    color=[0, 127, 255],
                    count=36,
                    value=1,
                    lives=1,
                    size=[7, 5, 12],
                    modifier={"name": widePaddle["name"], "chance": 0.1}),
    createBrickRing(elevation=48,
                    color=[0, 192, 127],
                    count=48,
                    value=2,
                    lives=2,
                    size=[7, 5, 14],
                    speed=0.02),
    createBrickRing(elevation=44,
                    color=[0, 127, 192],
                    count=56,
                    value=1,
                    lives=1,
                    size=[7, 5, 12]),
    createBrickRing(elevation=40,
                    color=[0, 127, 192],
                    count=56,
                    value=1,
                    lives=1,
                    size=[7, 5, 12]),
    createBrickRing(elevation=36,
                    color=[0, 127, 192],
                    count=56,
                    value=1,
                    lives=1,
                    size=[7, 5, 12])
  ]

  objects += [
    createWallRing(#visible=False,
                   elevation=30,
                   count=8,
                   speed=0.07,
                   size=[7, 5, 32])
  ]

  objects += [
    createBrickRing(elevation=28,
                    color=[63, 192, 192],
                    count=3,
                    value=1,
                    lives=1,
                    size=[7, 5, 14],
                    modifier={"name": superCharge["name"], "chance": 0.7})
  ]

  # create the floor exit wall
  objects.append(createFloor(True))

  # background ring sets
  objects += [
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0,10,10]),
                  spread=createValuePulser(0, 0.1, 50, [15, 30, 20]),
                  spreadOffset=[60, 30, 40],
                  count=10,
                  radiusScale=3.95,
                  lineWidth=4.2,
                  color=[0, 63, 255, 153]),
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0, 0, 0]),
                  spread=createValuePulser(0, 0.55, 40, [3, 0, 0]),
                  spreadOffset=[0, 10, 80],
                  radiusScale=1.4,
                  lineWidth=2.4,
                  count=20,
                  color=[0, 127, 127, 127]),
    # createRingSet(spin=createValuePulser(0, 0.2, 5, [0, 30, 30]),
    #               spread=createValuePulser(0.01, 0.16, 10, [0, 50, 0]),
    #               spreadOffset=[60, 60, 90],
    #               count=150,
    #               radiusScale=4,
    #               lineWidth=.9,
    #               color=[0, 255, 63, 176])
  ]
  # messages
  objects += [
    createMessage(text="STAGE # START",
                  color=[0, 27, 255],
                  size=25,
                  delay=1,
                  duration=3)
  ]
  config["startDelay"] = 5

  return config
