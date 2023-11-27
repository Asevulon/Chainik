#pragma once


// Drawer
#include<vector>
#include<algorithm>
#include"Drawer3D.h";
using namespace std;
using namespace Gdiplus;

#define LINECAP 100
class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
private:
	vector<vector<double>>data;
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

	
	std::vector<m_Color>Colors;
	vector<vector<m_Color>>Colordata;

	vector < vector<pair<double, double>>> lines;
	bool eqz = false;

	int linesnum = 6;
public:
	Drawer();
	virtual ~Drawer();

protected:
	
	double CalcStringLen(HDC hDC, CString string);
	void Drawer::Normalize();
	inline m_Color Drawer::GetColor(double val);
	inline Color Drawer::GetQuadreColor(int i, int j);
	inline void MakeColorData();
	pair<double, double>CalcLinesDot(double val, int i1, int j1, int i2, int j2);
	void CreateLines();
	double Distance(pair<double, double>& left, pair<double, double>& right);
	vector<pair<double, double>>MakeLine(vector<vector<bool>>& linedata);
	vector<pair<double, double>>TransformLine(vector<pair<double, double>>&ntline, double val);
	bool FindIntersectionT(int i, int j, double val, pair<double, double>& out);
	bool FindIntersectionB(int i, int j, double val, pair<double, double>& out);
	bool FindIntersectionL(int i, int j, double val, pair<double, double>& out);
	bool FindIntersectionR(int i, int j, double val, pair<double, double>& out);
	double CalcXc(double x1, double x2, double T1, double T2, double val);
	double CalcYc(double y1, double y2, double T1, double T2, double val);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void Drawer::SetData(std::vector<std::vector<double>>& in);

	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);
	void ShareWith(Drawer3D& target);
	
};


