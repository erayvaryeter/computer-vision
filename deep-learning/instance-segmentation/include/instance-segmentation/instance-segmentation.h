#pragma once

#include <object-detection/object-detection.h>
#include <age-estimator/age-estimator.h>
#include <gender-estimator/gender-estimator.h>
#include <map>
#include <memory>
#include <optional>

namespace base {
	class Logger;
}

namespace dl {

enum class InstanceSegmentationType {
	TENSORFLOW_MASK_RCNN = 1
};

class InstanceSegmentator : public BaseDetector {
public:
	InstanceSegmentator(InstanceSegmentationType type);
	~InstanceSegmentator() {}
	void InitializeNetworkPaths() override;
	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneObject = false) override;

private:
	std::vector<cv::Scalar> m_colors;
	std::vector<std::string> m_classes;
	InstanceSegmentationType m_segmentatorType;
	std::map<InstanceSegmentationType, NetworkProperties> m_networkPropertiesMap;
	static std::shared_ptr<base::Logger> m_logger;
};

}