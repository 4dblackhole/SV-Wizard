#pragma once
#include "framework.h"

class Image
{
	enum IMAGE_LOAD_FROM
	{
		LOAD_FROM_RESOURCE,
		LOAD_FROM_FILE,
		LOAD_FROM_EMPTY,
		LOAD_FROM_END,
	};

	typedef struct tagImageInfo
	{
		DWORD resourceID;
		HDC hMemDC;
		HBITMAP hBit, hOldbit;
		int X, Y, W, H;

		int currentFrameX, currentFrameY; // No.X Frame and No.Y Frame (zero base)
		int maxFrameX, maxFrameY; // Maximum Frame Count (zero base)
		int frameW, frameH; // Size of Frame

		BYTE loadType;
		RECT boundingBox;

		tagImageInfo()
		{
			ZeroMemory(this, sizeof(tagImageInfo));
		}
	}IMAGE_INFO, * LPIMAGE_INFO;

private:
	LPIMAGE_INFO	imageInfo;
	TCHAR*			imageFileName;
	BOOL			isTransparent;
	COLORREF		transparentColor;

public:
	Image();
	~Image();

	//Empty Bitmap Initialize
	HRESULT Init(int w, int h);

	//Bitmap Initialize from Files
	HRESULT Init(const TCHAR* filename, int w, int h,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	HRESULT Init(const TCHAR* filename, int x, int y, int w, int h,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//Image + Frame
	HRESULT Init(const TCHAR* filename, int x, int y, int w, int h, int fx = 1, int fy = 1,
		BOOL trans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	void Release();
	void SetTransColor(BOOL trans, COLORREF transColor) { isTransparent = trans, transparentColor = transColor; }

	void Render(HDC hdc);
	void Render(HDC hdc, int dx, int dy);
	void Render(HDC hdc, int dx, int dy, int sx, int sy, int sw, int sh);

	void LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY); // Render All Screen with tiling

};