#include "facedetector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <ostream>
#include <fstream>
#include <opencv2/objdetect.hpp>

//Helper functions //////////////////////////////////////////////////////////////////////////////////////////////////
// Get the names of the output layers (yolov only)
std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net)
{
    static std::vector<cv::String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
         
        //get the names of all the layers in the network
        std::vector<cv::String> layersNames = net.getLayerNames();
         
        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i){
            names[i] = layersNames[outLayers[i] - 1];
	    }
    }
    return names;
}

// Draw the predicted bounding box // yolov only
void drawPred(std::vector<std::string> classes, int classId, float conf, int left, int top, int right, int bottom,
               cv::Mat& frame)
{
    //Draw a rectangle displaying the bounding box
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 3);
     
    //Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);
    if (!classes.empty()){
        label = classes[classId] + ":" + label;
    }
   
    //Display the label at the top of the bounding box
    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - round(1.5*labelSize.height)), 
                  cv::Point(left + round(1.5*labelSize.width), top + baseLine), 
                  cv::Scalar(255, 255, 255), cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0),1);
}

std::vector<Match> postprocess(double confThreshold, double nmsThreshold, std::vector<std::string> 
                                  classes, cv::Mat& frame, const std::vector<cv::Mat>& outs, 
                                  int min_height, int min_width, bool visuals) // yolov only
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
     
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold && (classIdPoint.x == 0 || classIdPoint.x == 16)) 
            //label "person" is on idx 0 and "dog" is 16
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                
                if (height > min_height and width > min_width){
					//std::cout << "width: " << width << ", height: " << height << std::endl;
					classIds.push_back(classIdPoint.x);
					confidences.push_back((float)confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));
				}
            }
        }
    }
     
    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices, 0.1, 1);
    std::vector<Match> filtered_matches;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        Match match;
        match = (Match){box.x + box.width/2, box.y + box.height/4, box.height, box.width};   // <----- we try to focus on the head and therefore height is divided by 4, not 2
        if (visuals){
			drawPred(classes, classIds[idx], confidences[idx], box.x, box.y,
					 box.x + box.width, box.y + box.height, frame);
        }
        filtered_matches.push_back(match); /////we could optmimize here an return original boxes and indices
    }
    return filtered_matches;
}
//remove fisheye effect <--------------------------to be coded
//cv::Mat undistort_image(cv::Mat origin_image){
//	}


////end of helper functions ////////////////////////////////////////////////////////////////////////////////

FaceDetector::FaceDetector(){
	std::ifstream ifs(classesFile.c_str());
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	//read class labels from data 
	while (getline(ifs,line)) classes.push_back(line);
};

void FaceDetector::capture_image(int path){
     std::string cmd = "libcamera-still --camera " + std::to_string(path) + " -o " 
                        + paths[path] + " --width " + std::to_string(img_width) +" --height " 
                        + std::to_string(img_height) + " -t 1 --nopreview 1";
    //std::cout << cmd << std::endl; 
     system(cmd.c_str());      
};

std::vector<Match> FaceDetector::detect_faces(int path_id, bool visuals){
		std::string path = paths[path_id];
		cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
		//resizing image <--- is this necessary?
		//cv::Mat image; 
		//cv::resize(img, image, cv::Size(img_width, img_height), cv::INTER_LINEAR);

		// only yolov3 ///////////////////////////////////////////////////////////////////////
		cv::Mat blob;
		// Create a 4D blob from a frame.
		cv::dnn::blobFromImage(img, blob, 1/255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0,0,0), true, false);
			 
		//Sets the input to the network
		net.setInput(blob);
		 
		// Runs the forward pass to get output of the output layers
		std::vector<cv::Mat> outs;
		net.forward(outs, getOutputsNames(net));
			 
		// Remove the bounding boxes with low confidence
		std::vector<Match> boxes = postprocess(confThreshold, nmsThreshold, classes, 
		                                          img, outs, min_height, min_width, visuals);
		
		//if (visuals){
		//	cv::imshow("result", img);
		//	cv::waitKey(1);
		//	cv::destroyAllWindows();
		//}
    	
    	return boxes;
}

