
// SimsinSuryunDlg.cpp : ���� ����
//
#include "stdafx.h"
#include "SimsinSuryun.h"
#include "SimsinSuryunDlg.h"
#include "afxdialogex.h"
#include "Warning.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimsinSuryunDlg ��ȭ ����
// CEvent CSimsinSuryunDlg::g_CEventTerminated			(TRUE,  TRUE );	// ���ν����� ���� �˸� ����ȭ �̺�Ʈ�Դϴ�.
CSimsinSuryunDlg* CSimsinSuryunDlg::m_pDlg = NULL;

CSimsinSuryunDlg::CSimsinSuryunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimsinSuryunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pThread = NULL;
	m_bDestroy = FALSE;

	//���ǰ�
	m_dMaxExp = 1589756321;
	m_dAddExp = 5635245;
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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSimsinSuryunDlg �޽��� ó����

BOOL CSimsinSuryunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	
	// ��� ����
	SetBackgroundImage(IDB_SIMSIN);

	// ���� ��ư
	TransparentButton(); 

	// ũ�� ����
	SetWindowPos(NULL, 0, 0, 990, 806, SWP_NOMOVE);

	// Ƽ��Ÿ�� (default : 120��)
	m_lTicketTime = 120;

	// ������ �ð�
	m_endTime = CTime::GetCurrentTime() + CTimeSpan(0, 0, m_lTicketTime, 0);
	
	// 120�� �ڿ� �Ѱܳ�
	SetTimer(1, m_lTicketTime * 60 * 1000, NULL);

	// ����ġ ���� ������
	SetTimer(2, 100, NULL);

	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSimsinSuryunDlg::OnPaint()
{
// 	if (IsIconic())
// 	{
// 		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
// 
// 		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
// 
// 		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
// 		int cxIcon = GetSystemMetrics(SM_CXICON);
// 		int cyIcon = GetSystemMetrics(SM_CYICON);
// 		CRect rect;
// 		GetClientRect(&rect);
// 		int x = (rect.Width() - cxIcon + 1) / 2;
// 		int y = (rect.Height() - cyIcon + 1) / 2;
// 
// 		// �������� �׸��ϴ�.
// 		dc.DrawIcon(x, y, m_hIcon);
// 	}
// 	else
// 	{
// 		CDialogEx::OnPaint();
// 	}


	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	//dc.SelectStockObject(NULL_BRUSH);
	// �ٴ� �׷�����
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(106,447, 910, 491);	// 3��
	dc.Rectangle(106,565, 910, 611);	// 2��
	dc.Rectangle(0,693, 990, 806);		// 1��

	// ��ٸ� �׷�����
	dc.SelectStockObject(GRAY_BRUSH);
	dc.Rectangle(337, 447, 385, 534);	// 3�� ��ٸ�
	dc.Rectangle(696, 565, 743, 649);	// 2�� ��ٸ�
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSimsinSuryunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CSimsinSuryunDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if (nCtlColor == CTLCOLOR_BTN)
// 	{
// 		switch (pWnd->GetDlgCtrlID())
// 		{
// 			case IDC_SOGONG:
// 				pDC->SetBkMode(TRANSPARENT);
// 				hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
// 			break;
// 		default:
// 			break;
// 		}
// 	}

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

	// Ƽ�� �ð� �� ��.
	if (nIDEvent == 1)
	{
		EndDialog(true);
	}
	// ����ġ������ ����
	else if (nIDEvent == 2)
	{
		CreateExpThread();
	}
	// ����ġ static edit�� ��� (�ڵ� ������ �ȵ�..��
	else if (nIDEvent == 3)
	{
		StaticExpText();
	}
}

// �� ����Ŭ���� �ȸԳ�..
void CSimsinSuryunDlg::OnBnClickedSogong()
{
	// �Ұ� �ؽ�Ʈ �ڽ� ���
	Warning * clsWarning;
	clsWarning = new Warning();
	clsWarning->GetEndTime(m_endTime);
	short sRtn = clsWarning->DoModal();
	delete clsWarning;

	// �� Ŭ���� ����
	if (sRtn)
		EndDialog(TRUE);
}

short CSimsinSuryunDlg::CreateExpThread()
{
	if ( m_pThread != NULL )
	{
		EndDialog(FALSE);
		return FALSE;
	}
	m_pDlg = new CSimsinSuryunDlg;

	// ���ν����带 �����մϴ�.
	m_pThread = AfxBeginThread( CSimsinSuryunDlg::ExpProcThread, m_pDlg, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED );

	// �������ڵ� ��ȿ���� Ȯ���մϴ�.
	if ( m_pThread != NULL )
	{
		// �ڵ� ���ҽ� ������ �����ϰ� �����带 �����մϴ�.
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}
	else
	{
		// ������ ������ �����Ͽ��� ���.
		EndDialog(FALSE);
		return FALSE;
	}

	// ������ ���� �� ���� ����� ��ȯ�մϴ�.
	return TRUE;

}

UINT CSimsinSuryunDlg::ExpProcThread(LPVOID param)
{
	 CSimsinSuryunDlg* pObject = (CSimsinSuryunDlg*)param;

	try
	{

		while (1)
		{	
			Sleep(5 * 1000);

			if (pObject->m_bDestroy == TRUE)
				break;
			
			// �ڵ� ������ �ذ��� �ȵ�...
			//pObject->SetTimer(3, 10, NULL);
		}
	}
	catch (...)
	{
	
	}

	return 1;
}

long CSimsinSuryunDlg::StaticExpText()
{
	m_dMaxExp += m_dAddExp;

	CString csMsg;

	csMsg.Format(_T("����ġ�� ȹ���߽��ϴ� (+%.0f)"), m_dAddExp);
	SetDlgItemText(IDC_STATIC_OUTPUT, csMsg);
	// 2�ʵ� ����?
	Sleep(2 * 1000);
	SetDlgItemText(IDC_STATIC_OUTPUT, NULL);

	return TRUE;
}

void CSimsinSuryunDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	if ( m_pThread != NULL )
	{
		m_pDlg->m_bDestroy = TRUE;
		// ������ ���Ḧ ��ٸ��ϴ�.
		::WaitForSingleObject( m_pThread->m_hThread, INFINITE );

		delete m_pThread;
		m_pThread = NULL;
	}
	
	delete m_pDlg;
	m_pDlg = NULL;
}
