#include "../framework.h"
#include "../ResourceManager/Image.h"
#include "Note.h"

Note::Note()
{
	pos = 0;
	speed = 1.0;
	type = NOTE_DON;
	img = NULL;
}

Note::Note(const Note& n)
{
	pos = n.pos;
	speed = n.speed;
	type = n.type;
	img = n.img;
}

Note::~Note()
{
}
