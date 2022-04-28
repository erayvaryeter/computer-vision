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
KNN::AppendTrainImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension) {
	auto trainImages = base::File::GetFilesWithExtension(imageDirectory, imageExtension);
	ASSERT(trainImages.empty(), "No image found in train images directory", base::Logger::Severity::Error);
	std::vector<cv::Mat> inputImages;
	for (auto im : trainImages) {
		cv::Mat image = cv::imread(im.first.c_str());
		ASSERT(image.empty(), "Image could not be loaded: " + im.first, base::Logger::Severity::Error);
		inputImages.emplace_back(std::move(image));
	}
	auto pair = std::pair<int, std::vector<cv::Mat>>(label, inputImages);
	m_trainImagesWithLabels.insert(m_trainImagesWithLabels.end(), pair);
}

void
KNN::AppendTestImagesWithLabels(std::string imageDirectory, int label, std::string imageExtension) {
	auto testImages = base::File::GetFilesWithExtension(imageDirectory, imageExtension);
	ASSERT(testImages.empty(), "No image found in test images directory", base::Logger::Severity::Error);
	std::vector<cv::Mat> inputImages;
	for (auto im : testImages) {
		cv::Mat image = cv::imread(im.first.c_str());
		ASSERT(image.empty(), "Image could not be loaded: " + im.first, base::Logger::Severity::Error);
		inputImages.emplace_back(std::move(image));
	}
	auto pair = std::pair<int, std::vector<cv::Mat>>(label, inputImages);
	m_testImagesWithLabels.insert(m_testImagesWithLabels.end(), pair);
}

void 
KNN::Train() {
	m_trainData.release();
	m_trainLabels.release();
}

void
KNN::Test() {

}

}