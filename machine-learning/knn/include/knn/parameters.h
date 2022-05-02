#pragma once

struct HarrisCornerParams {
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
};

struct ShiTomasiCornerParams {
	int maxCorners = 25;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	int gradientSize = 3;
	bool useHarris = false;
	double k = 0.04;
};

struct SiftParams {
	int nFeatures = 0;
	int nOctaveLayers = 3;
	double contrastThreshold = 0.04;
	double edgeThreshold = 10.0;
	double sigma = 1.60;
};

struct SurfParams {
	double hessianThreshold = 100;
	int nOctaves = 4;
	int nOctaveLayers = 3;
	bool extended = false;
	bool upright = false;
};

struct FastParams {
	int threshold = 10;
	bool nonmaxSupression = true;
};

struct BriefParams {
	int bytes = 32;
	bool useOrientation = false;
};

struct OrbParams {
	int nFeatures = 500;
	float scaleFactor = 1.2f;
	int nLevels = 8;
	int edgeThreshold = 31;
	int firstLevel = 0;
	int WTA_K = 2;
	cv::ORB::ScoreType st = cv::ORB::ScoreType::HARRIS_SCORE;
	int patchSize = 31;
	int fastThreshold = 20;
};