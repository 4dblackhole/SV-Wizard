#include "Image.h"
#pragma comment(lib, "msimg32.lib")

Image::Image()
	:imageInfo(NULL), imageFileName(NULL), isTransparent(FALSE), visible(TRUE), transparentColor(RGB(0, 0, 0)), style(IMAGE_CHANGE_RESOURCE)
{
}

Image::~Image()
{
}

//Empty Bitmap
HRESULT Image::Init(int w, int h)
{
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(hRootWindow);

	imageInfo = new IMAGE_INFO;
	imageInfo->loadType = LOAD_FROM_EMPTY;
	imageInfo->resourceID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);  //empty DC
	imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, w, h);
	imageInfo->hOldbit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->bitmapWidth = w;
	imageInfo->bitmapHeight = h;
	imageInfo->drawArea = { 0,0,w,h };

	if (imageInfo->hBit == NULL)
	{
		Release();
		return E_FAIL;
	}

	ReleaseDC(hRootWindow, hdc);

	return S_OK;
}

HRESULT Image::Init(DWORD resourceID)
{
	return Image::Init(resourceID, 0, 0, 0, 0, 1, 1, FALSE, RGB(255, 0, 255));
}

HRESULT Image::Init(DWORD resourceID, int x, int y)
{
	return Image::Init(resourceID, x, y, 0, 0, 1, 1, FALSE, RGB(255, 0, 255));
}

HRESULT Image::Init(DWORD resourceID, int x, int y, int w, int h)
{
	return Image::Init(resourceID, x, y, w, h, 1, 1, FALSE, RGB(255, 0, 255));
}

HRESULT Image::Init(DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY)
{
	return Image::Init(resourceID, x, y, w, h, frameAmountX, frameAmountY, FALSE, RGB(255, 0, 255));
}

HRESULT Image::Init(DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY, BOOL trans, COLORREF transColor)
{
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(hRootWindow);

	imageInfo = new IMAGE_INFO;
	imageInfo->loadType = LOAD_FROM_RESOURCE;
	imageInfo->resourceID = resourceID;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);  //empty DC

	if ((style & IMAGE_CHANGE_POS) == 0) x = 0, y = 0;
	
	if ((style & IMAGE_CHANGE_SIZE) != 0)
	{
		imageInfo->hBit = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(resourceID), IMAGE_BITMAP, w, h, LR_DEFAULTCOLOR);
	}
	else
	{
		imageInfo->hBit = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(resourceID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_DEFAULTCOLOR);
		BITMAP tagBitmap;
		GetObject(imageInfo->hBit, sizeof(BITMAP), &tagBitmap);
		w = tagBitmap.bmWidth;
		h = tagBitmap.bmHeight;
	}
	imageInfo->hOldbit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);

	if ((style & IMAGE_CHANGE_FRAME) == 0)frameAmountX = 1, frameAmountY = 1;

	if ((style & IMAGE_CHANGE_TRANS) == 0) trans = FALSE, transColor = RGB(255, 0, 255);

	imageInfo->targetX = x;
	imageInfo->targetY = y;
	imageInfo->bitmapWidth = w;
	imageInfo->bitmapHeight = h;
	imageInfo->frameW = w / frameAmountX;
	imageInfo->frameH = h / frameAmountY;
	imageInfo->currentFrameX = 0;
	imageInfo->currentFrameY = 0;
	imageInfo->maxFrameX = frameAmountX - 1;
	imageInfo->maxFrameY = frameAmountY - 1;
	imageInfo->drawArea = { x, y, x + imageInfo->frameW, y + imageInfo->frameH };

	isTransparent = trans;
	transparentColor = transColor;

	if (imageInfo->hBit == NULL)
	{
		Release();
		return E_FAIL;
	}

	ReleaseDC(hRootWindow, hdc);

	return S_OK;
}

void Image::Resize(int w, int h)
{
	if (imageInfo == NULL) return;

	SelectObject(imageInfo->hMemDC, imageInfo->hOldbit);
	DeleteObject(imageInfo->hBit);

	imageInfo->hBit = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(imageInfo->resourceID), IMAGE_BITMAP, w, h, LR_DEFAULTCOLOR);

	imageInfo->hOldbit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);

	imageInfo->bitmapWidth = w;
	imageInfo->bitmapHeight = h;
	imageInfo->frameW = w / (imageInfo->maxFrameX + 1);
	imageInfo->frameH = h / (imageInfo->maxFrameY + 1);
}

void Image::Release()
{
	if (imageInfo != NULL)
	{
		SelectObject(imageInfo->hMemDC, imageInfo->hOldbit);
		DeleteObject(imageInfo->hBit);
		DeleteDC(imageInfo->hMemDC);

		SAFE_DELETE(imageInfo);
		SAFE_DELETE(imageFileName);

		isTransparent = FALSE;
		transparentColor = RGB(255, 0, 255);
	}
}

void Image::Render(HDC hdc)
{
	Render(hdc, imageInfo->targetX, imageInfo->targetY, 0, 0, imageInfo->bitmapWidth, imageInfo->bitmapHeight);
}

void Image::Render(HDC hdc, int dx, int dy)
{
	Render(hdc, dx, dy, 0, 0, imageInfo->bitmapWidth, imageInfo->bitmapHeight);
}

void Image::Render(HDC hdc, int dx, int dy, int sx, int sy, int sw, int sh)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,					//복사될 영역DC
			dx,					//복사될 좌표 X
			dy,					//복사될 좌표 Y
			sw,				//복사될 가로크기
			sh,				//복사될 세로크기
			imageInfo->hMemDC,
			sx, sy,			//복사해올 좌표
			sw,				//복사해올 가로크기
			sh,				//복사해올 세로크기
			transparentColor);
	}
	else
	{
		BitBlt(hdc, dx, dy,
			sw,
			sh,
			imageInfo->hMemDC,
			sx, sy,
			SRCCOPY);
	}
}

void Image::LoopRender(HDC hdc, const LPRECT lpDrawArea, int offsetX, int offsetY)
{

	if (offsetX < 0)offsetX = imageInfo->bitmapWidth +  (offsetX % imageInfo->bitmapWidth);
	if (offsetY < 0)offsetY = imageInfo->bitmapHeight + (offsetY % imageInfo->bitmapHeight);

	//오프셋 영역을 받아올 변수
	int sourWidth;
	int sourHeight;
	//그려주는 영역
	RECT rcDest;
	RECT rcSour;

	//그려주는 영역을 잡아준다
	int drawAreaX = lpDrawArea->left;
	int drawAreaY = lpDrawArea->top;
	int drawAreaW = lpDrawArea->right - drawAreaX;      //너비
	int drawAreaH = lpDrawArea->bottom - drawAreaY;     //높이

	//Y축 부터
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offsetY) % imageInfo->bitmapHeight;
		rcSour.bottom = imageInfo->bitmapHeight;

		sourHeight = rcSour.bottom - rcSour.top;

		//그려주는 영역의 높이보다 y축이 크면
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//화면밖으로 나간영역만큼 뿌려줄 위치에 산정한다
		//뿌려주는 영역
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;


		//x축
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offsetX) % imageInfo->bitmapWidth;
			rcSour.right = imageInfo->bitmapWidth;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			Render(hdc, rcDest.left, rcDest.top,//뿌려줄 위치 X, Y
				rcSour.left, rcSour.top,		//뿌려올 위치
				rcSour.right - rcSour.left,		//가로크기
				rcSour.bottom - rcSour.top);	//세로크기
		}
	}
}

void Image::FrameRender(HDC hdc, int dx, int dy, int currentFrameX, int currentFrameY)
{
	imageInfo->currentFrameX = currentFrameX;
	imageInfo->currentFrameY = currentFrameY;

	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,								//복사될 영역의 DC
			dx,								//복사될 좌표 X
			dy,								//복사될 좌표 Y
			imageInfo->frameW,				//복사될 크기(가로)
			imageInfo->frameH,			//복사될 크기(세로)
			imageInfo->hMemDC,					//복사해올 DC
			currentFrameX * imageInfo->frameW,
			currentFrameY * imageInfo->frameH,	//복사해올 좌표X,Y
			imageInfo->frameW,				//복사해올 크기
			imageInfo->frameH,			//복사해올 크기
			transparentColor);						//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
		BitBlt(hdc, dx, dy,
			imageInfo->frameW,
			imageInfo->frameH,
			imageInfo->hMemDC,
			currentFrameX * imageInfo->frameW,
			currentFrameY * imageInfo->frameH,
			SRCCOPY);
	}
}
