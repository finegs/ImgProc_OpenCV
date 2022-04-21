
#include <stdio.h>
#include <stdlib.h>

#include "main.hpp"

using namespace cv;

int main() {



#if CORNER_DETECTOR
	Mat input = imread("./images/building.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	cornerDetector(input);

#endif 

#if OPTICAL_FLOW
	opticalFlow();
#endif 

	return 0;
}