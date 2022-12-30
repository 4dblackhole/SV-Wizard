#include <cmath>
#include "MyShortCuts.h"

void ShortCut::TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm{};
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize{};

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);
	hdcSave = CreateCompatibleDC(hdc);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
	BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

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
