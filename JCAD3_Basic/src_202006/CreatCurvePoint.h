/////////////////////////////////////////////////////////////////////////////
// 曲線編集操作部プロトタイプ宣言
// CreatCurvePoint.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void InsertCurvePoint(int*, int*, PNTTYPE*, int*);						// S曲線中間制御点挿入
void CalcRPoint(int, int, int, double, int*, PNTTYPE*, int*);			// R頂点位置算出
void CalcSpiralPoint(PNTTYPE, int, double, double, int, int,
                     PNTTYPE*, int*);									// 螺旋制御点計算
void TrackerECurve(int, double*, int, int, PNTTYPE*, int*, int*,
                   int*, PNTTYPE*, int*);								// 曲線編集ﾄﾗｯｶｰ取得
void DeleteCurvePoint(int, double*, double*, int, double, int*, int,
                      PNTTYPE*, int*, int*);							// 制御点･通過点削除
void PickECurve(int, int*, int, double*, double*, int*, int, int,
                double, int*, OBJTYPE*, int, PNTTYPE*,
                PNTTYPE*, int*, int*, int*);							// 曲線編集 点選択
void MoveECurve(int, double*, PNTTYPE*, OBJTYPE*, int, int,
                PNTTYPE*, PNTTYPE*, int*, PNTTYPE*, int*);				// 制御点･通過点移動
BOOL ExtractionCurve(int, int, int, int, int, int, int,
                     PNTTYPE*, int*, int*, int*, PNTTYPE*, int*);		// 曲線抽出

//==========================<EOF:CreatCurvePoint.h>==========================