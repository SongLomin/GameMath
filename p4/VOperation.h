#pragma once
static class VOperation
{
public:
	VOperation();
	~VOperation();

	static Vector3 VectorSum(Vector3 v1, Vector3 v2);
	Vector3 V1subV2(Vector3 v1, Vector3 v2);
	float VLength(Vector3 v1, Vector3 v2);
	Vector3 NomalizeV(Vector3 v1, float Length);
	Vector3 VectorScalar(Vector3 v, float scalar);

	float VectorMulti(Vector3 v1, Vector3 v2);

	void PrintOut(HWND hDlg, INT EDIT_NAME, float temp);
};

