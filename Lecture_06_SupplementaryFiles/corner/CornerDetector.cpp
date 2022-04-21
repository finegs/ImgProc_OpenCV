#include "main.h"

void cornerDetector(Mat input)
{
	int x, y, xx, yy;
	int height, width;
    float conv_x, conv_y;
    float IxIx, IyIy, IxIy, det, tr;
    float k = 0.04, th = 200;
    float min = 1000000, max = -1000000;

    Scalar c;
    Point pCenter;
    int radius = 3;

	height = input.rows;
	width = input.cols;

    float* Ix = (float*)calloc(height * width, sizeof(float));
    float* Iy = (float*)calloc(height * width, sizeof(float));
    float* R = (float*)calloc(height * width, sizeof(float));
    int* cornerMap = (int*)calloc(height * width, sizeof(float));
    Mat output(height, width, CV_8UC3);
    Mat cornerness(height, width, CV_8UC1);

    int b_size = 3;
    int win_size = 3;
    const int size = 9;

    int mask_x[size] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
    int mask_y[size] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            conv_x = 0;
            conv_y = 0;
            for (yy = y - b_size / 2; yy <= y + b_size / 2; yy++) {
                for (xx = x - b_size / 2; xx <= x + b_size / 2; xx++) {
                    if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
                        conv_x += mask_x[(yy - (y - 1)) * b_size + (xx - (x - 1))] * input.at<uchar>(yy, xx)/255.0;
                        conv_y += mask_y[(yy - (y - 1)) * b_size + (xx - (x - 1))] * input.at<uchar>(yy, xx)/255.0;
                    }
                }
            }
            Ix[y * width + x] = conv_x;
            Iy[y * width + x] = conv_y;

            //
            output.at<Vec3b>(y, x)[0] = input.at<uchar>(y, x);
            output.at<Vec3b>(y, x)[1] = input.at<uchar>(y, x);
            output.at<Vec3b>(y, x)[2] = input.at<uchar>(y, x);
        }
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            IxIx = 0;
            IyIy = 0;
            IxIy = 0;

            for (yy = y - win_size; yy <= y + win_size; yy++) {
                for (xx = x - win_size; xx <= x + win_size; xx++) {
                    if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
                        IxIx += Ix[yy * width + xx] * Ix[yy * width + xx];
                        IyIy += Iy[yy * width + xx] * Iy[yy * width + xx];
                        IxIy += Ix[yy * width + xx] * Iy[yy * width + xx];
                    }
                }
            }
            det = IxIx * IyIy - IxIy * IxIy;
            tr = IxIx + IyIy;
            R[y * width + x] = det - k * tr * tr;

            if (R[y * width + x] > th) cornerMap[y * width + x] = 1;

            if (min > R[y * width + x]) min = R[y * width + x];
            if (max < R[y * width + x]) max = R[y * width + x];
        }
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (cornerMap[y * width + x] == 1) {
                pCenter.x = x;
                pCenter.y = y;
                c.val[0] = 0;
                c.val[1] = 255;
                c.val[2] = 0;
                circle(output, pCenter, radius, c, 2, 8, 0); // red circle
            }

            cornerness.at<uchar>(y, x) = 255-255 * (R[y * width + x] - min) / (max - min);
        }
    }

    imwrite("./images/cornerImage.bmp", output);
    imwrite("./images/cornerness.bmp", cornerness);

    free(Ix);
    free(Iy);
    free(R);
    free(cornerMap);
}