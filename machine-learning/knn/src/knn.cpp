#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <pca/pca.h>

#include "knn/knn.h"

std::shared_ptr<base::Logger> ml::KNN::m_logger = std::make_shared<base::Logger>();

namespace ml {

void
KNN::AppendTrainData(std::string imageDirectory, int label, std::string imageExtension) {
	m_trainImageDirectories.emplace_back(std::move(imageDirectory));
	m_trainLabels.emplace_back(std::move(label));
	m_trainExtensions.emplace_back(std::move(imageExtension));
}

void
KNN::AppendTestData(std::string imageDirectory, int label, std::string imageExtension) {
	m_testImageDirectories.emplace_back(std::move(imageDirectory));
	m_testLabels.emplace_back(std::move(label));
	m_testExtensions.emplace_back(std::move(imageExtension));
}

void 
KNN::Train() {
	ASSERT(m_trainImageDirectories.size() == m_trainExtensions.size(), "Train data size is not valid", base::Logger::Severity::Error);
	ASSERT(m_trainExtensions.size() == m_trainLabels.size(), "Train data size is not valid", base::Logger::Severity::Error);
	m_preprocessor->ClearTrainImages();
	for (size_t i = 0; i < m_trainImageDirectories.size(); ++i)
		m_preprocessor->AppendTrainImagesWithLabels(m_trainImageDirectories[i], m_trainLabels[i], m_trainExtensions[i]);
	
	switch (m_method) {
	case DataExtractionMethod::PCA:
	{
		m_preprocessor->ApplyPcaTrain(m_numberOfComponents);
		break;
	}
	case DataExtractionMethod::HARRIS_CORNERS:
	{
		m_preprocessor->ApplyHarrisCornersTrain(m_numberOfComponents, m_harrisCornerParams.blockSize, m_harrisCornerParams.apertureSize, m_harrisCornerParams.k);
		break;
	}
	case DataExtractionMethod::SHI_TOMASI_CORNERS:
	{
		m_preprocessor->ApplyShiTomasiCornersTrain(m_numberOfComponents, m_shiTomasiCornerParams.maxCorners, m_shiTomasiCornerParams.qualityLevel,
			m_shiTomasiCornerParams.minDistance, m_shiTomasiCornerParams.blockSize, m_shiTomasiCornerParams.gradientSize, m_shiTomasiCornerParams.useHarris,
			m_shiTomasiCornerParams.k);
		break;
	}
	case DataExtractionMethod::SIFT:
	{
		m_preprocessor->ApplySiftTrain(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
	}
	case DataExtractionMethod::SURF:
	{
		m_preprocessor->ApplySiftTrain(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
	}
	default: break;
	}

	auto trainData = m_preprocessor->GetTrainData();
	auto trainLabels = m_preprocessor->GetTrainLabels();
	m_knnPtr->train(trainData, cv::ml::ROW_SAMPLE, trainLabels);
}

void
KNN::Test() {
	ASSERT(m_testImageDirectories.size() == m_testExtensions.size(), "Test data size is not valid", base::Logger::Severity::Error);
	ASSERT(m_testExtensions.size() == m_testLabels.size(), "Test data size is not valid", base::Logger::Severity::Error);
	m_preprocessor->ClearTestImages();
	for (size_t i = 0; i < m_testImageDirectories.size(); ++i)
		m_preprocessor->AppendTestImagesWithLabels(m_testImageDirectories[i], m_testLabels[i], m_testExtensions[i]);
	
	switch (m_method) {
	case DataExtractionMethod::PCA:
	{
		m_preprocessor->ApplyPcaTest(m_numberOfComponents);
		break;
	}
	case DataExtractionMethod::HARRIS_CORNERS:
	{
		m_preprocessor->ApplyHarrisCornersTest(m_numberOfComponents, m_harrisCornerParams.blockSize, m_harrisCornerParams.apertureSize, m_harrisCornerParams.k);
		break;
	}
	case DataExtractionMethod::SHI_TOMASI_CORNERS:
	{
		m_preprocessor->ApplyShiTomasiCornersTest(m_numberOfComponents, m_shiTomasiCornerParams.maxCorners, m_shiTomasiCornerParams.qualityLevel, 
			m_shiTomasiCornerParams.minDistance, m_shiTomasiCornerParams.blockSize, m_shiTomasiCornerParams.gradientSize, m_shiTomasiCornerParams.useHarris,
			m_shiTomasiCornerParams.k);
		break;
	}
	case DataExtractionMethod::SIFT:
	{
		m_preprocessor->ApplySiftTest(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
	}
	case DataExtractionMethod::SURF:
	{
		m_preprocessor->ApplySiftTest(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
	}
	default: break;
	}

	auto testData = m_preprocessor->GetTestData();
	auto testLabels = m_preprocessor->GetTestLabels();

	int correctClassify = 0;
	int wrongClassify = 0;

	for (int i = 0; i < testData.rows; i++) {
		cv::Mat result;
		cv::Mat neighbors;
		m_knnPtr->findNearest(testData.row(i), m_k, result, neighbors);
		int expected = testLabels.at<int>(i);
		int predicted = (int)result.at<float>(0);
		if (expected == predicted)
			correctClassify++;
		else
			wrongClassify++;
		std::cout << "Expected: " << expected << " - Predicted: " << predicted << " Neighbors: " << neighbors << std::endl;
	}

	std::cout << "-------------------------------------------------------------------------------" << std::endl;

	std::cout << "Correctly classified % " << static_cast<float>(correctClassify) / static_cast<float>(correctClassify + wrongClassify)
		* 100 << std::endl;

	std::cout << "Wrongly classified % " << static_cast<float>(wrongClassify) / static_cast<float>(correctClassify + wrongClassify)
		* 100 << std::endl;
}

}