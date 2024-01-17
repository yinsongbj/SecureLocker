// CDecryptDlg.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CDecryptDlg.h"
#include "CFileLocker.h"


// CDecryptDlg 对话框

IMPLEMENT_DYNAMIC(CDecryptDlg, CDialog)

CDecryptDlg::CDecryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_DECRYPT, pParent)
{

}

CDecryptDlg::~CDecryptDlg()
{
}

void CDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDecryptDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CDecryptDlg::OnBnClickedButtonBrowser)
	ON_BN_CLICKED(IDOK, &CDecryptDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDecryptDlg 消息处理程序


void CDecryptDlg::OnBnClickedButtonBrowser()
{
	TCHAR strFilter[] = _T("All file (*.*)|*.*||");
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strTmpFile = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_FILENAME, strTmpFile);
	}
}


void CDecryptDlg::OnBnClickedOk()
{
	CFileLocker locker;
	TCHAR szFileName[MAX_PATH];
	CHAR szPublicKey[MAX_PATH];
	GetDlgItemText(IDC_EDIT_FILENAME, szFileName, sizeof(szFileName) / sizeof(szFileName[0]));

	if (locker.UnlockFile(szFileName)) {
		AfxMessageBox(_T("文件解密成功！"), MB_OK | MB_ICONINFORMATION);
		CDialog::OnOK();
	}
	else {
		AfxMessageBox(_T("文件解密失败"), MB_OK | MB_ICONERROR);
	}
}
