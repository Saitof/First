/////////////////////////////////////////////////////////////////////////////
// 曲面制御点生成部プロトタイプ宣言
// CreatCtrlPoint2.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputBasePoint1(int*);												// 断面生成位置入力1
BOOL InputBasePoint2(int*, int*, int*);									// 断面生成位置入力2
void ChangeLoop(int, PNTTYPE*, int*);									// 面ﾙｰﾌﾟ反転
void InitRevolve(int, int, PNTTYPE*, int*, int*, PNTTYPE*,int*);		// 回転初期処理
void CalcRevolvePoint(int, int, int, int, int, int, int, int, int,
                    double, PNTTYPE, PNTTYPE, PNTTYPE, PNTTYPE*, int*);	// 回転体制御点計算
void SetSRevolvePoint(int, int, int, int, int, double, PNTTYPE*,
                      int*, PNTTYPE);									// 回転体ｿﾘｯﾄﾞ制御点作成
BOOL GetRevolveInfo(int, int, double*, int*);							// 回転体情報取得
void InitPyramid(int, int, PNTTYPE*, PNTTYPE*);							// 錘初期処理
void Pyramid(int, int, int, PNTTYPE*, int*, PNTTYPE, int,
             PNTTYPE*, int*);											// 錘制御点生成
void CreatSpiralObject(int, int, int, int, double, double, int, int,
                       PNTTYPE, PNTTYPE, PNTTYPE*, int*);				// 螺旋曲面生成
void ReverseSurf(int, int, PNTTYPE*, int*);								// 曲面制御点反転
BOOL CheckSurface(int, int*, int*, int*, int*, int*, int*);				// 曲面連結ﾁｪｯｸ
void GetSEPoint(int, int, int, OBJTYPE**, int*, int*, int*,
                int*, PNTTYPE*, PNTTYPE*, SCRTYPE*, int*, int*);		// 始点終点制御点取得
BOOL ConctSEPoint(int, PNTTYPE*, PNTTYPE*, SCRTYPE*);					// 始点終点制御点連結
BOOL GetConctObj(int, SCRTYPE*, SCRTYPE*);								// 連結立体順取得
void ConctCurve(int, int, OBJTYPE**, SCRTYPE*);							// 曲線連結
void ConctSurface(int, int, int, OBJTYPE**, SCRTYPE*, SCRTYPE*);		// 通常曲面連結

//==========================<EOF:CreatCtrlPoint2.h>==========================