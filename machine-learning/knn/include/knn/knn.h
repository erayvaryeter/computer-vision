#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <preprocessing/preprocessing.h>

namespace base {
	class Logger;
}

namespace ml {

class KNN {
public:
	KNN(int k, int numberOfComponents, DataExtractionMethod method)
		: m_k(k),
		m_numberOfComponents(numberOfComponents)
	{
		m_knnPtr = cv::ml::KNearest::create();
		m_preprocessor = std::make_shared<Preprocessing>(m_k, m_numberOfComponents, method);
	}
	
	~KNN() {}

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
	static std::shared_ptr<base::Logger> m_logger;
};

}