
// MinesweeperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Minesweeper.h"
#include "MinesweeperDlg.h"
#include "afxdialogex.h"
#include "Pane.h"
#include <math.h> 
#include <cstdlib>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PRIMARYBUTTONNUM 100
#define PRIMARYMINENUM 10

int CMinesweeperDlg::_markedID = -1;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMinesweeperDlg 对话框



CMinesweeperDlg::CMinesweeperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMinesweeperDlg::IDD, pParent), _buttonNum(PRIMARYBUTTONNUM), _mineNum(PRIMARYMINENUM)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_buttonArrayList = new CButton*[_buttonNum];
	_paneList = new Pane*[_buttonNum];
	_remainMineNum = _mineNum;
}

void CMinesweeperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMinesweeperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMinesweeperDlg::OnBnClickedButton1)
	//动态添加按钮
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MYBUTTON, IDC_MYBUTTON + PRIMARYBUTTONNUM, &CMinesweeperDlg::OnMyBut)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CMinesweeperDlg 消息处理程序

BOOL CMinesweeperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMinesweeperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMinesweeperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMinesweeperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#pragma region 开始游戏按钮函数

void CMinesweeperDlg::OnBnClickedButton1()
{
	CPoint pt(65, 35);    //第一个按钮位置
	CSize sz(20, 20);    //按钮大小
	int lineNum = (int)sqrt((float)_buttonNum);    //每行的方块个数
	int initalPosX = pt.x;
	for (int i = 0; i < _buttonNum; i++)
	{
		if (i % lineNum == 0 && i != 0)
		{
			pt.y += 20;
			pt.x = initalPosX;
		}
		_paneList[i] = new Pane(pt, sz);
		_buttonArrayList[i] = _paneList[i]->CreatePane(IDC_MYBUTTON + i);
		_paneList[i]->_paneList = i;
		_paneList[i]->_state = "normal";
		pt.x += 20;
	}
	LayMines();
	SetMineNum();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
}

#pragma endregion

#pragma region 方块按钮函数(左键)

void CMinesweeperDlg::OnMyBut( UINT nID )
{
	int paneList = nID - IDC_MYBUTTON;
	if ( _paneList[paneList]->_isMine == true )
	{
		DisplayAll();
		return;
	}
	else
	{
		DisplayAround(paneList);
	}
}

#pragma endregion

#pragma region 方块按钮函数(右键)

#pragma endregion

#pragma region 随机布雷
void CMinesweeperDlg::LayMines()
{
	srand(time(0));
	mineList = new int[_mineNum];

	for (int k = 0; k < _mineNum; k++)
	{
		mineList[k] = -1;
	}

	////test
	//int lineNum = (int)sqrt((float)_buttonNum);
	//mineList[0] = lineNum - 1;
	//_paneList[mineList[0]]->_isMine = true;

	for (int i = 0; i < _mineNum; i++)
	{
		int paneList;
		bool bo = false;
		paneList = rand() % _buttonNum;
		for (int j = 0; j < _mineNum; j++)
		{
			if (paneList == mineList[j])
			{
				bo = true;
			}
		}

		if ( !bo )
		{
			mineList[i] = paneList;
			_paneList[paneList]->_isMine = true;
		}
		else
		{
			i--;
		}
	}
}
#pragma endregion

#pragma region 点击到雷时，显示所有方块信息
void CMinesweeperDlg::DisplayAll()
{
	for (int i = 0; i < _buttonNum; i++)
	{
		int NUMID = 0;
		_paneList[i]->_state = "open";
		if ( _paneList[i]->_isMine == true )
		{
			NUMID = IDB_MarkedWrong;
		}
		else
		{
			switch (_paneList[i]->_paneNumber)
			{
			case 1:
				NUMID = IDB_NUM1;
				break;
			case 2:
				NUMID = IDB_NUM2;
				break;
			case 3:
				NUMID = IDB_NUM3;
				break;
			case 4:
				NUMID = IDB_NUM4;
				break;
			case 5:
				NUMID = IDB_NUM5;
				break;
			case 6:
				NUMID = IDB_NUM6;
				break;
			case 7:
				NUMID = IDB_NUM7;
				break;
			case 8:
				NUMID = IDB_NUM8;
				break;
			case 0:
				NUMID = IDB_NUM0;
				break;
			}
		}
		if (NUMID != 0)
		{
			_buttonArrayList[i]->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(NUMID)));
			_buttonArrayList[i]->EnableWindow(0);
		}
	}
}
#pragma endregion

#pragma region 设置方块周围雷的数量
void CMinesweeperDlg::SetMineNum()
{
	int lineNum = (int)sqrt((float)_buttonNum);    //一行的方块数量
	for (int i = 0; i < _buttonNum; i++)
	{
		if ( _paneList[i]->_isMine == true )
		{
			continue;
		}

		#pragma region 角上四个按钮
		//第一个按钮
		if ( i == 0 )
		{
			for (int j = 0; j < _mineNum; j++)
			{
				if (mineList[j] == 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//第一行最后一个按钮
		if (i == lineNum - 1)
		{
			for (int j = 0; j < _mineNum; j++)
			{
				if (mineList[j] == i - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i + lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i + lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//第一列的最后一个按钮
		if (i == ( lineNum - 1 ) * lineNum)
		{
			for (int j = 0; j < _mineNum; j++)
			{
				if (mineList[j] == i + 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i - lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i - lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//最后一个按钮
		if (i == lineNum * lineNum - 1)
		{
			for (int j = 0; j < _mineNum; j++)
			{
				if (mineList[j] == i - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i - lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineList[j] == i - lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		#pragma endregion

        #pragma region 四条边
		//最左边一列
		if ( i % lineNum == 0 )
		{
			for (int j = 0; j < _mineNum; j++)
			{
				int mineNum = mineList[j];
				if (mineNum == i + 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//最右边一列
		if ( ( i + 1 ) % lineNum == 0 )
		{
			for (int j = 0; j < _mineNum; j++)
			{
				int mineNum = mineList[j];
				if (mineNum == i - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//最上边一行
		if (i < lineNum - 1)
		{
			for (int j = 0; j < _mineNum; j++)
			{
				int mineNum = mineList[j];
				if (mineNum == i + 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i + lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
		//最下边一行
		if ( i > lineNum * (lineNum - 1) )
		{
			for (int j = 0; j < _mineNum; j++)
			{
				int mineNum = mineList[j];
				if (mineNum == i + 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum - 1)
				{
					_paneList[i]->_paneNumber++;
				}
				else if (mineNum == i - lineNum + 1)
				{
					_paneList[i]->_paneNumber++;
				}
			}
			continue;
		}
        #pragma endregion
 
        #pragma region 中间各点
		for (int j = 0; j < _mineNum; j++)
		{
			int mineNum = mineList[j];
			if (mineNum == i + 1)
			{
				_paneList[i]->_paneNumber++;
			}
			else if (mineNum == i - 1)
			{
				_paneList[i]->_paneNumber++; 
			}
			else if (mineNum == i - lineNum)
			{ 
				_paneList[i]->_paneNumber++;
			}
			else if (mineNum == i - lineNum - 1)
			{
				_paneList[i]->_paneNumber++;
			}
			else if (mineNum == i - lineNum + 1)
			{
				_paneList[i]->_paneNumber++; 
			}
			else if (mineNum == i + lineNum)
			{ 
				_paneList[i]->_paneNumber++;
			}
			else if (mineNum == i + lineNum - 1)
			{ 
				_paneList[i]->_paneNumber++; 
			}
			else if (mineNum == i + lineNum + 1)
			{ 
				_paneList[i]->_paneNumber++;
			}
		}
        #pragma endregion
	}
}
#pragma endregion

#pragma region 点中空格时显示周围方块数字
void CMinesweeperDlg::DisplayAround(int paneList)
{
	if ( _paneList[paneList]->_state == "marker" )
	{
		return;
	}

	int lineNum = (int)sqrt((float)_buttonNum);    //一行的方块数量

    #pragma region 角上四个按钮
	//第一个按钮
	if ( paneList == 0 )
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if ( _paneList[paneList]->_paneNumber == 0 )
		{
			if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
			{
				DisplayAround(paneList + 1);
			}
			if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
			{
				DisplayAround(paneList + lineNum);
			}
			if (_paneList[paneList + lineNum + 1]->_isMine == false && _paneList[paneList + lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum + 1);
			}
		}
		return;
	}
	//第一行最后一个按钮
	if (paneList == lineNum - 1)
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
			{
				DisplayAround(paneList - 1);
			}
			if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
			{
				DisplayAround(paneList + lineNum);
			}
			if (_paneList[paneList + lineNum - 1]->_isMine == false && _paneList[paneList + lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum - 1);
			}
		}
		return;
	}
	//第一列的最后一个按钮
	if (paneList == lineNum * (lineNum - 1))
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
			{
				DisplayAround(paneList + 1);
			}
			if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
			{
				DisplayAround(paneList - lineNum);
			}
			if (_paneList[paneList - lineNum + 1]->_isMine == false && _paneList[paneList - lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum + 1);
			}
		}
		return;
	}
	//最后一个按钮
	if (paneList == lineNum * lineNum - 1)
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
			{
				DisplayAround(paneList - 1);
			}
			if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
			{
				DisplayAround(paneList - lineNum);
			}
			if (_paneList[paneList - lineNum - 1]->_isMine == false && _paneList[paneList - lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum - 1);
			}
		}
		return;
	}
    #pragma endregion

    #pragma region 四条边
	//最左边一列
	if (paneList % lineNum == 0 )
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
			{
				DisplayAround(paneList + 1);
			}
			if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
			{
				DisplayAround(paneList + lineNum);
			}
			if (_paneList[paneList + lineNum + 1]->_isMine == false && _paneList[paneList + lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum + 1);
			}
			if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
			{
				DisplayAround(paneList - lineNum);
			}
			if (_paneList[paneList - lineNum + 1]->_isMine == false && _paneList[paneList - lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum + 1);
			}
		}
		return;
	}
	//最右边一列
	if ( ( paneList + 1 ) % lineNum == 0)
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
			{
				DisplayAround(paneList - 1);
			}
			if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
			{
				DisplayAround(paneList + lineNum);
			}
			if (_paneList[paneList + lineNum - 1]->_isMine == false && _paneList[paneList + lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum - 1);
			}
			if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
			{
				DisplayAround(paneList - lineNum);
			}
			if (_paneList[paneList - lineNum - 1]->_isMine == false && _paneList[paneList - lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum - 1);
			}
		}
		return;
	}

	//最上边一行
	if ( paneList < lineNum - 1 )
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
			{
				DisplayAround(paneList - 1);
			}
			if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
			{
				DisplayAround(paneList + 1);
			}
			if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
			{
				DisplayAround(paneList + lineNum);
			}
			if (_paneList[paneList + lineNum - 1]->_isMine == false && _paneList[paneList + lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum - 1);
			}
			if (_paneList[paneList + lineNum + 1]->_isMine == false && _paneList[paneList + lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList + lineNum + 1);
			}
		}
		return;
	}


	//最下边一行
	if (paneList > lineNum * ( lineNum - 1 ))
	{
		_paneList[paneList]->_state = "open";
		AddBitMap(paneList);
		if (_paneList[paneList]->_paneNumber == 0)
		{
			if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
			{
				DisplayAround(paneList - 1);
			}
			if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
			{
				DisplayAround(paneList + 1);
			}
			if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
			{
				DisplayAround(paneList - lineNum);
			}
			if (_paneList[paneList - lineNum - 1]->_isMine == false && _paneList[paneList - lineNum - 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum - 1);
			}
			if (_paneList[paneList - lineNum + 1]->_isMine == false && _paneList[paneList - lineNum + 1]->_state != "open")
			{
				DisplayAround(paneList - lineNum + 1);
			}
		}
		return;
	}

    #pragma endregion

    #pragma region 中间各点
	
	_paneList[paneList]->_state = "open";
	AddBitMap(paneList);
	if (_paneList[paneList]->_paneNumber == 0)
	{
		if (_paneList[paneList - 1]->_isMine == false && _paneList[paneList - 1]->_state != "open")
		{
			DisplayAround(paneList - 1);
		}
		if (_paneList[paneList + 1]->_isMine == false && _paneList[paneList + 1]->_state != "open")
		{
			DisplayAround(paneList + 1);
		}
		if (_paneList[paneList - lineNum]->_isMine == false && _paneList[paneList - lineNum]->_state != "open")
		{
			DisplayAround(paneList - lineNum);
		}
		if (_paneList[paneList - lineNum - 1]->_isMine == false && _paneList[paneList - lineNum - 1]->_state != "open")
		{
			DisplayAround(paneList - lineNum - 1);
		}
		if (_paneList[paneList - lineNum + 1]->_isMine == false && _paneList[paneList - lineNum + 1]->_state != "open")
		{
			DisplayAround(paneList - lineNum + 1);
		}
		if (_paneList[paneList + lineNum]->_isMine == false && _paneList[paneList + lineNum]->_state != "open")
		{
			DisplayAround(paneList + lineNum);
		}
		if (_paneList[paneList + lineNum - 1]->_isMine == false && _paneList[paneList + lineNum - 1]->_state != "open")
		{
			DisplayAround(paneList + lineNum - 1);
		}
		if (_paneList[paneList + lineNum + 1]->_isMine == false && _paneList[paneList + lineNum + 1]->_state != "open")
		{
			DisplayAround(paneList + lineNum + 1);
		}
	}

    #pragma endregion
}
#pragma endregion

#pragma region 给方块添加图片

void CMinesweeperDlg::AddBitMap(int paneList)
{
	int NUMID = 0;
	switch (_paneList[paneList]->_paneNumber)
	{
	case 1:
		NUMID = IDB_NUM1;
		break;
	case 2:
		NUMID = IDB_NUM2;
		break;
	case 3:
		NUMID = IDB_NUM3;
		break;
	case 4:
		NUMID = IDB_NUM4;
		break;
	case 5:
		NUMID = IDB_NUM5;
		break;
	case 6:
		NUMID = IDB_NUM6;
		break;
	case 7:
		NUMID = IDB_NUM7;
		break;
	case 8:
		NUMID = IDB_NUM8;
		break;
	case 0:
		NUMID = IDB_NUM0;
		break;
	}
	if (NUMID != 0)
	{
		_buttonArrayList[paneList]->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(NUMID)));
		_buttonArrayList[paneList]->EnableWindow(0);
	}
}

#pragma endregion



void CMinesweeperDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//当单击newbutton按钮时，按钮会发送WM_RBUTTONDOWN消息给父窗口，父窗口接受到该消息后对全局变量markedID进行处理
	if ( _markedID != -1 )
	{
		int paneList = _markedID - IDC_MYBUTTON;
		if ( paneList >= _buttonNum || paneList < 0 )
		{
			return;
		}
		if ( _paneList[paneList]->_state == "normal" )
		{
			_paneList[paneList]->_state = "marker";
			for (int i = 0; i < _mineNum; i++)
			{
				if ( paneList == mineList[i] )
				{
					_remainMineNum--;
					break;
				}
			}

		}
		else if (_paneList[paneList]->_state == "marker")
		{
			_paneList[paneList]->_state = "normal";
			for (int i = 0; i < _mineNum; i++)
			{
				if (paneList == mineList[i])
				{
					_remainMineNum++;
					break;
				}
			}
		}
		if ( _remainMineNum == 0 )
		{
			MessageBox(L"Damn You Win!!!!!");
		}
		/*CString str = _paneList[paneList]->_state;
		MessageBox(str, L"Trigger");*/
	}
	_markedID = -1;    //若点击到按钮以外的部分，则将该全局变量值初始化
	//MessageBox(L"LFT Button Click!", L"Trigger");
	CDialogEx::OnRButtonDown(nFlags, point);
}
