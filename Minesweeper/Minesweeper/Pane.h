#pragma once


// Pane

class Pane : public CButton
{
	DECLARE_DYNAMIC(Pane)

public:
	Pane();
	Pane(CPoint pt, CSize sz);
	virtual ~Pane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CButton* CreatePane(int nID);
	afx_msg void OnMyBut();

public:
	CPoint _pt;
	CSize _sz;
	//判断该方格是不是地雷
	bool _isMine = false;
	//该方格的序号
	int _paneList;
	//该方格周围雷的数量
	int _paneNumber;
	//方格的状态，打开，正常，标记:open, normal, marker
	CString _state;
};


