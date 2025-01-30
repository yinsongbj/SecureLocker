#include "pch.h"
#include "CEreaserEdit.h"
BEGIN_MESSAGE_MAP(CEreaserEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CEreaserEdit::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)
	{
		int nDrag; //��ק�ļ�������
		nDrag = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if (nDrag == 1)
		{
			// ����ק���ļ����ļ���
			TCHAR Path[MAX_PATH + 1] = { 0 };
			// �õ�����ק���ļ���
			DragQueryFile(hDropInfo, 0, Path, MAX_PATH);
			DWORD dwAttr = GetFileAttributes(Path);

			// ���ļ�����ʾ����
			SetWindowText(Path);

			DragFinish(hDropInfo);
		}
		else
		{
			MessageBox(_T("ֻ����קһ���ļ���"));
		}
		CEdit::OnDropFiles(hDropInfo);
	}
}
