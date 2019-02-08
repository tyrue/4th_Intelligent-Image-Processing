#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
// ��������� ���� �Լ�
void Arithmetic_Add(Mat src, Mat &dst, int value);
void main()
{
	Mat src = imread("barbara.JPEG", IMREAD_GRAYSCALE);
	Mat dst = src.clone(); // ����ó�� �� �������
	imshow("Original", src); //"Original"������ ȭ�鿡 ���
	Arithmetic_Add(src, dst, 40); //��������+40
	imshow("Result", dst); //������� ȭ�鿡 ���
	waitKey(0); //Ű �Է��� ��ٸ���.
	src.release(); //�������� �Ҵ�� �޸𸮸� �����Ѵ�.
	dst.release(); //�������� �Ҵ�� �޸𸮸� �����Ѵ�.
}

void Arithmetic_Add(Mat src, Mat &dst, int value)
{
	int width = src.cols; // ������ ������
	int height = src.rows; // ������ ������
	uchar *srcData = src.data; // ��������
	uchar *dstData = dst.data; // �������(�ʱ⿡�� ���������� �״�� ����)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int temp = srcData[(y*width) + x] + value;
			//�� ȭ�Ұ��� value ���� ���Ѵ�.
			//uchar ������ �ʰ��ϴ��� �˻��Ѵ�.
			if (temp > 255)
				dstData[(y*width) + x] = 255;
			else if (temp < 0)
				dstData[(y*width) + x] = 0;
			else 
				dstData[(y*width) + x] = temp;
		}
	}
}