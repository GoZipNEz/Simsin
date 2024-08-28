#pragma once

#include "Include.h"
#include "resource.h"		// �� ��ȣ�Դϴ�.

// Warning ��ȭ �����Դϴ�.

class Warning : public CDialogEx
{
	DECLARE_DYNAMIC(Warning)

public:
	Warning(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Warning();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	// ��ư
	CButtonST	m_btnYes, m_btnNo, m_btnClose;
	// �����ð�
	CTime	m_endTime;

	void	TransparentButton();

public:
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedBtnClose();

	inline	void GetEndTime(CTime pEndTime){m_endTime = pEndTime;}
};
