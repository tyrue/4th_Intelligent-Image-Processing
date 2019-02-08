#pragma once
#include "Sample.h"
class Classifier
{
public:
	int Direction; // U축 혹은 V축 방향의 Polarity +1 or -1
	int Th; // U축 혹은 V축 위의
	int Axis; //기준 축, 1 = U축, 2 = V축

	double error;
	double alpha;

	Classifier(int direction, int th, int axis);
	int Classify(Sample& sample);

	Classifier();
	~Classifier();
};

