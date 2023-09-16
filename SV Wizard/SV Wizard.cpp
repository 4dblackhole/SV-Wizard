// SV Wizard.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "resource.h"
#include "Singletones.h"
#include "Objects/BackGround.h"
#include "Objects/MusicalLine.h"
#include "Objects/Note.h"
#include "SVDialog.h"
#include "SV Wizard.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hRootWindow;

TCHAR osuFilter[] = _T("osu Files\0*.osu\0");

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SVWProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPTSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SVWIZARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SVWIZARD));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YOUMUICON1));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_SVWIZARD);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   
	  if (!hWnd)
   {
	  return FALSE;
   }

   hRootWindow = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc, hMemDC;

	static BackGround BG;
	static SVDialog Dialog;

	static POINT moustPt = { 0,0 };
	static TCHAR sss[512];

	static TCHAR fileDirectory[MAX_PATH] = _T("");

	//static char* osuTXT; //most of osu files use UTF-8
	static string osuTXTtop, osuTXTbottom;

	static LineContainer qLines;
	static NoteContainer qNotes;

	switch (message)
	{
		case WM_CREATE:
		{
			Dialog.Init(IDD_SVWIZARD, hWnd);
			Dialog.SetSortType(SVDialog::SORT_TOPRIGHT);

			Dialog.SetLines(&qLines);
			Dialog.SetNotes(&qNotes);
			Dialog.SetTXT(&osuTXTtop, &osuTXTbottom);

			IMAGES.AddImage(_T("youmuImg"), IDB_YOUMUBG);
			IMAGES.AddImage(_T("nmImg"), IDB_NIGHTMAREBG);

			BG.SetBackGround(hWnd, IMAGES.FindImage(_T("youmuImg")));
			BG.SetBGMinX(double(Dialog.GetDialogWidth() + DIALOGDISTANCE * 2));
			BG.SetBGMinY(double(Dialog.GetDialogHeight() + DIALOGDISTANCE * 2));

		}
		break;

		case WM_WINDOWPOSCHANGING:
		{
			BG.ResizeNew(wParam, lParam);
		}
		break;

		case WM_SIZING:
		{
			BG.ResizingNew(wParam, lParam);
		}
		break;

		case WM_SIZE:
		{
			Dialog.Move();
		}
		break;

		case WM_MOVE:
		{
			Dialog.Move();
		}
		break;

		case WM_MOUSEMOVE:
		{
			moustPt.x = LOWORD(lParam);
			moustPt.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

		case WM_COMMAND:
		{
			// 메뉴 선택을 구문 분석합니다:
			switch (LOWORD(wParam))
			{
				case IDM_OPEN:
				{
					OpenFileDirectory(fileDirectory, Dialog);
				}
				break; 

				case IDM_UNDO:
				{
					tstring backupFile = fileDirectory + backupIdc;
					if (PathFileExists(backupFile.c_str()) == FALSE)
					{
						MessageBox(hWnd, _T("Directory Error"), _T("alert"), MB_OK);
						break;
					}
					if (CopyFile(backupFile.c_str(), fileDirectory, FALSE) == 0)
					{
						MessageBox(hWnd, _T("Undo Fail"), _T("alert"), MB_OK);
						break;
					}
					if (DeleteFile(backupFile.c_str()) == 0)
					{
						MessageBox(hWnd, _T("Backup File Deletion Fail"), _T("alert"), MB_OK);
						break;
					}
					MessageBeep(0);
				}
				break;

				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;

				case IDM_EXIT:
					if (MessageBox(hWnd, _T("Quit?"), _T("alert"), MB_YESNO) == IDYES) DestroyWindow(hWnd);
				break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			RECT crt;
			HBITMAP backBit, oldBackBit;
			GetClientRect(hWnd, &crt);
			int width = crt.right - crt.left;
			int height = crt.bottom - crt.top;

			hdc = BeginPaint(hWnd, &ps);
			hMemDC = CreateCompatibleDC(hdc);
			backBit = CreateCompatibleBitmap(hdc, width, height);
			oldBackBit = (HBITMAP)SelectObject(hMemDC, backBit);
			// PAINT START ======================================================

			BG.Render(hMemDC);

#ifdef _DEBUG
			_stprintf_s(sss, _T("Client Area : %d x %d\nMouse PT : %d %d\nKiai %d, SV %d, StartTiming %d\nVolume %d\nStart SV %g End SV %g"),
				crt.right, crt.bottom, moustPt.x, moustPt.y,
				Dialog.GetKiaiType(), Dialog.GetSVType(), Dialog.GetStartTiming(), Dialog.GetVolume(), Dialog.GetStartSV(),Dialog.GetEndSV());
			DrawText(hMemDC, sss, lstrlen(sss), &crt, DT_LEFT);
#endif
			// DOUBLE BUFFERING =================================================
			BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
			// PAINT END ========================================================
			SelectObject(hMemDC, oldBackBit);
			DeleteObject(backBit);
			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
		{
			IMAGES.Release();
			Dialog.Release();
			//SAFE_DELETE_ARR(osuTXT);
			PostQuitMessage(0);
		}
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OpenFileDirectory(_Out_ TCHAR* dir, SVDialog& dialog)
{
	TCHAR tempStr[300];
	OPENFILENAME OFN;

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hRootWindow;
	OFN.lpstrFilter = osuFilter;
	OFN.lpstrFile = dir;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrInitialDir = _T(".");

	if (GetOpenFileName(&OFN) != 0)
	{
		wsprintf(tempStr, _T("☆ %s ☆ has selected, Want to open?"), OFN.lpstrFile);
		if (MessageBox(hRootWindow, tempStr, _T("OPEN?"), MB_YESNO) == IDYES)
		{
			if (CheckUTF8(dir) == FALSE)
			{
				MessageBox(hRootWindow, _T("File must be UTF-8"), _T("alert"), MB_OK);
				return FALSE;
			}
			else if (CheckOsuVersion(dir) == FALSE)
			{
				MessageBox(hRootWindow, _T("osu File Version must be 14"), _T("alert"), MB_OK);
				return FALSE;
			}
			else
			{
				dialog.SetDirectory(dir);
				tstring cpy = Path::GetFileName(dir);
				SetWindowText(dialog.GetStFileDir(), cpy.c_str());
				
#define FILEBACKUP___1
#ifdef FILEBACKUP___
				//create back up file
				CopyFile(dir, (cpy + tstring(_T("_backup"))).c_str(), FALSE);
#else
#endif // FILEBACKUP___
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CheckUTF8(_In_ TCHAR* dir)
{
	BOOL result = TRUE;

	HANDLE hFile = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool isChecked = hFile != INVALID_HANDLE_VALUE;
	assert(isChecked); //load check

	WORD unicode{};
	BOOL readResult = 0;
	readResult = ReadFile(hFile, &unicode, sizeof(WORD), NULL, NULL);

	if (readResult != 0)
	{
		if (unicode == 0xfeff || unicode == 0xffef) // UTF 16 => FALSE
		{
			result = FALSE;
		}
	}

	if (hFile != NULL)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}

	return result;
}


BOOL CheckOsuVersion(_In_ TCHAR* dir)
{
	BOOL result = TRUE;

	HANDLE hFile = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool isChecked = hFile != INVALID_HANDLE_VALUE;
	assert(isChecked); //load check

	string version;
	char temp = 0;
	
	while (temp != '\n')
	{
		BOOL readResult = ReadFile(hFile, &temp, sizeof(char), NULL, NULL);
		if (readResult != 0)version += temp;
	}

	size_t locate = version.find("format v") + strlen("format v");
	string check = version.substr(locate, version.size());

	int val = atoi(check.c_str());
	if (val != 14)result = FALSE;

	if (hFile != NULL)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}

	return result;
}
