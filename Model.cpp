#include"pch.h"
#include"Model.h"


void Model::InitInstance()
{
	mc.InitInstance();
	psi = ka / dr;
	muN = 1;
	nuN = 0;
	mu0 = 1;
	nu0 = 0;

	imax = mc.GetHeight();
	jmax = mc.GetWidth();

	A = mc.GetA();
	B = mc.GetB();
	C = mc.GetC();

	Az = mc.GetAz();
	Bz = mc.GetBz();
	Cz = mc.GetCz();

	data = mc.GetT();
	
	ResizeAlphaBetta();

	mc.CalcD();
	mc.CalcDz();
	D = mc.GetD();
	Dz = mc.GetDz();
	
	print(A, "A");
	print(B, "B");
	print(C, "C");
	print(D, "D");

	print(Az, "Az");
	print(Bz, "Bz");
	print(Cz, "Cz");
	print(Dz, "Dz");
	CalcAlphaBetta();
	CalcAlphaBettaz();
	print(alpha, "alpha");
	print(alphaz, "alphaz");
	print(betta, "betta");
	print(bettaz, "bettaz");

}

void Model::CalcAlphaBetta()
{
	/*for (int i = 1; i < imax - 1; i++)
	{
		alpha[i][1] = mu0;
		betta[i][1] = nu0;
		for (int j = 2; j < jmax; j++)
		{
			alpha[i][j] = -B[i][j - 1] / (C[i][j - 1] + A[i][j - 1] * alpha[i][j - 1]);
			betta[i][j] = (D[i][j - 1] - A[i][j - 1] * betta[i][j - 1]) / (C[i][j - 1] + A[i][j - 1] * alpha[i][j - 1]);
		}
	}*/
	for (int i = 1; i < imax - 1; i++)
	{
		alpha[i][1] = mu0;
		betta[i][1] = nu0;
		for (int j = 2; j < jmax; j++)
		{
			alpha[i][j] = -B[i][j - 1] / (A[i][j - 1] * alpha[i][j - 1] + C[i][j - 1]);
			betta[i][j] = (D[i][j - 1] - A[i][j - 1] * betta[i][j - 1])/ (A[i][j - 1] * alpha[i][j - 1] + C[i][j - 1]);
		}
	}
}

void Model::CalcAlphaBettaz()
{
	for (int j = 1; j < jmax - 1; j++)
	{
		alphaz[1][j] = muN;
		bettaz[1][j] = nuN;
		for (int i = 2; i < imax; i++)
		{
			alphaz[i][j] = -Bz[i - 1][j] / (Az[i - 1][j] * alphaz[i - 1][j] + Cz[i - 1][j]);
			bettaz[i][j] = (Dz[i - 1][j] - Az[i - 1][j] * bettaz[i - 1][j]) / (Az[i - 1][j] * alphaz[i - 1][j] + Cz[i - 1][j]);
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

	/*data[0][0] = mu0 * data[0][1] + nu0;
	data[imax - 1][0] = mu0 * data[imax - 1][1] + nu0;
	data[0][jmax - 1] = muN * data[0][jmax - 2] + nuN;
	data[imax - 1][jmax - 1] = muN * data[imax - 1][jmax - 2] + nuN;*/

}

void Model::CalcHalfStepTz()
{
	for (int j = 1; j < jmax - 1; j++)
	{
		data[imax -1][j] = (muN * bettaz[imax - 1][j] + nuN) / (1 - muN * alphaz[imax - 1][j]);
		for (int i = imax - 2; i > 0; i--)
		{
			data[i - 1][j] = alphaz[i][j] * data[i][j] + bettaz[i][j];
		}
		data[0][j] = muN * data[1][j] + nuN;
	}
	/*data[0][0] = muN * data[1][0] + nuN;
	data[imax - 1][0] = muN * data[imax - 2][0] + nuN;
	data[0][jmax - 1] = muN * data[1][jmax - 1] + nuN;
	data[imax - 1][jmax - 1] = muN * data[imax - 2][jmax - 1] + nuN;*/
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
	//return;
	iteration = 0;
	Continue = true;
	while (Continue)
	{
		mc.CalcD();
		D = mc.GetD();
		CalcAlphaBetta();
		EnterCriticalSection(&cs);
		CalcHalfStepT();
		mc.SetT(data);
		LeaveCriticalSection(&cs);
		//print(D, "D");

		mc.CalcDz();
		Dz = mc.GetDz();
		CalcAlphaBettaz();
		EnterCriticalSection(&cs);
		CalcHalfStepTz();
		mc.SetT(data);
		LeaveCriticalSection(&cs);
		//print(Dz, "Dz");

		iteration++;
		//break;
		//psi = ka / sqrt(ka * iteration * dt / cva);
		//muN = ;
		//nuN = psi * T0 / (1 + psi / 2 / km);
	}
	
}


vector<vector<double>> Model::GetData()
{
	EnterCriticalSection(&cs);
	vector<vector<double>>res = data;
	LeaveCriticalSection(&cs);
	return res;
}

void Model::SetModelCells(ModelCells& mcs)
{
	mc = mcs;
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
			of << setw(8) << setprecision(2) << setfill(' ') << fixed << data[i][j];
		}
		of << endl;
	}
	of.close();
}

void ModelCells::PresetT()
{
	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j].T = p.T;
		}
	}
}

void ModelCells::SetMaterial()
{
	for (int i = 0; i < p.d + 1; i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j].type = metall;
		}
	}

	for (int i = p.d + 1; i < p.d + p.hj; i++)
	{
		for (int j = 0; j < p.wj + 1; j++)
		{
			data[i][j].type = liquid;
		}
	}
	
	for (int i = p.d + 1; i < p.d + p.hj; i++)
	{
		for (int j = p.wj + 1; j < data[i].size(); j++)
		{
			data[i][j].type = metall;
		}
	}

	for (int i = p.d + p.hj; i < 2 * p.d + p.hj; i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j].type = metall;
		}
	}
}

void ModelCells::SetBorders()
{
	for (int j = 1; j < p.wj + 1; j++)
	{
		data[p.d][j].borderz = true;
	}

	for (int j = 1; j < p.wj + 1; j++)
	{
		data[p.d + p.hj][j].borderz = true;
	}

	for (int i = p.d + 1; i < p.d + p.hj; i++)
	{
		data[i][p.wj + 1].border = true;
	}
}

void ModelCells::SetP0()
{
	P.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		P[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j].type == heater) P[i][j] = p.P0;
		}
	}
}

void ModelCells::SetABC()
{
	/*for (int i = 1; i < data.size() - 1; i++)
	{
		for (int j = 1; j < p.wn + 2; j++)
		{
			if (data[i][j].border)
			{
				data[i][j].A = Getk(i, j);
				data[i][j].B = Getk(i, j + 1);
				data[i][j].C = -data[i][j].A - data[i][j].B;
				data[i][j].D = 0;
			}
			else
			{
				A1(i, j);
				B1(i, j);
				C1(i, j);
			}
		}
	}
	for (int i = 1; i < data.size() - 1; i++)
	{
		for (int j = p.wn + 2; j < data[i].size() - 1; j++)
		{
			if (data[i][j].border)
			{
				data[i][j].A = Getk(i, j - 1);
				data[i][j].B = Getk(i, j);
				data[i][j].C = -data[i][j].A - data[i][j].B;
				data[i][j].D = 0;
			}
			else
			{
				A1(i, j);
				B1(i, j);
				C1(i, j);
			}
		}
	}

	int fst = p.d + 1;
	for (int i = 1; i < fst; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (data[i][j].borderz)
			{
				data[i][j].Az = Getk(i, j);
				data[i][j].Bz = Getk(i + 1, j);
				data[i][j].Cz = -data[i][j].Az - data[i][j].Bz;
				data[i][j].Dz = 0;
			}
			else
			{
				A2(i, j);
				B2(i, j);
				C2(i, j);
			}
		}
	}

	int half = p.hn / 2 + p.d + 4;
	for (int i = fst; i < half; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (data[i][j].borderz)
			{
				data[i][j].Az = Getk(i - 1, j);
				data[i][j].Bz = Getk(i, j);
				data[i][j].Cz = -data[i][j].Az - data[i][j].Bz;
				data[i][j].Dz = 0;
			}
			else
			{
				A2(i, j);
				B2(i, j);
				C2(i, j);
			}
		}
	}
	int snd = half + p.hn / 2 + 2;
	for (int i = half ; i < snd; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (data[i][j].borderz)
			{
				data[i][j].Az = Getk(i, j);
				data[i][j].Bz = Getk(i + 1, j);
				data[i][j].Cz = -data[i][j].Az - data[i][j].Bz;
				data[i][j].Dz = 0;
			}
			else
			{
				A2(i, j);
				B2(i, j);
				C2(i, j);
			}
		}
	}

	for (int i = snd; i < data.size() - 1; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (data[i][j].borderz)
			{
				data[i][j].Az = Getk(i - 1, j);
				data[i][j].Bz = Getk(i, j);
				data[i][j].Cz = -data[i][j].Az - data[i][j].Bz;
				data[i][j].Dz = 0;
			}
			else
			{
				A2(i, j);
				B2(i, j);
				C2(i, j);
			}
		}
	}*/

for (int i = 1; i < data.size() - 1; i++)
{
	for (int j = 1; j < data[i].size() - 1; j++)
	{
		if (data[i][j].border)
		{
			data[i][j].A = Getk(i, j - 1);
			data[i][j].B = Getk(i, j + 1);
			data[i][j].C = -data[i][j].A - data[i][j].B;
			data[i][j].D = 0;
		}
		else
		{
			A1(i, j);
			B1(i, j);
			C1(i, j);
		}

		if (data[i][j].borderz)
		{
			data[i][j].Az = Getk(i - 1, j);
			data[i][j].Bz = Getk(i + 1, j);
			data[i][j].Cz = -data[i][j].Az - data[i][j].Bz;
			data[i][j].Dz = 0;
		}
		else
		{
			A2(i, j);
			B2(i, j);
			C2(i, j);
		}
	}
}

}

inline void ModelCells::A1(int i, int j)
{
	double t1 = GetOmega(i, j) * p.dt / (4. * p.dr);
	double t2 = -1. / p.dr + 1. / (2. * j * p.dr);
	data[i][j].A = t1 * t2;
}

inline void ModelCells::B1(int i, int j)
{
	double t1 = -GetOmega(i, j) * p.dt / (4. * p.dr);
	double t2 = 1. / p.dr + 1. / (2. * j * p.dr);
	data[i][j].B = t1 * t2;
}

inline void ModelCells::C1(int i, int j)
{
	data[i][j].C = 1 + GetOmega(i, j) * p.dt / (2 * p.dr * p.dr);
}

inline void ModelCells::D1(int i, int j)
{
	double t1 = (2 - data[i][j].C) * data[i][j].T;
	double t2 = -data[i][j].B * data[i][j + 1].T;
	double t3 = -data[i][j].A * data[i][j - 1].T;
	double t4 = GetOmega(i, j) * p.dt * (data[i + 1][j].T + data[i - 1][j].T - 2 * data[i][j].T) / (2 * p.dr * p.dr);
	double t5 = p.dt * P[i][j] / 2;
	data[i][j].D = t1 + t2 + t3 + t4 + t5;
}

inline void ModelCells::A2(int i, int j)
{
	data[i][j].Az = - GetOmega(i, j) * p.dt / (4 * p.dr * p.dr);
}

inline void ModelCells::B2(int i, int j)
{
	data[i][j].Bz = - GetOmega(i, j) * p.dt / (4 * p.dr * p.dr);
}

inline void ModelCells::C2(int i, int j)
{
	data[i][j].Cz = 1 + GetOmega(i, j) * p.dt / (2 * p.dr * p.dr);
}

inline void ModelCells::D2(int i, int j)
{
	double t1 = GetOmega(i, j) * p.dt * (data[i][j + 1].T + data[i][j - 1].T - 2 * data[i][j].T) / (2 * p.dr * p.dr);
	double t2 = GetOmega(i, j) * p.dt * (data[i][j + 1].T - data[i][j - 1].T) / (2 * p.dr * j * 2 * p.dr);
	double t3 = p.dt * P[i][j] / 2;
	double t4 = (2 - data[i][j].Cz) * data[i][j].T - data[i][j].Az * data[i - 1][j].T - data[i][j].Bz * data[i + 1][j].T;
	data[i][j].Dz = t1 + t2 + t3 + t4;
}

inline double ModelCells::GetOmega(int i, int j)
{
	switch (data[i][j].type)
	{
	case metall: return p.Omegam;
	case heater: return p.Omegan;
	case liquid: return p.Omegaj;
	default: abort();
	}
}

inline double ModelCells::Getk(int i, int j)
{
	switch (data[i][j].type)
	{
	case metall:return p.km;
	case liquid:return p.kj;
	case heater:return p.kn;
	default:
		abort();
	}
}

void ModelCells::PrintCellType(char*name)
{
	ofstream out(name);
	auto mas = GetCellsTypes();

	out << setw(8) << setprecision(1) << setfill(' ') << ' ';

	for (int i = 0; i < mas[0].size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
	}
	out << endl;

	for (int i = 0; i < mas.size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
		for (int j = 0; j < mas[i].size(); j++)
		{
			string str;
			if (mas[i][j] == none)str = "none";
			else
				if (mas[i][j] == metall)str = "metall";
				else
					if (mas[i][j] == liquid)str = "liquid";
					else
						if (mas[i][j] == heater)str = "heater";
			out << setw(8) << setfill(' ') << str;
		}
		out << endl;
	}
	out.close();
}
void ModelCells::PrintCellBorders(char* name)
{
	ofstream out(name);

	out << setw(8) << setprecision(1) << setfill(' ') << ' ';

	for (int i = 0; i < data[0].size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
	}
	out << endl;

	for (int i = 0; i < data.size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
		for (int j = 0; j < data[i].size(); j++)
		{
			out << setw(8) << setfill(' ') << data[i][j].border;
		}
		out << endl;
	}
	out.close();
}
void ModelCells::PrintCellBordersz(char* name)
{
	ofstream out(name);

	out << setw(8) << setprecision(1) << setfill(' ') << ' ';

	for (int i = 0; i < data[0].size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
	}
	out << endl;

	for (int i = 0; i < data.size(); i++)
	{
		out << setw(8) << setprecision(1) << setfill(' ') << i;
		for (int j = 0; j < data[i].size(); j++)
		{
			out << setw(8) << setfill(' ') << data[i][j].borderz;
		}
		out << endl;
	}
	out.close();
}

vector<vector<CellType>> ModelCells::GetCellsTypes()
{
	vector<vector<CellType>> res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].type;
		}
	}
	return res;
}

void ModelCells::Create(ModelCellsParams& p)
{
	if (created)return;
	created = true;
	p.wj;
	p.hj;
	this->p = p;
	data.resize(p.hj + 2 * p.d);
	for (int i = 0; i < data.size(); i++)
	{
		data[i].resize(p.d + p.wj);
	}
	SetMaterial();
	SetBorders();
}

vector<vector<double>> ModelCells::GetA()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].A;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetB()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].B;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetC()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].C;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetD()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].D;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetAz()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].Az;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetBz()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].Bz;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetCz()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].Cz;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetDz()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].Dz;
		}
	}
	return res;
}

vector<vector<double>> ModelCells::GetT()
{
	vector<vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			res[i][j] = data[i][j].T;
		}
	}
	return res;
}

void ModelCells::SetT(vector<vector<double>>& ts)
{
	for (int i = 0; i < ts.size(); i++)
	{
		for (int j = 0; j < ts[i].size(); j++)
		{
			data[i][j].T = ts[i][j];
		}
	}
}

void ModelCells::CalcD()
{
	for (int i = 1; i < data.size() - 1; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (!data[i][j].border)D1(i, j);
		}
	}
}

void ModelCells::CalcDz()
{
	for (int i = 1; i < data.size() - 1; i++)
	{
		for (int j = 1; j < data[i].size() - 1; j++)
		{
			if (!data[i][j].borderz)D2(i, j);
		}
	}
}

int ModelCells::GetHeight()
{
	return data.size();
}

int ModelCells::GetWidth()
{
	return data[0].size();
}

vector<vector<DrawerParamsInfo>> ModelCells::GetDrawerData()
{
	vector<vector<DrawerParamsInfo>>res;
	res.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < data[i].size(); j++)
		{
			DrawerParamsInfo info;
			info.type = data[i][j].type;
			info.border = data[i][j].border;
			info.borderz = data[i][j].borderz;
			res[i][j] = info;
		}
	}
	return res;
}

void ModelCells::SetCellMaterial(int i, int j, CellType ct)
{
	data[i][j].type = ct;
	data[i][j].border = false;
	data[i][j].borderz = false;
}

void ModelCells::SetCellType(int i, int j, bool border, bool borderz)
{
	data[i][j].border = border;
	data[i][j].borderz = borderz;
}

void ModelCells::SetParams(ModelCellsParams& p)
{
	this->p = p;
}

void ModelCells::InitInstance()
{
	PresetT();
	SetP0();
	SetABC();

	print(P, "P");
	print(GetT(), "data");
}


