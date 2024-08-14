#include <sstream>
#include <fstream>
#include <string>
#include "facedetector.h"
#include <iostream>
#include <cmath>

std::vector<std::vector<float>> readMatFromFile(std::string str){
    std::vector<std::vector<float>> matAPP;
    std::vector<float> lineAPP;
    float x;
    std::string line;;
    std::ifstream APP;
    APP.open(str, std::ios::in);
    
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

void writeMatToFile(std::vector<std::vector<float>> data, std::string filename, std::string separator= " "){
    std::ofstream input_stream;
    input_stream.open(filename);
    for (int idx=0; idx<data.size(); idx++){
        for (int jdx=0; jdx<data[idx].size(); jdx++){
            input_stream << std::to_string(data[idx][jdx]);
            if (jdx+1 != data[idx].size()){
                input_stream << separator;
                }
        }
    input_stream << "\n";
    }
    input_stream.close();
}

/*
 * file format:
 * <num_of_matrices>
 * <num_of_rows for first matrix>
 * <row0,col0> <row0,col1> .....
 * ....
 * ...
 * ..
 * <num_of_rows for second matrix>
 * ...
 * ..
 * .
 */


std::vector<std::vector<std::vector<float>>> read3DMatFromFile(std::string filename){
    std::vector<std::vector<std::vector<float>>> matAPP;
    std::vector<std::vector<float>> matrix;
    std::vector<float> lineAPP;
    std::string line;
    std::ifstream APP;
    APP.open(filename, std::ios::in);
    
    int num_of_matrices;
    int num_of_rows;
    
    APP >> num_of_matrices;
    //std::cout << "Num of matrices " << num_of_matrices << "\n";
    
    for (int i=0; i<num_of_matrices; i++){
        APP >> num_of_rows;
        //std::cout << "num of rows " << num_of_rows << "\n";
        for (int j=0; j <= num_of_rows;j++){
            std::getline(APP, line);
            //std::cout << line << std::endl;
            if (line != ""){
                std::istringstream iss(line);
                float n;
                while (iss >> n){
                    lineAPP.push_back(n);
                }
                matrix.push_back(lineAPP);
                lineAPP.clear();
            }
        }
        matAPP.push_back(matrix);
        matrix.clear();
    }
    
    return matAPP;
}

void write3DMatToFile(std::vector<std::vector<std::vector<float>>> matrices, std::string filename){
    std::ofstream mat_stream;
    mat_stream.open(filename);
    mat_stream << matrices.size() << "\n"; //num of matrices
    for (int i=0; i<matrices.size(); i++){ //for every matrix
        mat_stream << matrices[i].size() << "\n"; //num of rows in current matrix
        for (int j=0; j<matrices[i].size(); j++){ //for every line
            for (int k=0; k<matrices[i][j].size(); k++){ //for every entry in line
                mat_stream << matrices[i][j][k] << " "; //write element
            }
            mat_stream << "\n"; //after line
        }
        mat_stream << "\n";    //after matrix
    }
    mat_stream.close();
}

Match get_aim(Match* last_match, std::vector<Match>* faces, float longest_dist_possible_sqrt){
	float shortest_so_far = longest_dist_possible_sqrt;
	int idx;
	for (int count = 0; count < faces->size(); count ++){
		int x = (*faces)[count].center_x;
		int y = (*faces)[count].center_y;
		if (std::abs(
			 (x-last_match->center_x)*(x-last_match->center_x)
			  + (y-last_match->center_y)*(y-last_match->center_y)
			  ) <= shortest_so_far){
			idx = count;
			}
	};
	return (*faces)[idx];
};

Match closest_to_center(std::vector<Match>* faces, FaceDetector* FD){
    float shortest_so_far = (FD->img_width/2 * FD->img_width/2) + (FD->img_height/2 * FD->img_height/2);
    int idx;
	for (int count = 0; count < faces->size(); count ++){
		int x = (*faces)[count].center_x;
		int y = (*faces)[count].center_y;
		if (std::abs(
			 (x-FD->img_width/2)*(x-FD->img_width/2)
			  + (y-FD->img_height/2)*(y-FD->img_height/2)
			  ) <= shortest_so_far){
			idx = count;
			}
	};
	return (*faces)[idx];    
    
}
	
std::pair<int, int> get_threshold_val(float thr, Match* aim, FaceDetector* FD){
	//returns 0 if values in bounds and -1 or 1 for each that is off in one way or the other respectivly 
	int x=0;
	int y=0;
	float dist_width = FD->img_width/2 - aim->center_x;
	std::cout << "dist_width in fun: " << dist_width << std::endl;
	float dist_height = FD->img_height/2 - aim->center_y ;
	std::cout << "dist_height in fun: " << dist_height << std::endl;
	if (std::abs(dist_width) > thr){
	    x = (aim->center_x > (FD->img_width / 2)) ? -1 : 1;
	}
	if (std::abs(dist_height) > thr){
	    y = (aim->center_y > (FD->img_height / 2)) ? -1 : 1;
	}
	std::pair<int, int> dist{x,y};
	return dist;	
};

void show_matrix(std::vector<std::vector<float>> matrix){
    std::cout << "Matrix Size : " << matrix.size() << "x" << matrix[0].size() << std::endl;
    for(int i=0; i < matrix.size(); i++){
        for(int j=0; j < matrix[i].size(); j++){
            std::cout << " " << matrix[i][j];
        }
        std::cout << '\n';
    }
}
