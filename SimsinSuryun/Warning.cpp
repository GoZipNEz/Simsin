// Warning.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Warning.h"
#include "afxdialogex.h"



// Warning 대화 상자입니다.

IMPLEMENT_DYNAMIC(Warning, CDialogEx)

Warning::Warning(CWnd* pParent /*=NULL*/)
	: CDialogEx(Warning::IDD, pParent)
{

}

Warning::~Warning()
{
}

void Warning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Warning, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_YES, &Warning::OnBnClickedYes)
	ON_BN_CLICKED(IDC_BTN_NO, &Warning::OnBnClickedNo)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &Warning::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// Warning 메시지 처리기입니다.

BOOL Warning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TransparentButton();

	SetBackgroundImage(IDB_WARNING);

	// 현재 시간
	CTime curTime = CTime::GetCurrentTime();
	// 끝 시간 
	CTimeSpan remainTime  = m_endTime - curTime;

	CString csMsg = _T("");
	int		iTime = 0;
	// 남은시간 계산 ( ~~분으로 표시 )
	iTime = _ttoi(remainTime.Format("%M")) + (_ttoi(remainTime.Format("%H")) * 60);
	csMsg.Format(L"정말로 나갈꺼야? 남은시간 : %d분",  iTime);

	SetDlgItemText(IDC_MSG, csMsg);

	return TRUE;
}

void Warning::TransparentButton()
{
	m_btnYes.SubclassDlgItem(IDC_BTN_YES, this);
	m_btnNo.SubclassDlgItem(IDC_BTN_NO, this);
	m_btnClose.SubclassDlgItem(IDC_BTN_CLOSE, this);

	m_btnYes.DrawTransparent(this->IDD);
	m_btnNo.DrawTransparent(this->IDD);
	m_btnClose.DrawTransparent(this->IDD);
}

void Warning::OnBnClickedYes()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(true);
}


void Warning::OnBnClickedNo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(false);
}


void Warning::OnBnClickedBtnClose()
{
	EndDialog(false);
}

