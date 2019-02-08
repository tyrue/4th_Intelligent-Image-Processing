#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// 산술연산을 위한 함수
void Arithmetic_Add(Mat src, Mat &dst, int value);
void main()
{
	Mat src = imread("barbara.JPEG", IMREAD_GRAYSCALE);
	Mat dst = src.clone(); // 영상처리 후 결과영상
	imshow("Original", src); //"Original"영상을 화면에 출력
	Arithmetic_Add(src, dst, 40); //원본영상+40
	imshow("Result", dst); //결과영상 화면에 출력
	waitKey(0); //키 입력을 기다린다.
	src.release(); //동적으로 할당된 메모리를 해제한다.
	dst.release(); //동적으로 할당된 메모리를 해제한다.
}

void Arithmetic_Add(Mat src, Mat &dst, int value)
{
	int width = src.cols; // 영상의 가로폭
	int height = src.rows; // 영상의 세로폭
	uchar *srcData = src.data; // 원본영상
	uchar *dstData = dst.data; // 결과영상(초기에는 원본영상을 그대로 복사)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int temp = srcData[(y*width) + x] + value;
			//각 화소값에 value 값을 더한다.
			//uchar 범위를 초과하는지 검사한다.
			if (temp > 255)
				dstData[(y*width) + x] = 255;
			else if (temp < 0)
				dstData[(y*width) + x] = 0;
			else 
				dstData[(y*width) + x] = temp;
		}
	}
}