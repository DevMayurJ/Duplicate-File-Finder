// DuplicateFileFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "Shlwapi.h"
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include "CheckSum.h"
using namespace std;

#ifdef UNICODE
typedef wstring STRING;
#else
typedef string STRING;
#endif

#define MAX_PATH_LENGTH 1024
#define MAX_CHECKSUM_LENGTH 40

void AddNewEntry(map<STRING, vector<STRING>> &mDuplicateFileList, TCHAR *pszFilePath)
{
	TCHAR szChecksum[MAX_CHECKSUM_LENGTH];
	CalclulateChecksum(pszFilePath, szChecksum);
	auto it = mDuplicateFileList.find(STRING(szChecksum));
	if (it == mDuplicateFileList.end())
	{
		vector<STRING> vctTemp;
		vctTemp.push_back(STRING(pszFilePath));
		mDuplicateFileList[STRING(szChecksum)] = vctTemp;
	}
	else
	{
		it->second.push_back(STRING(pszFilePath));
	}
}

void FindFilesRecursively(LPCTSTR lpFolder, map<STRING, vector<STRING>> &mDuplicateFileList, LPCTSTR lpFilePattern)
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
				FindFilesRecursively(szFullPattern, mDuplicateFileList, lpFilePattern);
			}
			else
			{

				TCHAR szFullPath[MAX_PATH_LENGTH];
				PathCombine(szFullPath, lpFolder, FindFileData.cFileName);
				AddNewEntry(mDuplicateFileList, szFullPath);
			}

		} while (FindNextFile(hFindFile, &FindFileData));

		FindClose(hFindFile);
	}
}


void Display_Duplicate_Files(const map<STRING, vector<STRING>> &mDuplicateFileList)
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

int main(int argc,TCHAR *argv)
{
	map<STRING, vector<STRING>> mDuplicateFileList;
	TCHAR szPath[2048]; //= _T("D:\\MayurJ\\DFF_Test");
    std::cout << "-------Duplicate File Finder--------------------!\n";
	_tprintf(_T("\nEnter Directory path:- "));
	_tscanf(_T("%s"), szPath);
	
	FindFilesRecursively(szPath, mDuplicateFileList, _T("*.txt"));
	Display_Duplicate_Files(mDuplicateFileList);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
