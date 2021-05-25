#include "stx.h"

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(int x, int y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}

Vector3::~Vector3()
{

}

//________________________________________________________________________________________________ 
// 
// Init은 벡터3 클래스의 초기화를 담당한다.
// 현재는 벡터3 클래스의 x, y, z 멤버의 초기화를 담당한다.
// 각각의 원소는 DlgItemInit 함수를 통해 에딧 박스의 값을 읽어오고 있다.
//________________________________________________________________________________________________ 

void Vector3::Init(HWND hDlg, INT a, INT b, INT c)
//초기화를 다이얼로그 에딧에서 가져오는 경우를 정의한다.
{
	x = DlgItemInit(hDlg, a);
	y = DlgItemInit(hDlg, b);
	z = DlgItemInit(hDlg, c);
}

void Vector3::Init(HWND hDlg, INT a, INT b)
//초기화를 다이얼로그 에딧에서 가져오는 경우를 정의한다.
{
	x = DlgItemInit(hDlg, a);
	y = DlgItemInit(hDlg, b);
	z = 0;
}

void Vector3::Init(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::Init(int x, int y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}

//________________________________________________________________________________________________ 
// 
// -PrintOut 주석-
// PrintOut은 핸들 값과 해당 클래스의 원소 x, y, z 값을 담아줄 EDIT_NAME 넘겨 받는다.
// EDIT_NAME은 define된 텍스트에 맞는 int 값을 담고 있으므로 
// 매개변수 타입은 string 타입이 아닌 INT 타입으로 받아온다.
// 값을 반환하지 않고 다이얼로그 에딧 박스에 담아주기만 한다.
// 따라서 해당 다이얼로그의 핸들값을 통해 값을 전달하는 방식을 사용한다.
//________________________________________________________________________________________________ 

void Vector3::PrintOut(HWND hDlg, INT EDIT_NAME)
{
	char str[256];

	sprintf_s(str, "(%.2f, %.2f, %.2f)", x, y, z);
	SetDlgItemText(hDlg, EDIT_NAME, str);
}

void Vector3::PrintOutT(HWND hDlg, INT EDIT_NAME, INT EDIT_NAME2, INT EDIT_NAME3)
{
	char str[256];

	sprintf_s(str, "%.2f", x);
	SetDlgItemText(hDlg, EDIT_NAME, str);

	sprintf_s(str, "%.2f", y);
	SetDlgItemText(hDlg, EDIT_NAME2, str);

	sprintf_s(str, "%.2f", z);
	SetDlgItemText(hDlg, EDIT_NAME3, str);

}

void Vector3::PrintOutT(HWND hDlg, INT EDIT_NAME, INT EDIT_NAME2)
{
	char str[256];

	sprintf_s(str, "%.2f", x);
	SetDlgItemText(hDlg, EDIT_NAME, str);

	sprintf_s(str, "%.2f", y);
	SetDlgItemText(hDlg, EDIT_NAME2, str);

}


Vector3 Vector3::VectorMulti(const Vector3 v)
{//
	Vector3 temp;

	temp.x = x * v.x;
	temp.y = y * v.y;
	temp.z = z * v.z;

	return temp;
}

//________________________________________________________________________________________________ 
//
// -두 벡터 사이의 각도 알고리즘 주석-
// v1과 v2가 단위벡터가 된다면 |v1|과 |v2| 값은 1이므로 공식이 좀 더 단순화될 수 있다.
// 해당 알고리즘에선 V1과 V2의 단위벡터를 모두 알고 있다. 따라서 단위벡터를 이용하여 계산한다.
// 인그리디언트는 두 벡터의 원소들을 각각 곱한 Vector3 값을 리턴 받는다.
// V1과 V2의 원소들의 곱셈으로 나온 인그리디언트의 원소들을 모두 더한 뒤 아크코사인으로 값을 변환하면
// θ값이 라디안으로 구해지게 된다. 우리가 원하는 값은 각도이므로
// 이후 라디안에서 디그리로 변환하기 위해 x(Radian) * 180 / PI 값을 계산해준다.
// 여기서 M_PI는 math.h에서 이미 정의된 파이 값이다.
// #define _USE_MATH_DEFINES을 math.h를 인클루드 하기 전에 선언하면
// math.h의 디파인 라이브러리를 사용할 수 있다.
//________________________________________________________________________________________________ 

double Vector3::Theta(Vector3 v)
{
	//v1과 v2가 단위벡터가 된다면 |v1|과 |v2| 값은 1이므로 공식이 좀 더 단순화될 수 있다.
	Vector3 nthis, nv;
	double temp;

	nthis = this->NomalizeV();
	nv = v.NomalizeV();
	temp = nthis * nv;

	temp = acos(temp);
	temp = temp * 180 / M_PI; //라디안을 디그리로 변환하는 과정

	return temp;
}

//________________________________________________________________________________________________ 
//
// -단위 벡터 주석-
// 지역 변수 temp를 선언해 해당 벡터3 클래스의 원소들을 노움으로 나눈 값을 기억하게 한다.
// 단위벡터 temp를 리턴한다.
//________________________________________________________________________________________________ 

Vector3 Vector3::NomalizeV(double Length)
{
	Vector3 temp;

	temp.x = x / Length;
	temp.y = y / Length;
	temp.z = z / Length;

	return temp;
}

Vector3 Vector3::NomalizeV()
{
	Vector3 temp;
	double length = x * x + y * y + z * z;
	
	length = sqrt(length);

	temp.x = x / length;
	temp.y = y / length;
	temp.z = z / length;

	return temp;
}

//________________________________________________________________________________________________ 
// 
// -벡터 덧셈 연산자 오버로딩 주석-
// 벡터3 클래스 타입의 지역 변수 temp를 선언한다.
// temp는 이항 연산자의 앞쪽의 벡터3 원소 x,y,z와 뒷쪽의 벡터3 원소 x,y,z를 각각 더한다.
// 이후엔 temp 값을 Vector3 클래스 타입으로 리턴한다.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator+(const Vector3 &v)
{ //벡터 합 연산자 오버로딩
	Vector3 temp;

	temp.x = x + v.x;
	temp.y = y + v.y;
	temp.z = z + v.z;

	return temp;
}

Vector3 Vector3::Rotate2D(double Degree, Vector3 v)
{
	double radian = Degree * M_PI / 180;
	Vector3 tmp;

	tmp.x = v.x * cos(radian) - v.y * sin(radian);
	tmp.y = v.x * sin(radian) - v.y * cos(radian);

	return tmp;
}

Vector3 Vector3::RotateX(Vector3 v, float angle)
{
	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;

	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[1][1] = cos(radian);
	t.n[1][2] = -sin(radian);
	t.n[2][1] = sin(radian);
	t.n[2][2] = cos(radian);

	tv = v * t;

	return tv;
}

Vector3 Vector3::RotateY(Vector3 v, float angle)
{

	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;


	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[0][0] = cos(radian);
	t.n[0][2] = sin(radian);
	t.n[2][0] = -sin(radian);
	t.n[2][2] = cos(radian);

	tv = v * t;

	return tv;
}

Vector3 Vector3::RotateZ(Vector3 v, float angle)
{

	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;


	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[0][0] = cos(radian);
	t.n[0][1] = -sin(radian);
	t.n[1][0] = sin(radian);
	t.n[1][1] = cos(radian);

	tv = v * t;

	return tv;
}

void Vector3::SetRotateX(float angle)
{
	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;

	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[1][1] = cos(radian);
	t.n[1][2] = -sin(radian);
	t.n[2][1] = sin(radian);
	t.n[2][2] = cos(radian);

	*this = *this * t;

}

void Vector3::SetRotateY(float angle)
{

	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;


	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[0][0] = cos(radian);
	t.n[0][2] = sin(radian);
	t.n[2][0] = -sin(radian);
	t.n[2][2] = cos(radian);

	*this = *this * t;

}

void Vector3::SetRotateZ(float angle)
{

	float radian = angle * M_PI / 180;
	Matrix3 t;
	Vector3 tv;


	for (int i = 0; i < 3; i++)
		t.n[i][i] = 1;

	t.n[0][0] = cos(radian);
	t.n[0][1] = -sin(radian);
	t.n[1][0] = sin(radian);
	t.n[1][1] = cos(radian);

	*this = *this * t;
}

//________________________________________________________________________________________________ 
//
// -벡터 뺄셈 연산자 오버로딩 주석-
// '-' 연산자의 오버로딩이다. 매개변수로 const 즉, 상수 타입의 벡터3 클래스 v의 주소값을 받는다.
// 이는 Vector3 v의 값이 레퍼런스로 넘어오기 때문에 변동될 수 있다는 뜻이므로 
// 매개변수 v값이 변하지 않는다면 const를 통해서 상수값으로 사용하는 것이 안전하다.
// Vector3 클래스 타입의 지역 변수 temp를 선언하여 각자 원소 xyz를 서로 빼준다.
// 따라서 리턴되는 Vector3 temp 값은 (V1 - V2)가 된다. 
// 만약 벡터V를 구하기 위해 매개변수 V1과 V2가 주어진다면, V2-V1이 벡터 V의 값이 된다.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator-(const Vector3& v)
{//벡터 뺄셈 연산자 오버로딩
	Vector3 temp;

	temp.x = x - v.x;
	temp.y = y - v.y;
	temp.z = z - v.z;

	return temp;
}

//________________________________________________________________________________________________ 
// 
// -벡터 곱셈 연산자 오버로딩 주석-
// 벡터3 클래스 타입의 지역 변수 temp를 선언한다.
// temp는 이항 연산자의 앞쪽에 있는 Vector3의 각 원소 x, y ,z를 뒷쪽에 오는 스칼라k와 곱한 값을 기억한다.
// 이후엔 temp 값을 Vector3 클래스 타입으로 리턴한다.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator*(const double &scalarK)
{//벡터와 스칼라 곱 연산자 오버로딩
	Vector3 temp;

	temp.x = x * scalarK;
	temp.y = y * scalarK;
	temp.z = z * scalarK;

	return temp;
}

//________________________________________________________________________________________________ 
// 
// -벡터 내적 곱셈 연산자 오버로딩 주석-
// 이전에 곱셈 오퍼레이터와는 다르게 후항의 자료형 타입이 Vector3로 다르다.
// 마찬가지로 temp의 자료형 또한 double으로 리턴 값도 double 타입이다.
// 전항 후항의 원소들을 서로 곱하여 temp에 더해준다.
// 더해준 값을 temp에 담아 리턴해준다. 이것이 V1와 V2의 내적이다.
//________________________________________________________________________________________________ 

double Vector3::operator*(const Vector3& v)
{// 벡터 곱 후 더하는 수식 연산자 오버로딩
	double temp = 0;

	temp += x * v.x;
	temp += y * v.y;
	temp += z * v.z;

	return temp;
}

Vector3 Vector3::operator*(const Matrix3& m)
{// 벡터 곱 후 더하는 수식 연산자 오버로딩
	Vector3 temp;

	temp.x = m.n[0][0] * this->x + m.n[0][1] * this->y + m.n[0][2] * this->z;
	temp.y = m.n[1][0] * this->x + m.n[1][1] * this->y + m.n[1][2] * this->z;
	temp.z = m.n[2][0] * this->x + m.n[2][1] * this->y + m.n[2][2] * this->z;

	return temp;
}


Vector3 Vector3::CrossProduct(Vector3 v)
{
	Vector3 temp;

	temp.x = y * v.z - z * v.y;
	temp.y = z * v.x - x * v.z;
	temp.z = x * v.y - y * v.x;

	return temp;
}

Vector3 Vector3::Projection(double theta, double length)
{
	double cosTheta;
	double ProjectionLength;

	cosTheta = cos(theta / 180 * M_PI);

	ProjectionLength = cosTheta * length;

	return *this * ProjectionLength;
}

Vector3 Vector3::Projection(double theta)
{
	double cosTheta;
	double ProjectionLength;
	double length = sqrt(x * x + y * y + z * z);

	cosTheta = cos(theta / 180 * M_PI);

	ProjectionLength = cosTheta * length;

	return *this * ProjectionLength;
}

Vector3 Vector3::Projection(Vector3 v)
{
	double cosTheta;
	double ProjectionLength;
	double i = this->Theta(v);
	double lengthdebug = Length();
	cosTheta = Length() * cos(this->Theta(v) * M_PI / 180);

	return v.NomalizeV() * cosTheta;
}

double Vector3::Length()
{
	return sqrt(x*x+y*y+z*z);
}