#include "BackGround.h"

BackGround::BackGround() :image(NULL), origHeight(0), origWidth(0), targetWnd(0)
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

    int width{}, height{};
    double rate = 1.0;

    if (BGMaxXPixel < origWidth) rate = min(rate, ((double)BGMaxXPixel / (double)origWidth));
    if (BGMaxYPixel < origHeight) rate = min(rate, ((double)BGMaxYPixel / (double)origHeight));

    width = int((double)origWidth * rate);
    height = int((double)origHeight * rate);

    SHORTCUT.SetClientRect(hWnd, width, height); //calls WM_SIZE
}

void BackGround::Resize(WPARAM wParam,LPARAM lParam)
{
    if (image == NULL)return;

    int width = LOWORD(lParam);
    int height = HIWORD(lParam);

    image->Resize(width, height);

    if (width < (height * (origWidth / origHeight)))
    {
        RECT wrt, crt;
        GetWindowRect(targetWnd, &wrt);
        GetClientRect(targetWnd, &crt);

        int border = wrt.right - wrt.left - crt.right;
        int header = wrt.bottom - wrt.top - crt.bottom;

        width = (height * (origWidth / origHeight)) + border;
        height = height + header;

        SetWindowPos(targetWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void BackGround::Resizing(WPARAM wParam, LPARAM lParam)
{
    if (image == NULL)return;

    RECT* lprc = (RECT*)lParam;
    RECT wrt, crt;
    GetWindowRect(targetWnd, &wrt);
    GetClientRect(targetWnd, &crt);

    int border = wrt.right - wrt.left - crt.right;
    int header = wrt.bottom - wrt.top - crt.bottom;

    int width, height;

    //Minimum size

    BGMinRate = max(BGMinXPixel / (double)origWidth, BGMinYPixel / (double)origHeight);

    width = (int)((double)origWidth * BGMinRate) + border;
    height = (int)((double)origHeight * BGMinRate) + header;

    if (lprc->right - lprc->left < width)
        lprc->right = lprc->left + width;

    if (lprc->bottom - lprc->top < height)
        lprc->bottom = lprc->top + height;
    
    // Maximum size

    BGMaxRate = min(BGMaxXPixel / (double)origWidth, BGMaxYPixel / (double)origHeight);

    width = (int)((double)origWidth * BGMaxRate) + border;
    height = (int)((double)origHeight * BGMaxRate) + header;

    if (lprc->right - lprc->left > width)
        lprc->right = lprc->left + width;

    if (lprc->bottom - lprc->top > height)
        lprc->bottom = lprc->top + height;

    width = lprc->right - lprc->left;
    height = lprc->bottom - lprc->top;

    switch (wParam)
    {
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
        {
            height = (int)((double)(width - border) * ((double)origHeight / (double)origWidth)) + header;
            lprc->bottom = lprc->top + height;
        }
        break;
        
        case WMSZ_TOP:
        case WMSZ_BOTTOM:
        default:
        {
            width = (int)((double)(height - header) * ((double)origWidth / (double)origHeight)) + border;
            lprc->right = lprc->left + width;
        }
        break;
    }
}

void BackGround::Render(HDC hdc)
{
    if (image == NULL)return;
    image->Render(hdc);
}
