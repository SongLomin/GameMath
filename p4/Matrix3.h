#pragma once
class Matrix3
{
public:
	double n[3][3];

	Matrix3();
	~Matrix3();
	void Init(HWND hDlg, INT a1, INT a2, INT a3, INT b1, INT b2, INT b3, INT c1, INT c2, INT c3);

	void PrintOut(HWND hDlg, INT EDIT_NAME1, INT EDIT_NAME2, INT EDIT_NAME3);\

	Matrix3 operator+(const Matrix3& v);

	Matrix3 operator-(const Matrix3& M);

	Matrix3 operator*(const Matrix3& M);

	Matrix3 operator*(const double& k);

	Matrix3 Transposed();

	double Determinant();

	Matrix3 Adj();

	Matrix3 Inversed();
};

