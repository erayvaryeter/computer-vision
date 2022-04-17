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

class BRIEFDetector {
public:
	BRIEFDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct BRIEFDetectorUiOptions {
		int bytes = 32;
		int useOrientation = 0;
	};

	struct BRIEFDetectorAlgOptions {
		int bytes = 32;
		bool useOrientation = false;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplyBRIEF(image, bytes, useOrientation);
			featureDetector->GetImageWithKeypoints().copyTo(outputImage);
		}
	};

	void Run() {

		auto OnBytesTrackbar = [](int val, void* ptr) {
			if (val == 16 || val == 32 || val == 64) {
				auto obj = static_cast<BRIEFDetectorAlgOptions*>(ptr);
				obj->bytes = val;
				obj->Run();
			}
		};

		auto OnOrientationTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<BRIEFDetectorAlgOptions*>(ptr);
			val == 0 ? obj->useOrientation = false : obj->useOrientation = true;
			obj->Run();
		};

		cv::namedWindow("BRIEF Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Bytes", "BRIEF Detector", &uiOptions.bytes, 65, OnBytesTrackbar, &algOptions);
		cv::createTrackbar("Use Orientation", "BRIEF Detector", &uiOptions.useOrientation, 1, OnOrientationTrackbar, &algOptions);

		OnBytesTrackbar(32, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("BRIEF Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	BRIEFDetectorUiOptions uiOptions;
	BRIEFDetectorAlgOptions algOptions;
};