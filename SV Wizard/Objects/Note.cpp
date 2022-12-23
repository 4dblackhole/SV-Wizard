#include "../framework.h"
#include "Note.h"

Note::Note()
{
	pos = 0;
	speed = 1.0;
	type = TYPE_CIRCLE;
	color = COLOR_DON;
	img = NULL;
}

Note::Note(const Note& n) :MusicalObject(n)
{
	pos = n.pos;
	speed = n.speed;
	type = n.type;
	color = n.color;
	img = n.img;
}

Note::~Note()
{
}