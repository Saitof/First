/////////////////////////////////////////////////////////////////////////////
// 曲面編集制御点操作部プロトタイプ宣言
// EditCtrlPoint2.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/26 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void DeleteCLine(int, int*, int, int, int, int, int*, int*, PNTTYPE*,
                 int*, int*);											// 対象制御点列削除
void InsertCLine(int, int, int, int*, int*, int, int*, int, PNTTYPE,
                 PNTTYPE*, int*);										// 対象制御点列挿入
void PutSPoint(int, int, int, int, int, PNTTYPE, PNTTYPE*, int*,
               PNTTYPE, VECTYPE, PNTTYPE*, PNTTYPE*, int*);				// 断面制御点情報更新
void SetSweepCtrl(int, int, int, int, PNTTYPE*, int*, PNTTYPE*, int*);	// 掃引曲面制御点変換
void DeleteSweep(int, int*, int, int, int*, int*, PNTTYPE*, int*);		// 掃引制御点列削除
void MoveSweep(PNTTYPE*, int, int, int, PNTTYPE*, int, int*, PNTTYPE*);	// 掃引制御点移動
void InsertSweep(int, int, int, int*, int*, PNTTYPE*, int*);			// 掃引制御点列挿入
void CtrlPointSpace(PNTTYPE*, PNTTYPE*, int*, int, PNTTYPE, PNTTYPE,
                    int, int, double, double, double, double, double,
                    double, double, double, double, double);			// 制御点ｽﾍﾟｰｽ(CPS)

//===========================<EOF:EditCtrlPoint2.h>==========================
