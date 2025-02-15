#include "pch.h"
#include "CEreaserEdit.h"
BEGIN_MESSAGE_MAP(CEreaserEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CEreaserEdit::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)
	{
		int nDrag; //拖拽文件的数量
		nDrag = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if (nDrag == 1)
		{
			// 被拖拽的文件的文件名
			TCHAR Path[MAX_PATH + 1] = { 0 };
			// 得到被拖拽的文件名
			DragQueryFile(hDropInfo, 0, Path, MAX_PATH);
			DWORD dwAttr = GetFileAttributes(Path);

			// 把文件名显示出来
			SetWindowText(Path);

			DragFinish(hDropInfo);
		}
		else
		{
			MessageBox(_T("只能拖拽一个文件！"));
		}
		CEdit::OnDropFiles(hDropInfo);
	}
}
