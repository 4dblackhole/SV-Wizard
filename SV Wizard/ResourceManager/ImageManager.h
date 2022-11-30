#pragma once
#include "SingletonBase.h"
#include "Image.h"
#include "framework.h"

class ImageManager : public SingletonBase<ImageManager>
{
private:
	typedef map<tstring, Image>				ImageList;
	typedef map<tstring, Image>::iterator	ImageListIt;

	ImageList imageList;

public:
	void Init();
	void Release();

	Image* addImage(tstring key, int w, int h);
};