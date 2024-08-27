
// SimsinSuryunDlg.h : 헤더 파일
//
#include "Include.h"
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
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

	long m_lstartTime, m_lEndTime, m_lTicketTime;

	CButtonST m_btnSogong;


	void	TransparentButton();

public:
	afx_msg void OnBnClickedSogong();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
