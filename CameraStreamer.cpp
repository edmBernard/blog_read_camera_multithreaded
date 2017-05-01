#include "CameraStreamer.hpp"

CameraStreamer::CameraStreamer(vector<string> stream_source)
{
    this->camera_source = stream_source;
    this->camera_count = this->camera_source.size();
    isUSBCamera = false;

    this->startMultiCapture();
}

CameraStreamer::CameraStreamer(vector<int> capture_index)
{
    this->camera_index = capture_index;
    this->camera_count = capture_index.size();
    isUSBCamera = true;

    this->startMultiCapture();
}

CameraStreamer::~CameraStreamer()
{
    this->stopMultiCapture();
}

void CameraStreamer::captureFrame(int index)
{
    VideoCapture *capture = this->camera_capture[index];
    while (true)
    {
        Mat frame;
        // Grab frame from camera capture
        (*capture) >> frame;
        // Put frame to the queue
        this->frame_queue[index]->push(frame);
        // relase frame resource
        frame.release();
    }
}

void CameraStreamer::startMultiCapture()
{
    VideoCapture *capture;
    thread *t;
    concurrent_queue<Mat> *q;
    for (int i = 0; i < this->camera_count; i++)
    {
        // Make VideoCapture instance
        if (!isUSBCamera)
        {
            string url = this->camera_source[i];
            capture = new VideoCapture(url);
            cout << "Camera Setup: " << url << endl;
        }
        else
        {
            int idx = this->camera_index[i];
            capture = new VideoCapture(idx);
            cout << "Camera Setup: " << to_string(idx) << endl;
        }

        // Put VideoCapture to the vector
        this->camera_capture.push_back(capture);

        // Make thread instance
        t = new thread(&CameraStreamer::captureFrame, this, i);

        // Put thread to the vector
        this->camera_thread.push_back(t);

        // Make a queue instance
        q = new concurrent_queue<Mat>;

        // Put queue to the vector
        this->frame_queue.push_back(q);
    }
}

void CameraStreamer::stopMultiCapture()
{
    VideoCapture *cap;
    for (int i = 0; i < this->camera_count; i++)
    {
        cap = this->camera_capture[i];
        if (cap->isOpened())
        {
            // Relase VideoCapture resource
            cap->release();
            cout << "Capture " << i << " released" << endl;
        }
    }
}