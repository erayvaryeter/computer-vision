#pragma once

#include <face-detection/face-detection.h>
#include <opencv2/face.hpp>
#include <map>
#include <memory>

#define WARPED_FACE_WIDTH 160
#define WARPED_FACE_HEIGHT 240

namespace base {
	class Logger;
}

namespace dl {

struct Warping {
	std::vector<cv::Point2f> originalLandmarks;
	std::vector<cv::Point2f> warpedLandmarks;
	cv::Mat warpedFaceImage;
	cv::Mat warpedFaceImageWithLandmarks;
};

struct WarpingResult {
	std::vector<Warping> warpingResults;
	cv::Mat imageWithLandmarks;
};

class FaceWarper {
public:
	FaceWarper() { 
		m_facemark = cv::face::FacemarkLBF::create(); 
		m_facemark->loadModel("../../../../deep-learning/face-warper/resource/lbfmodel.yaml");
	}
	~FaceWarper() {}

	void DrawPolyline(cv::Mat& im, const std::vector<cv::Point2f>& landmarks, const int start, const int end, bool isClosed = false);
	void DrawLandmarks(cv::Mat& im, std::vector<cv::Point2f>& landmarks);
	WarpingResult WarpWithLandmarks(const DetectionResult& detectionResult);

private:
	cv::Ptr<cv::face::Facemark> m_facemark;
	static std::shared_ptr<base::Logger> m_logger;
};

}