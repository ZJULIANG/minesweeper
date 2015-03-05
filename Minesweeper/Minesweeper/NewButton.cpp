// NewButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Minesweeper.h"
#include "NewButton.h"
#include "MinesweeperDlg.h"


// NewButton

IMPLEMENT_DYNAMIC(NewButton, CButton)

NewButton::NewButton()
:markedFlag(false)
{

}

NewButton::~NewButton()
{
}


BEGIN_MESSAGE_MAP(NewButton, CButton)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// NewButton ��Ϣ�������




void NewButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*CString str;
	str.Format(_T("NUM:%d"), id);
	MessageBox(str, L"Trigger"); */

	if (this->markedFlag == false)
	{
		this->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_Marked)));
		this->markedFlag = true;
	}
	else
	{
		this->SetBitmap(NULL);
		this->markedFlag = false;
	}
	CMinesweeperDlg::_markedID = GetDlgCtrlID();
	GetParent()->SendMessage(WM_RBUTTONDOWN);    //���Ҽ������Ϣ���ݸ�������
	CButton::OnRButtonDown(nFlags, point);
}
