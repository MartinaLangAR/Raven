//move_to_percentage via input

#include "servos.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    //open GPIO-Chip device, handler needed for GPIO-access
    int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
    if (h < 0) {
        std::cout << "Could not open GPIO-Chip device";
    }
    system("figlet RAVEN");
    std::cout << "-------------------Interactivly control your Raven!---------------------------------------" << std::endl;
    std::cout << "After the setup you will soon enter the input loop." << std::endl;
    std::cout << "Press Ctrl+C if you want to exit the loop." << std::endl;
    std::cout << "Type in the following format: <percentage for front>; <percentage for back>" << std::endl;
    
    //setup servos
    Servos servos(h);
    //for debugging
    servos.print_info();
    servos.set_back_servos();
    
    
    std::string line;
    std::string front;
    std::string back;  
      
      
    while (true){
		std::getline(std::cin, line);
		
		std::stringstream ss(line);
		while (ss.good()){
		    std::getline(ss, front, ';');
		    std::getline(ss, back, ';');
		}
		
		std::cout << front << " " << back << std::endl;
		servos.move_to_percentage(stoi(front), stoi(back));
		servos.echo_pos();
		
		
    }
    
}
