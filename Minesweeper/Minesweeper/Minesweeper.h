
// Minesweeper.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMinesweeperApp: 
// �йش����ʵ�֣������ Minesweeper.cpp
//

class CMinesweeperApp : public CWinApp
{
public:
	CMinesweeperApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	

	DECLARE_MESSAGE_MAP()
};

extern CMinesweeperApp theApp;