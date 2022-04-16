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

class SIFTDetector {
public:
	SIFTDetector(std::string imagePath) {
		algOptions.image = cv::imread(imagePath.c_str());
		algOptions.featureDetector = std::make_shared<features::FeatureDetector>();
	}

	struct SIFTDetectorUiOptions {
		int nFeatures = 0;
		int nOctaveLayers = 3;
		int contrastThreshold = 4;
		int edgeThreshold = 10;
		int sigma = 16;
	};

	struct SIFTDetectorAlgOptions {
		int nFeatures = 0;
		int nOctaveLayers = 3;
		double contrastThreshold = 0.04;
		double edgeThreshold = 10.0;
		double sigma = 1.60;
		cv::Mat image;
		std::shared_ptr<features::FeatureDetector> featureDetector;
		std::mutex mtx;
		cv::Mat outputImage;
		void Run() {
			featureDetector->ApplySIFT(image, nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
			featureDetector->GetImageWithKeypoints().copyTo(outputImage);
		}
	};

	void Run() {
		auto OnNFeaturesTrackbar = [](int val, void* ptr) {
			auto obj = static_cast<SIFTDetectorAlgOptions*>(ptr);
			obj->nFeatures = val;
			obj->Run();
		};

		auto OnNOctaveLayers = [](int val, void* ptr) {
			if (val > 0) {
				auto obj = static_cast<SIFTDetectorAlgOptions*>(ptr);
				obj->nOctaveLayers = val;
				obj->Run();
			}
		};

		auto OnContrastThreshold = [](int val, void* ptr) {
			auto obj = static_cast<SIFTDetectorAlgOptions*>(ptr);
			obj->contrastThreshold = static_cast<double>(val) / 100.0;
			obj->Run();
		};

		auto OnEdgeThreshold = [](int val, void* ptr) {
			auto obj = static_cast<SIFTDetectorAlgOptions*>(ptr);
			obj->edgeThreshold = val;
			obj->Run();
		};

		auto OnSigma = [](int val, void* ptr) {
			if (val > 0) {
				auto obj = static_cast<SIFTDetectorAlgOptions*>(ptr);
				obj->sigma = static_cast<double>(val) / 10;
				obj->Run();
			}
		};

		cv::namedWindow("SIFT Detector", cv::WINDOW_KEEPRATIO);
		cv::createTrackbar("Nr. of Features", "SIFT Detector", &uiOptions.nFeatures, 500, OnNFeaturesTrackbar, &algOptions);
		cv::createTrackbar("Nr. of Octave Layers", "SIFT Detector", &uiOptions.nOctaveLayers, 21, OnNOctaveLayers, &algOptions);
		cv::createTrackbar("Contrast Threshold", "SIFT Detector", &uiOptions.contrastThreshold, 100, OnContrastThreshold, &algOptions);
		cv::createTrackbar("Edge Threshold", "SIFT Detector", &uiOptions.edgeThreshold, 100, OnEdgeThreshold, &algOptions);
		cv::createTrackbar("Sigma", "SIFT Detector", &uiOptions.sigma, 100, OnSigma, &algOptions);

		OnNFeaturesTrackbar(0, &algOptions);

		while (1) {
			std::lock_guard<std::mutex> lock(algOptions.mtx);
			cv::imshow("SIFT Detector", algOptions.outputImage);
			auto key = cv::waitKey(10);
			if (key == 27)
				break;
		}
	}

private:
	SIFTDetectorUiOptions uiOptions;
	SIFTDetectorAlgOptions algOptions;
};