
version 1 of Raven

Specs:
 1 camera on Ravens left side
 2 Servos (stacked)
  on Raspi 4B, Bullseys
  
  
 Servos:
  back_servo (rolling the head)
  front servo (tilting the head)
  
  back_servo: range 0 - 270 degree, facing the raven zero deg would be
              on the right side, 135 degree straight upwards and 270 degree on the left

version 2 of Raven

Specs:
 1 camera on Ravens left side
 2 Servos (stacked)
  on Raspi 5, Bookworm
  
  
 Servos:
  back_servo (rolling the head)
  front servo (tilting the head)
  
  back_servo: range 0 - 270 degree, facing the raven zero deg would be
              on the right side, 135 degree straight upwards and 270 degree on the left
              
Building project: using CMAKE: - browse in Raven\version1
                               - cmake .
                               - make
                               - ./Raven (-refers to main.cpp compiled output)

Art Project for motor-controlled raven sculpture

