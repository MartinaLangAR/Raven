#include <iostream>
#include "servos.h"

/*cmd-application for testing servos
 * uses W-A-S-D to relativly move both servos
 * adding Q-E-Y-X for combined movement
 * 
 * compile:
 *     g++ -Wall -c "%f" -l lgpio  
 * build:
 *     g++ -Wall -o "%e" "%f"  -l lgpio servos.cpp
 */
 
 void capture_image(unsigned int ending, unsigned int camera = 1){
    std::string cmd = "libcamera-still --camera " + std::to_string(camera) + " -o images/out" + std::to_string(ending)  + ".jpg --width 600 --height 500 -n -t 1";
    system(cmd.c_str()); 
 }

int main(){
  unsigned int ending_idx = 0;
  int camera = 1;
  
  //open GPIO-Chip device, handler needed for GPIO-access
  int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
  if (h < 0) {
      std::cout << "Could not open GPIO-Chip device";
  }

  Servos servos(h);
  servos.set_back_servos();
  
  std::pair<float,float> pos = servos.echo_pos();
  
  std::cout << "Press W-A-S-D buttons to move servos for 5 degree \n";
  std::cout << "A and D are front servo \n";
  std::cout << "W and S are back \n";
  std::cout << "i takes an image \n";
  std::cout << "Exit with c \n";
  char input;
  //set console to raw mode
  system("stty raw");
  while (1)
  {
      input = std::getchar();
      if (input == 'w'){
        servos.move_to_percentage(pos.first, pos.second + 5.0);
      }
      else if (input == 'a'){
        servos.move_to_percentage(pos.first - 5.0 , pos.second);
      }
      else if (input == 's'){
        servos.move_to_percentage(pos.first, pos.second - 5.0 );
      }
      else if (input == 'd'){
        servos.move_to_percentage(pos.first + 5.0 , pos.second);
      }
      else if (input == 'q'){
        servos.move_to_percentage(pos.first - 5.0 , pos.second + 5.0);
      }
      else if (input == 'e'){
        servos.move_to_percentage(pos.first + 5.0 , pos.second + 5.0);
      }
      else if (input == 'y'){
       servos.move_to_percentage(pos.first - 5.0 , pos.second - 5.0);
      }
      else if (input == 'x'){
       servos.move_to_percentage(pos.first + 5.0, pos.second - 5.0);
      }
      else if (input == 'i'){
        std::cout << "taking an image \n";
         capture_image(ending_idx, camera);
         ending_idx += 1;
      }
      else if (input == 'c'){
        //set console back
        servos.set_back_servos();
        system("stty cooked"); 
        exit(0);
      
    }
    pos = servos.echo_pos();
    
  }
  system("stty cooked"); 
  return 0;
}
