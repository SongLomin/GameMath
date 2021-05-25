#pragma once
#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <algorithm>

#include "resource.h" 
#define _USE_MATH_DEFINES //math.h�� ���� �Ǿ��ִ� ������ ��, DEFINE ���̺귯���� ���
#include <math.h>
#include "totalFunction.h"
#include "Vector3.h"
#include "Matrix3.h"

using namespace std;


#define SUNSIZE		20
#define EARTHSIZE	10
#define MOONSIZE	5

#define SUNPOS_X	0
#define SUNPOS_Y	0
#define EARTHPOS_X	60
#define EARTHPOS_Y	60
#define MOONPOS_X	EARTHPOS_X+30
#define MOONPOS_Y	EARTHPOS_Y+30

//________________________________________________________________________________________________ 
// 
// ��� ���� ����
// Vector3.h���� ���� �Լ� ����
// main�� �ִ� �Լ��� Vector3���� ����ϱ� ���� #include "main.cpp"�� ���־���.
//________________________________________________________________________________________________ 