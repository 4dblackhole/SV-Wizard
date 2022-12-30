#pragma once
#include "../framework.h"

#define IMAGE_CHANGE_POS		(0x1 << 0)
#define IMAGE_CHANGE_SIZE		(0x1 << 1)
#define IMAGE_CHANGE_FRAME		(0x1 << 2)
#define IMAGE_CHANGE_TRANS		(0x1 << 3)
#define IMAGE_CHANGE_RESOURCE	(0x1 << 4)

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
		int targetX, targetY, origWidth, origHeight, bitmapWidth, bitmapHeight;

		int currentFrameX, currentFrameY; // No.X Frame and No.Y Frame (zero base)
		int maxFrameX, maxFrameY; // Maximum Frame Count (zero base)
		int frameW, frameH; // Size of Frame

		BYTE loadType;
		RECT drawArea;

		tagImageInfo()
		{
			ZeroMemory(this, sizeof(tagImageInfo));
		}
	}IMAGE_INFO, * LPIMAGE_INFO;

private:
	LPIMAGE_INFO	imageInfo;
	TCHAR*			imageFileName;
	BOOL			isTransparent;
	BOOL            visible;
	COLORREF		transparentColor;
	DWORD           style;

public:
	Image();
	~Image();

	//Empty Bitmap Initialize
	HRESULT Init(int w, int h);

	/*
	//Bitmap Initialize from Files
	HRESULT Init(const TCHAR* filename, int w, int h,
		BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	HRESULT Init(const TCHAR* filename, int x, int y, int w, int h,
		BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	HRESULT Init(const TCHAR* filename, int x, int y, int w, int h, int frameAmountX = 1, int frameAmountY = 1,
		BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));
		*/

	//Bitmap Initialize from ResourceID
	HRESULT Init(DWORD resourceID);
	HRESULT Init(DWORD resourceID, int x, int y);
	HRESULT Init(DWORD resourceID, int x, int y, int w, int h);
	HRESULT Init(DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY);

	HRESULT Init(DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY,
		BOOL trans, COLORREF transColor);

	void Resize(int w, int h);

	void Release();
	void SetTransColor(BOOL trans, COLORREF transColor) { isTransparent = trans, transparentColor = transColor; }

	void Render(HDC hdc);
	void Render(HDC hdc, int dx, int dy);
	void Render(HDC hdc, int dx, int dy, int sx, int sy, int sw, int sh);

	void LoopRender(HDC hdc, const LPRECT lpDrawArea, int offsetX, int offsetY); // Render All Screen with tiling

	void FrameRender(HDC hdc, int dx = 0, int dy = 0, int currentFrameX = 0, int currentFrameY = 0);

	inline DWORD GetStyle() { return style; }
	inline void SetStyle(DWORD s) { style = s; }
	inline void AddStyle(DWORD s) { style |= s; }
	inline void RemoveStyle(DWORD s) { style &= ~s; }

	inline BOOL GetVisible() { return visible; }
	inline void SetVisible(const BOOL& b) { visible = b; }

	inline int GetOrigWidth() { return imageInfo->origWidth; }
	inline int GetOrigHeight() { return imageInfo->origHeight; }

	inline int GetWidth() { return imageInfo->bitmapWidth; }
	inline int GetHeight() { return imageInfo->bitmapHeight; }

	inline int GetMaxFrameX() { return imageInfo->maxFrameX; }
	inline int GetMaxFrameY() { return imageInfo->maxFrameY; }

	inline int GetFrameX() { return imageInfo->currentFrameX; }
	inline int GetFrameY() { return imageInfo->currentFrameY; }

	inline int GetFrameWidth() { return imageInfo->frameW; }
	inline int GetFrameHeight() { return imageInfo->frameH; }

	inline RECT GetDrawArea() { return imageInfo->drawArea; }

	inline DWORD GetResourceID() { return imageInfo->resourceID; }

	inline void SetFrameX(int frameX)
	{
		imageInfo->currentFrameX = frameX;
		if (frameX > imageInfo->maxFrameX) imageInfo->currentFrameX = imageInfo->maxFrameX;
	}

	inline void SetFrameY(int frameY)
	{
		imageInfo->currentFrameY = frameY;
		if (frameY > imageInfo->maxFrameY) imageInfo->currentFrameY = imageInfo->maxFrameY;
	}
};