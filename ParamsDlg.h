#pragma once
#include "afxdialogex.h"


// Диалоговое окно ParamsDlg
#include"Model.h"
#include"DrawerParams.h"
class ParamsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParamsDlg)

public:
	ParamsDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ParamsDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
public:
	DrawerParams drw;
	virtual BOOL OnInitDialog();
	ModelCells mc;
	CComboBox MatType;
	CComboBox CBCellType;

	afx_msg LRESULT OnMsCellChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();

	ModelCellsParams out;
	double km;
	double kj;
	double kn;
	double cm;
	double cj;
	double cn;
	double P;
	double T;
	double dr;
	double dt;
	Model* m;

};
