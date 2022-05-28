#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {

	dl::AgeEstimatorProperties ageProp = { dl::AgeEstimatorType::ONNX_200x200, "imageinput", "classoutput" };
	dl::GenderEstimatorProperties genderProp = { dl::GenderEstimatorType::ONNX_200x200, "imageinput", "classoutput" };
	dl::EthnicityEstimatorProperties ethnicityProp = { dl::EthnicityEstimatorType::ONNX_200x200, "imageinput", "classoutput" };

	auto detector = std::make_shared<dl::FaceDetector>(dl::FaceDetectorType::CAFFE_300x300, ageProp, genderProp, ethnicityProp);
	dl::DetectionParameters params;
	params.confidenceThreshold = 0.5;
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