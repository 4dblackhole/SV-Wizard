#pragma once
#include "../framework.h"

class ShortCut
{
public:
	static ShortCut& GetInstance()
	{
		static ShortCut s;
		return s;
	}

	void SetClientRect(HWND hWnd, int width, int height);
	void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);

	inline double GetAngle(double x1, double y1, double x2, double y2);
	inline double GetAngle(int x1, int y1, int x2, int y2);
	inline double GetAngle(const POINT& pt1, const POINT& pt2);

	template<typename POS,typename VAL>
	VAL Interpolation_Linear(const POS& startPos, const POS& endPos, const VAL& startVal, const VAL& endVal, const POS& currentPos)
	{
		POS area = endPos - startPos;
		POS currentPosInArea = currentPos - startPos;

		VAL left = startVal * (VAL)(area - currentPosInArea);
		VAL right = endVal * (VAL)currentPosInArea;
		return (left + right) / area;
	}

	template<typename POS, typename VAL>
	VAL Interpolation_Exponential(const POS& startPos, const POS& endPos, const VAL& startVal, const VAL& endVal, const POS& currentPos)
	{
		POS area = endPos - startPos;
		POS currentPosInArea = currentPos - startPos;

		VAL left = pow(startVal, (VAL)(area - currentPosInArea) / area);
		VAL right = pow(endVal, (VAL)currentPosInArea/area);
		return left * right;
	}

	template<typename POS, typename VAL>
	VAL Interpolation_Harmonic(const POS& startPos, const POS& endPos, const VAL& startVal, const VAL& endVal, const POS& currentPos)
	{
		POS area = endPos - startPos;
		POS currentPosInArea = currentPos - startPos;

		VAL focusing = (VAL)area * endVal / startVal;
		VAL resultA = focusing + (area - focusing) * (currentPosInArea / area);

		return endVal * area / resultA;
	}

private:
	ShortCut() {};
	~ShortCut() {};
	ShortCut(const ShortCut& ref) { }
	ShortCut& operator=(const ShortCut& ref) { }
};
