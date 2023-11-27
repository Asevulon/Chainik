// ParamsDlg.cpp: файл реализации
//

#include "pch.h"
#include "Chainik.h"
#include "afxdialogex.h"
#include "ParamsDlg.h"


// Диалоговое окно ParamsDlg

IMPLEMENT_DYNAMIC(ParamsDlg, CDialogEx)

ParamsDlg::ParamsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, km(80.4)
	, kj(0.56)
	, kn(80.4)
	, cm(460)
	, cj(4200)
	, cn(460)
	, P(5000)
	, T(300)
	, dr(1e-4)
	, dt(1e-8)
{

}

ParamsDlg::~ParamsDlg()
{
}

void ParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPICTURE, drw);
	DDX_Control(pDX, IDC_COMBO1, MatType);
	DDX_Control(pDX, IDC_COMBO2, CBCellType);
	DDX_Text(pDX, IDC_EDIT3, km);
	DDX_Text(pDX, IDC_EDIT2, kj);
	DDX_Text(pDX, IDC_EDIT1, kn);
	DDX_Text(pDX, IDC_EDIT6, cm);
	DDX_Text(pDX, IDC_EDIT5, cj);
	DDX_Text(pDX, IDC_EDIT4, cn);
	DDX_Text(pDX, IDC_EDIT8, P);
	DDX_Text(pDX, IDC_EDIT7, T);
	DDX_Text(pDX, IDC_EDIT10, dr);
	DDX_Text(pDX, IDC_EDIT9, dt);
}


BEGIN_MESSAGE_MAP(ParamsDlg, CDialogEx)
	ON_MESSAGE(MS_CELL_CHANGE, &ParamsDlg::OnMsCellChange)
	ON_BN_CLICKED(IDOK, &ParamsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений ParamsDlg


BOOL ParamsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	ModelCellsParams p;
	p.wj = 30;
	p.hj = 30;
	p.d = 5;
	mc.Create(p);
	drw.SetData(mc.GetDrawerData());
	drw.SetPadding(10, 5, 22, 22);
	drw.Invalidate();

	MatType.InsertString(-1, L"Нагреватель");
	MatType.InsertString(-1, L"Металл");
	MatType.InsertString(-1, L"Жидкость");
	MatType.SetCurSel(0);

	CBCellType.InsertString(-1, L"Материал");
	CBCellType.InsertString(-1, L"Граница r");
	CBCellType.InsertString(-1, L"Граница z");
	CBCellType.SetCurSel(0);

	drw.parent = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

LRESULT ParamsDlg::OnMsCellChange(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CBCellType.GetLBText(CBCellType.GetCurSel(), str);
	if (str == L"Материал")
	{
		MatType.GetLBText(MatType.GetCurSel(), str);
		CellType ct = none;
		if (str == L"Металл")ct = metall;
		else
			if (str == L"Жидкость")ct = liquid;
			else
				if (str == L"Нагреватель")ct = heater;
		mc.SetCellMaterial(drw.ci, drw.cj, ct);
	}
	else
	{
		bool br(false), bz(false);
		if (str == L"Граница r")
		{
			br = true;
			bz = false;
		}
		else
			if (str == L"Граница z")
			{
				br = false;
				bz = true;
			}
		mc.SetCellType(drw.ci, drw.cj, br, bz);
	}
	drw.SetData(mc.GetDrawerData());
	drw.Invalidate();
	return LRESULT();
}


void ParamsDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	out.dr = dr;
	out.dt = dt;

	out.P0 = P / cn;
	out.T = T;
	
	out.kj = kj;
	out.kn = kn;
	out.km = km;
	out.Omegaj = kj / cj;
	out.Omegam = km / cm;
	out.Omegan = kn / cn;
	mc.SetParams(out);
	m->SetModelCells(mc);
	CDialogEx::OnOK();
}
