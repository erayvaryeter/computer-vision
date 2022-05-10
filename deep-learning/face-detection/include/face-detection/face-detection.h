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

class FaceDetector : public BaseDetector {
public:
	FaceDetector(FaceDetectorType type);
	~FaceDetector() {}
	void InitializeNetworkPaths() override;
	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneFace = false) override;

private:
	FaceDetectorType m_faceDetectorType;
	std::map<FaceDetectorType, NetworkProperties> m_networkPropertiesMap;
	static std::shared_ptr<base::Logger> m_logger;
};

}