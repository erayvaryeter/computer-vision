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

enum class FaceDetectorType {
	CAFFE_300x300 = 1,
	TENSORFLOW_300x300 = 2,
	CAFFE_227x227_AGE = 3,
	CAFFE_227x227_GENDER = 4
};

class FaceDetector : public BaseDetector {
public:
	FaceDetector(FaceDetectorType type, std::optional<AgeEstimatorType> ageEstimatorType = std::nullopt, 
		std::optional<GenderEstimatorType> genderEstimatorType = std::nullopt);
	~FaceDetector() {}
	void InitializeNetworkPaths() override;
	DetectionResult Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneFace = false) override;

private:
	FaceDetectorType m_faceDetectorType;
	std::map<FaceDetectorType, NetworkProperties> m_networkPropertiesMap;
	std::optional<std::shared_ptr<AgeEstimator>> m_ageEstimator;
	std::optional<std::shared_ptr<GenderEstimator>> m_genderEstimator;
	static std::shared_ptr<base::Logger> m_logger;
};

}