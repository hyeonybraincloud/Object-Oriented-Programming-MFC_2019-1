
// Assignment 4Dlg.h: 헤더 파일
//

#pragma once

class Snake						// class for snake's head, trunk, and tail
{
public:
	int snake_x = NULL;			// coordinate x of the snake element
	int snake_y = NULL;			// coordinate y of the snake element
	int timer_code = NULL;		// to use SetTimer function properly
	Snake* next_element = NULL;	// next element's address
	Snake* prev_element = NULL;	// previous element's address
};

class Prey					// class for prey
{
public:
	int x = NULL;			// coordinate x of the prey
	int y = NULL;			// coordinate y of the prey
};

// CAssignment4Dlg 대화 상자
class CAssignment4Dlg : public CDialogEx
{
// 생성입니다.
public:
	CAssignment4Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	bool init_state = TRUE;						// to know whether the snake have encountered her prey. If not, init_state is TRUE.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSIGNMENT4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DrawBackground();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Growup(Snake* tail);
};