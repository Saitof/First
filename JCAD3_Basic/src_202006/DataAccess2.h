/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部プロトタイプ宣言
// DataAccess2.h
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS2_H
#define ___DATAACCESS2_H

#include "DataType.h"

void CreatOif(int, OBJTYPE*, char*, OIFTYPE*, OIFTYPE**);				// 立体構成情報領域確保
void FreeOif(int, OIFTYPE*);											// 立体構成情報領域解放
void SetOifObj(OIFTYPE*, OBJTYPE*);										// 構成情報立体ﾎﾟｲﾝﾀ設定
void GetOifObj(OIFTYPE*, OBJTYPE**);									// 構成情報立体ﾎﾟｲﾝﾀ取得
void GetObjOif(OBJTYPE*, OIFTYPE**);									// 立体構成情報ﾎﾟｲﾝﾀ取得
void BaseOifPtr(int, int, OIFTYPE**);									// 立体情報ﾍﾞｰｽﾎﾟｲﾝﾀ取得
void NextOifPtr(int, int, OIFTYPE**);									// 次立体情報ﾎﾟｲﾝﾀ移動
void SetOifName(int, OIFTYPE*, char*);									// 立体構成名称設定
void GetOifName(OIFTYPE*, char*);										// 立体構成名称取得
void SetOifLevel(OIFTYPE*, int);										// 立体構成ﾚﾍﾞﾙ設定
int  GetOifLevel(OIFTYPE*);												// 立体構成ﾚﾍﾞﾙ取得
void InitOifNo(void);													// 立体構成立体No初期化
void SetOifNo(OIFTYPE*, int);											// 立体構成立体No設定
int  GetOifNo(OIFTYPE*);												// 立体構成立体No取得
void SortOifData(void);													// 立体構成ﾃﾞｰﾀｿｰﾄ
void GetDataBasePtr(OBJTYPE**, PNTTYPE**, int**);						// ﾃﾞｰﾀﾍﾞｰｽ先頭ﾎﾟｲﾝﾀ取得
void OnResetObj(OBJTYPE*);												// ﾘｾｯﾄ立体登録
void OffResetObj(void);													// ﾘｾｯﾄ立体解除
BOOL GetResetObjMode(void);												// ﾘｾｯﾄ立体ﾓｰﾄﾞ取得
void CreatObj(int, int, int, OBJTYPE**);								// 立体ﾃﾞｰﾀ領域確保
void ChangeObj(OBJTYPE*, int, int, int);								// 立体ﾃﾞｰﾀ領域変更
void TempObj(OBJTYPE*);													// 立体ﾃﾞｰﾀ一時削除
void FreeObj(OBJTYPE*, int);											// 立体ﾃﾞｰﾀ領域解放
void UndoObj(OBJTYPE*);													// 立体復元
void LevelObj(int, OBJTYPE*);											// 同一ﾚﾍﾞﾙ立体接続
void BaseObjPtr(int, OBJTYPE**, int*, int*);							// 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
BOOL CheckObjPtr(int, OBJTYPE*);										// 指定立体存在ﾁｪｯｸ
void NextObjPtr(int, OBJTYPE**, int*, int*);							// 次立体ﾎﾟｲﾝﾀ移動
void GroupBaseObjPtr(int, OBJTYPE**);									// 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得(group)
void GroupNextObjPtr(int, OBJTYPE**);									// 次立体ﾎﾟｲﾝﾀ移動(group)
void LevelBaseObjPtr(int, OBJTYPE**);									// 同一ﾚﾍﾞﾙ先頭立体取得
void LevelNextObjPtr(int, OBJTYPE**);									// 同一ﾚﾍﾞﾙ次立体移動
void SelBaseObjPtr(int, OBJTYPE**);										// 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
void SelNextObjPtr(int, OBJTYPE**);										// 次立体ﾎﾟｲﾝﾀ移動
void ColorBaseObjPtr(int, OBJTYPE**);									// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
void ColorNextObjPtr(int, OBJTYPE**);									// ｶﾗｰ別次立体ﾎﾟｲﾝﾀ移動
void SetObjPtr(OBJTYPE*);												// 処理対象立体設定
void GetObjPtr(OBJTYPE**);												// 処理対象立体取得
void OnDspFlg(int, OBJTYPE*);											// 表示(有効)ﾌﾗｸﾞON
void OffDspFlg(int, OBJTYPE*);											// 表示(有効)ﾌﾗｸﾞOFF
void OnEdtFlg(int, OBJTYPE*);											// 編集対象ﾌﾗｸﾞON
void OffEdtFlg(int, OBJTYPE*);											// 編集対象ﾌﾗｸﾞOFF
void SetObjNo(OBJTYPE*, int);											// 立体No設定
int  GetObjNo(OBJTYPE*);												// 立体No取得
void ChangeTestFlg(int);												// ﾃｽﾄ用ﾌﾗｸﾞ変更
void PutObjAtr(OBJTYPE*, int);											// 属性No設定
int  GetObjAtr(OBJTYPE*);												// 属性No取得
void PutObjPrt(OBJTYPE*, int, int);										// ﾊﾟｰﾂNo設定
int  GetObjPrt(OBJTYPE*);												// ﾊﾟｰﾂNo取得
void PutObjCol(OBJTYPE*, int);											// ｶﾗｰNo設定
int  GetObjCol(OBJTYPE*);												// ｶﾗｰNo取得
void PutListNo(OBJTYPE*, unsigned int);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄNo設定
unsigned int GetListNo(OBJTYPE*);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄNo取得
void PutListMode(OBJTYPE*, int);										// ﾘｽﾄﾓｰﾄﾞ設定
int  GetListMode(OBJTYPE*);												// ﾘｽﾄﾓｰﾄﾞ取得
void PutGroup(OBJTYPE*, int*);											// ｸﾞﾙｰﾌﾟ情報設定
void GetGroup(OBJTYPE*, int*);											// ｸﾞﾙｰﾌﾟ情報取得
void SetGroupNo(OBJTYPE*, int, int);									// ｸﾞﾙｰﾌﾟ番号設定
void SetSelNo(OBJTYPE*, int);											// 選択No設定
int  GetSelNo(OBJTYPE*);												// 選択No取得
int  GetSelectNo(void);													// 立体選択No取得
void ResetSelNo(void);													// 選択Noﾘｾｯﾄ
int  GetVtxNum(OBJTYPE*);												// 立体頂点数取得
void PutVtx(OBJTYPE*, int, PNTTYPE);									// 指定頂点設定
void GetVtx(OBJTYPE*, int, PNTTYPE*);									// 指定頂点取得
int  GetFlpNum1(OBJTYPE*);												// 立体面ﾙｰﾌﾟ数取得1
void PutFlp1(OBJTYPE*, int, int, int);									// 指定面ﾙｰﾌﾟ番号設定1
void GetFlp1(OBJTYPE*, int, int*, int*);								// 指定面ﾙｰﾌﾟ番号取得1
void GetFlpVtx1(OBJTYPE*, int, PNTTYPE*, int*);							// 指定面ﾙｰﾌﾟ座標取得1
int  GetFlpNum2(OBJTYPE*);												// 立体面ﾙｰﾌﾟ数取得2
void PutFlp2(OBJTYPE*, int, int, int);									// 指定面ﾙｰﾌﾟ番号設定2
void GetFlp2(OBJTYPE*, int, int*, int*);								// 指定面ﾙｰﾌﾟ番号取得2
void GetFlpVtx2(OBJTYPE*, int, PNTTYPE*, int*);							// 指定面ﾙｰﾌﾟ座標取得2
void SetObjBox(OBJTYPE*);												// 立体ﾎﾞｯｸｽ設定
void GetObjBox(OBJTYPE*, BOXTYPE*);										// 立体ﾎﾞｯｸｽ取得
void SetScrBox(void);													// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
void GetScrBox(BOXTYPE*);												// ｽｸﾘｰﾝﾎﾞｯｸｽ取得
void GetAreaBox(int, BOXTYPE*);											// ｴﾘｱﾎﾞｯｸｽ取得
void GetUnitBox(OBJTYPE*, BOXTYPE*);									// ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
void ChangeObjVtx(int, OBJTYPE*, double, double, double);				// 立体頂点変更
void InitCVtx(void);													// 頂点ｿｰﾄ初期処理
void SetCVtx(PNTTYPE);													// ｿｰﾄ頂点設定
void SetCFace(void);													// 面ﾙｰﾌﾟ終了設定
int  SortCVtx(void);													// 頂点情報ｿｰﾄ&ｶｳﾝﾄ
void SetCVtxObj(OBJTYPE*);												// 頂点情報設定

#endif ___DATAACCESS2_H

//============================<EOF:DataAccess2.h>============================