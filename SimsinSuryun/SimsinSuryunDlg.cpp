
// SimsinSuryunDlg.cpp : 구현 파일
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


// CSimsinSuryunDlg 대화 상자
// CEvent CSimsinSuryunDlg::g_CEventTerminated			(TRUE,  TRUE );	// 메인스레드 종료 알림 동기화 이벤트입니다.
CSimsinSuryunDlg* CSimsinSuryunDlg::m_pDlg = NULL;

CSimsinSuryunDlg::CSimsinSuryunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimsinSuryunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pThread = NULL;
	m_bDestroy = FALSE;

	//임의값
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


// CSimsinSuryunDlg 메시지 처리기

BOOL CSimsinSuryunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	// 배경 설정
	SetBackgroundImage(IDB_SIMSIN);

	// 투명 버튼
	TransparentButton(); 

	// 크기 조절
	SetWindowPos(NULL, 0, 0, 990, 806, SWP_NOMOVE);

	// 티켓타임 (default : 120분)
	m_lTicketTime = 120;

	// 끝나는 시간
	m_endTime = CTime::GetCurrentTime() + CTimeSpan(0, 0, m_lTicketTime, 0);
	
	// 120분 뒤에 쫓겨남
	SetTimer(1, m_lTicketTime * 60 * 1000, NULL);

	// 경험치 증가 쓰레드
	SetTimer(2, 100, NULL);

	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSimsinSuryunDlg::OnPaint()
{
// 	if (IsIconic())
// 	{
// 		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
// 
// 		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
// 
// 		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
// 		int cxIcon = GetSystemMetrics(SM_CXICON);
// 		int cyIcon = GetSystemMetrics(SM_CYICON);
// 		CRect rect;
// 		GetClientRect(&rect);
// 		int x = (rect.Width() - cxIcon + 1) / 2;
// 		int y = (rect.Height() - cyIcon + 1) / 2;
// 
// 		// 아이콘을 그립니다.
// 		dc.DrawIcon(x, y, m_hIcon);
// 	}
// 	else
// 	{
// 		CDialogEx::OnPaint();
// 	}


	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	//dc.SelectStockObject(NULL_BRUSH);
	// 바닥 그려보기
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(106,447, 910, 491);	// 3층
	dc.Rectangle(106,565, 910, 611);	// 2층
	dc.Rectangle(0,693, 990, 806);		// 1층

	// 사다리 그려보기
	dc.SelectStockObject(GRAY_BRUSH);
	dc.Rectangle(337, 447, 385, 534);	// 3층 사다리
	dc.Rectangle(696, 565, 743, 649);	// 2층 사다리
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

	// 티켓 시간 다 됨.
	if (nIDEvent == 1)
	{
		EndDialog(true);
	}
	// 경험치쓰레드 시작
	else if (nIDEvent == 2)
	{
		CreateExpThread();
	}
	// 경험치 static edit에 출력 (핸들 문제로 안됨..ㅠ
	else if (nIDEvent == 3)
	{
		StaticExpText();
	}
}

// 왜 더블클릭이 안먹냐..
void CSimsinSuryunDlg::OnBnClickedSogong()
{
	// 소공 텍스트 박스 출력
	Warning * clsWarning;
	clsWarning = new Warning();
	clsWarning->GetEndTime(m_endTime);
	short sRtn = clsWarning->DoModal();
	delete clsWarning;

	// 네 클릭시 종료
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

	// 메인스레드를 생성합니다.
	m_pThread = AfxBeginThread( CSimsinSuryunDlg::ExpProcThread, m_pDlg, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED );

	// 스레드핸들 유효성을 확인합니다.
	if ( m_pThread != NULL )
	{
		// 자동 리소스 정리를 해제하고 스레드를 실행합니다.
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}
	else
	{
		// 스레드 생성에 실패하였을 경우.
		EndDialog(FALSE);
		return FALSE;
	}

	// 스레드 생성 및 실행 결과를 반환합니다.
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
			
			// 핸들 문제가 해결이 안됨...
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

	csMsg.Format(_T("경험치를 획득했습니다 (+%.0f)"), m_dAddExp);
	SetDlgItemText(IDC_STATIC_OUTPUT, csMsg);
	// 2초뒤 삭제?
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
		// 스레드 종료를 기다립니다.
		::WaitForSingleObject( m_pThread->m_hThread, INFINITE );

		delete m_pThread;
		m_pThread = NULL;
	}
	
	delete m_pDlg;
	m_pDlg = NULL;
}
