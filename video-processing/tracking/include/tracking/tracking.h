#pragma once

#include <face-detection/face-detection.h>
#include <object-detection/object-detection.h>
#include <instance-segmentation/instance-segmentation.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <opencv2/core/ocl.hpp>
#include <optional>

namespace base {
	class Logger;
}

namespace video {

enum class TrackerType {
	BOOSTING = 1,
	CSRT = 2,
	KCF = 3,
	MEDIANFLOW = 4,
	MIL = 5,
	MOSSE = 6,
	TLD = 7
};

struct TrackingResult {
	cv::Rect bbox;
	std::optional<std::string> objClass;
	std::optional<float> confidence;
	std::optional<std::string> ageEstimation;
	std::optional<std::string> genderEstimation;
	std::optional<std::string> ethnicityEstimation;
	std::optional<dl::SegmentationDrawingElement> drawingElement;
};

class Tracker {
public:
	Tracker(int redetectSteps)
	: m_redetectSteps(redetectSteps) {
		m_multiTracker = cv::legacy::MultiTracker::create();
	}

	~Tracker() {}

	void AppendFaceDetector(std::shared_ptr<dl::FaceDetector> detector);
	void AppendInstanceSegmentator(std::shared_ptr<dl::InstanceSegmentator> segmentator);
	std::vector<TrackingResult> ApplyDetectionOnSingleFrame(const cv::Mat& image);
	bool AppendTracker(std::vector<TrackerType> types, const cv::Mat& initialImage, std::vector<TrackingResult>& initialDetectionResults);
	std::vector<TrackingResult> PushFrame(cv::Mat& image);
	void Run(cv::VideoCapture& cap);

private:
	std::vector<std::pair<dl::Object, std::shared_ptr<dl::BaseDetector>>> m_detectors;
	cv::Ptr<cv::legacy::MultiTracker> m_multiTracker;
	int m_redetectSteps;
	int m_lastNumberOfObjects = 0;
	bool m_segmentationDrawing = false;
	std::vector<TrackingResult> m_lastTrackingResults;
	std::vector<TrackerType> m_trackerTypes;
	static std::shared_ptr<base::Logger> m_logger;
};

}