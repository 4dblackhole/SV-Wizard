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
    static TCHAR ssss[512]=_T("");

    static TCHAR fileDirectory[MAX_PATH] = _T("");

    static char* osuTXT; //most of osu files use UTF-8
    static string osuTXTtop, osuTXTbottom;

    static LineContainer qLines;
    static NoteContainer qNotes;

    switch (message)
    {
        case WM_CREATE:
        {
            Dialog.Init(IDD_SVWIZARD, hWnd);
            //Dialog.SetSortType(SVDialog::SORT_TOPLEFT);

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
                case IDM_OPEN:
                {
                    if (OpenFileDirectory(fileDirectory, Dialog) == TRUE)
                    {
                        SAFE_DELETE_ARR(osuTXT)
                        osuTXT = GetOsuFileTXT(fileDirectory);
                        InitMusicalObjects(osuTXT, qLines, qNotes);
                        SeparateOsuTXT(osuTXT, osuTXTtop, osuTXTbottom);
                    }
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
            _stprintf_s(sss, _T("Client Area : %d x %d\nMouse PT : %d %d\nKiai %d, SV %d, StartTiming %d\nVolume %d\nStart SV %g End SV %g"),
                crt.right, crt.bottom, moustPt.x, moustPt.y,
                Dialog.GetKiaiType(), Dialog.GetSVType(), Dialog.GetStartTiming(), Dialog.GetVolume(), Dialog.GetStartSV(),Dialog.GetEndSV());
            DrawText(hMemDC, sss, lstrlen(sss), &crt, DT_LEFT);

            TextOut(hMemDC, 10, 500, ssss, lstrlen(ssss));
            
            
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
            IMAGES.Release();
            Dialog.Release();
            SAFE_DELETE_ARR(osuTXT)
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

void SeparateOsuTXT(_In_ const string& txt, _Out_ string& top, _Out_ string& bottom)
{
    size_t timingPos = txt.find(timingPointIdc) + timingPointIdc.size();
    size_t hitObjPos = txt.find(hitObjectIdc);

    top = txt.substr(0, timingPos);
    bottom = txt.substr(hitObjPos, txt.size());
}

void SetMusicalLine(_In_ const string& txt, LineContainer& lines)
{
    lines.clear();
    vector<string> vLines;
    String::SplitString(&vLines, txt, "\r\n");

    vector<string> vLineFactor;
    vLineFactor.reserve(8); // Timing Point, BPM, measure, Hitsound Type, Custom No, Volume, Line Type, Kiai

    double currentBPM = 0.0;

    for (int i = 0; i < vLines.size(); i++)
    {
        String::SplitString(&vLineFactor, vLines[i], ",");
        MusicalLine tempML;
        MusicalLine_tag tempMLtag;

        // Timing Point
        tempML.SetTiming(atof(vLineFactor[0].c_str()));

        // BPM or SV
        double tempBPM = atof(vLineFactor[1].c_str());
        if (tempBPM > 0) // Red Line
        {
            tempMLtag.lineType = Line_RED;
            tempMLtag.bpm = 60000.0 / tempBPM;
        }
        else if (tempBPM < 0) // Green Line
        {
            tempMLtag.lineType = Line_GREEN;
            tempMLtag.sv = 100.0 / -tempBPM;
        }
        else // can't divide by 0
        {
            vLineFactor.clear();
            MessageBox(hRootWindow, _T("BPM or SV Value Error"), _T("alert"), MB_OK);
            return;
        }

        // Measure
        tempMLtag.measure = atoi(vLineFactor[2].c_str());

        // HitSound
        tempMLtag.hsType.sampleSet = (HitSound_SampleSet)atoi(vLineFactor[3].c_str());
        tempMLtag.hsType.idx = atoi(vLineFactor[4].c_str());

        // Volume
        tempMLtag.volume = atoi(vLineFactor[5].c_str());

        // Kiai
        tempMLtag.kiai = (BOOL)atoi(vLineFactor[7].c_str());

        // Register Information
        tempML.SetInfo(tempMLtag);

        lines.insert(make_pair(tempML.GetTiming(), tempML));
        vLineFactor.clear();
    }
}


void SetNoteType(Note& note, int flag, _Out_ BOOL& LNflag)
{
    /* each bit
    0: Hit circle
    1: Slider
    3: Spinner
    7: osu!mania hold
    The remaining bits are used for distinguishing new combos and optionally skipping combo colours (commonly called "colour hax"):
    2: New combo
    4–6: A 3-bit integer specifying how many combo colours to skip, if this object starts a new combo.
    */
    LNflag = FALSE;

    if (flag == 0 || flag == 1)note.SetNoteType(Note::TYPE_CIRCLE);
    else if ((flag & (1 << 1)) != 0)note.SetNoteType(Note::TYPE_SLIDER), LNflag = TRUE;
    else if ((flag & (1 << 3)) != 0)note.SetNoteType(Note::TYPE_SPINNER), LNflag = TRUE;
    else if ((flag & (1 << 7)) != 0)note.SetNoteType(Note::TYPE_OSUMANIA_HOLD), LNflag = TRUE;
}

void SetNoteColor(Note& note, int flag)
{
    /* each bit
    0: Normal
    1: Whistle
    2: Finish
    3: Clap
    If no bits are set, the normal hitsound is used by default.
    */

    if ((flag & (1 << 1)) != 0 || (flag & (1 << 3)) != 0)// kat check
    {
        if ((flag & (1 << 2)) != 0)   note.SetNoteColor(Note::COLOR_BIGKAT);
        else                        note.SetNoteColor(Note::COLOR_KAT);
    }
    else
    {
        if ((flag & (1 << 2)) != 0)   note.SetNoteColor(Note::COLOR_BIGDON);
        else                        note.SetNoteColor(Note::COLOR_DON);
    }
}

void SetNote(_In_ const string& txt, NoteContainer& notes)
{
    notes.clear();
    vector<string> vNotes;
    String::SplitString(&vNotes, txt, "\r\n");

    vector<string> vNoteFactor;
    vNoteFactor.reserve(11); // see https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29

    for (int i = 0; i < vNotes.size(); i++)
    {
        BOOL lncheck = FALSE;
        String::SplitString(&vNoteFactor, vNotes[i], ",");
        Note tempNote;

        // Timing
        tempNote.SetTiming(atof(vNoteFactor[2].c_str()));

        // Note Type
        SetNoteType(tempNote, atoi(vNoteFactor[3].c_str()), lncheck);

        SetNoteColor(tempNote, atoi(vNoteFactor[4].c_str()));

        if (lncheck == TRUE)
        {
            tempNote.SetEndTiming(atof(vNoteFactor[5].c_str()));

        }
        notes.insert(make_pair(tempNote.GetTiming(), tempNote));
        vNoteFactor.clear();
    }
    DEBUG;
}

void InitMusicalObjects(_In_ char* osufile, LineContainer& lines, NoteContainer& notes)
{
    string txt = osufile;

    // Init Lines =============================================================
    size_t startPoint = txt.find(timingPointIdc);
    size_t endPoint = txt.find(hitObjectIdc);

    if (startPoint == string::npos|| endPoint==string::npos)
    {
        MessageBox(hRootWindow, _T("No Indicator Detected"), _T("alert"), MB_OK);
        return;
    }

    startPoint += timingPointIdc.size(); //index of next char of indicator
    string objectTXT = txt.substr(startPoint, endPoint - startPoint); //Lines Area
    SetMusicalLine(objectTXT, lines);

    endPoint += hitObjectIdc.size();
    objectTXT = txt.substr(endPoint);
    SetNote(objectTXT, notes);
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
            else
            {
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
    assert(ReadFile(hFile, &unicode, sizeof(WORD), NULL, NULL));

    if (unicode == 0xfeff || unicode == 0xffef) // UTF 16 => FALSE
    {
        result = FALSE;
    }

    CloseHandle(hFile);

    return result;
}

char* GetOsuFileTXT(_In_ TCHAR* dir)
{
    assert(lstrlen(dir) > 0); //length check

    HANDLE hFile = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    bool isChecked = hFile != INVALID_HANDLE_VALUE;
    assert(isChecked); //load check
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    DWORD dwRead;

    char* resultTXT = new char[fileSize + 1];
    assert(ReadFile(hFile, resultTXT, fileSize, &dwRead, NULL));
    resultTXT[fileSize] = 0;

    if (hFile != NULL)
    {
        CloseHandle(hFile);
        hFile = NULL;
    }

    return resultTXT;
}