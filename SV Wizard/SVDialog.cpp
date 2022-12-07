#include "SVDialog.h"

SVDialog::SVDialog()
{
    parentWindow = 0;
    dialogWindow = 0;
    visible = TRUE;

    dlgWrt = { 0 }, dlgCrt = { 0 };
    startSV = endSV = 1.0L;
    kiaiType = KIAI_AUTO;
    svType = SV_EXP;
    volume = 100;
    volumeAuto = TRUE;
    sortType = SORT_TOPRIGHT;

    startPos = endPos = 0;

}

SVDialog::~SVDialog()
{
}

void SVDialog::Init(DWORD dialogID, HWND hWndParent)
{
    parentWindow = hWndParent;
    dialogWindow = CreateDialogParam(hInst, MAKEINTRESOURCE(dialogID), hWndParent, SVWProcWrapper, reinterpret_cast<LPARAM>(this));
    GetWindowRect(dialogWindow, &dlgWrt);
    GetClientRect(dialogWindow, &dlgCrt);

    ShowWindow(dialogWindow, visible);
}

void SVDialog::Move()
{
    RECT wrt;
    GetWindowRect(parentWindow, &wrt);

    RECT crt;
    DWORD Style, ExStyle;

    SetRect(&crt, 0, 0, 100, 100);

    Style = (DWORD)GetWindowLongPtr(parentWindow, GWL_STYLE);
    ExStyle = (DWORD)GetWindowLongPtr(parentWindow, GWL_EXSTYLE);
    AdjustWindowRectEx(&crt, Style, GetMenu(parentWindow) != NULL, ExStyle);
    if (Style & WS_VSCROLL)crt.right += GetSystemMetrics(SM_CXVSCROLL);
    if (Style & WS_HSCROLL)crt.bottom += GetSystemMetrics(SM_CYVSCROLL);
    
    switch (sortType)
    {
    case SORT_TOPLEFT:
        SetWindowPos(dialogWindow, HWND_TOP, wrt.left - crt.left + DIALOGDISTANCE, wrt.top - crt.top + DIALOGDISTANCE, 
            0, 0, SWP_NOSIZE | SWP_NOZORDER);
        break;

    case SORT_TOPCENTER:
        SetWindowPos(dialogWindow, HWND_TOP, (wrt.left+wrt.right)/2 - GetDialogWidth()/2 , wrt.top - crt.top + DIALOGDISTANCE,
            0, 0, SWP_NOSIZE | SWP_NOZORDER);
        break;

    case SORT_TOPRIGHT:
        SetWindowPos(dialogWindow, HWND_TOP, wrt.right + crt.left - DIALOGDISTANCE - GetDialogWidth(), wrt.top - crt.top + DIALOGDISTANCE,
            0, 0, SWP_NOSIZE | SWP_NOZORDER);
        break;

    default: break;
    }

}

void SVDialog::Release()
{
}

INT_PTR CALLBACK SVDialog::SVWProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static PAINTSTRUCT ps;

    static HMENU hMenu, hPopup;

    static TCHAR t[128];

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
        SetWindowLongPtr(hDlg, GWL_STYLE, 0);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_SVTYPE_EXP), BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(GetDlgItem(hDlg, IDC_VOLUME_AUTO), BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(GetDlgItem(hDlg, IDC_KIAI_AUTO), BM_SETCHECK, BST_CHECKED, 1);

        _stprintf_s(t, _T("%.lf"), startSV);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT_STARTSV), t);

        _stprintf_s(t, _T("%.lf"), endSV);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT_ENDSV), t);

        _stprintf_s(t, _T("%d"), volume);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT_VOLUME), t);
        }
        return (INT_PTR)TRUE;

    case WM_CONTEXTMENU:
        {
        POINT pt;
        pt.x = (short)LOWORD(lParam);
        pt.y = (short)HIWORD(lParam);

        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_SVWIAZRDLOCATE));
        hPopup = GetSubMenu(hMenu, 0);

        TrackPopupMenu(hPopup, TPM_LEFTALIGN, pt.x, pt.y, 0, hDlg, NULL);

        DestroyMenu(hMenu);
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_RADIO_SVTYPE_LINEAR: svType = SV_LINEAR; break;
        case IDC_RADIO_SVTYPE_EXP: svType = SV_EXP; break;
        case IDC_RADIO_SVTYPE_FOCUS: svType = SV_FOCUS; break;

        case IDC_KIAI_AUTO: kiaiType = KIAI_AUTO; break;
        case IDC_KIAI_ON: kiaiType = KIAI_ON; break;
        case IDC_KIAI_OFF: kiaiType = KIAI_OFF; break;

        case IDC_VOLUME_AUTO: volumeAuto = TRUE; break;
        case IDC_VOLUME_CHANGE: volumeAuto = FALSE; break;

        case ID_LOCATE_TOPLEFT: 
            {
            sortType = SORT_TOPLEFT; 
            Move();
            }
            break;
        case ID_LOCATE_TOPCENTER: 
            {
            sortType = SORT_TOPCENTER;
            Move();
            }
            break;
        case ID_LOCATE_TOPRIGHT: 
            {
            sortType = SORT_TOPRIGHT;
            Move();
            }
            break;
        case IDOK:
        case IDCANCEL:
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;

        default:
            break;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR SVDialog::SVWProcWrapper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    SVDialog* cls;

    if (WM_INITDIALOG == message)
    {
        cls = reinterpret_cast<SVDialog*>(lParam);
        SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cls));
    }
    else cls = reinterpret_cast<SVDialog*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));

    if (cls) return cls->SVWProc(hDlg, message, wParam, lParam);

    return FALSE;
}