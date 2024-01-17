#pragma once
#include "CAesKey.h"
#include "CRsaKey.h"
#include "Windows.h"

typedef struct stFILEHEADER {
	CHAR szFlag[4];
	DWORD dwVersion;
	DWORD dwAesKeySize;
	DWORD dwIvecSize;
	DWORD dwUnUse;
}ST_FILEHEADER, *PST_FILEHEADER;

class CFileLocker
{
private:
	CAesKey m_KeyAes;
	CRsaKey m_KeyRsa;
public:
	CFileLocker();
	~CFileLocker();
	int LockFile(LPCTSTR pszFileName, LPCSTR pszPublicKey);
	int UnlockFile(LPCTSTR pszFileName);
};

