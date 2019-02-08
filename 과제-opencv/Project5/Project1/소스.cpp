#include <opencv2/opencv.hpp>
#include <iostream>

#define CORNER_MAX 1024
#define PERIOD_MAX 100

using namespace std;
using namespace cv;

int main()
{
	VideoCapture cap(0); // 캠 객체
	if (!cap.isOpened()) // 캠을 사용할 수 없으면 종료
		return -1;

	Mat gray1, //현재 영상
		gray2; //이전 영상

	vector<Point2f> points1, points2; // 이전 특징점
	int cornerCount = 0; // 코너의 개수
	int cornerPeriod = 0; // 코너 유지 기간

	int rx = 100, ry = 100;
	while (1) 
	{
		Mat color;
		cap >> color; // 캠 영상을 저장
		cvtColor(color, gray1, CV_BGR2GRAY); // gray1에 캠 영상(현재 영상)을 저장

		vector<uchar> isFound;
		vector<float> error;
		Rect rect(rx, ry, 200, 200);

		if (!gray2.empty() && (cornerPeriod <= 0)) // 이전 영상이 있을 경우에만
		{
			//이전 영상에서 코너 특징점 검출
			goodFeaturesToTrack(gray2, points2, CORNER_MAX, 0.01, 6);

			cornerCount = points2.size(); // 코너 특징점의 개수
			cornerPeriod = PERIOD_MAX;
		}
		else
		{
			if (gray2.empty())	// 이전 영상이 없으면 할당 해제
			{
				gray2.release();
				gray2 = gray1.clone(); // 현재 영상을 이전 영상으로 저장함

				//이전 영상에서 코너 특징점 검출
				goodFeaturesToTrack(gray2, points2, CORNER_MAX, 0.01, 6);

				cornerCount = points2.size(); // 코너 특징점의 개수
				cornerPeriod = PERIOD_MAX;
			}

			//이전 프레임(gray2)의 옵티컬 플로우 추적 결과를 다음 옵티컬 플로우(gray1)의 시작점으로 활용
			//옵티컬 플로우 검출
			calcOpticalFlowPyrLK(gray2, gray1, points2, points1, isFound,
				error, Size(5, 5), 3, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3));

			cornerCount = points1.size(); // 코너 특징점의 개수
			//옵티컬 플로우가 부정확하게 계산된 코너점들 제거
			int nuCornerCount = 0;
			for (int i = 0; i < cornerCount; i++)
			{
				if (isFound[i] && error[i] < 500)
				{
					points1[nuCornerCount] = points1[i]; // 코너점 저장
					points2[nuCornerCount] = points2[i]; // 현재 영상에 대한 코너 점 저장
					nuCornerCount++;
				}
			} 
			cornerCount = nuCornerCount;

			int rectPointCount = 0; //사각형 내부의 포인트 개수
			double moveX = 0;//x축 방향 움직임
			double moveY = 0;//y축 방향 움직임

			//찾아낸 코너점들을 화면에 출력
			for (int i = 0; i < cornerCount; i++)
			{
				circle(color, points1[i], 2, Scalar(0, 0, 255));
				line(color, points1[i], points2[i], Scalar(255, 255, 0), 2);
				if ((rect.x <= points1[i].x && points1[i].x <= (rect.x + rect.width)) &&
					(rect.y <= points1[i].y && points1[i].y <= (rect.y + rect.height)))
				{
					moveX += points1[i].x - points2[i].x;
					moveY += points1[i].y - points2[i].y;
					rectPointCount++;
				}
			}

			//사각형을 moveX, moveY에 따라 움직인다.
			if (rectPointCount != 0)
			{
				rect.x += moveX / rectPointCount;
				if (rect.x < 0) rect.x = 0;
				else if (rect.x > 250) rect.x = 250;

				rect.y += moveY / rectPointCount;
				if (rect.y < 0) rect.y = 0;
				else if (rect.y > 250) rect.y = 250;
				rx = rect.x;
				ry = rect.y;
			}
			rectangle(color, rect, Scalar(0, 255, 0), 3);
		}
		cornerPeriod--;

		imshow("color", color); // 영상 출력

		if ('q' == waitKey(1))
			break;
		if (gray2.empty())	// 이전 영상이 없으면 할당 해제
			gray2.release();
		gray2 = gray1.clone(); // 현재 영상을 이전 영상으로 저장함
	}
	cap.release();
	return 0;
}