#include "stdafx.h"
#include "Defect.h"


Defect::Defect()
{

}

Defect::Defect(int num)
{
	this->num = num;
	this->center_x = 0;
	this->center_y = 0;

	// �� ������ ũ�⸦ ��ǥ�� �ּ�, �ִ밪���� ���ϱ� ����
	this->up = 9999;	
	this->down = 0;
	this->right = 0;
	this->left = 9999;
}

Defect::~Defect()
{

}
