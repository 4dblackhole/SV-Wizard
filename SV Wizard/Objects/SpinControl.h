#pragma once
#include "../framework.h"

class SpinControl
{
public:
	SpinControl();
	~SpinControl();

	void Init(HWND, DWORD);

private:
	HWND ParentWindow;
	HWND hSpin;
	DWORD resourceID;

};