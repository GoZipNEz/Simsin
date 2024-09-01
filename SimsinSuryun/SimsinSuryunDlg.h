
// SimsinSuryunDlg.h : 헤더 파일
//
#include "Include.h"
#include "ParmDef.h"
#pragma once

// 캐릭터 상태값
#define CHARACTER_STATUS_GROUND	1 // 땅위 
#define CHARACTER_STATUS_AIR	2 // 공중
#define CHARACTER_STATUS_LADDER	3 // 사다리

// CSimsinSuryunDlg 대화 상자
class CSimsinSuryunDlg : public CDialogEx
{
// 생성입니다.
public:
	CSimsinSuryunDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SIMSINSURYUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.

private:
//	static CEvent g_CEventTerminated;			// When MainThread terminate, be set.

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSogong();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	// 남은시간 관련
	CTime	m_endTime;
	long	m_lTicketTime;

	// 버튼
	CButtonST m_btnSogong;

	// 쓰레드
	CWinThread * m_pThread;

	// 버튼 투명화 
	void	TransparentButton();

	// 값 초기화
	void	InitVars();

	// 레벨, 경험치
	int		m_iLevel;
	double	m_dMaxExp, m_dNowExp, m_dAddExp;
	CProgressCtrl	m_prExp;

	void	IncreaseExp();

	// 캐릭터
	int		m_iSizeX, m_iSizeY; // 캐릭터 크기 
	int		m_iPosX, m_iPosY;	// 캐릭터 위치
	int		m_iMoveX, m_iMoveY; // 속도
	short	m_sStatus;			// 캐릭터 상태
	BOOL	m_bDown;			// 엎드린 상태 여부
	short	m_sDirect;			// 보는 방향 (1 : 오른쪽, -1 : 왼쪽)

	void	CheckStatus();
	BOOL	IsLadder();
	int		GetGround();

	// 쓰레드
	bool	m_bDestroy;
	static CSimsinSuryunDlg* m_pDlg;

//	static UINT ThreadStartFuntion_WinThread( LPVOID param );		
	short	CreateExpThread();
	static	UINT	ExpProcThread(LPVOID param);
	long	StaticExpText();
	void	ClearStaticExpText();
	
	static HWND		m_hWndDialog;

	//키입력
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Sleep 이벤트
	void TimerSleep(unsigned int pinMiliSecond);

};
