from object_specs import *

def generate():
  config = RoundConfig('Round1')
  config.paddleSize = (16, 8, 40)
  config.ballRadius = 8
  config.modifierRadius = 9

  # balls.... random?

  widePaddle = ModifierSpec(ModifierType.PaddleWidth, 'widePaddle',
                            amount= 1.5,
                            duration= 5.0,
                            color= (0, 0, 0, 255))

  narrowPaddle = ModifierSpec(ModifierType.PaddleWidth, 'narrowPaddle',
                              amount= 0.5,
                              duration= 5.0,
                              color= (255, 0, 0, 255))

  rows = 12
  cols = 10
  for row in range(rows):
    for col in range(cols):
      # float s = col / (cols * 1.0);
      # auto& spec = config->addBrick()
      #   .setElevation(30 + 3 * row)
      #   .setHeading(s * 360 + row * 2 + ((col % 2) ? 5 : -5))
      #   .setColor(ofColor(s * 255,
      #                     row / (rows * 1.0) * 255,
      #                     (1 - s) * 255))
      #   .setLives((row % 3 == 1) ? 2 : 1)
      #   .setValue(1)
      #   .setSpeed(0)
      #   .setSize(ofVec3f(7.0f, 5.0f, 17.0f));
      # if (col % 3 == 0 && row % 3 == 0)
      #   spec.modifierName = widePaddleName;
      # else if (col % 7 == 0 && row % 5 == 0)
      #   spec.modifierName = narrowPaddleName;
      s = col / (cols * 1.0)
      brick = BrickSpec(elevation= 30 + 3 * row,
                        heading= s * 360 + row * 2 + (5 if (col % 2) else -5),
                        color= (s * 255,
                                row / float(rows) * 255,
                                (1.0 - s) * 255),
                        lives= 2 if (row % 3 == 1) else 1,
                        value= 1,
                        speed= 0,
                        size= (7, 5, 17))
      
      pass

  pass
