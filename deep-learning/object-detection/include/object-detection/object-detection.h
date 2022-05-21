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
	std::optional<cv::Mat> objectMask;
	std::optional<std::string> ageEstimation;
	std::optional<std::string> genderEstimation;
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

enum class DetectionFeature {
	CONFIDENCE = 1,
	BBOX_LEFT = 2,
	BBOX_TOP = 3,
	BBOX_RIGHT = 4,
	BBOX_BOTTOM = 5,
	CLASS_ID = 6,
};

struct DetectionParameters {
	double scaleFactor = 1.0;
	cv::Scalar meanValues = { 104.0, 177.0, 123.0 };
	float confidenceThreshold = 0.75f;
	std::string inputName = "data";
	std::string outputDetectionName = "detection_out";
	std::string outputMaskName;
	std::map<DetectionFeature, int> detectionFeatureMap = { 
		{ dl::DetectionFeature::CLASS_ID, 1 }, 
		{ dl::DetectionFeature::CONFIDENCE, 2 },
		{ dl::DetectionFeature::BBOX_LEFT, 3 },
		{ dl::DetectionFeature::BBOX_TOP, 4 },
		{ dl::DetectionFeature::BBOX_RIGHT, 5 },
		{ dl::DetectionFeature::BBOX_BOTTOM, 6 },
	};
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

	void SetDetectionParameters(DetectionParameters params) {
		m_scaleFactor = params.scaleFactor;
		m_meanValues = params.meanValues;
		m_confidenceThreshold = params.confidenceThreshold;
		m_inputName = params.inputName;
		m_outputDetectionName = params.outputDetectionName;
		m_outputMaskName = params.outputMaskName;
		m_detectionFeatureMap = params.detectionFeatureMap;
	}

	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork);

	static std::string ConvertObjectTypeToString(Object object);
	static Object ConvertObjectStringToType(const std::string& objectStr);

private:
	std::string m_configFilePath;
	std::string m_weightFilePath;
	NetworkType m_networkType;
	cv::dnn::Net m_network;
	// Detection Parameters
	double m_scaleFactor;
	cv::Scalar m_meanValues;
	float m_confidenceThreshold;
	std::string m_inputName;
	std::string m_outputDetectionName;
	std::string m_outputMaskName;
	std::map<DetectionFeature, int> m_detectionFeatureMap;
	// Logger
	static std::shared_ptr<base::Logger> m_logger;
};

class BaseDetector {
public:
	BaseDetector() {}
	~BaseDetector() {}

	virtual void InitializeNetworkPaths() = 0;

	void SetDetectionParameters(DetectionParameters params) {
		m_detector->SetDetectionParameters(params);
	}

	virtual DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneFace = false) = 0;

protected:
	std::shared_ptr<Detector> m_detector;
	NetworkProperties m_networkProperties;

};

}