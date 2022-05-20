#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "object-detection/object-detection.h"

std::shared_ptr<base::Logger> dl::Detector::m_logger = std::make_shared<base::Logger>();

namespace dl {

DetectionResult
Detector::Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork) {
    DetectionResult retVal;
    frame.copyTo(retVal.imageWithBbox);
    frame.copyTo(retVal.originalImage);

    int imageWidth, imageHeight;
    imageWidth = frame.size().width;
    imageHeight = frame.size().height;

    cv::Mat inputBlob;
    if (m_networkType == NetworkType::CAFFE)
        inputBlob = cv::dnn::blobFromImage(frame, m_scaleFactor, cv::Size(imageWidth, imageHeight), m_meanValues, false, false);
    else if (m_networkType == NetworkType::TENSORFLOW)
        inputBlob = cv::dnn::blobFromImage(frame, m_scaleFactor, cv::Size(imageWidth, imageHeight), m_meanValues, true, false);

    m_network.setInput(inputBlob, m_inputName);
    cv::Mat detection = m_network.forward(m_outputDetectionName);
    cv::Mat detection_matrix(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    for (int i = 0; i < detection_matrix.rows; i++) {
        float confidence = detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::CONFIDENCE]);
        if (confidence < m_confidenceThreshold) {
            continue;
        }
        int classId = static_cast<int>(detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::CLASS_ID]));
        int left = static_cast<int>(detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::BBOX_LEFT]) * frame.cols);
        int top = static_cast<int>(detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::BBOX_TOP]) * frame.rows);
        int right = static_cast<int>(detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::BBOX_RIGHT]) * frame.cols);
        int bottom = static_cast<int>(detection_matrix.at<float>(i, m_detectionFeatureMap[DetectionFeature::BBOX_BOTTOM]) * frame.rows);
        Detection res;
        res.bbox = cv::Rect(left, top, (right - left), (bottom - top));
        res.confidence = confidence;
        cv::Point textPoint = cv::Point(res.bbox.x, res.bbox.y + 15);
        cv::putText(retVal.imageWithBbox, std::to_string(res.confidence), textPoint, 1, 1, cv::Scalar(255, 0, 0));
        if (oneClassNetwork.has_value()) {
            res.objectClass = oneClassNetwork.value();
            res.objectClassString = ConvertObjectTypeToString(oneClassNetwork.value());
        }
        retVal.detections.emplace_back(std::move(res));
    }

    for (auto& det : retVal.detections) {
        cv::rectangle(retVal.imageWithBbox, det.bbox, cv::Scalar(0, 255, 0), 1, 8, 0);
        if (det.objectClassString.has_value()) {
            cv::Point textPoint = cv::Point(det.bbox.x, det.bbox.y);
            cv::putText(retVal.imageWithBbox, det.objectClassString.value(), textPoint, 1, 1, cv::Scalar(255, 0, 0));
        }
    }

    return retVal;
}

std::string 
Detector::ConvertObjectTypeToString(Object object) {
	switch (object) {
	case Object::FACE: return "Face";
	default: return "";
	}
}

Object
Detector::ConvertObjectStringToType(const std::string& objectStr) {
    if (objectStr == "Face")
        return Object::FACE;
    else
        return Object::OTHER;
}

}