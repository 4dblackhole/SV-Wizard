#pragma once
#include "SingletonBase.h"
#include "Image.h"
#include <unordered_map>

class ImageManager : public SingletonBase<ImageManager>
{
private:
	typedef unordered_map<tstring, Image*>				ImageList;
	typedef unordered_map<tstring, Image*>::iterator	ImageListIter;

	ImageList imageList;

public:
	void Init();
	void Release();

	Image* FindImage(tstring);

	Image* AddImage(tstring, int, int);
	Image* AddImage(tstring, DWORD);
	Image* AddImage(tstring, DWORD, int x, int y);
	Image* AddImage(tstring, DWORD, int x, int y, int w, int h);
	Image* AddImage(tstring, DWORD, int x, int y, int w, int h, int frameAmountX, int frameAmountY);
	Image* AddImage(tstring, DWORD, int x, int y, int w, int h, int frameAmountX, int frameAmountY,
		BOOL trans, COLORREF transColor);

	void Render(tstring, HDC);
	void Render(tstring, HDC, int dx, int dy);
	void Render(tstring, HDC, int dx, int dy, int sx, int sy, int sw, int sh);

	void LoopRender(tstring key, HDC hdc, const LPRECT lpDrawArea, int offsetX, int offsetY);

	void FrameRender(tstring key, HDC hdc, int dx = 0, int dy = 0, int currentFrameX = 0, int currentFrameY = 0);

	BOOL DeleteImage(tstring);
	BOOL DeleteALL();
};