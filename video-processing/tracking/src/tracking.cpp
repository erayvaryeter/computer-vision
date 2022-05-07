#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>


#include "tracking/tracking.h"

std::shared_ptr<base::Logger> video::Tracker::m_logger = std::make_shared<base::Logger>();

namespace video {

void
Tracker::AppendDetector(std::shared_ptr<dl::Detector> detector) {
    m_detectors.emplace_back(std::move(detector));
}

std::vector<TrackingResult>
Tracker::ApplyDetectionOnSingleFrame(const cv::Mat& image) {
    std::vector<TrackingResult> results;
    for (auto& detector : m_detectors) {
        // apply detections here
    }
    return results;
}

void
Tracker::AppendTracker(std::vector<TrackerType> types, const cv::Mat& initialImage, std::vector<TrackingResult>& initialDetectionResults) {
    std::vector<cv::Rect> ROIs;
    if (!initialDetectionResults.empty()) {
        ASSERT(types.size() == initialDetectionResults.size(), "Size of vectors of tracker types and initial detection results must be equal",
            base::Logger::Severity::Error);
        for (auto result : initialDetectionResults) {
            ROIs.push_back(result.bbox);
        }
    }
    else {
        cv::selectROIs("tracker", initialImage, ROIs);
        ASSERT(types.size() == ROIs.size(), "Size of vectors of tracker types and ROIs must be equal",
            base::Logger::Severity::Error);
    }

    auto CreateTrackerByName = [](TrackerType type) -> cv::Ptr<cv::Tracker> {
        switch (type) {
        case TrackerType::BOOSTING:
        {
            return cv::TrackerBoosting::create();
        }
        case TrackerType::CSRT:
        {
            return cv::TrackerCSRT::create();
        }
        case TrackerType::GOTURN:
        {
            return cv::TrackerGOTURN::create();
        }
        case TrackerType::KCF:
        {
            return cv::TrackerKCF::create();
        }
        case TrackerType::MEDIANFLOW:
        {
            return cv::TrackerMedianFlow::create();
        }
        case TrackerType::MIL:
        {
            return cv::TrackerMIL::create();
        }
        case TrackerType::MOSSE:
        {
            return cv::TrackerMOSSE::create();
        }
        case TrackerType::TLD:
        {
            return cv::TrackerTLD::create();
        }
        default:
        {
            return nullptr;
        }
        }
    };

    // initialize the tracker
    std::vector<cv::Rect2d> objects;
    std::vector<cv::Ptr<cv::Tracker>> algorithms;
    for (size_t i = 0; i < ROIs.size(); ++i) {
        algorithms.push_back(CreateTrackerByName(types[i]));
        objects.push_back(ROIs[i]);
    }

    m_multiTracker->add(algorithms, initialImage, objects);
}

std::optional<TrackingResult>
Tracker::PushFrame(cv::Mat& image) {
    static int counter = 0;
    std::vector<cv::Rect2d> bbox;
    bool ok = m_multiTracker->update(image, bbox);
    if (ok) {
        
    }
    return std::nullopt;
}

}