#pragma once

class MusicalObject
{
public:
	MusicalObject();
	MusicalObject(const MusicalObject&);
	virtual ~MusicalObject();

	inline double GetTiming() { return timing; }
	inline double GetEndTiming() { return endTiming; }
	inline BOOL GetVisible() { return visible; }

	inline void SetTiming(double i) { timing = i; }
	inline void SetEndTiming(double i) { endTiming = i; }
	inline void SetVisible(BOOL b) { visible = b; }

protected:
	double timing; //decimal point error fuck
	double endTiming; //
	BOOL visible;
};

