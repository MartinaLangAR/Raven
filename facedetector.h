#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <ostream>
#include <fstream>
#include <opencv2/objdetect.hpp>


struct Match {
    int center_x;
    int center_y;
    int height;
    int width;    
};


class FaceDetector{
	
	private:
	
	    std::vector<std::string> paths {"out0.jpg","out1.jpg"};
	    // Initialize the parameters
	    float confThreshold = 0.8; // Confidence threshold
	    float nmsThreshold = 0.9;  // Non-maximum suppression threshold
	    int inpWidth = 416;        // Width of network's input image
	    int inpHeight = 416;  
	    std::string classesFile = "coco.names";
	    std::string line;
	    std::vector<std::string> classes;

	    cv::dnn::Net net = cv::dnn::readNet("../darknet/yolov3.weights", ".././darknet/cfg/yolov3.cfg");
    
    public:
    
	    int img_height = 500;
	    int img_width = 600;
	    //min measurement for detected objects
	    int min_width = 30;
	    int min_height = 30;

	    FaceDetector();
	    void capture_image(int path);
	    std::vector<Match> detect_faces(int path_id, bool visuals);
    
    	
};
