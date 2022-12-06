#pragma once
#include "framework.h"
#include "resource.h"

class SVDialog
{
public:
	SVDialog();
	~SVDialog();

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

	enum VOLUMETYPE
	{
		VOLUME_AUTO,
		VOLUME_CHANGE,
		VOLUME_MAX
	};

	void Init(HINSTANCE hInstance, DWORD dialogID, HWND hWndParent);
	void Move();

	inline BOOL GetVisible() { return visible; }

private:
	HWND parentWindow;
	HWND dialogWindow;
	BOOL visible;

	static INT_PTR CALLBACK SVWProc(HWND, UINT, WPARAM, LPARAM);
};

