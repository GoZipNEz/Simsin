
// SimsinSuryunDlg.cpp : ���� ����
//
#include "stdafx.h"
#include "SimsinSuryun.h"
#include "SimsinSuryunDlg.h"
#include "afxdialogex.h"
#include "Warning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimsinSuryunDlg ��ȭ ����



CSimsinSuryunDlg::CSimsinSuryunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimsinSuryunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimsinSuryunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimsinSuryunDlg, CDialogEx)
	ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	//ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SOGONG, &CSimsinSuryunDlg::OnBnClickedSogong)
END_MESSAGE_MAP()


// CSimsinSuryunDlg �޽��� ó����

BOOL CSimsinSuryunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	
	// ��� ����
	SetBackgroundImage(IDB_SIMSIN);

	// ���� ��ư
	TransparentButton();

	// ũ�� ����
	SetWindowPos(NULL, 0, 0, 990, 806, SWP_NOMOVE);

	// Ƽ��Ÿ�� (default : 30��)
	m_lTicketTime = 30 * 60 * 1000;
	// ���� �ð�
	m_lstartTime = GetTickCount();
	// �� �ð� 
	m_lEndTime = m_lstartTime + m_lTicketTime;
	
	// 30�� �ڿ� �Ѱܳ�
	SetTimer(1, m_lTicketTime, NULL);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSimsinSuryunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSimsinSuryunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��ư ����ó��
HBRUSH CSimsinSuryunDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_BTN)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_SOGONG:
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
			break;
		default:
			break;
		}
	}

	return hbr;
}

void CSimsinSuryunDlg::TransparentButton()
{
	m_btnSogong.SubclassDlgItem(IDC_SOGONG, this);

	m_btnSogong.DrawTransparent(this->IDD);
}

void CSimsinSuryunDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	KillTimer(nIDEvent);

	if (nIDEvent == 1)
	{
		EndDialog(true);
	}
}

// �� ����Ŭ���� �ȸԳ�..
void CSimsinSuryunDlg::OnBnClickedSogong()
{
	// �Ұ� �ؽ�Ʈ �ڽ� ���
	Warning * clsWarning;
	clsWarning = new Warning();
	short sRtn = clsWarning->DoModal();
	delete clsWarning;

	// �� Ŭ���� ����
	if (sRtn)
		EndDialog(true);
}
