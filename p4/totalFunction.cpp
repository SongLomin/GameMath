#include "stx.h"

//________________________________________________________________________________________________ 
// 
// -totalFunction 주석-
// 사용자 정의 전역 함수를 여기에 정의한다.
//________________________________________________________________________________________________ 

//________________________________________________________________________________________________ 
// 
// -DlgItemClear 주석-
// Clear 버튼을 눌렀을 때 에딧 박스의 수치를 초기화 해준다.
// 사용자가 직접 입력 가능한 벡터 V1과 V2 그리고 스칼라 K 값은 숫자 0으로 초기화 해주고
// 입력이 불가능한 나머지 에딧 박스는 빈 문자열로 초기화 해준다.
// 이는 GetDlgItemText와 마찬가지로 resource.h의 디파인 값이
// 연속적이라면 반복문을 통해 간단하게 표현할 수 있다.
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
// -DlgItemInit 주석-
// DlgItemInit 함수는 에딧 박스에 담겨진 char[] 값을 가져와 double 값으로 변환하여 리턴하는 함수이다.
// 매개변수로 다이얼로그의 핸들 값과 해당 다이얼로그에서 찾을 DEFINE을 읽어올 INT 코드 값을 가진다.
// 임시 변수 str[16] 크기의 문자열을 선언하여 해당 다이얼로그에 에딧 박스에 담겨진 값을 가져온다. 
// 이후 atof함수를 통해 문자열에서 double 타입으로 변환되어 리턴된다.
//________________________________________________________________________________________________ 

double DlgItemInit(HWND hDlg, int code)
{
	char str[16];

	GetDlgItemText(hDlg, code, str, 16);
	return atof(str);
}

//________________________________________________________________________________________________ 
// 
// -PrintOut 주석-
// PrintOut은 에딧 박스에 double 단일 값을 문자열로 변환하여 보내주는 역할을 하는 함수이다.
// 매개변수로 해당 다이얼로그의 핸들 값 hDlg, 
// 보내줄 에딧 박스 디파인 숫자 에딧네임, 보낼 값 temp를 받는다.
// sprintf_s를 통해 temp값을 문자열로 변환하여 임시 문자열 str에 담아준다.
// SetDlgItemText는 윈도우 API 라이브러리 함수이다.
// str을 매개변수로 받아온 에디트 박스 코드에 보내준다.
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

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)  //비트맵 그리는 함수
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

