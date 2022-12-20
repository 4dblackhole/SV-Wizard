#pragma once
#include "MusicalObject.h"

enum HitSound_SampleSet
{
	HIT_NORMAL,
	HIT_SOFT,
	HIT_DRUM,
	HIT_MAX
};

struct HitSound_tag
{
	HitSound_SampleSet sampleSet;
	int idx;

	HitSound_tag()
	{
		sampleSet = HIT_NORMAL;
		idx = 0;
	}

	/*HitSound_tag(const HitSound_tag& t)
	{
		sampleSet = t.sampleSet;
		idx = t.idx;
	}*/
};

struct MusicalLine_tag
{
	double sv;
	double bpm;
	int volume;
	HitSound_tag hsType;
	BOOL kiai;

	MusicalLine_tag()
	{
		sv = 1.0;
		bpm = 180.0;
		volume = 100;
		kiai = FALSE;
	}

	/*MusicalLine_tag(MusicalLine_tag& t)
	{
		sv = t.sv;
		bpm = t.bpm;
		volume = t.volume;
		hsType = t.hsType;
		kiai = t.kiai;
	}*/
};

class MusicalLine :public MusicalObject
{
public:
	MusicalLine();
	MusicalLine(const MusicalLine&);
	virtual ~MusicalLine();

	MusicalLine_tag GetInfo() { return info; }

	void SetInfo(const MusicalLine_tag& d) { info = d; }

private:
	MusicalLine_tag info;
};
