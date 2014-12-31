from object_specs import *

def generate():
  config = {
    "name": "islands",
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
    createBrickQuads(color1=[255, 0, 63],
                     color2=[63, 0, 255],
                     size=[10, 8, 14],
                     elevation=56,
                     elevationSpacing=2.5,
                     headingSpacing=5,
                     count=4,
                     modifier={"name": superCharge["name"], "chance": 0.1}),
    createBrickQuads(color1=[127, 0, 95],
                     color2=[95, 0, 127],
                     size=[10, 8, 14],
                     elevation=46,
                     elevationSpacing=2.5,
                     headingSpacing=5,
                     count=4,
                     phase=30,
                     modifier={"name": widePaddle["name"], "chance": 0.2}),
    createBrickQuads(color1=[27, 0, 195],
                     color2=[195, 0, 27],
                     size=[10, 8, 14],
                     elevation=40,
                     elevationSpacing=2.5,
                     headingSpacing=5,
                     count=4,
                     phase=60),
    createBrickQuads(color1=[255, 0, 195],
                     color2=[195, 0, 255],
                     size=[6, 6, 8],
                     elevation=35,
                     elevationSpacing=1.5,
                     headingSpacing=2,
                     count=16,
                     phase=90,
                     modifier={"name": narrowPaddle["name"], "chance": 0.2}),
    createBrickQuads(color1=[255, 0, 195],
                     color2=[195, 0, 255],
                     size=[6, 4, 6],
                     elevation=30,
                     elevationSpacing=1.3,
                     headingSpacing=2,
                     count=24,
                     phase=120,
                     speed=0.01,
                     modifier={"name": widePaddle["name"], "chance": 0.1})
  ]

  # create the floor exit wall
  objects.append(createFloor(True))

  # background ring sets
  objects += [
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0,10,10]),
                  spread=createValuePulser(0, 0.4, 10, [15, 30, 20]),
                  spreadOffset=[60, 70, 40],
                  count=80,
                  radiusScale=4.95,
                  lineWidth=3.2,
                  color=[127, 0, 255, 127]),
    createRingSet(spin=createValuePulser(0, 0.3, 5, [0, 0, 0]),
                  spread=createValuePulser(0, 0.55, 40, [3, 0, 0]),
                  spreadOffset=[0, 10, 80],
                  radiusScale=1.4,
                  lineWidth=2.4,
                  count=20,
                  color=[255, 0, 127, 127])
  ]
  # messages
  objects += [
    createMessage(text="STAGE # START",
                  color=[255, 0, 127],
                  size=25,
                  delay=1,
                  duration=3)
  ]
  config["startDelay"] = 5

  return config
