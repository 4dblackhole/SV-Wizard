#pragma once
#include "MusicalObject.h"

enum LineType //Green(0) Red(1)
{
	Line_GREEN,
	Line_RED,
	Line_MAX
};

enum HitSound_SampleSet //Normal(1) Soft(2) Drum(3)
{
	HIT_NONE,
	HIT_NORMAL,
	HIT_SOFT,
	HIT_DRUM,
	HIT_MAX
};

struct HitSound_tag
{
	HitSound_SampleSet sampleSet;
	int idx; // default(0)

	HitSound_tag()
	{
		sampleSet = HIT_NONE;
		idx = 444;
	}

	HitSound_tag(const HitSound_tag& t)
	{
		sampleSet = t.sampleSet;
		idx = t.idx;
	}
};

struct MusicalLine_tag
{
	double sv;
	double bpm;
	int volume;
	int measure;
	LineType lineType;
	HitSound_tag hsType;
	BOOL kiai; // OFF(0) ON(1)

	MusicalLine_tag()
	{
		sv = 1.1234;
		bpm = 180.1234;
		volume = 12;
		measure = 9;
		lineType = Line_MAX;
		kiai = FALSE;
	}

	MusicalLine_tag(const MusicalLine_tag& t)
	{
		sv = t.sv;
		bpm = t.bpm;
		volume = t.volume;
		measure = t.measure;
		lineType = t.lineType;
		hsType = t.hsType;
		kiai = t.kiai;
	}
};

class MusicalLine :public MusicalObject
{
public:
	MusicalLine();
	MusicalLine(const MusicalLine&);
	virtual ~MusicalLine();

	MusicalLine_tag GetInfo() { return info; }
	void SetInfo(const MusicalLine_tag& d) { info = d; }

	bool operator<(const MusicalLine& t) const
	{
		bool result = false;
		if (timing < t.timing)result = true;
		else if (timing == t.timing && info.lineType > t.info.lineType)result = true; // red line first

		return result;
	}

	bool operator>(const MusicalLine& t) const
	{
		bool result = false;
		if (timing > t.timing)result = true;
		else if (timing == t.timing && info.lineType < t.info.lineType)result = true; // red line first

		return result;
	}

private:
	MusicalLine_tag info;
};

typedef multimap<double, MusicalLine> LineContainer;