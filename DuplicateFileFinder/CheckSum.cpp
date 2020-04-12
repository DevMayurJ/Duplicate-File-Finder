#include <stdio.h>
#include "CheckSum.h"
#include <Wincrypt.h>
#include <tchar.h>

#define BUFSIZE 1024
#define MD5LEN  16

DWORD CalclulateChecksum(TCHAR *pszFilePath,TCHAR *pszCheckSum)
{
	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";
	// Logic to check usage goes here.

	hFile = CreateFile(pszFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		_tprintf(_T("Error opening file %s\nError: %d\n"), pszFilePath, dwStatus);
		return dwStatus;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		_tprintf(_T("CryptAcquireContext failed: %d\n"), dwStatus);
		CloseHandle(hFile);
		return dwStatus;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		_tprintf(_T("CryptAcquireContext failed: %d\n"), dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return dwStatus;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			_tprintf(_T("CryptHashData failed: %d\n"), dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return dwStatus;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		_tprintf(_T("ReadFile failed: %d\n"), dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return dwStatus;
	}

	cbHash = MD5LEN;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		//printf("MD5 hash of file %s is: ", pszFilePath);
		int iIndex = 0;
		for (DWORD i = 0; i < cbHash; i++)
		{
			//printf("%c%c", rgbDigits[rgbHash[i] >> 4],
			//	rgbDigits[rgbHash[i] & 0xf]);

			TCHAR szCurr[3];
			_stprintf(szCurr, _T("%c%c"), rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
			pszCheckSum[iIndex++] = rgbDigits[rgbHash[i] >> 4];
			pszCheckSum[iIndex++] = rgbDigits[rgbHash[i] & 0xf];
		}

		pszCheckSum[iIndex++] = '\0';
		//printf("\n");
	}
	else
	{
		dwStatus = GetLastError();
		_tprintf(_T("CryptGetHashParam failed: %d\n"), dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return dwStatus;
}