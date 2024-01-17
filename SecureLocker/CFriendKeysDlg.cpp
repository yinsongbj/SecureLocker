// CFriendKeysDlg.cpp: 实现文件
//

#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "CFriendKeysDlg.h"


// CFriendKeysDlg 对话框

IMPLEMENT_DYNAMIC(CFriendKeysDlg, CDialog)

CFriendKeysDlg::CFriendKeysDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_FRIENDKEYS, pParent)
{
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, m_szSavePath);
	_tcscat_s(m_szSavePath, _T("\\SecureLocker\\Keys\\"));
	CreateDirectory(m_szSavePath, NULL);
}

CFriendKeysDlg::~CFriendKeysDlg()
{
}

void CFriendKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_KEYS, m_list);
}


BEGIN_MESSAGE_MAP(CFriendKeysDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, &CFriendKeysDlg::OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_DELKEY, &CFriendKeysDlg::OnBnClickedButtonDelkey)
END_MESSAGE_MAP()


// CFriendKeysDlg 消息处理程序


void CFriendKeysDlg::OnBnClickedButtonAddkey()
{
	TCHAR strFilter[] = _T("Key file (*.key)|*.key||");
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strTmpFile = dlg.GetPathName();
		TCHAR szFirendKey[MAX_PATH] = { 0 };
		_tcscpy_s(szFirendKey, m_szSavePath);
		_tcscat_s(szFirendKey, dlg.GetFileName());

		if (CopyFile(strTmpFile, szFirendKey, FALSE)) {
			LoadKeys();
			AfxMessageBox(_T("好友密钥导入成功！"), MB_OK | MB_ICONINFORMATION);
		}
	}
}


BOOL CFriendKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_imgList.Create(32, 32, ILC_COLOR32, 2, 6);
	HICON hIcon = theApp.LoadIcon(IDR_MAINFRAME);
	m_imgList.Add(hIcon);
	m_list.SetImageList(&m_imgList, LVSIL_SMALL);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("好友密钥"), 0, 300);
	LoadKeys();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CFriendKeysDlg::LoadKeys()
{
	TCHAR szTemp[MAX_PATH];
	m_rsa.LoadFriendKeys();
	m_list.DeleteAllItems();
	for (size_t i = 0; i < m_rsa.m_vtFriendKeys.size(); i++)
	{
		memset(szTemp, 0, sizeof(szTemp));
		MultiByteToWideChar(CP_ACP, 0, m_rsa.m_vtFriendKeys[i].c_str(), m_rsa.m_vtFriendKeys[i].length(), szTemp, MAX_PATH);
		int nItem = m_list.InsertItem(m_list.GetItemCount(), szTemp, 0);
	}
	return 1;
}

void CFriendKeysDlg::OnBnClickedButtonDelkey()
{
	if (IDYES == MessageBox(_T("您确定删除用户密钥吗？"), NULL, MB_YESNOCANCEL + MB_ICONQUESTION))
	{
		TCHAR szFileName[MAX_PATH] = { 0 };
		for (int i = 0; i < m_list.GetItemCount(); i++)
		{
			if (m_list.GetCheck(i)) {
				_tcscpy_s(szFileName, m_szSavePath);
				_tcscat_s(szFileName, m_list.GetItemText(i, 0));
				DeleteFile(szFileName);
			}
		}
		LoadKeys();
	}
}
