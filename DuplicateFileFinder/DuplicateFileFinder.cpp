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

#define NODE (filell*)malloc(sizeof(filell))
#define MAX_PATH_LENGTH 1024

typedef struct filell
{
	TCHAR *fpath;
	TCHAR *fname;
	//STRING strName;
	long fsize;
	struct filell *next;
}fll;

static fll*head = NULL;              /*Text File Linked List Head*/

void Create_File_List(fll**head, TCHAR*fpath1, TCHAR *fname1, long fsize1)
{
	fll*newnode = NODE;
	int iLen = 0;

	iLen = (_tcslen(fname1) + 1) * sizeof(TCHAR);
	newnode->fname = (TCHAR*)malloc(iLen);
	_tcscpy(newnode->fname, fname1);
	//newnode->strName = fname1;

	iLen = (_tcslen(fpath1) + 1) * sizeof(TCHAR);
	newnode->fpath = (TCHAR*)malloc(iLen);
	_tcscpy(newnode->fpath, fpath1);
	//newnode->fpath = fpath1;
	newnode->fsize = fsize1;
	newnode->next = NULL;
	if (*head == NULL) 
	{
		*head = newnode;
	}
	else 
	{
		fll*prev = NULL, *temp = NULL;
		for (temp = prev = *head; temp != NULL && temp->fsize < newnode->fsize; prev = temp, temp = temp->next);

		if (temp == *head) 
		{
			newnode->next = *head;
			*head = newnode;
		}
		else 
		{
			newnode->next = temp;
			prev->next = newnode;
		}
	}
}//create()
//---------------------------------------------------------------------------


void FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
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

				TCHAR szFullPath[1024];
				_tcscpy(szFullPath, lpFolder);
				_tcscat(szFullPath, _T("\\"));
				_tcscat(szFullPath, FindFileData.cFileName);
				Create_File_List(&head, (TCHAR *)lpFolder, FindFileData.cFileName, FindFileData.nFileSizeLow);
			}

		} while (FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
}

void Display_Duplicate_Files(fll**head)
{
	map<STRING, vector<STRING>> mDuplicateFileList;

	if (*head)
	{
		fll*temp = *head;
		while (temp != NULL)
		{
			TCHAR szFilePath[2048];
			TCHAR szChecksum[40];
			_tcscpy(szFilePath, temp->fpath);
			_tcscat(szFilePath, _T("\\"));
			_tcscat(szFilePath, temp->fname);
			CalclulateChecksum(szFilePath, szChecksum);
			//_tprintf(_T("FILE=%s\\%-40s SIZE=%10ld Bytes Checksum %s \n"), temp->fpath, temp->fname, temp->fsize, szChecksum);
			temp = temp->next;

			auto it = mDuplicateFileList.find(STRING(szChecksum));
			if (it == mDuplicateFileList.end())
			{
				vector<STRING> vctTemp;
				vctTemp.push_back(STRING(szFilePath));
				mDuplicateFileList[STRING(szChecksum)] = vctTemp;
			}
			else
			{
				it->second.push_back(STRING(szFilePath));
			}
		}

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
		printf("File List is Empty\n");
}

int main(int argc,TCHAR *argv)
{
	TCHAR szPath[2048];
    std::cout << "-------Duplicate File Finder--------------------!\n";
	_tprintf(_T("\nEnter Directory path:- "));
	_tscanf(_T("%s"), szPath);
	
	FindFilesRecursively(szPath, _T("*.txt"));
	Display_Duplicate_Files(&head);
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
