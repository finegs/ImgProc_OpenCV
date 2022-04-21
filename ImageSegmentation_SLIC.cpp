#include "main.hpp"
#include "SLIC.h"

void SLICSegmentation(Mat image);

void imageSegmentation_SLIC() {

	Mat input = imread("./images/building.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	SLICSegmentation(input);
	
}

void SLICSegmentation(Mat image) {
	SLIC slic;
	int x, y;
	int height, width;
	int numLabels; // Generated number of superpixels
	int m_spcount = 1000;	// Desired number of superpixels
	double m_compactness = 30;	// 20.0 // compatness factor  (1~40)

	height = image.rows;
	width = image.cols;

	uint *ubuff = (uint*)calloc(height*width, sizeof(uint));
	int *labels = (int*)calloc(height*width, sizeof(int));


	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ubuff[y * width + x] = (int)image.at<Vec3b>(y, x)[0] 
				+ ((int)image.at<Vec3b>(y, x)[1] << 8) 
				+ ((int)image.at<Vec3b>(y, x)[2] << 16);
		}
	}

	slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(ubuff, width, height, labels, 
		numLabels, m_spcount, m_compactness);

	// for drawing SLIC superpixels
	Mat result(height, width, CV_8UC3);

	slic.DrawContoursAroundSegments(ubuff, labels, width, height, 0);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			result.at<Vec3b>(y, x)[0] = ubuff[y*width + x] & 0xff;
			result.at<Vec3b>(y, x)[1] = ubuff[y*width + x] >> 8  & 0xff;
			result.at<Vec3b>(y, x)[2] = ubuff[y*width + x] >> 16 & 0xff;
		}
	}

	imwrite("./images/SLC_segmentation.bmp", result);

	free(ubuff);
	free(labels);
}