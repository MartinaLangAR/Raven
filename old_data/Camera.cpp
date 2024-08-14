
#include <libcamera/libcamera/stream.h>
#include "Camera.h"
#include <time.h>

libCamera::libCamera()
{
    app = new LibcameraApp(std::make_unique<Options>());
    options = static_cast<Options *>(app->GetOptions());
    still_flags = LibcameraApp::FLAG_STILL_NONE;
    options->photo_width = 4056;
    options->photo_height = 3040;
    options->video_width = 640;
    options->video_height = 480;
    options->framerate = 30;
    options->denoise = "auto";
    options->timeout = 1000;
    options->setMetering(Metering_Modes::METERING_MATRIX);
    options->setExposureMode(Exposure_Modes::EXPOSURE_NORMAL);
    options->setWhiteBalance(WhiteBalance_Modes::WB_AUTO);
    options->contrast = 1.0f;
    options->saturation = 1.0f;
    still_flags |= LibcameraApp::FLAG_STILL_RGB;
    running.store(false, std::memory_order_release);;
    frameready.store(false, std::memory_order_release);;
    framebuffer=nullptr;
    camerastarted=false;
}

libCamera::~libCamera()
{
    delete app;
}

void libCamera::getImage(cv::Mat &frame, CompletedRequestPtr &payload)
{
    unsigned int w, h, stride;
    libcamera::Stream *stream = app->StillStream();
	app->StreamDimensions(stream, &w, &h, &stride);
    const std::vector<libcamera::Span<uint8_t>> mem = app->Mmap(payload->buffers[stream]);
    frame.create(h,w,CV_8UC3);
    uint ls = w*3;
    uint8_t *ptr = (uint8_t *)mem[0].data();
    for (unsigned int i = 0; i < h; i++, ptr += stride)
    {
        memcpy(frame.ptr(i),ptr,ls);
    }
}

bool libCamera::startPhoto()
{
    app->OpenCamera();
    app->ConfigureStill(still_flags);
    camerastarted=true;
    return true;
}
bool libCamera::stopPhoto()
{
    if(camerastarted){
        camerastarted=false;
        app->Teardown();
        app->CloseCamera();
    }
    return true;
}

bool libCamera::capturePhoto(cv::Mat &frame)
{
    if(!camerastarted){
        app->OpenCamera();
        app->ConfigureStill(still_flags);
    }
    app->StartCamera();
    LibcameraApp::Msg msg = app->Wait();
    if (msg.type == LibcameraApp::MsgType::Quit)
        return false;
    else if (msg.type != LibcameraApp::MsgType::RequestComplete)
        return false;
    if (app->StillStream())
    {
        app->StopCamera();
        getImage(frame, std::get<CompletedRequestPtr>(msg.payload));
        app->Teardown();
        app->CloseCamera();
    } else {
        std::cerr<<"Incorrect stream received"<<std::endl;
        return false;
        app->StopCamera();
        if(!camerastarted){
            app->Teardown();
            app->CloseCamera();
        }
    }
    return true;
}
