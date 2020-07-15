/////////////////////////////////////////////////////////////////////////////
// 曲面編集制御点操作部プロトタイプ宣言
// EditCtrlPoint1.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void MoveCPoint(int, PNTTYPE*, OBJTYPE*, int, int, int, int, double,
                int*, int*, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);		// 制御点移動
void TwistCLine(PNTTYPE*, VECTYPE*, double, int, int*, int, OBJTYPE*,
               int, int, int, int, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);	// 制御点ひねり
void ScaleCLine(PNTTYPE*, VECTYPE*, int, int, double, int, int*, int,
     OBJTYPE*, int, int, int, int, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);	// 制御点拡大/縮小
void MoveSCurve(int, double*, PNTTYPE*, OBJTYPE*, int, int, PNTTYPE*,
                PNTTYPE*, int*, PNTTYPE*, int*);						// 断面制御点移動
void MoveSPoint(OBJTYPE*, int, int, int, int, PNTTYPE*, PNTTYPE*,
                PNTTYPE*, int*);										// 断面制御点移動
void MoveBPoint(int, PNTTYPE*, PNTTYPE*, OBJTYPE*, int, int,
                int, int, double, int, int, int*, int*,
                PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);					// ﾍﾞｰｽ線移動

//===========================<EOF:EditCtrlPoint1.h>==========================