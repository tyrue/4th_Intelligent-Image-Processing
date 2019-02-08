#pragma once
class Defect
{
public:
	Defect();
	Defect(int num); // num 번째 결함
	~Defect();

	int num; // 결함의 개수

	// 결함의 중심 좌표
	int center_x;	
	int center_y;	

	// 결함의 크기
	int up;
	int down;
	int right;
	int left;
};

