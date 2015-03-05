
// MinesweeperDlg.h : 头文件
//
#include "Pane.h"

#pragma once


// CMinesweeperDlg 对话框
class CMinesweeperDlg : public CDialogEx
{
// 构造
public:
	CMinesweeperDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MINESWEEPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMyBut(UINT nID);    //动态添加按钮
	//afx_msg void OnRButtonDown(UINT flags, CPoint XY);
	CButton* NewMyButton(int nID, CRect rect, int nStyle);
	void LayMines();    //布雷
	void DisplayAll();    //显示所有方块信息
	void DisplayAround(int paneList);    //显示周围方块信息
	void AddBitMap(int paneList);    //为方块添加图片
	void SetMineNum();    //设置方块周围雷的数量
public:
	Pane **_paneList;    //方块对象指针数组
	CButton **_buttonArrayList;    //方块按钮控件指针数组
	int _buttonNum;    //按钮数量
	int _mineNum;    //炸弹数量
	int _remainMineNum;    //标记后剩余的炸弹数量
	int *mineList;    //雷的序列号（数组）
	static int _markedID;    //newbutton响应右键时，修改该值
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
