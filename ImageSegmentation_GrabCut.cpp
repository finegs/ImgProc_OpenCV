#include "main.hpp"

void ImageSegmentation_GrabCut() {
	int x, y;
	int height, width;

	   
	Mat image = imread("./images/car_3.bmp");

	Rect rectangle(10, 100, 380, 280);

	height = image.rows;
	width = image.cols;

	Mat result;
	Mat bg, fg;

	grabCut(image, result, rectangle, bg, fg, 5, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);

	Mat foreground(image.size(), CV_8UC3, Scalar(255, 255, 255));
	image.copyTo(foreground, result);

	imshow("Foreground", foreground);
	waitKey(0);
}

void ImageSegmentation_GrabCut_mask() {
	int x, y;
	int height, width;

	Mat input = imread("./images/car.jpg");
	Mat input_with_mask = imread("./images/car_with_mask.bmp");

	height = input.rows;
	width = input.cols;

	Rect rectangle(0, 0, width - 1, height - 1);
	Mat mask = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;

	// Mask generation j: red -> foreground, blue -> background
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (input_with_mask.at<Vec3b>(y, x)[0] == 255
				&& input_with_mask.at<Vec3b>(y, x)[1] == 0
				&& input_with_mask.at<Vec3b>(y, x)[2] == 0) {
				mask.at<uchar>(y, x) = GC_BGD;
			}
			else if (input_with_mask.at<Vec3b>(y, x)[0] == 0
				&& input_with_mask.at<Vec3b>(y, x)[1] == 0
				&& input_with_mask.at<Vec3b>(y, x)[2] == 255) {
				mask.at<uchar>(y, x) = GC_FGD;
			}
		}
	}

	Mat final_result = Mat::zeros(height, width, CV_8UC3);
	Mat mask_result = Mat::zeros(height, width, CV_8UC1);

	Mat bg, fg;

	final_result = input.clone();

	// TODO : Error 
	grabCut(input, mask, rectangle, bg, fg, 8, GC_INIT_WITH_MASK);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (mask.at<uchar>(y, x) == GC_FGD 
				|| mask.at<uchar>(y, x) == GC_PR_FGD) {
				mask_result.at<uchar>(y, x) = 255;
			}
			else {
				// For Visualization
				final_result.at<Vec3b>(y, x)[0] = 255;
				final_result.at<Vec3b>(y, x)[1] = 255;
				final_result.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}

	imwrite("./images/result.bmp", final_result);
}