﻿#include "Objects/Note.h"
#include "Objects/MusicalLine.h"
#include "Singletones.h"
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

	Notes = NULL;
	Lines = NULL;
	txtTop = NULL;
	txtBottom = NULL;

	osuTXT = NULL;
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
			POINT pt{};
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
			switch (LOWORD(wParam))
			{
				case IDC_SPIN_STARTSV:
				{
					SetSVEditBySpin(dlg_Ctr->heStartSV, lParam, startSV, 0.1);
				}
				break;

				case IDC_SPIN_STARTSV_SMALL:
				{
					SetSVEditBySpin(dlg_Ctr->heStartSV, lParam, startSV, 0.01);
				}
				break;

				case IDC_SPIN_ENDSV:
				{
					SetSVEditBySpin(dlg_Ctr->heEndSV, lParam, endSV, 0.1);
				}
				break;

				case IDC_SPIN_ENDSV_SMALL:
				{
					SetSVEditBySpin(dlg_Ctr->heEndSV, lParam, endSV, 0.01);
				}
				break;

				default: break;
			}
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
					if (CheckOsuFileTXT(osuTXT, mapDirectory) == FALSE) break;

					InitMusicalObjects(osuTXT, *Lines, *Notes);
					SeparateOsuTXT(osuTXT, *txtTop, *txtBottom);
					if (Generate()        == FALSE) break;
					if (ReLoadOsuWindow() == FALSE) break;
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
		break;

		case WM_DESTROY:
		{
			SAFE_DELETE_ARR(osuTXT);
		}
		break;

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

BOOL SVDialog::Generate_ValueCheck()
{
	BOOL success = TRUE;

	// Load Path
	GetWindowText(dlg_Ctr->hstFileDir, mapDirectory, MAX_PATH);
	if (lstrcmp(mapDirectory, _T("Not Selected")) == 0)
	{
		MessageBox(dialogWindow, _T("Beatmap isn't Selected"), _T("Error"), MB_OK);
		success = FALSE;
	}

	TCHAR tempTchar[MAX_PATH];

	//Load Volume
	GetWindowText(dlg_Ctr->heVolume, tempTchar, MAX_PATH);
	volume = _ttoi(tempTchar);
	if (volume < 0 || volume>100)
	{
		MessageBox(dialogWindow, _T("Volume is out of range(0~100)"), _T("Error"), MB_OK);
		success = FALSE;
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
		success = FALSE;
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
		success = FALSE;
	}

	return success;
}

MusicalLine SVDialog::GetCurrentLineOfNote(Note* note)
{
	LineContainer::iterator result;
	result = Lines->lower_bound((note->GetTiming()));
	result--;

	return (result->second);
}

BOOL SVDialog::Generate()
{
	BOOL rs = FileBackUp(mapDirectory);
	if (startTiming > endTiming) return FALSE; //No Area Detected
	if (Notes->empty() || Lines->empty()) return FALSE; // Check Container
	if (Generate_ValueCheck() == FALSE) return FALSE; // Failure of Initialize

	NoteContainer::iterator startNote, endNote;

	startNote = Notes->lower_bound((double)startTiming);
	endNote = Notes->upper_bound((double)endTiming);

	if (startNote == endNote) return FALSE; // No Note detected


	double startpos = startNote->first;
	double endpos = (--endNote)->first;
	++endNote;

	//LineContainer tempLines;

	LineContainer::iterator currentLine, nextLine;
	currentLine = Lines->lower_bound(startNote->first + lineOffset);
	nextLine = currentLine--;

	if (currentLine == Lines->end()) // can't insert any green lines in front of first red line
	{
		startNote++;
	}

	for (NoteContainer::iterator it = startNote; it != endNote; it++)
	{
		while (nextLine != Lines->end() && it->first + (double)lineOffset >= nextLine->first)
		{
			MoveIteratorToNext(currentLine, nextLine);
		}

		MusicalLine tLine;
		MusicalLine_tag tLinetag;

		tLine.SetTiming(it->first + (double)lineOffset);

		tLinetag.lineType = Line_GREEN;
		tLinetag.bpm = currentLine->second.GetInfo().bpm;
		tLinetag.measure = currentLine->second.GetInfo().measure;
		tLinetag.hsType = currentLine->second.GetInfo().hsType;

		GenerateSV(startpos, endpos, it->first, currentLine, tLinetag.sv);
		GenerateVolume(currentLine, tLinetag.volume);
		GenerateKiai(currentLine, tLinetag.kiai);

		tLine.SetInfo(tLinetag);

		Lines->insert(make_pair(tLine.GetTiming(), tLine));
	}

	//insert red lines' timings into timing list
	if (lineOffset != 0)
	{
		LineContainer::iterator startLine, endLine;
		startLine = Lines->lower_bound((double)startTiming);
		endLine = Lines->upper_bound((double)endTiming);

		for (LineContainer::iterator it = startLine; it != endLine; it++)
		{
			MusicalLine tLine;
			MusicalLine_tag tLinetag;

			if (it->second.GetInfo().lineType == Line_RED)
			{

				tLine.SetTiming(it->first);

				tLinetag.lineType = Line_GREEN;
				tLinetag.bpm = it->second.GetInfo().bpm;
				tLinetag.measure = it->second.GetInfo().measure;
				tLinetag.hsType = it->second.GetInfo().hsType;

				GenerateSV(startpos, endpos, it->first, it, tLinetag.sv);
				GenerateVolume(it, tLinetag.volume);
				GenerateKiai(it, tLinetag.kiai);

				tLine.SetInfo(tLinetag);

				Lines->insert(make_pair(tLine.GetTiming(), tLine));
			}
		}
	}
	GenerateLineText(*Lines);

	return TRUE;
}

BOOL SVDialog::GenerateSV(double startpos, double endpos, double timing, LineContainer::iterator currentLine, _Out_ double& sv)
{
	BOOL result = TRUE;

	switch (svType)
	{
	case SV_LINEAR:
		sv = SHORTCUT.Interpolation_Linear(startpos, endpos, startSV, endSV, timing);
		break;

	case SV_EXP:
		sv = SHORTCUT.Interpolation_Exponential(startpos, endpos, startSV, endSV, timing);
		break;

	case SV_FOCUS:
		sv = SHORTCUT.Interpolation_Harmonic(startpos, endpos, startSV, endSV, timing);
		break;

	default:
		sv = 1.0;
		result = FALSE;
		MessageBox(dialogWindow, _T("SV Type Error"), _T("alert"), MB_OK);
		break;
	}

	if (baseBPMenable == TRUE)
	{
		if (baseBPM == 0.0)return FALSE;
		sv *= (baseBPM / currentLine->second.GetInfo().bpm);
	}

	return result;
}

BOOL SVDialog::GenerateVolume(LineContainer::iterator it, _Out_ int& vol)
{
	BOOL result = TRUE;

	if (volumeAuto == TRUE) // Get Current Volume
	{
		vol = it->second.GetInfo().volume;
	}
	else
	{
		vol = SVDialog::volume; // Change Volume you set
	}

	return result;
}


BOOL SVDialog::GenerateKiai(LineContainer::iterator it, _Out_ BOOL& kiai)
{
	BOOL result = TRUE;

	kiai = FALSE;

	switch (kiaiType)
	{
		case KIAI_AUTO:
			kiai = it->second.GetInfo().kiai;
			break;

		case KIAI_ON:
			kiai = TRUE;
			break;

		case KIAI_OFF:
			kiai = FALSE;
			break;

		default:
			result = FALSE;
			break;
	}

	return result;
}

static BOOL CALLBACK enumChild(HWND hWnd, LPARAM lParam)
{
	static vector<HWND>* handles = reinterpret_cast<vector<HWND>*>(lParam);
		
	static TCHAR str[256];
	GetWindowText(hWnd, str, 256);
	handles->push_back(hWnd);

	return TRUE;
}

//TODO: Auto Reload
BOOL SVDialog::ReLoadOsuWindow()
{
	BOOL result = TRUE;
	tstring windowName = _T("osu!  - ");
	TCHAR mapName[MAX_PATH];
	GetWindowText(dlg_Ctr->hstFileDir, mapName, MAX_PATH);

	windowName += mapName;

	HWND hOsu = FindWindow(NULL, windowName.c_str());
   
	if (hOsu == NULL)return FALSE;

	//RECT rp;
	//SHORTCUT.SetClientRect(hOsu, 1300, 600);
	SetFocus(hOsu);
	
	//vector<HWND> Childs;
	//EnumChildWindows(hOsu, enumChild, reinterpret_cast<LPARAM>(&Childs));

	PostMessage(hOsu, WM_KEYDOWN, 'V', NULL);
	PostMessage(hOsu, WM_KEYUP, 'V', NULL);

	return result;
}

BOOL SVDialog::FileBackUp(TCHAR* dir)
{
	//checkbox Check
	BOOL backup = FALSE;
	if (SendMessage(dlg_Ctr->hbBackUp, BM_GETCHECK, 0, 0) == BST_CHECKED)backup = TRUE;

	BOOL result = TRUE; 
	if (backup == TRUE)result = CopyFile(dir, (Path::GetFileName(dir) + tstring(_T("_backup"))).c_str(), FALSE);

	return result;
}

void SVDialog::GenerateLineText(LineContainer& lines)
{
	HANDLE file = CreateFile(mapDirectory, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwWrite = 0;

	WriteFile(file, txtTop->c_str(), (DWORD)txtTop->size(), &dwWrite, NULL);

	for (LineContainer::iterator it = lines.begin(); it != lines.end(); it++)
	{
		string linetxt = string();
		linetxt.reserve(50000);
		LineToText(it->second, linetxt);
		WriteFile(file, linetxt.c_str(), (DWORD)linetxt.size(), &dwWrite, NULL);
	}

	WriteFile(file, txtBottom->c_str(), (DWORD)txtBottom->size(), &dwWrite, NULL);
	CloseHandle(file);
}

void SVDialog::LineToText(MusicalLine& line,_Out_ string& txt)
{
	char c[99] = { 0 };
	
	double bpmORsv = 1.0;
	int linecolor = 0;

	if (line.GetInfo().lineType == Line_GREEN)
	{
		bpmORsv = -100.0 / line.GetInfo().sv;
	}
	else if (line.GetInfo().lineType == Line_RED)
	{
		bpmORsv = 60000.0 / line.GetInfo().bpm;
		linecolor = 1;
	}

	MusicalLine_tag&& info = (line.GetInfo());

	sprintf_s(c, "%.20g,%.20g,%d,%d,%d,%d,%d,%d\r\n",
		line.GetTiming(), bpmORsv, info.measure,
		info.hsType.sampleSet, info.hsType.idx,
		info.volume, linecolor, (int)info.kiai);

	txt = c;
}

void SVDialog::MoveIteratorToNext(LineContainer::iterator& current, LineContainer::iterator& next)
{
	current = next++;
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

	dlg_Ctr->hbBackUp = GetDlgItem(hDlg, IDC_CHECK_BACKUP);

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
	_stprintf_s(t, _T("%g"), startSV);
	SetWindowText(dlg_Ctr->heStartSV, t);
	_stprintf_s(t, _T("%g"), endSV);
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

void SVDialog::SetSVEditBySpin(HWND editCtr, LPARAM lParam, double& target, double delta)
{
	LPNMUPDOWN nmud = (LPNMUPDOWN)lParam;
	TCHAR tt[16] = { 0 };
	double tempsv;
	GetWindowText(editCtr, tt, 16);
	tempsv = _ttof(tt);
	if (tempsv == 0.0)return;
	tempsv += delta * (-(nmud->iDelta));
	target = tempsv;
	_stprintf_s(tt, _T("%g"), tempsv);
	SetWindowText(editCtr, tt);
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

	double currentBPM = 180.0;

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
			currentBPM = 60000.0 / tempBPM;
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
		tempMLtag.bpm = currentBPM;

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

	if (startPoint == string::npos || endPoint == string::npos)
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


char* ReadOsuFileTXT(_In_ TCHAR* dir)
{
	if (lstrlen(dir) < 1)return NULL; //length check

	HANDLE hFile = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool isChecked = hFile != INVALID_HANDLE_VALUE;
	assert(isChecked); //load check

	DWORD fileSize = GetFileSize(hFile, NULL);
	DWORD dwRead = 0;
	BOOL readResult = 0;
	char* resultTXT = new char[fileSize + 1];
	readResult = ReadFile(hFile, resultTXT, fileSize, &dwRead, NULL);
	if (readResult != 0)
	{
		resultTXT[fileSize] = 0;
	}
	else
	{
		MessageBox(hRootWindow, _T("osu File Road Failure"), _T("alert"), MB_OK);
		SAFE_DELETE_ARR(resultTXT);
	}

	if (hFile != NULL)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}
	
	return resultTXT;
}

BOOL CheckOsuFileTXT(_In_ char* &txt, _In_ TCHAR* dir)
{
	SAFE_DELETE_ARR(txt);
	txt = ReadOsuFileTXT(dir);
	if (txt == NULL)
	{
		MessageBox(hRootWindow, _T("No File Selected"), _T("alert"), MB_OK);
		return FALSE;
	}
	return TRUE;
}
