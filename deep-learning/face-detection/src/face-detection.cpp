#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face-detection/face-detection.h"

std::shared_ptr<base::Logger> dl::FaceDetector::m_logger = std::make_shared<base::Logger>();

namespace dl {

FaceDetector::FaceDetector(FaceDetectorType type, std::optional<AgeEstimatorType> ageEstimatorType, 
	std::optional<GenderEstimatorType> genderEstimatorType) {
	m_faceDetectorType = type;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_faceDetectorType];
	m_detector = std::make_shared<Detector>(m_networkProperties.networkType, m_networkProperties.configFilePath, m_networkProperties.weightFilePath);
	if (ageEstimatorType.has_value())
		m_ageEstimator = std::make_shared<AgeEstimator>(ageEstimatorType.value());
	if (genderEstimatorType.has_value())
		m_genderEstimator = std::make_shared<GenderEstimator>(genderEstimatorType.value());
}

void
FaceDetector::InitializeNetworkPaths() {
	// CAFFE_300x300 FACE DETECTOR
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/face-detection/resource/face/caffe/300x300/deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/face-detection/resource/face/caffe/300x300/res10_300x300_ssd_iter_140000_fp16.caffemodel";
	prop1.imageInputWidth = 300;
	prop1.imageInputHeight = 300;
	prop1.networkType = NetworkType::CAFFE;
	auto pair1 = std::make_pair(FaceDetectorType::CAFFE_300x300, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
	// TENSORFLOW_300x300 FACE DETECTOR
	NetworkProperties prop2;
	prop2.configFilePath = "../../../../deep-learning/face-detection/resource/face/tensorflow/300x300/opencv_face_detector.pbtxt";
	prop2.weightFilePath = "../../../../deep-learning/face-detection/resource/face/tensorflow/300x300/opencv_face_detector_uint8.pb";
	prop2.imageInputWidth = 300;
	prop2.imageInputHeight = 300;
	prop2.networkType = NetworkType::TENSORFLOW;
	auto pair2 = std::make_pair(FaceDetectorType::TENSORFLOW_300x300, prop2);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair2);
}

DetectionResult
FaceDetector::Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneFace) {
	auto frameWidth = frame.size().width;
	auto frameHeight = frame.size().height;
	auto ratioWidth = static_cast<double>(frameWidth) / static_cast<double>(m_networkProperties.imageInputWidth);
	auto ratioHeight = static_cast<double>(frameHeight) / static_cast<double>(m_networkProperties.imageInputHeight);
	auto ratio = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;
	DetectionResult retVal;

	auto CorrectBoundingBoxes = [&]() {
		std::vector<dl::Detection> detections;
		for (size_t i = 0; i < retVal.detections.size(); ++i) {
			if (retVal.detections[i].bbox.x * ratio >= frameWidth ||
				retVal.detections[i].bbox.y * ratio >= frameHeight ||
				retVal.detections[i].bbox.width * ratio > frameWidth ||
				retVal.detections[i].bbox.height * ratio > frameHeight ||
				(retVal.detections[i].bbox.x + retVal.detections[i].bbox.width) * ratio > frameWidth ||
				(retVal.detections[i].bbox.y + retVal.detections[i].bbox.height) * ratio > frameHeight) {
				;
			}
			else {
				detections.push_back(retVal.detections[i]);
			}
		}
		retVal.detections.clear();
		if (oneFace) {
			double maxDistance = static_cast<double>(frameWidth);
			dl::Detection oneFaceDetection;
			for (size_t i = 0; i < detections.size(); ++i) {
				double distance;
				auto det = detections[i];
				auto dist1 = (((2 * det.bbox.x) + det.bbox.width) / 2) - (frameWidth / 2);
				auto dist2 = (((2 * det.bbox.y) + det.bbox.height) / 2) - (frameHeight / 2);
				distance = std::sqrt(std::pow(dist1, 2) + std::pow(dist2, 2));
				if (distance < maxDistance) {
					retVal.detections.clear();
					oneFaceDetection = detections[i];
					maxDistance = distance;
					retVal.detections.emplace_back(std::move(oneFaceDetection));
				}
			}
		}
		else {
			retVal.detections = detections;
		}
	};

	// resize input frame
	cv::Mat resizedFrame;
	cv::resize(frame, resizedFrame, cv::Size(static_cast<int>(frameWidth / ratio), static_cast<int>(frameHeight / ratio)));
	retVal = m_detector->Detect(resizedFrame, oneClassNetwork);
	// adjust the resized detection result
	frame.copyTo(retVal.imageWithBbox);
	frame.copyTo(retVal.originalImage);
	// remove the wrongly detected out of region bbox
	CorrectBoundingBoxes();
	// iterate over detections
	for (auto& det : retVal.detections) {
		det.bbox.x *= ratio;
		det.bbox.y *= ratio;
		det.bbox.width *= ratio;
		det.bbox.height *= ratio;
		cv::rectangle(retVal.imageWithBbox, det.bbox, cv::Scalar(0, 255, 0), 1, 8, 0);
		if (det.objectClassString.has_value()) {
			cv::Point textPoint = cv::Point(det.bbox.x, det.bbox.y);
			cv::putText(retVal.imageWithBbox, det.objectClassString.value(), textPoint, 1, 1, cv::Scalar(255, 0, 0));
		}
		cv::Point textPoint = cv::Point(det.bbox.x, det.bbox.y + 15);
		cv::putText(retVal.imageWithBbox, std::to_string(det.confidence), textPoint, 1, 1, cv::Scalar(255, 0, 0));
		// Age Estimation
		if (m_ageEstimator.has_value()) {
			auto ageResult = m_ageEstimator.value()->Estimate(retVal.originalImage(det.bbox));
			det.ageEstimation = ageResult;
			textPoint = cv::Point(det.bbox.x, det.bbox.y + 30);
			cv::putText(retVal.imageWithBbox, "Age: " + ageResult, textPoint, 1, 1, cv::Scalar(255, 0, 0));
		}
		// Gender Estimation
		if (m_genderEstimator.has_value()) {
			auto genderResult = m_genderEstimator.value()->Estimate(retVal.originalImage(det.bbox));
			det.genderEstimation = genderResult;
			textPoint = cv::Point(det.bbox.x, det.bbox.y + 45);
			cv::putText(retVal.imageWithBbox, "Gender: " + genderResult, textPoint, 1, 1, cv::Scalar(255, 0, 0));
		}
	}

	return retVal;
}

}