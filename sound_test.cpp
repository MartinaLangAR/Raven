//waiting-test

#include <unistd.h>
#include <iostream>

int main(){
std::cout << "started!" << std::endl;
system("paplay sounds/success-fanfare.mp3");
usleep(20000000);
std::cout << "ended!" << std::endl;
}
