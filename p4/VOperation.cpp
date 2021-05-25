#include "stx.h"
#include "Vector3.h"


VOperation::VOperation()
{
}


VOperation::~VOperation()
{
}

Vector3 VOperation::VectorSum(Vector3 v1, Vector3 v2)
{
	Vector3 temp;

	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	temp.z = v1.z + v2.z;

	return temp;
}

Vector3 VOperation::V1subV2(Vector3 v1, Vector3 v2)
{
	Vector3 temp;

	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	temp.z = v1.z - v2.z;

	return temp;
}

float VOperation::VLength(Vector3 v1, Vector3 v2)
{
	float temp = 0;

	temp += v1.x + v2.x;
	temp += v1.y + v2.y;
	temp += v1.z + v2.z;

	return sqrt(temp);
}

Vector3 VOperation::NomalizeV(Vector3 v, float Length)
{
	Vector3 temp;

	temp.x = v.x / Length;
	temp.y = v.y / Length;
	temp.z = v.z / Length;

	return temp;
}

Vector3 VOperation::VectorScalar(Vector3 v, float scalar)
{
	Vector3 temp;

	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	temp.z = v.z * scalar;

	return temp;
}

float VOperation::VectorMulti(Vector3 v1, Vector3 v2)
{
	float temp = 0;

	temp += v1.x * v2.x;
	temp += v1.y * v2.y;
	temp += v1.z * v2.z;

	return temp;
}

void VOperation::PrintOut(HWND hDlg, INT EDIT_NAME, float temp)
{
	char str[256];

	sprintf_s(str, "%.2f", temp);
	SetDlgItemText(hDlg, EDIT_NAME, str);
}