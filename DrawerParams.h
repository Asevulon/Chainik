#pragma once


// Drawer
#include<gdiplus.h>
#include<vector>
#include<algorithm>
using namespace std;
using namespace Gdiplus;
#include"Model.h"
#define MS_CELL_CHANGE WM_USER + 1

class DrawerParams : public CStatic
{
	DECLARE_DYNAMIC(DrawerParams)
private:
	vector<vector<DrawerParamsInfo >> data;
	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;
	double zmax;
	double zmin;
	double outzmax;
	double outzmin;
	ULONG_PTR token;
	
	CString title;

	bool holded = false;
	int width;
	int height;
public:
	DrawerParams();
	virtual ~DrawerParams();

protected:
	double CalcStringLen(HDC hDC, CString string);
	inline Color GetCellColor(int i, int j);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void DrawerParams::SetData(std::vector < std::vector <DrawerParamsInfo >> &in);

	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetTitle(CString str);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CWnd* parent;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	int ci = 0;
	int cj = 0;
};


