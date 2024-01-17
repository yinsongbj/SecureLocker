// CEncryptDlg.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CEncryptDlg.h"
#include "CFriendKeysDlg.h"

#include "CFileLocker.h"

// CEncryptDlg 对话框

IMPLEMENT_DYNAMIC(CEncryptDlg, CDialog)

CEncryptDlg::CEncryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_ENCRYPT, pParent)
{

}

CEncryptDlg::~CEncryptDlg()
{
}

void CEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERKEYS, m_comboKeys);
}


BEGIN_MESSAGE_MAP(CEncryptDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEncryptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_USERMGR, &CEncryptDlg::OnBnClickedButtonUsermgr)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CEncryptDlg::OnBnClickedButtonBrowser)
END_MESSAGE_MAP()


// CEncryptDlg 消息处理程序


void CEncryptDlg::OnBnClickedOk()
{
	CFileLocker locker;
	TCHAR szFileName[MAX_PATH];
	CHAR szPublicKey[MAX_PATH];
	GetDlgItemText(IDC_EDIT_FILENAME, szFileName, sizeof(szFileName)/ sizeof(szFileName[0]));
	int nIndex = m_comboKeys.GetCurSel();
	strcpy_s(szPublicKey, m_rsa.m_szFriendsPath);
	strcat_s(szPublicKey, m_rsa.m_vtFriendKeys[nIndex].c_str());
	if (locker.LockFile(szFileName, szPublicKey)) {
		AfxMessageBox(_T("文件加密成功！"), MB_OK | MB_ICONINFORMATION);
		CDialog::OnOK();
	}
	else {
		AfxMessageBox(_T("文件加密失败"), MB_OK | MB_ICONERROR);
	}
}

void CEncryptDlg::OnBnClickedButtonUsermgr()
{
	CFriendKeysDlg dlg;
	dlg.DoModal();
}


BOOL CEncryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCHAR szTemp[MAX_PATH];
	m_rsa.LoadFriendKeys();
	for (size_t i = 0; i < m_rsa.m_vtFriendKeys.size(); i++)
	{
		memset(szTemp, 0, sizeof(szTemp));
		MultiByteToWideChar(CP_ACP, 0, m_rsa.m_vtFriendKeys[i].c_str(), m_rsa.m_vtFriendKeys[i].length(), szTemp, MAX_PATH);
		m_comboKeys.InsertString(i, szTemp);
	}
	m_comboKeys.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEncryptDlg::OnBnClickedButtonBrowser()
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
