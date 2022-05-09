#pragma once

#include <face-detection/face-detection.h>
#include <face-warper/face-warper.h>
#include <opencv2/face/facerec.hpp>
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace base {
	class Logger;
}

namespace dl {

enum class FaceRecognizerType {
	EIGEN = 1,
	FISHER = 2,
	LBPH = 3
};

struct FaceDatabase {
	std::vector<std::string> imagePaths;
	std::vector<cv::Mat> warpedImages;
	int label;
};

struct RecognitionResult {
	std::string predictedLabel;
	double distance;
	std::optional<std::string> expectedLabel;
	std::optional<bool> recognitionCorrect;
};

class FaceRecognizer {
public:
	FaceRecognizer(FaceRecognizerType type, const std::shared_ptr<dl::FaceDetector>& faceDetector, const std::shared_ptr<dl::FaceWarper>& faceWarper);
	~FaceRecognizer() {}

	RecognitionResult Predict(cv::Mat warpedFaceImage, std::optional<std::string> expectedLabel);

private:
	void LoadFaceDatabase();
	void Train();

	FaceRecognizerType m_recognizerType;
	std::map<std::string, FaceDatabase> m_faceDatabase;
	std::map<int, std::string> m_faceLabelMap;
	std::shared_ptr<dl::FaceDetector> m_faceDetector;
	std::shared_ptr<dl::FaceWarper> m_faceWarper;
	cv::Ptr<cv::face::FaceRecognizer> m_faceRecognizer;
	static std::shared_ptr<base::Logger> m_logger;
};

}