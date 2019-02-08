#pragma once
class CMatrix
{
public:
	CMatrix();
	CMatrix(double* data, int row, int col);
	~CMatrix();

	int getRow();
	int getCol();

	void PrintMatrix();

	/*	operator overroding */
	CMatrix operator+(const CMatrix& m);
	CMatrix operator-(const CMatrix& m);
	CMatrix operator*(const CMatrix& m);

	/* 연산 함수 */
	CMatrix transpose();	// 전치행렬
	CMatrix inverse();		// 역행렬

	double* data;			// 행렬 데이터
private:
	int row;				// 행 크기
	int col;				// 열 크기
};

