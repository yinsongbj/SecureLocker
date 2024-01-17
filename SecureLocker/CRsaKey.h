#pragma once
#include <string.h>
#include <Windows.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <vector>
#include <string>

using namespace std;

#define PRIVATE_KEY_FILENAME "private.pem"
#define PUBLIC_KEY_FILENAME "public.pem"

class CRsaKey
{
private:
	RSA* m_KeyPrivate;
	RSA* m_KeyPublic;
public:
	CHAR m_szSavePath[MAX_PATH];
	CHAR m_szFriendsPath[MAX_PATH];
	CHAR m_szPrivateKey[MAX_PATH];
	CHAR m_szPublicKey[MAX_PATH];
	vector<string> m_vtFriendKeys;
public:
	CRsaKey();
	~CRsaKey();
public:
	int KeyGen();
	unsigned char* EncryptByPublicKey(const unsigned char* szPlainText, int nPlainTextLen, unsigned char* szCipherText, int* nCypherTextLen);
	unsigned char* DecryptByPrivateKey(const unsigned char* szCipherText, int nCypherTextLen, unsigned char* szPlainText, int* nPlainTextLen);
	char* EncryptByPrivateKey(char* str);
	char* DecryptByPublicKey(char* str);
	int LoadKey();
	int LoadPrivateKey(LPCSTR pszPrivateKey = NULL);
	int LoadPublicKey(LPCSTR pszPublicKey = NULL);
	int ReleaseKey();
	static int PassCallback(char* buf, int size, int rwflag, void* u);
	int SetPrivateKey(char* pszPrivteKeyFileName);
	int SetPublicKey(char* pszPublicKeyFileName);
	int SavePublicKey();
	int LoadFriendKeys();
};

