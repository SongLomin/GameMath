#pragma once
class Position
{
public:
	Position();
	~Position();

	void Init(INT a, INT b, INT c);

	float DlgItemInit(HWND hDlg, int code);

	float x, y, z;
	HWND hDlg;

private:

};

