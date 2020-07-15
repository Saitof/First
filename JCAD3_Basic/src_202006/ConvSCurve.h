/////////////////////////////////////////////////////////////////////////////
// Ｓ曲線 制御点<->通過点 変換部プロトタイプ宣言
// ConvSCurve.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void SetCurveMatrix1(int, double, int, int, int*, double*, double*);	// S曲面変換ﾏﾄﾘｯｸｽ設定1
void SetCurveMatrix2(int, double, int, int, int*, double*, double*);	// S曲面変換ﾏﾄﾘｯｸｽ設定2
void SetCurveMatrix3(int, double, int, int, int*, double*, double*);	// S曲面変換ﾏﾄﾘｯｸｽ設定3
void CalcCurvePoint(int, int, PNTTYPE*, PNTTYPE*, double*);				// 制御点<=>通過点計算
void CalcSurfacePoint(int, int, int, int, int, int, int, double, int*,
                      PNTTYPE*, PNTTYPE*);								// 制御点<=>通過点計算

//=============================<EOF:ConvSCurve.h>============================
