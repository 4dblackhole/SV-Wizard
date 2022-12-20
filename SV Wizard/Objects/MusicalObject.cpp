#include "../framework.h"
#include "MusicalObject.h"

MusicalObject::MusicalObject()
{
	timing = 0;
	visible = TRUE;
}

MusicalObject::MusicalObject(const MusicalObject& n)
{
	timing = n.timing;
	visible = n.visible;
}

MusicalObject::~MusicalObject()
{
}
