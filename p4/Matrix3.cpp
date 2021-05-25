#include "Stx.h"



Matrix3::Matrix3()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			n[i][j] = 0;
		}
	}
}


Matrix3::~Matrix3()
{

}

void Matrix3::Init(HWND hDlg, INT a1, INT a2, INT a3, INT b1, INT b2, INT b3, INT c1, INT c2, INT c3)
//초기화를 다이얼로그 에딧에서 가져오는 경우를 정의한다.
{
	n[0][0] = DlgItemInit(hDlg, a1);
	n[0][1] = DlgItemInit(hDlg, a2);
	n[0][2] = DlgItemInit(hDlg, a3);
	n[1][0] = DlgItemInit(hDlg, b1);
	n[1][1] = DlgItemInit(hDlg, b2);
	n[1][2] = DlgItemInit(hDlg, b3);
	n[2][0] = DlgItemInit(hDlg, c1);
	n[2][1] = DlgItemInit(hDlg, c2);
	n[2][2] = DlgItemInit(hDlg, c3);
}

void Matrix3::PrintOut(HWND hDlg, INT EDIT_NAME1, INT EDIT_NAME2, INT EDIT_NAME3)
{
	char str[256];
	int name[3] = { EDIT_NAME1, EDIT_NAME2, EDIT_NAME3 };

	for (int i = 0; i < 3; i++)
	{
		sprintf_s(str, "| %.1f, %.1f, %.1f |", n[i][0], n[i][1], n[i][2]);
		SetDlgItemText(hDlg, name[i], str);
	}
}

Matrix3 Matrix3::operator+(const Matrix3& M)
{
	Matrix3 temp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.n[i][j] = this->n[i][j] + M.n[i][j];
		}
	}

	return temp;
}

Matrix3 Matrix3::operator-(const Matrix3& M)
{
	Matrix3 temp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.n[i][j] = this->n[i][j] - M.n[i][j];
		}
	}

	return temp;
}

Matrix3 Matrix3::operator*(const Matrix3& M)
{
	Matrix3 temp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				temp.n[i][j] += this->n[i][k] * M.n[k][j];
			}

		}
	}

	return temp;
}

Matrix3 Matrix3::operator*(const double& k)
{
	Matrix3 temp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.n[i][j] = k * this->n[i][j];
		}
	}

	return temp;
}

Matrix3 Matrix3::Transposed()
{
	Matrix3 temp = *this;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i != j)
				temp.n[i][j] = this->n[j][i];
		}
	}

	return temp;
}

double Matrix3::Determinant()
{
	double temp;

	temp = 
		  n[0][0] * n[1][1] * n[2][2]
		+ n[0][1] * n[1][2] * n[2][0]
		+ n[0][2] * n[1][0] * n[2][1]
		- n[0][0] * n[1][2] * n[2][1]
		- n[0][1] * n[1][0] * n[2][2]
		- n[0][2] * n[1][1] * n[2][0];
	
	return temp;
}

Matrix3 Matrix3::Adj()
{
	Matrix3 temp;

	temp.n[0][0] = n[1][1] * n[2][2] - n[1][2] * n[2][1];
	temp.n[0][1] = -(n[0][1] * n[2][2] - n[0][2] * n[2][1]);
	temp.n[0][2] = n[0][1] * n[1][2] - n[0][2] * n[1][1];
	temp.n[1][0] = -(n[1][0] * n[2][2] - n[1][2] * n[2][0]);
	temp.n[1][1] = n[0][0] * n[2][2] - n[0][2] * n[2][0];
	temp.n[1][2] = -(n[0][0] * n[1][2] - n[0][2] * n[1][0]);
	temp.n[2][0] = n[1][0] * n[2][1] - n[1][1] * n[2][0];
	temp.n[2][1] = -(n[0][0] * n[2][1] - n[0][1] * n[2][0]);
	temp.n[2][2] = n[0][0] * n[1][1] - n[0][1] * n[1][0];


	return temp;
}

Matrix3 Matrix3::Inversed()
{
	return (Adj()*((double)1.0f/Determinant()));
}