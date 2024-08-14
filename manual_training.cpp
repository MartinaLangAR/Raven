//manual training script!

#include <unistd.h> //for usleep
#include <iostream>
#include "servos.h"
#include "facedetector.h"

/*cmd-application for testing servos
 * uses W-A-S-D to relativly move both servos
 * adding Q-E-Y-X for combined movement
 *
 * compile:
 *     g++ -Wall -c "%f" -l lgpio  
 * build:
 *     g++ -Wall -o "%e" "%f"  -l lgpio servos.cpp
 */
 
 std::pair<Match, std::pair<float, float>> new_sample(FaceDetector* FD, int active_camera, Servos* servos ){
   //capture image 
  FD->capture_image(active_camera);
  //detect the faces, second arg is about visual output or not
  std::vector<Match> faces = FD->detect_faces(active_camera, true);
  if (faces.size() == 0){system("paplay sounds/sorry.mp3");}
  return std::pair<Match, std::pair<float, float>> {faces[0], servos->echo_pos()};
  }
 

int main(){
  int active_camera = 1;
  
  //Setup network for human body detection
  FaceDetector FD;
  Match aim;
  //------------------our dataset
  std::vector<Match> aims;  //contains center_x, center_y, height, width
  std::vector<std::pair<float,float>> servo_pos; //contains front, back tuple for servo position when first image was taken
  std::vector<int> cam_idx; //contains camera idx per round <-----this may be redundant now, but when we take the second camera into account this will help us
  std::vector<std::pair<float, float>> labels; //contains front, back tuple for servo position when the match is finally focused
  //------------------end of dataset
      
  //for file output
  std::ofstream input_stream;
  std::ofstream label_stream;
  
  input_stream.open("input_cam2_short.csv", std::ios_base::app);
  label_stream.open("label_cam2_short.csv", std::ios_base::app);
  
  //open GPIO-Chip device, handler needed for GPIO-access
  int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
  if (h < 0) {
      std::cout << "Could not open GPIO-Chip device";
  }

  Servos servos(h);
  servos.set_back_servos();
  
  std::pair<float,float> pos = servos.echo_pos();
  std::pair<float, float> input_pos;
  
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
        servos.move_to_percentage(pos.first, pos.second + 2.0);
      }
      else if (input == 'a'){
        servos.move_to_percentage(pos.first - 2.0 , pos.second);
      }
      else if (input == 's'){
        servos.move_to_percentage(pos.first, pos.second - 2.0 );
      }
      else if (input == 'd'){
        servos.move_to_percentage(pos.first + 2.0 , pos.second);
      }
      
      //else if (input == 'c'){
      //  active_camera = (active_camera + 1) % 2;
      //}

      else if (input == 'i'){
        std::cout << "new sample! \n";
        FD.capture_image(active_camera);
        usleep(10);
        //detect the faces, second arg is about visual output or not
        std::vector<Match> faces = FD.detect_faces(active_camera, true);
        if (faces.size() == 0){system("paplay sounds/sorry.mp3");}
        else {
          aim = faces[0];
          input_pos = servos.echo_pos();
        }
      }
      
      else if (input == 'k'){
        std::cout << "commiting!" << std::endl;
        aims.push_back(aim);
        servo_pos.push_back(input_pos);
        cam_idx.push_back(active_camera);
        
        labels.push_back(servos.echo_pos());
        }
      else if (input == 'x'){
        //set console back
        servos.set_back_servos();
        system("stty cooked"); 
        //sanity check!	    
        if (aims.size() != labels.size()){std::cout << "Number of inputs do not align!" << std::endl;}
        else {std::cout << "sanity check passed!! " << aims.size() << std::endl;}
        
        //write to file
        for (int i=0; i<aims.size(); i++) {
        input_stream << std::to_string(aims[i].center_x) << ", " << std::to_string(aims[i].center_y) << ", "
                     << std::to_string(aims[i].height) << ", " << std::to_string(aims[i].width) << ", " 
                     << std::to_string(servo_pos[i].first/100) << ", " << std::to_string(servo_pos[i].second/100) << " \n" ;
                     //<< std::to_string(cam_idx[i]) << " \n";
               
        label_stream << std::to_string(labels[i].first/100) << ", "<< std::to_string(labels[i].second/100) << " \n";
        };
        
        input_stream.close();
        label_stream.close();
        exit(0);
      
    }
    pos = servos.echo_pos();
    
  }
    
  system("stty cooked"); 
  return 0;
}
