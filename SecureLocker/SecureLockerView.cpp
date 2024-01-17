
// SecureLockerView.cpp: CSecureLockerView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SecureLocker.h"
#endif

#include "SecureLockerDoc.h"
#include "SecureLockerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSecureLockerView

IMPLEMENT_DYNCREATE(CSecureLockerView, CView)

BEGIN_MESSAGE_MAP(CSecureLockerView, CView)
END_MESSAGE_MAP()

// CSecureLockerView 构造/析构

CSecureLockerView::CSecureLockerView() noexcept
{
	// TODO: 在此处添加构造代码

}

CSecureLockerView::~CSecureLockerView()
{
}

BOOL CSecureLockerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSecureLockerView 绘图

void CSecureLockerView::OnDraw(CDC* /*pDC*/)
{
	CSecureLockerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSecureLockerView 诊断

#ifdef _DEBUG
void CSecureLockerView::AssertValid() const
{
	CView::AssertValid();
}

void CSecureLockerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSecureLockerDoc* CSecureLockerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSecureLockerDoc)));
	return (CSecureLockerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSecureLockerView 消息处理程序
