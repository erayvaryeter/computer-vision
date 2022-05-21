#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	auto detector = std::make_shared<dl::FaceDetector>(dl::FaceDetectorType::CAFFE_300x300, 
		dl::AgeEstimatorType::CAFFE_227x227, dl::GenderEstimatorType::CAFFE_227x227);
	dl::DetectionParameters params;
	detector->SetDetectionParameters(params);

	auto tracker = std::make_shared<video::Tracker>(7);
	tracker->AppendFaceDetector(detector);
	
	auto videoFile = "../../../../video-processing/tracking/resource/Faces.mp4";

	cv::VideoCapture cap(videoFile);
	if (!cap.isOpened()) {
		std::cout << "Error opening video file" << std::endl;
		return -1;
	}

	tracker->Run(cap);

	return 0;
}