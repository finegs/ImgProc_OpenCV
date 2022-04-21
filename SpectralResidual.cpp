#include "main.hpp"

#define resImWidth		64
#define resImHeight		64

void siliencyDetection() {
	Mat input = imread("./images/input.jpg");
	float* SR_saliency = (float*)calloc(input.rows * input.cols, sizeof(float));

	computeSaliencyMap(input, SR_saliency);
	grabCut_Saliency(input, SR_saliency);

	free(SR_saliency);
}

void grabCut_Saliency(Mat input, float* saliencyMap) {
	int x, y;
	int height, width, th;
	float min = 1000000, max = -1000000;

	height = input.rows;
	width = input.cols;

	th = 100;

	Rect rectangle(0, 0, width - 1, height - 1);

	Mat mask = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;
	Mat salMask = Mat::zeros(height, width, CV_8UC1);
	Mat maskImg = Mat::zeros(height, width, CV_8UC3);

	Mat final_result = Mat::zeros(height, width, CV_8UC3);
	Mat mask_result = Mat::zeros(height, width, CV_8UC1);

	Mat bg, fg;

	final_result = input.clone();
	maskImg = input.clone();


	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (min > saliencyMap[y*width + x]) min = saliencyMap[y*width + x];
			if (max < saliencyMap[y*width + x]) max = saliencyMap[y*width + x];
		}
	}

	// foreground seed
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			salMask.at<uchar>(y, x) = 255 * (saliencyMap[y*width + x] - min) / (max - min);

			if (salMask.at<uchar>(y, x) > th) {
				mask.at<uchar>(y, x) = GC_FGD;

				maskImg.at<Vec3b>(y, x)[0] = 0;
				maskImg.at<Vec3b>(y, x)[1] = 0;
				maskImg.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}

	// bvackground seed
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (x < 20 || x > width - 20) {
				mask.at<uchar>(y, x) = GC_BGD;

				maskImg.at<Vec3b>(y, x)[0] = 255;
				maskImg.at<Vec3b>(y, x)[1] = 0;
				maskImg.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}

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

	imwrite("./images/mask.bmp", maskImg);
	imwrite("./images/saliencyMap.bmp", salMask);
	imwrite("./images/result_with_saliency.bmp", final_result);
}


void computeSaliencyMap(Mat image, float *SR_saliency)
{
	char filename[200];
	int x, y;
	float min = 1000000, max = -1000000;

	Mat input, result;
	Mat grayTemp, grayDown;
	std::vector<Mat> mv;
	Size resizedImageSize(resImWidth, resImHeight);

	Mat realImage(resizedImageSize, CV_64F);
	Mat imaginaryImage(resizedImageSize, CV_64F);
	imaginaryImage.setTo(0);
	Mat combinedImage(resizedImageSize, CV_64FC2);
	Mat imageDFT;
	Mat logAmplitude;
	Mat angle(resizedImageSize, CV_64F);
	Mat magnitude(resizedImageSize, CV_64F);
	Mat logAmplitude_blur, imageGR;

	if (image.channels() == 3)
	{
		cvtColor(image, imageGR, COLOR_BGR2GRAY);
		resize(imageGR, grayDown, resizedImageSize, 0, 0, INTER_LINEAR);
	}
	else
		resize(image, grayDown, resizedImageSize, 0, 0, INTER_LINEAR);

	grayDown.convertTo(realImage, CV_64F);

	mv.push_back(realImage);
	mv.push_back(imaginaryImage);
	merge(mv, combinedImage);
	dft(combinedImage, imageDFT);
	split(imageDFT, mv);

	//-- Get magnitude and phase of frequency spectrum --//
	cartToPolar(mv.at(0), mv.at(1), magnitude, angle, false);
	log(magnitude, logAmplitude);
	//-- Blur log amplitude with averaging filter --//
	blur(logAmplitude, logAmplitude_blur, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
	exp(logAmplitude - logAmplitude_blur, magnitude);
	//-- Back to cartesian frequency domain --//
	polarToCart(magnitude, angle, mv.at(0), mv.at(1), false);
	merge(mv, imageDFT);
	dft(imageDFT, combinedImage, DFT_INVERSE);
	split(combinedImage, mv);

	cartToPolar(mv.at(0), mv.at(1), magnitude, angle, false);
	GaussianBlur(magnitude, magnitude, Size(5, 5), 8, 0, BORDER_DEFAULT);
	magnitude = magnitude.mul(magnitude);

	double minVal, maxVal;
	minMaxLoc(magnitude, &minVal, &maxVal);

	magnitude = magnitude / maxVal;
	magnitude.convertTo(magnitude, CV_32F);

	Mat saliencyMap;

	resize(magnitude, saliencyMap, image.size(), 0, 0, INTER_LINEAR);

	for (y = 0; y < image.rows; y++) {
		for (x = 0; x < image.cols; x++) {
			SR_saliency[y*image.cols + x] = saliencyMap.at<float>(y, x);
		}
	}

	// image store
	//Mat SR_map(image.rows, image.cols, CV_8UC1);

	//for (y = 0; y < image.rows; y++) {
	//	for (x = 0; x < image.cols; x++) {
	//		if (min > saliencyMap.at<float>(y, x)) min = saliencyMap.at<float>(y, x);
	//		if (max < saliencyMap.at<float>(y, x)) max = saliencyMap.at<float>(y, x);
	//	}
	//}

	//for (y = 0; y < image.rows; y++) {
	//	for (x = 0; x < image.cols; x++) {
	//		SR_map.at<uchar>(y, x) = 255 * (saliencyMap.at<float>(y, x) - min) / (max - min);
	//	}
	//}

	//sprintf(filename, "./images/SR_map.bmp");
	//imwrite(filename, SR_map);
}