#include "servos.h"

//referring to front and back servo
//front servo range: 0 - 270 degree
//back  servo range: 0 - 270 degree
//leads to differences in calculation for hightime



Servos::Servos(int handler_from_lgGPIOchipOpen){
    handler = handler_from_lgGPIOchipOpen;
}

bool Servos::legal_frontangle(float angle){
    if (angle > legal_frontangle_max | angle < legal_frontangle_min) {
        //std::cout << "angle is out of range at front: "<< angle << " current " << curr_frontangle <<std::endl;
        return false;
    }    
    return true;
}

bool Servos::legal_backangle(float angle){
    if (angle > legal_backangle_max | angle < legal_backangle_min) {
        //std::cout << "angle is out of range at back: "<< angle << std::endl;
        return false;
    }    
    return true;
}

std::pair<bool,bool> Servos::set_back_servos(){
    //sets back to middle position
    return move_to_percentage(50,50);    
}

//move function should control 2 servos at once
std::pair<bool,bool> Servos::move(float front_angle, float back_angle){
    //check if input angles are valid
    if (!legal_frontangle(front_angle) | !legal_backangle(back_angle)) {
        return std::pair<bool,bool> {legal_frontangle(front_angle),legal_backangle(back_angle)};
    }
    
    float hightime_front = (7.4 * front_angle + 500); //measured values (extern)
    float hightime_back = (7.4 * back_angle +  500);  //measured value (intern)
    //calc pulses needed for servo to get in place
    int pulses_front = abs((front_angle - curr_frontangle)*33/270 + 10);
    int pulses_back = abs((back_angle - curr_backangle)*33/270 + 10);
    //but wait for both to finish, no hurry here
    int pulses = std::max(pulses_front, pulses_back);    
    for (int i=0; i < pulses; i++){
         //set FRONT high
        lgGpioWrite(handler, FRONT, 1);
        //wait for lenght of signal
        usleep(hightime_front);
        
        //set FRONT low
        lgGpioWrite(handler, FRONT, 0);
        //finish cycle lenght
        usleep(25000 - hightime_front); // deleted zero
        
        //set BACK high
        lgGpioWrite(handler, BACK, 1);
        usleep(hightime_back);
        
        //set BACK low
        lgGpioWrite(handler, BACK, 0);
        usleep(25000 - hightime_back);
        
    }
    //update current angles
    curr_frontangle = front_angle;
    curr_backangle = back_angle;
    return std::pair<bool,bool>{true, true};
}

std::pair<bool,bool> Servos::move_to_percentage(float front, float back){  
    float min = 0.01;
    float deg_front = legal_frontangle_min + std::max((legal_frontangle_max - legal_frontangle_min) * front /100, min);
    float deg_back = legal_backangle_min + std::max((legal_backangle_max - legal_backangle_min)* back /100, min);
    //rounding ?
    return move(deg_front, deg_back);
};


std::pair<bool,bool> Servos::move_relative(float front_angle, float back_angle){
    float abs_front_angle = curr_frontangle + front_angle;
    float abs_back_angle = curr_backangle + back_angle;
    return move(abs_front_angle, abs_back_angle);

}


void Servos::print_info(){    
    lgChipInfo_t cInfo;

    int status = lgGpioGetChipInfo(handler, &cInfo);

    if (status == LG_OKAY)
    {
       printf("lines=%d name=%s label=%s\n",
          cInfo.lines, cInfo.name, cInfo.label);
    }
    
    lgGpioClaimOutput(handler, LFLAGS, FRONT, 0);
    lgGpioClaimOutput(handler, LFLAGS, BACK, 0);
    
    lgLineInfo_t lInfo;

    status = lgGpioGetLineInfo(handler, FRONT, &lInfo);

    if (status == LG_OKAY)
    {
       printf("lFlags=%d name=%s user=%s\n",
          lInfo.lFlags, lInfo.name, lInfo.user);
    }
    status = lgGpioGetLineInfo(handler, BACK, &lInfo);

    if (status == LG_OKAY)
    {
       printf("lFlags=%d name=%s user=%s\n",
          lInfo.lFlags, lInfo.name, lInfo.user);
    }
}  
float Servos::map_frontangle_to_per(float angle){
    float min = 0.01;
    return (std::max(angle - legal_frontangle_min, min) * 100 /(legal_frontangle_max - legal_frontangle_min));    
};

float Servos::map_backangle_to_per(float angle){
    float min = 0.01;
    return (std::max(angle - legal_backangle_min, min) * 100 / (legal_backangle_max - legal_backangle_min));    
};

std::pair<float, float> Servos::echo_pos(){
    //std::cout << "Frontangle: " <<map_frontangle_to_per(curr_frontangle)<< " percentage Backangle: " << map_backangle_to_per(curr_backangle)<< " percentage" << std::endl;
    return std::pair<float, float>(map_frontangle_to_per(curr_frontangle), map_backangle_to_per(curr_backangle));    
}

void Servos::nod(){
    set_back_servos();
    usleep(200000);
    move(110.0, 180.0);
    usleep(200000);
    set_back_servos();
    }


void Servos::demo_run(){
    set_back_servos();
    usleep(200000);
    move(110.0, 180.0);
    usleep(200000);
    move(120.0, 180.0);
    usleep(200000);
    move(90.0, 180.0);
    usleep(200000);
    move(70.0, 180.0);
    usleep(200000);
    move(110.0, 150.0);
    usleep(200000);
    move(70.0, 150.0);
    usleep(200000);
    move(110.0, 180.0);
    usleep(200000);
    move(90.0, 100.0);
    usleep(200000);
    move(110.0, 150.0);
    usleep(200000);
    move(70.0, 150.0);
    usleep(200000);
    set_back_servos();
    usleep(200000);
}
