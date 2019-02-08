#include "stdafx.h"
#include "Classifier.h"

Classifier::Classifier(int direction, int th, int axis)
{
	this->Direction = direction;
	this->Th = th;
	this->Axis = axis;
}

int Classifier::Classify(Sample& sample)
{
	if (Axis == 1) //u�� ����
	{
		if (Direction * sample.x_u >= Direction * Th)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else if (Axis == 2) //v�� ����
	{
		if (Direction * sample.x_v >= Direction * Th)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else//����
	{
		return 0;
	}
}

Classifier::Classifier()
{

}

Classifier::~Classifier()
{

}