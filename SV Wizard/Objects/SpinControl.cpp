#include "SpinControl.h"

SpinControl::SpinControl()
{
	ParentWindow = 0;
	hSpin = 0;
	resourceID = 0;
}

SpinControl::~SpinControl()
{
}

void SpinControl::Init(HWND pWindow, DWORD resourceID)
{
	this->ParentWindow = pWindow;
	hSpin = GetDlgItem(pWindow, resourceID);
}
