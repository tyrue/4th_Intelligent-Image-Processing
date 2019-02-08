#pragma once
#include "Sample.h"
class Classifier
{
public:
	int Direction; // U�� Ȥ�� V�� ������ Polarity +1 or -1
	int Th; // U�� Ȥ�� V�� ����
	int Axis; //���� ��, 1 = U��, 2 = V��

	double error;
	double alpha;

	Classifier(int direction, int th, int axis);
	int Classify(Sample& sample);

	Classifier();
	~Classifier();
};

