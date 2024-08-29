
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

	// �� �ʱ�ȭ
	InitVars();
	
	// 120�� �ڿ� �Ѱܳ�
	SetTimer(1, m_lTicketTime * 60 * 1000, NULL);

	// ����ġ ���� ������
	SetTimer(2, 100, NULL);

	// ĳ���� ���� üũ
	 SetTimer(4, 10, NULL);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
	// ĳ���� ���� üũ
	else if (nIDEvent == 4)
	{
		CheckStatus();
		SetTimer(4, 10, NULL);
	}
}

void CSimsinSuryunDlg::InitVars()
{
	// Ƽ��Ÿ�� (default : 120��)
	m_lTicketTime = 120;

	// ������ �ð�
	m_endTime = CTime::GetCurrentTime() + CTimeSpan(0, 0, m_lTicketTime, 0);

	// ĳ���� ũ�� (����)
	m_iSizeX = 11;
	m_iSizeY = 20;

	// ĳ���� �ʱ� ��ġ
	m_iPosY = 300;
	m_iPosX = 300;

	// ó���� ���߿��� ����
	m_iMoveX = 0;
	m_iMoveY = 0;
	m_sStatus = CHARACTER_STATUS_AIR;
	m_bDown = FALSE;
	// ó���� ������ ��
	m_sDirect = 1;
}

void CSimsinSuryunDlg::CheckStatus()
{
	switch (m_sStatus)
	{
		// ��ٸ��� �ٿö󰡰ų�.. �������������� ��ٸ����� ��������.
		case CHARACTER_STATUS_LADDER:
			// ��ٸ� �ٿö�����
			if (!IsLadder())
			{
				m_iMoveY = 0;
				m_sStatus = CHARACTER_STATUS_AIR;
			}
			break;
		case CHARACTER_STATUS_GROUND:	
			break;
		case CHARACTER_STATUS_AIR:
			// �ٴ� ������ (���ǹ� �����ؾ���
			if (m_iMoveY >= 0 && m_iPosY == GetGround() - m_iSizeY)
			{
				m_sStatus = CHARACTER_STATUS_GROUND;
				// 				m_iMoveY = 0;
				// 				m_iMoveX = 0;
			}

			// ���� ���ӵ�
			m_iMoveY += 1;

			// �ִ� ���ϼӵ�
			m_iMoveY = min(100, m_iMoveY);


		default:
			break;
	}
	
	if (m_sStatus == CHARACTER_STATUS_LADDER)
		m_iMoveX = 0;
	// x�� �̵�
	m_iPosX += m_iMoveX;
	// m_iSizeX <= m_iPosX <= 990 - m_iSizeX
	m_iPosX = min(max(m_iSizeX, m_iPosX), 990 - m_iSizeX);
	

	// y�� �̵�
	m_iPosY += m_iMoveY;

	int iGroundY = GetGround();

	// m_iSizeY <= m_iPosY <= iGroundY - m_iSizeY
	if (m_sStatus != CHARACTER_STATUS_LADDER)
		m_iPosY = min(max(m_iSizeY, m_iPosY), iGroundY - m_iSizeY);
	// ��ٸ� ���� �κ��� ��� ����
//	else

	Invalidate();
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

BOOL CSimsinSuryunDlg::IsLadder()
{
	//(337, 447, 385, 534) 3�� ��ٸ�
	//(696, 565, 743, 649) 2�� ��ٸ�
	
	// ������ �ٴڰ� ��ٸ��� ������ �ϵ��ڵ�

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
	//(106, 447, 910, 491)	3��
	//(106, 565, 910, 611)	2��
	//(0, 693, 990, 806)	1��

	// ������ �ٴڰ� ��ٸ��� ������ �ϵ��ڵ�

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
		// ������ ���Ḧ ��ٸ��ϴ�.
		::WaitForSingleObject( m_pThread->m_hThread, INFINITE );

		delete m_pThread;
		m_pThread = NULL;
	}
	
	delete m_pDlg;
	m_pDlg = NULL;
}


void CSimsinSuryunDlg::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	//dc.SelectStockObject(NULL_BRUSH);
	// �ٴ� �׷�����
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(106,447, 910, 491);	// 3��
	dc.Rectangle(106,565, 910, 611);	// 2��
	dc.Rectangle(0,693, 990, 806);		// 1��

	// ��ٸ� �׷�����
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(337, 447, 385, 534);	// 3�� ��ٸ�
	dc.Rectangle(696, 565, 743, 649);	// 2�� ��ٸ�

	// ĳ���� �׸���
	dc.SelectStockObject(BLACK_BRUSH);
	// ���ִ� ����
	if (!m_bDown)
		dc.Rectangle(m_iPosX - m_iSizeX, m_iPosY - m_iSizeY, 
					m_iPosX + m_iSizeX, m_iPosY + m_iSizeY);
	// ���λ���
	else
		// ������
		if (m_sDirect == 1)
			dc.Rectangle(m_iPosX - m_iSizeX, m_iPosY + m_iSizeY - 2 * m_iSizeX, 
						m_iPosX - m_iSizeX + m_iSizeY * 2 , m_iPosY + m_iSizeY);
		// ����
		else
			dc.Rectangle(m_iPosX + m_iSizeX - 2 * m_iSizeY, m_iPosY + m_iSizeY - 2 * m_iSizeX, 
						m_iPosX + m_iSizeX, m_iPosY + m_iSizeY);
}

// Ű�Է�
BOOL CSimsinSuryunDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		switch (pMsg->wParam)
		{
			// ����
			case VK_LEFT:
				m_iMoveX = -5;
				m_sDirect = -1;
				break;
			// ������
			case VK_RIGHT:
				m_iMoveX = 5;
				m_sDirect = 1;
				break;
			// ��Ű
	 		case VK_UP:
				switch (m_sStatus)
				{
					case CHARACTER_STATUS_AIR:
					case CHARACTER_STATUS_GROUND:
						// ��Ű �������� ��ٸ��� ��ٸ� Ÿ��
						m_iMoveY = 0;
						if (IsLadder())
						{
							m_sStatus = CHARACTER_STATUS_LADDER;
						}
						break;
					case CHARACTER_STATUS_LADDER:
						// ��ٸ�Ÿ�� �ö󰡱�
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
						// �Ʒ�Ű �������� ��ٸ��� ��ٸ� Ÿ��
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
						//��ٸ�Ÿ�� ��������
						m_iMoveY = 5;
						break;
					case CHARACTER_STATUS_AIR:
					default:
						break;
				}
	 			break;
			case VK_MENU: // ALT
				// ����
				if (m_sStatus == CHARACTER_STATUS_GROUND)
				{
					m_iMoveY = -12;
					m_iPosY -= 1;
					m_sStatus = CHARACTER_STATUS_AIR;
				}
				// ��ٸ� ����
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

			// ���� â ���°� �߰� ����
			case VK_ESCAPE:
				AfxMessageBox(_T("����â"));
				// OnSetting();
				break;

			// ������ â ���°� �߰� ����
			case 0x49:
				AfxMessageBox(_T("������â"));
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
