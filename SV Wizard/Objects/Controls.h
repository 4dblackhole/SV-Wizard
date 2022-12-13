#pragma once
#include "../framework.h"
#include "IControls.h"

typedef struct Controls : public IControls
{
public:
	virtual void Init(HWND);

	HWND heStartSV, heEndSV, heStartTiming, heEndTiming, heBaseBPM, heLineOffset, heVolume;
	HWND hstFileDir;
	HWND hbBaseBPM;
	HWND hslVolume;
	HWND hspStartSV, hspStartSVsm, hspEndSV, hspEndSVsm, hspStartTiming, hspStartTimingsm, hspEndTiming, hspEndTimingsm;
	HWND hspVolume, hspVolumesm;

	Controls() {};
	~Controls() {};

}Controls, *LPControls;