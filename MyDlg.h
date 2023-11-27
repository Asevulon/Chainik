
// MyDlg.h: файл заголовка
//

#pragma once
#include"Drawer.h"
#include"OpenGlDlg.h"
#include"ParamsDlg.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAINIK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer drw;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	int imax;
	int jmax;
	double phase = 0;
	UINT_PTR timerid;
	afx_msg void OnBnClickedButton3d();

	OpenGlDlg* dlg3d;
	HANDLE ModelThread;
	Model m;
	ParamsDlg pdlg;
	bool open3d = false;
};

DWORD WINAPI ModelThreadFunc(LPVOID p);
