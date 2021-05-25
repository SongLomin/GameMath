#pragma once
class Vector3;

void DlgItemClear(HWND hDlg);
double DlgItemInit(HWND hDlg, int code);

void PrintOut(HWND hDlg, INT EDIT_NAME, double temp);

void SetRectPos(POINT* p, int size);
void SetElpPos(POINT* p, int size);
void SetPos(POINT* p, int size, int x, int y);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void AllPosRotation_Z(POINT* p, int count,Vector3 Pos, int angle);
void AllPosRotation_Z(Vector3* p, int count, Vector3 Pos, int angle);