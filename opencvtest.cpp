#include <lccv.hpp>
#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat image;
    cv::Mat image1;
    lccv::PiCamera cam;
    
    
    cam.options->photo_width=2028;
    cam.options->photo_height=1520;
    cam.options->verbose=true;
    
    cam.startPhoto();

    lccv::PiCamera cam1;
    cam1.options->photo_width=2028;
    cam1.options->photo_height=1520;
    cam1.options->verbose=true;
    cam1.startPhoto();
    
    cam.capturePhoto(image);
    cv::imwrite("image.jpg", image);
    cam1.capturePhoto(image1);
    cv::imwrite("image1.jpg", image1);
    
    cam.stopPhoto();
    cam1.stopPhoto();

}
