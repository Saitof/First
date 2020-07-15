/////////////////////////////////////////////////////////////////////////////
// 曲面編集操作部プロトタイプ宣言
// EditSurface.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void GetSurfCmd1(CMDTYPE*, int*, int*, int*, int*, int*, double*,
                 int*, PNTTYPE*, int*);									// 曲面ｺﾏﾝﾄﾞ取得1
void GetSurfCmd2(CMDTYPE*, int, int*, int*, int*, int*, int*, int*,
                 double*, int*, PNTTYPE*, int*, int*, PNTTYPE*, int*);	// 曲面ｺﾏﾝﾄﾞ取得2
void ExecSurface(int, OBJTYPE*, int, int, int, int, int, double,
                 int, PNTTYPE*, int*, PNTTYPE*, OBJTYPE**);				// 通常曲面実行
void ExecSweepSurf(OBJTYPE*, int, int, int, int, int, double, int,
                   int, PNTTYPE*, int*, PNTTYPE*, PNTTYPE*, OBJTYPE**);	// 掃引曲面実行
void AttachObj(OBJTYPE*);												// ｺﾏﾝﾄﾞ再接続/再実行
void PickPointMove(int, int, int*, int, int, int, int, double,
                   int, int, int, int, OBJTYPE*, PNTTYPE*,
                   PNTTYPE*, int*, int*, int*);							// 制御点移動 点選択
void PickLineMove(int, int, int*, int, int, int, int, int,
                  double, int, int, int, int, int, int*,
                  OBJTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*,
                  PNTTYPE*, int*, int*, int*, int*, int*);				// 制御点列移動 列選択
BOOL CheckLineCopy(int, int, int*, int, int, int, int, int, int*, int*);// 制御点列複写ﾁｪｯｸ
void PickLineInsert(int, int, int*, int*, int*, int, int, int*,
                    int, int, int, int*, int, OBJTYPE*, PNTTYPE,
                    PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, int*);			// 制御点列挿入 列選択
BOOL PickLineDivide(int, int, int*, int, int, int, int, int, double,
                    int, OBJTYPE*, PNTTYPE*, PNTTYPE*, int*, int*);		// 制御点列分割
void PickSurfLine(int, int*, int, int, OBJTYPE*, int*, int*, int*);		// 制御点列選択
void PickSPointMove1(int, int, double*, double*, int, int, int, int,
                     int, int, double, int, int, int*, PNTTYPE*,
                     PNTTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*, VECTYPE*,
                     PNTTYPE*, int*, int*, int*, int*, int*, int*,
                     int*, OBJTYPE*, OBJTYPE*, OBJTYPE**, OBJTYPE**);	// 断面制御点移動 列選択
void PickSPointMove2(int, int*, int, double*, double*, int, int, double,
                     int, PNTTYPE*, PNTTYPE*, int*, int*, int*);		// 断面制御点移動 点選択
void ChangeSPointMove(int, int, int, int, int, int, int, int, int,
                      double, int, int, int, PNTTYPE, VECTYPE, PNTTYPE,
                      PNTTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*,
                      PNTTYPE*, int*, int*, int*, int*, CMDTYPE*,
                      OBJTYPE*, OBJTYPE*, OBJTYPE*, OBJTYPE*);			// 断面制御点移動 断面変更
void PickBPointMove(int, int*, int, int, OBJTYPE*, int*, int*);			// ﾍﾞｰｽ線移動 列選択
BOOL InitCScale(int*, int*);											// 拡大/縮小初期処理
void EraseObjSPointMove(int*, int*, OBJTYPE*);							// 断面制御点移動 立体非表示
void DispObjSPointMove(int, int*, OBJTYPE*);							// 断面制御点移動 立体表示
void AdjustCPoint(int, int, int, PNTTYPE*, int, int, int*, PNTTYPE*,
                  PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, int*, int*);		// 制御点曲線合わせ
BOOL GetSurfaceCurve(int, int, int, int, int, int, int, int,
                     OBJTYPE*, int*, int*, PNTTYPE*);					// 曲面曲線取得

//============================<EOF:EditSurface.h>============================