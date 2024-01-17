#pragma once

#include <openssl/aes.h>
#pragma comment(lib,"libcrypto.lib")

#define AES_KEY_SIZE 128 // AES��Կ����
#define AES_BLOCK_SIZE 16 // AES�ֿ��С

class CAesKey
{
public:
	CAesKey();
	~CAesKey();
public:
	unsigned char m_RandBuffer[AES_KEY_SIZE];
	unsigned char m_ivecEnc[AES_BLOCK_SIZE];
	unsigned char m_ivecDec[AES_BLOCK_SIZE];
public:
	int AesEncrypt(const unsigned char* plaintext, unsigned int plaintextLen, unsigned char* ciphertext);
	int AesDecrypt(const unsigned char* ciphertext, unsigned int ciphertextLen, unsigned char* plaintext);
	int KeyGen();
	int PrintKey();
	int RandomIVEC();
	int SetKey(unsigned char* pszAesKey);
	int SetIvec(unsigned char* pszIvec);
};