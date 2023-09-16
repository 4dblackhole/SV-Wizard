#pragma once
#include "../framework.h"
#include "../ResourceManager/Image.h"
#include "../Singletones.h"

class BackGround
{
public:
	BackGround();
	~BackGround();

	void SetBackGround(HWND, Image*);
	void ResizeNew(WPARAM, LPARAM);
	void ResizingNew(WPARAM, LPARAM);
	void Render(HDC);

	inline double GetBGMinSizeRate() { return BGMinSizeRate; }
	inline double GetBGMaxSizeRate() { return BGMaxSizeRate; }

	inline void SetBGMinSizeRate(double a) { BGMinSizeRate = a; }
	inline void SetBGMaxSizeRate(double a) { BGMaxSizeRate = a; }

	void SetBGMinX(double a);
	void SetBGMinY(double a);

private:
	Image* image;
	int origWidth, origHeight; //image's Width and Height
	double origWHRate; //image's H/W Rate
	int sideBeingDragged;

	HWND targetWnd;
	double BGMinSizeRate = 0.8L;
	double BGMinXPixel = 256.0L;
	double BGMinYPixel = 572.0L;
	double BGMaxSizeRate = 1.0L;
	double BGMaxXPixel = 1440.0L;
	double BGMaxYPixel = 960.0L;
};

