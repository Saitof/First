/////////////////////////////////////////////////////////////////////////////
// ライン描画クラス(CLine)宣言
// Line.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "DataType.h"

class CLine
{
// コンストラクション
public:
	CLine(void);

// オペレーション
public:
	void Setup(PNTTYPE* ps, PNTTYPE* pe, int* cl, int pn, int md);
	void Draw(int plane);
	void Reset(void);
	BOOL EnableDraw(void) { return SetFlg; }

// インプリメンテーション
public:
	virtual ~CLine(void) { Reset(); }

private:
	BOOL SetFlg;
	UINT Lsno;
	int  Mode;
};

//===============================<EOF:Line.h>================================
