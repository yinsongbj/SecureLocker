#pragma once
#include "afxdialogex.h"
#include "CRsaKey.h"


// CFriendKeysDlg 对话框

class CFriendKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CFriendKeysDlg)

public:
	CFriendKeysDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFriendKeysDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FRIENDKEYS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddkey();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	int LoadKeys();
private:
	TCHAR m_szSavePath[MAX_PATH];
	CRsaKey m_rsa;
	CImageList m_imgList;
public:
	afx_msg void OnBnClickedButtonDelkey();
};
