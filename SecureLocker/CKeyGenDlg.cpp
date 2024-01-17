// CKeyGenDlg.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CKeyGenDlg.h"

// CKeyGenDlg 对话框

IMPLEMENT_DYNAMIC(CKeyGenDlg, CDialog)

CKeyGenDlg::CKeyGenDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_KEYGEN, pParent)
{

}

CKeyGenDlg::~CKeyGenDlg()
{
}

void CKeyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeyGenDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CKeyGenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKeyGenDlg 消息处理程序

void CKeyGenDlg::OnBnClickedOk()
{
	CHAR szPassword[255] = { 0 };
	CHAR szRePass[255] = { 0 };
	GetDlgItemTextA(this->m_hWnd, IDC_EDIT_PASSWORD, szPassword, sizeof(szPassword) - 1);
	GetDlgItemTextA(this->m_hWnd, IDC_EDIT_REPASSWORD, szRePass, sizeof(szRePass) - 1);
	if (0 != strcmp(szPassword, szRePass)) {
		AfxMessageBox(_T("两次输入的密码不一致，请重新输入。"), MB_OK+MB_ICONINFORMATION);
		return;
	}
	strcpy_s(m_szPassword, szPassword);
	CDialog::OnOK();
}
