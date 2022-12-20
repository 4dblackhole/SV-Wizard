#include "../framework.h"
#include "Path.h"
#include <shlwapi.h>
#include "String.h"
#pragma comment(lib, "shlwapi.lib")

bool Path::ExistFile(tstring path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());

	return fileValue < 0xFFFFFFFF;
}

bool Path::ExistDirectory(tstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES &&
		(attribute & FILE_ATTRIBUTE_DIRECTORY));

	return temp == TRUE;
}

tstring Path::Combine(tstring path1, tstring path2)
{
	return path1 + path2;
}

tstring Path::Combine(vector<tstring> paths)
{
	tstring temp = _T("");
	for (tstring path : paths)
		temp += path;

	return temp;
}

tstring Path::GetDirectoryName(tstring path)
{
	String::Replace(&path, _T("\\"), _T("/"));
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

tstring Path::GetUpDirectoryName(tstring path)
{
	String::Replace(&path, _T("\\"), _T("/"));
	size_t index = path.find_last_of('/');

	tstring up = path.substr(0, index);
	String::Replace(&up, _T("\\"), _T("/"));
	size_t upIndex = up.find_last_of('/');

	return up.substr(upIndex + 1, up.length());
}

tstring Path::GetUpDirectoryAndFileName(tstring path)
{
	String::Replace(&path, _T("\\"), _T("/"));
	size_t index = path.find_last_of('/');

	tstring up = path.substr(0, index);
	String::Replace(&up, _T("\\"), _T("/"));
	size_t upIndex = up.find_last_of('/');

	return path.substr(upIndex + 1, path.length());
}

tstring Path::GetExtension(tstring path)
{
	String::Replace(&path, _T("\\"), _T("/"));
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}


string Path::GetFileName(string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

wstring Path::GetFileName(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

string Path::GetFileNameWithoutExtension(string path)
{
	string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

wstring Path::GetFileNameWithoutExtension(wstring path)
{
	wstring fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}


string Path::DeleteExtension(string path)
{
	size_t index = path.find_last_of('.');
	return path.substr(0, index);
}

wstring Path::DeleteExtension(wstring path)
{
	size_t index = path.find_last_of('.');
	return path.substr(0, index);
}


const TCHAR* Path::ImageFilter = _T("Image\0*.png;*.bmp;*.jpg");

//path : ../Temp/
//filter : *.txt
void Path::GetFiles(vector<tstring>* files, tstring path, tstring filter, bool bFindSubFolder)
{
	tstring file = path + filter;

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					tstring folder = path + tstring(findData.cFileName) + _T("/");

					GetFiles(files, folder, filter, bFindSubFolder);
				}
			}
			else
			{
				tstring fileName = path + tstring(findData.cFileName);
				files->push_back(fileName);
			}
		} while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
}

void Path::CreateFolder(tstring path)
{
	if (ExistDirectory(path) == false)
		CreateDirectory(path.c_str(), NULL);
}

void Path::CreateFolders(tstring path)
{
	String::Replace(&path, _T("\\"), _T("/"));

	vector<tstring> folders;
	String::SplitString(&folders, path, _T("/"));

	tstring temp = _T("");
	for (tstring folder : folders)
	{
		temp += folder + _T("/");

		CreateFolder(temp);
	}
}

bool Path::IsRelativePath(tstring path)
{
	BOOL b = PathIsRelative(path.c_str());

	return b >= TRUE;
}
