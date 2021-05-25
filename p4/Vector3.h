#pragma once
class Matrix3;


class Vector3
{
public:
	double x, y, z;

	Vector3();
	Vector3(int x, int y, int z);
	Vector3(int x, int y);
	~Vector3();

	void Init(HWND hDlg, INT x, INT y, INT z);
	void Init(HWND hDlg, INT a, INT b);
	void Init(int x, int y, int z);
	void Init(int x, int y);
	void PrintOut(HWND, INT);

	void PrintOutT(HWND hDlg, INT EDIT_NAME, INT EDIT_NAME2, INT EDIT_NAME3);

	void PrintOutT(HWND hDlg, INT EDIT_NAME, INT EDIT_NAME2);
	
	double Theta(const Vector3 v);
	Vector3 NomalizeV(double Length);
	Vector3 NomalizeV();


	Vector3 Rotate2D(double Degree, Vector3 v);
	Vector3 RotateX(Vector3 v, float angle);
	Vector3 RotateY(Vector3 v, float angle);
	Vector3 RotateZ(Vector3 v, float angle);

	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);

	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v);
	Vector3 operator*(const double& scalarK);
	double operator*(const Vector3& v);

	Vector3 operator*(const Matrix3& m);

	Vector3 CrossProduct(Vector3 v);

	Vector3 Projection(double theta, double length);

	Vector3 Projection(double theta);

	Vector3 Projection(Vector3 v);

	double Length();
	

private:
	

	Vector3 VectorMulti(const Vector3 v);

	
	
};
