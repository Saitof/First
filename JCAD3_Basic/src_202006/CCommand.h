/////////////////////////////////////////////////////////////////////////////
// 立体生成コマンド部プロトタイプ宣言
// CCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
int  MakeSCurve(int*, int, int, double, PNTTYPE*, int*);				// S曲線生成
void CrtCurve(int, int, int, double, PNTTYPE*, int*);					// 曲線生成
void RedoCrtCurve(int, int, PNTTYPE*, int*);							// 曲線生成(再実行)
void ChgCurve(OBJTYPE*, int, int, int, double, PNTTYPE*, int*);			// 曲線変更
void RedoChgCurve(int, int, PNTTYPE*, int*);							// 曲線変更(再実行)
void SetCurve(int, int, PNTTYPE*);										// 曲線制御点生成
void RedoSetCurve(int, int, PNTTYPE*);									// 曲線制御点生成(再実行)
void CrtSurface(int, int, int, int, int, double, int, PNTTYPE*, int*);	// 曲面生成
void RedoCrtSurface(int, int, int, int, PNTTYPE*, int*);				// 曲面生成(再実行)
void RedoCrtSurface2(int, int, int, int, int, PNTTYPE*, int*);			// 曲面生成2(再実行)
void ChgSurface(OBJTYPE*, int, int, int, int, int, double, PNTTYPE*,
                int*);													// 曲面変更
void RedoChgSurface(int, int, int, int, PNTTYPE*, int*);				// 曲面変更(再実行)
void SetSurface(int, int, int, int, PNTTYPE*);							// 曲面制御点生成
void RedoSetSurface(int, int, int, int, PNTTYPE*);						// 曲面制御点生成(再実行)
void Thickness(OBJTYPE*, double);										// 曲面厚みづけ
void MakeDirectCommand(OBJTYPE*);										// 直接生成ｺﾏﾝﾄﾞ作成
void RedoDirect(int, PNTTYPE*);											// 直接生成(再実行)
void CrtSweepSurf(int, int, int, int, int, double, int, int, PNTTYPE*,
                  int*);												// 掃引曲面生成
void RedoCrtSweepSurf(int, int, int, int, PNTTYPE*, int*);				// 掃引曲面生成(再実行)
void ChgSweepSurf(OBJTYPE*, int, int, int, int, int, double, PNTTYPE*,
                  int*);												// 掃引曲面変更
void RedoChgSweepSurf(int, int, int, int, PNTTYPE*, int*);				// 掃引曲面変更(再実行)

//==============================<EOF:CCommand.h>=============================