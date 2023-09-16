#include "BackGround.h"

BackGround::BackGround() :image(NULL), origHeight(0), origWidth(0), targetWnd(0),
sideBeingDragged(0), origWHRate(0)
{
}

BackGround::~BackGround()
{
}

void BackGround::SetBackGround(HWND hWnd, Image* img)
{
	image = img;
	targetWnd = hWnd;
	if (image == NULL)return;

	origWidth = img->GetOrigWidth();
	origHeight = img->GetOrigHeight();
	origWHRate = (double)origWidth / (double)origHeight;

	sideBeingDragged = -1;

	int width, height;
	double rate = 1.0;

	if (BGMaxXPixel < origWidth) rate = min(rate, ((double)BGMaxXPixel / (double)origWidth));
	if (BGMaxYPixel < origHeight) rate = min(rate, ((double)BGMaxYPixel / (double)origHeight));

	width = int((double)origWidth * rate);
	height = int((double)origHeight * rate);

	SHORTCUT.SetClientRect(hWnd, width, height); //calls WM_SIZE
}

void BackGround::ResizeNew(WPARAM wParam, LPARAM lParam)
{
	if (image == NULL)return;

	WINDOWPOS *winPos = reinterpret_cast<WINDOWPOS*>(lParam);
	if (winPos->flags & SWP_NOSIZE)return;

	RECT wrt, crt;
	GetWindowRect(targetWnd, &wrt);
	GetClientRect(targetWnd, &crt);

	int header = wrt.bottom - wrt.top - crt.bottom;
	int border = wrt.right - wrt.left - crt.right;

	int clientWidth = winPos->cx - border;
	int clientHeight = winPos->cy - header;

	switch (sideBeingDragged)
	{
		case WMSZ_BOTTOM:
		case WMSZ_TOP:
			clientHeight = (int)max((double)clientHeight, (double)origHeight * BGMinSizeRate);
			clientHeight = (int)min((double)clientHeight, (double)origHeight * BGMaxSizeRate);
			winPos->cx = (int)((double)clientHeight * origWHRate) + border;
			winPos->cy = clientHeight + header;
			clientWidth = winPos->cx - border;
		break;

		case WMSZ_LEFT:
		case WMSZ_TOPLEFT:
		case WMSZ_BOTTOMLEFT:
		case WMSZ_RIGHT:
		case WMSZ_TOPRIGHT:
		case WMSZ_BOTTOMRIGHT:
			clientWidth = (int)max((double)clientWidth, (double)origWidth * BGMinSizeRate);
			clientWidth = (int)min((double)clientWidth, (double)origWidth * BGMaxSizeRate);
			winPos->cy = (int)((double)clientWidth / origWHRate) + header;
			winPos->cx = clientWidth + border;
			clientHeight = winPos->cy - header;
		break;
	}
	
	image->Resize(clientWidth, clientHeight);
}

void BackGround::ResizingNew(WPARAM wParam, LPARAM lParam)
{
	sideBeingDragged = (int)wParam;
}

void BackGround::Render(HDC hdc)
{
	if (image == NULL)return;
	image->Render(hdc);
}

void BackGround::SetBGMinX(double a)
{
	BGMinXPixel = a;
	BGMinSizeRate = max(a / (double)origWidth, BGMinSizeRate);
}

void BackGround::SetBGMinY(double a)
{
	BGMinYPixel = a;
	BGMinSizeRate = max(a / (double)origHeight, BGMinSizeRate);
}
