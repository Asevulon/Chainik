#pragma once
#include<vector>
using namespace std;

enum CellType
{
	metall,
	heater,
	liquid,
	none
};
struct ModelCellsParams
{
	int hj = 0;
	int wj = 0;
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

struct DrawerParamsInfo
{
	CellType type = none;
	bool borderz = false;
	bool border = false;
};
class ModelCells
{
private:
	double ka = 0.0278;

	
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

	bool created = false;
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
	ModelCellsParams GetParams();
	void CalcD();
	void CalcDz();

	int GetHeight();
	int GetWidth();

	vector<vector<DrawerParamsInfo>> GetDrawerData();
	void SetCellMaterial(int i, int j, CellType ct);
	void SetCellType(int i, int j, bool border, bool borderz);
	void SetParams(ModelCellsParams&p);
	void InitInstance();
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

	//высота нагревателя
	int hn;
	//ширина нагревателя
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
	//коэффициенты для SetParams 
	double L0 = 1;
	double t0 = 1;
	double TT0 = 1;

	long long iteration = 0;
	ModelCells mc;

protected:
	void ResizeAlphaBetta();
	void InitInstance();
	void CalcAlphaBetta();
	void CalcAlphaBettaz();
	void CalcHalfStepT();
	void CalcHalfStepTz();
public:
	CRITICAL_SECTION cs;

	Model();
	~Model();
	void main();
	bool Continue;
	vector<vector<double>>GetData();
	void SetModelCells(ModelCells& mcs);
};

void print(vector<vector<double>>& data, char* filename);

