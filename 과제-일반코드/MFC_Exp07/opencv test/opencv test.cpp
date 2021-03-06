// opencv test.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;
int main()
{
	//이미지를 src로 불러온다.
	Mat src = imread("lena.bmp", IMREAD_GRAYSCALE);
	//“test”라는 영상창에 src 영상을 출력한다.
	imshow("test", src);
	//키 입력을 기다린다.
	waitKey(0);
	//동적으로 할당된 메모리를 해제한다.
	src.release();
}
