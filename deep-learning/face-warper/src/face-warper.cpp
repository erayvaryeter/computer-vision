#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face-warper/face-warper.h"

std::shared_ptr<base::Logger> dl::FaceWarper::m_logger = std::make_shared<base::Logger>();

namespace dl {

void
FaceWarper::DrawPolyline(cv::Mat& im, const std::vector<cv::Point2f>& landmarks, const int start, const int end, bool isClosed) {
	std::vector<cv::Point> points;
	for (int i = start; i <= end; i++) {
		points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
	}
	// Draw polylines. 
	cv::polylines(im, points, isClosed, cv::Scalar(255, 200, 0), 2, 16);
}

void
FaceWarper::DrawLandmarks(cv::Mat& im, std::vector<cv::Point2f>& landmarks) {
	// Draw face for the 68-point model.
	if (landmarks.size() == 68) {
		DrawPolyline(im, landmarks, 0, 16);           // Jaw line
		DrawPolyline(im, landmarks, 17, 21);          // Left eyebrow
		DrawPolyline(im, landmarks, 22, 26);          // Right eyebrow
		DrawPolyline(im, landmarks, 27, 30);          // Nose bridge
		DrawPolyline(im, landmarks, 30, 35, true);    // Lower nose
		DrawPolyline(im, landmarks, 36, 41, true);    // Left eye
		DrawPolyline(im, landmarks, 42, 47, true);    // Right Eye
		DrawPolyline(im, landmarks, 48, 59, true);    // Outer lip
		DrawPolyline(im, landmarks, 60, 67, true);    // Inner lip
	}
	else {	// If the number of points is not 68, we do not know which 
			// points correspond to which facial features. So, we draw 
			// one dot per landamrk.
		for (int i = 0; i < landmarks.size(); i++) {
			circle(im, landmarks[i], 3, cv::Scalar(255, 200, 0), cv::FILLED);
		}
	}
}

WarpingResult
FaceWarper::WarpWithLandmarks(const DetectionResult& detectionResult) {
	WarpingResult retVal;
	auto fullImage = detectionResult.originalImage;
	fullImage.copyTo(retVal.imageWithLandmarks);
	std::vector<std::vector<cv::Point2f>> allLandmarks;
	std::vector<cv::Rect> faces;
	for (auto det : detectionResult.detections) {
		faces.emplace_back(std::move(det.bbox));
	}
	bool success = m_facemark->fit(fullImage, faces, allLandmarks);
	if (success) {
        for (int i = 0; i < allLandmarks.size(); i++) {
			DrawLandmarks(retVal.imageWithLandmarks, allLandmarks[i]);
		}
		for (auto landmarks : allLandmarks) {
			Warping res;
			res.landmarks = landmarks;
			// here warp the image and assign the images
			retVal.warpingResults.emplace_back(std::move(res));
		}
	}
	return retVal;
}

}