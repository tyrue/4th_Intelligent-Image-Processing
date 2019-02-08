#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void main() 
{
	//샘플 데이터를 생성한다.
	//32
	float trainData[40] = 
	{ 
		5, 10, 34, 29, 99, 63, 82, 36, 165, 140, 169, 84,
		190, 118, 198, 153, 30, 163, 19, 58, 98, 89, 138, 
		153, 54, 140, 76, 50, 81, 170, 210, 201,
		150, 100, 200, 200, 225, 225, 250, 250
	};
	float trainClass[20] = { 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

	Mat dataMat = Mat(20, 2, CV_32FC1, trainData);
	Mat classMat = Mat(20, 1, CV_32SC1, trainClass);

	//분류기와 관련된 설정
	Ptr<ml::Boost> boost = ml::Boost::create();
	boost->setBoostType(ml::Boost::Types::REAL);
	boost->setWeakCount(5);
	boost->setWeightTrimRate(0);
	boost->setMaxDepth(3);
	boost->setUseSurrogates(false);

	//분류기에 샘플 데이터를 넣어 학습한다.
	boost->train(dataMat, ml::SampleTypes::ROW_SAMPLE, classMat);

	//테스트 결과를 보여줄 영상
	Mat image = Mat::zeros(Size(256, 256), CV_8UC3);

	//영상의 모든 점에 대해 테스트한다.
	for (int y = 0; y < image.rows; y++) 
	{
		for (int x = 0; x < image.cols; x++) 
		{
			float test[2] = { x, y };
			Mat testMat = Mat(1, 2, CV_32FC1, test);// cvMat(1, 2, CV_32FC1, test);
			float result = boost->predict(testMat, testMat, 0);//학습된 분류기를 이용한 분류
			if (result > 0)
				image.at<cv::Vec3b>(y, x) = Vec3b(100, 0, 0);
			else
				image.at<cv::Vec3b>(y, x) = Vec3b(0, 100, 0);
		}
	}
	//훈련 샘플에 대한 결과를 보여준다
	for (int i = 0; i < 20; i++)
	{
		int x = trainData[i * 2];
		int y = trainData[i * 2 + 1];
		float test[2] = { x, y };
		Mat testMat = Mat(1, 2, CV_32FC1, test);// cvMat(1, 2, CV_32FC1, test);
		float result = boost->predict(testMat, testMat, 0);//학습된 분류기를 이용한 분류
		if (result > 0)
			circle(image, Point(x, y), 2, Scalar(255, 0, 0));
		else
			circle(image, Point(x, y), 2, Scalar(0, 255, 0));
	}
	imshow("image", image);
	waitKey(0);
}