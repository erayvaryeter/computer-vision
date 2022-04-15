#include <catch2/catch.hpp>
#include <logger/logger.h>
#include <fstream>
#include "Fixture.h"

auto filestorageLogger = std::make_shared<base::Logger>();

// #define DUMP_HARRIS_FEATURES

TEST_CASE_METHOD(Fixture, "Harris Corner Detection") {
	cv::Mat img = cv::imread("../../../../features/feature-detector/resource/Chessboard.jpg");
	auto detector = std::make_shared<features::FeatureDetector>();
	detector->ApplyHarrisCornerDetection(img, 4, 3, 0.04, 175.0f);
	auto features = detector->GetFeatures();

	std::string fileName = "../../../../features/feature-detector/resource/HarrisCorner.features";
#ifdef DUMP_HARRIS_FEATURES
	std::ofstream file_out(fileName.c_str(), std::ios::out, std::ios::binary);
	for (const auto& feature : features) {
		file_out.write((char*)&feature, sizeof(feature));
	}
	file_out.close();
	if (!file_out.good()) {
		std::cout << "Error occurred at writing time!" << std::endl;
		exit(0);
	}
#else
	std::ifstream file_in(fileName.c_str(), std::ios::in, std::ios::binary);
	for (const auto& feature : features) {
		cv::KeyPoint keypoint;
		file_in.read((char*)&keypoint, sizeof(keypoint));
		CHECK(feature.angle == keypoint.angle);
		CHECK(feature.class_id == keypoint.class_id);
		CHECK(feature.octave == keypoint.octave);
		CHECK(feature.pt == keypoint.pt);
		CHECK(feature.response == keypoint.response);
		CHECK(feature.size == keypoint.size);
	}
	file_in.close();
	if (!file_in.good()) {
		std::cout << "Error occurred at reading time!" << std::endl;
		exit(0);
	}
#endif
}