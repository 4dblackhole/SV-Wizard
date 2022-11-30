#include "Image.h"
#pragma comment(lib, "msimg32.lib")

Image::Image()
	:imageInfo(NULL), imageFileName(NULL), isTransparent(FALSE), transparentColor(RGB(0, 0, 0))
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
	imageInfo->W = w;
	imageInfo->H = h;

	if (imageInfo->hBit == NULL)
	{
		Release();
		return E_FAIL;
	}

	ReleaseDC(hRootWindow, hdc);

	return S_OK;
}

HRESULT Image::Init(const TCHAR* filename, int w, int h, BOOL trans, COLORREF transColor)
{
	return Image::Init(filename, 0, 0, w, h, 1, 1, trans, transColor);
}

HRESULT Image::Init(const TCHAR* filename, int x, int y, int w, int h, BOOL trans, COLORREF transColor)
{
	return Image::Init(filename, x, y, w, h, 1, 1, trans, transColor);
}

HRESULT Image::Init(const TCHAR* filename, int x, int y, int w, int h, int fx, int fy, BOOL trans, COLORREF transColor)
{
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(hRootWindow);

	imageInfo = new IMAGE_INFO;
	imageInfo->loadType = LOAD_FROM_FILE;
	imageInfo->resourceID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);  //empty DC
	imageInfo->hBit = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);
	imageInfo->hOldbit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->X = x;
	imageInfo->Y = y;
	imageInfo->W = w;
	imageInfo->H = h;
	imageInfo->frameW = w / fx;
	imageInfo->frameH = h / fy;
	imageInfo->currentFrameX = 0;
	imageInfo->currentFrameY = 0;
	imageInfo->maxFrameX = fx - 1;
	imageInfo->maxFrameY = fy - 1;

	int len = lstrlen(filename);

	imageFileName = new TCHAR[len + 1];
	_tcscpy_s(imageFileName, static_cast<rsize_t>(len) + 1, filename);

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
	Render(hdc, imageInfo->X, imageInfo->Y, 0, 0, imageInfo->W, imageInfo->H);
}

void Image::Render(HDC hdc, int dx, int dy)
{
	Render(hdc, dx, dy, 0, 0, imageInfo->W, imageInfo->H);
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

void Image::LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{

	if (offsetX < 0)offsetX = imageInfo->W + (offsetX % imageInfo->W);
	if (offsetY < 0)offsetY = imageInfo->H + (offsetY % imageInfo->H);

	//오프셋 영역을 받아올 변수
	int sourWidth;
	int sourHeight;
	//그려주는 영역
	RECT rcDest;
	RECT rcSour;

	//그려주는 영역을 잡아준다
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;      //너비
	int drawAreaH = drawArea->bottom - drawAreaY;     //높이

	//Y축 부터
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offsetY) % imageInfo->H;
		rcSour.bottom = imageInfo->H;

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
			rcSour.left = (x + offsetX) % imageInfo->W;
			rcSour.right = imageInfo->W;

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
