#include "MyShortCuts.h"

void ShortCut::SetClientRect(HWND hWnd, int width, int height)
{
	RECT crt;
	DWORD Style, ExStyle;

	SetRect(&crt, 0, 0, width, height);
	Style = (DWORD)GetWindowLongPtr(hWnd, GWL_STYLE);
	ExStyle = (DWORD)GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	AdjustWindowRectEx(&crt, Style, GetMenu(hWnd) != NULL, ExStyle);
	if (Style & WS_VSCROLL)crt.right += GetSystemMetrics(SM_CXVSCROLL);
	if (Style & WS_HSCROLL)crt.bottom += GetSystemMetrics(SM_CYVSCROLL);
	SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
		SWP_NOMOVE | SWP_NOZORDER);
}

inline double ShortCut::GetAngle(double x1, double y1, double x2, double y2)
{
	return atan2l(y2 - y1, x2 - x1);
}

inline double ShortCut::GetAngle(int x1, int y1, int x2, int y2)
{
	double dx1 = static_cast<double>(x1);
	double dy1 = static_cast<double>(y1);
	double dx2 = static_cast<double>(x2);
	double dy2 = static_cast<double>(y2);

	return ShortCut::GetAngle(dx1, dy1, dx2, dy2);
}

inline double ShortCut::GetAngle(const POINT& pt1, const POINT& pt2)
{
	double dx1 = static_cast<double>(pt1.x);
	double dy1 = static_cast<double>(pt1.y);
	double dx2 = static_cast<double>(pt2.x);
	double dy2 = static_cast<double>(pt2.y);

	return ShortCut::GetAngle(dx1, dy1, dx2, dy2);
}
