/////////////////////////////////////////////////////////////////////////////
// コマンド関連部プロトタイプ宣言
// RCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
OBJTYPE* CopyObject(void);												// 立体&ｺﾏﾝﾄﾞ複写
void Copy(void);														// 立体複写
void Delete(void);														// 立体削除
void InitPasteObject(int*);												// 立体ﾍﾟｰｽﾄ初期処理
void EndPasteObject(int);												// 立体ﾍﾟｰｽﾄ終了処理
void EditPasteObject1(int, PNTTYPE, double, PNTTYPE, PNTTYPE);			// 立体ﾍﾟｰｽﾄ配置(移動･回転)
void EditPasteObject2(int, double);										// 立体ﾍﾟｰｽZ軸回転
void PCutObject(PNTTYPE, PNTTYPE, int);									// 立体平面分割
void BooleanObject(int, int);											// 立体図形演算

//=============================<EOF:RCommand.h>==============================
