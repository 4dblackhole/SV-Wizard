// SV Wizard.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
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
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SVWIZARD);
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

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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

    static Image youmuImg, nmImg;
    static BackGround BG;
    static SVDialog Dialog;

    static POINT moustPt = { 0,0 };
    static TCHAR sss[512];

    static TCHAR fileDirectory[MAX_PATH] = _T("");

    switch (message)
    {
    case WM_CREATE:
        {
        Dialog.Init(IDD_SVWIZARD, hWnd);

        youmuImg.Init(IDB_YOUMUBG);
        nmImg.Init(IDB_NIGHTMAREBG);
        BG.SetBackGround(hWnd, &youmuImg);
        BG.SetBGMinX(double(Dialog.GetDialogWidth() + DIALOGDISTANCE*2));
        BG.SetBGMinY(double(Dialog.GetDialogHeight() + DIALOGDISTANCE*2));
        }
        break;

    case WM_SIZING:
        {
        BG.Resizing(wParam, lParam);
        }
        break;

    case WM_SIZE:
        {
        BG.Resize(wParam, lParam);
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
            case IDM_LOAD:
                {
                OpenFileDirectory(fileDirectory, Dialog);
                }
                break; 
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
            wsprintf(sss, _T("Client Area : %d x %d\nMouse PT : %d %d\nKiai %d, SV %d, Volume %d"),
                crt.right, crt.bottom, moustPt.x, moustPt.y, Dialog.GetKiaiType(), Dialog.GetSVType(), Dialog.GetVolume());
            DrawText(hMemDC, sss, lstrlen(sss), &crt, DT_LEFT);

            BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
            
            // PAINT END ========================================================
            SelectObject(hMemDC, oldBackBit);
            DeleteObject(backBit);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        youmuImg.Release();
        Dialog.Release();
        PostQuitMessage(0);
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

void OpenFileDirectory(_Out_ TCHAR* dir, SVDialog& dialog)
{
    TCHAR tempStr[300];
    OPENFILENAME OFN;

    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = hRootWindow;
    OFN.lpstrFilter = osuFilter;
    OFN.lpstrFile = dir;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrInitialDir = L".";

    if (GetOpenFileName(&OFN) != 0)
    {
        wsprintf(tempStr, L"☆ %s ☆ has selected, Want to open?", OFN.lpstrFile);
        if (MessageBox(hRootWindow, tempStr, L"OPEN?", MB_YESNO) == IDYES)
        {
            //TODO: send directory TCHAR to Dialog
            HWND hst = dialog.GetStFileDir();
            SetWindowText(hst, dir);
        }
    }
}