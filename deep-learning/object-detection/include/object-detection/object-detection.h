#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <optional>

namespace base {
	class Logger;
}

namespace dl {

enum class Object {
	FACE = 1,
	OTHER = 2
};

enum class NetworkType {
	CAFFE = 1,
	TENSORFLOW = 2
};

struct Detection {
	cv::Rect bbox;
	float confidence;
	std::optional<Object> objectClass;
	std::optional<std::string> objectClassString;
};

struct DetectionResult {
	std::vector<Detection> detections;
	cv::Mat originalImage;
	cv::Mat imageWithBbox;
};

struct NetworkProperties {
	std::string configFilePath = "";
	std::string weightFilePath = "";
	int imageInputWidth = 0;
	int imageInputHeight = 0;
	NetworkType networkType;
};

class Detector {
public:
	Detector(NetworkType type, const std::string& configFilePath, const std::string& weightFilePath)
		: m_networkType(type), m_configFilePath(configFilePath), m_weightFilePath(weightFilePath) 
	{
		switch (m_networkType) {
		case NetworkType::CAFFE: 
		{
			m_network = cv::dnn::readNetFromCaffe(m_configFilePath, m_weightFilePath);
			break;
		}
		case NetworkType::TENSORFLOW:
		{
			m_network = cv::dnn::readNetFromTensorflow(m_weightFilePath, m_configFilePath);
			break;
		}
		default: 
		{
			break;
		}
		}
	}

	~Detector() {}

	void SetDetectionParameters(double scaleFactor = 1.0, cv::Scalar meanValues = { 104., 177.0, 123.0 }, float confidenceThreshold = 0.75f) {
		m_scaleFactor = scaleFactor;
		m_meanValues = meanValues;
		m_confidenceThreshold = confidenceThreshold;
	}

	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork);

	static std::string ConvertObjectTypeToString(Object object);
	static Object ConvertObjectStringToType(const std::string& objectStr);

private:
	std::string m_configFilePath;
	std::string m_weightFilePath;
	NetworkType m_networkType;
	cv::dnn::Net m_network;
	double m_scaleFactor;
	cv::Scalar m_meanValues;
	float m_confidenceThreshold;
	static std::shared_ptr<base::Logger> m_logger;
};

class BaseDetector {
public:
	BaseDetector() {}
	~BaseDetector() {}

	virtual void InitializeNetworkPaths() = 0;

	void SetDetectionParameters(double scaleFactor = 1.0, cv::Scalar meanValues = { 104., 177.0, 123.0 }, float confidenceThreshold = 0.75f) {
		m_detector->SetDetectionParameters(scaleFactor, meanValues, confidenceThreshold);
	}

	virtual DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneFace = false) = 0;

protected:
	std::shared_ptr<Detector> m_detector;
	NetworkProperties m_networkProperties;

};

}