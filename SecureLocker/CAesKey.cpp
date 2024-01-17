#include "pch.h"

#include "CAesKey.h"
#include <stdlib.h>
#include<string.h>
#include <time.h>
#include <stdio.h>


CAesKey::CAesKey()
{
    memset(m_RandBuffer, 0, sizeof(m_RandBuffer));
    KeyGen();
    RandomIVEC();
}
CAesKey::~CAesKey()
{

}

// 加密函数
int CAesKey::AesEncrypt(const unsigned char* plaintext, unsigned int plaintextLen, unsigned char* ciphertext) {
    AES_KEY aes_key;
    int num = 0;
    AES_set_encrypt_key(m_RandBuffer, AES_KEY_SIZE, &aes_key);
    AES_cfb128_encrypt(plaintext, ciphertext, plaintextLen, &aes_key, m_ivecEnc, &num, AES_ENCRYPT);

    /*
    unsigned char szTest[128] = { 0 };
    int cipherLen = num;
    num = 0;
    AES_set_encrypt_key(m_RandBuffer, AES_KEY_SIZE, &aes_key);
    AES_cfb128_encrypt(ciphertext, szTest, cipherLen, &aes_key, m_ivecDec, &num, AES_DECRYPT);
    */
    return num;
}

// 解密函数
int CAesKey::AesDecrypt(const unsigned char* ciphertext, unsigned int ciphertextLen, unsigned char* plaintext) {
    AES_KEY aes_key;
    int num = 0;
    AES_set_encrypt_key(m_RandBuffer, AES_KEY_SIZE, &aes_key);
    AES_cfb128_encrypt(ciphertext, plaintext, ciphertextLen, &aes_key, m_ivecDec, &num, AES_DECRYPT);
    return num;
}

int CAesKey::KeyGen()
{
    srand(time(NULL));
    for (int i = 0; i < AES_KEY_SIZE; i++)
    {
        m_RandBuffer[i] = rand();
    }
    // for test
    // memset(m_RandBuffer, 0, sizeof(m_RandBuffer));
    return 1;
}


int CAesKey::PrintKey()
{
    printf("AES密钥：");
    for (size_t i = 0; i < AES_KEY_SIZE; i++) {
        printf("%02x", m_RandBuffer[i]);
    }
    printf("\n");
    printf("ivec：");
    for (size_t i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x", m_ivecEnc[i]);
    }
    printf("\n");
    return 0;
}


int CAesKey::RandomIVEC()
{
    srand(time(NULL));
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
    {
        m_ivecEnc[i] = rand();
    }
    // for test
    //memset(m_ivecEnc, 0, sizeof(m_ivecEnc));
    memcpy(m_ivecDec, m_ivecEnc, sizeof(m_ivecDec));
    return 1;
}


int CAesKey::SetKey(unsigned char* pszAesKey)
{
    memcpy(m_RandBuffer, pszAesKey, sizeof(m_RandBuffer));
    return 0;
}


int CAesKey::SetIvec(unsigned char* pszIvec)
{
    memcpy(m_ivecEnc, pszIvec, sizeof(m_ivecEnc));
    memcpy(m_ivecDec, pszIvec, sizeof(m_ivecEnc));
    return 0;
}