#pragma once
#include "afxdialogex.h"


// Диалоговое окно OpenGlDlg
#include"Drawer3D.h"

class OpenGlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OpenGlDlg)

public:
	OpenGlDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~OpenGlDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	Drawer3D drw;
	void SetData(std::vector<std::vector<double>>& data);
	
	bool shown = false;
};
