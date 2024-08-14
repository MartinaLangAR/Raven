#include <string>
#include "servos.h"
#include <sstream>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "facedetector.h"
#include "utils.hpp"

//threashold for pixel distance to be considered ok
float thr = 15;
//how many samples should we get?
int BATCH_SIZE = 20;
//how often to we adjust front and back servo to match our aim
int adjustment_iterations = 2;


int main() {
    //open GPIO-Chip device, handler needed for GPIO-access
    int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
    if (h < 0) {
        std::cout << "Could not open GPIO-Chip device";
    }
    system("figlet RAVEN");
    std::cout << "-------------------This is how to collect data to train your Raven!---------------------------------------" << std::endl;
    std::cout << "After the setup you will soon enter the collecting loop." << std::endl;
    std::cout << "Press Ctrl+C if you want to exit the loop and watch your Raven get smarter in the meantime." << std::endl;
    
    //setup servos
    Servos servos(h);
    //for debugging
    servos.print_info();
    servos.move_to_percentage(50,50);
    
    FaceDetector FD;
    
    //for file output
    std::ofstream input_stream;
    std::ofstream label_stream;
    
    input_stream.open("inputs.csv", std::ios_base::app);
    label_stream.open("labels.csv", std::ios_base::app);

    float longest_dist_possible_sqrt = FD.img_height * FD.img_height 
                                  + FD.img_width * FD.img_width;
    
    //these are start values, we might/will change them later
    int active_camera = 1;
    Match last_match = (Match) {0,0,0,0};
    
    std::pair<bool,bool> limits_ok;
    
    //------------------our dataset
    std::vector<Match> aims;  //contains center_x, center_y, height, width
    std::vector<std::pair<float,float>> servo_pos; //contains front, back tuple for servo position when first image was taken
    std::vector<int> cam_idx; //contains camera idx per round <-----this may be redundant now, but when we take the second camera into account this will help us
    std::vector<std::pair<float, float>> labels; //contains front, back tuple for servo position when the match is finally focused
    //------------------end of dataset
    
    std::vector<Match> faces;
    Match aim;
    Match moving_aim;

    float percentage_front; //will be 0 -> 100
    float percentage_back;  //as above
    
    
    //!!!!!!!!!!!!!!!!!!!!!!!!!workflow begins here!!!!!!!!!!!!!!!!!!/////////////////////////////////
    //play sound: Please stand still!
    system("paplay sounds/dont_move.mp3");
    
    
    for (int i=0; i< BATCH_SIZE; i++){
	    //set back limits
	    limits_ok = {true, true};
	    //capture image 
	    FD.capture_image(active_camera);
	    //detect the faces, second arg is about visual output or not
	    faces = FD.detect_faces(active_camera, true);
	    // if faces found
	    if (faces.size() > 0){
		    //init aim (=match neartest to last match in image) and label (=percentage of front an back servo to move)
		    aim = get_aim(&last_match, &faces, longest_dist_possible_sqrt);
		    //legal match test
		    if (aim.center_x != 0 && aim.center_y != 0){
			//save match for dataset
		       aims.push_back(aim);
		       
		       for (int i=0; i<adjustment_iterations; i++){
		       
			       std::pair<int, int> dist = get_threshold_val(thr, &aim, &FD);
			       percentage_front = servos.echo_pos().first;
			       percentage_back = servos.echo_pos().second;
			       
			       //fill in data
			       servo_pos.push_back(std::pair<float, float> (percentage_front/100, percentage_back/100));
			       cam_idx.push_back(active_camera);
			       
			       dist = get_threshold_val(thr, &aim, &FD);
			       
			       //Move back servo iterativly and take photos until label is revealed
			       while (dist.second != 0 && limits_ok.second){
				       //capture image 
				       FD.capture_image(active_camera);
				       //find matches again
				       faces = FD.detect_faces(active_camera, false);
				       //check if match could be found
				       if (faces.size() == 0) break;
				       //update aim if found
				       moving_aim = get_aim(&aim, &faces, longest_dist_possible_sqrt);
				       //check if we need to adjust again
				       dist = get_threshold_val(thr, &moving_aim, &FD);
				       //adjust servo at back for a little bit
				       percentage_back += dist.second * std::abs(moving_aim.center_y - 250) / 10; // <-------- hard coded with experimentally found values 
				       limits_ok = servos.move_to_percentage(percentage_front, percentage_back);

				       //if aim is out of range, limits_ok.second will be false and we just take it to the limit
				       if (!limits_ok.second){
					       percentage_back = (dist.second < 0) ? 0.0001 : 99.0; 
					       std::cout << "Moving to a limit on back servo!" << std::endl;
					       servos.move_to_percentage(percentage_front, percentage_back);
				       }
			       }
			       std::cout << "--------------------Height is set!---------------------------------" << std::endl;
			       
			       
			       while (dist.first != 0 && limits_ok.first){
				       //capture image 
				       FD.capture_image(active_camera);
				       //find matches again
				       faces = FD.detect_faces(active_camera, false);
				       //check if match could be found
				       if (faces.size() == 0) break;
				       //update aim if found
				       moving_aim = get_aim(&aim, &faces, longest_dist_possible_sqrt);
				       //check if we need to adjust again
				       dist = get_threshold_val(thr, &moving_aim, &FD);
				       //adjust servo at front for a little bit
				       percentage_front += dist.first * std::abs(moving_aim.center_x - 300) / 12; // <-------- hard coded with experimentally found values 
				       limits_ok = servos.move_to_percentage(percentage_front, percentage_back);
		
			       
				       //if aim is out of range, limits_ok.first will be false and we just take it to the limit
				       if (!limits_ok.first){
					       std::cout << "Percentage front before limit is reached: " << percentage_front << "\n ";
					       std::cout << "dist: " << dist.first << "\n ";
					       percentage_front = (dist.first < 0) ? 0.0001: 99.0; 
					       std::cout << "Moving to a limit on front servo!" << percentage_front << std::endl;
					       servos.move_to_percentage(percentage_front, percentage_back);
				       }
			       }
			       
			}
		       std::cout << "--------------------Width is set!---------------------------------" << std::endl;
		       
		       
		       //play sound: this is blocking, so everybodys got time to move
		       //system("paplay sounds/female_countdown5.mp3");
		       system("paplay sounds/count_shortbit.mp3");
		    
		       //save label
		       std::pair<float, float> label{servos.echo_pos().first/100 ,servos.echo_pos().second/100}; // as output for FFN is in range 0.0 -> 1.0
		       labels.push_back(label);
		    
		   }
	    }
	    else {
		std::cout << "No person found this time!" << std::endl;
		system("paplay sounds/sorry.mp3");
	    }
	    
		
	    }
     system("paplay sounds/success-fanfare.mp3");

    //sanity check!	    
    if (aims.size() != labels.size()){std::cout << "Number of inputs do not align!";}
    
    //write to file
    for (int i=0; i<aims.size(); i++) {
	input_stream << std::to_string(aims[i].center_x) << ", " << std::to_string(aims[i].center_y) << ", "
	             << std::to_string(aims[i].height) << ", " << std::to_string(aims[i].width) << ", " 
	             << std::to_string(servo_pos[i].first) << ", " << std::to_string(servo_pos[i].second) << ", " 
	             << std::to_string(cam_idx[i]) << " \n";
		     
	label_stream << std::to_string(labels[i].first) << ", "<< std::to_string(labels[i].second) << " \n";
    };
    
    input_stream.close();
    label_stream.close();
    
    return 1;
		
}
