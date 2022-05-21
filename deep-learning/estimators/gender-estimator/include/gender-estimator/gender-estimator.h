#pragma once

#include <object-detection/object-detection.h>
#include <map>
#include <memory>
#include <optional>

namespace base {
	class Logger;
}

namespace dl {

enum class GenderEstimatorType {
	CAFFE_227x227 = 1
};

class GenderEstimator {
public:
	GenderEstimator(GenderEstimatorType type);
	~GenderEstimator() {}
	void InitializeNetworkPaths();
	std::string Estimate(const cv::Mat& face);

private:
	GenderEstimatorType m_ageEstimatorType;
	cv::dnn::Net m_network;
	NetworkProperties m_networkProperties;
	std::map<GenderEstimatorType, NetworkProperties> m_networkPropertiesMap;
	std::vector<std::string> m_genderList = { "Male", "Female" };
	static std::shared_ptr<base::Logger> m_logger;
};

}