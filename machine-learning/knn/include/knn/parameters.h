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