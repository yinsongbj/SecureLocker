#pragma once
#include "afxdialogex.h"
#include "CRsaKey.h"

// CDecryptDlg 对话框

class CDecryptDlg : public CDialog
{
	DECLARE_DYNAMIC(CDecryptDlg)

public:
	CDecryptDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDecryptDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DECRYPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowser();
	afx_msg void OnBnClickedOk();
private:
	CRsaKey m_rsa;
};
