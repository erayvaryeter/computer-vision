#pragma once

#include <object-detection/object-detection.h>
#include <map>
#include <memory>

namespace base {
	class Logger;
}

namespace dl {

enum class FaceDetectorType {
	CAFFE_300x300 = 1,
	TENSORFLOW_300x300 = 2
};

class FaceDetector {
public:
	FaceDetector(FaceDetectorType type);
	~FaceDetector() {}
	void InitializeNetworkPaths();

	void SetDetectionParameters(double scaleFactor = 1.0, cv::Scalar meanValues = { 104., 177.0, 123.0 }, float confidenceThreshold = 0.75f) {
		m_detector->SetDetectionParameters(scaleFactor, meanValues, confidenceThreshold);
	}

	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork);

private:
	std::shared_ptr<Detector> m_detector;
	FaceDetectorType m_faceDetectorType;
	NetworkProperties m_networkProperties;
	std::map<FaceDetectorType, NetworkProperties> m_networkPropertiesMap;
	static std::shared_ptr<base::Logger> m_logger;
};

}