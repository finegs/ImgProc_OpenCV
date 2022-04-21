
#include <stdio.h>
#include <stdlib.h>

#include "main.hpp"

using namespace cv;

#define CORNER_DETECTOR 0
#define OPTICAL_FLOW 0
#define IMG_SEGMENTATION_SLIC 0
#define IMG_SEGMENTATION_GRABCUT 0
#define IMG_SEGMENTATION_GRABCUT_MASK 0 // grabcut error !!!
#define IMG_SILIENCYDETECTION 0

#if 0
#if defined(CORNER_DETECTOR) \
|| defined(OPTICAL_FLOW) \
|| defined(IMG_SEGMENTATION_SLIC) \
|| defined(IMG_SEGMENTATION_GRABCUT) \
|| defined(IMG_SEGMENTATION_GRABCUT) \
|| defined(IMG_SEGMENTATION_GRABCUT_MASK) \
|| defined(IMG_SILIENCYDETECTION)


int main() {


#if CORNER_DETECTOR
	Mat input = imread("./images/building.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	cornerDetector(input);

#endif 

#if OPTICAL_FLOW
	opticalFlow();
#endif 

#if IMG_SEGMENTATION_SLIC
	imageSegmentation_SLIC();
#endif 

#if IMG_SEGMENTATION_GRABCUT
	ImageSegmentation_GrabCut();
#endif

#if IMG_SEGMENTATION_GRABCUT_MASK
	ImageSegmentation_GrabCut_mask();
#endif

#if IMG_SILIENCYDETECTION
	siliencyDetection();
#endif
	return 0;
}

#endif

#endif