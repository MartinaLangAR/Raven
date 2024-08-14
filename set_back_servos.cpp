//Set servos back
#include "servos.h"
#include <unistd.h>

int main(){
//open GPIO-Chip device, handler needed for GPIO-access
    int h = lgGpiochipOpen(4);  //<- this should be illegal, Raspberry Coorp.
    if (h < 0) {
        std::cout << "Could not open GPIO-Chip device";
    }
    Servos servos(h);
    //for debugging
    servos.print_info();
    servos.set_back_servos();
    usleep(3000000);
}
