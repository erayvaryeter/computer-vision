#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "age-estimator/age-estimator.h"

std::shared_ptr<base::Logger> dl::AgeEstimator::m_logger = std::make_shared<base::Logger>();

namespace dl {

AgeEstimator::AgeEstimator(AgeEstimatorType type, const std::string& inputName, const std::string& outputName) {
	m_ageEstimatorType = type;
	m_inputName = inputName;
	m_outputName = outputName;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_ageEstimatorType];
	if (m_networkProperties.expectedList.has_value())
		m_ageList = m_networkProperties.expectedList.value();

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
AgeEstimator::InitializeNetworkPaths() {
	// CAFFE_227x227 AGE DETECTOR
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/estimators/age-estimator/resource/caffe/227x227/age_deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/estimators/age-estimator/resource/caffe/227x227/age_net.caffemodel";
	prop1.imageInputWidth = 227;
	prop1.imageInputHeight = 227;
	prop1.networkType = NetworkType::CAFFE;
	prop1.expectedList = { "(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)", "(38-43)", "(48-53)", "(60-100)" };
	prop1.meanValues = cv::Scalar(78.4263377603, 87.7689143744, 114.895847746);
	auto pair1 = std::make_pair(AgeEstimatorType::CAFFE_227x227, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
	// ONNX_200x200 AGE DETECTOR
	NetworkProperties prop2;
	prop2.weightFilePath = "../../../../deep-learning/estimators/age-estimator/resource/onnx/200x200/AgePredictor.onnx";
	prop2.imageInputWidth = 200;
	prop2.imageInputHeight = 200;
	prop2.networkType = NetworkType::ONNX;
	prop2.expectedList = { 
		"(1-3)", "(4-6)", "(7-9)", "(10-12)", "(13-15)", "(16-18)", "(19-21)", "(22-24)", "(25-27)", "(28-30)",
		"(31-33)", "(34-36)", "(37-39)", "(40-42)", "(43-45)", "(46-48)", "(49-51)", "(52-54)", "(55-57)", "(58-60)",
		"(61-63)", "(64-66)", "(67-69)", "(70-72)", "(73-75)", "(76-78)", "(79-81)", "(82-84)", "(85-87)", "(88-90)",
		"(91-93)", "(94-96)", "(97-100)", "100+"
	};
	prop2.meanValues = cv::Scalar(0, 0, 0);
	auto pair2 = std::make_pair(AgeEstimatorType::ONNX_200x200, prop2);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair2);
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
		m_networkProperties.meanValues, false);
	m_network.setInput(inputBlob, m_inputName);
	std::vector<float> agePreds = m_network.forward();
	int maxIndiceAge = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
	auto age = m_ageList[maxIndiceAge];
	return age;
}

}