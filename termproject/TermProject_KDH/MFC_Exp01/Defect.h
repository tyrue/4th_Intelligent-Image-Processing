#pragma once
class Defect
{
public:
	Defect();
	Defect(int num); // num ��° ����
	~Defect();

	int num; // ������ ����

	// ������ �߽� ��ǥ
	int center_x;	
	int center_y;	

	// ������ ũ��
	int up;
	int down;
	int right;
	int left;
};

