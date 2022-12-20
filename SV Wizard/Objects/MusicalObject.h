#pragma once

class MusicalObject
{
public:
	MusicalObject();
	MusicalObject(const MusicalObject&);
	virtual ~MusicalObject();

	inline int GetTiming() { return timing; }
	inline BOOL GetVisible() { return visible; }

	inline void SetTiming(int i) { timing = i; }
	inline void SetVisible(BOOL b) { visible = b; }

protected:
	int timing;
	BOOL visible;
};

