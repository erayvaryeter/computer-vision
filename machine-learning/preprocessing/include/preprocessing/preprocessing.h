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