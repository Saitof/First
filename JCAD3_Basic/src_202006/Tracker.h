/////////////////////////////////////////////////////////////////////////////
// トラッカークラス(CTracker)宣言
// Tracker.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
class CTracker
{
// コンストラクション
public:
	CTracker(void);

// オペレーション
public:
	void Setup(PNTTYPE* pa, int* ia, int* op, int pn, int mod, int dsp);
	void Reset(int flg);
	void Limit(double x, double y, double z, int x1, int x2, int y1, int y2, int z1, int z2);
	void Draw(int plane);
	BOOL EnableTrack(void) { return SetFlg; }
	int  HitTracker(int flg, int plane, const PNTTYPE& pnt, PNTTYPE* pa, int pn);
	int  HitTrackerS(int flg, int plane, const PNTTYPE& pnt, PNTTYPE* pa, int pn, int *ha);
	int  HitTracker3D(int flg, const CPoint& pnt, PNTTYPE* pa, int pn);

// インプリメンテーション
public:
	virtual ~CTracker(void) { Reset(1); }

private:
	BOOL   SetFlg;
	UINT   Lsno;
	int    Mode, Disp;
	float  PntSize;
	double XLimit1, XLimit2, YLimit1, YLimit2, ZLimit1, ZLimit2;
};

//==============================<EOF:Tracker.h>==============================