#include "../framework.h"
#include "MusicalLine.h"

MusicalLine::MusicalLine()
{
}

MusicalLine::MusicalLine(const MusicalLine& l) :MusicalObject(l)
{
	info = l.info;
}

MusicalLine::~MusicalLine()
{
}
