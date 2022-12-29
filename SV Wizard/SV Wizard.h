#pragma once

BOOL OpenFileDirectory(_Out_ TCHAR*, SVDialog&);
BOOL CheckUTF8(_In_ TCHAR*);
char* GetOsuFileTXT(_In_ TCHAR*);

void InitMusicalObjects(_In_ char* osufile, LineContainer& lines, NoteContainer& notes);

void SetMusicalLine(_In_ const string& txt, LineContainer& lines);
BOOL SetMusicalLineTiming(MusicalLine&, _In_ const string&, LineContainer&);

void SeparateOsuTXT(_In_ const string& txt, _Out_ string& top, _Out_ string& bottom);

void SetNote(_In_ const string& txt, NoteContainer& notes);

