#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <pca/pca.h>
#include <feature-detector/feature-detector.h>

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
ApplyPCA(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents) {
	auto pcaPtr = std::make_shared<ml::PCA>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		pcaPtr->ClearImages();
		for (auto image : data.second) {
			pcaPtr->AppendImage(image);
		}
		pcaPtr->ApplyPCA(numComponents);
		auto principalComponents = pcaPtr->GetPrincipalComponents();
		auto pair = std::make_pair(data.first, principalComponents);
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

std::map<int, std::vector<cv::Mat>>
ApplyHarrisCorners(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents, int blockSize, int apertureSize, double k) {
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		std::vector<cv::Mat> principalComponents;
		for (auto image : data.second) {
			featureDetector->ApplyHarrisCornerDetection(image, blockSize, apertureSize, k);
			auto component = featureDetector->GetPrincipalComponents();
			if (component.rows != 0 && component.cols != 0) {
				auto rectangleSize = numComponents <= component.cols ? numComponents : component.cols;
				component = component(cv::Rect(0, 0, rectangleSize, 1));
				principalComponents.emplace_back(std::move(component.t()));
			}
		}
		auto pair = std::make_pair(data.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplyHarrisCornersTrain(int numComponents, int blockSize, int apertureSize, double k) {
	m_trainData.release();
	m_trainLabels.release();
	auto harrisResult = ApplyHarrisCorners(m_trainImagesWithLabels, numComponents, blockSize, apertureSize, k);
	for (auto components : harrisResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}

void
Preprocessing::ApplyHarrisCornersTest(int numComponents, int blockSize, int apertureSize, double k) {
	m_testData.release();
	m_testLabels.release();
	auto harrisResult = ApplyHarrisCorners(m_testImagesWithLabels, numComponents, blockSize, apertureSize, k);
	for (auto components : harrisResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

std::map<int, std::vector<cv::Mat>>
ApplyShiTomasiCorners(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents, int maxCorners, double qualityLevel, double minDistance, 
	int blockSize, int gradientSize, bool useHarris, double k) {
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		std::vector<cv::Mat> principalComponents;
		for (auto image : data.second) {
			featureDetector->ApplyShiTomasiCornerDetection(image, maxCorners, qualityLevel, minDistance, blockSize, gradientSize, useHarris, k);
			auto component = featureDetector->GetPrincipalComponents();
			if (component.rows != 0 && component.cols != 0) {
				auto rectangleSize = numComponents <= component.cols ? numComponents : component.cols;
				component = component(cv::Rect(0, 0, rectangleSize, 1));
				principalComponents.emplace_back(std::move(component.t()));
			}
		}
		auto pair = std::make_pair(data.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplyShiTomasiCornersTrain(int numComponents, int maxCorners, double qualityLevel, double minDistance, int blockSize, int gradientSize, 
	bool useHarris, double k) {
	m_trainData.release();
	m_trainLabels.release();
	auto shiTomasiResult = ApplyShiTomasiCorners(m_trainImagesWithLabels, numComponents, maxCorners, qualityLevel, minDistance, blockSize, gradientSize, 
		useHarris, k);
	for (auto components : shiTomasiResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}
void
Preprocessing::ApplyShiTomasiCornersTest(int numComponents, int maxCorners, double qualityLevel, double minDistance, int blockSize, int gradientSize,
	bool useHarris, double k) {
	m_testData.release();
	m_testLabels.release();
	auto shiTomasiResult = ApplyShiTomasiCorners(m_testImagesWithLabels, numComponents, maxCorners, qualityLevel, minDistance, blockSize, gradientSize,
		useHarris, k);
	for (auto components : shiTomasiResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

std::map<int, std::vector<cv::Mat>>
ApplySift(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents, int nFeatures, int nOctaveLayers, double contrastThreshold, 
	double edgeThreshold, double sigma) {
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		std::vector<cv::Mat> principalComponents;
		for (auto image : data.second) {
			featureDetector->ApplySIFT(image, nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
			auto component = featureDetector->GetPrincipalComponents();
			if (component.rows != 0 && component.cols != 0) {
				auto rectangleSize = numComponents <= component.cols ? numComponents : component.cols;
				component = component(cv::Rect(0, 0, rectangleSize, 1));
				principalComponents.emplace_back(std::move(component.t()));
			}
		}
		auto pair = std::make_pair(data.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplySiftTrain(int numComponents, int nFeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma) {
	m_trainData.release();
	m_trainLabels.release();
	auto siftResult = ApplySift(m_trainImagesWithLabels, numComponents, nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
	for (auto components : siftResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}

void
Preprocessing::ApplySiftTest(int numComponents, int nFeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma) {
	m_testData.release();
	m_testLabels.release();
	auto siftResult = ApplySift(m_testImagesWithLabels, numComponents, nFeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
	for (auto components : siftResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

std::map<int, std::vector<cv::Mat>>
ApplySurf(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents, double hessianThreshold, int nOctaves, int nOctaveLayers, bool extended, 
	bool upright) {
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		std::vector<cv::Mat> principalComponents;
		for (auto image : data.second) {
			featureDetector->ApplySURF(image, hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
			auto component = featureDetector->GetPrincipalComponents();
			if (component.rows != 0 && component.cols != 0) {
				auto rectangleSize = numComponents <= component.cols ? numComponents : component.cols;
				component = component(cv::Rect(0, 0, rectangleSize, 1));
				principalComponents.emplace_back(std::move(component.t()));
			}
		}
		auto pair = std::make_pair(data.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplySurfTrain(int numComponents, double hessianThreshold, int nOctaves, int nOctaveLayers, bool extended, bool upright) {
	m_trainData.release();
	m_trainLabels.release();
	auto siftResult = ApplySurf(m_trainImagesWithLabels, numComponents, hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
	for (auto components : siftResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}

void
Preprocessing::ApplySurfTest(int numComponents, double hessianThreshold, int nOctaves, int nOctaveLayers, bool extended, bool upright) {
	m_testData.release();
	m_testLabels.release();
	auto siftResult = ApplySurf(m_testImagesWithLabels, numComponents, hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
	for (auto components : siftResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

std::map<int, std::vector<cv::Mat>>
ApplyFast(std::map<int, std::vector<cv::Mat>> imagesWithLabels, int numComponents, int threshold, bool nonmaxSupression) {
	auto featureDetector = std::make_shared<features::FeatureDetector>();
	std::map<int, std::vector<cv::Mat>> retVal;
	for (auto data : imagesWithLabels) {
		std::vector<cv::Mat> principalComponents;
		for (auto image : data.second) {
			featureDetector->ApplyFAST(image, threshold, nonmaxSupression);
			auto component = featureDetector->GetPrincipalComponents();
			if (component.rows != 0 && component.cols != 0) {
				auto rectangleSize = numComponents <= component.cols ? numComponents : component.cols;
				component = component(cv::Rect(0, 0, rectangleSize, 1));
				principalComponents.emplace_back(std::move(component.t()));
			}
		}
		auto pair = std::make_pair(data.first, principalComponents);
		retVal.insert(retVal.end(), std::move(pair));
	}
	return retVal;
}

void
Preprocessing::ApplyFastTrain(int numComponents, int threshold, bool nonmaxSupression) {
	m_trainData.release();
	m_trainLabels.release();
	auto fastResult = ApplyFast(m_trainImagesWithLabels, numComponents, threshold, nonmaxSupression);
	for (auto components : fastResult) {
		for (auto component : components.second) {
			m_trainData.push_back(component.t());
			m_trainLabels.push_back(components.first);
		}
	}
}

void
Preprocessing::ApplyFastTest(int numComponents, int threshold, bool nonmaxSupression) {
	m_testData.release();
	m_testLabels.release();
	auto fastResult = ApplyFast(m_testImagesWithLabels, numComponents, threshold, nonmaxSupression);
	for (auto components : fastResult) {
		for (auto component : components.second) {
			m_testData.push_back(component.t());
			m_testLabels.push_back(components.first);
		}
	}
}

}