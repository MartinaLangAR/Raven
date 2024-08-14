//convert input txt-files to .csv
#include "utils.hpp"


int main(){
	std::vector<std::vector<float>> inputData = readMatFromFile("inputs.txt");
	writeMatToFile(inputData, "input_Data.csv", ", ");
	std::vector<std::vector<float>> labelData = readMatFromFile("labels.txt");
	writeMatToFile(labelData, "label_Data.csv", ", ");    

}
