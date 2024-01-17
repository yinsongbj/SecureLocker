// CKeyManger.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CKeyMangerDlg.h"
#include "CKeyGenDlg.h"


// CKeyManger 对话框

IMPLEMENT_DYNAMIC(CKeyMangerDlg, CDialog)

CKeyMangerDlg::CKeyMangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_KEYMGR, pParent)
{

}

CKeyMangerDlg::~CKeyMangerDlg()
{
}

void CKeyMangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeyMangerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GENKEY, &CKeyMangerDlg::OnBnClickedButtonGenkey)
END_MESSAGE_MAP()


// CKeyManger 消息处理程序


void CKeyMangerDlg::OnBnClickedButtonGenkey()
{
	CKeyGenDlg dlg;
	dlg.DoModal();
}
