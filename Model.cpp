#include"pch.h"
#include"Model.h"

void Model::ABCD1()
{
	Omega = Omegam;

	for (int i = 1; i < d + 1; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{
			D[i][j] = D1(i, j);
		}
	}

	//верхняя граница дна
	for (int j = 1; j < wn + 2; j++)
	{
		D[d + 1][j] = 0;
	}

	if (wn < wj)
	{
		for (int j = wn + 2; j < wj + 2; j++)
		{
			D[d + 1][j] = 0;
		}
	}
	D[d + 1][wj + 1] = 0;

	//вертикальные границы воды/нагревателя
	if (wn < wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			D[i][wn + 1] = 0;
		}
		for (int i = d + 2; i < d + hj + 3; i++)
		{
			D[i][wj + 1] = 0;
		}
		D[d + hj + 2][wj + 1] = 0;
	}
	if (wn == wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			D[i][wn + 1] = 0;
		}
		D[d + hj + 2][wj + 1] = 0;
	}

	//горизонтальная граница нагревателя
	if (hn < hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			D[d + 2 + hn][j] = 0;
		}
	}

	//нижняя граница крышки
	if (hn < hj)
	{
		for (int j = 1; j < wj + 1; j++)
		{
			D[d + hj + 2][j] = 0;
		}
	}

	if (hn == hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			D[d + hj + 2][j] = 0;
		}
	}

	//крышка
	Omega = Omegam;

	for (int i = d + hj + 3; i < d + d + hj + 3; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{
			D[i][j] = D1(i, j);
		}
	}

	//нагреватель
	Omega = Omegan;

	for (int i = d + 2; i < d + hn + 2; i++)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			D[i][j] = D1(i, j);
		}
	}
	//вода
	Omegaj;

	if (wn < wj)
	{
		if (hn < hj)
		{
			for (int i = d + 2; i < d + hn + 3; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					D[i][j] = D1(i, j);
				}
			}
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					D[i][j] = D1(i, j);
				}
			}
		}
		else
		{
			for (int i = d + 2; i < d + hj + 2; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					D[i][j] = D1(i, j);
				}
			}
		}
	}
	else
	{
		if (hn < hj)
		{
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					D[i][j] = D1(i, j);
				}
			}
		}
	}

	//металл
	Omega = Omegam;

	for (int i = d + 1; i < d + hj + 3; i++)
	{
		for (int j = wj + 2; j < jmax - 1; j++)
		{
			D[i][j] = D1(i, j);
		}
	}
}

double Model::A1(int j)
{
	double t1 = Omega * dt / (4. * dr);
	double t2 = -1. / dr + 1. / (2. * j * dr);
	return t1 * t2;
}
double Model::B1(int j)
{
	double t1 = -Omega * dt / (4. * dr);
	double t2 = 1. / dr + 1. / (2. * j * dr);
	return t1 * t2;
}

double Model::C1()
{
	return 1 + Omega * dt / (2 * dr * dr);
}

double Model::D1(int i, int j)
{
	double t1 = (2 - C[i][j]) * data[i][j];
	double t2 = -B[i][j] * data[i][j + 1];
	double t3 = -A[i][j] * data[i][j - j];
	double t4 = Omega * dt * (data[i + 1][j] + data[i - 1][j] - 2 * data[i][j]) / (2 * dr * dr);
	double t5 = dt * P[i][j] / 2;
	return t1 + t2 + t3 + t4 + t5;
}

void Model::ABCD2()
{
	Omega = Omegam;

	for (int i = 1; i < d + 1; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{
			Dz[i][j] = D2(i, j);
		}
	}

	//верхняя граница дна
	for (int j = 1; j < wn + 2; j++)
	{
		Dz[d + 1][j] = 0;
	}

	if (wn < wj)
	{
		for (int j = wn + 2; j < wj + 2; j++)
		{
			Dz[d + 1][j] = 0;
		}
	}
	Dz[d + 1][wj + 1] = 0;

	//вертикальные границы воды/нагревателя
	if (wn < wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			Dz[i][wn + 1] = 0;
		}
		Dz[d + hn + 2][wn + 1] = 0;

		for (int i = d + 2; i < d + hj + 3; i++)
		{
			Dz[i][wj + 1] = 0;
		}
	}
	if (wn == wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			Dz[i][wn + 1] = 0;
		}
		Dz[d + hn + 2][wn + 1] = 0;
	}

	//горизонтальная граница нагревателя
	if (hn < hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			Dz[d + 2 + hn][j] = 0;
		}
	}

	//нижняя граница крышки
	if (hn < hj)
	{
		for (int j = 1; j < wj + 1; j++)
		{
			Dz[d + hj + 2][j] = 0;
		}
	}

	if (hn == hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			Dz[d + hj + 2][j] = 0;
		}
	}

	//крышка
	Omega = Omegam;

	for (int i = d + hj + 3; i < d + d + hj + 3; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{

			Dz[i][j] = D2(i, j);
		}
	}

	//нагреватель
	Omega = Omegan;

	for (int i = d + 2; i < d + hn + 2; i++)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			Dz[i][j] = D2(i, j);
		}
	}
	//вода
	Omega = Omegaj;

	if (wn < wj)
	{
		if (hn < hj)
		{
			for (int i = d + 2; i < d + hn + 3; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					Dz[i][j] = D2(i, j);
				}
			}
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					Dz[i][j] = D2(i, j);
				}
			}
		}
		else
		{
			for (int i = d + 2; i < d + hj + 2; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					Dz[i][j] = D2(i, j);
				}
			}
		}
	}
	else
	{
		if (hn < hj)
		{
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					Dz[i][j] = D2(i, j);
				}
			}
		}
	}

	//металл
	Omega = Omegam;
	for (int i = d + 1; i < d + hj + 3; i++)
	{
		for (int j = wj + 2; j < jmax - 1; j++)
		{
			Dz[i][j] = D2(i, j);
		}
	}

	

}

double Model::A2()
{
	return -Omega * dt / (4 * dr * dr);
}

double Model::B2()
{
	return -Omega * dt / (4 * dr * dr);;
}

double Model::C2()
{
	return 1 + Omega * dt / (2 * dr * dr);;
}

double Model::D2(int i, int j)
{
	double t1 = Omega * dt * (data[i][j + 1] + data[i][j - 1] - 2 * data[i][j]) / (2 * dr * dr);
	double t2 = Omega * dt * (data[i][j + 1] - data[i][j - 1]) / (2 * dr * j * 2 * dr);
	double t3 = dt * P[i][j] / 2;
	double t4 = (2 - Cz[i][j]) * data[i][j] - Az[i][j] * data[i - 1][j] - Bz[i][j] * data[i + 1][j];
	return t1 + t2 + t3 + t4;
}

void Model::InitInstance()
{
	psi = ka / dr / km;
	muN = 1 - psi / 2 / km;
	muN /= 1 + psi / 2 / km;
	nuN = psi * T0 / (1 + psi / 2 / km);

	Omega = 1;
	Omegam = km * t0 / cvm / L0 / L0;
	Omegaj = kj * t0 / cvj / L0 / L0;
	Omegan = kn * t0 / cvn / L0 / L0;

	imax = 2 * d + hj + 4;
	if (hn == hj)imax--;

	jmax = wj + d + 3;

	MakeStartVals();
	
	MakeP();
	
	MakeABC();
	ResizeAlphaBetta();

	/*print(A, "A");
	print(B, "B");
	print(C, "C");

	print(Az, "Az");
	print(Bz, "Bz");
	print(Cz, "Cz");

	print(P, "P");*/
}

void Model::CalcAlphaBetta()
{
	for (int i = 1; i < imax - 1; i++)
	{
		alpha[i][1] = mu0;
		betta[i][1] = nu0;
		for (int j = 2; j < jmax; j++)
		{
			alpha[i][j] = -B[i][j - 1] / (C[i][j - 1] + A[i][j - 1] * alpha[i][j - 1]);
			betta[i][j] = (D[i][j - 1] - A[i][j - 1] * betta[i][j - 1]) / (C[i][j - 1] + A[i][j - 1] * alpha[i][j - 1]);
		}
	}
}

void Model::CalcAlphaBettaz()
{
	for (int j = 1; j < jmax - 1; j++)
	{
		alpha[1][j] = muN;
		betta[1][j] = nuN;
		for (int i = 2; i < imax; i++)
		{
			alphaz[i][j] = -Bz[i - 1][j] / (Cz[i - 1][j] + Az[i - 1][j] * alphaz[i - 1][j]);
			bettaz[i][j] = (Dz[i - 1][j] - Az[i - 1][j] * bettaz[i - 1][j]) / (Cz[i - 1][j] + Az[i - 1][j] * alphaz[i - 1][j]);
		}
	}
}

void Model::CalcHalfStepT()
{
	for (int i = 1; i < imax - 1; i++)
	{
		data[i][jmax - 1] = (muN * betta[i][jmax - 1] + nuN) / (1 - muN * alpha[i][jmax - 1]);
		for (int j = jmax - 2; j > 0; j--)
		{
			data[i][j - 1] = alpha[i][j] * data[i][j] + betta[i][j];
		}
		data[i][0] = mu0 * data[i][1] + nu0;
	}

	data[0][0] = mu0 * data[0][1] + nu0;
	data[imax - 1][0] = mu0 * data[imax - 1][1] + nu0;
	data[0][jmax - 1] = muN * data[0][jmax - 2] + nuN;
	data[imax - 1][jmax - 1] = muN * data[imax - 1][jmax - 2] + nuN;

}

void Model::CalcHalfStepTz()
{
	for (int j = 1; j < jmax - 1; j++)
	{
		data[imax -1][j] = (muN * betta[imax - 1][j] + nuN) / (1 - muN * alpha[imax - 1][j]);
		for (int i = imax - 2; i > 0; i--)
		{
			data[i - 1][j] = alpha[i][j] * data[i][j] + betta[i][j];
		}
		data[0][j] = muN * data[1][j] + nuN;
	}
	data[0][0] = muN * data[1][0] + nuN;
	data[imax - 1][0] = muN * data[imax - 2][0] + nuN;
	data[0][jmax - 1] = muN * data[1][jmax - 1] + nuN;
	data[imax - 1][jmax - 1] = muN * data[imax - 2][jmax - 1] + nuN;
}

Model::Model()
{
	InitializeCriticalSection(&cs);
}

Model::~Model()
{
	DeleteCriticalSection(&cs);
}

void Model::main()
{
	InitInstance();
	iteration = 0;
	Continue = true;
	while (Continue)
	{
		ABCD1();
		print(D, "D");
		CalcAlphaBetta();
		EnterCriticalSection(&cs);
		CalcHalfStepT();
		LeaveCriticalSection(&cs);

		ABCD2();
		print(Dz, "Dz");
		CalcAlphaBettaz();
		EnterCriticalSection(&cs);
		CalcHalfStepTz();
		LeaveCriticalSection(&cs);
		print(data, "data");

		iteration++;
		psi = ka / sqrt(ka * iteration * dt / cva);
		muN = 1 - psi / 2 / km;
		muN /= 1 + psi / 2 / km;
		nuN = psi * T0 / (1 + psi / 2 / km);
	}
	
}

void Model::SetParams(int hn, int wn, int hj, int wj, int d, double kj, double kn, double km, double T, double P, double dt, double dr, double cvm, double cvn, double cvj)
{
	this->hn = hn;
	this->wn = wn;
	this->hj = hj;
	this->wj = wj;
	this->d = d;
	this->kj = kj;
	this->kn = kn;
	this->km = km;
	this->T0 = T;
	this->dt = dt;
	this->dr = dr;
	this->cvm = cvm;
	this->cvj = cvj;
	this->cvn = cvn;
	this->P0 = P / cvn;

}

vector<vector<double>> Model::GetData()
{
	EnterCriticalSection(&cs);
	vector<vector<double>>res = data;
	LeaveCriticalSection(&cs);
	return res;
}

void Model::MakeStartVals()
{
	data.resize(imax);
	for (int i = 0; i < imax; i++)
	{
		data[i].resize(jmax);
		for (int j = 0; j < jmax; j++)
		{
			data[i][j] = T0;
		}
	}
}

void Model::MakeP()
{
	P.resize(imax);
	for (int i = 0; i < imax; i++)P[i].resize(jmax);

	int iborder = d + 2 + hn;
	for (int i = d + 2; i < iborder; i++)
	{
		for (int j = 1; j <= wn; j++)
		{
			P[i][j] = P0;
		}
	}
}

void Model::MakeABC()
{
	A.resize(imax);
	B.resize(imax);
	C.resize(imax);
	D.resize(imax);

	Az.resize(imax);
	Bz.resize(imax);
	Cz.resize(imax);
	Dz.resize(imax);
	for (int i = 0; i < imax; i++)
	{
		A[i].resize(jmax);
		B[i].resize(jmax);
		C[i].resize(jmax);
		D[i].resize(jmax);

		Az[i].resize(jmax);
		Bz[i].resize(jmax);
		Cz[i].resize(jmax);
		Dz[i].resize(jmax);
	}

	//дно
	Omega = Omegam;
	for (int i = 1; i < d + 1; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{
			A[i][j] = A1(j);
			B[i][j] = B1(j);
			C[i][j] = C1();

			Az[i][j] = A2();
			Bz[i][j] = B2();
			Cz[i][j] = C2();
		}
	}

	//верхняя граница дна
	for (int j = 1; j < wn + 2; j++)
	{
		A[d + 1][j] = km;
		B[d + 1][j] = km;
		C[d + 1][j] = -km - km;

		Az[d + 1][j] = km;
		Bz[d + 1][j] = kn;
		Cz[d + 1][j] = -km - kn;
	}

	if (wn < wj)
	{
		for (int j = wn + 2; j < wj + 2; j++)
		{
			A[d + 1][j] = km;
			B[d + 1][j] = km;
			C[d + 1][j] = -km - km;

			Az[d + 1][j] = km;
			Bz[d + 1][j] = kj;
			Cz[d + 1][j] = -km - kj;
		}
	}
	Bz[d + 1][wj + 1] = km;
	Cz[d + 1][wj + 1] = -km - km;

	//вертикальные границы воды/нагревателя
	if (wn < wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			A[i][wn + 1] = kn;
			B[i][wn + 1] = kj;
			C[i][wn + 1] = -kn - kj;

			Az[i][wn + 1] = kn;
			Bz[i][wn + 1] = kn;
			Cz[i][wn + 1] = -kn - kn;
		}
		Bz[d + hn + 2][wn + 1] = kj;
		Cz[d + hn + 2][wn + 1] = -kn - kj;

		for (int i = d + 2; i < d + hj + 3; i++)
		{
			A[i][wj + 1] = kj;
			B[i][wj + 1] = km;
			C[i][wj + 1] = -kj - km;

			Az[i][wj + 1] = km;
			Bz[i][wj + 1] = km;
			Cz[i][wj + 1] = -km - km;
		}
		A[d + hj + 2][wj + 1] = km;
		B[d + hj + 2][wj + 1] = km;
		C[d + hj + 2][wj + 1] = -km - km;
	}
	if (wn == wj)
	{
		for (int i = d + 2; i < d + hn + 3; i++)
		{
			A[i][wn + 1] = kn;
			B[i][wn + 1] = km;
			C[i][wn + 1] = -kn - km;

			Az[i][wn + 1] = kn;
			Bz[i][wn + 1] = kn;
			Cz[i][wn + 1] = -kn - kn;
		}
		if (hn < hj)
		{
			Bz[d + hn + 2][wn + 1] = kj;
			Cz[d + hn + 2][wn + 1] = -kn - kj;
		}
		else
		{
			Bz[d + hn + 2][wn + 1] = km;
			Cz[d + hn + 2][wn + 1] = -kn - km;
		}
		A[d + hj + 2][wj + 1] = km;
		B[d + hj + 2][wj + 1] = km;
		C[d + hj + 2][wj + 1] = -km - km;
	}

	//горизонтальная граница нагревателя
	if (hn < hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			A[d + 2 + hn][j] = kn;
			B[d + 2 + hn][j] = kn;
			C[d + 2 + hn][j] = -kn - kn;

			Az[d + 2 + hn][j] = kn;
			Bz[d + 2 + hn][j] = kj;
			Cz[d + 2 + hn][j] = -kn - kj;
		}
	}

	//нижняя граница крышки
	if (hn < hj)
	{
		for (int j = 1; j < wj + 1; j++)
		{
			A[d + hj + 2][j] = km;
			B[d + hj + 2][j] = km;
			C[d + hj + 2][j] = -km - km;

			Az[d + hj + 2][j] = kj;
			Bz[d + hj + 2][j] = km;
			Cz[d + hj + 2][j] = -kj - km;
		}
	}

	if (hn == hj)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			A[d + hj + 2][j] = km;
			B[d + hj + 2][j] = km;
			C[d + hj + 2][j] = -km - km;

			Az[d + hj + 2][j] = kn;
			Bz[d + hj + 2][j] = km;
			Cz[d + hj + 2][j] = -kn - km;
		}
	}

	//крышка
	Omega = Omegam;

	for (int i = d + hj + 3; i < d + d + hj + 3; i++)
	{
		for (int j = 1; j < jmax - 1; j++)
		{
			A[i][j] = A1(j);
			B[i][j] = B1(j);
			C[i][j] = C1();

			Az[i][j] = A2();
			Bz[i][j] = B2();
			Cz[i][j] = C2();
		}
	}

	//нагреватель
	Omega = Omegan;

	for (int i = d + 2; i < d + hn + 2; i++)
	{
		for (int j = 1; j < wn + 1; j++)
		{
			A[i][j] = A1(j);
			B[i][j] = B1(j);
			C[i][j] = C1();

			Az[i][j] = A2();
			Bz[i][j] = B2();
			Cz[i][j] = C2();
		}
	}
	//вода
	Omega = Omegaj;

	if (wn < wj)
	{
		if (hn < hj)
		{
			for (int i = d + 2; i < d + hn + 3; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					A[i][j] = A1(j);
					B[i][j] = B1(j);
					C[i][j] = C1();

					Az[i][j] = A2();
					Bz[i][j] = B2();
					Cz[i][j] = C2();
				}
			}
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					A[i][j] = A1(j);
					B[i][j] = B1(j);
					C[i][j] = C1();

					Az[i][j] = A2();
					Bz[i][j] = B2();
					Cz[i][j] = C2();
				}
			}
		}
		else
		{
			for (int i = d + 2; i < d + hj + 2; i++)
			{
				for (int j = wn + 2; j < wj + 1; j++)
				{
					A[i][j] = A1(j);
					B[i][j] = B1(j);
					C[i][j] = C1();

					Az[i][j] = A2();
					Bz[i][j] = B2();
					Cz[i][j] = C2();
				}
			}
		}
	}
	else
	{
		if (hn < hj)
		{
			for (int i = d + hn + 3; i < d + hj + 2; i++)
			{
				for (int j = 1; j < wj + 1; j++)
				{
					A[i][j] = A1(j);
					B[i][j] = B1(j);
					C[i][j] = C1();

					Az[i][j] = A2();
					Bz[i][j] = B2();
					Cz[i][j] = C2();
				}
			}
		}
	}

	//металл
	Omega = Omegam;

	for (int i = d + 1; i < d + hj + 3; i++)
	{
		for (int j = wj + 2; j < jmax - 1; j++)
		{
			A[i][j] = A1(j);
			B[i][j] = B1(j);
			C[i][j] = C1();

			Az[i][j] = A2();
			Bz[i][j] = B2();
			Cz[i][j] = C2();
		}
	}
}

void Model::ResizeAlphaBetta()
{
	alpha.resize(imax);
	alphaz.resize(imax);
	betta.resize(imax);
	bettaz.resize(imax);
	for (int i = 0; i < imax; i++)
	{
		alpha[i].resize(jmax);
		alphaz[i].resize(jmax);
		betta[i].resize(jmax);
		bettaz[i].resize(jmax);
	}
}

#include<fstream>
#include<iomanip>
void print(vector<vector<double>>& data, char* filename)
{
	ofstream of(filename);
	of << filename << ':' << endl << endl;
	of << setw(8) << setprecision(1) << setfill(' ') << ' ';

	for (int i = 0; i < data[0].size(); i++)
	{
		of << setw(8) << setprecision(1) << setfill(' ') << i;
	}
	of << endl;

	for (int i = 0; i < data.size(); i++)
	{
		of << setw(8) << setprecision(1) << setfill(' ') << i;
		for (int j = 0; j < data[i].size(); j++)
		{
			of << setw(8) << setprecision(1) << setfill(' ') << fixed << data[i][j];
		}
		of << endl;
	}
	of.close();
}
