#include "stx.h"

//________________________________________________________________________________________________ 
// 
// -totalFunction �ּ�-
// ����� ���� ���� �Լ��� ���⿡ �����Ѵ�.
//________________________________________________________________________________________________ 

//________________________________________________________________________________________________ 
// 
// -DlgItemClear �ּ�-
// Clear ��ư�� ������ �� ���� �ڽ��� ��ġ�� �ʱ�ȭ ���ش�.
// ����ڰ� ���� �Է� ������ ���� V1�� V2 �׸��� ��Į�� K ���� ���� 0���� �ʱ�ȭ ���ְ�
// �Է��� �Ұ����� ������ ���� �ڽ��� �� ���ڿ��� �ʱ�ȭ ���ش�.
// �̴� GetDlgItemText�� ���������� resource.h�� ������ ����
// �������̶�� �ݺ����� ���� �����ϰ� ǥ���� �� �ִ�.
//________________________________________________________________________________________________ 

void DlgItemClear(HWND hDlg)
{
	

	for (int i = 1002; i <= 1082; i++)
	{
		SetDlgItemInt(hDlg, i, 0, TRUE);
	}



	SetDlgItemText(hDlg, IDC_EDIT_V, "");
	SetDlgItemText(hDlg, IDC_EDIT_LENGTH, "");
	SetDlgItemText(hDlg, IDC_EDIT_NOMALV, "");
	SetDlgItemText(hDlg, IDC_EDIT_SCALAR_KV, "");
	SetDlgItemText(hDlg, IDC_EDIT_ADD, "");
	SetDlgItemText(hDlg, IDC_EDIT_SUB, "");
	SetDlgItemText(hDlg, IDC_EDIT_NORMALV1, "");
	SetDlgItemText(hDlg, IDC_EDIT_NORMALV2, "");
	SetDlgItemText(hDlg, IDC_EDIT_DOTPROV, "");
	SetDlgItemText(hDlg, IDC_EDIT_DOTPROS, "");
}

//________________________________________________________________________________________________ 
// 
// -DlgItemInit �ּ�-
// DlgItemInit �Լ��� ���� �ڽ��� ����� char[] ���� ������ double ������ ��ȯ�Ͽ� �����ϴ� �Լ��̴�.
// �Ű������� ���̾�α��� �ڵ� ���� �ش� ���̾�α׿��� ã�� DEFINE�� �о�� INT �ڵ� ���� ������.
// �ӽ� ���� str[16] ũ���� ���ڿ��� �����Ͽ� �ش� ���̾�α׿� ���� �ڽ��� ����� ���� �����´�. 
// ���� atof�Լ��� ���� ���ڿ����� double Ÿ������ ��ȯ�Ǿ� ���ϵȴ�.
//________________________________________________________________________________________________ 

double DlgItemInit(HWND hDlg, int code)
{
	char str[16];

	GetDlgItemText(hDlg, code, str, 16);
	return atof(str);
}

//________________________________________________________________________________________________ 
// 
// -PrintOut �ּ�-
// PrintOut�� ���� �ڽ��� double ���� ���� ���ڿ��� ��ȯ�Ͽ� �����ִ� ������ �ϴ� �Լ��̴�.
// �Ű������� �ش� ���̾�α��� �ڵ� �� hDlg, 
// ������ ���� �ڽ� ������ ���� ��������, ���� �� temp�� �޴´�.
// sprintf_s�� ���� temp���� ���ڿ��� ��ȯ�Ͽ� �ӽ� ���ڿ� str�� ����ش�.
// SetDlgItemText�� ������ API ���̺귯�� �Լ��̴�.
// str�� �Ű������� �޾ƿ� ����Ʈ �ڽ� �ڵ忡 �����ش�.
//________________________________________________________________________________________________ 

void PrintOut(HWND hDlg, INT EDIT_NAME, double temp)
{
	char str[16];

	sprintf_s(str, "%.2f", temp);
	SetDlgItemText(hDlg, EDIT_NAME, str);
}


void SetRectPos(POINT *p, int size)
{
	p[0].x += -size;
	p[0].y += size;
	p[1].x += size;
	p[1].y += size;
	p[2].x += size;
	p[2].y += -size;
	p[3].x += -size;
	p[3].y += -size;
}

void SetElpPos(POINT* p, int size)
{
	p[0].x -= size;
	p[0].y += size;
	p[1].x += size;
	p[1].y -= size;
}

void SetPos(POINT *p,int size, int x, int y)
{
	for (int i = 0; i < size; i++)
	{
		p[i].x = x;
		p[i].y = y;
	}
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)  //��Ʈ�� �׸��� �Լ�
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void AllPosRotation_Z(POINT* p, int count,Vector3 Pos, int angle)
{
	Vector3 temp;

	if (count == 1)
	{
		temp.Init(p->x - Pos.x, p->y - Pos.y);
		temp.SetRotateZ(angle);
		p->x = temp.x + Pos.x;
		p->y = temp.y + Pos.y;
	}

	else
	{
		for (int i = 0; i < count; i++)
		{

			temp.Init(p[i].x - Pos.x, p[i].y - Pos.y);
			temp.SetRotateZ(angle);
			p[i].x = temp.x + Pos.x;
			p[i].y = temp.y + Pos.y;
		}
	}
}

void AllPosRotation_Z(Vector3 *p, int count, Vector3 Pos, int angle)
{
	Vector3 temp;

	if (count == 1)
	{
		temp.Init(p->x - Pos.x, p->y - Pos.y);
		temp.SetRotateZ(angle);
		p->x = temp.x + Pos.x;
		p->y = temp.y + Pos.y;
	}

	else
	{
		for (int i = 0; i < count; i++)
		{

			temp.Init(p[i].x - Pos.x, p[i].y - Pos.y);
			temp.SetRotateZ(angle);
			p[i].x = temp.x + Pos.x;
			p[i].y = temp.y + Pos.y;
		}
	}

	
}

