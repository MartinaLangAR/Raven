#pragma once
#include <functional>
#include <stdlib.h>
#include <lgpio.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h> //for usleep

//headerfile for servos.cpp

#if !defined(LFLAGS)
#define LFLAGS 0 //used for Pin init in main
#endif
#if !defined(FRONT)
#define FRONT 18 //Pinnumber for Back servo, BCM-mode
#endif
#if !defined(BACK)
#define BACK 15 // same for front
#endif


class Servos {
    
    private:
        float legal_frontangle_min = 5;
        float legal_frontangle_max = 185;
        float legal_backangle_min = 65;
        float legal_backangle_max = 185;
        
        int handler; //as returned by lgGPIOchipOpen
	
	float map_frontangle_to_per(float angle);
        float map_backangle_to_per(float angle);
	
        
    public:
    		
        //worst case: first position might be wrong as we do not wait long enough
        // if the servos were not left in middle position. You may want to change that
        // with a init-procedure, future-me :)
        float curr_frontangle = 135;
        float curr_backangle = 135;
      


        Servos(int handler_from_lgGPIOchipOpen);
	
	bool legal_frontangle(float angle);
	bool legal_backangle(float angle);
	//limit for moving degrees is set in class, dirty on the go
	//you may wanna change that, future-me :)
	// Actually I won't. It seems to be a good decision.
	std::pair<bool,bool>  move(float front_angle, float back_angle);
	std::pair<bool,bool>  move_to_percentage(float front, float back);
	std::pair<bool,bool>  move_relative(float front_angle, float back_angle);
	std::pair<bool,bool> set_back_servos();
	
	std::pair<float, float> echo_pos();

	//debugging stuff
	void print_info();
	void nod();
	void demo_run();
};
