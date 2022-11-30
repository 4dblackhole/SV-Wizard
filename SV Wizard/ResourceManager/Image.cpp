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
			hdc,					//����� ����DC
			dx,					//����� ��ǥ X
			dy,					//����� ��ǥ Y
			sw,				//����� ����ũ��
			sh,				//����� ����ũ��
			imageInfo->hMemDC,
			sx, sy,			//�����ؿ� ��ǥ
			sw,				//�����ؿ� ����ũ��
			sh,				//�����ؿ� ����ũ��
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

	//������ ������ �޾ƿ� ����
	int sourWidth;
	int sourHeight;
	//�׷��ִ� ����
	RECT rcDest;
	RECT rcSour;

	//�׷��ִ� ������ ����ش�
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;      //�ʺ�
	int drawAreaH = drawArea->bottom - drawAreaY;     //����

	//Y�� ����
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offsetY) % imageInfo->H;
		rcSour.bottom = imageInfo->H;

		sourHeight = rcSour.bottom - rcSour.top;

		//�׷��ִ� ������ ���̺��� y���� ũ��
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//ȭ������� ����������ŭ �ѷ��� ��ġ�� �����Ѵ�
		//�ѷ��ִ� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;


		//x��
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

			Render(hdc, rcDest.left, rcDest.top,//�ѷ��� ��ġ X, Y
				rcSour.left, rcSour.top,		//�ѷ��� ��ġ
				rcSour.right - rcSour.left,		//����ũ��
				rcSour.bottom - rcSour.top);	//����ũ��
		}
	}
}
