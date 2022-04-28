#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <pca/pca.h>

#include "preprocessing/preprocessing.h"

std::shared_ptr<base::Logger> ml::Preprocessing::m_logger = std::make_shared<base::Logger>();

namespace ml {

void
Preprocessing::AppendTrainImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension) {
	auto trainImages = base::File::GetFilesWithExtension(imageDirectory, imageExtension);
	ASSERT(!trainImages.empty(), "No image found in train images directory", base::Logger::Severity::Error);
	std::vector<cv::Mat> inputImages;
	for (auto im : trainImages) {
		cv::Mat image = cv::imread(im.first.c_str());
		ASSERT(!image.empty(), "Image could not be loaded: " + im.first, base::Logger::Severity::Error);
		inputImages.emplace_back(std::move(image));
	}
	auto pair = std::pair<int, std::vector<cv::Mat>>(label, inputImages);
	m_trainImagesWithLabels.insert(m_trainImagesWithLabels.end(), pair);
}

void
Preprocessing::AppendTestImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension) {
	auto testImages = base::File::GetFilesWithExtension(imageDirectory, imageExtension);
	ASSERT(!testImages.empty(), "No image found in test images directory", base::Logger::Severity::Error);
	std::vector<cv::Mat> inputImages;
	for (auto im : testImages) {
		cv::Mat image = cv::imread(im.first.c_str());
		ASSERT(!image.empty(), "Image could not be loaded: " + im.first, base::Logger::Severity::Error);
		inputImages.emplace_back(std::move(image));
	}
	auto pair = std::pair<int, std::vector<cv::Mat>>(label, inputImages);
	m_testImagesWithLabels.insert(m_testImagesWithLabels.end(), pair);
}

std::map<int, std::vector<cv::Mat>>
ApplyPCA(std::map<int, std::vector<cv::Mat>> trainImagesWithLabels, int numComponents) {
	auto pcaPtr = std::make_shared<ml::PCA>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto trainData : trainImagesWithLabels) {
		pcaPtr->ClearImages();
		for (auto image : trainData.second) {
			pcaPtr->AppendImage(image);
		}
		pcaPtr->ApplyPCA(numComponents);
		auto principalComponents = pcaPtr->GetPrincipalComponents();
		auto pair = std::make_pair(trainData.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplyPcaTrain(int numComponents) {
	m_trainData.release();
	m_trainLabels.release();
	auto pcaResult = ApplyPCA(m_trainImagesWithLabels, numComponents);
	for (auto components : pcaResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}

void
Preprocessing::ApplyPcaTest(int numComponents) {
	m_testData.release();
	m_testLabels.release();
	auto pcaResult = ApplyPCA(m_testImagesWithLabels, numComponents);
	for (auto components : pcaResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

}