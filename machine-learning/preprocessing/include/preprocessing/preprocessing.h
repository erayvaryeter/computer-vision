#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

namespace base {
	class Logger;
}

namespace ml {

enum class DataExtractionMethod {
	PCA = 0,
	HARRIS_CORNERS = 1,
	SHI_TOMASI_CORNERS = 2,
	SIFT = 3,
	SURF = 4,
	FAST = 5,
	BRIEF = 6,
	ORB = 7,
	BRISK = 8
};

class Preprocessing {
public:
	Preprocessing(int k, int numberOfComponents, DataExtractionMethod method)
		: m_dataExtractionMethod(method) {}
	
	~Preprocessing() {}

	void ClearTrainImages() { m_trainImagesWithLabels.clear(); }
	void ClearTestImages() { m_testImagesWithLabels.clear(); }

	void AppendTrainImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension = ".jpg");
	void AppendTestImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension = ".jpg");

	cv::Mat GetTrainData() { return m_trainData; }
	cv::Mat GetTrainLabels() { return m_trainLabels; }
	cv::Mat GetTestData() { return m_testData; }
	cv::Mat GetTestLabels() { return m_testLabels; }

	// Get principal components for data - label & vector of principal components
	void ApplyPcaTrain(int numComponents);
	void ApplyPcaTest(int numComponents);

	// Harris Corners
	void ApplyHarrisCornersTrain(int numComponents, int blockSize = 2, int apertureSize = 3, double k = 0.04);
	void ApplyHarrisCornersTest(int numComponents, int blockSize = 2, int apertureSize = 3, double k = 0.04);

	// Shi Tomasi Corners
	void ApplyShiTomasiCornersTrain(int numComponents, int maxCorners = 25, double qualityLevel = 0.01, double minDistance = 10, int blockSize = 3,
		int gradientSize = 3, bool useHarris = false, double k = 0.04);
	void ApplyShiTomasiCornersTest(int numComponents, int maxCorners = 25, double qualityLevel = 0.01, double minDistance = 10, int blockSize = 3,
		int gradientSize = 3, bool useHarris = false, double k = 0.04);

	// SIFT
	void ApplySiftTrain(int numComponents, int nFeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10.0,
		double sigma = 1.60);
	void ApplySiftTest(int numComponents, int nFeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10.0,
		double sigma = 1.60);

	// SURF
	void ApplySurfTrain(int numComponents, double hessianThreshold = 100, int nOctaves = 4, int nOctaveLayers = 3, bool extended = false,
		bool upright = false);
	void ApplySurfTest(int numComponents, double hessianThreshold = 100, int nOctaves = 4, int nOctaveLayers = 3, bool extended = false,
		bool upright = false);

	// FAST
	void ApplyFastTrain(int numComponents, int threshold = 10, bool nonmaxSupression = true);
	void ApplyFastTest(int numComponents, int threshold = 10, bool nonmaxSupression = true);

private:
	cv::Mat m_trainData;
	cv::Mat m_trainLabels;
	cv::Mat m_testData;
	cv::Mat m_testLabels;
	std::map<int, std::vector<cv::Mat>> m_trainImagesWithLabels;
	std::map<int, std::vector<cv::Mat>> m_testImagesWithLabels;
	DataExtractionMethod m_dataExtractionMethod;
	static std::shared_ptr<base::Logger> m_logger;
};

}