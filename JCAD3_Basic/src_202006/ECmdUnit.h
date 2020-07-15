/////////////////////////////////////////////////////////////////////////////
// ユニット立体編集コマンド部プロトタイプ宣言
// ECmdUnit.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void InitArrange(int, int, double, PNTTYPE, int*, int*, int*,
                 PNTTYPE*, VECTYPE*, VECTYPE*);							// 石枠配置初期処理
void EndArrange(int);													// 石枠配置終了処理
void ArrangeMove(double, int, int, int*, int*, PNTTYPE*, PNTTYPE*,
                 VECTYPE*);												// 石枠移動処理
void ArrangeRotate1(double, int, int, int*, int*, PNTTYPE*);			// 石枠回転処理1
void ArrangeRotate2(double, int, int, int*, int*, PNTTYPE*, VECTYPE*);	// 石枠回転処理2
void InitBasePos(int, int, int, int, PNTTYPE*, int*, int*, int*,
                 PNTTYPE*);												// 基準移動初期処理
void EndBasePos(void);													// 基準移動終了処理
void EditBasePos(int, int, int, int, PNTTYPE*, int, PNTTYPE*,
                 int, int*, int*, PNTTYPE*);							// 基準移動編集処理
void ObjectLinesEdit(int, int, int, int, PNTTYPE, PNTTYPE);				// 曲線指定編集処理

//==============================<EOF:ECmdUnit.h>=============================