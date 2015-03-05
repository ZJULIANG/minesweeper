
// MinesweeperDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMinesweeperDlg �Ի���



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
	//��̬��Ӱ�ť
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MYBUTTON, IDC_MYBUTTON + PRIMARYBUTTONNUM, &CMinesweeperDlg::OnMyBut)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CMinesweeperDlg ��Ϣ�������

BOOL CMinesweeperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMinesweeperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMinesweeperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#pragma region ��ʼ��Ϸ��ť����

void CMinesweeperDlg::OnBnClickedButton1()
{
	CPoint pt(65, 35);    //��һ����ťλ��
	CSize sz(20, 20);    //��ť��С
	int lineNum = (int)sqrt((float)_buttonNum);    //ÿ�еķ������
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

#pragma region ���鰴ť����(���)

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

#pragma region ���鰴ť����(�Ҽ�)

#pragma endregion

#pragma region �������
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

#pragma region �������ʱ����ʾ���з�����Ϣ
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

#pragma region ���÷�����Χ�׵�����
void CMinesweeperDlg::SetMineNum()
{
	int lineNum = (int)sqrt((float)_buttonNum);    //һ�еķ�������
	for (int i = 0; i < _buttonNum; i++)
	{
		if ( _paneList[i]->_isMine == true )
		{
			continue;
		}

		#pragma region �����ĸ���ť
		//��һ����ť
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
		//��һ�����һ����ť
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
		//��һ�е����һ����ť
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
		//���һ����ť
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

        #pragma region ������
		//�����һ��
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
		//���ұ�һ��
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
		//���ϱ�һ��
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
		//���±�һ��
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
 
        #pragma region �м����
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

#pragma region ���пո�ʱ��ʾ��Χ��������
void CMinesweeperDlg::DisplayAround(int paneList)
{
	if ( _paneList[paneList]->_state == "marker" )
	{
		return;
	}

	int lineNum = (int)sqrt((float)_buttonNum);    //һ�еķ�������

    #pragma region �����ĸ���ť
	//��һ����ť
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
	//��һ�����һ����ť
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
	//��һ�е����һ����ť
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
	//���һ����ť
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

    #pragma region ������
	//�����һ��
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
	//���ұ�һ��
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

	//���ϱ�һ��
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


	//���±�һ��
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

    #pragma region �м����
	
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

#pragma region ���������ͼƬ

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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//������newbutton��ťʱ����ť�ᷢ��WM_RBUTTONDOWN��Ϣ�������ڣ������ڽ��ܵ�����Ϣ���ȫ�ֱ���markedID���д���
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
	_markedID = -1;    //���������ť����Ĳ��֣��򽫸�ȫ�ֱ���ֵ��ʼ��
	//MessageBox(L"LFT Button Click!", L"Trigger");
	CDialogEx::OnRButtonDown(nFlags, point);
}
