#pragma once
#include "framework.h"
#include "resource.h"

#define DIALOGDISTANCE 50

class SVDialog
{
public:
	SVDialog();
	~SVDialog();

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

	void Init(DWORD dialogID, HWND hWndParent);
	void Move();

	void Release();

	inline BOOL GetVisible() { return visible; }
	inline int GetDialogWidth() { return (dlgWrt.right - dlgWrt.left); }
	inline int GetDialogHeight() { return (dlgWrt.bottom - dlgWrt.top); }

	inline SORTTYPE GetSortType() { return sortType; }
	inline void SetSortType(SORTTYPE a) { sortType = a; }

private:
	HWND parentWindow;
	HWND dialogWindow;
	BOOL visible;

	RECT dlgWrt, dlgCrt;
	SORTTYPE sortType;
	double startSV, endSV;
	int kiaiType, svType, volume;
	int startPos, endPos;
	BOOL volumeAuto;

	INT_PTR CALLBACK SVWProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK SVWProcWrapper(HWND, UINT, WPARAM, LPARAM);
};

