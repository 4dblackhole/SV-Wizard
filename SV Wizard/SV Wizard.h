#pragma once

#include "resource.h"
#include "Objects/BackGround.h"
#include "Singletones.h"

BOOL OpenFileDirectory(_Out_ TCHAR*, SVDialog&);
BOOL CheckUTF8(_In_ TCHAR*);
char* GetOsuFileTXT(_In_ TCHAR*);

void InitMusicalObjects(_In_ char*, queue<MusicalLine>&, queue<Note>&);

void SetMusicalLine(_In_ const string& txt, queue<MusicalLine>& lines);
BOOL SetMusicalLineTiming(MusicalLine&, _In_ const string&, queue<MusicalLine>&);

void SetNote(_In_ const string& txt, queue<Note>& notes);

