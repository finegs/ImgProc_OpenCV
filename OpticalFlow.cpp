#include "main.hpp"

#define Width 320
#define Height 240

using namespace std;

void opticalFlow(void) {

	//VideoCapture capture(0);
	VideoCapture capture("./images/box.mp4");
	if (capture.isOpened() == false) {
		printf("unable to open file\n");
		exit(0);
	}

	int x, y, key;
	Mat flow, frame, prevFrame, img;


	while (1) {

		capture >> frame;
		resize(frame, img, Size(Width, Height)); // Resize frame to Size
		cvtColor(img, frame, CV_BGR2GRAY);		 // Convert to GrayScale

		if (prevFrame.empty() == false) { // Skit First Frame

			calcOpticalFlowFarneback(prevFrame, frame, flow, 0.4, 1, 12, 2, 8, 1.2, 0);

			for(y = 0; y < Height; y += 5) {
				for (x = 0; x < Width; x += 5) {
					const Point2f flowatxy = flow.at<Point2f>(y, x) * 2;
					line(img, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(0, 255, 0));
					circle(img, Point(x, y), 1, Scalar(0, 0, 0), -1);
				}
			}

			frame.copyTo(prevFrame); // Copy to prevFrame
		}
		else {
			frame.copyTo(prevFrame);
		}
		imshow("Optical Flow", img);
		key = waitKey(20);
	}
}