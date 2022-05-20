#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image", "Image path", cxxopts::value<std::string>()->default_value("../../../../deep-learning/face-detection/resource/1.jpg"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string imagePath;

	if (result.count("image")) {
		imagePath = result["image"].as<std::string>();
		if (!base::File::FileExists(imagePath)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(imagePath);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		imagePath = result["image"].as<std::string>();
	}

	cv::Mat image = cv::imread(imagePath.c_str());
	auto detector = std::make_shared<dl::FaceDetector>(dl::FaceDetectorType::CAFFE_300x300);
	dl::DetectionParameters params;
	detector->SetDetectionParameters(params);
	auto detectionResults = detector->Detect(image, dl::Object::FACE);

	auto tracker = std::make_shared<video::Tracker>(7);
	tracker->AppendFaceDetector(detector);
	
	cv::VideoCapture cap(1);
	if (!cap.isOpened()) {
		std::cout << "Error opening webcam" << std::endl;
		return -1;
	}

	tracker->Run(cap);

	return 0;
}