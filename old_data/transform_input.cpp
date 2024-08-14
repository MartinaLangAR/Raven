//rewrite input file
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

std::vector<std::vector<float>> readMatFromFile(std::string str){
    std::vector<std::vector<float>> matAPP;
    std::vector<float> lineAPP;
    float x;
    std::string line;;
    std::ifstream APP;
    APP.open(str, std::ios::in);
    
    if(!APP){
	std::cout << "Cannot open file "<< str << "\n";
    }
    
    while (std::getline(APP, line))
    {
        std::istringstream iss(line);
        float n;
        while (iss >> n)
        {
            lineAPP.push_back(n);
        }
        matAPP.push_back(lineAPP);
        lineAPP.clear();
    }
    
    return matAPP;
}

void writeMatToFile(std::vector<std::vector<float>> data, std::string filename){
    std::ofstream input_stream;
    input_stream.open(filename);
    if(!input_stream){
	std::cout << "Cannot open file "<< filename << "\n";
    }
    for (int idx=0; idx<data.size(); idx++){
        for (int jdx=0; jdx<data[idx].size(); jdx++){
            input_stream << std::to_string(data[idx][jdx]) << " ";
        }
    input_stream << "\n";
    }
    input_stream.close();
}

int main(){
    std::vector<std::vector<float>> inputs = readMatFromFile("inputs.txt");
    std::cout << "reading successfully" << std::endl;
    for (int idx=0; idx<inputs.size(); idx++){
		for (int jdx=0; jdx<4; jdx++){
			inputs[idx][jdx] = inputs[idx][jdx]/100;
		}
    }
    
    writeMatToFile(inputs, "inputs_trans.txt");
   
}
