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

    if (m_inputName.empty())
        m_network.setInput(inputBlob);
    else
        m_network.setInput(inputBlob, m_inputName);

    std::vector<std::string> outNames;
    if (m_outputMaskName.empty())
        outNames = { m_outputDetectionName };
    else
        outNames = { m_outputDetectionName, m_outputMaskName };

    std::vector<cv::Mat> outs;
    m_network.forward(outs, outNames);

    cv::Mat detection = outs[0];

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
        res.classId = classId;
        cv::Point textPoint = cv::Point(res.bbox.x, res.bbox.y + 15);
        cv::putText(retVal.imageWithBbox, std::to_string(res.confidence), textPoint, 1, 1, cv::Scalar(255, 0, 0));
        if (oneClassNetwork.has_value()) {
            res.objectClass = oneClassNetwork.value();
            res.objectClassString = ConvertObjectTypeToString(oneClassNetwork.value());
        }
        // segmentation result
        if (!m_outputMaskName.empty()) {
            cv::Mat outMasks = outs[1];
            cv::Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));
			SegmentationDrawingElement e;
			e.mask = objectMask;
			e.bbox = res.bbox;
			res.drawingElement = e;
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

std::vector<std::string>
Detector::GetOutputsNames(const cv::dnn::Net& net) {
    static std::vector<std::string> names;
    if (names.empty()) {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<std::string> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

std::string 
Detector::ConvertObjectTypeToString(Object object) {
	switch (object) {
	case Object::FACE: return "face";
	case Object::PERSON: return "person";
	case Object::BICYCLE: return "bicycle";
	case Object::CAR: return "car";
	case Object::MOTORCYCLE: return "motorcycle";
	case Object::AIRPLANE: return "airplane";
	case Object::BUS: return "bus";
	case Object::TRAIN: return "train";
	case Object::TRUCK: return "truck";
	case Object::BOAT: return "boat";
	case Object::TRAFFIC_LIGHT: return "traffic light";
	case Object::FIRE_HYDRANT: return "fire hydrant";
	case Object::STOP_SIGN: return "stop sign";
	case Object::PARKING_METER: return "parking meter";
	case Object::BENCH: return "bench";
	case Object::BIRD: return "bird";
	case Object::CAT: return "cat";
	case Object::DOG: return "dog";
	case Object::HORSE: return "horse";
	case Object::SHEEP: return "sheep";
	case Object::COW: return "cow";
	case Object::ELEPHANT: return "elephant";
	case Object::BEAR: return "bear";
	case Object::ZEBRA: return "zebra";
	case Object::GIRAFFE: return "giraffe";
	case Object::BACKPACK: return "backpack";
	case Object::UMBRELLA: return "umbrella";
	case Object::HANDBAG: return "handbag";
	case Object::TIE: return "tie";
	case Object::SUITCASE: return "suitcase";
	case Object::FRISBEE: return "frisbee";
	case Object::SNOWBOARD: return "snowboard";
	case Object::SPORTS_BALL: return "sports ball";
	case Object::KITE: return "kite";
	case Object::BASEBALL_BAT: return "baseball bat";
	case Object::BASEBALL_GLOVE: return "baseball glove";
	case Object::SKATEBOARD: return "skateboard";
	case Object::SURFBOARD: return "surfboard";
	case Object::TENNIS_RACKET: return "tennis racket";
	case Object::BOTTLE: return "bottle";
	case Object::WINE_GLASS: return "wine glass";
	case Object::CUP: return "cup";
	case Object::FORK: return "fork";
	case Object::KNIFE: return "knife";
	case Object::SPOON: return "spoon";
	case Object::BOWL: return "bowl";
	case Object::BANANA: return "banana";
	case Object::APPLE: return "apple";
	case Object::SANDWITCH: return "sandwitch";
	case Object::ORANGE: return "orange";
	case Object::BROCCOLI: return "brocoli";
	case Object::CARROT: return "carrot";
	case Object::HOTDOG: return "hot dog";
	case Object::PIZZA: return "pizza";
	case Object::DONUT: return "donut";
	case Object::CAKE: return "cake";
	case Object::CHAIR: return "chair";
	case Object::COUCH: return "couch";
	case Object::POTTED_PLANT: return "potted plant";
	case Object::BED: return "bed";
	case Object::DINING_TABLE: return "dining table";
	case Object::TOILET: return "toilet";
	case Object::TV: return "tv";
	case Object::LAPTOP: return "laptop";
	case Object::MOUSE: return "mouse";
	case Object::REMOTE: return "remote";
	case Object::KEYBOARD: return "keyboard";
	case Object::CELL_PHONE: return "cell phone";
	case Object::MICROWAVE: return "microwave";
	case Object::OVEN: return "oven";
	case Object::TOASTER: return "toaster";
	case Object::SINK: return "sink";
	case Object::REFRIGERATOR: return "refrigerator";
	case Object::BOOK: return "book";
	case Object::CLOCK: return "clock";
	case Object::VASE: return "vase";
	case Object::SCISSORS: return "scissors";
	case Object::TEDDY_BEAR: return "teddy bear";
	case Object::HAIR_DRIER: return "hair driver";
	case Object::TOOTHBRUSH: return "toothbrush";
	case Object::OTHER: return "other";
	default: return "";
	}
}

Object
Detector::ConvertObjectStringToType(const std::string& objectStr) {
	if ("face") return Object::FACE;
	else if ("person") return Object::PERSON;
	else if ("bicycle") return Object::BICYCLE;
	else if ("car") return Object::CAR;
	else if ("motorcycle") return Object::MOTORCYCLE;
	else if ("airplane") return Object::AIRPLANE;
	else if ("bus") return Object::BUS;
	else if ("train") return Object::TRAIN;
	else if ("truck") return Object::TRUCK;
	else if ("boat") return Object::BOAT;
	else if ("traffic light") return Object::TRAFFIC_LIGHT;
	else if ("fire hydrant") return Object::FIRE_HYDRANT;
	else if ("stop sign") return Object::STOP_SIGN;
	else if ("parking meter") return Object::PARKING_METER;
	else if ("bench") return Object::BENCH;
	else if ("bird") return Object::BIRD;
	else if ("cat") return Object::CAT;
	else if ("dog") return Object::DOG;
	else if ("horse") return Object::HORSE;
	else if ("sheep") return Object::SHEEP;
	else if ("cow") return Object::COW;
	else if ("elephant") return Object::ELEPHANT;
	else if ("bear") return Object::BEAR;
	else if ("zebra") return Object::ZEBRA;
	else if ("giraffe") return Object::GIRAFFE;
	else if ("backpack") return Object::BACKPACK;
	else if ("umbrella") return Object::UMBRELLA;
	else if ("handbag") return Object::HANDBAG;
	else if ("tie") return Object::TIE;
	else if ("suitcase") return Object::SUITCASE;
	else if ("frisbee") return Object::FRISBEE;
	else if ("snowboard") return Object::SNOWBOARD;
	else if ("sports ball") return Object::SPORTS_BALL;
	else if ("kite") return Object::KITE;
	else if ("baseball bat") return Object::BASEBALL_BAT;
	else if ("baseball glove") return Object::BASEBALL_GLOVE;
	else if ("skateboard") return Object::SKATEBOARD;
	else if ("surfboard") return Object::SURFBOARD;
	else if ("tennis racket") return Object::TENNIS_RACKET;
	else if ("bottle") return Object::BOTTLE;
	else if ("wine glass") return Object::WINE_GLASS;
	else if ("cup") return Object::CUP;
	else if ("fork") return Object::FORK;
	else if ("knife") return Object::KNIFE;
	else if ("spoon") return Object::SPOON;
	else if ("bowl") return Object::BOWL;
	else if ("banana") return Object::BANANA;
	else if ("apple") return Object::APPLE;
	else if ("sandwitch") return Object::SANDWITCH;
	else if ("orange") return Object::ORANGE;
	else if ("brocoli") return Object::BROCCOLI;
	else if ("carrot") return Object::CARROT;
	else if ("hot dog") return Object::HOTDOG;
	else if ("pizza") return Object::PIZZA;
	else if ("donut") return Object::DONUT;
	else if ("cake") return Object::CAKE;
	else if ("chair") return Object::CHAIR;
	else if ("couch") return Object::COUCH;
	else if ("potted plant") return Object::POTTED_PLANT;
	else if ("bed") return Object::BED;
	else if ("dining table") return Object::DINING_TABLE;
	else if ("toilet") return Object::TOILET;
	else if ("tv") return Object::TV;
	else if ("laptop") return Object::LAPTOP;
	else if ("mouse") return Object::MOUSE;
	else if ("remote") return Object::REMOTE;
	else if ("keyboard") return Object::KEYBOARD;
	else if ("cell phone") return Object::CELL_PHONE;
	else if ("microwave") return Object::MICROWAVE;
	else if ("oven") return Object::OVEN;
	else if ("toaster") return Object::TOASTER;
	else if ("sink") return Object::SINK;
	else if ("refrigerator") return Object::REFRIGERATOR;
	else if ("book") return Object::BOOK;
	else if ("clock") return Object::CLOCK;
	else if ("vase") return Object::VASE;
	else if ("scissors") return Object::SCISSORS;
	else if ("teddy bear") return Object::TEDDY_BEAR;
	else if ("hair driver") return Object::HAIR_DRIER;
	else if ("toothbrush") return Object::TOOTHBRUSH;
	else if ("other") return Object::OTHER;
	else return Object::OTHER;
}

}