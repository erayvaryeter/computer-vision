#include <object-detection/object-detection.h>
#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>

#include "instance-segmentation/instance-segmentation.h"

std::shared_ptr<base::Logger> dl::InstanceSegmentator::m_logger = std::make_shared<base::Logger>();

namespace dl {

InstanceSegmentator::InstanceSegmentator(InstanceSegmentationType type) {
	m_segmentatorType = type;
	InitializeNetworkPaths();
	m_networkProperties = m_networkPropertiesMap[m_segmentatorType];
	// read classes
	auto classesPath = m_networkProperties.labelsPath;
	std::ifstream ifs(classesPath.value().c_str());
	std::string line;
	while (std::getline(ifs, line)) m_classes.push_back(line);
	// read colors
	auto colorsPath = m_networkProperties.colorsPath;
	std::ifstream colorFptr(colorsPath.value().c_str());
	while (std::getline(colorFptr, line)) {
		char* pEnd;
		double r, g, b;
		r = strtod(line.c_str(), &pEnd);
		g = strtod(pEnd, NULL);
		b = strtod(pEnd, NULL);
		m_colors.push_back(cv::Scalar(r, g, b, 255.0));
	}
	// initialize detector
	m_detector = std::make_shared<Detector>(m_networkProperties.networkType, m_networkProperties.configFilePath, m_networkProperties.weightFilePath);
}

void
InstanceSegmentator::InitializeNetworkPaths() {
	// TENSORFLOW_MASK_RCNN INSTANCE SEGMENTATION
	NetworkProperties prop1;
	prop1.configFilePath = "../../../../deep-learning/instance-segmentation/resource/tensorflow/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
	prop1.weightFilePath = "../../../../deep-learning/instance-segmentation/resource/tensorflow/frozen_inference_graph.pb";
	prop1.labelsPath = "../../../../deep-learning/instance-segmentation/resource/tensorflow/mscoco_labels.names";
	prop1.colorsPath = "../../../../deep-learning/instance-segmentation/resource/tensorflow/colors.txt";
	prop1.imageInputWidth = 300;
	prop1.imageInputHeight = 300;
	prop1.networkType = NetworkType::TENSORFLOW;
	auto pair1 = std::make_pair(InstanceSegmentationType::TENSORFLOW_MASK_RCNN, prop1);
	m_networkPropertiesMap.insert(m_networkPropertiesMap.end(), pair1);
}

DetectionResult
InstanceSegmentator::Detect(const cv::Mat& frame, std::optional<Object> oneClassNetwork, bool oneObject) {
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
		if (oneObject) {
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
	std::vector<SegmentationDrawingElement> drawingElements;
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
		else {
			auto objectClass = m_classes[det.classId];
			det.objectClassString = objectClass;
			det.objectClass = Detector::ConvertObjectStringToType(objectClass);
			cv::Point textPoint = cv::Point(det.bbox.x, det.bbox.y);
			cv::putText(retVal.imageWithBbox, objectClass, textPoint, 1, 1, cv::Scalar(255, 0, 0));
		}
		cv::Point textPoint = cv::Point(det.bbox.x, det.bbox.y + 15);
		cv::putText(retVal.imageWithBbox, std::to_string(det.confidence), textPoint, 1, 1, cv::Scalar(255, 0, 0));
		// segmentation masks
		if (det.drawingElement.has_value()) {
			auto color = m_colors[det.classId % m_colors.size()];
			auto& objectMask = det.drawingElement.value().mask;
			// Resize the mask, threshold, color and apply it on the image
			cv::resize(objectMask, objectMask, cv::Size(det.bbox.width, det.bbox.height));
			cv::Mat mask = (objectMask > det.confidence);
			cv::Mat coloredRoi = (0.3 * color + 0.7 * retVal.imageWithBbox(det.bbox));
			coloredRoi.convertTo(coloredRoi, CV_8UC3);
			// Draw the contours on the image
			std::vector<cv::Mat> contours;
			cv::Mat hierarchy;
			mask.convertTo(mask, CV_8U);
			findContours(mask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
			drawContours(coloredRoi, contours, -1, color, 5, cv::LINE_8, hierarchy, 100);
			SegmentationDrawingElement e = { coloredRoi, det.bbox, mask };
			det.drawingElement = e;
			drawingElements.push_back(e);
		}
	}

	if (!drawingElements.empty()) {
		retVal.imageWithBbox.copyTo(retVal.imageWithBboxAndMasks);
		for (auto& e : drawingElements) {
			e.coloredRoi.copyTo(retVal.imageWithBboxAndMasks(e.bbox), e.mask);
		}
	}

	return retVal;
}

}