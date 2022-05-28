#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "gender-estimator/gender-estimator.h"

std::shared_ptr<base::Logger> dl::GenderEstimator::m_logger = std::make_shared<base::Logger>();

namespace dl {

GenderEstimator::GenderEstimator(GenderEstimatorType type, const std::string& inputName, const std::string& outputName) {
	m_ageEstimatorType = type;
	m_inputName = inputName;
	m_outputName = outputName;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_ageEstimatorType];
	if (m_networkProperties.expectedList.has_value())
		m_genderList = m_networkProperties.expectedList.value();

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
	case NetworkType::ONNX:
	{
		m_network = cv::dnn::readNetFromONNX(m_networkProperties.weightFilePath);
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
	// CAFFE_227x227 GENDER DETECTOR
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/estimators/gender-estimator/resource/caffe/227x227/gender_deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/estimators/gender-estimator/resource/caffe/227x227/gender_net.caffemodel";
	prop1.imageInputWidth = 227;
	prop1.imageInputHeight = 227;
	prop1.networkType = NetworkType::CAFFE;
	prop1.expectedList = { "Male", "Female" };
	prop1.meanValues = cv::Scalar(78.4263377603, 87.7689143744, 114.895847746);
	auto pair1 = std::make_pair(GenderEstimatorType::CAFFE_227x227, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
	// ONNX_200x200 GENDER DETECTOR
	NetworkProperties prop2;
	prop2.weightFilePath = "../../../../deep-learning/estimators/gender-estimator/resource/onnx/200x200/GenderPredictor.onnx";
	prop2.imageInputWidth = 200;
	prop2.imageInputHeight = 200;
	prop2.networkType = NetworkType::ONNX;
	prop2.expectedList = { "Female", "Male" };
	prop1.meanValues = cv::Scalar(0, 0, 0);
	auto pair2 = std::make_pair(GenderEstimatorType::ONNX_200x200, prop2);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair2);
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
		m_networkProperties.meanValues, false);
	m_network.setInput(inputBlob, m_inputName);
	std::vector<float> genderPreds = m_network.forward();
	int maxIndiceGender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
	auto gender = m_genderList[maxIndiceGender];
	return gender;
}

}