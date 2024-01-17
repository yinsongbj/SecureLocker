#include "pch.h"

#include "CRsaKey.h"
#include "CPasswordDlg.h"
#include "CKeyGenDlg.h"

CRsaKey::CRsaKey()
{
    m_KeyPublic = NULL;
    m_KeyPrivate = NULL;
    // Get default file name
    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, m_szSavePath);
    strcat_s(m_szSavePath, "\\SecureLocker\\");
    strcpy_s(m_szFriendsPath, m_szSavePath);
    strcat_s(m_szFriendsPath, "Keys\\");
    CreateDirectoryA(m_szSavePath, NULL);
    CreateDirectoryA(m_szFriendsPath, NULL);
    strcpy_s(m_szPrivateKey, m_szSavePath);
    strcpy_s(m_szPublicKey, m_szSavePath);
    strcat_s(m_szPrivateKey, PRIVATE_KEY_FILENAME);
    strcat_s(m_szPublicKey, PUBLIC_KEY_FILENAME);

    LoadFriendKeys();
}

CRsaKey::~CRsaKey()
{
    ReleaseKey();
}

/*
callback回调函数定义：
typedef int pem_password_cb (char *buf, int size, int rwflag, void *userdata);
buf参数表示PEM_do_header()为回调提供的口令接收缓冲区。
size参数为口令缓冲区的长度，通常为1024字节。
rwflag参数为加解密选项，1表示加密，0表示解密。
userdata为回调参数，由PEM_do_header()传入。
*/
int CRsaKey::PassCallback(char* buf, int size, int rwflag, void* u)
{
    //*
    if (1 == rwflag) {
        CKeyGenDlg dlg;
        if (IDOK != dlg.DoModal()) {
            return 0;
        }
        else {
            int len = strlen(dlg.m_szPassword);
            if (len <= 0) return 0;
            // 如果口令超出给定长度，就把多余的删掉
            if (len > size) len = size;
            memcpy(buf, dlg.m_szPassword, len);
            return len;
        }
    }
    else {
        CPasswordDlg dlg;
        if (IDOK != dlg.DoModal()) {
            return 0;
        }
        else {
            int len = strlen(dlg.m_szPassword);
            if (len <= 0) return 0;
            // 如果口令超出给定长度，就把多余的删掉
            if (len > size) len = size;
            memcpy(buf, dlg.m_szPassword, len);
            return len;
        }
    }
    /*/
    char szPassword[32] = { 0 };
    unsigned int nPasswordLen = sizeof(szPassword)/ sizeof(szPassword[0]);
    printf("Please input your password for RSA Key: ");
    scanf_s("%s", szPassword, nPasswordLen);
    int len = strlen(szPassword);
    if (len <= 0) return 0;
    // 如果口令超出给定长度，就把多余的删掉
    if (len > size) len = size;
    memcpy(buf, szPassword, len);
    return len;
    */
}

int CRsaKey::KeyGen()
{
    ReleaseKey();
    m_KeyPrivate = RSA_new();
    BIGNUM* bn = BN_new();
    int ret = 0;

    BN_set_word(bn, RSA_F4);
    ret = RSA_generate_key_ex(m_KeyPrivate, 4096, bn, NULL);
    if (ret != 1) {
        printf("Failed to generate RSA key pair.\n");
        return 0;
    }

    // 写入私钥
    BIO* bio = BIO_new_file(m_szPrivateKey, "w+");
    ret = PEM_write_bio_RSAPrivateKey(bio, m_KeyPrivate, EVP_des_ede3_cbc(), NULL, 0, PassCallback, NULL);
    BIO_free(bio);

    // 写入公钥
    bio = BIO_new_file(m_szPublicKey, "w+");
    ret = PEM_write_bio_RSAPublicKey(bio, m_KeyPrivate);
    BIO_free(bio);

    //RSA_free(m_Key);
    BN_free(bn);
    return 1;
}


int CRsaKey::LoadKey()
{
    // release key if loaded
    if (m_KeyPrivate) {
        RSA_free(m_KeyPrivate);
        m_KeyPrivate = NULL;
    }
    if (m_KeyPublic)
    {
        RSA_free(m_KeyPublic);
        m_KeyPublic = NULL;
    }
    // Read private key from a file.
    BIO* bio = BIO_new_file(m_szPrivateKey, "r");
    PEM_read_bio_RSAPrivateKey(bio, &m_KeyPrivate, PassCallback, NULL);
    BIO_free(bio);
    // Read public key from a file.
    bio = BIO_new_file(m_szPublicKey, "r");
    PEM_read_bio_RSAPublicKey(bio, &m_KeyPublic, NULL, NULL);
    BIO_free(bio);
    if (m_KeyPrivate && m_KeyPublic) {
        return 1;
    }
    else {
        ReleaseKey();
        return 0;
    }
}

int CRsaKey::LoadPrivateKey(LPCSTR pszPrivateKey)
{
    // release key if loaded
    if (m_KeyPrivate) {
        RSA_free(m_KeyPrivate);
        m_KeyPrivate = NULL;
    }
    // Read private key from a file.
    if (NULL == pszPrivateKey) {
        pszPrivateKey = m_szPrivateKey;
    }
    BIO* bio = BIO_new_file(m_szPrivateKey, "r");
    PEM_read_bio_RSAPrivateKey(bio, &m_KeyPrivate, PassCallback, NULL);
    BIO_free(bio);
    if (m_KeyPrivate) {
        return 1;
    }
    else {
        return 0;
    }
}

int CRsaKey::LoadPublicKey(LPCSTR pszPublicKey)
{
    // release key if loaded
    if (m_KeyPublic)
    {
        RSA_free(m_KeyPublic);
        m_KeyPublic = NULL;
    }
    // Read public key from a file.
    if (NULL == pszPublicKey) {
        pszPublicKey = m_szPublicKey;
    }
    BIO*  bio = BIO_new_file(pszPublicKey, "r");
    PEM_read_bio_RSAPublicKey(bio, &m_KeyPublic, NULL, NULL);
    BIO_free(bio);
    if (m_KeyPublic) {
        return 1;
    }
    else {
        return 0;
    }
}


/***************************
**2------------公钥加密
****************************/
unsigned char* CRsaKey::EncryptByPublicKey(const unsigned char* szPlainText, int nPlainTextLen, unsigned char* szCipherText, int* nCypherTextLen)
{
    int rsa_len = RSA_size(m_KeyPublic);

    //为加密后的内容 申请空间（根据秘钥的长度）
    int enLen = rsa_len;
    if (*nCypherTextLen < enLen)
    {
        *nCypherTextLen = enLen;
        return NULL;
    }
    //RSA_public_encrypt-->加解密函数
    //函数原型：int RSA_public_encrypt(intflen, const unsigned char *from, unsigned char *to, RSA *rsa,int padding); 
    //参数1：密钥长度          参数2：加密的内容            参数3:为加密后内容申请的空间       
    //参数4：密钥            参数5:有三种模式
    int nRet = RSA_public_encrypt(rsa_len, (unsigned char*)szPlainText, (unsigned char*)szCipherText, m_KeyPublic, RSA_NO_PADDING);
    if (nRet < 0) {
        perror(" RSA_public_encrypt error");
        //printf("========3===\n");
        return NULL;
    }
    //printf("===========en %s \n", szCipherText);

    return szCipherText;
}

/***************************
**3------------私钥加密
****************************/
char* CRsaKey::EncryptByPrivateKey(char* str)
{
    char* en = NULL;
    int len = strlen(str);
    int rsa_len = RSA_size(m_KeyPrivate);

    //为加密后的内容 申请空间（根据秘钥的长度+1）
    en = (char*)malloc(rsa_len + 1);
    if (en) {
        memset(en, 0, rsa_len + 1);
        //RSA_private_encrypt-->加解密函数
        //函数原型：int RSA_private_encrypt(intflen, const unsigned char *from, unsigned char *to, RSA *rsa,int padding); 
        //参数1：密钥长度          参数2：加密的内容            参数3:为加密后内容申请的空间       
        //参数4：密钥            参数5:有三种模式
        if (RSA_private_encrypt(rsa_len, (unsigned char*)str, (unsigned char*)en, m_KeyPublic, RSA_NO_PADDING) < 0) {
            perror(" RSA_private_encrypt error");
            printf("========3===\n");
            return NULL;
        }
        printf("===========en %s \n", en);
    }
    return en;
}

unsigned char* CRsaKey::DecryptByPrivateKey(const unsigned char* szCipherText, int nCypherTextLen, unsigned char* szPlainText, int* nPlainTextLen)
{
    int rsa_len = RSA_size(m_KeyPrivate);

    //为解密后的内容 申请空间（根据秘钥的长度）
    int deLen = rsa_len;
    if (*nPlainTextLen < deLen)
    {
        *nPlainTextLen = deLen;
        return NULL;
    }
    //解密函数
    int nRet = RSA_private_decrypt(rsa_len, szCipherText, szPlainText, m_KeyPrivate, RSA_NO_PADDING);
    if (nRet < 0) {
        perror(" RSA_private_decrypt error");
        return NULL;
    }
    return szPlainText;
}

char* CRsaKey::DecryptByPublicKey(char* str)
{
    int rsa_len = RSA_size(m_KeyPublic);

    //为解密后的内容 申请空间（根据秘钥的长度+1）
    char* de = (char*)malloc(rsa_len + 1);

    if (de) {
        memset(de, 0, rsa_len + 1);
        //解密函数
        if (RSA_public_decrypt(rsa_len, (unsigned char*)str, (unsigned char*)de, m_KeyPrivate, RSA_NO_PADDING) < 0) {
            perror(" RSA_public_decrypt error");
            return NULL;
        }
    }
    return de;
}


int CRsaKey::ReleaseKey()
{
    if (m_KeyPrivate) {
        RSA_free(m_KeyPrivate);
        m_KeyPrivate = NULL;
    }
    if (m_KeyPublic)
    {
        RSA_free(m_KeyPublic);
        m_KeyPublic = NULL;
    }
    return 0;
}


int CRsaKey::SetPrivateKey(char* pszPrivteKeyFileName)
{
    // release key if loaded
    if (m_KeyPrivate) {
        RSA_free(m_KeyPrivate);
        m_KeyPrivate = NULL;
    }
    // Read private key from a file.
    BIO* bio = BIO_new_file(pszPrivteKeyFileName, "r");
    PEM_read_bio_RSAPrivateKey(bio, &m_KeyPrivate, PassCallback, NULL);
    BIO_free(bio);
    if (m_KeyPrivate) {
        return 1;
    }
    else {
        return 0;
    }
}


int CRsaKey::SetPublicKey(char* pszPublicKeyFileName)
{
    // release key if loaded
    if (m_KeyPublic)
    {
        RSA_free(m_KeyPublic);
        m_KeyPublic = NULL;
    }
    // Read private key from a file.
    BIO* bio = BIO_new_file(pszPublicKeyFileName, "r");
    PEM_read_bio_RSAPublicKey(bio, &m_KeyPublic, NULL, NULL);
    BIO_free(bio);
    if (m_KeyPublic) {
        return 1;
    }
    else {
        return 0;
    }
}


int CRsaKey::SavePublicKey()
{
    // 写入公钥
    BIO* bio = BIO_new_file(m_szPublicKey, "w+");
    int ret = PEM_write_bio_RSAPublicKey(bio, m_KeyPrivate);
    BIO_free(bio);
    return 1;
}


int CRsaKey::LoadFriendKeys()
{
    m_vtFriendKeys.clear();

    CHAR szFindName[MAX_PATH] = { 0 };
    WIN32_FIND_DATAA stFindData;

    strcpy_s(szFindName, m_szSavePath);
    strcat_s(szFindName, "Keys\\*.key");
    HANDLE hFind = FindFirstFileA(szFindName, &stFindData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    do
    {
        string strKeyFile = ""; // m_szFriendsPath;
        strKeyFile += stFindData.cFileName;
        m_vtFriendKeys.push_back(strKeyFile);
    } while (FindNextFileA(hFind, &stFindData));
    FindClose(hFind);
    return 1;
}
