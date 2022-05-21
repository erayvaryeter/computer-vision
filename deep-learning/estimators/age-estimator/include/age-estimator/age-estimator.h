#pragma once

#include <object-detection/object-detection.h>
#include <map>
#include <memory>
#include <optional>

namespace base {
	class Logger;
}

namespace dl {

enum class AgeEstimatorType {
	CAFFE_227x227 = 1
};

class AgeEstimator {
public:
	AgeEstimator(AgeEstimatorType type);
	~AgeEstimator() {}
	void InitializeNetworkPaths();
	std::string Estimate(const cv::Mat& face);

private:
	AgeEstimatorType m_ageEstimatorType;
	cv::dnn::Net m_network;
	NetworkProperties m_networkProperties;
	std::map<AgeEstimatorType, NetworkProperties> m_networkPropertiesMap;
	std::vector<std::string> m_ageList = { "(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)", "(38-43)", "(48-53)", "(60-100)" };
	static std::shared_ptr<base::Logger> m_logger;
};

}