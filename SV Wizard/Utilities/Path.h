#pragma once

class Path
{
public:
	static bool ExistFile(tstring path);

	static bool ExistDirectory(tstring path);

	static tstring Combine(tstring path1, tstring path2);

	static tstring Combine(vector<tstring> paths);

	static tstring GetDirectoryName(tstring path);

	static tstring GetUpDirectoryName(tstring path);

	static tstring GetUpDirectoryAndFileName(tstring path);

	static tstring GetExtension(tstring path);

	static string GetFileName(string path);
	static wstring GetFileName(wstring path);

	static string GetFileNameWithoutExtension(string path);
	static wstring GetFileNameWithoutExtension(wstring path);

	static string DeleteExtension(string path);
	static wstring DeleteExtension(wstring path);

	const static TCHAR* ImageFilter;
	const static TCHAR* BinModelFilter;
	const static TCHAR* FbxModelFilter;
	const static TCHAR* ShaderFilter;

	/*static void OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd = NULL);
	static void SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd = NULL);

	static void OpenFileDialog3(wstring file, const WCHAR* filter, wstring folder, void* func, HWND hwnd);
	static wstring GetFilePath(const WCHAR *filter = L"");*/

	static void GetFiles(vector<tstring>* files, tstring path, tstring filter, bool bFindSubFolder);

	static void CreateFolder(tstring path);

	static void CreateFolders(tstring path);

	static bool IsRelativePath(tstring path);
	
};
