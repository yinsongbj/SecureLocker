
// EreaserDlg.cpp: 实现文件
//
#include "pch.h"
#include "SecureLocker.h"
#include "afxdialogex.h"
#include "EreaserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CEreaserDlg 对话框



CEreaserDlg::CEreaserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EREASER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEreaserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_Edit);
}

BEGIN_MESSAGE_MAP(CEreaserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEreaserDlg::OnBnClickedOk)
//	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CEreaserDlg 消息处理程序

BOOL CEreaserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	CWnd* pEdit = GetDlgItem(IDC_EDIT_FILENAME);
	DWORD dwStyle = pEdit->GetExStyle();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEreaserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEreaserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


long file_size(FILE* f)
{
	long len;
	fseek(f, 0, SEEK_END);    /* jump to the and of file */
	len = ftell(f);
	fseek(f, 0, SEEK_SET);    /* restore */
	return len;    /*感谢网友冰尘醉*/
}

void block_wipe(FILE* f, char c)
{
	long len = file_size(f);

	fwrite(&c, sizeof(char), len, f);    /* 覆盖，直接一次性写入 */

}

static BOOL DeepEreaseFile(LPCTSTR pszFileName)
{
	char szFileName[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, pszFileName, (int)wcslen(pszFileName), szFileName, sizeof(szFileName), NULL, NULL);
	int i, j;
	srand((int)time(NULL));    /* randomize */

	FILE* f;
	for (i = 1; i < 7; ++i) {
		/* process each file */
		fopen_s(&f, szFileName, "r+b");
		if (f == NULL) {/* fail to open file */
			// fprintf(stderr, "Error when open %s:\n", pszFileName);
			return FALSE;
		}

		for (j = 0; j < 3; ++j) {
			/* DOD5220.22M Step 1 */
			/* v 0.2.0 新增*/
			block_wipe(f, 0x00);
			block_wipe(f, 0x01);
		}

		block_wipe(f, rand() % 256);    /* Step 2 */

		if (rename(szFileName, "C")) {
			/* Step 3*/
			// fprintf(stderr, "Error when rename %s\n", pszFileName);
			return FALSE;

			/* XXX:文件名冲突的解决？可以考虑使用tmpnam()吗？*/
		}

		remove("C"); /* XXX:如果是一个符号连接怎样保证删除的是真正的文件？ */
		fclose(f);
	}
	return TRUE;
}

BOOL EreaseFile(LPCTSTR szFileName)
{
	CFile f;
	CFileException e;
	if (!f.Open(szFileName, CFile::modeReadWrite, &e)) {
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
		return FALSE;
	}
	else {
		char szBuffer[4 * 1024] = {0};
		ULONGLONG ullLen = f.GetLength();
		ULONGLONG ullWrite = 0;
		for (int i = 0; i < sizeof(szBuffer); i++)
		{
			szBuffer[i] = rand();
		}
		for (int i = 0; i < 7; i++)
		{
			f.SeekToBegin();
			while (true)
			{
				f.Write(szBuffer, sizeof(szBuffer));
				ullWrite += sizeof(szBuffer);
				if (ullWrite >= ullLen)
				{
					break;
				}
			}
		}
		f.Close();
		DeleteFile(szFileName);
	}
	return TRUE;
}

BOOL EreasePath(LPCTSTR szPath)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	TCHAR szScanPath[MAX_PATH];
	_stprintf_s(szScanPath, _T("%s\\*"), szPath);

	hFind = FindFirstFile(szScanPath, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf("  %s   <DIR>\n", ffd.cFileName);
			if (0 == _tcscmp(_T("."), ffd.cFileName) || 0 == _tcscmp(_T(".."), ffd.cFileName)) {
				continue;
			}
			else {
				TCHAR szNewPath[MAX_PATH];
				_stprintf_s(szNewPath, _T("%s\\%s"), szPath, ffd.cFileName);
				EreasePath(szNewPath);
			}
		}
		else
		{
			TCHAR szFileName[MAX_PATH];
			_stprintf_s(szFileName, _T("%s\\%s"), szPath, ffd.cFileName);
			EreaseFile(szFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return TRUE;
}

UINT CEreaserDlg::EreaserProc(LPVOID p)
{
	CEreaserDlg* pDlg = (CEreaserDlg*)p;
	CString strFileName;
	pDlg->GetDlgItemText(IDC_EDIT_FILENAME, strFileName);

	DWORD dwAttr = GetFileAttributes(strFileName);
	BOOL fBit = dwAttr & FILE_ATTRIBUTE_DIRECTORY;
	if (fBit){
		if (!EreasePath(strFileName)) {
			pDlg->MessageBox(_T("目录擦除失败！"), 0, MB_ICONWARNING);
			return 0;
		}
	}
	else {
		if (!EreaseFile(strFileName)) {
			pDlg->MessageBox(_T("文件擦除失败！"), 0, MB_ICONWARNING);
			return 0;
		}
	}
	MessageBeep(MB_OK);
	pDlg->GetDlgItem(IDOK)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	pDlg->MessageBox(_T("擦除文件成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
	
	return 0;
}


void CEreaserDlg::OnBnClickedOk()
{
	// disable button
	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(FALSE);
	// start process in thread
	AfxBeginThread(EreaserProc, this);
}