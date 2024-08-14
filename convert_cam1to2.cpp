//convert data from camera 1 to camera 2
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

std::vector<std::vector<float>> readMatFromFile(std::string str){
    std::vector<std::vector<float>> matAPP;
    std::vector<float> lineAPP;
    float x;
    std::string line;;
    std::ifstream APP;
    APP.open(str, std::ios::in);
    
    while (std::getline(APP, line))
    {
        std::stringstream ss(line);
        std::string n;
        while (ss.good())
        {
            std::getline(ss, n, ',');
            float f_val = std::stof(n);
            lineAPP.push_back(f_val);
        }
        matAPP.push_back(lineAPP);
        lineAPP.clear();
    }
    
    return matAPP;
}

void writeMatToFile(std::vector<std::vector<float>> data, std::string filename, std::string separator= " "){
    std::ofstream input_stream;
    input_stream.open(filename);
    for (int idx=0; idx<data.size(); idx++){
        for (int jdx=0; jdx<data[idx].size(); jdx++){
            input_stream << std::to_string(data[idx][jdx]);
            if (jdx != data[idx].size()){
                input_stream << separator;
                }
        }
    input_stream << "\n";
    }
    input_stream.close();
}

int main() {

	std::vector<std::vector<float>> input_data = readMatFromFile("data/inputs.csv");
	std::cout << "read input data" << std::endl;
    std::cout << input_data[0][4] << std::endl;
	/*for (int i = 0; i < input_data.size(); i++){

		input_data[i][4] =  1.0 - input_data[i][4];
		input_data[i][5] = 1.0 - input_data[i][5];
		input_data[i][6] = 0;
	}
    std::cout << input_data[0][4] << std::endl;
    std::cout << "changed input data" << std::endl;
    */
    writeMatToFile(input_data, "data/input_cam2.csv", ", ");
    std::cout << "input data written to file" << std::endl;
    
    /*std::vector<std::vector<float>> label_data = readMatFromFile("data/labels.csv");
    std::cout << "read label data" << std::endl;
	for (int i = 0; i < label_data.size(); i++){
		label_data[i][0] = 1.0 - label_data[i][0];
		label_data[i][1] = 1.0 - label_data[i][1];
	}
    
    std::cout << "changed label data" << std::endl;
    writeMatToFile(label_data, "data/label_cam2.csv", ", ");
    std::cout << "label data written to file" << std::endl;
*/

return 0;
}
