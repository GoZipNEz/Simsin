#pragma once

#include "Include.h"
#include "resource.h"		// 주 기호입니다.

// Warning 대화 상자입니다.

class Warning : public CDialogEx
{
	DECLARE_DYNAMIC(Warning)

public:
	Warning(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Warning();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	CButtonST	m_btnYes, m_btnNo, m_btnClose;

	void	TransparentButton();

public:
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedBtnClose();
};
