#include "pch.h"
#include "CFileLocker.h"
#include "tchar.h"

CFileLocker::CFileLocker()
{

}

CFileLocker::~CFileLocker()
{

}

int CFileLocker::LockFile(LPCTSTR pszFileName, LPCSTR pszPublicKey)
{
	TCHAR szEncFileName[MAX_PATH] = { 0 };
	unsigned char szBuffer[AES_BLOCK_SIZE] = { 0 };
	unsigned char szEnc[AES_BLOCK_SIZE] = { 0 };
	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	_tcscpy_s(szEncFileName, pszFileName);
	_tcscat_s(szEncFileName, _T(".enc"));
	HANDLE hEncFile = CreateFile(szEncFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hEncFile)
	{
		return 0;
	}
	// Encrypt AES Key
	CRsaKey rsa;
	if (!rsa.LoadPublicKey(pszPublicKey)) {
		return 0;
	}
	unsigned char szEncAesKey[512];
	int nEncAesKeyLen = sizeof(szEncAesKey);
	if (NULL == rsa.EncryptByPublicKey(m_KeyAes.m_RandBuffer, sizeof(m_KeyAes.m_RandBuffer), szEncAesKey, &nEncAesKeyLen)) {
		return 0;
	}

	// ÎÄ¼þÍ·
	ST_FILEHEADER stFileHeader;
	memset(&stFileHeader, 0, sizeof(stFileHeader));
	stFileHeader.szFlag[0] = 'L';
	stFileHeader.szFlag[1] = 'O';
	stFileHeader.szFlag[2] = 'C';
	stFileHeader.szFlag[3] = 'K';
	stFileHeader.dwVersion = 1;
	stFileHeader.dwAesKeySize = nEncAesKeyLen;	//sizeof(m_KeyAes.m_RandBuffer);
	stFileHeader.dwIvecSize = sizeof(m_KeyAes.m_ivecDec);

	// Write file header
	DWORD dwRead = 0;
	DWORD dwWrite = 0;
	if (!WriteFile(hEncFile, &stFileHeader, sizeof(stFileHeader), &dwWrite, NULL)) {
		CloseHandle(hFile);
		CloseHandle(hEncFile);
		return 0;
	}
	// Write AES key & ivec
	/*	if (!WriteFile(hEncFile, m_KeyAes.m_RandBuffer, sizeof(m_KeyAes.m_RandBuffer), &dwWrite, NULL)) {
		CloseHandle(hFile);
		CloseHandle(hEncFile);
		return 0;
	}*/
	if (!WriteFile(hEncFile, szEncAesKey, sizeof(szEncAesKey), &dwWrite, NULL)) {
		CloseHandle(hFile);
		CloseHandle(hEncFile);
		return 0;
	}
	if (!WriteFile(hEncFile, m_KeyAes.m_ivecDec, sizeof(m_KeyAes.m_ivecDec), &dwWrite, NULL)) {
		CloseHandle(hFile);
		CloseHandle(hEncFile);
		return 0;
	}
	// Write encrypted file data
	while (ReadFile(hFile, szBuffer, AES_BLOCK_SIZE-1, &dwRead, NULL))
	{
		if (0 == dwRead) {
			break;
		}
		int nRead = m_KeyAes.AesEncrypt(szBuffer, dwRead, szEnc);
		// write to enc file
		if (!WriteFile(hEncFile, szEnc, nRead, &dwWrite, NULL)) {
			CloseHandle(hFile);
			CloseHandle(hEncFile);
			return 0;
		}
	}
	CloseHandle(hFile);
	CloseHandle(hEncFile);
	return 1;
}

int CFileLocker::UnlockFile(LPCTSTR pszFileName)
{
	TCHAR szDecFileName[MAX_PATH] = { 0 };
	unsigned char szBuffer[AES_BLOCK_SIZE] = { 0 };
	unsigned char szDec[AES_BLOCK_SIZE] = { 0 };
	HANDLE hEncFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	_tcscpy_s(szDecFileName, pszFileName);
	if (0 == _tcsicmp(szDecFileName+_tcslen(szDecFileName)-4, _T(".enc"))){
		szDecFileName[_tcslen(szDecFileName) - 4] = 0;
	}
	else {
		_tcscat_s(szDecFileName, _T(".dec"));
	}
	HANDLE hDecFile = CreateFile(szDecFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == hDecFile || INVALID_HANDLE_VALUE == hEncFile)
	{
		return 0;
	}
	CRsaKey rsa;
	if (!rsa.LoadPrivateKey()) {
		return 0;
	}
	// Read file header
	DWORD dwRead = 0;
	DWORD dwWrite = 0;
	ST_FILEHEADER stFileHeader;
	memset(&stFileHeader, 0, sizeof(stFileHeader));
	if (!ReadFile(hEncFile, &stFileHeader, sizeof(stFileHeader), &dwRead, NULL)) {
		CloseHandle(hDecFile);
		CloseHandle(hEncFile);
		return 0;
	}
	// Read AES key & ivec
	unsigned char szEncAesKey[512] = { 0 };
	unsigned char szDecAesKey[512] = { 0 };
	unsigned char szIvec[AES_BLOCK_SIZE] = { 0 };
	if (!ReadFile(hEncFile, szEncAesKey, sizeof(szEncAesKey), &dwRead, NULL)) {
		CloseHandle(hDecFile);
		CloseHandle(hEncFile);
		return 0;
	}
	// Decrypt Aes key by rsa private key
	int nDecAesKeyLen = sizeof(szDecAesKey);
	if (NULL == rsa.DecryptByPrivateKey(szEncAesKey, sizeof(szEncAesKey), szDecAesKey, &nDecAesKeyLen)) {
		return 0;
	}
	unsigned char szAesKey[AES_KEY_SIZE] = { 0 };
	memcpy(szAesKey, szDecAesKey, sizeof(szAesKey));
	// Set Aes Key
	m_KeyAes.SetKey(szAesKey);
	if (!ReadFile(hEncFile, szIvec, sizeof(szIvec), &dwRead, NULL)) {
		CloseHandle(hDecFile);
		CloseHandle(hEncFile);
		return 0;
	}
	// Set Ivec
	m_KeyAes.SetIvec(szIvec);

	// Read file data and encrypt
	while (ReadFile(hEncFile, szBuffer, AES_BLOCK_SIZE-1, &dwRead, NULL))
	{
		if (0 == dwRead)
		{
			break;
		}
		int nRead = m_KeyAes.AesDecrypt(szBuffer, dwRead, szDec);
		// write to enc file
		if (!WriteFile(hDecFile, szDec, nRead, &dwWrite, NULL)) {
			CloseHandle(hDecFile);
			CloseHandle(hEncFile);
			return 0;
		}
	}
	CloseHandle(hDecFile);
	CloseHandle(hEncFile);
	return 1;
}
