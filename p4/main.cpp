#include "stx.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK VectorDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK MatrixDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK TRANSLATE_2D_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK TRANSLATE_3D_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK ShortestLineToPointDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK IntersectionLineDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SolarSystem_Init(HWND hwnd);

void DlgItemClear(HWND);
double DlgItemInit(HWND, int);
void PrintOut(HWND, INT, double);
bool solarSystem_Active = false;
bool lineToPoint_Active = false;
bool interLine = false;

HINSTANCE g_hInst;
HWND m_hwnd;
HWND m_interDlg;

static POINT sunPoint[4];
static POINT earthPoint[4];
static POINT moonPoint[2];

static POINT nowSunPoint[4];
static POINT nowEarthPoint[4];
static POINT nowMoonPoint[2];

HBRUSH hBrush, hOldBrush;

HDC hdc;
static RECT screen;
PAINTSTRUCT ps;

static Vector3 sunV(SUNPOS_X, SUNPOS_Y);
static Vector3 earthV(EARTHPOS_X, EARTHPOS_Y);
static Vector3 moonV(MOONPOS_X, MOONPOS_Y);

static int timerCount;
static double moonsizeCount;

static HWND hEdit;

Vector3 nowEarthV;
Vector3 nowMoonV;

// �ִܰŸ�

static Vector3 lineP1, lineP2;
static Vector3 dot;

// ����
double litteZero = 0.0001;
static Vector3 rectPos[4], linePos[2];
static Vector3 v0v1, v0v3, lineV, crossNV;
static Vector3 intersecPos;

static POINT ipolygon[4], iline[2];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND   hwnd;
	MSG    msg;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(190,190,190));
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name",
		"GAME MATH",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	

	switch (iMsg)
	{
	case WM_CREATE:
		m_hwnd = hwnd;
		GetClientRect(hwnd, &screen);
		return 0;

	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case ID_FILESAVE:
				break;

			case ID_FILEOPEN:
				break;

			case ID_FILE_EXIT:
				DestroyWindow(hwnd);
				break;

			case ID_VECTOR_OPERATION:
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_VECTOR), hwnd, (DLGPROC)VectorDlgProc);
				break;

			case ID_MATRIX_OPERATOR:
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MATRIX), hwnd, (DLGPROC)MatrixDlgProc);
				break;

			case ID_TRANSFORM_2D:
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_TRANSFORM_2D), hwnd, (DLGPROC)TRANSLATE_2D_DlgProc);
				break;

			case ID_TRANSFORM_3D:
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_TRANSFORM_3D), hwnd, (DLGPROC)TRANSLATE_3D_DlgProc);
				break;

			case ID_SOLARSYSTEM_ACTIVE:
				SolarSystem_Init(hwnd);
				solarSystem_Active = 1 - solarSystem_Active;
				break;

			case ID_SHORTEST_LINETOPOINT:
				lineToPoint_Active = 1 - lineToPoint_Active;
				SetTimer(hwnd, 1, 16, NULL);
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_LINETOPOINT), hwnd, (DLGPROC)ShortestLineToPointDlgProc);
				break;

			case ID_INTERSECTION_LINE:
				interLine = 1 - interLine;
				//SetTimer(hwnd, 2, 16, NULL); //������ �����̰� ��
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INTERSECTION),hwnd,(DLGPROC)IntersectionLineDlgProc);
				InvalidateRect(hwnd, NULL, true);
				break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetMapMode(hdc, MM_LOENGLISH);
		SetViewportOrgEx(hdc, screen.right / 2, screen.bottom / 2, NULL);

		if (solarSystem_Active)
		{
			

			hBrush = CreateSolidBrush(RGB(200, 60, 60));//�÷�
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Polygon(hdc, nowSunPoint, sizeof(nowSunPoint) / sizeof(POINT));

			DeleteObject(SelectObject(hdc, hOldBrush));

			hBrush = CreateSolidBrush(RGB(60, 60, 200));//�÷�
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Polygon(hdc, nowEarthPoint, sizeof(earthPoint) / sizeof(POINT));

			DeleteObject(SelectObject(hdc, hOldBrush));

			hBrush = CreateSolidBrush(RGB(230, 230, 230));//�÷�
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, nowMoonPoint[0].x, nowMoonPoint[0].y, nowMoonPoint[1].x, nowMoonPoint[1].y);
			DeleteObject(SelectObject(hdc, hOldBrush));
		}

		if (lineToPoint_Active)
		{
			///��� ��ǥ
			MoveToEx(hdc, -3000, 0, NULL);
			LineTo(hdc, 3000, 0);
			MoveToEx(hdc, 0, 3000, NULL);
			LineTo(hdc, 0, -3000);
			///����

			MoveToEx(hdc, lineP1.x, lineP1.y, NULL);
			LineTo(hdc, lineP2.x, lineP2.y);

			MoveToEx(hdc, dot.x-3, dot.y, NULL);
			LineTo(hdc, dot.x + 3, dot.y);
			MoveToEx(hdc, dot.x, dot.y - 3, NULL);
			LineTo(hdc, dot.x, dot.y + 3);

		}

		if (interLine)
		{
			MoveToEx(hdc, -3000, 0, NULL);
			LineTo(hdc, 3000, 0);
			MoveToEx(hdc, 0, 3000, NULL);
			LineTo(hdc, 0, -3000);

			Polygon(hdc, ipolygon, sizeof(ipolygon) / sizeof(POINT));
			MoveToEx(hdc, iline[0].x, iline[0].y, NULL);
			LineTo(hdc, iline[1].x, iline[1].y);

		}

		EndPaint(hwnd, &ps);
		
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 0: //Solar_system
			copy(sunPoint, sunPoint + sizeof(sunPoint) / sizeof(POINT), nowSunPoint);
			copy(earthPoint, earthPoint + sizeof(earthPoint) / sizeof(POINT), nowEarthPoint);
			copy(moonPoint, moonPoint + sizeof(moonPoint) / sizeof(POINT), nowMoonPoint);
			nowEarthV = earthV;
			nowMoonV = moonV;



			timerCount++;
			moonsizeCount += 0.2;

			AllPosRotation_Z(nowSunPoint, sizeof(sunPoint) / sizeof(POINT), sunV, 3 * timerCount);

			AllPosRotation_Z(nowEarthPoint, sizeof(nowEarthPoint) / sizeof(POINT), earthV, 4 * timerCount);
			AllPosRotation_Z(nowEarthPoint, sizeof(nowEarthPoint) / sizeof(POINT), sunV, 2 * timerCount);

			AllPosRotation_Z(&nowEarthV, 1, sunV, 2 * timerCount); //���� ������ ������ ã����

			AllPosRotation_Z(&nowMoonV, 1, sunV, 2 * timerCount);
			AllPosRotation_Z(&nowMoonV, 1, nowEarthV, 5 * timerCount);
			SetPos(nowMoonPoint, sizeof(moonPoint) / sizeof(POINT), nowMoonV.x, nowMoonV.y);
			SetElpPos(nowMoonPoint, MOONSIZE + ((int)moonsizeCount % 8));

			InvalidateRect(hwnd, NULL, true);
			break;

		case 1: //Shortest distance of line to point
			InvalidateRect(hwnd, NULL, true);
			break;

		case 2:
			linePos[0] = linePos[0] + lineV.NomalizeV();
			linePos[1] = linePos[1] + lineV.NomalizeV();
			double rectLength = (rectPos[0] - linePos[0]).Length();
			double lineLength = lineV.Length();
			

			if (fabs(lineV.NomalizeV() * crossNV) < litteZero)
			{

				for (int i = 0; i < 2; i++)
				{
					iline[i].x = linePos[i].x;
					iline[i].y = linePos[i].y;
				}
			}
			else //����
			{
				if (rectLength < lineLength)
				{
					float scalar_result;
					float posDotN;
					Vector3 v;

					v = rectPos[0] - linePos[0];
					scalar_result = v * crossNV;
					posDotN = linePos[0] * crossNV;
					scalar_result = scalar_result / posDotN;
					intersecPos = linePos[0] + lineV.NomalizeV() * scalar_result;
					intersecPos.PrintOutT(m_interDlg, IDC_IX, IDC_IY, IDC_IZ);
					KillTimer(hwnd, 2);
				}

				else
				{
					for (int i = 0; i < 2; i++)
					{
						iline[i].x = linePos[i].x;
						iline[i].y = linePos[i].y;
					}
				}
			}

			InvalidateRect(hwnd, NULL, true);
			break;
		}
		
		break;



	case WM_DESTROY:
		if(solarSystem_Active)
			KillTimer(hwnd, 0);
		if (lineToPoint_Active)
			KillTimer(hwnd, 1);
		else if (interLine)
			KillTimer(hwnd, 2);

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);

}


BOOL CALLBACK VectorDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	Vector3 V1, V2;
	double ScalarK;

	Vector3 Vsum; //���� ���� ����
	Vector3 V; //���� V2-V1�� ����
	double Length; //Length
	double LengthV2;

	Vector3 NomalV;
	Vector3 NomalV1, NomalV2;
	Vector3 ScalarKV;
	Vector3 V1subV2;
	double DotP;
	Vector3 NomalLength;
	double Theta;
	Vector3 CrossProductV;
	Vector3 ProjectionV;
	Vector3 VerticalV;


	switch (uMsg)
	{
	case WM_INITDIALOG:
		DlgItemClear(hDlg);
		//���̾�α� ���� ��Ʈ�� 0���� �ʱ�ȭ
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			
		case IDC_BTN_CALCULATE:

			//________________________________________________________________________________________________ 
			//
			//	resource.h�� Define ���� ���ӵȴٸ� �ݺ����� ���� ȿ�������� ���� �о�� �� �ִ�.
			//
			// for (int i = 0; i < 6; i++)
			// {
			// 	 Vectors[i] = DlgItemInit(hDlg, IDC_EDIT_V1_X+i);
			// }
			// ScalarK = DlgItemInit(hDlg,IDC_EDIT_SCALARK);
			//________________________________________________________________________________________________ 

			//________________________________________________________________________________________________ 
			//
			// ���� ���� �����ϴ� �κ��̴�. Init�� �Ű������� ���� �ڽ��� �о�� ���̾�α� �ڵ� ���� 
			// ���� ���� x, y, z �� ���� �־��� ���� �ڽ��� Define���� �����ش�.
			//________________________________________________________________________________________________ 
			V1.Init(hDlg, IDC_EDIT_V1_X, IDC_EDIT_V1_X2, IDC_EDIT_V1_X3);
			V2.Init(hDlg, IDC_EDIT_V2_X, IDC_EDIT_V2_X2, IDC_EDIT_V2_X3);

			ScalarK = DlgItemInit(hDlg, IDC_EDIT_SCALAR_K);

			
			//________________________________________________________________________________________________ 
			//
			// -Vector V (V2 - V1)�� ���ϴ� �˰���-
			// ����V2�� ����V1�� �̿��� ���� V�� ���� ���̴�. 
			// ����� ���� Ŭ���� ����3Ÿ���� �����ڷ� ������ �Ұ��������� ������ �����ε��� ����
			// Ŭ���������� ���� ������ ������ ��ó�� ���̰� �����.
			// ������ �����ε� ���۷�����+�� ���Ƿ� �̵��غ���. <(V2 - V1)�� ���� ���� �̵�>
			// ���� Vector3�� ��� �Լ� PrintOut�� ���� ���� �����Ѵ�.
			// �Ű������� ����Ʈ �ڽ� ���޿� �ʿ��� ���̾�α��� �ڵ� ���� ����Ʈ �ڽ��� �̸� �������� �Ѱ��ش�.
			//________________________________________________________________________________________________ 

			V = V2 - V1;
			V.PrintOut(hDlg, IDC_EDIT_V);


			//________________________________________________________________________________________________ 
			//
			// -���� V�� ��� ���ϴ� �˰���-
			// V�� ��� ������ ��(x^2+y^2+z^2)�̴�. �츮�� ���� ������ © ���� ������
			// math.h���� ��Ʈ ���� ���̺귯���� �����ϰ� �ִ�. 
			// ���� ������ �����ε��� ���� ���ҵ��� ���� ���ϰ� ���� ��
			// math.h�� ���Ե� ���̺귯�� �Լ� sqrt()�� ����� ��Ʈ�� Length���� �����Ѵ�.
			// ���� ���� �����ϰ� ����Ʈ �ڽ��� ��½�Ų��.
			// ������ Length�� double Ÿ���̱� ������ Vector3�� ��� �Լ��� PrintOut�� ����� �� ����.
			// ���� ������ �̸��� ���� �Լ� PrintOut�� ���� �Ű������� ���Խ��� �Ѱ��ش�.
			//________________________________________________________________________________________________ 

			Length = sqrt(V * V);
			PrintOut(hDlg, IDC_EDIT_LENGTH, Length);
			

			//________________________________________________________________________________________________
			//
			// -���� V�� �������� ���ϴ� �˰���-
			// �������� �Լ��� �����ϴµ��� �� ���� ����� �ִ�. ������ ũ��(���) ���� �Ű������� �Ѱ��ְų�,
			// �Ű����� ���� �ش� ������ ���ҵ�� ����� ���ؼ� �������͸� ���� �� �ִ�.
			// �� �˰��򿡼��� ���� �ڽ��� ���� ��� ������� ����ϰ� �ִ�.
			// ���� �̹� ������ ���� ��� ���� �Ű������� �߰����� ���� ���� ���� V�� �������͸� ���� �� �ִ�.
			// �������ʹ� �� ���ҿ� ��� ������ ������ ������ ũ�Ⱑ 1�� ����, �������Ͱ� �ȴ�.
			//________________________________________________________________________________________________ 

			NomalV = V.NomalizeV(Length);
			NomalV.PrintOut(hDlg, IDC_EDIT_NOMALV);


			//________________________________________________________________________________________________
			//
			// -���� V�� ��Į�� K�� ���� ���ϴ� �˰���-
			// �ش� �� ���� ������ �����ε��� ���� �ذ��Ѵ�.
			// ���Ƿ� �̵��غ���.
			//________________________________________________________________________________________________

			ScalarKV = V * ScalarK;
			ScalarKV.PrintOut(hDlg, IDC_EDIT_SCALAR_KV);


			//________________________________________________________________________________________________
			//
			// -���� V1�� ���� V2�� ���� ���ϴ� �˰���-
			// �ش� �� ���� ������ �����ε��� ���� �ذ��Ѵ�.
			// ���Ƿ� �̵��غ���.
			//________________________________________________________________________________________________
			Vsum = V1 + V2;
			Vsum.PrintOut(hDlg, IDC_EDIT_ADD);


			//________________________________________________________________________________________________
			//
			// -���� V1 - V2�� ���ϴ� �˰���-
			// ���� V�� ���ϴ� ���İ��� �ݴ�� ���� �������� ���ʰ� �ڰ� �ٲ�� �ִ�.
			// ���� ���̹Ƿ� �Ѱ� �ִ� ���� �ݴ�� �Ѱ��־� ����Ѵ�.
			//________________________________________________________________________________________________

			V1subV2 = V1 - V2;
			V1subV2.PrintOut(hDlg, IDC_EDIT_SUB);


			//________________________________________________________________________________________________
			//
			// -���� V1�� �������͸� ���ϴ� �˰���-
			// �Ű������� ���������� ����� �Ű������� �Ѱ��־� ����Ѵ�. ���� V�� �������͸� ���� ���� ����������
			// V1�� ����� ����� ���� ���� ������ ������ �����ε��� math.h�� ���Ե� sqrt �Լ��� �̿��Ͽ� ����� ���Ѵ�.
			//________________________________________________________________________________________________
			
			NomalV1 = V1.NomalizeV(sqrt(V1 * V1));

			NomalV1.PrintOut(hDlg, IDC_EDIT_NORMALV1);


			//________________________________________________________________________________________________
			//
			// -���� V2�� �������͸� ���ϴ� �˰���-
			// ���� �����ϴ�. ���������� ����� ����ؼ� �Ű������� �����ش�.
			//________________________________________________________________________________________________

			NomalV2 = V2.NomalizeV(sqrt(V2 * V2));

			NomalV2.PrintOut(hDlg, IDC_EDIT_NORMALV2);


			//________________________________________________________________________________________________
			//
			// -�������� V1�� �������� V2�� ������ ���ϴ� �˰���-
			// ���������� ������ �����ε��� �̿��Ѵ�. ������ ������ ���� ������ �����ε����� �ٸ���.
			// ������ ���� ������ �����ε��� ���� Ÿ���� ����3�� ������ ����3, ������ double Ÿ���̾��ٸ�
			// �ش� ������ ���� Ÿ���� double�̰� ������ ����3 Ÿ���̴�.
			// ���� ���� �����̶� �ٸ� Oprator �Լ��� ���Ƿ� �ٸ� �����̴�.
			//________________________________________________________________________________________________

			DotP = NomalV1 * NomalV2;

			PrintOut(hDlg, IDC_EDIT_DOTPROV, DotP);

			//________________________________________________________________________________________________
			//
			// -�� ���Ͱ� �̷�� ������ ���ϴ� �˰���-
			// V1�� V2�� �踦 ���ϴ� ������ V1�� V2�� ���Ҹ� ��� ���� ���� ���Ѵ�.
			// ���� ��ũ�ڻ����� ����� ��Ÿ ���� �˾Ƴ� �� �ִ�. math.h���� ��ũ�ڻ��ο� ���� ���̺귯�� �Լ��� �����Ѵ�.
			// ���ǿ��� Ȯ���غ���
			//________________________________________________________________________________________________

			Theta = NomalV1.Theta(NomalV2);
			PrintOut(hDlg, IDC_EDIT_DOTPROS, Theta);
			
			///////////////////////////////////////////

			CrossProductV = NomalV1.CrossProduct(NomalV2);
			CrossProductV.PrintOut(hDlg, IDC_EDIT_CROSSVECTOR);

			ProjectionV = NomalV2.Projection(Theta,sqrt(V1*V1));
			ProjectionV.PrintOut(hDlg, IDC_EDIT_PROJECTION);

			VerticalV = V1 - ProjectionV;
			VerticalV.PrintOut(hDlg, IDC_EDIT_VERTICAL);


			break;

		case IDC_BTN_CLEAR:
			DlgItemClear(hDlg);
			//���̾�α� ���� ��Ʈ�� 0���� �ʱ�ȭ

			break;

		case IDC_BTN_CLOSE:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}

BOOL CALLBACK MatrixDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
	Matrix3 A, B;
	double scalarK;

	Matrix3 sumN, subN, MultiplyN;
	Matrix3 KA, KB;
	Matrix3 TransposedA, TransposedB;
	double DeterminantA, DeterminantB;
	Matrix3 AdjA, AdjB;
	Matrix3 InverseA, InverseB;


	switch (uMsg)
	{
	case WM_INITDIALOG:
		
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BTN_CALCULATE:
			A.Init(hDlg, IDC_A110, IDC_A12, IDC_A13, IDC_A21, IDC_A22, IDC_A23, IDC_A31, IDC_A32, IDC_A33);
			B.Init(hDlg, IDC_B11, IDC_B12, IDC_B13, IDC_B21, IDC_B22, IDC_B23, IDC_B31, IDC_B32, IDC_B33);
			scalarK = DlgItemInit(hDlg, IDC_SCALARK);

			sumN = A+B;
			sumN.PrintOut(hDlg, IDC_MATRIX_SUM1, IDC_MATRIX_SUM2, IDC_MATRIX_SUM3);

			subN = A - B;
			subN.PrintOut(hDlg, IDC_MATRIX_SUB1, IDC_MATRIX_SUB2, IDC_MATRIX_SUB3);

			MultiplyN = A*B;
			MultiplyN.PrintOut(hDlg, IDC_MATRIX_MULTIPL1, IDC_MATRIX_MULTIPL2, IDC_MATRIX_MULTIPL3);

			KA = A * scalarK;
			KA.PrintOut(hDlg, IDC_MATRIX_KA1, IDC_MATRIX_KA2, IDC_MATRIX_KA3);

			KB = B * scalarK;
			KB.PrintOut(hDlg, IDC_MATRIX_KB1, IDC_MATRIX_KB2, IDC_MATRIX_KB3);

			TransposedA = A.Transposed();
			TransposedA.PrintOut(hDlg, IDC_MATRIX_TRANSA1, IDC_MATRIX_TRANSA2, IDC_MATRIX_TRANSA3);

			TransposedB = B.Transposed();
			TransposedB.PrintOut(hDlg, IDC_MATRIX_TRANSB1, IDC_MATRIX_TRANSB2, IDC_MATRIX_TRANSB3);

			DeterminantA = A.Determinant();
			PrintOut(hDlg, IDC_DeterA, DeterminantA);

			DeterminantB = B.Determinant();
			PrintOut(hDlg, IDC_DeterB, DeterminantB);

			AdjA = A.Adj();
			AdjA.PrintOut(hDlg, IDC_MATRIX_ADJOINT_A1, IDC_MATRIX_ADJOINT_A2, IDC_MATRIX_ADJOINT_A3);

			AdjB = B.Adj();
			AdjB.PrintOut(hDlg, IDC_MATRIX_ADJOINT_B1, IDC_MATRIX_ADJOINT_B2, IDC_MATRIX_ADJOINT_B3);

			InverseA = A.Inversed();
			InverseA.PrintOut(hDlg, IDC_InvrseA1, IDC_InvrseA2, IDC_InvrseA3);

			InverseB = B.Inversed();
			InverseB.PrintOut(hDlg, IDC_InvrseB1, IDC_InvrseB2, IDC_InvrseB3);
			break;

		case IDC_BTN_CLEAR:
			DlgItemClear(hDlg);
			break;

		case IDC_BTN_CLOSE:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}

BOOL CALLBACK TRANSLATE_2D_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Vector3 vector2, dvector2;
	Vector3 translate_vector2;
	Vector3 rotate_vector2;
	double degree;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_CALCULATE:
			vector2.Init(hDlg, IDC_X, IDC_Y);
			dvector2.Init(hDlg, IDC_DX, IDC_DY);
			degree = DlgItemInit(hDlg, IDC_DEGREE);

			translate_vector2 = vector2 + dvector2;
			translate_vector2.PrintOutT(hDlg, IDC_TRANSLATE_X, IDC_TRANSLATE_Y);

			rotate_vector2.Init(vector2.x, vector2.y);
			rotate_vector2.Rotate2D(degree, vector2);
			rotate_vector2.PrintOutT(hDlg, IDC_ROTATION_X, IDC_ROTATION_Y);
			
			break;

		case IDC_CLEAR:
			DlgItemClear(hDlg);
			break;

		case IDC_BTN_CLOSE:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}

BOOL CALLBACK TRANSLATE_3D_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Vector3 v;
	Vector3 angles;
	Vector3 mv;
	Vector3 tv;
	Vector3 rx, ry, rz;

	switch (uMsg)
	{
	case WM_INITDIALOG:

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BTN_CALCULATE:
			v.Init(hDlg, IDC_X, IDC_Y, IDC_Z);
			angles.Init(hDlg, IDC_RX, IDC_RY, IDC_RZ);
			mv.Init(hDlg, IDC_MX, IDC_MY, IDC_MZ);

			tv = v + mv;
			tv.PrintOutT(hDlg, IDC_3DTRAN_X, IDC_3DTRAN_Y, IDC_3DTRAN_Z);
			
			rx = rx.RotateX(v, angles.x);
			rx.PrintOutT(hDlg, IDC_Xls_X, IDC_Xls_Y, IDC_Xls_Z);

			ry = ry.RotateY(v, angles.y);
			ry.PrintOutT(hDlg, IDC_Yls_X, IDC_Yls_Y, IDC_Yls_Z);

			rz = rz.RotateZ(v, angles.z);
			rz.PrintOutT(hDlg, IDC_Zls_X, IDC_Zls_Y, IDC_Zls_Z);
			
			break;

		case IDC_CLEAR:
			DlgItemClear(hDlg);
			break;

		case IDC_BTN_CLOSE:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}

BOOL CALLBACK ShortestLineToPointDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Vector3 lV, startToPtV, nlV, nStartToPtV, prjecV, shrtV;
	double dtPrdct, length, shrtLength;;


	switch (uMsg)
	{
	case WM_INITDIALOG:

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_SCALCULATE:
			lineP1.Init(hDlg, IDC_P1X, IDC_P1Y, IDC_P1Z);
			lineP2.Init(hDlg, IDC_P2X, IDC_P2Y, IDC_P2Z);
			dot.Init(hDlg, IDC_DOTX, IDC_DOTY, IDC_DOTZ);

			lV = lineP2 - lineP1;
			startToPtV = dot - lineP1;
			
			nlV = lV.NomalizeV();
			nStartToPtV = startToPtV.NomalizeV();

			dtPrdct = nlV * nStartToPtV;

			shrtV = lineP1;

			if (dtPrdct >= 0 && dtPrdct <= 1)
			{
				prjecV = startToPtV.Projection(lV);
				length = prjecV.Length();
				shrtV = lineP1 + prjecV;
			}

			else if ((dot - lineP1).Length() >= (dot - lineP2).Length())
					shrtV = lineP2;

			shrtLength = (shrtV - dot).Length();

			shrtV.PrintOutT(hDlg, IDC_SX, IDC_SY, IDC_SZ);
			PrintOut(hDlg, IDC_SD, shrtLength);

			break;

		case IDC_SCLEAR:
			DlgItemClear(hDlg);
			break;
			
		case IDCANCEL:
			lineToPoint_Active = false;
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}

BOOL CALLBACK IntersectionLineDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	
	static bool isDraw = false;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		m_interDlg = hDlg;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_DRAW:
			rectPos[0].Init(hDlg, IDC_P1X, IDC_P1Y, IDC_P1Z);
			rectPos[1].Init(hDlg, IDC_P2X, IDC_P2Y, IDC_P2Z);
			rectPos[2].Init(hDlg, IDC_P3X, IDC_P3Y, IDC_P3Z);
			rectPos[3].Init(hDlg, IDC_P4X, IDC_P4Y, IDC_P4Z);
			linePos[0].Init(hDlg, IDC_DOTX, IDC_DOTY, IDC_DOTZ);
			linePos[1].Init(hDlg, IDC_DOTX2, IDC_DOTY2, IDC_DOTZ2);

			v0v1 = rectPos[1] - rectPos[0];
			v0v3 = rectPos[3] - rectPos[0];
			lineV = linePos[1] - linePos[0];

			crossNV = v0v1.NomalizeV().CrossProduct(v0v3.NomalizeV());
			crossNV = crossNV.NomalizeV();
			
			for (int i = 0; i < 4; i++)
			{
				ipolygon[i].x = rectPos[i].x;
				ipolygon[i].y = rectPos[i].y;
				if (i < 2)
				{
					iline[i].x = linePos[i].x;
					iline[i].y = linePos[i].y;
				}
			}

			InvalidateRect(m_hwnd, NULL, true);

			//intersecPos.PrintOutT(hDlg, IDC_SX, IDC_SY, IDC_SZ);

			break;

		case IDC_PLAY:
			SetTimer(m_hwnd, 2, 16, NULL);
			break;

		case IDC_STOP:
			KillTimer(m_hwnd, 2);
			break;

		case IDC_ICLEAR:
			KillTimer(m_hwnd, 2);
			DlgItemClear(hDlg);
			break;

		case IDCANCEL:
			KillTimer(m_hwnd, 2);
			interLine = false;
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}

	return 0;
}


void SolarSystem_Init(HWND hwnd)
{
	

	SetPos(sunPoint, sizeof(sunPoint) / sizeof(POINT), sunV.x, sunV.y);
	SetRectPos(sunPoint, SUNSIZE);
	copy(sunPoint, sunPoint + sizeof(sunPoint) / sizeof(POINT), nowSunPoint);

	SetPos(earthPoint, sizeof(earthPoint) / sizeof(POINT), earthV.x, earthV.y);
	SetRectPos(earthPoint, EARTHSIZE);
	copy(earthPoint, earthPoint + sizeof(earthPoint) / sizeof(POINT), nowEarthPoint);

	SetPos(moonPoint, sizeof(moonPoint) / sizeof(POINT), moonV.x, moonV.y);
	SetElpPos(moonPoint, MOONSIZE);
	copy(moonPoint, moonPoint + sizeof(moonPoint) / sizeof(POINT), nowMoonPoint);


	SetTimer(hwnd, 0, 20, NULL);
}