#include <catch2/catch.hpp>
#include <logger/logger.h>
#include <fstream>
#include "Fixture.h"

auto filestorageLogger = std::make_shared<base::Logger>();

//#define DUMP_HARRIS_FEATURES
//#define DUMP_SHI_TOMASI_FEATURES
//#define DUMP_SIFT_FEATURES
//#define DUMP_SURF_FEATURES

void
DumpFeatures(std::vector<cv::KeyPoint>& features, const char* filename) {
	std::ofstream file_out(filename, std::ios::out, std::ios::binary);
	for (const auto& feature : features) {
		file_out.write((char*)&feature, sizeof(feature));
	}
	file_out.close();
	if (!file_out.good()) {
		std::cout << "Error occurred at writing time!" << std::endl;
		exit(0);
	}
}

void
CompareFeatures(std::vector<cv::KeyPoint>& features, const char* filename) {
	std::ifstream file_in(filename, std::ios::in, std::ios::binary);
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
}

bool 
CompareKeypoints(cv::KeyPoint k1, cv::KeyPoint k2) {
	auto k1Val = std::sqrt(std::pow(k1.pt.x, 2) + std::pow(k1.pt.y, 2));
	auto k2Val = std::sqrt(std::pow(k2.pt.x, 2) + std::pow(k2.pt.y, 2));
	return (k1Val < k2Val);
}

TEST_CASE_METHOD(Fixture, "Harris Corner Detection") {
	cv::Mat img = cv::imread("../../../../features/feature-detector/resource/Chessboard.jpg");
	auto detector = std::make_shared<features::FeatureDetector>();
	detector->ApplyHarrisCornerDetection(img, 4, 3, 0.04, 175.0f);
	auto features = detector->GetKeypoints();
	std::sort(features.begin(), features.end(), CompareKeypoints);

	std::string fileName = "../../../../features/feature-detector/resource/HarrisCorner.features";
#ifdef DUMP_HARRIS_FEATURES
	DumpFeatures(features, fileName.c_str());
#else
	CompareFeatures(features, fileName.c_str());
#endif
}

TEST_CASE_METHOD(Fixture, "Shi Tomasi Corner Detection") {
	cv::Mat img = cv::imread("../../../../features/feature-detector/resource/Chessboard.jpg");
	auto detector = std::make_shared<features::FeatureDetector>();
	detector->ApplyShiTomasiCornerDetection(img, 100, 0.26, 10.0, 3, 3, false, 0.04);
	auto features = detector->GetKeypoints();
	std::sort(features.begin(), features.end(), CompareKeypoints);

	std::string fileName = "../../../../features/feature-detector/resource/ShiTomasiCorner.features";
#ifdef DUMP_SHI_TOMASI_FEATURES
	DumpFeatures(features, fileName.c_str());
#else
	CompareFeatures(features, fileName.c_str());
#endif
}

TEST_CASE_METHOD(Fixture, "SIFT") {
	cv::Mat img = cv::imread("../../../../features/feature-detector/resource/Chessboard.jpg");
	auto detector = std::make_shared<features::FeatureDetector>();
	detector->ApplySIFT(img, 0, 3, 0.4, 10.0, 1.6);
	auto features = detector->GetKeypoints();
	std::sort(features.begin(), features.end(), CompareKeypoints);

	std::string fileName = "../../../../features/feature-detector/resource/SIFT.features";
#ifdef DUMP_SIFT_FEATURES
	DumpFeatures(features, fileName.c_str());
#else
	CompareFeatures(features, fileName.c_str());
#endif
}

TEST_CASE_METHOD(Fixture, "SURF") {
	cv::Mat img = cv::imread("../../../../features/feature-detector/resource/Chessboard.jpg");
	auto detector = std::make_shared<features::FeatureDetector>();
	detector->ApplySURF(img, 100.0, 4, 3, false, false);
	auto features = detector->GetKeypoints();
	std::sort(features.begin(), features.end(), CompareKeypoints);

	std::string fileName = "../../../../features/feature-detector/resource/SURF.features";
#ifdef DUMP_SURF_FEATURES
	DumpFeatures(features, fileName.c_str());
#else
	// CompareFeatures(features, fileName.c_str());
#endif
}