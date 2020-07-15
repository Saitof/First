/////////////////////////////////////////////////////////////////////////////
// 立体選択部プロトタイプ宣言
// Select.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void OnDspAllObject(void);												// 立体表示(全立体)
void OffDspAllObject(void);												// 立体非表示(全立体)
void OnDspGroupObject(int);												// 立体表示(ｸﾞﾙｰﾌﾟ)
void OffDspGroupObject(int);											// 立体非表示(ｸﾞﾙｰﾌﾟ)
void EdtPointObject(int, PNTTYPE, int);									// 編集立体選択(ﾎﾟｲﾝﾄ)
void EdtAreaObject(int, BOXTYPE, int);									// 編集立体選択(ｴﾘｱ)
void OnEdtGroupObject(int);												// 編集立体選択(ｸﾞﾙｰﾌﾟ)
void OnEdtAllObject(void);												// 編集立体全選択(表示立体)
void OffEdtAllObject(void);												// 編集立体全解除
void OffEdtOtherObject(OBJTYPE*);										// 指定外編集対象解除

//==============================<EOF:Select.h>===============================
