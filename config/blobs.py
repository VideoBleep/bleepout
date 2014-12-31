from object_specs import *

def generate():
  config = {
    "name": "blobs",
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

  def addBrickBlob(centerElevation, centerHeading, objects=objects):
    colors = ValueRotator(
      [164, 2, 253],
      [0, 253, 170],
      [255, 139, 0])
    lives = ValueRotator(
      1,
      2,
      1
    )
    for h in range(-1, 1):
      heading = centerHeading + (h * 4.6)
      elevation = centerElevation + 8
      objects += [
        createBrick(elevation=elevation,
                    heading=heading,
                    size=[5, 5, 10],
                    color=colors.next(),
                    speed=0,
                    value=1,
                    lives=lives.next())
      ]
    for e in range(3, 6, 2):
      for h in range(-2, 2):
        heading = centerHeading + (h * 4.6)
        elevation = centerElevation + e
        objects += [
          createBrick(elevation=elevation,
                      heading=heading,
                      size=[5, 5, 10],
                      color=colors.next(),
                      speed=0,
                      value=1,
                      lives=lives.next())
        ]
    for h in range(-3, 3):
      heading = centerHeading + (h * 4.4)
      elevation = centerElevation
      objects += [
        createBrick(elevation=elevation,
                    heading=heading,
                    size=[5, 5, 10],
                    color=colors.next(),
                    speed=0,
                    value=1,
                    lives=lives.next())
      ]
    for h in range(-2, 2):
      heading = centerHeading + (h * 4.4)
      elevation = centerElevation - 3
      objects += [
        createBrick(elevation=elevation,
                    heading=heading,
                    size=[5, 5, 10],
                    color=colors.next(),
                    speed=0,
                    value=1,
                    lives=lives.next())
      ]
    for h in range(-1, 1):
      heading = centerHeading + (h * 4.6)
      elevation = centerElevation - 6
      objects += [
        createBrick(elevation=elevation,
                    heading=heading,
                    size=[5, 5, 10],
                    color=colors.next(),
                    speed=0,
                    value=1,
                    lives=lives.next())
      ]


  for e in range(6):
    addBrickBlob(30, e * 60)
  for e in range(6):
    addBrickBlob(50, 30 + e * 60)

  # objects += [
  #   createBrickRing(elevation=72,
  #                   color=[0, 0, 0],
  #                   count=12,
  #                   value=2,
  #                   lives=2,
  #                   speed=0.6,
  #                   size=[7, 5, 17])
  # ]

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
