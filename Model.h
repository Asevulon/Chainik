#pragma once
#include<vector>
using namespace std;


struct ModelCellsParams
{
	int hj = 0;
	int wj = 0;
	int hn = 0;
	int wn = 0;
	int d = 0;

	double kj = 0;
	double kn = 0;
	double km = 0;

	double Omegam = 0;
	double Omegaj = 0;
	double Omegan = 0;

	double T = 0;
	double dr = 0;
	double dt = 0;

	double P0;
};
class ModelCells
{
private:
	
	enum CellType
	{
		metall,
		heater,
		liquid,
		none
	};
	struct Cell
	{
		double A = 0;
		double B = 0;
		double C = 0;
		double D = 0;

		double Az = 0;
		double Bz = 0;
		double Cz = 0;
		double Dz = 0;

		double T = 0;
		
		CellType type = none;

		bool border = false;
		bool borderz = false;
	};

	ModelCellsParams p;
	vector<vector<Cell>>data;
	vector<vector<double>>P;
protected:
	void PresetT();
	void SetMaterial();
	void SetBorders();
	void SetP0();
	void SetABC();
	inline void A1(int i, int j);
	inline void B1(int i, int j);
	inline void C1(int i, int j);
	inline void D1(int i, int j);
	inline void A2(int i, int j);
	inline void B2(int i, int j);
	inline void C2(int i, int j);
	inline void D2(int i, int j);
	inline double GetOmega(int i, int j);
	inline double Getk(int i, int j);
	void ModelCells::PrintCellBordersz(char* name);
	void ModelCells::PrintCellBorders(char* name);
	void PrintCellType(char*);
	vector<vector<CellType>> GetCellsTypes();
public:
	void Create(ModelCellsParams& p);
	vector<vector<double>> GetA();
	vector<vector<double>> GetB();
	vector<vector<double>> GetC();
	vector<vector<double>> GetD();
	vector<vector<double>> GetAz();
	vector<vector<double>> GetBz();
	vector<vector<double>> GetCz();
	vector<vector<double>> GetDz();

	vector<vector<double>> GetT();
	void SetT(vector<vector<double>>& ts);

	void CalcD();
	void CalcDz();

	int GetHeight();
	int GetWidth();
};





class Model
{
private:
	vector<vector<double>>data;
	vector<vector<double>>
		A, B, C, D, alpha, betta,
		Az, Bz, Cz, Dz, alphaz, bettaz;

	double T0 = 0;
	int imax = 0;
	int jmax = 0;

	double dt = 0;
	double dr = 0;

	double Omega = 0;
	double Omegam = 0;
	double Omegaj = 0;
	double Omegan = 0;

	double kn;
	double km;
	double kj;

	//высота нагревател€
	int hn;
	//ширина нагревател€
	int wn;
	//толщина стенок
	int d;

	//высота жидкости
	int hj;
	//ширина жидкости
	int wj;

	double P0;

	double mu0 = 1;
	double nu0 = 0;

	double muN = 1;
	double nuN = 0;

	double psi = 0;
	double ka = 0.0278;
	double cva = 1000;

	double cvm = 0;
	double cvn = 0;
	double cvj = 0;
	//коэффициенты дл€ SetParams 
	double L0 = 1;
	double t0 = 1;
	double TT0 = 1;

	CRITICAL_SECTION cs;
	long long iteration = 0;
	ModelCells mc;

protected:
	void MakeStartVals();
	void ResizeAlphaBetta();
	void InitInstance();
	void CalcAlphaBetta();
	void CalcAlphaBettaz();
	void CalcHalfStepT();
	void CalcHalfStepTz();
public:
	Model();
	~Model();
	void main();
	bool Continue;
	void SetParams(int hn, int wn, int hj, int wj, int d, double kj, double kn, double km, double T, double P, double dt, double dr, double cvm, double cvn, double cvj);
	vector<vector<double>>GetData();
};

void print(vector<vector<double>>& data, char* filename);

/*
модель:
граница не входит в толщину
получаетс€ така€ штука

//граница металла и атмосферы/////////  - не входит в толщину металла
//металл//////////////////////////////
//металл//////////////////////////////
//граница металлa и воды//////////////  - не входит в толщину металла
~~вода~~~~~~~~~~~~~~~~~~~~~~~~~~~~////
~~вода~~~~~~~~~~~~~~~~~~~~~~~~~~~~////
~~вода~~~~~~~~~~~~~~~~~~~~~~~~~~~~////
~~вода~~~~~~~~~~~~~~~~~~~~~~~~~~~~////
~~вода~~~~~~~~~~~~~~~~~~~~~~~~~~~~////
||граница нагревател€ и воды||~~~~////
||нагреватель|||||||||||||||||~~~~////
||нагреватель|||||||||||||||||~~~~////
||нагреватель|||||||||||||||||~~~~////
//граница нагревател€ и металла///////  - не входит в толщину металла
//металл//////////////////////////////
//металл//////////////////////////////
//граница металла и атмосферы/////////  - не входит в толщину металла


итого всего отсчетов:
	по высоте:
		если hn < hj
		2 * d + hj + 5 
		
		то есть граничными отсчетами €вл€ютс€:
		0,
		d + 1,
		d + hn + 2,
		d + hj + 3,
		2 * d + hj + 4

		если hn = hj
		2 * d + hj + 4
		то есть граничными отсчетами €вл€ютс€:
		0,
		d + 1,
		d + hn + 2,
		2 * d + hn + 3

	по ширине:
		если wn < wj
		wj + d + 2

		то есть граничными отсчетами €вл€ютс€:
		0, 
		wn + 1,
		wj + 1
		wj + d + 1
		
		если wj = wn
		wj + d + 2

		то есть граничными отсчетами €вл€ютс€:
		0,
		wn + 1
		wn + d + 2
*/



