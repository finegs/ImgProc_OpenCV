#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Optical Flow
#include <opencv2/video/tracking.hpp>
#include <vector>
#include <Windows.h>


using namespace cv;

void cornerDetector(Mat input);
void opticalFlow();
void imageSegmentation_SLIC();