#pragma once
#include "framework.h"
#include "resource.h"

#define DIALOGDISTANCE 50

#define LINEOFFSET_DEFAULTPOS -5

#define VOLUMEMAX 100
#define VOLUMEMIN 0
#define VOLUME_SPINUNIT 5
#define VOLUME_SLIDERUNIT 10

#define TIMING_SPINUNIT 10

class SVDialog
{
public:
	enum SORTTYPE
	{
		SORT_TOPLEFT,
		SORT_TOPCENTER,
		SORT_TOPRIGHT,
		SORT_MAX
	};

	enum KIAITYPE
	{
		KIAI_OFF,
		KIAI_ON,
		KIAI_AUTO,
		KIAI_MAX
	};

	enum SVTYPE
	{
		SV_LINEAR,
		SV_EXP,
		SV_FOCUS,
		SV_MAX
	};

	typedef struct Controls
	{
		HWND heStartSV, heEndSV, heStartTiming, heEndTiming, heBaseBPM, heLineOffset, heVolume;
		HWND hstFileDir;
		HWND hbBaseBPM;
		HWND hslVolume;
		HWND hspStartSV, hspStartSVsm, hspEndSV, hspEndSVsm, hspStartTiming, hspStartTimingsm, hspEndTiming, hspEndTimingsm;
		HWND hspVolume, hspVolumesm, hspLineOffset;
		HWND hbBackUp;

	}Controls, * LPControls;

	SVDialog();
	~SVDialog();

	void Init(DWORD dialogID, HWND hWndParent);
	void Move();

	void Release();

	inline HWND GetWindow() { return dialogWindow; }

	inline BOOL GetVisible() { return visible; }
	inline int GetDialogWidth() { return (dlgWrt.right - dlgWrt.left); }
	inline int GetDialogHeight() { return (dlgWrt.bottom - dlgWrt.top); }

	inline SORTTYPE GetSortType() { return sortType; }
	inline void SetSortType(SORTTYPE a) { sortType = a; }

	inline int GetKiaiType() { return kiaiType; }
	inline int GetSVType() { return svType; }
	inline int GetStartTiming() { return startTiming; }
	inline int GetVolume() { return volume; }
	inline double GetStartSV() { return startSV; }
	inline double GetEndSV() { return endSV; }

	inline HWND GetStFileDir() { return dlg_Ctr->hstFileDir; }//Get Static control handle which contains File Directory 

	void SetNotes(NoteContainer* t) { Notes = t; }
	void SetLines(LineContainer* t) { Lines = t; }
	void SetTXT(string* top, string* bottom) { txtTop = top, txtBottom = bottom; }
	void SetDirectory(TCHAR* t) 
	{
		_stprintf_s(mapDirectory, _T("%s"), t);

	}

private:
	HWND parentWindow;
	HWND dialogWindow;

	LPControls dlg_Ctr;

	BOOL visible;
	RECT dlgWrt, dlgCrt;//Window Rect, Client Rect

	SORTTYPE sortType;
	double startSV, endSV;
	double baseBPM;
	int kiaiType, svType, volume;
	int startTiming, endTiming, lineOffset;
	BOOL volumeAuto, baseBPMenable;
	TCHAR mapDirectory[MAX_PATH]{};

	char* osuTXT; //most of osu files use UTF-8

	NoteContainer* Notes;
	LineContainer* Lines;

	string* txtTop, * txtBottom;

	INT_PTR CALLBACK SVWProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK SVWProcWrapper(HWND, UINT, WPARAM, LPARAM);

	void AdjustIntRange(_Inout_ int&, _In_ int, _In_ int);
	void SetEditInt(HWND, int);
	void SetEditVolume(HWND, int);

	MusicalLine GetCurrentLineOfNote(Note*);

	BOOL Generate_ValueCheck();
	BOOL Generate();
	BOOL GenerateSV(double, double, double, LineContainer::iterator, _Out_ double&);
	BOOL GenerateVolume(LineContainer::iterator, _Out_ int&);
	BOOL GenerateKiai(LineContainer::iterator, _Out_ BOOL&);
	BOOL ReLoadOsuWindow();

	BOOL FileBackUp(TCHAR*);

	void GenerateLineText(LineContainer&);
	void LineToText(MusicalLine&, _Out_ string&);

	void InitDialogControlHandles(LPControls&, HWND); //Init HWND values
	void SetSVEditBySpin(HWND, LPARAM, double&, double);

	void MoveIteratorToNext(LineContainer::iterator&, LineContainer::iterator&);
};


void InitMusicalObjects(_In_ char* osufile, LineContainer& lines, NoteContainer& notes);

void SetMusicalLine(_In_ const string& txt, LineContainer& lines);
BOOL SetMusicalLineTiming(MusicalLine&, _In_ const string&, LineContainer&);
void SeparateOsuTXT(_In_ const string& txt, _Out_ string& top, _Out_ string& bottom);
void SetNote(_In_ const string& txt, NoteContainer& notes);
char* ReadOsuFileTXT(_In_ TCHAR*);
BOOL CheckOsuFileTXT(_In_ char*&, _In_ TCHAR*);