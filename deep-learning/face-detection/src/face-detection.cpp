#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face-detection/face-detection.h"

std::shared_ptr<base::Logger> dl::FaceDetector::m_logger = std::make_shared<base::Logger>();

namespace dl {

FaceDetector::FaceDetector(FaceDetectorType type) {
	m_faceDetectorType = type;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_faceDetectorType];
	m_detector = std::make_shared<Detector>(m_networkProperties.networkType, m_networkProperties.configFilePath, m_networkProperties.weightFilePath);
}

void
FaceDetector::InitializeNetworkPaths() {
	// CAFFE_300x300
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/face-detection/resource/face/caffe/300x300/deploy.prototxt";
	prop1.weightFilePath = "../../../../deep-learning/face-detection/resource/face/caffe/300x300/res10_300x300_ssd_iter_140000_fp16.caffemodel";
	prop1.imageInputWidth = 300;
	prop1.imageInputHeight = 300;
	prop1.networkType = NetworkType::CAFFE;
	auto pair1 = std::make_pair(FaceDetectorType::CAFFE_300x300, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
	// TENSORFLOW_300x300
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
FaceDetector::Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork) {
	auto frameWidth = frame.size().width;
	auto frameHeight = frame.size().height;
	auto ratioWidth = static_cast<double>(frameWidth) / static_cast<double>(m_networkProperties.imageInputWidth);
	auto ratioHeight = static_cast<double>(frameHeight) / static_cast<double>(m_networkProperties.imageInputHeight);
	auto ratio = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;
	bool resizeRequired = false;
	if (ratio > 1.2)
		resizeRequired = true;
	DetectionResult retVal;
	if (resizeRequired) {
		// resize input frame
		cv::Mat resizedFrame;
		cv::resize(frame, resizedFrame, cv::Size(static_cast<int>(frameWidth / ratio), static_cast<int>(frameHeight / ratio)));
		retVal = m_detector->Detect(resizedFrame, oneClassNetwork);
		// adjust the resized detection result
		frame.copyTo(retVal.imageWithBbox);
		frame.copyTo(retVal.originalImage);
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
		}
	}
	else {
		retVal = m_detector->Detect(frame, oneClassNetwork);
	}
	
	return retVal;
}

}