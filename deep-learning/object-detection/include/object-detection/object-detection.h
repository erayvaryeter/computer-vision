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
	INSTANCE_SEGMENTATION = 2,
	PERSON = 3,
	BICYCLE = 4,
	CAR = 5,
	MOTORCYCLE = 6,
	AIRPLANE = 7,
	BUS = 8,
	TRAIN = 9,
	TRUCK = 10,
	BOAT = 11,
	TRAFFIC_LIGHT = 12,
	FIRE_HYDRANT = 13,
	STOP_SIGN = 14,
	PARKING_METER = 15,
	BENCH = 16,
	BIRD = 17,
	CAT = 18,
	DOG = 19,
	HORSE = 20,
	SHEEP = 21,
	COW = 22,
	ELEPHANT = 23,
	BEAR = 24,
	ZEBRA = 25,
	GIRAFFE = 26,
	BACKPACK = 27,
	UMBRELLA = 28,
	HANDBAG = 29,
	TIE = 30,
	SUITCASE = 31,
	FRISBEE = 32,
	SNOWBOARD = 33,
	SPORTS_BALL = 34,
	KITE = 35,
	BASEBALL_BAT = 36,
	BASEBALL_GLOVE = 37,
	SKATEBOARD = 38,
	SURFBOARD = 39,
	TENNIS_RACKET = 40,
	BOTTLE = 41,
	WINE_GLASS = 42,
	CUP = 43,
	FORK = 44,
	KNIFE = 45,
	SPOON = 46,
	BOWL = 47,
	BANANA = 48,
	APPLE = 49,
	SANDWITCH = 50,
	ORANGE = 51,
	BROCCOLI = 52,
	CARROT = 53,
	HOTDOG = 54,
	PIZZA = 55,
	DONUT = 56,
	CAKE = 57,
	CHAIR = 58,
	COUCH = 59,
	POTTED_PLANT = 60,
	BED = 61,
	DINING_TABLE = 62,
	TOILET = 63,
	TV = 64,
	LAPTOP = 65,
	MOUSE = 66,
	REMOTE = 67,
	KEYBOARD = 68,
	CELL_PHONE = 69,
	MICROWAVE = 70,
	OVEN = 71,
	TOASTER = 72,
	SINK = 73,
	REFRIGERATOR = 74,
	BOOK = 75,
	CLOCK = 76,
	VASE = 77,
	SCISSORS = 78,
	TEDDY_BEAR = 79,
	HAIR_DRIER = 80,
	TOOTHBRUSH = 81,
	OTHER = 82
};

enum class NetworkType {
	CAFFE = 1,
	TENSORFLOW = 2
};

// structs for drawing instance segmentation
struct SegmentationDrawingElement {
	cv::Mat coloredRoi;
	cv::Rect bbox;
	cv::Mat mask;
};

struct Detection {
	cv::Rect bbox;
	float confidence;
	int classId;
	std::optional<Object> objectClass;
	std::optional<std::string> objectClassString;
	std::optional<std::string> ageEstimation;
	std::optional<std::string> genderEstimation;
	std::optional<SegmentationDrawingElement> drawingElement;
};

struct DetectionResult {
	std::vector<Detection> detections;
	cv::Mat originalImage;
	cv::Mat imageWithBbox;
	cv::Mat imageWithBboxAndMasks;
};

struct NetworkProperties {
	std::string configFilePath = "";
	std::string weightFilePath = "";
	std::optional<std::string> labelsPath = "";
	std::optional<std::string> colorsPath = "";
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
			m_network.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
			m_network.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
			break;
		}
		case NetworkType::TENSORFLOW:
		{
			m_network = cv::dnn::readNetFromTensorflow(m_weightFilePath, m_configFilePath);
			m_network.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
			m_network.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
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
	std::vector<std::string> GetOutputsNames(const cv::dnn::Net& net);

	std::string m_configFilePath = "";
	std::string m_weightFilePath = "";
	NetworkType m_networkType;
	cv::dnn::Net m_network;
	// Detection Parameters
	double m_scaleFactor = 0.0;
	cv::Scalar m_meanValues;
	float m_confidenceThreshold = 0.0f;
	std::string m_inputName = "";
	std::string m_outputDetectionName = "";
	std::string m_outputMaskName = "";
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

	virtual DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneObject = false) = 0;

protected:
	std::shared_ptr<Detector> m_detector;
	NetworkProperties m_networkProperties;

};

}