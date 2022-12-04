#include "BackGround.h"

BackGround::BackGround() :image(NULL), origHeight(0), origWidth(0)
{
}

BackGround::~BackGround()
{
}

void BackGround::Init(HWND hWnd, Image* img)
{
    image = img;
    targetWnd = hWnd;
    if (image == NULL)return;

    origWidth = img->GetWidth();
    origHeight = img->GetHeight();
    SHORTCUT.SetClientRect(hWnd, origWidth, origHeight);
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

    width = ((double)origWidth * BGMinRate) + border;
    height = ((double)origHeight * BGMinRate) + header;

    //Minimum size
    if (lprc->right - lprc->left < width)
        lprc->right = lprc->left + width;

    if (lprc->bottom - lprc->top < height)
        lprc->bottom = lprc->top + height;

    // Maximum width
    if (lprc->right - lprc->left > 1440 + border)
        lprc->right = lprc->left + 1440 + border;

    if (lprc->bottom - lprc->top > 960 + header)
        lprc->bottom = lprc->top + 960 + header;

    width = lprc->right - lprc->left;
    height = lprc->bottom - lprc->top;

    switch (wParam)
    {
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
        {
            height = ((width - border) * ((double)origHeight / (double)origWidth)) + header;
            lprc->bottom = lprc->top + height;
        }
        break;
        
        case WMSZ_TOP:
        case WMSZ_BOTTOM:
        default:
        {
            width = ((height - header) * ((double)origWidth / (double)origHeight)) + border;
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
