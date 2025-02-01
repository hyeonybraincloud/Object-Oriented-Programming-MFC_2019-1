	
// Assignment 4Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Assignment 4.h"
#include "Assignment 4Dlg.h"
#include "afxdialogex.h"
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// classes defined and declared on Assignment 4Dlg.h
Snake head;												// form an object for snake's head
Snake initial_trunk;									// form an object for snake's initial trunk
Snake tail;												// form an object for snake's tail
Prey prey;												// form an object for prey

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAssignment4Dlg 대화 상자



CAssignment4Dlg::CAssignment4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSIGNMENT4_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAssignment4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAssignment4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CAssignment4Dlg 메시지 처리기

BOOL CAssignment4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(0, 0, 0));				// Paint the background in black
	srand(1);
	if (MessageBox(L"이 게임은 뱀에 대한 잔혹한 묘사가 포함되어 있습니다.\n무서운 것에 서투른 분은 플레이를 삼가해 주십시오.\n게임 진행을 원하십니까?", L"경고문", MB_YESNO) == IDNO)
		this->DestroyWindow();						// An pop-up message before the game starts

	else
		MoveWindow(0, 0, 850, 850);					// Pop up the dialog box in size of 850*850

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	head.snake_x = 30 + 30 * 12, head.snake_y = 30 + 30 * 12;						// initial position of snake's head
	head.next_element = &initial_trunk;												// link with next element (initial trunk) using linked-list

	initial_trunk.snake_x = 30 + 30 * 13, initial_trunk.snake_y = 30 + 30 * 12;		// initial position of snake's intial trunk
	initial_trunk.next_element = &tail, initial_trunk.prev_element = &head;			// link with next and previous element using linked list
	
	tail.snake_x = 30 + 30 * 14, tail.snake_y = 30 + 30 * 12;						// initial position of snake's tail
	tail.prev_element = &initial_trunk;												// link with previous element using linked-list

	prey.x = 30 + 30 * (rand() % 25), prey.y = 30 + 30 * (rand() % 25);				// initial random position of prey

	SetTimer(0, 55, nullptr);														// to make snake move leftside at first
	head.timer_code = 0;															// 0 is timercode for moving leftside

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAssignment4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.



void CAssignment4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CDialogEx::OnPaint();
		DrawBackground();						// a function declared and defined on 219th row
		int width = 30;							// also height equals to width. In this program, width is used entirely.

		CBrush brush_head(RGB(0, 255, 0));		// head positioning
		dc.SelectObject(brush_head);
		dc.Ellipse(head.snake_x, head.snake_y, head.snake_x + width, head.snake_y + width);

		Snake* trunk = &initial_trunk;			// initial trunk positioning
		if (init_state == TRUE)					// when the snake has not encountered any preys
		{
			CBrush brush_trunk(RGB(255, 255, 0));	// only initial trunk positioning
			dc.SelectObject(brush_trunk);
			dc.Rectangle(trunk->snake_x, trunk->snake_y, trunk->snake_x + width, trunk->snake_y + width);
		}
		else										// when the snake has encountered any preys
		{
			while (trunk->next_element != NULL)		// positioning for not only initial trunk as but also any other trunks
			{
				CBrush brush_trunk(RGB(255, 255, 0));
				dc.SelectObject(brush_trunk);
				dc.Rectangle(trunk->snake_x, trunk->snake_y, trunk->snake_x + width, trunk->snake_y + width);
				trunk = trunk->next_element;
			}
		}
		CBrush brush_tail(RGB(0, 216, 255));		// tail positioning
		dc.SelectObject(brush_tail);
		dc.Ellipse(tail.snake_x, tail.snake_y, tail.snake_x + width, tail.snake_y + width);

		CBrush brush_prey(RGB(255, 0, 0));			// prey positioning
		dc.SelectObject(brush_prey);
		dc.Rectangle(prey.x, prey.y, prey.x + width, prey.y + width);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAssignment4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAssignment4Dlg::DrawBackground()				// To draw grid for the snake to move on
{
	CClientDC dc(this);

	CPen Mypen(PS_SOLID, 0, RGB(140, 140, 140));	// To paint boader line in gray
	dc.SelectObject(&Mypen);

	CBrush MyBrush(RGB(140, 140, 140));				// To paint a grid where the snake moves actually in gray
	dc.SelectObject(&MyBrush);
	dc.Rectangle(30, 30, 30 + 30 * 25, 30 + 30 * 25);

	CPen Mypen2(PS_SOLID, 1, RGB(255, 255, 255));	// To paint boundary in white
	dc.SelectObject(&Mypen2);

	for (int x = 0; x <= 25; x++)					// To draw each vertical line
	{
		dc.MoveTo(30 + 30 * x, 30);
		dc.LineTo(30 + 30 * x, 30 + 30 * 25);
	}

	for (int y = 0; y <= 25; y++)					// To draw each horizontal line
	{
		dc.MoveTo(30, 30 + 30 * y);
		dc.LineTo(30 + 30 * 25, 30 + 30 * y);
	}
}


void CAssignment4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	int width = 30;												// interval
	int boundary_most_left = 30;								// most left boundary
	int boundary_most_right = 30 + width * 25;					// most right boundary
	int boundary_most_top = 30;									// most upper boundary
	int boundary_most_bottom = 30 + width * 25;					// most lower boundary
	switch (nIDEvent)
	{
	case 0:						// move to leftside
		if (head.snake_x < boundary_most_left)					// when the head is over the left boundary (meet wall)
		{
			if (MessageBox(L"벽에 쿵!", L"실패", MB_ICONERROR) == IDOK)
				exit(0);
		}
		else if (init_state == TRUE)								// The snake has not ever encounter prey. 
		{
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				tail.snake_x = initial_trunk.snake_x;				// keep moving leftside
				initial_trunk.snake_x = head.snake_x;
				tail.snake_y = initial_trunk.snake_y;
				initial_trunk.snake_y = head.snake_y;
				head.snake_x -= width;
				Invalidate(TRUE);
				break;
			}
			else             // encounter prey
			{
				init_state = FALSE;						// the snake has a experience that she has met prey
				Growup(&tail);							// grow up
				initial_trunk.snake_x = head.snake_x;	// keep moving leftside
				initial_trunk.snake_y = head.snake_y;
				head.snake_x -= width;
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form prey on other place
				Invalidate(TRUE);
				break;
			}
		}
		else if (init_state == FALSE)					// the snake has encountered the prey
		{
			Snake* prev_is_null = &tail;				// to use structure linked-list
			while (prev_is_null->prev_element != &head)	// when the snake's head meets her any trunks
			{
				if (head.snake_x == prev_is_null->snake_x && head.snake_y == prev_is_null->snake_y)
				{
					if (MessageBox(L"몸에 쿵!", L"실패", MB_ICONERROR) == IDOK)
						exit(0);
				}
				else
					prev_is_null = prev_is_null->prev_element;
			}
			prev_is_null = &tail;
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				while (prev_is_null->prev_element != NULL)			// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_x -= width;
				Invalidate(TRUE);
				break;
			}
			else			// encounter prey
			{
				Growup(&tail);			// grow up
				while (prev_is_null->prev_element != NULL)		// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_x -= width;

				prev_is_null = &tail;
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// construct new prey
				while (prev_is_null != NULL)	// to find out that the position of new prey equals to the snake's position
				{
					if (prey.x == prev_is_null->snake_x && prey.y == prev_is_null->snake_y)
					{
						prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);
						prev_is_null = prev_is_null->prev_element;
					}
					else
						prev_is_null = prev_is_null->prev_element;
				}
				Invalidate(TRUE);
				break;
			}
		}

	case 1:							// move to downside
		if (head.snake_y == boundary_most_bottom)						// collision with wall on the bottom boundary
		{
			if (MessageBox(L"벽에 쿵!", L"실패", MB_ICONERROR) == IDOK)
				exit(0);
		}
		else if (init_state == TRUE)			// when the snake has not encounterd any prey
		{
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				tail.snake_x = initial_trunk.snake_x;	// keep moving
				initial_trunk.snake_x = head.snake_x;
				tail.snake_y = initial_trunk.snake_y;
				initial_trunk.snake_y = head.snake_y;
				head.snake_y += width;
				Invalidate(TRUE);
				break;
			}
			else						// encounter prey
			{
				init_state = FALSE;		// the snake has a experience that she has met prey
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form new prey
				Growup(&tail);			// grow up
				initial_trunk.snake_x = head.snake_x;	// keep moving
				initial_trunk.snake_y = head.snake_y;
				head.snake_y += width;
				Invalidate(TRUE);
				break;

			}
		}
		else if (init_state == FALSE)	// snake has encountered a prey
		{
			Snake* prev_is_null = &tail;
			while (prev_is_null->prev_element != &head)		// collision with the snake's body
			{
				if (head.snake_x == prev_is_null->snake_x && head.snake_y == prev_is_null->snake_y)
				{
					if (MessageBox(L"몸에 쿵!", L"실패", MB_ICONERROR) == IDOK)
						exit(0);
				}
				else
					prev_is_null = prev_is_null->prev_element;
			}
			prev_is_null = &tail;
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				while (prev_is_null->prev_element != NULL)		// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_y += width;
				Invalidate(TRUE);
				break;
			}
			else			// encounter prey
			{
				Growup(&tail);													// growup
				while (prev_is_null->prev_element != NULL)						// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_y += width;

				prev_is_null = &tail;
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form new prey
				while (prev_is_null != NULL)	// find out whether the new prey's position equals to the snake's position
				{
					if (prey.x == prev_is_null->snake_x && prey.y == prev_is_null->snake_y)
					{
						prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);
						prev_is_null = prev_is_null->prev_element;
					}
					else
						prev_is_null = prev_is_null->prev_element;
				}

				Invalidate(TRUE);
				break;
			}
		}
	case 2:						// move to rightside
		if (head.snake_x == boundary_most_right)	// collision with the right boundary
		{
			if (MessageBox(L"벽에 쿵!", L"실패", MB_ICONERROR) == IDOK)
				exit(0);
		}
		else if (init_state == TRUE)				// the snake has not encountered prey
		{
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				tail.snake_x = initial_trunk.snake_x;	// keep moving
				initial_trunk.snake_x = head.snake_x;
				tail.snake_y = initial_trunk.snake_y;
				initial_trunk.snake_y = head.snake_y;
				head.snake_x += width;
				Invalidate(TRUE);
				break;
			}
			else											// encounter prey
			{
				init_state = FALSE;							// the snake has a experience that she has met prey 
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25); // form new prey
				Growup(&tail);								// grow up
				initial_trunk.snake_x = head.snake_x;		// keep moving
				initial_trunk.snake_y = head.snake_y;
				head.snake_x += width;
				Invalidate(TRUE);
				break;
			}
		}
		else if (init_state == FALSE)			// the snake has encountered prey
		{
			Snake* prev_is_null = &tail;
			while (prev_is_null->prev_element != &head)		// collisition with snake's body
			{
				if (head.snake_x == prev_is_null->snake_x && head.snake_y == prev_is_null->snake_y)
				{
					if (MessageBox(L"몸에 쿵!", L"실패", MB_ICONERROR) == IDOK)
						exit(0);
				}
				else
					prev_is_null = prev_is_null->prev_element;
			}
			prev_is_null = &tail;
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				while (prev_is_null->prev_element != NULL)		// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_x += width;
				Invalidate(TRUE);
				break;
			}
			else												// encounter prey
			{
				Growup(&tail);									// grow up
				while (prev_is_null->prev_element != NULL)		// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_x += width;

				prev_is_null = &tail;
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form new prey
				while (prev_is_null != NULL)				// find out whether the prey's position equals to the snake's position
				{
					if (prey.x == prev_is_null->snake_x && prey.y == prev_is_null->snake_y)
					{
						prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);
						prev_is_null = prev_is_null->prev_element;
					}
					else
						prev_is_null = prev_is_null->prev_element;
				}

				Invalidate(TRUE);
				break;
			}
		}

	case 3:						// move to upside
		if (head.snake_y < boundary_most_top)	// collision with top boundary
		{
			if (MessageBox(L"벽에 쿵!", L"실패", MB_ICONERROR) == IDOK)
				exit(0);
		}
		else if (init_state == TRUE)		// the snake has not encountered the prey
		{
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				tail.snake_x = initial_trunk.snake_x;		// keep moving
				initial_trunk.snake_x = head.snake_x;
				tail.snake_y = initial_trunk.snake_y;
				initial_trunk.snake_y = head.snake_y;
				head.snake_y -= width;
				Invalidate(TRUE);
				break;
			}
			else						// encounter prey
			{
				init_state = FALSE;		// the snake has a experience that she has met prey
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form new prey
				Growup(&tail);			// grow up
				initial_trunk.snake_x = head.snake_x;	// keep moving
				initial_trunk.snake_y = head.snake_y;
				head.snake_y -= width;
				Invalidate(TRUE);
				break;
			}
		}
		else if (init_state == FALSE)			// the snake has encountered the prey
		{
			Snake* prev_is_null = &tail;
			while (prev_is_null->prev_element != &head)		// collision with the snake's body
			{
				if (head.snake_x == prev_is_null->snake_x && head.snake_y == prev_is_null->snake_y)
				{
					if (MessageBox(L"몸에 쿵!", L"실패", MB_ICONERROR) == IDOK)
						exit(0);
				}
				else
					prev_is_null = prev_is_null->prev_element;
			}
			prev_is_null = &tail;
			if (head.snake_x != prey.x || head.snake_y != prey.y)	// not encounter prey
			{
				while (prev_is_null->prev_element != NULL)			// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_y -= width;
				Invalidate(TRUE);
				break;
			}
			else			// encounter prey
			{
				Growup(&tail);		// grow up
				while (prev_is_null->prev_element != NULL)		// keep moving
				{
					prev_is_null->snake_x = prev_is_null->prev_element->snake_x;
					prev_is_null->snake_y = prev_is_null->prev_element->snake_y;
					prev_is_null = prev_is_null->prev_element;
				}
				head.snake_y -= width;

				prev_is_null = &tail;
				prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);	// form new prey
				while (prev_is_null != NULL)	// find out whether the new prey's position equals to the snake's position
				{
					if (prey.x == prev_is_null->snake_x && prey.y == prev_is_null->snake_y)
					{
						prey.x = 30 + width * (rand() % 25), prey.y = 30 + width * (rand() % 25);
						prev_is_null = prev_is_null->prev_element;
					}
					else
						prev_is_null = prev_is_null->prev_element;
				}

				Invalidate(TRUE);
				break;
			}
		}
		CDialogEx::OnTimer(nIDEvent);
		}
	}


void CAssignment4Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
 {
	switch (nChar)
	{
	case 'w':									// move to upside
	case 'W':
		if (head.timer_code == 3 || head.timer_code == 1)	// when previous movement directs to upside or downside
			break;
		else                                    // when previous movement directs to rightside or leftside
		{
			KillTimer(head.timer_code);				// delete previous processing timer
			SetTimer(3, 55, nullptr);
			head.timer_code = 3;
			break;
		}

	case 's':									// move to downside
	case 'S':
		if (head.timer_code == 1 || head.timer_code == 3)	// when previous movement directs to downside or upside
			break;
		else                                    // when previous movement directs to rightside or leftside
		{
			KillTimer(head.timer_code);				// delete previous processing timer
			SetTimer(1, 55, nullptr);
			head.timer_code = 1;
			break;
		}
	case 'd':									// move to rightside
	case 'D':
		if (head.timer_code == 2 || head.timer_code == 0)	// when previous movement directs to rightside or leftside
			break;
		else                                    // when previous movement directs to upside or downside
		{
			KillTimer(head.timer_code);				// delete previous processing timer
			SetTimer(2, 55, nullptr);
			head.timer_code = 2;
			break;
		}

	case 'a':									// move to leftside
	case 'A':
		if (head.timer_code == 0 || head.timer_code == 2)	// when previous movement directs to rightside or leftside
			break;
		else                                    // when previous movement directs to upside or downside
		{
			KillTimer(head.timer_code);				// delete previous processing timer
			SetTimer(0, 55, nullptr);
			head.timer_code = 0;
			break;
		}
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAssignment4Dlg::Growup(Snake* tail)	// a function to lengthen the snake's length when the snake encounters a prey
{
	Snake* additional_trunk = new Snake;	// form new trunk via dynamic allocation

	additional_trunk->snake_x = tail->prev_element->snake_x;	// by linked-list
	additional_trunk->snake_y = tail->prev_element->snake_y;
	additional_trunk->next_element = tail;
	additional_trunk->prev_element = tail->prev_element;
	tail->prev_element->next_element = additional_trunk;
	tail->prev_element = additional_trunk;
}