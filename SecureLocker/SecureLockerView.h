﻿
// SecureLockerView.h: CSecureLockerView 类的接口
//

#pragma once


class CSecureLockerView : public CView
{
protected: // 仅从序列化创建
	CSecureLockerView() noexcept;
	DECLARE_DYNCREATE(CSecureLockerView)

// 特性
public:
	CSecureLockerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CSecureLockerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SecureLockerView.cpp 中的调试版本
inline CSecureLockerDoc* CSecureLockerView::GetDocument() const
   { return reinterpret_cast<CSecureLockerDoc*>(m_pDocument); }
#endif

