#include <face-detection/face-detection.h>
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
	auto detector = std::make_shared<dl::FaceDetector>(dl::FaceDetectorType::CAFFE_300x300, 
		dl::AgeEstimatorType::CAFFE_227x227, dl::GenderEstimatorType::CAFFE_227x227);
	dl::DetectionParameters params;

	detector->SetDetectionParameters(params);
	auto detectionResults = detector->Detect(image, dl::Object::FACE);

	cv::imshow("Result", detectionResults.imageWithBbox);
	cv::waitKey(0);

	return 0;
}