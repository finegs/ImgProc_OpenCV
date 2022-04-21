#include "main.hpp"
#include "SLIC.h"

void SLICSegmentation(Mat image);

void ImageSegmentation_SLIC() {

	

}

void SLICSegmentation(Mat image) {
	SLIC slic;
	int x, y;
	int height, width;
	int numLabels;
	int m_spcount = 1000;
	double m_compactness = 30;

	height = image.rows;
	width = image.cols;

	uint *ubuff = (uint*)calloc(height*width, sizeof(uint));
	int *labels = (int*)calloc(height*width, sizeof(int));


	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ubuff[y + width + x] = (int)image.at<Vec3b>(y, x)[0] + ((int)image.at<Vec3b>(y, x)[1] << 8) +
				((int)image.at<Vec3b>(y, x)[2] << 16);
		}
	}

	slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(ubuff, width, height, labels, numLabels, m_spcount, m_compactness);

	free(ubuff);
	free(labels);
}