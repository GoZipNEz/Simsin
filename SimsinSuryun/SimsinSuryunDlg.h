
// SimsinSuryunDlg.h : ��� ����
//
#include "Include.h"
#pragma once


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
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
