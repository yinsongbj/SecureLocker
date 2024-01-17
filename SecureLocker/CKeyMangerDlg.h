﻿#pragma once
#include "afxdialogex.h"


// CKeyManger 对话框

class CKeyMangerDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyMangerDlg)

public:
	CKeyMangerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKeyMangerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KEYMANAGER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGenkey();
};
