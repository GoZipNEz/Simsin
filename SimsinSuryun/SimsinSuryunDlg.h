
// SimsinSuryunDlg.h : 헤더 파일
//
#include "Include.h"
#include "ParmDef.h"
#pragma once


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

	// 레벨, 경험치
	int		m_iLevel;
	double	m_dMaxExp, m_dAddExp;

	bool	m_bDestroy;

	// 쓰레드
	static CSimsinSuryunDlg* m_pDlg;

//	static UINT ThreadStartFuntion_WinThread( LPVOID param );		
	short	CreateExpThread();
	static	UINT	ExpProcThread(LPVOID param);
	long	StaticExpText();
	

public:
	static HWND		m_hWndDialog;
};
