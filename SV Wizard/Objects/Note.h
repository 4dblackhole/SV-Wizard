#pragma once
#include "MusicalObject.h"
#include "../ResourceManager/Image.h"


class Note :public MusicalObject
{
public:

	enum NOTE_TYPE_FLAG
	{
		TYPE_CIRCLE = (1 << 0),
		TYPE_SLIDER = (1 << 1),
		TYPE_NEWCOMBO = (1 << 2),
		TYPE_SPINNER = (1 << 3),
		TYPE_OSUMANIA_HOLD = (1 << 7),
	};

	enum NOTE_COLOR
	{
		COLOR_DON,
		COLOR_KAT,
		COLOR_BIGDON,
		COLOR_BIGKAT
	};

	Note();
	Note(const Note&);
	virtual ~Note();

	inline int GetPos() { return pos; }
	inline double GetSpeed() { return speed; }
	inline NOTE_TYPE_FLAG GetNoteType() { return type; }
	inline NOTE_COLOR GetNoteColor() { return color; }
	inline Image* GetImage() { return img; }

	inline void SetPos(int i) { pos = i; }
	inline void SetSpeed(double d) { speed = d; }
	inline void SetNoteType(NOTE_TYPE_FLAG t) { type = t; }
	inline void SetNoteColor(NOTE_COLOR t) { color = t; }
	inline void SetImage(Image* i) { img = i; }

protected:
	int pos; //Position on the Screen
	double speed; //Speed on the Screen
	NOTE_TYPE_FLAG type;
	NOTE_COLOR color;

private:
	Image* img;
};