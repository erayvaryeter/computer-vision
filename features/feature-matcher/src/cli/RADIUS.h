#pragma once

#include <feature-detector/feature-detector.h>
#include <feature-matcher/feature-matcher.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <file/file.h>
#include <cxxopts.hpp>
#include <thread>
#include <mutex>

class RadiusMatching {
public:
	RadiusMatching(std::string imagePath1, std::string imagePath2) {
		algOptions.image1 = cv::imread(imagePath1.c_str());
		algOptions.image2 = cv::imread(imagePath2.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
		algOptions.featureMatcher = std::make_shared<features::FeatureMatcher>(cv::DescriptorMatcher::MatcherType::BRUTEFORCE_HAMMING);
	}

	struct RadiusDetectorUiOptions {
		int thresh = 30;
		int octaves = 3;
		int patternScale = 10;
		int ratioThreshold = 70;
	};

	struct RadiusDetectorAlgOptions {
		int thresh = 30;
		int octaves = 3;
		float patternScale = 1.0f;
		float ratioThreshold = 0.7f;
		cv::Mat image1;
		cv::Mat image2;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::shared_ptr<features::FeatureMatcher> featureMatcher;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplyBRISK(image1, thresh, octaves, patternScale);
			auto keypoints1 = featureDetector->GetKeypoints();
			auto descriptors1 = featureDetector->GetDescriptors();
			featureDetector->ApplyBRISK(image2, thresh, octaves, patternScale);
			auto keypoints2 = featureDetector->GetKeypoints();
			auto descriptors2 = featureDetector->GetDescriptors();
			featureMatcher->ApplyRadiusMatching(descriptors1, descriptors2);
			auto matches = featureMatcher->GetMatches();
			cv::drawMatches(image1, keypoints1, image2, keypoints2, matches, outputImage, cv::Scalar::all(-1),
				cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		}
	};

	void Run() {

		auto OnThreshTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<RadiusDetectorAlgOptions*>(ptr);
			obj->thresh = val;
			obj->Run();
		};

		auto OnOctavesTrackbar = [](int val, void* ptr) {
			if (val < 10) {
				auto obj = static_cast<RadiusDetectorAlgOptions*>(ptr);
				obj->octaves = val;
				obj->Run();
			}
		};

		auto OnPatternScaleTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<RadiusDetectorAlgOptions*>(ptr);
			obj->patternScale = static_cast<float>(val) / 10.0f;
			obj->Run();
		};

		auto OnRatioThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<RadiusDetectorAlgOptions*>(ptr);
			obj->ratioThreshold = static_cast<float>(val) / 100.0f;
			obj->Run();
		};

		cv::namedWindow("Radius Matching", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Threshold", "Radius Matching", &uiOptions.thresh, 255, OnThreshTrackbar, &algOptions);
		cv::createTrackbar("Octaves", "Radius Matching", &uiOptions.octaves, 21, OnOctavesTrackbar, &algOptions);
		cv::createTrackbar("Pattern Scale", "Radius Matching", &uiOptions.patternScale, 100, OnPatternScaleTrackbar, &algOptions);
		cv::createTrackbar("Ratio Threshold", "Radius Matching", &uiOptions.ratioThreshold, 100, OnRatioThresholdTrackbar, &algOptions);

		OnThreshTrackbar(30, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("Radius Matching", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	RadiusDetectorUiOptions uiOptions;
	RadiusDetectorAlgOptions algOptions;
};