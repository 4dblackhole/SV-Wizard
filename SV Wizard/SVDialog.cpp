#include "SVDialog.h"

SVDialog::SVDialog()
{
    parentWindow = 0;
    dialogWindow = 0;
    visible = TRUE;

    dlgWrt = { 0 }, dlgCrt = { 0 };
    startSV = endSV = baseBPM = 1.0L;
    kiaiType = KIAI_AUTO;
    svType = SV_EXP;
    volume = 100;
    volumeAuto = TRUE;
    baseBPMenable = FALSE;
    sortType = SORT_TOPRIGHT;

    startTiming = endTiming = 0;
    lineOffset = LINEOFFSET_DEFAULTPOS;
    dlg_Ctr = NULL;
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
    
    int x{};
    int y= wrt.top - crt.top + DIALOGDISTANCE;
    switch (sortType)
    {
    case SORT_TOPLEFT:
        x = wrt.left - crt.left + DIALOGDISTANCE;
        //y = wrt.top - crt.top + DIALOGDISTANCE;
        break;

    case SORT_TOPCENTER:
        x = (wrt.left + wrt.right) / 2 - GetDialogWidth() / 2;
        //y = wrt.top - crt.top + DIALOGDISTANCE;
        break;

    case SORT_TOPRIGHT:
        x = wrt.right + crt.left - DIALOGDISTANCE - GetDialogWidth();
        //y = wrt.top - crt.top + DIALOGDISTANCE;
        break;

    default: break;
    }

    SetWindowPos(dialogWindow, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

void SVDialog::Release()
{
    SAFE_DELETE(dlg_Ctr);
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
            InitDialogControlHandles(dlg_Ctr, hDlg);
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

        case WM_NOTIFY:
        {
            LPNMHDR ncode = (LPNMHDR)lParam;
            switch (ncode->code)
            {
                case UDN_DELTAPOS:
                {
                    LPNMUPDOWN nmud = (LPNMUPDOWN)lParam;
                    switch (nmud->hdr.idFrom)
                    {
                        case IDC_SPIN_STARTTIMING:
                        case IDC_SPIN_ENDTIMING:
                        {
                            nmud->iDelta *= TIMING_SPINUNIT;
                        }
                        break;
                        case IDC_SPIN_VOLUME:
                        {
                            nmud->iDelta *= VOLUME_SPINUNIT;
                        }
                        break;
                    }

                }
                break;
            }        
        }
        break;

        case WM_HSCROLL:
        {
            if ((HWND)lParam == dlg_Ctr->hslVolume)
            {
                volume = (int)SendMessage(dlg_Ctr->hslVolume, TBM_GETPOS, 0, 0);
                SetEditVolume(dlg_Ctr->heVolume, volume);
            }
        }
        break;

        case WM_COMMAND:
        {
        switch (HIWORD(wParam))
        {
            case EN_KILLFOCUS:
            {
                switch (LOWORD(wParam))
                {
                    TCHAR tt[16];

                    case IDC_EDIT_STARTSV:
                    {
                        GetWindowText(dlg_Ctr->heStartSV, tt, 16);
                        double tv = _ttof(tt);
                        if (tv != 0)startSV = tv;
                        else
                        {
                            startSV = 1;
                            _stprintf_s(tt, _T("%.lf"), startSV);
                            SetWindowText(dlg_Ctr->heStartSV, tt);
                        }
                    }
                    break;

                    case IDC_EDIT_ENDSV:
                    {
                        GetWindowText(dlg_Ctr->heStartSV, tt, 16);
                        double tv = _ttof(tt);
                        if (tv != 0)endSV = tv;
                        else
                        {
                            endSV = 1;
                            _stprintf_s(tt, _T("%.lf"), endSV);
                            SetWindowText(dlg_Ctr->heEndSV, tt);

                        }
                    }
                    break;

                    case IDC_EDIT_STARTTIMING:
                    {
                        GetWindowText(dlg_Ctr->heStartTiming, tt, 16);
                        int tv = _ttoi(tt);
                        startTiming = tv;
                    }
                    break;

                    case IDC_EDIT_ENDTIMING:
                    {
                        GetWindowText(dlg_Ctr->heEndTiming, tt, 16);
                        int tv = _ttoi(tt);
                        endTiming = tv;
                    }
                    break;

                    case IDC_EDIT_BASEBPM:
                    {
                        GetWindowText(dlg_Ctr->heBaseBPM, tt, 16);
                        double tv = _ttof(tt);
                        if (tv != 0)baseBPM = tv;
                        else
                        {
                            baseBPM = 180;
                            _stprintf_s(tt, _T("%.lf"), baseBPM);
                            SetWindowText(dlg_Ctr->heBaseBPM, tt);

                        }
                    }
                    break;

                    case IDC_EDIT_VOLUME:
                    {
                        GetWindowText(dlg_Ctr->heVolume, tt, 16);
                        int tv = _ttoi(tt);

                        if (tv < VOLUMEMIN || tv > VOLUMEMAX) SetEditVolume(dlg_Ctr->heVolume, tv);

                        SendMessage(dlg_Ctr->hslVolume, TBM_SETPOS, TRUE, tv);
                        volume = (int)SendMessage(dlg_Ctr->hslVolume, TBM_GETPOS, 0, 0);

                    }
                    break;
                    default:
                    break;
                }
            }
            break;
            default:
            break;
        }

        switch (LOWORD(wParam))
        {
            case IDC_GENERATE:
            {
                Generate_ValueSetting();
            }
            break;

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

void SVDialog::AdjustIntRange(_Inout_ int& v, _In_ int min, _In_ int max)
{
    if (v < min)v = min;
    else if (v > max)v = max;
}

void SVDialog::SetEditInt(HWND h, int v)
{
    TCHAR t[15];
    _stprintf_s(t, _T("%d"), v);
    SetWindowText(h, t);
}

void SVDialog::SetEditVolume(HWND hVolume, int v)
{
    AdjustIntRange(v, VOLUMEMIN, VOLUMEMAX);
    SetEditInt(hVolume, v);
}

void SVDialog::Generate_ValueSetting()
{
    // Load Path
    GetWindowText(dlg_Ctr->hstFileDir, mapDirectory, MAX_PATH);
    if (lstrcmp(mapDirectory, _T("Not Selected")) == 0)
    {
        MessageBox(dialogWindow, _T("Beatmap isn't Selected"), _T("Error"), MB_OK);

    }

    TCHAR tempTchar[MAX_PATH];

    //Load Volume
    GetWindowText(dlg_Ctr->heVolume, tempTchar, MAX_PATH);
    volume = _ttoi(tempTchar);
    if (volume < 0 || volume>100)
    {
        MessageBox(dialogWindow, _T("Volume is out of range(0~100)"), _T("Error"), MB_OK);

    }

    //Load Timing Points
    GetWindowText(dlg_Ctr->heStartTiming, tempTchar, MAX_PATH);
    startTiming = _ttoi(tempTchar);
    GetWindowText(dlg_Ctr->heEndTiming, tempTchar, MAX_PATH);
    endTiming = _ttoi(tempTchar);

    //Load SV Range
    GetWindowText(dlg_Ctr->heStartSV, tempTchar, MAX_PATH);
    startSV = _ttof(tempTchar);
    GetWindowText(dlg_Ctr->heEndSV, tempTchar, MAX_PATH);
    endSV = _ttof(tempTchar);
    if (startSV == 0 || endSV == 0)
    {
        MessageBox(dialogWindow, _T("SV value is Zero"), _T("Error"), MB_OK);
    }

    //Load Line Offset
    GetWindowText(dlg_Ctr->heLineOffset, tempTchar, MAX_PATH);
    lineOffset = _ttoi(tempTchar);

    //Load Base BPM
    if (SendMessage(dlg_Ctr->hbBaseBPM, BM_GETCHECK, 0, 0) == BST_CHECKED)baseBPMenable = TRUE;
    else if (SendMessage(dlg_Ctr->hbBaseBPM, BM_GETCHECK, 0, 0) == BST_UNCHECKED)baseBPMenable = FALSE;

    GetWindowText(dlg_Ctr->heBaseBPM, tempTchar, MAX_PATH);
    baseBPM = _ttof(tempTchar);
    if (baseBPMenable == TRUE && baseBPM == 0)
    {
        MessageBox(dialogWindow, _T("BPM is Zero"), _T("Error"), MB_OK);
    }
}

void SVDialog::InitDialogControlHandles(LPControls& dlg_Ctr, HWND hDlg)
{
    TCHAR t[128];

    // window handle Init
    SAFE_DELETE(dlg_Ctr);
    dlg_Ctr = new Controls;

    dlg_Ctr->hstFileDir = GetDlgItem(hDlg, IDC_STATIC_MAPNAME);
    dlg_Ctr->hbBaseBPM = GetDlgItem(hDlg, IDC_CHECK_BASEBPM);

    dlg_Ctr->heStartSV = GetDlgItem(hDlg, IDC_EDIT_STARTSV);
    dlg_Ctr->heEndSV = GetDlgItem(hDlg, IDC_EDIT_ENDSV);
    dlg_Ctr->heStartTiming = GetDlgItem(hDlg, IDC_EDIT_STARTTIMING);
    dlg_Ctr->heEndTiming = GetDlgItem(hDlg, IDC_EDIT_ENDTIMING);

    dlg_Ctr->heBaseBPM = GetDlgItem(hDlg, IDC_EDIT_BASEBPM);
    dlg_Ctr->heLineOffset = GetDlgItem(hDlg, IDC_EDIT_LINEOFFSET);

    dlg_Ctr->heVolume = GetDlgItem(hDlg, IDC_EDIT_VOLUME);
    dlg_Ctr->hslVolume = GetDlgItem(hDlg, IDC_SLIDER_VOLUME);

    // Init Spin Controls
    dlg_Ctr->hspStartSV = GetDlgItem(hDlg, IDC_SPIN_STARTSV);
    dlg_Ctr->hspStartSVsm = GetDlgItem(hDlg, IDC_SPIN_STARTSV_SMALL);
    dlg_Ctr->hspEndSV = GetDlgItem(hDlg, IDC_SPIN_ENDSV);
    dlg_Ctr->hspEndSVsm = GetDlgItem(hDlg, IDC_SPIN_ENDSV_SMALL);

    dlg_Ctr->hspStartTiming = GetDlgItem(hDlg, IDC_SPIN_STARTTIMING);
    dlg_Ctr->hspStartTimingsm = GetDlgItem(hDlg, IDC_SPIN_STARTTIMING_SMALL);
    dlg_Ctr->hspEndTiming = GetDlgItem(hDlg, IDC_SPIN_ENDTIMING);
    dlg_Ctr->hspEndTimingsm = GetDlgItem(hDlg, IDC_SPIN_ENDTIMING_SMALL);

    dlg_Ctr->hspVolume = GetDlgItem(hDlg, IDC_SPIN_VOLUME);
    dlg_Ctr->hspVolumesm = GetDlgItem(hDlg, IDC_SPIN_VOLUME_SMALL);
    dlg_Ctr->hspLineOffset = GetDlgItem(hDlg, IDC_SPIN_LINEOFFSET);


    SetWindowLongPtr(hDlg, GWL_STYLE, 0);

    SendMessage(GetDlgItem(hDlg, IDC_RADIO_SVTYPE_EXP), BM_SETCHECK, BST_CHECKED, 1);
    SendMessage(GetDlgItem(hDlg, IDC_VOLUME_AUTO), BM_SETCHECK, BST_CHECKED, 1);
    SendMessage(GetDlgItem(hDlg, IDC_KIAI_AUTO), BM_SETCHECK, BST_CHECKED, 1);

    //sv
    _stprintf_s(t, _T("%.lf"), startSV);
    SetWindowText(dlg_Ctr->heStartSV, t);
    _stprintf_s(t, _T("%.lf"), endSV);
    SetWindowText(dlg_Ctr->heEndSV, t);

    _stprintf_s(t, _T("%d"), startTiming);
    SetWindowText(dlg_Ctr->heStartTiming, t);
    _stprintf_s(t, _T("%d"), endTiming);
    SetWindowText(dlg_Ctr->heEndTiming, t);

    //volume
    SetEditVolume(dlg_Ctr->heVolume, volume);
    SendMessage(dlg_Ctr->hslVolume, TBM_SETRANGE, TRUE, MAKELPARAM(VOLUMEMIN, VOLUMEMAX));
    SendMessage(dlg_Ctr->hslVolume, TBM_SETPOS, TRUE, volume);
    SendMessage(dlg_Ctr->hslVolume, TBM_SETPAGESIZE, 0, VOLUME_SLIDERUNIT);

    //line offset
    _stprintf_s(t, _T("%d"), LINEOFFSET_DEFAULTPOS);
    SetWindowText(dlg_Ctr->heLineOffset, t);

    //spin
    SendMessage(dlg_Ctr->hspStartSV,   UDM_SETBUDDY, (WPARAM)dlg_Ctr->heStartSV, 0);
    SendMessage(dlg_Ctr->hspStartSVsm, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heStartSV, 0);

    SendMessage(dlg_Ctr->hspEndSV,   UDM_SETBUDDY, (WPARAM)dlg_Ctr->heEndSV, 0);
    SendMessage(dlg_Ctr->hspEndSVsm, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heEndSV, 0);

    SendMessage(dlg_Ctr->hspStartTiming,   UDM_SETBUDDY, (WPARAM)dlg_Ctr->heStartTiming, 0);
    SendMessage(dlg_Ctr->hspStartTimingsm, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heStartTiming, 0);

    SendMessage(dlg_Ctr->hspEndTiming,   UDM_SETBUDDY, (WPARAM)dlg_Ctr->heEndTiming, 0);
    SendMessage(dlg_Ctr->hspEndTimingsm, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heEndTiming, 0);

    HWND* timingSpins[4] = { &dlg_Ctr->hspStartTiming,&dlg_Ctr->hspStartTimingsm,&dlg_Ctr->hspEndTiming,&dlg_Ctr->hspEndTimingsm };
    for (int i = 0; i < 4; i++)SendMessage(*(timingSpins[i]), UDM_SETRANGE32, 0, INT_MAX - 11);

    SendMessage(dlg_Ctr->hspVolume,   UDM_SETBUDDY, (WPARAM)dlg_Ctr->heVolume, 0);
    SendMessage(dlg_Ctr->hspVolumesm, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heVolume, 0);
    SendMessage(dlg_Ctr->hspLineOffset, UDM_SETBUDDY, (WPARAM)dlg_Ctr->heLineOffset, 0);

    SendMessage(dlg_Ctr->hspVolume, UDM_SETRANGE, 0, MAKELPARAM(VOLUMEMAX, VOLUMEMIN));
    SendMessage(dlg_Ctr->hspVolumesm, UDM_SETRANGE, 0, MAKELPARAM(VOLUMEMAX, VOLUMEMIN));
    SendMessage(dlg_Ctr->hspLineOffset, UDM_SETRANGE, 0, MAKELPARAM(25, -25));

}
