#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void main()
{
	Mat src = imread("lena.bmp", IMREAD_GRAYSCALE);
	Mat disp = Mat(src.size(), CV_32FC1, Scalar(1));
	cvtColor(src, disp, CV_GRAY2BGR);
	vector<Point2f> corners;
	int cornerCount = 1024;//코너의 최대 개수를 설정
	goodFeaturesToTrack(src, corners, cornerCount, 0.01, 6);
	for (int i = 0; i < cornerCount; i++) 
	{
		circle(disp, corners[i], 2, Scalar(0,0,255));
	}
	imshow("disp", disp);
	waitKey(0);
	src.release();
	disp.release();
}