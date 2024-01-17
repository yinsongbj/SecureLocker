// CPasswordDlg.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CPasswordDlg.h"


// CPasswordDlg 对话框

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PASSWORD, pParent)
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPasswordDlg 消息处理程序


void CPasswordDlg::OnBnClickedOk()
{
	GetDlgItemTextA(this->GetSafeHwnd(), IDC_EDIT_PASSWORD, m_szPassword, sizeof(m_szPassword));
	CDialog::OnOK();
}
