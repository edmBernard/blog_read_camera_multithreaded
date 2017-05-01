#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "opencv2/videoio.hpp"


class VideoStream
{
    public:
        VideoStream(std::string url);
        ~VideoStream();
        cv::Mat getFrame();

    private:
        void captureFrame();
        void start();
        void stop();
        
        int switch;
        std::vector<cv::Mat> frames;
        std::thread* t;
        cv::VideoCapture* stream;
};

VideoStream::VideoStream(std::string url)
{
    this->switch = 0;
    this->stream = new cv::VideoCapture(url);

    (*this->stream) >> frame;
    this->frames.push_back(frame);
    this->frames.push_back(frame);

    this->start()
}

VideoStream::~VideoStream()
{
    this->stop();
}

cv::Mat VideoStream::getFrame()
{
    this->switch = this->switch == 1 ? 0 : 1;
    return frames[this->switch == 1 ? 0 : 1];
}

void VideoStream::captureFrame()
{
    while (true)
    {
        Mat frame;
        // Grab frame from camera capture
        (*this->stream) >> frame;
        // Put frame to the "queue"
        this->frames[this->switch] = frame;
        // relaese frame resource
        frame.release();
    }
}

void VideoStream::start()
{
    this->t = new thread(&VideoStream::captureFrame, this);
}

void VideoStream::stop()
{
    cap = this->stream;
    if (cap->isOpened())
    {
        // Relase VideoCapture resource
        cap->release();
        cout << "Capture " << i << " released" << endl;
    }
}
