#pragma once
#include "afxdialogex.h"
#include "CRsaKey.h"

// CEncryptDlg 对话框

class CEncryptDlg : public CDialog
{
	DECLARE_DYNAMIC(CEncryptDlg)

public:
	CEncryptDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEncryptDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ENCRYPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonUsermgr();
	virtual BOOL OnInitDialog();
private:
	CRsaKey m_rsa;
public:
	// 好友key列表
	CComboBox m_comboKeys;
	afx_msg void OnBnClickedButtonBrowser();
};
