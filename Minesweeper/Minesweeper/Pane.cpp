// Pane.cpp : 实现文件
//

#include "stdafx.h"
#include "Minesweeper.h"
#include "Pane.h"


// Pane

IMPLEMENT_DYNAMIC(Pane, CButton)

Pane::Pane()
:_paneNumber(0)
{

}

Pane::Pane(CPoint pt, CSize sz) :_paneNumber(0)
{
	_pt = pt;
	_sz = sz;
}

Pane::~Pane()
{
}

NewButton* Pane::CreatePane(int nID)
{
	CRect rect(_pt, _sz);
	CString m_Caption = _T("");
	NewButton *p_Button = new NewButton();
	ASSERT_VALID(p_Button);
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	CWnd* pWnd = FromHandle( hWnd );    //获取主窗口指针
	p_Button->Create(m_Caption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rect, pWnd, nID);
	return p_Button;
}



BEGIN_MESSAGE_MAP(Pane, CButton)
END_MESSAGE_MAP()



// Pane 消息处理程序


