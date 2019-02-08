#include "stdafx.h"
#include "Matrix.h"
#include <iostream>
using namespace std;
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
CMatrix::CMatrix(){}

CMatrix::CMatrix(double* data, int row, int col)
{
	this->data = data;
	this->col = col;
	this->row = row;
}

CMatrix::~CMatrix(void)
{
	delete[] data;
}

int CMatrix::getCol()
{
	return this->col;
}

int CMatrix::getRow()
{
	return this->row;
}

void CMatrix::PrintMatrix()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			cout << data [i * col + j] << " " ;
		}
		cout << endl;
	}
}

CMatrix CMatrix::operator+(const CMatrix& m)
{
	double* mat = new double[m.row * m.col + m.col];
	int idx;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			idx = i * col + j;
			mat[idx] = data[idx]+m.data[idx];
		}
	}
	return CMatrix(mat, row, col);
}

CMatrix CMatrix::operator-(const CMatrix& m)
{
	double* mat = new double[m.row * m.col + m.col];
	int idx;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			idx = i * col + j;
			mat[idx] = data[idx] - m.data[idx];
		}
	}
	return CMatrix(mat, row, col);
}

CMatrix CMatrix::operator*(const CMatrix& m)
{
	double* mat = new double[row * m.col + m.col];

	int idx;
	float sum = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < m.col; j++)
		{
			idx = i *  m.col + j;
			sum = 0;
			for (int k = 0; k < col; k++)
			{
				sum += data[i * col + k] * m.data[k * m.col + j];
			}
			mat[idx] = sum;
		}
	}
	return CMatrix(mat, row, m.col);
}

CMatrix CMatrix::transpose()
{
	double* mat = new double[row*col];
	for (int i = 0 ; i < row; i++)
	{
		for ( int j = 0; j < col; j++)
		{
			mat[j * row + i] = data[i* col + j];
		}
	}
	return CMatrix(mat,col,row);
}

CMatrix CMatrix::inverse()
{
	double* argMatrix = new double[col * 2 * row];	// argmented Matrix (확대행렬)
	double* mat = new double[row*col];
	int idx1, idx2;
	int maxIdx = 0;
	double max = 0;
	double ratio =0.0;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			idx1 = i * col*2 + j;
			idx2 = i * col + j;
			argMatrix[idx1] = data[idx2];
			// 단위 행렬 추가
			if (i == j) argMatrix[idx1+col] = 1;
			else argMatrix[idx1+col] = 0;
		}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if ( i != j)
			{
				ratio = argMatrix [ j * 2*col + i]  /  argMatrix [i * 2*col + i]  ;
				for (int k = 0; k < 2 * col; k++)
				{
					argMatrix[j * 2*col + k] -= ratio * argMatrix[i * 2*col + k];
				}
			}
		}
	}
	double a;
	for ( int i = 0; i < row; i++)
	{
		a = argMatrix[i * 2*col +i];
		for ( int j = 0; j < 2 * col ; j++)
		{
			argMatrix[i* 2*col + j] /= a;
		}
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = col; j < 2* col; j++)
		{
			mat[i * col + j - col] = argMatrix[i * 2*col + j];
		}
	}
	delete[] argMatrix;
	return CMatrix(mat, row, col);
}