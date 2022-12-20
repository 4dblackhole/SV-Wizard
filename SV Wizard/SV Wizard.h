#pragma once

#include "resource.h"
#include "Objects/BackGround.h"
#include "Singletones.h"

BOOL OpenFileDirectory(_Out_ TCHAR*, SVDialog&);
BOOL CheckUTF8(_In_ TCHAR*);
char* GetOsuFileTXT(_In_ TCHAR*);