// OpenGlDlg.cpp: файл реализации
//

#include "pch.h"
#include "Chainik.h"
#include "afxdialogex.h"
#include "OpenGlDlg.h"


// Диалоговое окно OpenGlDlg

IMPLEMENT_DYNAMIC(OpenGlDlg, CDialogEx)

OpenGlDlg::OpenGlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

OpenGlDlg::~OpenGlDlg()
{
}

void OpenGlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_3D_PICTURE, drw);
}


BEGIN_MESSAGE_MAP(OpenGlDlg, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений OpenGlDlg


BOOL OpenGlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

void OpenGlDlg::SetData(std::vector<std::vector<double>>& data)
{
	if (data.empty())return;
	drw.SetData(data);
	drw.Invalidate();
}
