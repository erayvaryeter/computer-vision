#pragma once

#include <object-detection/object-detection.h>
#include <map>
#include <memory>
#include <optional>

namespace base {
	class Logger;
}

namespace dl {

enum class EthnicityEstimatorType {
	ONNX_200x200 = 1
};

class EthnicityEstimator {
public:
	EthnicityEstimator(EthnicityEstimatorType type, const std::string& inputName, const std::string& outputName);
	~EthnicityEstimator() {}
	void InitializeNetworkPaths();
	std::string Estimate(const cv::Mat& face);

private:
	EthnicityEstimatorType m_ethnicityEstimatorType;
	cv::dnn::Net m_network;
	NetworkProperties m_networkProperties;
	std::map<EthnicityEstimatorType, NetworkProperties> m_networkPropertiesMap;
	std::vector<std::string> m_ethnicityList = { "Asian", "Black", "Indian", "Other", "White" };
	std::string m_inputName;
	std::string m_outputName;
	static std::shared_ptr<base::Logger> m_logger;
};

}