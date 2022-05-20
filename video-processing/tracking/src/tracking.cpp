#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>


#include "tracking/tracking.h"

std::shared_ptr<base::Logger> video::Tracker::m_logger = std::make_shared<base::Logger>();

namespace video {

void
Tracker::AppendFaceDetector(std::shared_ptr<dl::FaceDetector> detector) {
    auto pair = std::make_pair(dl::Object::FACE, detector);
    m_detectors.emplace_back(std::move(pair));
}

std::vector<TrackingResult>
Tracker::ApplyDetectionOnSingleFrame(const cv::Mat& image) {
    std::vector<TrackingResult> results;
    std::vector<dl::DetectionResult> detectionResults;
    for (const auto& detector : m_detectors) {
        switch (detector.first) {
        case dl::Object::FACE:
        {
            auto result = static_pointer_cast<dl::FaceDetector>(detector.second)->Detect(image, dl::Object::FACE);
            detectionResults.emplace_back(std::move(result));
            break;
        }
        default: break;
        }
    }
    for (const auto& dets : detectionResults) {
        for (const auto& det : dets.detections) {
            TrackingResult res;
            res.bbox = det.bbox;
            res.confidence = det.confidence;
            if (det.objectClassString.has_value())
                res.objClass = det.objectClassString;
            results.emplace_back(std::move(res));
        }
    }
    m_lastNumberOfObjects = results.size();
    m_lastTrackingResults = results;
    return results;
}

bool
Tracker::AppendTracker(std::vector<TrackerType> types, const cv::Mat& initialImage, std::vector<TrackingResult>& initialDetectionResults) {
    std::vector<cv::Rect> ROIs;
    if (!initialDetectionResults.empty()) {
        ASSERT(types.size() == initialDetectionResults.size(), "Size of vectors of tracker types and initial detection results must be equal",
            base::Logger::Severity::Error);
        for (auto result : initialDetectionResults) {
            ROIs.push_back(result.bbox);
        }
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
    m_trackerTypes.clear();
    for (size_t i = 0; i < ROIs.size(); ++i) {
        algorithms.push_back(CreateTrackerByName(types[i]));
        objects.push_back(ROIs[i]);
        m_trackerTypes.push_back(types[i]);
    }

    if (!ROIs.empty()) {
        m_multiTracker->add(algorithms, initialImage, objects);
        return true;
    }
    else {
        return false;
    }
}

std::vector<TrackingResult>
Tracker::PushFrame(cv::Mat& image) {
    std::vector<TrackingResult> retVal;
    bool ok = m_multiTracker->update(image);
    if (ok) {
        auto objects = m_multiTracker->getObjects();
        if (objects.size() == m_lastNumberOfObjects && objects.size() == m_lastTrackingResults.size()) {
            for (size_t i = 0; i < objects.size(); ++i) {
                TrackingResult res;
                res.bbox = objects[i];
                if (m_lastTrackingResults[i].confidence.has_value())
                    res.confidence = m_lastTrackingResults[i].confidence.value();
                if (m_lastTrackingResults[i].objClass.has_value())
                    res.objClass = m_lastTrackingResults[i].objClass.value();
                retVal.emplace_back(std::move(res));
            }
        }
        else {
            for (auto object : objects) {
                TrackingResult res;
                res.bbox = object;
                retVal.emplace_back(std::move(res));
            }
            m_lastNumberOfObjects = objects.size();
        }
    }
    else {
        m_logger->LogCritical("Multi tracker update failed, triggering the neural network for initial detection ...");
        auto detections = ApplyDetectionOnSingleFrame(image);
        if (!detections.empty()) {
            m_multiTracker->clear();
            m_multiTracker = cv::MultiTracker::create();
            if (AppendTracker(m_trackerTypes, image, detections))
                return PushFrame(image);
        }
    }
    return retVal;
}

void
Tracker::Run(cv::VideoCapture& cap) {
    bool first = true;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
            break;
        cv::Mat drawImage;
        frame.copyTo(drawImage);

        if (first) {
            auto detectionResults = ApplyDetectionOnSingleFrame(frame);
            if (detectionResults.empty()) {
                std::cout << "Waiting for first frame with a face object" << std::endl;
            }
            else {
                std::vector<video::TrackerType> types;
                for (auto det : detectionResults) {
                    types.push_back(video::TrackerType::KCF);
                }
                if (AppendTracker(types, frame, detectionResults))
                    first = false;
            }
        }
        else {
            auto detections = PushFrame(frame);
            for (auto& det : detections) {
                cv::rectangle(drawImage, det.bbox, cv::Scalar(255, 0, 0));
                if (det.objClass.has_value())
                    cv::putText(drawImage, det.objClass.value(), cv::Point(det.bbox.x, det.bbox.y), 1, 1, cv::Scalar(0, 255, 0));
                if (det.confidence.has_value())
                    cv::putText(drawImage, std::to_string(det.confidence.value()), cv::Point(det.bbox.x, det.bbox.y + 10), 1, 1, cv::Scalar(0, 255, 0));
            }
        }

        cv::imshow("Webcam with Face Detections", drawImage);

        char c = (char)cv::waitKey(25);
        if (c == 27)
            break;
    }

    cap.release();
    cv::destroyAllWindows();
}

}