#include <instance-segmentation/instance-segmentation.h>
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
	auto segmentator = std::make_shared<dl::InstanceSegmentator>(dl::InstanceSegmentationType::TENSORFLOW_MASK_RCNN);
	dl::DetectionParameters params;
	params.inputName = "";
	params.meanValues = { 0.0, 0.0, 0.0 };
	params.outputDetectionName = "detection_out_final";
	params.outputMaskName = "detection_masks";

	segmentator->SetDetectionParameters(params);
	auto detectionResults = segmentator->Detect(image, std::nullopt);

	cv::imshow("Result", detectionResults.imageWithBboxAndMasks);
	cv::waitKey(0);

	return 0;
}