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

class ShiTomasiCornerDetector {
public:
	ShiTomasiCornerDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct ShiTomasiCornerUiOptions {
		int maxCorners = 100;
		int qualityLevel = 26;
		int minDistance = 10;
		int blockSize = 3;
		int gradientSize = 3;
		int useHarris = 0;
		int k = 4;
	};

	struct ShiTomasiCornerAlgOptions {
		int maxCorners = 100;
		double qualityLevel = 0.26;
		double minDistance = 10;
		int blockSize = 3;
		int gradientSize = 3;
		bool useHarris = false;
		double k = 0.04;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
	};

	void Run() {
		auto OnMaxCornersTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
			obj->maxCorners = val;
			obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
				obj->gradientSize, obj->useHarris, obj->k);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		auto OnQualityLevelTrackbar = [](int val, void* ptr) {
			if (val > 0) {
				auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
				obj->qualityLevel = static_cast<double>(val) / 100.0;
				obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
					obj->gradientSize, obj->useHarris, obj->k);
				obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
			}
		};

		auto OnMinDistanceTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
			obj->minDistance = static_cast<double>(val);
			obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
				obj->gradientSize, obj->useHarris, obj->k);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		auto OnBlockSizeTrackbar = [](int val, void* ptr) {
			if (val % 2 != 0) {
				auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
				obj->blockSize = val;
				obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
					obj->gradientSize, obj->useHarris, obj->k);
				obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
			}
		};

		auto OnGradientSizeTrackbar = [](int val, void* ptr) {
			if (val % 2 != 0) {
				auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
				obj->gradientSize = val;
				obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
					obj->gradientSize, obj->useHarris, obj->k);
				obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
			}
		};

		auto OnKTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
			obj->k = static_cast<double>(val) / 100.0;
			obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
				obj->gradientSize, obj->useHarris, obj->k);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		auto OnUseHarrisTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<ShiTomasiCornerAlgOptions*>(ptr);
			val == 0 ? obj->k = false : obj->k = true;
			obj->featureDetector->ApplyShiTomasiCornerDetection(obj->image, obj->maxCorners, obj->qualityLevel, obj->minDistance, obj->blockSize,
				obj->gradientSize, obj->useHarris, obj->k);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		cv::namedWindow("Shi Tomasi Corner Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Max Corners", "Shi Tomasi Corner Detector", &uiOptions.maxCorners, 100, OnMaxCornersTrackbar, &algOptions);
		cv::createTrackbar("Quality Level", "Shi Tomasi Corner Detector", &uiOptions.qualityLevel, 100, OnQualityLevelTrackbar, &algOptions);
		cv::createTrackbar("Min Distance", "Shi Tomasi Corner Detector", &uiOptions.minDistance, 50, OnMinDistanceTrackbar, &algOptions);
		cv::createTrackbar("Block Size", "Shi Tomasi Corner Detector", &uiOptions.blockSize, 21, OnBlockSizeTrackbar, &algOptions);
		cv::createTrackbar("Gradient Size", "Shi Tomasi Corner Detector", &uiOptions.gradientSize, 21, OnGradientSizeTrackbar, &algOptions);
		cv::createTrackbar("K", "Shi Tomasi Corner Detector", &uiOptions.k, 100, OnKTrackbar, &algOptions);
		cv::createTrackbar("Use Harris", "Shi Tomasi Corner Detector", &uiOptions.useHarris, 1, OnUseHarrisTrackbar, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("Shi Tomasi Corner Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	ShiTomasiCornerUiOptions uiOptions;
	ShiTomasiCornerAlgOptions algOptions;
};