#pragma once

#include<gl/GL.h>
#include<vector>
#include<algorithm>
#pragma comment(lib, "opengl32.lib")
#include<gdiplus.h>

// Drawer3D
struct  m_Color
{
	double r = 0; double g = 0; double b = 0;
	m_Color() {}
	m_Color(double r, double g, double b) :r(r), g(g), b(b) {}
	Gdiplus::Color AsColor() { return Gdiplus::Color(r, g, b); }
	m_Color& operator+ (const m_Color& right) { r += right.r; g += right.g; b += right.b; r /= 2; g /= 2; b /= 2; return *this; }
	void ToOpenGlRange() { r /= 255; g /= 255; b /= 255; }
};
class Drawer3D : public CStatic
{
	DECLARE_DYNAMIC(Drawer3D)

private:
	friend class Drawer;
	std::vector<m_Color>Colors;
	HGLRC glRC = NULL;
	std::vector<std::vector<double>>data;
	std::vector<std::vector<m_Color>>Colordata;

	double xmax = 0;
	double xmin = 0;
	double ymax = 0;
	double ymin = 0;
	double zmax = 0;
	double zmin = 0;

	double RealL = 0;
	double RealR = 0;

	
	bool init = false;

	bool lbdown = false;
	CPoint p, p2;
	UINT_PTR timerid;

	double scale = 1;
public:
	Drawer3D();
	virtual ~Drawer3D();

protected:
	void AllColorsToGlRange();
	inline m_Color GetColor(double val);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	void Drawer3D::InitInstance(HDC hdc);
	void SetData(std::vector<std::vector<double>>& in);
	void Normalize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void ResetRotate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	double rx = 90;
	double ry = 0;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

};


