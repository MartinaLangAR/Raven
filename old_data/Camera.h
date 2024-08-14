#pragma once

#include <mutex>
#include <atomic>
#include <pthread.h>
#include <opencv2/opencv.hpp>

#include "libcamera_app.hpp"
#include "libcamera_app_options.hpp"


class libCamera {

	protected:
		void run();
	protected:
		LibcameraApp *app;
		void getImage(cv::Mat &frame, CompletedRequestPtr &payload);
		unsigned int still_flags;
		unsigned int vw,vh,vstr;
		std::atomic<bool> running,frameready;
		uint8_t *framebuffer;
		std::mutex mtx;
		bool camerastarted;
    
    public:
		libCamera();
		~libCamera();

		Options *options;

		bool startPhoto();
		bool capturePhoto(cv::Mat &frame);
		bool stopPhoto();

};

