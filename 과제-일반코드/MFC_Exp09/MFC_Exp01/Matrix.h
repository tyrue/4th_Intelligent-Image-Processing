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

	/* ���� �Լ� */
	CMatrix transpose();	// ��ġ���
	CMatrix inverse();		// �����

	double* data;			// ��� ������
private:
	int row;				// �� ũ��
	int col;				// �� ũ��
};

