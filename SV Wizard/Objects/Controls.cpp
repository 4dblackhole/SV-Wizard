#include "Controls.h"
#include "../resource.h"

void Controls::Init(HWND hDlg)
{
    hstFileDir = GetDlgItem(hDlg, IDC_STATIC_MAPNAME);
    hbBaseBPM = GetDlgItem(hDlg, IDC_CHECK_BASEBPM);

    heStartSV = GetDlgItem(hDlg, IDC_EDIT_STARTSV);
    heEndSV = GetDlgItem(hDlg, IDC_EDIT_ENDSV);
    heStartTiming = GetDlgItem(hDlg, IDC_EDIT_STARTTIMING);
    heEndTiming = GetDlgItem(hDlg, IDC_EDIT_ENDTIMING);

    heBaseBPM = GetDlgItem(hDlg, IDC_EDIT_BASEBPM);
    heLineOffset = GetDlgItem(hDlg, IDC_EDIT_LINEOFFSET);

    heVolume = GetDlgItem(hDlg, IDC_EDIT_VOLUME);
    hslVolume = GetDlgItem(hDlg, IDC_SLIDER_VOLUME);

    // Init Spin Controls
    hspStartSV = GetDlgItem(hDlg, IDC_SPIN_STARTSV);
    hspStartSVsm = GetDlgItem(hDlg, IDC_SPIN_STARTSV_SMALL);
    hspEndSV = GetDlgItem(hDlg, IDC_SPIN_ENDSV);
    hspEndSVsm = GetDlgItem(hDlg, IDC_SPIN_ENDSV_SMALL);
}
