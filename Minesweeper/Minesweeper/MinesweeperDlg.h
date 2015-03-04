
// MinesweeperDlg.h : ͷ�ļ�
//
#include "Pane.h"

#pragma once


// CMinesweeperDlg �Ի���
class CMinesweeperDlg : public CDialogEx
{
// ����
public:
	CMinesweeperDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MINESWEEPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMyBut(UINT nID);    //��̬��Ӱ�ť
	afx_msg void RightClickButton(UINT nID);    //��̬��Ӱ�ť
	CButton* NewMyButton(int nID, CRect rect, int nStyle);
	void LayMines();    //����
	void DisplayAll();    //��ʾ���з�����Ϣ
	void DisplayAround(int paneList);    //��ʾ��Χ������Ϣ
	void AddBitMap(int paneList);    //Ϊ�������ͼƬ
	void SetMineNum();    //���÷�����Χ�׵�����
public:
	Pane **_paneList;    //�������ָ������
	CButton **_buttonArrayList;    //���鰴ť�ؼ�ָ������
	int _buttonNum;    //��ť����
	int _mineNum;    //ը������
	int *mineList;    //�׵����кţ����飩
};
