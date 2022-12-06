#include "SVDialog.h"

SVDialog::SVDialog() :parentWindow(0), dialogWindow(0), visible(TRUE)
{
}

SVDialog::~SVDialog()
{
}

void SVDialog::Init(HINSTANCE hInstance, DWORD dialogID, HWND hWndParent)
{
    parentWindow = hWndParent;
	dialogWindow = CreateDialog(hInstance, MAKEINTRESOURCE(dialogID), hWndParent, SVWProc);
    ShowWindow(dialogWindow, visible);
}

void SVDialog::Move()
{
    RECT wrt, dlgRect;
    GetWindowRect(parentWindow, &wrt);
    GetWindowRect(dialogWindow, &dlgRect);
    dlgRect.right -= dlgRect.left;
    dlgRect.bottom -= dlgRect.top;
    SetWindowPos(dialogWindow, HWND_TOP, wrt.right - dlgRect.right - 50, wrt.top + 100, 0, 0, SWP_NOSIZE);
}

INT_PTR CALLBACK SVDialog::SVWProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static PAINTSTRUCT ps;
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowLongPtr(hDlg, GWL_STYLE, 0);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_SVTYPE_EXP), BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(GetDlgItem(hDlg, IDC_VOLUME_AUTO), BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(GetDlgItem(hDlg, IDC_KIAI_AUTO), BM_SETCHECK, BST_CHECKED, 1);
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
