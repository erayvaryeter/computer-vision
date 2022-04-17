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

class HarrisCornerDetector {
public:
	HarrisCornerDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct HarrisCornerUiOptions {
		int blockSize = 4;
		int apertureSize = 3;
		int k = 4;
		int thresh = 90;
	};

	struct HarrisCornerAlgOptions {
		int blockSize = 4;
		int apertureSize = 3;
		double k = 0.04;
		float thresh = 90.0f;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
	};

	void Run() {
		auto OnBlockSizeTrackbar = [](int val, void* ptr) {
			if (val > 0) {
				auto obj = static_cast<HarrisCornerAlgOptions*>(ptr);
				obj->blockSize = val;
				obj->featureDetector->ApplyHarrisCornerDetection(obj->image, obj->blockSize, obj->apertureSize, obj->k, obj->thresh);
				obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
			}
		};

		auto OnApertureSizeTrackbar = [](int val, void* ptr) {
			if (val % 2 == 1) {
				auto obj = static_cast<HarrisCornerAlgOptions*>(ptr);
				obj->apertureSize = val;
				obj->featureDetector->ApplyHarrisCornerDetection(obj->image, obj->blockSize, obj->apertureSize, obj->k, obj->thresh);
				obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
			}
		};

		auto OnKTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<HarrisCornerAlgOptions*>(ptr);
			obj->k = static_cast<double>(val) / 100.0;
			obj->featureDetector->ApplyHarrisCornerDetection(obj->image, obj->blockSize, obj->apertureSize, obj->k, obj->thresh);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		auto OnThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<HarrisCornerAlgOptions*>(ptr);
			obj->thresh = static_cast<float>(val);
			obj->featureDetector->ApplyHarrisCornerDetection(obj->image, obj->blockSize, obj->apertureSize, obj->k, obj->thresh);
			obj->featureDetector->GetImageWithKeypoints().copyTo(obj->outputImage);
		};

		cv::namedWindow("Harris Corner Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Block Size", "Harris Corner Detector", &uiOptions.blockSize, 21, OnBlockSizeTrackbar, &algOptions);
		cv::createTrackbar("Aperture Size", "Harris Corner Detector", &uiOptions.apertureSize, 15, OnApertureSizeTrackbar, &algOptions);
		cv::createTrackbar("K", "Harris Corner Detector", &uiOptions.k, 100, OnKTrackbar, &algOptions);
		cv::createTrackbar("Threshold", "Harris Corner Detector", &uiOptions.thresh, 255, OnThresholdTrackbar, &algOptions);

		OnBlockSizeTrackbar(4, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("Harris Corner Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	HarrisCornerUiOptions uiOptions;
	HarrisCornerAlgOptions algOptions;
};