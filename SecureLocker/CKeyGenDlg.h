#pragma once
#include "afxdialogex.h"


// CKeyGenDlg 对话框

class CKeyGenDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyGenDlg)

public:
	CKeyGenDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKeyGenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KEYGEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	char m_szPassword[1024];
};
