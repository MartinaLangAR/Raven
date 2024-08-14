#include <iostream>
#include <fstream>
#include <string>


int main() {
	int a=5;
	std::ofstream myfile;
	myfile.open("test.txt", std::ios_base::app);
	myfile << std::to_string(a) << "writing to file, but longer and longer\n";
	myfile.close();
	
	return 0;
}
