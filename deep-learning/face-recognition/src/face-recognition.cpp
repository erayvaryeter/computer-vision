#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face-recognition/face-recognition.h"

std::shared_ptr<base::Logger> dl::FaceRecognizer::m_logger = std::make_shared<base::Logger>();

namespace dl {

FaceRecognizer::FaceRecognizer(FaceRecognizerType type, const std::shared_ptr<dl::FaceDetector>& faceDetector, const std::shared_ptr<dl::FaceWarper>& faceWarper) {
	m_recognizerType = type;
	m_faceDetector = faceDetector;
	m_faceWarper = faceWarper;
	LoadFaceDatabase();

	switch (m_recognizerType) {
	case FaceRecognizerType::EIGEN:
	{
		m_faceRecognizer = cv::face::EigenFaceRecognizer::create(320);
		break;
	}
	case FaceRecognizerType::FISHER:
	{
		m_faceRecognizer = cv::face::FisherFaceRecognizer::create(320);
		break;
	}
	case FaceRecognizerType::LBPH:
	{
		m_faceRecognizer = cv::face::LBPHFaceRecognizer::create();
		break;
	}
	default: break;
	}

	Train();
}

RecognitionResult
FaceRecognizer::Predict(cv::Mat warpedFaceImage, std::optional<std::string> expectedLabel) {
	ASSERT((warpedFaceImage.size().width == WARPED_FACE_WIDTH), "Input warped face image width is incorrect", base::Logger::Severity::Error);
	ASSERT((warpedFaceImage.size().height == WARPED_FACE_HEIGHT), "Input warped face image height is incorrect", base::Logger::Severity::Error);
	int predictedLabel = -1;
	double distance = 0.0;
	// static int x = 0;
	// cv::imwrite(std::to_string(x++) + ".jpg", warpedFaceImage);
	cv::Mat grayscale;
	cv::cvtColor(warpedFaceImage, grayscale, cv::COLOR_BGR2GRAY);
	m_faceRecognizer->predict(grayscale, predictedLabel, distance);
	RecognitionResult res;
	res.distance = distance;
	res.predictedLabel = m_faceLabelMap[predictedLabel];
	if (expectedLabel.has_value()) {
		res.expectedLabel = expectedLabel.value();
		if (res.predictedLabel == res.expectedLabel)
			res.recognitionCorrect = true;
		else
			res.recognitionCorrect = false;
	}
	return res;
}

void
FaceRecognizer::LoadFaceDatabase() {
	m_faceDatabase.clear();
	auto directories = base::File::GetDirectories("../../../../deep-learning/face-recognition/resource");
	std::vector<std::string> names;
	for (auto& dir : directories) {
		dir = base::String::ReplaceAll(dir, "\\", "/");
		auto splitted = base::String::SplitString(dir, "/");
		auto name = splitted[splitted.size() - 1];
		names.emplace_back(std::move(name));
	}

	for (size_t i = 0; i < directories.size(); ++i) {
		FaceDatabase newFaceDatabase;
		newFaceDatabase.label = i;
		auto files = base::File::GetFilesWithExtension(directories[i]);
		for (auto& filePath : files) {
			cv::Mat image = cv::imread(filePath.first.c_str());
			ASSERT(!image.empty(), "Error while loading the image", base::Logger::Severity::Error);
			auto detectionResults = m_faceDetector->Detect(image, dl::Object::FACE, true);
			auto warpResults = m_faceWarper->WarpWithLandmarks(detectionResults);
			for (auto warpResult : warpResults.warpingResults) {
				newFaceDatabase.imagePaths.emplace_back(std::move(base::String::ReplaceAll(filePath.first, "\\", "/")));
				newFaceDatabase.warpedImages.emplace_back(std::move(warpResult.warpedFaceImage));
			}
		}
		// make pair
		auto pair = std::make_pair(names[i], newFaceDatabase);
		m_faceDatabase.insert(m_faceDatabase.end(), pair);
	}
}

void
FaceRecognizer::Train() {
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	m_faceLabelMap.clear();
	for (auto& faceIt : m_faceDatabase) {
		std::string logMsg = "Training model for images of " + faceIt.first;
		m_logger->LogInfo(logMsg.c_str());
		for (auto warpedImage : faceIt.second.warpedImages) {
			cv::Mat grayscale;
			cv::cvtColor(warpedImage, grayscale, cv::COLOR_BGR2GRAY);
			images.emplace_back(std::move(grayscale));
			labels.emplace_back(faceIt.second.label);
		}
		auto pair = std::make_pair(faceIt.second.label, faceIt.first);
		m_faceLabelMap.insert(m_faceLabelMap.end(), pair);
	}
	m_faceRecognizer->train(images, labels);
}

}