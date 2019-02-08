#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void ExtractByHsv(Mat hsv, Mat bin);
void HoughTransform(Mat mbin, Mat dst);
void main()
{
	Mat source = imread("00001.jpg");		// 원본 영상
	Mat hsv = Mat(source.size(), CV_8UC3);
	
	cvtColor(source, hsv, CV_BGR2HSV);
	Mat channel[3];
	split(hsv, channel);
	HoughTransform(channel[0], source);

	imshow("source", source);
	waitKey(0);
}

void ExtractByHsv(Mat hsv, Mat bin)
{
	const int width = hsv.cols;		// 영상의 가로 크기
	const int height = hsv.rows;	// 영상의 세로 크기
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{		
			Vec3b val = hsv.at<Vec3b>(y, x);
			if ((90 < val[0] && val[0] < 140)		// 색상
				&& (90 < val[1] && val[1] < 360)	// 채도
				&& (90 < val[2] && val[2] < 360)	// 명도
				)
			{
				bin.at<unsigned char>(y, x) = 255;
			}
			else
			{
				bin.at<unsigned char>(y, x) = 0;
			}
		}
	}
}

void HoughTransform(Mat mbin, Mat dst)
{
	
	IplImage* bin = cvCloneImage(&(IplImage)mbin);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	lines = cvHoughLines2(bin, storage, CV_HOUGH_STANDARD, 1, CV_PI / 180, 10);
	
	for (int i = 0; i < MIN(lines->total, 10); i++)
	{
		float* line = (float*)cvGetSeqElem(lines, i);
		//그리기 위한 임시라인
		float rho = line[0];
		float theta = line[1];
		//극좌표를 직교좌표로 바꿔준다.
		CvPoint pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;//직선이 지나는
		//무한히 긴 라인을 그리기 위한 작업
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		cv::line(dst, pt1, pt2, Scalar(0, 0, 255));
	}
	cvReleaseMemStorage(&storage);
}