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
// Init�� ����3 Ŭ������ �ʱ�ȭ�� ����Ѵ�.
// ����� ����3 Ŭ������ x, y, z ����� �ʱ�ȭ�� ����Ѵ�.
// ������ ���Ҵ� DlgItemInit �Լ��� ���� ���� �ڽ��� ���� �о���� �ִ�.
//________________________________________________________________________________________________ 

void Vector3::Init(HWND hDlg, INT a, INT b, INT c)
//�ʱ�ȭ�� ���̾�α� �������� �������� ��츦 �����Ѵ�.
{
	x = DlgItemInit(hDlg, a);
	y = DlgItemInit(hDlg, b);
	z = DlgItemInit(hDlg, c);
}

void Vector3::Init(HWND hDlg, INT a, INT b)
//�ʱ�ȭ�� ���̾�α� �������� �������� ��츦 �����Ѵ�.
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
// -PrintOut �ּ�-
// PrintOut�� �ڵ� ���� �ش� Ŭ������ ���� x, y, z ���� ����� EDIT_NAME �Ѱ� �޴´�.
// EDIT_NAME�� define�� �ؽ�Ʈ�� �´� int ���� ��� �����Ƿ� 
// �Ű����� Ÿ���� string Ÿ���� �ƴ� INT Ÿ������ �޾ƿ´�.
// ���� ��ȯ���� �ʰ� ���̾�α� ���� �ڽ��� ����ֱ⸸ �Ѵ�.
// ���� �ش� ���̾�α��� �ڵ鰪�� ���� ���� �����ϴ� ����� ����Ѵ�.
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
// -�� ���� ������ ���� �˰��� �ּ�-
// v1�� v2�� �������Ͱ� �ȴٸ� |v1|�� |v2| ���� 1�̹Ƿ� ������ �� �� �ܼ�ȭ�� �� �ִ�.
// �ش� �˰��򿡼� V1�� V2�� �������͸� ��� �˰� �ִ�. ���� �������͸� �̿��Ͽ� ����Ѵ�.
// �α׸����Ʈ�� �� ������ ���ҵ��� ���� ���� Vector3 ���� ���� �޴´�.
// V1�� V2�� ���ҵ��� �������� ���� �α׸����Ʈ�� ���ҵ��� ��� ���� �� ��ũ�ڻ������� ���� ��ȯ�ϸ�
// �谪�� �������� �������� �ȴ�. �츮�� ���ϴ� ���� �����̹Ƿ�
// ���� ���ȿ��� ��׸��� ��ȯ�ϱ� ���� x(Radian) * 180 / PI ���� ������ش�.
// ���⼭ M_PI�� math.h���� �̹� ���ǵ� ���� ���̴�.
// #define _USE_MATH_DEFINES�� math.h�� ��Ŭ��� �ϱ� ���� �����ϸ�
// math.h�� ������ ���̺귯���� ����� �� �ִ�.
//________________________________________________________________________________________________ 

double Vector3::Theta(Vector3 v)
{
	//v1�� v2�� �������Ͱ� �ȴٸ� |v1|�� |v2| ���� 1�̹Ƿ� ������ �� �� �ܼ�ȭ�� �� �ִ�.
	Vector3 nthis, nv;
	double temp;

	nthis = this->NomalizeV();
	nv = v.NomalizeV();
	temp = nthis * nv;

	temp = acos(temp);
	temp = temp * 180 / M_PI; //������ ��׸��� ��ȯ�ϴ� ����

	return temp;
}

//________________________________________________________________________________________________ 
//
// -���� ���� �ּ�-
// ���� ���� temp�� ������ �ش� ����3 Ŭ������ ���ҵ��� ������� ���� ���� ����ϰ� �Ѵ�.
// �������� temp�� �����Ѵ�.
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
// -���� ���� ������ �����ε� �ּ�-
// ����3 Ŭ���� Ÿ���� ���� ���� temp�� �����Ѵ�.
// temp�� ���� �������� ������ ����3 ���� x,y,z�� ������ ����3 ���� x,y,z�� ���� ���Ѵ�.
// ���Ŀ� temp ���� Vector3 Ŭ���� Ÿ������ �����Ѵ�.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator+(const Vector3 &v)
{ //���� �� ������ �����ε�
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
// -���� ���� ������ �����ε� �ּ�-
// '-' �������� �����ε��̴�. �Ű������� const ��, ��� Ÿ���� ����3 Ŭ���� v�� �ּҰ��� �޴´�.
// �̴� Vector3 v�� ���� ���۷����� �Ѿ���� ������ ������ �� �ִٴ� ���̹Ƿ� 
// �Ű����� v���� ������ �ʴ´ٸ� const�� ���ؼ� ��������� ����ϴ� ���� �����ϴ�.
// Vector3 Ŭ���� Ÿ���� ���� ���� temp�� �����Ͽ� ���� ���� xyz�� ���� ���ش�.
// ���� ���ϵǴ� Vector3 temp ���� (V1 - V2)�� �ȴ�. 
// ���� ����V�� ���ϱ� ���� �Ű����� V1�� V2�� �־����ٸ�, V2-V1�� ���� V�� ���� �ȴ�.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator-(const Vector3& v)
{//���� ���� ������ �����ε�
	Vector3 temp;

	temp.x = x - v.x;
	temp.y = y - v.y;
	temp.z = z - v.z;

	return temp;
}

//________________________________________________________________________________________________ 
// 
// -���� ���� ������ �����ε� �ּ�-
// ����3 Ŭ���� Ÿ���� ���� ���� temp�� �����Ѵ�.
// temp�� ���� �������� ���ʿ� �ִ� Vector3�� �� ���� x, y ,z�� ���ʿ� ���� ��Į��k�� ���� ���� ����Ѵ�.
// ���Ŀ� temp ���� Vector3 Ŭ���� Ÿ������ �����Ѵ�.
//________________________________________________________________________________________________ 

Vector3 Vector3::operator*(const double &scalarK)
{//���Ϳ� ��Į�� �� ������ �����ε�
	Vector3 temp;

	temp.x = x * scalarK;
	temp.y = y * scalarK;
	temp.z = z * scalarK;

	return temp;
}

//________________________________________________________________________________________________ 
// 
// -���� ���� ���� ������ �����ε� �ּ�-
// ������ ���� ���۷����Ϳʹ� �ٸ��� ������ �ڷ��� Ÿ���� Vector3�� �ٸ���.
// ���������� temp�� �ڷ��� ���� double���� ���� ���� double Ÿ���̴�.
// ���� ������ ���ҵ��� ���� ���Ͽ� temp�� �����ش�.
// ������ ���� temp�� ��� �������ش�. �̰��� V1�� V2�� �����̴�.
//________________________________________________________________________________________________ 

double Vector3::operator*(const Vector3& v)
{// ���� �� �� ���ϴ� ���� ������ �����ε�
	double temp = 0;

	temp += x * v.x;
	temp += y * v.y;
	temp += z * v.z;

	return temp;
}

Vector3 Vector3::operator*(const Matrix3& m)
{// ���� �� �� ���ϴ� ���� ������ �����ε�
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