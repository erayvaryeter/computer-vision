#include <object-detection/object-detection.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image", "Image path", cxxopts::value<std::string>()->default_value("../../../../deep-learning/object-detection/resource/1.jpg"))
		("config", "Config file path", cxxopts::value<std::string>()->default_value("../../../../deep-learning/object-detection/resource/face/caffe/deploy.prototxt"))
		("weight", "Weight file path", cxxopts::value<std::string>()->default_value("../../../../deep-learning/object-detection/resource/face/caffe/res10_300x300_ssd_iter_140000_fp16.caffemodel"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string imagePath, configPath, weightPath;

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

	if (result.count("config")) {
		configPath = result["config"].as<std::string>();
		if (!base::File::FileExists(configPath)) {
			std::cout << "Config file with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(configPath);
		if (ext != ".prototxt" && ext != ".pbtxt") {
			std::cout << "Config file path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		configPath = result["config"].as<std::string>();
	}

	if (result.count("weight")) {
		weightPath = result["weight"].as<std::string>();
		if (!base::File::FileExists(weightPath)) {
			std::cout << "Weight file with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(weightPath);
		if (ext != ".caffemodel" && ext != ".pb") {
			std::cout << "Weight file path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		weightPath = result["weight"].as<std::string>();
	}

	cv::Mat image = cv::imread(imagePath.c_str());
	auto detector = std::make_shared<dl::Detector>(dl::NetworkType::CAFFE, configPath, weightPath);
	detector->SetDetectionParameters(1.0, {104.0, 177.0, 123.0}, 0.70f);
	auto detectionResults = detector->Detect(image, dl::Object::FACE, 300, 300);

	cv::imshow("Result", detectionResults.imageWithBbox);
	cv::waitKey(0);

	return 0;
}