#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <tchar.h>
#include <iostream>

#ifdef UNICODE
typedef std::wstring STRING;
#else
typedef std::string STRING;
#endif

class DuplicateFileFinder
{
	std::map<STRING, std::vector<STRING>> mDuplicateFileList;
	void AddNewEntry(const TCHAR *pcszFilePath);
public:
	void FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern);
	void Display_Duplicate_Files();
};

