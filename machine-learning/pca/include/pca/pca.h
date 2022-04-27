#pragma once

#include <feature-matcher/feature-matcher.h>
#include <opencv2/opencv.hpp>

namespace base {
	class Logger;
}

namespace ml {

class PCA {
public:
	PCA() {}
	~PCA() {}

	cv::Mat GetTransformationMatrix() { return m_transformationMatrix; }
	std::vector<cv::Mat> GetPrincipalComponents();

	void AppendImage(cv::Mat image);
	void ClearImages();
	cv::Mat Normalize(cv::Mat& src);
	cv::Mat FormatImagesForPCA(const std::vector<cv::Mat>& data);
	void ApplyPCA(int numComponents);

private:
	std::vector<cv::Mat> m_inputImages;
	std::vector<cv::Mat> m_grayscaleImages;
	cv::Mat m_dataMatrix;
	cv::Mat m_transformationMatrix;
	static std::shared_ptr<base::Logger> m_logger;
};

}