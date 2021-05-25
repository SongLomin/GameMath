#pragma once
#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <algorithm>

#include "resource.h" 
#define _USE_MATH_DEFINES //math.h에 정의 되어있는 수학적 값, DEFINE 라이브러리를 사용
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
// 헤더 파일 관리
// Vector3.h에서 벡터 함수 정의
// main에 있는 함수를 Vector3에서 사용하기 위해 #include "main.cpp"를 해주었다.
//________________________________________________________________________________________________ 