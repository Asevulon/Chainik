// Drawer3D.cpp: файл реализации
//

#include "pch.h"
#include "Chainik.h"
#include "Drawer3D.h"
#include<fstream>
using namespace std;
// Drawer3D

IMPLEMENT_DYNAMIC(Drawer3D, CStatic)

Drawer3D::Drawer3D():
	glRC(NULL)
{
	ifstream ifstr("DivergentColorMap.txt");
	for (int i = 0; i < 11; i++)
	{
		double r, g, b;
		ifstr >> r >> g >> b;
		r /= 255;
		g /= 255;
		b /= 255;
		Colors.push_back(m_Color(r, g, b));
	}
	ifstr.close();
	ResetRotate();
}

Drawer3D::~Drawer3D()
{
	wglDeleteContext(glRC);
}


BEGIN_MESSAGE_MAP(Drawer3D, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer3D




inline m_Color Drawer3D::GetColor(double val)
{
	double step = (zmax - zmin) / 9;
	double start = zmin;
	int i = 0;
	if (step != 0)
	{
		double ii = (val - zmin) / step;
		i = ii;
		if (ii - i > 0)i++;
	}
	m_Color res;
	double k = (zmin + step * i - val) / step;
	res.r = (1 - k) * Colors[i + 1].r + k * Colors[i].r;
	res.g = (1 - k) * Colors[i + 1].g + k * Colors[i].g;
	res.b = (1 - k) * Colors[i + 1].b + k * Colors[i].b;
	return res;
}

void Drawer3D::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* hdc = GetDC();
	if (!init)
	{
		InitInstance(*hdc);
		init = true;
	}
	wglMakeCurrent(*hdc, glRC);
	glClearColor(0.8, 0.8, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	if (data.empty())return;

	/*glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(rx, 1, 0, 0);
	glRotated(ry, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double b;
	b = scale * max(fabs(xmax), (fabs(ymin)));
	glOrtho(-b, b, -b, b, -b, b);
	//glTranslatef(0, b / 4., 0);

	int width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	int x0(0), y0(0);
	if (width > height)
	{
		x0 = (width - height) / 2.;
		glViewport(x0, y0, height, height);
	}
	else
	{
		y0 = (height - width) / 2.;
		glViewport(x0, y0, width, width);

	}


	

	glBegin(GL_LINES);

	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(b, 0, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, b, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, b);


	glEnd();


	for (int i = 0; i < ymax - 1; i++)
	{
		for (int j = 0; j < xmax - 1; j++)
		{
			glBegin(GL_QUADS);

			auto& ij = Colordata[i][j];
			ij.ToOpenGlRange();
			glColor3d(ij.r, ij.g, ij.b);
			glVertex3d(i, j, data[i][j]);

			auto& i1j = Colordata[i + 1][j];
			i1j.ToOpenGlRange();
			glColor3d(i1j.r, i1j.g, i1j.b);
			glVertex3d(i + 1, j, data[i + 1][j]);

			auto& i1j1 = Colordata[i + 1][j + 1];
			i1j1.ToOpenGlRange();
			glColor3d(i1j1.r, i1j1.g, i1j1.b);
			glVertex3d(i + 1, j + 1, data[i + 1][j + 1]);

			auto& ij1 = Colordata[i][j + 1];
			ij1.ToOpenGlRange();
			glColor3d(ij1.r, ij1.g, ij1.b);
			glVertex3d(i, j + 1, data[i][j + 1]);

			glEnd();
		}
	}


	

	glFlush();
	SwapBuffers(*hdc);
	wglMakeCurrent(NULL, NULL);
}

void Drawer3D::InitInstance(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32
	};

	pfd.cDepthBits = 64;
	int Pixelformat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, Pixelformat, &pfd);
	glRC = wglCreateContext(hdc);
}

void Drawer3D::SetData(std::vector<std::vector<double>>& in)
{
	data = in;
	vector<double>Max;
	Max.resize(data.size());
	vector<double>Min = Max;
	for (int i = 0; i < data.size(); i++)
	{
		Max[i] = *max_element(data[i].begin(), data[i].end());
		Min[i] = *min_element(data[i].begin(), data[i].end());
	}
	zmax = *max_element(Max.begin(), Max.end());
	zmin = *min_element(Min.begin(), Min.end());
	
	xmax = data[0].size();
	xmin = 0;
	ymax = data.size();
	ymin = 0;

	Normalize();
}

void Drawer3D::Normalize()
{
	double Norm = max(xmax, ymax) / max(fabs(zmax), fabs(zmin));
	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j] *= Norm;
		}
	}
	zmax *= Norm;
	zmin *= Norm;
}


void Drawer3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	SetCapture();
	lbdown = true;
	p = point;
	KillTimer(timerid);
	timerid = SetTimer(123, 25, NULL);
	CStatic::OnLButtonDown(nFlags, point);
}


void Drawer3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	lbdown = false;
	KillTimer(timerid);
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}


void Drawer3D::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (lbdown)
	{
		p2 = point;
	}
	CStatic::OnMouseMove(nFlags, point);
}

void Drawer3D::ResetRotate()
{
	rx = -45;
	ry = -135;
}


void Drawer3D::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	switch (nChar)
	{
	case VK_LEFT:rx--;
		break;
	case VK_RIGHT:rx++;
		break;
	case VK_UP:ry++;
		break;
	case VK_DOWN:ry--;
		break;
	case 0x52:ResetRotate();
		break;
	default:
		break;
	}
	Invalidate();
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}




void Drawer3D::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (p2.x - p.x > 0)ry++;
	if (p2.x - p.x < 0)ry--;
	if (p2.y - p.y > 0) rx++;
	if (p2.y - p.y < 0) rx--;
	Invalidate();
	CStatic::OnTimer(nIDEvent);
}


BOOL Drawer3D::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (zDelta > 0)
	{
		scale /= 1 + double(zDelta) / (1 + zDelta);
	}
	else
	{
		zDelta *= -1;
		scale *= 1 + double(zDelta) / (1 + zDelta);
	}
	Invalidate();
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
