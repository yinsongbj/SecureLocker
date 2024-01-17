
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "SecureLocker.h"

#include "MainFrm.h"
#include "CKeyMangerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CFriendKeysDlg.h"
#include "CDecryptDlg.h"
#include "CEncryptDlg.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_KEYMANGER, &CMainFrame::OnKeymanger)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_COMMAND(ID_FRIENDKEYS, &CMainFrame::OnFriendkeys)
	ON_COMMAND(ID_ENCRYPTFILE, &CMainFrame::OnEncryptfile)
	ON_COMMAND(ID_DECRYPTFILE, &CMainFrame::OnDecryptfile)
	ON_COMMAND(ID_KEYGEN, &CMainFrame::OnKeygen)
	ON_COMMAND(ID_KEYREMOVE, &CMainFrame::OnKeyRemove)
	ON_COMMAND(ID_KEYIMPORT, &CMainFrame::OnKeyImport)
	ON_COMMAND(ID_KEYEXPORT, &CMainFrame::OnKeyExport)
	ON_UPDATE_COMMAND_UI(ID_KEYGEN, &CMainFrame::OnUpdateKeygen)
	ON_UPDATE_COMMAND_UI(ID_KEYREMOVE, &CMainFrame::OnUpdateKeyremove)
	ON_COMMAND(ID_KEYPUBLIC, &CMainFrame::OnKeypublic)
	ON_COMMAND(ID_POPMENU_OPENFOLDER, &CMainFrame::OnPopmenuOpenfolder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
	DeleteTray();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	ToTray();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



void CMainFrame::OnKeymanger()
{
	CKeyMangerDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)
	{
		//最小化到托盘的函数
		//ToTray();
	}
	else {
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}

int CMainFrame::ToTray()
{
	CString strName;
	strName.LoadString(AFX_IDS_APP_TITLE);
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;     nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy_s(nid.szTip, sizeof(nid.szTip) / sizeof(nid.szTip[0]), strName);    //信息提示条   
	Shell_NotifyIcon(NIM_ADD, &nid);    //在托盘区添加图标   
	ShowWindow(SW_HIDE);    //隐藏主窗口
	return 0;
}

void CMainFrame::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;		//自定义的消息名称
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy_s(nid.szTip, sizeof(nid.szTip) / sizeof(nid.szTip[0]), _T("程序名称"));    		//信息提示条为“计划任务提醒”
	Shell_NotifyIcon(NIM_DELETE, &nid);	//在托盘区删除图标
}

//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
LRESULT CMainFrame::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
	{
		return 1;
	}
	switch (lParam)
	{
	case WM_RBUTTONUP:	//右键起来时弹出快捷菜单，这里只有一个“关闭”
	{
		//得到鼠标位置
		POINT p;
		::GetCursorPos(&p);

		//声明一个弹出式菜单
		CMenu menu, popMenu;
		menu.LoadMenu(IDR_MENUPOP);
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	}
	break;
	case WM_LBUTTONDBLCLK: //双击左键的处理
	{
		//this->ShowWindow(SW_SHOW);//显示主窗口
		//DeleteTray();
	}
	break;
	default:
		break;
	}
	return 0;
}

void CMainFrame::OnFriendkeys()
{
	CFriendKeysDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnEncryptfile()
{
	// TODO: 在此添加命令处理程序代码
	CEncryptDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnDecryptfile()
{
	CDecryptDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnKeygen()
{
	if (0 == m_rsa.KeyGen()) {
		AfxMessageBox(_T("创建私钥失败，请重新再试。"), MB_OK | MB_ICONERROR);
	}
	else {
		AfxMessageBox(_T("创建私钥成功。"), MB_OK | MB_ICONINFORMATION);
	}
}


void CMainFrame::OnKeyRemove()
{
	if (IDYES == MessageBox(_T("为了确保删除密钥，需要您确认密钥的密码。"), NULL, MB_YESNO + MB_ICONQUESTION))
	{
		if (m_rsa.LoadPrivateKey()) {
			if (IDYES == MessageBox(_T("您确定删除密钥吗？如果没有导出备份，一旦删除密钥无法恢复。"), NULL, MB_YESNOCANCEL + MB_ICONQUESTION))
			{
				m_rsa.ReleaseKey();
				DeleteFileA(m_rsa.m_szPrivateKey);
				DeleteFileA(m_rsa.m_szPublicKey);
				AfxMessageBox(_T("密钥文件已经删除。", MB_OK + MB_ICONINFORMATION));
			}
		}
	}
}


void CMainFrame::OnKeyImport()
{
	TCHAR strFilter[] = _T("Key file (*.key)|*.key||");
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strTmpFile = dlg.GetPathName();
		TCHAR szPrivateKey[MAX_PATH] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, m_rsa.m_szPrivateKey, strlen(m_rsa.m_szPrivateKey) + 1, szPrivateKey, MAX_PATH);

		if (IDYES == MessageBox(_T("您确定导入密钥吗？导入密钥会覆盖原密钥文件，且原密钥文件无法恢复。"), NULL, MB_YESNOCANCEL + MB_ICONQUESTION))
		{
			if (CopyFile(strTmpFile, szPrivateKey, FALSE)) {
				if (m_rsa.LoadPrivateKey()) {
					m_rsa.SavePublicKey();
					AfxMessageBox(_T("文件导入成功！"), MB_OK | MB_ICONINFORMATION);
				}
				else {
					AfxMessageBox(_T("密钥文件加载错误，导入失败！"), MB_OK | MB_ICONINFORMATION);
				}
				return;
			}
			AfxMessageBox(_T("文件导入失败！"), MB_OK | MB_ICONINFORMATION);
		}
	}
}

void CMainFrame::OnKeyExport()
{
	TCHAR szFilter[] = _T("Key file (*.key)|*.key||");
	CFileDialog dlg(FALSE, _T("xlsx"), _T("默认文件名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strTmpFile = dlg.GetPathName();
		TCHAR szPrivateKey[MAX_PATH] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, m_rsa.m_szPrivateKey, strlen(m_rsa.m_szPrivateKey) + 1, szPrivateKey, MAX_PATH);
		if (CopyFile(szPrivateKey, strTmpFile, TRUE)) {
			AfxMessageBox(_T("文件导出成功！"), MB_OK | MB_ICONINFORMATION);
		}
	}
}

void CMainFrame::OnUpdateKeygen(CCmdUI* pCmdUI)
{
	BOOL fEnable = FALSE;
	DWORD dwAttrib = GetFileAttributesA(m_rsa.m_szPrivateKey);
	if (INVALID_FILE_ATTRIBUTES == dwAttrib || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
		fEnable = TRUE;
	}
	pCmdUI->Enable(fEnable);
}


void CMainFrame::OnUpdateKeyremove(CCmdUI* pCmdUI)
{
	BOOL fEnable = TRUE;
	DWORD dwAttrib = GetFileAttributesA(m_rsa.m_szPrivateKey);
	if (INVALID_FILE_ATTRIBUTES == dwAttrib || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
		fEnable = FALSE;
	}
	pCmdUI->Enable(fEnable);
}

void CMainFrame::OnKeypublic()
{
	TCHAR szFilter[] = _T("Key file (*.key)|*.key||");
	CFileDialog dlg(FALSE, _T("xlsx"), _T("默认文件名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strTmpFile = dlg.GetPathName();
		TCHAR szPublicKey[MAX_PATH] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, m_rsa.m_szPublicKey, strlen(m_rsa.m_szPublicKey) + 1, szPublicKey, MAX_PATH);
		if (CopyFile(szPublicKey, strTmpFile, TRUE)) {
			AfxMessageBox(_T("分享文件导出成功！"), MB_OK | MB_ICONINFORMATION);
		}
	}
}


void CMainFrame::OnPopmenuOpenfolder()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, m_rsa.m_szSavePath, strlen(m_rsa.m_szSavePath) + 1, szPath, MAX_PATH);
	ShellExecute(NULL, _T("open"), _T("Explorer.exe"), szPath, NULL, SW_SHOWDEFAULT);
}
