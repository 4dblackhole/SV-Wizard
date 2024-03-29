﻿// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <Shlwapi.h>
// C 런타임 헤더 파일입니다.
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <commctrl.h>
#include <commdlg.h>
#include <cstdio>
#include <functional>
#include <malloc.h>
#include <map>
#include <memory.h>
#include <queue>
#include <set>
#include <stack>
#include <stdlib.h>
#include <string>
#include <tchar.h>

#include <iostream>
using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define SAFE_DELETE(p)		{ if(p) {delete (p); (p) = NULL; }}
#define SAFE_DELETE_ARR(p)	{ if(p) {delete[] (p); (p) = NULL; }}

#include "Utilities/Path.h"
#include "Utilities/String.h"

extern HWND hRootWindow; //Handle of Main Window
extern HINSTANCE hInst;
extern TCHAR osuFilter[];

static tstring backupIdc = _T("_backup");

static string timingPointIdc = "[TimingPoints]\r\n";
static string hitObjectIdc = "[HitObjects]\r\n";

#define DEBUG int aijewfwefsadf = 1

