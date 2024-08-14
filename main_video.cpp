//Ravi main script
/* compile & build only via cmake 
 * - browse to Raven\version1
 * - cmake .
 * - make
 * - ./Raven (-refers to main.cpp compiled output)
 *  
 * changes (adding Libs etc) via CMakeLists.txt
 */

#define MLPACK_ENABLE_ANN_SERIALIZATION

#include "servos.h"
#include <unistd.h> //for usleep
#include <opencv2/opencv.hpp>
#include "facedetector.h"
#include "utils.hpp"
#include <mlpack.hpp>

int input_size = 7; //<---- this must match the trained network that is to be loaded!!!
int hidden_neurons_per_layer = 5; //<---- this must match the trained network that is to be loaded!!!
int output_size = 2; //<---- this must match the trained network that is to be loaded!!!
int precision_threshold = 10; //<--offset for pixel to still be considered ok
int free_movement_bound = 5; //<-- if counter reaches this value raven will move freely to look for humans


float clipping(float value, float lower = 1.0, float upper = 100.0){
    if ( lower > value ) return lower;
    if ( upper < value ) return upper;
    //std::cout << "value " << value  << " is in bounds" << std::endl;
    return value;
    }

int main(){
    //open GPIO-Chip device, handler needed for GPIO-access
    int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
    if (h < 0) {
        std::cout << "Could not open GPIO-Chip device";
    }
    std::cout << "started script" << std::endl;
    //setup servos
    Servos servos(h);
    //for debugging
    servos.print_info();
    servos.move_to_percentage(50,50);
    
    //Setup network for human body detection
    FaceDetector FD;

    //Setup servo models
    mlpack::FFN<> model0;
    mlpack::data::Load("models/model_cam2_0_short_1.xml", "model", model0);
    mlpack::FFN<> model1;
    mlpack::data::Load("models/model_0_02.xml", "model", model1);
    std::cout << "Loaded model .." << std::endl;
    
    
    float longest_dist_possible_sqrt = FD.img_height * FD.img_height 
                                  + FD.img_width * FD.img_width;
    
    //these are start values, we might/will change them later
    int active_camera = 0;
    
    lccv::PiCamera cam0;
    cam0.options->photo_width=2028;
    cam0.options->photo_height=1520;
    cam0.options->verbose=true;
    
    //counter to ensure freeing movement from time to time
    int free_counter = 0;
    std::vector<std::pair<int, int>> positions{std::pair<int, int>(50, 50), std::pair<int, int>(60, 60),
                                               std::pair<int, int>(70, 50), std::pair<int, int>(70, 60),
                                               std::pair<int, int>(80, 60), std::pair<int, int>(80, 60),
                                               std::pair<int, int>(70, 50), std::pair<int, int>(60, 60),
                                               std::pair<int, int>(40, 50), std::pair<int, int>(40, 40),
                                               std::pair<int, int>(30, 30), std::pair<int, int>(40, 20),
                                               std::pair<int, int>(50, 30), std::pair<int, int>(50, 40),
                                               std::pair<int, int>(50, 50), std::pair<int, int>(60, 60),
                                               std::pair<int, int>(70, 50), std::pair<int, int>(70, 60),
                                               std::pair<int, int>(80, 60), std::pair<int, int>(80, 60),
                                               std::pair<int, int>(70, 50), std::pair<int, int>(60, 60),
                                               std::pair<int, int>(40, 50), std::pair<int, int>(40, 40),
                                               std::pair<int, int>(30, 30), std::pair<int, int>(40, 20),
                                               std::pair<int, int>(50, 30), std::pair<int, int>(50, 40),
                                               std::pair<int, int>(50, 50), std::pair<int, int>(60, 60)};

    //get nearest match for tracking (using squared values to save time and space)
    Match aim;
    std::vector<Match> faces; 
    arma::mat predictionTmp;
    //!!!!!!!!!!!!!!!!!!!!!!!!!workflow begins here!!!!!!!!!!!!!!!!!!/////////////////////////////////
 
    
    while (true){
        //capture image 
        //std::cout << "restarting loop!" << std::endl;
        
        FD.capture_image(active_camera);
        //detect the faces, second arg is about visual output or not
        faces = FD.detect_faces(active_camera, true);
        //usleep(1);
    
        if (faces.size() > 0){
                free_counter = 0;
                // get servo position
                std::pair<float,float> servo_pos = servos.echo_pos();
                //init aim (=match neartest to last match in image) 
                aim = closest_to_center(&faces, &FD);
                //calc dist between aims and center to see if we want to adjust (or rewrite get_aim)
                bool adjust = (std::abs(aim.center_x - FD.img_width/2) 
                                / std::abs(aim.center_y - FD.img_height/2) 
                                < precision_threshold * precision_threshold ) ? true : false;
                                
                if (adjust){
                    //std::cout << "adjust is true!" << aim.center_x << ", " << aim.center_y << std::endl;
                    //std::cout << "camera " << active_camera << "has a match" << std::endl; 
                    if (active_camera = 1) {
                        
                        arma::mat current_input{(float) aim.center_x, (float) aim.center_y, 
                                                (float) aim.height, (float) aim.width, 
                                                servo_pos.first/100, servo_pos.second/100, (float) active_camera};
                        current_input.reshape(7,1);

                        model1.Predict(current_input, predictionTmp);
                        float predicted_front = clipping(predictionTmp(0,0), 0.01 , 1) * 100;
                        float predicted_back = clipping(predictionTmp(1,0), 0.01, 0.5) * 100;
                        //std::cout <<"prediction : " << predictionTmp << std::endl;
                                                                                  
                        servos.move_to_percentage(predicted_front, predicted_back);
                        
                        //std::cout << "able to move" << std::endl;
                        }
                    else {
                        arma::mat current_input{(float) aim.center_x, (float) aim.center_y, 
                                                (float) aim.height, (float) aim.width, 
                                                servo_pos.first/100, servo_pos.second/100}; // (float) active_camera};
                        current_input.reshape(6,1);

                        model0.Predict(current_input, predictionTmp);
                        float predicted_front = clipping(predictionTmp(0,0), 0.01 , 1) * 100;
                        float predicted_back = clipping(predictionTmp(1,0), 0.01, 0.5) * 100;
                        //std::cout <<"prediction : " << predicted_front << " , " << predicted_back << std::endl;
                                                                                    
                        servos.move_to_percentage(predicted_front, predicted_back);
                        //std::cout << "able to move" << std::endl;
                    }
                    
                }
        }
                
        else {
            active_camera = (active_camera + 1) % 2;  //<---------------------------------------------comment out for debugging purpose, include again please!
            std::cout << active_camera << std::endl;
            free_counter++;
            if (free_counter >= free_movement_bound){
                std::pair<float, float> position = positions[free_counter%positions.size()];
                servos.move_to_percentage(position.first, position.second);
                usleep(2);
            }
        }
                
        
                
    }
   return 0;
}
 
