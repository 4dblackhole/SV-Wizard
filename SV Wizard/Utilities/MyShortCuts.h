#pragma once
#include "../framework.h"

class ShortCut
{
public:
	static ShortCut& GetInstance()
	{
		static ShortCut s;
		return s;
	}

	void SetClientRect(HWND hWnd, int width, int height);
	void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);

	inline double GetAngle(double x1, double y1, double x2, double y2);
	inline double GetAngle(int x1, int y1, int x2, int y2);
	inline double GetAngle(const POINT& pt1, const POINT& pt2);

private:
	ShortCut() {};
	~ShortCut() {};
	ShortCut(const ShortCut& ref) { }
	ShortCut& operator=(const ShortCut& ref) { }
};