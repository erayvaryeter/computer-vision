#pragma once

#include <homography-calculator/homography-calculator.h>
#include <opencv2/opencv.hpp>

namespace base {
	class Logger;
}

namespace homography {

class Stitcher {
public:
	Stitcher(cv::Stitcher::Mode m) { m_stitcher = cv::Stitcher::create(m); }
	~Stitcher() {}

	cv::Mat GetStitchedImage() { return m_stitchedImage; }

	void ClearInputImages();
	void AppendInputImage(cv::Mat image);
	bool StitchImages();

private:
	cv::Ptr<cv::Stitcher> m_stitcher;
	std::vector<cv::Mat> m_inputImages;
	cv::Mat m_stitchedImage;
	static std::shared_ptr<base::Logger> m_logger;
};

}