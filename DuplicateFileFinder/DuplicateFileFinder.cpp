// DuplicateFileFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DuplicateFileFinder.h"
#include <Shlwapi.h>
#include "CheckSum.h"
using namespace std;


#define MAX_PATH_LENGTH 1024
#define MAX_CHECKSUM_LENGTH 40

void DuplicateFileFinder::AddNewEntry(const TCHAR *pcszFilePath)
{
	TCHAR szChecksum[MAX_CHECKSUM_LENGTH];
	CalclulateChecksum(pcszFilePath, szChecksum);
	STRING strCheckSum(szChecksum);
	STRING strFilePath(pcszFilePath);

	auto it = mDuplicateFileList.find(strCheckSum);
	if (it == mDuplicateFileList.end())
	{
		vector<STRING> vctTemp;
		vctTemp.push_back(strFilePath);
		mDuplicateFileList[strCheckSum] = vctTemp;
	}
	else
	{
		it->second.push_back(strFilePath);
	}
}

void DuplicateFileFinder::FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
{
	HANDLE hFindFile;
	TCHAR szFullPattern[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	// first we are going to process any subdirectories
	PathCombine(szFullPattern, lpFolder, _T("*"));
	hFindFile = FindFirstFile(szFullPattern, &FindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FindFileData.cFileName[0] == '.' || FindFileData.cFileName[0] == '..')
			{
				continue;
			}

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// found a subdirectory; recurse into it
				PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
				FindFilesRecursively(szFullPattern, lpFilePattern);
			}
			else
			{

				TCHAR szFullPath[MAX_PATH_LENGTH];
				PathCombine(szFullPath, lpFolder, FindFileData.cFileName);
				AddNewEntry(szFullPath);
			}

		} while (FindNextFile(hFindFile, &FindFileData));

		FindClose(hFindFile);
	}
}


void DuplicateFileFinder::Display_Duplicate_Files()
{
	if (!mDuplicateFileList.empty())
	{
		printf("\n");
		for (auto it : mDuplicateFileList)
		{
			if (it.second.size() > 1)
			{
				printf("\n------------------------------------------------------\n");
				for (auto itVct : it.second)
				{
					_tprintf(_T("\n%s"), itVct.c_str());
				}

				printf("\n------------------------------------------------------");
			}
		}
	}
	else
	{
		_tprintf(_T("File List is Empty\n"));
	}
}

