#pragma once
#include "framework.h"
#include "resource.h"

#define DIALOGDISTANCE 50
#define DEFAULTOFFSET -5

class SVDialog
{
	enum SORTTYPE
	{
		SORT_TOPLEFT,
		SORT_TOPCENTER,
		SORT_TOPRIGHT,
		SORT_MAX
	};

	enum KIAITYPE
	{
		KIAI_AUTO,
		KIAI_ON,
		KIAI_OFF,
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

	}Controls, * LPControls;

public:
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
	inline int GetVolume() { return volume; }

	inline HWND GetStFileDir() { return dlg_Ctr->hstFileDir; }//Get Static control handle which contains File Directory 


private:
	HWND parentWindow;
	HWND dialogWindow;

	LPControls dlg_Ctr;

	BOOL visible;
	RECT dlgWrt, dlgCrt;//Window Rect, Client Rect
	SORTTYPE sortType;
	double startSV, endSV;
	int kiaiType, svType, volume;
	int startPos, endPos;
	BOOL volumeAuto;

	INT_PTR CALLBACK SVWProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK SVWProcWrapper(HWND, UINT, WPARAM, LPARAM);

	void SetEditVolume(HWND, int);

	void InitDialogControlHandles(LPControls&, HWND); //Init HWND values
};

