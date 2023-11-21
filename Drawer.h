#pragma once


// Drawer
#include<vector>
#include<algorithm>
#include"Drawer3D.h";
using namespace std;
using namespace Gdiplus;


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

public:
	Drawer();
	virtual ~Drawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	void Drawer::Normalize();
	inline m_Color Drawer::GetColor(double val);
	inline Color Drawer::GetQuadreColor(int i, int j);
	inline void MakeColorData();
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


