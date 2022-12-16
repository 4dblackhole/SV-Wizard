#pragma once
#include "../ResourceManager/Image.h"
#include "../framework.h"
#include "../Singletones.h"

class BackGround
{
public:
	BackGround();
	~BackGround();

	void SetBackGround(HWND, Image*);
	void Resize(WPARAM, LPARAM);
	void Resizing(WPARAM, LPARAM);
	void Render(HDC);

	inline double GetBGMinRate() { return BGMinRate; }
	inline double GetBGMaxRate() { return BGMaxRate; }

	inline void SetBGMinRate(double a) { BGMinRate = a; }
	inline void SetBGMaxRate(double a) { BGMaxRate = a; }

	inline void SetBGMinX(double a) { BGMinXPixel= a; }
	inline void SetBGMinY(double a) { BGMinYPixel = a; }

private:
	Image* image;
	int origWidth, origHeight;
	HWND targetWnd;
	double BGMinRate = 0.2L;
	double BGMinXPixel = 256.0L;
	double BGMinYPixel = 192.0L;
	double BGMaxRate = 1.2L;
	double BGMaxXPixel = 1440.0L;
	double BGMaxYPixel = 960.0L;
};

