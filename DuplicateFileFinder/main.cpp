#include "DuplicateFileFinder.h"

int main(int argc, TCHAR *argv)
{
	TCHAR szPath[2048];// = _T("D:\\MayurJ\\DFF_Test");
	std::cout << "-------Duplicate File Finder--------------------!\n";
	_tprintf(_T("\nEnter Directory path:- "));
	_tscanf(_T("%s"), szPath);

	DuplicateFileFinder objDuplicateFileFinder;
	objDuplicateFileFinder.FindFilesRecursively(szPath, _T("*.txt"));
	objDuplicateFileFinder.Display_Duplicate_Files();
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
