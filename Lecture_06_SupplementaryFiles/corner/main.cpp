#include  "main.h"

void main()
{
	Mat input = imread("./images/building.jpg", 0);

	cornerDetector(input);
}