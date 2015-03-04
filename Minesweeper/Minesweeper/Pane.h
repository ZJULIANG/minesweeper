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
	//�жϸ÷����ǲ��ǵ���
	bool _isMine = false;
	//�÷�������
	int _paneList;
	//�÷�����Χ�׵�����
	int _paneNumber;
	//�����״̬���򿪣����������:open, normal, marker
	CString _state;
};


