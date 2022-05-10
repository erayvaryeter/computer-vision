#pragma once

#include <face-detection/face-detection.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <optional>

namespace base {
	class Logger;
}

namespace video {

enum class TrackerType {
	BOOSTING = 1,
	CSRT = 2,
	GOTURN = 3,
	KCF = 4,
	MEDIANFLOW = 5,
	MIL = 6,
	MOSSE = 7,
	TLD = 8
};

struct TrackingResult {
	cv::Rect bbox;
	std::optional<std::string> objClass;
	std::optional<float> confidence;
};

class Tracker {
public:
	Tracker(int redetectSteps)
	: m_redetectSteps(redetectSteps) {
		m_multiTracker = cv::MultiTracker::create();
	}

	~Tracker() {}

	void AppendFaceDetector(std::shared_ptr<dl::FaceDetector> detector);
	std::vector<TrackingResult> ApplyDetectionOnSingleFrame(const cv::Mat& image);
	bool AppendTracker(std::vector<TrackerType> types, const cv::Mat& initialImage, std::vector<TrackingResult>& initialDetectionResults);
	std::vector<TrackingResult> PushFrame(cv::Mat& image);

private:
	std::vector<std::pair<dl::Object, std::shared_ptr<dl::BaseDetector>>> m_detectors;
	cv::Ptr<cv::MultiTracker> m_multiTracker;
	int m_redetectSteps;
	int m_lastNumberOfObjects = 0;
	std::vector<TrackingResult> m_lastTrackingResults;
	std::vector<TrackerType> m_trackerTypes;
	static std::shared_ptr<base::Logger> m_logger;
};

}