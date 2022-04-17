#pragma once

#include <feature-detector/feature-detector.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <file/file.h>
#include <cxxopts.hpp>
#include <thread>
#include <mutex>

class ORBDetector {
public:
	ORBDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct ORBDetectorUiOptions {
		int nFeatures = 500;
		int scaleFactor = 12;
		int nLevels = 8;
		int edgeThreshold = 31;
		int firstLevel = 0;
		int WTA_K = 2;
		int st = 0;
		int patchSize = 31;
		int fastThreshold = 20;
	};

	struct ORBDetectorAlgOptions {
		int nFeatures = 500;
		float scaleFactor = 1.2f;
		int nLevels = 8;
		int edgeThreshold = 31;
		int firstLevel = 0;
		int WTA_K = 2;
		cv::ORB::ScoreType st = cv::ORB::ScoreType::HARRIS_SCORE;
		int patchSize = 31;
		int fastThreshold = 20;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplyORB(image, nFeatures, scaleFactor, nLevels, edgeThreshold, firstLevel, WTA_K, st, patchSize, fastThreshold);
			featureDetector->GetImageWithKeypoints().copyTo(outputImage);
		}
	};

	void Run() {

		auto OnNFeaturesTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
			obj->nFeatures = val;
			obj->Run();
		};

		auto OnScaleFactorTrackbar = [](int val, void* ptr) {
			if (val >= 10 & val <= 27) {
				auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
				obj->scaleFactor = static_cast<float>(val) / 10.0f;
				obj->Run();
			}
		};

		auto OnNLevelsTrackbar = [](int val, void* ptr) {
			if (val > 0) {
				auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
				obj->nLevels = val;
				obj->Run();
			}
		};

		auto OnEdgeThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
			obj->edgeThreshold = val;
			obj->Run();
		};

		auto OnFirstLevelTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
			obj->firstLevel = val;
			obj->Run();
		};

		auto OnWtakTrackbar = [](int val, void* ptr) {
			if (val >= 2 && val <= 4) {
				auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
				obj->WTA_K = val;
				obj->Run();
			}
		};

		auto OnScoreTypeTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
			val == 0 ? obj->st = cv::ORB::ScoreType::HARRIS_SCORE : obj->st = cv::ORB::ScoreType::FAST_SCORE;
			obj->Run();
		};

		auto OnPatchSizeTrackbar = [](int val, void* ptr) {
			if (val >= 2) {
				auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
				obj->patchSize = val;
				obj->Run();
			}
		};

		auto OnFastThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ORBDetectorAlgOptions*>(ptr);
			obj->fastThreshold = val;
			obj->Run();
		};

		cv::namedWindow("ORB Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Nr. Features", "ORB Detector", &uiOptions.nFeatures, 1000, OnNFeaturesTrackbar, &algOptions);
		cv::createTrackbar("Scale Factor", "ORB Detector", &uiOptions.scaleFactor, 100, OnScaleFactorTrackbar, &algOptions);
		cv::createTrackbar("Nr. Levels", "ORB Detector", &uiOptions.nLevels, 40, OnNLevelsTrackbar, &algOptions);
		cv::createTrackbar("Edge Threshold", "ORB Detector", &uiOptions.edgeThreshold, 100, OnEdgeThresholdTrackbar, &algOptions);
		cv::createTrackbar("First Level", "ORB Detector", &uiOptions.firstLevel, 21, OnFirstLevelTrackbar, &algOptions);
		cv::createTrackbar("WTA_K", "ORB Detector", &uiOptions.WTA_K, 4, OnWtakTrackbar, &algOptions);
		cv::createTrackbar("Score Type", "ORB Detector", &uiOptions.st, 1, OnScoreTypeTrackbar, &algOptions);
		cv::createTrackbar("Patch Size", "ORB Detector", &uiOptions.patchSize, 100, OnPatchSizeTrackbar, &algOptions);
		cv::createTrackbar("Fast Threshold", "ORB Detector", &uiOptions.fastThreshold, 100, OnFastThresholdTrackbar, &algOptions);

		OnNFeaturesTrackbar(500, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("FAST Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	ORBDetectorUiOptions uiOptions;
	ORBDetectorAlgOptions algOptions;
};