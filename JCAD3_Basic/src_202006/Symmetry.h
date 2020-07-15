/////////////////////////////////////////////////////////////////////////////
// 対称軸編集操作部プロトタイプ宣言
// Symmetry.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputRevolveDivi(int*);											// 回転対称分割数取得
void SetSymmetryLine(int, int, PNTTYPE, int, int, int, int*, int*);		// 対称軸設定
BOOL CheckSymmetryArea(int*, PNTTYPE, int, PNTTYPE, int, int, int);		// 対称領域ﾁｪｯｸ
int  CopySymmetryArea(int, PNTTYPE, int, int, int, int*, int*,
                      PNTTYPE*, int*, int*, int*);						// 対称領域複写
void PickSymmetryArea(int, int, int, int, int, int, int*, int*);		// 対称領域ﾄﾗｯｶｰ選択
void MoveSymmetryArea(PNTTYPE, PNTTYPE, int, PNTTYPE, int, int, int, int,
                      int, int, int, int, int*, PNTTYPE*, PNTTYPE*);	// 対称領域移動量設定

//==============================<EOF:Symmetry.h>=============================