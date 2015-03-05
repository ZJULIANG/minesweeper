#pragma once
// NewButton

class NewButton : public CButton
{
	DECLARE_DYNAMIC(NewButton)

public:
	NewButton();
	virtual ~NewButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	bool markedFlag;
};


