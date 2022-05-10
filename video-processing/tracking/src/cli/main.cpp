#include <tracking/tracking.h>
#include <cxxopts.hpp>
#include <file/file.h>
#include <assertion/assertion.h>

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("image", "Image path", cxxopts::value<std::string>()->default_value("../../../../deep-learning/face-detection/resource/1.jpg"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string imagePath;

	if (result.count("image")) {
		imagePath = result["image"].as<std::string>();
		if (!base::File::FileExists(imagePath)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(imagePath);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		imagePath = result["image"].as<std::string>();
	}

	cv::Mat image = cv::imread(imagePath.c_str());
	auto detector = std::make_shared<dl::FaceDetector>(dl::FaceDetectorType::CAFFE_300x300);
	detector->SetDetectionParameters(1.0, { 104.0, 177.0, 123.0 }, 0.70f);
	auto detectionResults = detector->Detect(image, dl::Object::FACE);

	auto tracker = std::make_shared<video::Tracker>(7);
	tracker->AppendFaceDetector(detector);
	
	cv::VideoCapture cap(1);
	if (!cap.isOpened()) {
		std::cout << "Error opening webcam" << std::endl;
		return -1;
	}

	bool first = true;

	while (true) {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		cv::Mat drawImage;
		frame.copyTo(drawImage);

		if (first) {
			auto detectionResults = tracker->ApplyDetectionOnSingleFrame(frame);
			if (detectionResults.empty()) {
				std::cout << "Waiting for first frame with a face object" << std::endl;
			}
			else {
				std::vector<video::TrackerType> types;
				for (auto det : detectionResults) {
					types.push_back(video::TrackerType::KCF);
				}
				if(tracker->AppendTracker(types, frame, detectionResults))
					first = false;
			}
		}
		else {
			auto detections = tracker->PushFrame(frame);
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

	return 0;
}