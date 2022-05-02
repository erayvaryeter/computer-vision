#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <preprocessing/preprocessing.h>

#include "parameters.h"

namespace base {
	class Logger;
}

namespace ml {

class KNN {
public:
	KNN(int k, int numberOfComponents, DataExtractionMethod method)
		: m_k(k),
		m_numberOfComponents(numberOfComponents),
		m_method(method)
	{
		m_knnPtr = cv::ml::KNearest::create();
		m_preprocessor = std::make_shared<Preprocessing>(m_k, m_numberOfComponents, m_method);
	}
	
	~KNN() {}

	// PARAMETERS
	void SetHarrisCornerParams(int blockSize = 2, int apertureSize = 3, double k = 0.04) {
		m_harrisCornerParams.blockSize = blockSize;
		m_harrisCornerParams.apertureSize = apertureSize;
		m_harrisCornerParams.k = k;
	}

	void SetShiTomasiCornerParams(int maxCorners = 25, double qualityLevel = 0.01, double minDistance = 10, int blockSize = 3,
		int gradientSize = 3, bool useHarris = false, double k = 0.04) {
		m_shiTomasiCornerParams.maxCorners = maxCorners;
		m_shiTomasiCornerParams.qualityLevel = qualityLevel;
		m_shiTomasiCornerParams.minDistance = minDistance;
		m_shiTomasiCornerParams.blockSize = blockSize;
		m_shiTomasiCornerParams.gradientSize = gradientSize;
		m_shiTomasiCornerParams.useHarris = useHarris;
		m_shiTomasiCornerParams.k = k;
	}

	void SetSiftParams(int nFeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10.0, double sigma = 1.60) {
		m_siftParams.nFeatures = nFeatures;
		m_siftParams.nOctaveLayers = nOctaveLayers;
		m_siftParams.contrastThreshold = contrastThreshold;
		m_siftParams.edgeThreshold = edgeThreshold;
		m_siftParams.sigma = sigma;
	}

	void SetSurfParams(double hessianThreshold = 100, int nOctaves = 4, int nOctaveLayers = 3, bool extended = false, bool upright = false) {
		m_surfParams.hessianThreshold = hessianThreshold;
		m_surfParams.nOctaves = nOctaves;
		m_surfParams.nOctaveLayers = nOctaveLayers;
		m_surfParams.extended = extended;
		m_surfParams.upright = upright;
	}

	void SetFastParams(int threshold = 10, bool nonmaxSupression = true) {
		m_fastParams.threshold = threshold;
		m_fastParams.nonmaxSupression = nonmaxSupression;
	}

	void SetBriefParams(int bytes = 32, bool useOrientation = false) {
		m_briefParams.bytes = bytes;
		m_briefParams.useOrientation = useOrientation;
	}

	void SetOrbParams(int nFeatures = 500, float scaleFactor = 1.2f, int nLevels = 8, int edgeThreshold = 31, int firstLevel = 0,
			int WTA_K = 2, cv::ORB::ScoreType st = cv::ORB::ScoreType::HARRIS_SCORE, int patchSize = 31, int fastThreshold = 20) {
		m_orbParams.nFeatures = nFeatures;
		m_orbParams.scaleFactor = scaleFactor;
		m_orbParams.nLevels = nLevels;
		m_orbParams.edgeThreshold = edgeThreshold;
		m_orbParams.firstLevel = firstLevel;
		m_orbParams.WTA_K = WTA_K;
		m_orbParams.st = st;
		m_orbParams.patchSize = patchSize;
		m_orbParams.fastThreshold = fastThreshold;
	}

	void ClearTrainData() { 
		m_trainImageDirectories.clear();
		m_trainLabels.clear();
		m_trainExtensions.clear();
	}

	void ClearTestData() {
		m_testImageDirectories.clear();
		m_testLabels.clear();
		m_testExtensions.clear();
	}

	void AppendTrainData(std::string imageDirectory, int label, std::string imageExtension = ".jpg");
	void AppendTestData(std::string imageDirectory, int label, std::string imageExtension = ".jpg");

	void Train();
	void Test();

private:
	int m_numberOfComponents;
	int m_k;
	std::vector<std::string> m_trainImageDirectories;
	std::vector<std::string> m_testImageDirectories;
	std::vector<int> m_trainLabels;
	std::vector<int> m_testLabels;
	std::vector<std::string> m_trainExtensions;
	std::vector<std::string> m_testExtensions;
	cv::Ptr<cv::ml::KNearest> m_knnPtr;
	std::shared_ptr<Preprocessing> m_preprocessor;
	DataExtractionMethod m_method;
	static std::shared_ptr<base::Logger> m_logger;

	HarrisCornerParams m_harrisCornerParams;
	ShiTomasiCornerParams m_shiTomasiCornerParams;
	SiftParams m_siftParams;
	SurfParams m_surfParams;
	FastParams m_fastParams;
	BriefParams m_briefParams;
	OrbParams m_orbParams;
};

}