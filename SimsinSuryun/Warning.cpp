// Warning.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Warning.h"
#include "afxdialogex.h"



// Warning ��ȭ �����Դϴ�.

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


// Warning �޽��� ó�����Դϴ�.

BOOL Warning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TransparentButton();

	SetBackgroundImage(IDB_WARNING);

	CString csMsg = _T("");
	csMsg = "������ ��������?";

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(true);
}


void Warning::OnBnClickedNo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(false);
}


void Warning::OnBnClickedBtnClose()
{
	EndDialog(false);
}

