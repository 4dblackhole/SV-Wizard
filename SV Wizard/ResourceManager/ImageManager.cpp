#include "ImageManager.h"

void ImageManager::Init()
{
}

void ImageManager::Release()
{
	DeleteALL();
}

Image* ImageManager::FindImage(tstring key)
{
	ImageListIter it = imageList.find(key);
	if (it != imageList.end()) return it->second;

	return NULL;
}

Image* ImageManager::AddImage(tstring key,int w,int h)
{
	Image* img = FindImage(key);
	if (img) return img;

	img = new Image;
	if (FAILED(img->Init(w, h)))
	{
		SAFE_DELETE(img);
		return NULL;
	}
	imageList.insert(make_pair(key, img));

	return img;
}

Image* ImageManager::AddImage(tstring key, DWORD resourceID)
{
	return AddImage(key, resourceID, 0, 0, 0, 0, 1, 1, FALSE, RGB(255, 0, 255));
}

Image* ImageManager::AddImage(tstring key, DWORD resourceID, int x, int y)
{
	return AddImage(key, resourceID, x, y, 0, 0, 1, 1, FALSE, RGB(255, 0, 255));
}

Image* ImageManager::AddImage(tstring key, DWORD resourceID, int x, int y, int w, int h)
{
	return AddImage(key, resourceID, x, y, w, h, 1, 1, FALSE, RGB(255, 0, 255));
}

Image* ImageManager::AddImage(tstring key, DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY)
{
	return AddImage(key, resourceID, x, y, w, h, frameAmountX, frameAmountY, FALSE, RGB(255, 0, 255));
}

Image* ImageManager::AddImage(tstring key, DWORD resourceID, int x, int y, int w, int h, int frameAmountX, int frameAmountY, BOOL trans, COLORREF transColor)
{
	Image* img = FindImage(key);
	if (img) return img;

	img = new Image;

	if (FAILED(img->Init(resourceID, x, y, w, h, frameAmountX, frameAmountY, trans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}
	imageList.insert(make_pair(key, img));

	return img;
}

void ImageManager::Render(tstring key, HDC hdc)
{
	Image* img = FindImage(key);
	if (img) img->Render(hdc);
}

void ImageManager::Render(tstring key, HDC hdc, int dx, int dy)
{
	Image* img = FindImage(key);
	if (img) img->Render(hdc, dx, dy);
}

void ImageManager::Render(tstring key, HDC hdc, int dx, int dy, int sx, int sy, int sw, int sh)
{
	Image* img = FindImage(key);
	if (img) img->Render(hdc, dx, dy, sx, sy, sw, sh);
}

void ImageManager::LoopRender(tstring key, HDC hdc, const LPRECT lpDrawArea, int offsetX, int offsetY)
{
	Image* img = FindImage(key);
	if (img) img->LoopRender(hdc, lpDrawArea, offsetX, offsetY);
}

void ImageManager::FrameRender(tstring key, HDC hdc, int dx, int dy, int currentFrameX, int currentFrameY)
{
	Image* img = FindImage(key);
	if (img) img->FrameRender(hdc, dx, dy, currentFrameX, currentFrameY);
}

BOOL ImageManager::DeleteImage(tstring key)
{
	ImageListIter it = imageList.find(key);
	if (it != imageList.end())
	{
		it->second->Release();
		SAFE_DELETE(it->second);
		imageList.erase(it);

		return TRUE;
	}
	return FALSE;
}

BOOL ImageManager::DeleteALL()
{
	for (ImageListIter it = imageList.begin(); it != imageList.end();)
	{
		if (it->second != NULL)
		{
			SAFE_DELETE(it->second);
			it = imageList.erase(it);
		}
		else it++;
	}
	imageList.clear();

	return TRUE;
}
