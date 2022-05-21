#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "gender-estimator/gender-estimator.h"

std::shared_ptr<base::Logger> dl::GenderEstimator::m_logger = std::make_shared<base::Logger>();

namespace dl {

GenderEstimator::GenderEstimator(GenderEstimatorType type) {
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
GenderEstimator::InitializeNetworkPaths() {
	// CAFFE_227x227 AGE DETECTOR
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/estimators/gender-estimator/resource/caffe/227x227/gender_deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/estimators/gender-estimator/resource/caffe/227x227/gender_net.caffemodel";
	prop1.imageInputWidth = 227;
	prop1.imageInputHeight = 227;
	prop1.networkType = NetworkType::CAFFE;
	auto pair1 = std::make_pair(GenderEstimatorType::CAFFE_227x227, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
}

std::string
GenderEstimator::Estimate(const cv::Mat& face) {
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
	std::vector<float> genderPreds = m_network.forward("prob");
	int maxIndiceGender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
	auto gender = m_genderList[maxIndiceGender];
	return gender;
}

}