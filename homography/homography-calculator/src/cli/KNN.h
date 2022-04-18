#pragma once

#include <homography-calculator/homography-calculator.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <file/file.h>
#include <cxxopts.hpp>
#include <thread>
#include <mutex>

class KnnMatching {
public:
	KnnMatching(std::string imagePath1, std::string imagePath2) {
		algOptions.image1 = cv::imread(imagePath1.c_str());
		algOptions.image2 = cv::imread(imagePath2.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
		algOptions.featureMatcher = std::make_shared<features::FeatureMatcher>(cv::DescriptorMatcher::MatcherType::BRUTEFORCE_HAMMING);
		algOptions.homographyCalculator = std::make_shared<homography::HomographyCalculator>();
	}

	struct KnnDetectorUiOptions {
		int thresh = 57;
		int octaves = 9;
		int patternScale = 16;
		int k = 5;
		int ratioThreshold = 70;
	};

	struct KnnDetectorAlgOptions {
		int thresh = 57;
		int octaves = 9;
		float patternScale = 1.6f;
		int k = 5;
		float ratioThreshold = 0.7f;
		cv::Mat image1;
		cv::Mat image2;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::shared_ptr<features::FeatureMatcher> featureMatcher;
		std::shared_ptr<homography::HomographyCalculator> homographyCalculator;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplyBRISK(image1, thresh, octaves, patternScale);
			auto keypoints1 = featureDetector->GetKeypoints();
			auto descriptors1 = featureDetector->GetDescriptors();
			featureDetector->ApplyBRISK(image2, thresh, octaves, patternScale);
			auto keypoints2 = featureDetector->GetKeypoints();
			auto descriptors2 = featureDetector->GetDescriptors();
			featureMatcher->ApplyKnnMatching(descriptors1, descriptors2, k, ratioThreshold);
			auto matches = featureMatcher->GetMatches();
			homographyCalculator->AddPoints(keypoints1, keypoints2, matches);
			auto warped = homographyCalculator->FindHomography(image1, image2);
			cv::Mat weighted;
			cv::addWeighted(warped, 0.5, image2, 0.5, 0, weighted);
			weighted.copyTo(outputImage);
		}
	};

	void Run() {

		auto OnThreshTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<KnnDetectorAlgOptions*>(ptr);
			obj->thresh = val;
			obj->Run();
		};

		auto OnOctavesTrackbar = [](int val, void* ptr) {
			if (val < 10) {
				auto obj = static_cast<KnnDetectorAlgOptions*>(ptr);
				obj->octaves = val;
				obj->Run();
			}
		};

		auto OnPatternScaleTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<KnnDetectorAlgOptions*>(ptr);
			obj->patternScale = static_cast<float>(val) / 10.0f;
			obj->Run();
		};

		auto OnKTrackbar = [](int val, void* ptr) {
			if (val > 1 && val % 2 == 1) {
				auto obj = static_cast<KnnDetectorAlgOptions*>(ptr);
				obj->k = val;
				obj->Run();
			}
		};

		auto OnRatioThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<KnnDetectorAlgOptions*>(ptr);
			obj->ratioThreshold = static_cast<float>(val) / 100.0f;
			obj->Run();
		};

		cv::namedWindow("KNN Matching", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Threshold", "KNN Matching", &uiOptions.thresh, 255, OnThreshTrackbar, &algOptions);
		cv::createTrackbar("Octaves", "KNN Matching", &uiOptions.octaves, 21, OnOctavesTrackbar, &algOptions);
		cv::createTrackbar("Pattern Scale", "KNN Matching", &uiOptions.patternScale, 100, OnPatternScaleTrackbar, &algOptions);
		cv::createTrackbar("K", "KNN Matching", &uiOptions.k, 21, OnKTrackbar, &algOptions);
		cv::createTrackbar("Ratio Threshold", "KNN Matching", &uiOptions.ratioThreshold, 100, OnRatioThresholdTrackbar, &algOptions);

		OnThreshTrackbar(30, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("KNN Matching", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	KnnDetectorUiOptions uiOptions;
	KnnDetectorAlgOptions algOptions;
};