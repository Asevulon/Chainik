// Drawer.cpp: файл реализации
//

#include "pch.h"
#include"DrawerParams.h"
#include<fstream>

// Drawer

IMPLEMENT_DYNAMIC(DrawerParams, CStatic)

DrawerParams::DrawerParams()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
}

DrawerParams::~DrawerParams()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(DrawerParams, CStatic)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




void DrawerParams::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics ToWindow(lpDrawItemStruct->hDC);
	width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (data.empty())
	{
		ToWindow.DrawImage(&bmp, 0, 0);
		return;
	}

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / 10.;
	float steplenX = (right - left) / 10.;

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < 11; i++)
	{

		CString str;
		str.Format(L"%.2f", top - i * steplenY);

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY -6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		str.Format(L"%.2f", right - i * steplenX);

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}


	PointF ps[5];
	SolidBrush databrush(Color::White);
	Pen datapen(Color::White);
	for (int i = 0; i < top; i++)
	{
		for (int j = 0; j < right; j++)
		{
			Color clr = GetCellColor(i, j);
			databrush.SetColor(clr);
			ps[0].X = j;
			ps[0].Y = i;

			ps[1].X = j + 1;
			ps[1].Y = i;
			
			ps[2].X = j + 1;
			ps[2].Y = i + 1;

			ps[3].X = j;
			ps[3].Y = i + 1;

			matr.TransformPoints(ps, 4);
			gr.FillPolygon(&databrush, ps, 4);
			ps[4] = ps[0];
			gr.DrawLines(&datapen, ps, 5);
		}
	}
	ToWindow.DrawImage(&bmp, 0, 0);
}


void DrawerParams::SetData(std::vector<std::vector<DrawerParamsInfo>>& in)
{
	if (in.empty())return;
	data = in;
	right = data[0].size();
	left = 0;
	top = data.size();
	bot = 0;
}


void DrawerParams::SetPadding(double lP, double rP, double tP, double bP)
{
	lPadding = lP;
	rPadding = rP;
	tPadding = tP;
	bPadding = bP;
}
void DrawerParams::SetTitle(CString str)
{
	title = str;
}

double DrawerParams::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}

inline Color DrawerParams::GetCellColor(int i, int j)
{
	if (data[i][j].border)return Color::SandyBrown;
	if (data[i][j].borderz)return Color::SaddleBrown;

	if (data[i][j].type == metall)return Color::LightGray;
	if (data[i][j].type == liquid)return Color::CornflowerBlue;
	if (data[i][j].type == heater)return Color::Red;
}


void DrawerParams::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	ReleaseCapture();
	holded = false;
	CStatic::OnLButtonUp(nFlags, point);
}


void DrawerParams::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	SetCapture();
	holded = true;
	CStatic::OnLButtonDown(nFlags, point);
}


void DrawerParams::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (holded)
	{
		double actWidth(right - left);
		double actHeight(top - bot);
		double actLP = actWidth * lPadding / 100.;
		double actRP = actWidth * rPadding / 100.;
		double actTP = actHeight * tPadding / 100.;
		double actBP = actHeight * bPadding / 100.;

		double actTop = top + actTP;
		double actBot = bot - actBP;
		double actLeft = left - actLP;
		double actRight = right + actRP;

		actWidth = actRight - actLeft;
		actHeight = actTop - actBot;


		double xScale = width / actWidth;
		double yScale = -height / actHeight;

		Pen BackGroundPen(Color(200, 200, 200));

		Matrix matr;
		matr.Scale(xScale, yScale);
		matr.Translate(actLP - left, -actTop);
		matr.Invert();
		PointF p;
		p.X = point.x;
		p.Y = point.y;
		matr.TransformPoints(&p);
		ci = p.Y;
		cj = p.X;
		if ((ci >= 0) && (ci < top) && (cj >= 0) && (cj < right))
		parent->SendMessage(MS_CELL_CHANGE);
	}
	CStatic::OnMouseMove(nFlags, point);
}
