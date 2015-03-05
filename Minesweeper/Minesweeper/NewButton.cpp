// NewButton.cpp : 实现文件
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



// NewButton 消息处理程序




void NewButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	GetParent()->SendMessage(WM_RBUTTONDOWN);    //将右键点击消息传递给父窗口
	CButton::OnRButtonDown(nFlags, point);
}
