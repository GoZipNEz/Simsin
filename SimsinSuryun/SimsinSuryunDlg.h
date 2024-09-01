
// SimsinSuryunDlg.h : ��� ����
//
#include "Include.h"
#include "ParmDef.h"
#pragma once

// ĳ���� ���°�
#define CHARACTER_STATUS_GROUND	1 // ���� 
#define CHARACTER_STATUS_AIR	2 // ����
#define CHARACTER_STATUS_LADDER	3 // ��ٸ�

// CSimsinSuryunDlg ��ȭ ����
class CSimsinSuryunDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSimsinSuryunDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SIMSINSURYUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.

private:
//	static CEvent g_CEventTerminated;			// When MainThread terminate, be set.

protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSogong();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	// �����ð� ����
	CTime	m_endTime;
	long	m_lTicketTime;

	// ��ư
	CButtonST m_btnSogong;

	// ������
	CWinThread * m_pThread;

	// ��ư ����ȭ 
	void	TransparentButton();

	// �� �ʱ�ȭ
	void	InitVars();

	// ����, ����ġ
	int		m_iLevel;
	double	m_dMaxExp, m_dNowExp, m_dAddExp;
	CProgressCtrl	m_prExp;

	void	IncreaseExp();

	// ĳ����
	int		m_iSizeX, m_iSizeY; // ĳ���� ũ�� 
	int		m_iPosX, m_iPosY;	// ĳ���� ��ġ
	int		m_iMoveX, m_iMoveY; // �ӵ�
	short	m_sStatus;			// ĳ���� ����
	BOOL	m_bDown;			// ���帰 ���� ����
	short	m_sDirect;			// ���� ���� (1 : ������, -1 : ����)

	void	CheckStatus();
	BOOL	IsLadder();
	int		GetGround();

	// ������
	bool	m_bDestroy;
	static CSimsinSuryunDlg* m_pDlg;

//	static UINT ThreadStartFuntion_WinThread( LPVOID param );		
	short	CreateExpThread();
	static	UINT	ExpProcThread(LPVOID param);
	long	StaticExpText();
	void	ClearStaticExpText();
	
	static HWND		m_hWndDialog;

	//Ű�Է�
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Sleep �̺�Ʈ
	void TimerSleep(unsigned int pinMiliSecond);

};
