#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "age-estimator/age-estimator.h"

std::shared_ptr<base::Logger> dl::AgeEstimator::m_logger = std::make_shared<base::Logger>();

namespace dl {

AgeEstimator::AgeEstimator(AgeEstimatorType type) {
	m_ageEstimatorType = type;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_ageEstimatorType];

	switch (m_networkProperties.networkType) {
	case NetworkType::CAFFE:
	{
		m_network = cv::dnn::readNetFromCaffe(m_networkProperties.configFilePath, m_networkProperties.weightFilePath);
		break;
	}
	case NetworkType::TENSORFLOW:
	{
		m_network = cv::dnn::readNetFromTensorflow(m_networkProperties.weightFilePath, m_networkProperties.configFilePath);
		break;
	}
	default:
	{
		break;
	}
	}
}

void
AgeEstimator::InitializeNetworkPaths() {
	// CAFFE_227x227 AGE DETECTOR
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/estimators/age-estimator/resource/caffe/227x227/age_deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/estimators/age-estimator/resource/caffe/227x227/age_net.caffemodel";
	prop1.imageInputWidth = 227;
	prop1.imageInputHeight = 227;
	prop1.networkType = NetworkType::CAFFE;
	auto pair1 = std::make_pair(AgeEstimatorType::CAFFE_227x227, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
}

std::string
AgeEstimator::Estimate(const cv::Mat& face) {
	auto frameWidth = face.size().width;
	auto frameHeight = face.size().height;
	auto ratioWidth = static_cast<double>(frameWidth) / static_cast<double>(m_networkProperties.imageInputWidth);
	auto ratioHeight = static_cast<double>(frameHeight) / static_cast<double>(m_networkProperties.imageInputHeight);
	auto ratio = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;
	cv::Mat resizedFace;
	cv::resize(face, resizedFace, cv::Size(static_cast<int>(frameWidth / ratio), static_cast<int>(frameHeight / ratio)));
	cv::Mat inputBlob = cv::dnn::blobFromImage(face, 1.0f, cv::Size(m_networkProperties.imageInputWidth, m_networkProperties.imageInputHeight),
		cv::Scalar(78.4263377603, 87.7689143744, 114.895847746), false);
	m_network.setInput(inputBlob);
	std::vector<float> agePreds = m_network.forward("prob");
	int maxIndiceAge = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
	auto age = m_ageList[maxIndiceAge];
	return age;
}

}