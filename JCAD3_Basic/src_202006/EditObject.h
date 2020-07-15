/////////////////////////////////////////////////////////////////////////////
// 立体編集操作部プロトタイプ宣言
// EditObject.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/08 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputCopyNum(int*, PNTTYPE*, int*, SCRTYPE*);						// 複写情報入力
void TrackerScale1(BOXTYPE, PNTTYPE, PNTTYPE*);							// ﾘｻｲｽﾞ用ﾄﾗｯｶｰ取得1
void TrackerScale2(int, BOXTYPE, PNTTYPE, PNTTYPE*, int*, int*);		// ﾘｻｲｽﾞ用ﾄﾗｯｶｰ取得2
void TrackerBend(BOXTYPE, PNTTYPE, PNTTYPE*, int*);						// 円筒曲げ用ﾄﾗｯｶｰ取得
void TrackerBox3D(BOXTYPE, PNTTYPE*);									// ﾎﾞｯｸｽ3D用ﾄﾗｯｶｰ取得
void TrackerBox2D(int, BOXTYPE, PNTTYPE*, int*);						// ﾎﾞｯｸｽ2D用ﾄﾗｯｶｰ取得
void TrackerBoxCrs(BOXTYPE, PNTTYPE, PNTTYPE*);							// ﾎﾞｯｸｽ十字用ﾄﾗｯｶｰ取得
void TrackerBoxCrs2(BOXTYPE, PNTTYPE, PNTTYPE*);						// ﾎﾞｯｸｽ十字用ﾄﾗｯｶｰ取得2
BOOL GetObjCrossPnt(int, PNTTYPE, PNTTYPE*, VECTYPE*, double*);			// 立体交点取得

//=============================<EOF:EditObject.h>============================
