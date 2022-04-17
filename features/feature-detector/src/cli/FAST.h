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

class FASTDetector {
public:
	FASTDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct FASTDetectorUiOptions {
		int threshold = 10;
		int nonmaxSupression = 1;
	};

	struct FASTDetectorAlgOptions {
		int threshold = 10;
		bool nonmaxSupression = true;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplyFAST(image);
			featureDetector->GetImageWithKeypoints().copyTo(outputImage);
		}
	};

	void Run() {

		auto OnThresholdTracker = [](int val, void* ptr) {
			auto obj = static_cast<FASTDetectorAlgOptions*>(ptr);
			obj->threshold = val;
			obj->Run();
		};

		auto OnNonmaxSupressionTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<FASTDetectorAlgOptions*>(ptr);
			val == 0 ? obj->nonmaxSupression = true : obj->nonmaxSupression = false;
			obj->Run();
		};

		cv::namedWindow("FAST Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Threshold", "FAST Detector", &uiOptions.threshold, 255, OnThresholdTracker, &algOptions);
		cv::createTrackbar("Nonmax Supression", "FAST Detector", &uiOptions.nonmaxSupression, 1, OnNonmaxSupressionTrackbar, &algOptions);

		OnThresholdTracker(10, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("FAST Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	FASTDetectorUiOptions uiOptions;
	FASTDetectorAlgOptions algOptions;
};