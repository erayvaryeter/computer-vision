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

class SURFDetector {
public:
	SURFDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct SURFDetectorUiOptions {
		int hessianThreshold = 100;
		int nOctaves = 4;
		int nOctaveLayers = 3;
		int extended = 0;
		int upright = 0;
	};

	struct SURFDetectorAlgOptions {
		double hessianThreshold = 100;
		int nOctaves = 4;
		int nOctaveLayers = 3;
		bool extended = false;
		bool upright = false;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplySURF(image);
			featureDetector->GetImageWithFeatures().copyTo(outputImage);
		}
	};

	void Run() {

		auto OnHessianThresholdTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SURFDetectorAlgOptions*>(ptr);
			obj->hessianThreshold = static_cast<double>(val);
			obj->Run();
		};

		auto OnNOctavesTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SURFDetectorAlgOptions*>(ptr);
			obj->nOctaves = val;
			obj->Run();
		};

		auto OnNOctaveLayersTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SURFDetectorAlgOptions*>(ptr);
			obj->nOctaveLayers = val;
			obj->Run();
		};

		auto OnExtendedTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SURFDetectorAlgOptions*>(ptr);
			val == 0 ? obj->extended = false : obj->extended = true;
			obj->Run();
		};

		auto OnUprightTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SURFDetectorAlgOptions*>(ptr);
			val == 0 ? obj->upright = false : obj->upright = true;
			obj->Run();
		};

		cv::namedWindow("SURF Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Hessian Threshold", "SURF Detector", &uiOptions.hessianThreshold, 255, OnHessianThresholdTrackbar, &algOptions);
		cv::createTrackbar("Nr. Octaves", "SURF Detector", &uiOptions.nOctaves, 20, OnNOctavesTrackbar, &algOptions);
		cv::createTrackbar("Nr. Octave Layers", "SURF Detector", &uiOptions.nOctaveLayers, 20, OnNOctaveLayersTrackbar, &algOptions);
		cv::createTrackbar("Extended", "SURF Detector", &uiOptions.extended, 1, OnExtendedTrackbar, &algOptions);
		cv::createTrackbar("Upright", "SURF Detector", &uiOptions.upright, 1, OnUprightTrackbar, &algOptions);

		OnHessianThresholdTrackbar(100, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("SURF Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	SURFDetectorUiOptions uiOptions;
	SURFDetectorAlgOptions algOptions;
};