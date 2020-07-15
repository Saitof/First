/////////////////////////////////////////////////////////////////////////////
// 曲面編集トラッカー操作部プロトタイプ宣言
// EditCtrlTracker.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void GetMoveValue(int, PNTTYPE, PNTTYPE*);								// ﾄﾗｯｶｰ移動量取得
void SetParsTracker(int, int*, int, int*, PNTTYPE*, int*, int,
                    PNTTYPE*, int*, int*);								// 透視図ﾄﾗｯｶｰ設定
void DispParsTracker(int, int, PNTTYPE*, int*, int, PNTTYPE*,
                     int*, int);										// 透視図ﾄﾗｯｶｰ表示
void HitCtrlTracker1(UINT, const CPoint&, int, int, PNTTYPE, PNTTYPE*,
                     int, PNTTYPE*, int*, int, int*, int*, int*, int*);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
void HitCtrlTracker2(UINT, const CPoint&, int, int, PNTTYPE, PNTTYPE*,
                     int, int, int*, int*, int*);						// 全ﾄﾗｯｶｰﾁｪｯｸ2
void DragCtrlTracker1(int, BOXTYPE, PNTTYPE*, int, PNTTYPE*, int*,
                      int, int*, int*, int*);							// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
void DragCtrlTracker2(int, BOXTYPE, PNTTYPE*, int, int*, int*);			// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
void SetLineNo1(int, int, int, int, int*);								// ﾗｲﾝ番号設定1
void SetLineNo2(int, int, int, int, int*);								// ﾗｲﾝ番号設定2
void SetLineNo3(int, int, int*, OBJTYPE*);								// ﾗｲﾝ番号設定3
void PickCPoint(PNTTYPE, int, int, PNTTYPE*, int, int, int, int, int,
                int, int, int*, int*);									// 対象制御点選択
void CheckCLine(int, int, int, int, PNTTYPE*, int*, int*);				// 制御点列確認
void TrackerCLine(int, int, int, int, PNTTYPE*, int, int, int*,
                  PNTTYPE*, int*);										// 制御点列ﾄﾗｯｶｰ取得
void PickCLine(int, int, int, int, int, int, int, int, PNTTYPE*,
               int*, int*);												// 対象制御点列選択
void TrackerCInsert(int, int, int, int, int, int, PNTTYPE*, int*,
                    PNTTYPE*, int*);									// 制御点列ﾄﾗｯｶｰ取得
void TrackerULine(int, int, int, PNTTYPE*, int, int, int*, PNTTYPE*,
                  int*);												// 制御点列ﾄﾗｯｶｰ取得
void TrackerVLine(int, int, PNTTYPE*, int, int, int, int*, PNTTYPE*,
                  int*);												// 制御点列ﾄﾗｯｶｰ取得
void SelAreaCtrl(int, BOXTYPE, int, PNTTYPE*, int*, int);				// 制御点選択(ｴﾘｱ)
void GetBasePoint(int, int, PNTTYPE*, int, PNTTYPE*, VECTYPE*);			// 基準点取得

//==========================<EOF:EditCtrlTracker.h>==========================