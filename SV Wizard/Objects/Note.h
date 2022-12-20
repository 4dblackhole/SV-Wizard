#pragma once
#include "MusicalObject.h"

class Note :public MusicalObject
{
	enum NOTE_TYPE
	{
		NOTE_DON,
		NOTE_KAT,
		NOTE_BIGDON,
		NOTE_BIGKAT,
		NOTE_MAX,
	};

public:
	Note();
	Note(const Note&);
	virtual ~Note();

	inline int GetPos() { return pos; }
	inline double GetSpeed() { return speed; }
	inline NOTE_TYPE GetNoteType() { return type; }
	inline Image* GetImage() { return img; }

	inline void SetPos(int i) { pos = i; }
	inline void SetSpeed(double d) { speed = d; }
	inline void SetNoteType(NOTE_TYPE t) { type = t; }
	inline void SetImage(Image* i) { img = i; }

private:
	int pos;
	double speed;
	NOTE_TYPE type;
	Image* img;
};

