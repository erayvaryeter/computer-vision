#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "ethnicity-estimator/ethnicity-estimator.h"

std::shared_ptr<base::Logger> dl::EthnicityEstimator::m_logger = std::make_shared<base::Logger>();

namespace dl {

EthnicityEstimator::EthnicityEstimator(EthnicityEstimatorType type, const std::string& inputName, const std::string& outputName) {
	m_ethnicityEstimatorType = type;
	m_inputName = inputName;
	m_outputName = outputName;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_ethnicityEstimatorType];
	if (m_networkProperties.expectedList.has_value())
		m_ethnicityList = m_networkProperties.expectedList.value();

	switch (m_networkProperties.networkType) {
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
EthnicityEstimator::InitializeNetworkPaths() {
	// ONNX_200x200 ETHNICITY DETECTOR
	NetworkProperties prop1;
	prop1.weightFilePath = "../../../../deep-learning/estimators/ethnicity-estimator/resource/onnx/200x200/EthnicityPredictor.onnx";
	prop1.imageInputWidth = 200;
	prop1.imageInputHeight = 200;
	prop1.networkType = NetworkType::ONNX;
	prop1.expectedList = { "Asian", "Black", "Indian", "Other", "White" };
	prop1.meanValues = cv::Scalar(0, 0, 0);
	auto pair1 = std::make_pair(EthnicityEstimatorType::ONNX_200x200, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
}

std::string
EthnicityEstimator::Estimate(const cv::Mat& face) {
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
	std::vector<float> ethnicityPreds = m_network.forward();
	int maxIndiceEthnicity = std::distance(ethnicityPreds.begin(), max_element(ethnicityPreds.begin(), ethnicityPreds.end()));
	auto ethnicity = m_ethnicityList[maxIndiceEthnicity];
	return ethnicity;
}

}