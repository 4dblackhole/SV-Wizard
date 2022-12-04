#pragma once
#include "../ResourceManager/Image.h"
#include "../framework.h"

class BackGround
{
public:
	BackGround();
	~BackGround();

	void Init(HWND, Image*);
	void Resize(WPARAM, LPARAM);
	void Resizing(WPARAM, LPARAM);
	void Render(HDC);

	inline double GetBGMinRate() { return BGMinRate; }
	inline double GetBGMaxRate() { return BGMaxRate; }

	inline void SetBGMinRate(double a) { BGMinRate = a; }
	inline void SetBGMaxRate(double a) { BGMaxRate = a; }

private:
	Image* image;
	int origWidth, origHeight;
	HWND targetWnd;
	double BGMinRate = 0.2L;
	double BGMaxRate = 1.2L;
};

