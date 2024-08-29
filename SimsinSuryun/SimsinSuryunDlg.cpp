
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

	// 값 초기화
	InitVars();
	
	// 120분 뒤에 쫓겨남
	SetTimer(1, m_lTicketTime * 60 * 1000, NULL);

	// 경험치 증가 쓰레드
	SetTimer(2, 100, NULL);

	// 캐릭터 상태 체크
	 SetTimer(4, 10, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
	// 캐릭터 상태 체크
	else if (nIDEvent == 4)
	{
		CheckStatus();
		SetTimer(4, 10, NULL);
	}
}

void CSimsinSuryunDlg::InitVars()
{
	// 티켓타임 (default : 120분)
	m_lTicketTime = 120;

	// 끝나는 시간
	m_endTime = CTime::GetCurrentTime() + CTimeSpan(0, 0, m_lTicketTime, 0);

	// 캐릭터 크기 (절반)
	m_iSizeX = 11;
	m_iSizeY = 20;

	// 캐릭터 초기 위치
	m_iPosY = 300;
	m_iPosX = 300;

	// 처음엔 공중에서 착지
	m_iMoveX = 0;
	m_iMoveY = 0;
	m_sStatus = CHARACTER_STATUS_AIR;
	m_bDown = FALSE;
	// 처음엔 오른쪽 봄
	m_sDirect = 1;
}

void CSimsinSuryunDlg::CheckStatus()
{
	switch (m_sStatus)
	{
		// 사다리는 다올라가거나.. 점프눌렀을때만 사다리에서 내려와짐.
		case CHARACTER_STATUS_LADDER:
			// 사다리 다올라갔으면
			if (!IsLadder())
			{
				m_iMoveY = 0;
				m_sStatus = CHARACTER_STATUS_AIR;
			}
			break;
		case CHARACTER_STATUS_GROUND:	
			break;
		case CHARACTER_STATUS_AIR:
			// 바닥 착지시 (조건문 변경해야함
			if (m_iMoveY >= 0 && m_iPosY == GetGround() - m_iSizeY)
			{
				m_sStatus = CHARACTER_STATUS_GROUND;
				// 				m_iMoveY = 0;
				// 				m_iMoveX = 0;
			}

			// 낙하 가속도
			m_iMoveY += 1;

			// 최대 낙하속도
			m_iMoveY = min(100, m_iMoveY);


		default:
			break;
	}
	
	if (m_sStatus == CHARACTER_STATUS_LADDER)
		m_iMoveX = 0;
	// x축 이동
	m_iPosX += m_iMoveX;
	// m_iSizeX <= m_iPosX <= 990 - m_iSizeX
	m_iPosX = min(max(m_iSizeX, m_iPosX), 990 - m_iSizeX);
	

	// y축 이동
	m_iPosY += m_iMoveY;

	int iGroundY = GetGround();

	// m_iSizeY <= m_iPosY <= iGroundY - m_iSizeY
	if (m_sStatus != CHARACTER_STATUS_LADDER)
		m_iPosY = min(max(m_iSizeY, m_iPosY), iGroundY - m_iSizeY);
	// 사다리 착지 부분을 어떻게 하지
//	else

	Invalidate();
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

BOOL CSimsinSuryunDlg::IsLadder()
{
	//(337, 447, 385, 534) 3층 사다리
	//(696, 565, 743, 649) 2층 사다리
	
	// 지금은 바닥과 사다리가 적으니 하드코딩

	if (m_iPosX >= 337 && m_iPosX <= 385)
		if (m_iPosY > 447 - m_iSizeY && m_iPosY < 534 + m_iSizeY)
		{
			m_iPosX = (int)(385 + 337) / 2;
			return TRUE;
		}



	if (m_iPosX >= 696 && m_iPosX <= 743)
		if (m_iPosY > 565 - m_iSizeY && m_iPosY < 649 + m_iSizeY)
		{
			m_iPosX = (int)(696 + 743) / 2;
			return TRUE;
		}

	return FALSE;
}


int CSimsinSuryunDlg::GetGround()
{
	//(106, 447, 910, 491)	3층
	//(106, 565, 910, 611)	2층
	//(0, 693, 990, 806)	1층

	// 지금은 바닥과 사다리가 적으니 하드코딩

	int iGroundY = 693;

	if (m_iPosX >= 106 && m_iPosX <= 910)
	{
		if (m_iPosY <= 447 + m_iSizeY)
			iGroundY = 447;
		else if (m_iPosY <= 565 + m_iSizeY)
			iGroundY = 565;
	}
	else if (m_iPosY < 693 - m_iSizeY)
		m_sStatus = CHARACTER_STATUS_AIR;

	return iGroundY;
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


void CSimsinSuryunDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	//dc.SelectStockObject(NULL_BRUSH);
	// 바닥 그려보기
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(106,447, 910, 491);	// 3층
	dc.Rectangle(106,565, 910, 611);	// 2층
	dc.Rectangle(0,693, 990, 806);		// 1층

	// 사다리 그려보기
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(337, 447, 385, 534);	// 3층 사다리
	dc.Rectangle(696, 565, 743, 649);	// 2층 사다리

	// 캐릭터 그리기
	dc.SelectStockObject(BLACK_BRUSH);
	// 서있는 상태
	if (!m_bDown)
		dc.Rectangle(m_iPosX - m_iSizeX, m_iPosY - m_iSizeY, 
					m_iPosX + m_iSizeX, m_iPosY + m_iSizeY);
	// 숙인상태
	else
		// 오른쪽
		if (m_sDirect == 1)
			dc.Rectangle(m_iPosX - m_iSizeX, m_iPosY + m_iSizeY - 2 * m_iSizeX, 
						m_iPosX - m_iSizeX + m_iSizeY * 2 , m_iPosY + m_iSizeY);
		// 왼쪽
		else
			dc.Rectangle(m_iPosX + m_iSizeX - 2 * m_iSizeY, m_iPosY + m_iSizeY - 2 * m_iSizeX, 
						m_iPosX + m_iSizeX, m_iPosY + m_iSizeY);
}

// 키입력
BOOL CSimsinSuryunDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		switch (pMsg->wParam)
		{
			// 왼쪽
			case VK_LEFT:
				m_iMoveX = -5;
				m_sDirect = -1;
				break;
			// 오른쪽
			case VK_RIGHT:
				m_iMoveX = 5;
				m_sDirect = 1;
				break;
			// 위키
	 		case VK_UP:
				switch (m_sStatus)
				{
					case CHARACTER_STATUS_AIR:
					case CHARACTER_STATUS_GROUND:
						// 윗키 누른곳이 사다리면 사다리 타기
						m_iMoveY = 0;
						if (IsLadder())
						{
							m_sStatus = CHARACTER_STATUS_LADDER;
						}
						break;
					case CHARACTER_STATUS_LADDER:
						// 사다리타고 올라가기
						m_iMoveY = -5;
						break;
					default:
						break;
				}
	 			break;
	 		case VK_DOWN:
				switch (m_sStatus)
				{
					case CHARACTER_STATUS_GROUND:
						// 아래키 누른곳이 사다리면 사다리 타기
						m_iPosY += 1;
						if (IsLadder())
						{
							m_sStatus = CHARACTER_STATUS_LADDER;
							m_iMoveY = 5;
						}
						else
						{
							m_iPosY -= 1;
							m_bDown = TRUE;
						}
						break;
					case CHARACTER_STATUS_LADDER:
						//사다리타고 내려가기
						m_iMoveY = 5;
						break;
					case CHARACTER_STATUS_AIR:
					default:
						break;
				}
	 			break;
			case VK_MENU: // ALT
				// 점프
				if (m_sStatus == CHARACTER_STATUS_GROUND)
				{
					m_iMoveY = -12;
					m_iPosY -= 1;
					m_sStatus = CHARACTER_STATUS_AIR;
				}
				// 사다리 점프
				else if (m_sStatus == CHARACTER_STATUS_LADDER)
				{
					if (GetAsyncKeyState(VK_RIGHT))
					{
						m_iMoveX = 5;
						m_iMoveY = -6;
						m_sStatus = CHARACTER_STATUS_AIR;
					}
					else if (GetAsyncKeyState(VK_LEFT))
					{
						m_iMoveX = -5;
						m_iMoveY = -6;
						m_sStatus = CHARACTER_STATUS_AIR;
					}
				}
				
				return TRUE;
				break;

			// 설정 창 여는거 추가 예정
			case VK_ESCAPE:
				AfxMessageBox(_T("설정창"));
				// OnSetting();
				break;

			// 아이템 창 여는거 추가 예정
			case 0x49:
				AfxMessageBox(_T("아이템창"));
				// OnInventory();
				break;

			default:
				break;
		}
		// 40 <= m_iPosY <= 693 - 40
		m_iPosY = min(max(m_iSizeY, m_iPosY), 693 - m_iSizeY);
		// 20 <= m_iPosX <= 990 - 20
		m_iPosX = min(max(m_iSizeX, m_iPosX), 990 - m_iSizeX);
		Invalidate();
		break;
	case WM_KEYUP:
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			m_iMoveX = 0;
			break;
		case VK_RIGHT:
			m_iMoveX = 0;
			break;
		case VK_UP:
			m_iMoveY = 0;
			break;
		case VK_DOWN:
			m_iMoveY = 0;
			m_bDown = FALSE;
			break;
		}
		break;
		
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
