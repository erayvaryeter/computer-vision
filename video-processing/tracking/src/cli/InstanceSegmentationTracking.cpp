#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	auto segmentator = std::make_shared<dl::InstanceSegmentator>(dl::InstanceSegmentationType::TENSORFLOW_MASK_RCNN);
	dl::DetectionParameters params;
	params.inputName = "";
	params.meanValues = { 0.0, 0.0, 0.0 };
	params.outputDetectionName = "detection_out_final";
	params.outputMaskName = "detection_masks";
	segmentator->SetDetectionParameters(params);

	auto tracker = std::make_shared<video::Tracker>(7);
	tracker->AppendInstanceSegmentator(segmentator);

	auto videoFile = "../../../../video-processing/tracking/resource/Cars.mp4";

	cv::VideoCapture cap(videoFile);
	if (!cap.isOpened()) {
		std::cout << "Error opening video file" << std::endl;
		return -1;
	}
	tracker->Run(cap);
	cap.release();

	return 0;
}