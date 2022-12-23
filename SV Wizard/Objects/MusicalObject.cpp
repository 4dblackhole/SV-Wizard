#include "../framework.h"
#include "MusicalObject.h"

MusicalObject::MusicalObject()
{
	timing = 0;
	endTiming = 0;
	visible = TRUE;
}

MusicalObject::MusicalObject(const MusicalObject& n)
{
	timing = n.timing;
	endTiming = n.endTiming;
	visible = n.visible;
}

MusicalObject::~MusicalObject()
{
}
