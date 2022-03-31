

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

#define canny_edge_detection 1

#if canny_edge_detection

int main(int argc, char* argv) {


	Mat image = imread("test.bmp", 0);

	Mat result;

	int TH_low = 50;
	int TH_high = 150;

	Canny(image, result, TH_low, TH_high);

	imwrite("result.bmp", result);
	imshow("result", result);
	waitKey(0);


	return 0;
}
#endif

#if jpg2bmp

int main() {
	cv::Mat color_image = cv::imread("bear.jpg", 1); // imread는 이미지 불러오는 함수. 1을 인자로 가져가면 color로 읽기
	cv::Mat gray_image = cv::imread("bear.jpg", 0); // 0을 인자로 가져가면 gray로 읽기

	//이미지의 cols, rows, depth, type, channel 출력
	cout << "color cols : " << color_image.cols << endl;
	cout << "color rows : " << color_image.rows << endl;
	cout << "color depth : " << color_image.depth() << endl;
	cout << "color type : " << color_image.type() << endl;
	cout << "color channel : " << color_image.channels() << endl << endl;

	cout << "gray cols : " << gray_image.cols << endl;
	cout << "gray rows : " << gray_image.rows << endl;
	cout << "gray depth : " << gray_image.depth() << endl;
	cout << "gray type : " << gray_image.type() << endl;
	cout << "gray channel : " << gray_image.channels() << endl;

	//참고 : CV_8U (8비트양의정수) / CV_32S (32비트부호있는정수) / CV_64F(64비트부통소수점)

	//depth값 => 0:CV_8U / 1:CV_8S / 2:CV_16U / 3:CV_16S / 4:CV_32S / 5:CV_32F / 6:CV_64F / 7:CV_USERTYPE1



	cv::imshow("color", color_image); //이미지 출력
	cv::imshow("gray", gray_image);

	cv::Mat flip_image; //반전 이미지 저장 및 출력을 위해 변수 하나 선언
	cv::flip(color_image, flip_image, 0); //color_image를 flip시켜서 flip_image에 저장, 0은 y축으로 flip/1은 x축으로 flip/-1은 xy축으로 flip
	cv::imshow("flip", flip_image);



	vector<int> param_jpg, param_png, param_bmp;  //vector이라는 int형 변수 선언. 여러 요소를 추가삽입할 수 있음. 배열같은 것.
	param_jpg.push_back(cv::IMWRITE_JPEG_QUALITY);  //JPEG 파일로 저장하기 위해
	param_jpg.push_back(50);       //높은 값일수록 화질 좋음.용량 높음. 0~100. defalut 값은 95)
	param_png.push_back(cv::IMWRITE_PNG_COMPRESSION);  //PNG 파일로 저장하기 위해
	param_png.push_back(1);        //PNG 압축률 (0~9. default 값은 3). 여러 값 넣어본 결과 용량차이 없었음


	cv::imwrite("FlipBear.bmp", flip_image);
	cv::imwrite("FlipBear.jpg", flip_image, param_jpg);
	cv::imwrite("FlipBear.png", flip_image, param_png);

	cv::waitKey(0);
}

#endif

#if t1
int main()
{
	Mat imgColor = imread("test.jpg", CV_LOAD_IMAGE_COLOR);
	Mat imgGray = imread("test.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("color", imgColor);
	imshow("gray", imgGray);

	waitKey(30000);

	return 0;
}
#endif

