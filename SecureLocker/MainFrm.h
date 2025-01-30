
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "CAesKey.h"
#include "CRsaKey.h"
// 自定义消息ID
#define WM_SHOWTASK (WM_USER +1) 

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CRsaKey m_rsa;
	CAesKey m_aes;
// 操作
public:
	int ToTray();
	void DeleteTray();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeymanger();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnFriendkeys();
	afx_msg void OnEncryptfile();
	afx_msg void OnDecryptfile();
	afx_msg void OnEreasefile();
	afx_msg void OnKeygen();
	afx_msg void OnKeyRemove();
	afx_msg void OnKeyImport();
	afx_msg void OnKeyExport();
	afx_msg void OnUpdateKeygen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKeyremove(CCmdUI* pCmdUI);
	afx_msg void OnKeypublic();
	afx_msg void OnPopmenuOpenfolder();
};


