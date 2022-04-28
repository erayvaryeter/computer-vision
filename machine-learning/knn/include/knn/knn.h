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

class KNN {
public:
	KNN(int k, int numberOfComponents, DataExtractionMethod method)
		: m_k(k),
		m_numberOfComponents(numberOfComponents),
		m_dataExtractionMethod(method)
	{
		m_knnPtr = cv::ml::KNearest::create();
	}
	
	~KNN() {}

	void ClearTrainImages() { m_trainImagesWithLabels.clear(); }
	void ClearTestImages() { m_testImagesWithLabels.clear(); }

	void AppendTrainImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension = ".jpg");
	void AppendTestImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension = ".jpg");

	void Train();
	void Test();

private:
	cv::Mat m_trainData;
	cv::Mat m_trainLabels;
	cv::Mat m_testData;
	cv::Mat m_testLabels;
	int m_numberOfComponents;
	int m_k;
	std::map<int, std::vector<cv::Mat>> m_trainImagesWithLabels;
	std::map<int, std::vector<cv::Mat>> m_testImagesWithLabels;
	cv::Ptr<cv::ml::KNearest> m_knnPtr;
	DataExtractionMethod m_dataExtractionMethod;
	static std::shared_ptr<base::Logger> m_logger;
};

}