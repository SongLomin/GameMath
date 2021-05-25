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

// 최단거리

static Vector3 lineP1, lineP2;
static Vector3 dot;

// 교점
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
	WndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(120,230,230));
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name",
		"Window Title Name",
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
				//SetTimer(hwnd, 2, 16, NULL); //직선을 움직이게 함
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
			

			hBrush = CreateSolidBrush(RGB(200, 60, 60));//컬러
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Polygon(hdc, nowSunPoint, sizeof(nowSunPoint) / sizeof(POINT));

			DeleteObject(SelectObject(hdc, hOldBrush));

			hBrush = CreateSolidBrush(RGB(60, 60, 200));//컬러
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Polygon(hdc, nowEarthPoint, sizeof(earthPoint) / sizeof(POINT));

			DeleteObject(SelectObject(hdc, hOldBrush));

			hBrush = CreateSolidBrush(RGB(230, 230, 230));//컬러
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, nowMoonPoint[0].x, nowMoonPoint[0].y, nowMoonPoint[1].x, nowMoonPoint[1].y);
			DeleteObject(SelectObject(hdc, hOldBrush));


			
		}

		if (lineToPoint_Active)
		{
			///평면 좌표
			MoveToEx(hdc, -3000, 0, NULL);
			LineTo(hdc, 3000, 0);
			MoveToEx(hdc, 0, 3000, NULL);
			LineTo(hdc, 0, -3000);
			///직선

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

			AllPosRotation_Z(&nowEarthV, 1, sunV, 2 * timerCount); //현재 지구의 중점을 찾아줌

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
			else //교점
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

	Vector3 Vsum; //벡터 합의 정답
	Vector3 V; //벡터 V2-V1의 정답
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
		//다이얼로그 에딧 컨트롤 0으로 초기화
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			
		case IDC_BTN_CALCULATE:

			//________________________________________________________________________________________________ 
			//
			//	resource.h의 Define 값이 연속된다면 반복문을 통해 효율적으로 값을 읽어올 수 있다.
			//
			// for (int i = 0; i < 6; i++)
			// {
			// 	 Vectors[i] = DlgItemInit(hDlg, IDC_EDIT_V1_X+i);
			// }
			// ScalarK = DlgItemInit(hDlg,IDC_EDIT_SCALARK);
			//________________________________________________________________________________________________ 

			//________________________________________________________________________________________________ 
			//
			// 벡터 값을 대입하는 부분이다. Init의 매개변수로 에딧 박스를 읽어올 다이얼로그 핸들 값과 
			// 벡터 원소 x, y, z 세 곳에 넣어줄 에딧 박스의 Define값을 보내준다.
			//________________________________________________________________________________________________ 
			V1.Init(hDlg, IDC_EDIT_V1_X, IDC_EDIT_V1_X2, IDC_EDIT_V1_X3);
			V2.Init(hDlg, IDC_EDIT_V2_X, IDC_EDIT_V2_X2, IDC_EDIT_V2_X3);

			ScalarK = DlgItemInit(hDlg, IDC_EDIT_SCALAR_K);

			
			//________________________________________________________________________________________________ 
			//
			// -Vector V (V2 - V1)을 구하는 알고리즘-
			// 벡터V2와 벡터V1을 이용해 벡터 V를 얻어내는 식이다. 
			// 사용자 정의 클래스 벡터3타입은 연산자로 연산이 불가능하지만 연산자 오버로딩을 통해
			// 클래스끼리의 덧셈 연산이 가능한 것처럼 보이게 만든다.
			// 연산자 오버로딩 오퍼레이터+의 정의로 이동해보자. <(V2 - V1)를 통해 정의 이동>
			// 이후 Vector3의 멤버 함수 PrintOut를 통해 값을 전달한다.
			// 매개변수로 에디트 박스 전달에 필요한 다이얼로그의 핸들 값과 에디트 박스의 이름 번지수를 넘겨준다.
			//________________________________________________________________________________________________ 

			V = V2 - V1;
			V.PrintOut(hDlg, IDC_EDIT_V);


			//________________________________________________________________________________________________ 
			//
			// -벡터 V의 노옴 구하는 알고리즘-
			// V의 노옴 공식은 √(x^2+y^2+z^2)이다. 우리가 직접 계산식을 짤 수도 있지만
			// math.h에서 루트 연산 라이브러리를 제공하고 있다. 
			// 따라서 연산자 오버로딩을 통해 원소들을 서로 곱하고 더한 뒤
			// math.h에 포함된 라이브러리 함수 sqrt()를 사용해 루트를 Length에게 적용한다.
			// 이후 위와 동일하게 에디트 박스에 출력시킨다.
			// 하지만 Length는 double 타입이기 때문에 Vector3의 멤버 함수인 PrintOut을 사용할 수 없다.
			// 따라서 동일한 이름의 전역 함수 PrintOut을 통해 매개변수로 포함시켜 넘겨준다.
			//________________________________________________________________________________________________ 

			Length = sqrt(V * V);
			PrintOut(hDlg, IDC_EDIT_LENGTH, Length);
			

			//________________________________________________________________________________________________
			//
			// -벡터 V의 단위벡터 구하는 알고리즘-
			// 단위벡터 함수를 정의하는데엔 두 가지 방식이 있다. 벡터의 크기(노옴) 값을 매개변수로 넘겨주거나,
			// 매개변수 없이 해당 벡터의 원소들로 노옴을 구해서 단위벡터를 구할 수 있다.
			// 이 알고리즘에서는 에딧 박스에 들어가는 계산 결과들을 기억하고 있다.
			// 따라서 이미 위에서 구한 노옴 값을 매개변수로 추가적인 연산 없이 벡터 V의 단위벡터를 구할 수 있다.
			// 단위벡터는 각 원소에 노움 값으로 나누면 벡터의 크기가 1인 벡터, 단위벡터가 된다.
			//________________________________________________________________________________________________ 

			NomalV = V.NomalizeV(Length);
			NomalV.PrintOut(hDlg, IDC_EDIT_NOMALV);


			//________________________________________________________________________________________________
			//
			// -벡터 V와 스칼라 K의 곱을 구하는 알고리즘-
			// 해당 식 또한 연산자 오버로딩을 통해 해결한다.
			// 정의로 이동해보자.
			//________________________________________________________________________________________________

			ScalarKV = V * ScalarK;
			ScalarKV.PrintOut(hDlg, IDC_EDIT_SCALAR_KV);


			//________________________________________________________________________________________________
			//
			// -벡터 V1과 벡터 V2의 합을 구하는 알고리즘-
			// 해당 식 또한 연산자 오버로딩을 통해 해결한다.
			// 정의로 이동해보자.
			//________________________________________________________________________________________________
			Vsum = V1 + V2;
			Vsum.PrintOut(hDlg, IDC_EDIT_ADD);


			//________________________________________________________________________________________________
			//
			// -벡터 V1 - V2를 구하는 알고리즘-
			// 벡터 V를 구하는 공식과는 반대로 이항 연산자의 앞쪽과 뒤가 바뀌어 있다.
			// 같은 식이므로 넘겨 주는 값을 반대로 넘겨주어 계산한다.
			//________________________________________________________________________________________________

			V1subV2 = V1 - V2;
			V1subV2.PrintOut(hDlg, IDC_EDIT_SUB);


			//________________________________________________________________________________________________
			//
			// -벡터 V1의 단위벡터를 구하는 알고리즘-
			// 매개변수로 단위벡터의 노움을 매개변수로 넘겨주어 계산한다. 벡터 V의 단위벡터를 구할 때와 동일하지만
			// V1의 노움을 계산한 적이 없기 때문에 연산자 오버로딩과 math.h에 포함된 sqrt 함수를 이용하여 노움을 구한다.
			//________________________________________________________________________________________________
			
			NomalV1 = V1.NomalizeV(sqrt(V1 * V1));

			NomalV1.PrintOut(hDlg, IDC_EDIT_NORMALV1);


			//________________________________________________________________________________________________
			//
			// -벡터 V2의 단위벡터를 구하는 알고리즘-
			// 위와 동일하다. 마찬가지로 노움을 계산해서 매개변수로 보내준다.
			//________________________________________________________________________________________________

			NomalV2 = V2.NomalizeV(sqrt(V2 * V2));

			NomalV2.PrintOut(hDlg, IDC_EDIT_NORMALV2);


			//________________________________________________________________________________________________
			//
			// -단위벡터 V1과 단위벡터 V2의 내적을 구하는 알고리즘-
			// 마찬가지로 연산자 오버로딩을 이용한다. 하지만 이전의 곱셈 연산자 오버로딩과는 다르다.
			// 이전의 곱셈 연산자 오버로딩은 리턴 타입이 벡터3에 전항이 벡터3, 후항이 double 타입이었다면
			// 해당 곱셈은 리턴 타입이 double이고 이항이 벡터3 타입이다.
			// 따라서 같은 곱셈이라도 다른 Oprator 함수에 들어가므로 다른 연산이다.
			//________________________________________________________________________________________________

			DotP = NomalV1 * NomalV2;

			PrintOut(hDlg, IDC_EDIT_DOTPROV, DotP);

			//________________________________________________________________________________________________
			//
			// -두 벡터가 이루는 각도를 구하는 알고리즘-
			// V1과 V2의 θ를 구하는 공식은 V1과 V2의 원소를 모두 곱한 다음 더한다.
			// 이후 아크코사인을 사용해 세타 값을 알아낼 수 있다. math.h에선 아크코사인에 대한 라이브러리 함수를 지원한다.
			// 정의에서 확인해보자
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
			//다이얼로그 에딧 컨트롤 0으로 초기화

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