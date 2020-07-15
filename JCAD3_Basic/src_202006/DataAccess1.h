/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部プロトタイプ宣言
// DataAccess1.h
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS1_H
#define ___DATAACCESS1_H

#include "DataType.h"

void ReadMemDataFile(void);												// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ読込
void WriteMemDataFile(void);											// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ書込
int  GetIntMax(void);													// 整数ﾊﾟﾗﾒｰﾀ配列数取得
void SetIntMax(int);													// 整数ﾊﾟﾗﾒｰﾀ配列数設定
int  GetPntMax(void);													// 座標ﾊﾟﾗﾒｰﾀ配列数取得
void SetPntMax(int);													// 座標ﾊﾟﾗﾒｰﾀ配列数設定
int  GetUndoMax(void);													// Undo/Redoﾊﾟﾗﾒｰﾀ配列数取得
void SetUndoMax(int);													// Undo/Redoﾊﾟﾗﾒｰﾀ配列数設定
int  GetCCtlMax(void);													// 最大曲線制御点数取得
void SetCCtlMax(int);													// 最大曲線制御点数設定
int  GetCCtlAMax(void);													// 曲線制御点配列数取得
int  GetSCtlMax(void);													// 最大曲面制御点数取得
void SetSCtlMax(int);													// 最大曲面制御点数設定
int  GetSCtlAMax(void);													// 曲面制御点配列数取得
int  GetWCtlMax(void);													// 作業用制御点配列数取得
void SetWCtlMax(int);													// 作業用制御点配列数設定
int  GetCPntMax(void);													// 最大曲線頂点数取得
void SetCPntMax(int);													// 最大曲線頂点数設定
int  GetSPntMax(void);													// 最大曲面頂点数取得
void SetSPntMax(int);													// 最大曲面頂点数設定
int  GetOPntMax(void);													// 最大立体頂点数取得
void SetOPntMax(int);													// 最大立体頂点数設定
int  GetBPntMax(void);													// 最大断面頂点数取得
void SetBPntMax(int);													// 最大断面頂点数設定
void InitVar(void);														// 変数初期化(ｼｽﾃﾑ起動時)
void ResetVar(void);													// 変数初期化(ｼｽﾃﾑ稼働中)
void FreeMem(void);														// ﾒﾓﾘ解放(ｼｽﾃﾑ終了時)
void ChangeAreaAdr(int, int, int, int);									// 配列ｱﾄﾞﾚｽ変更
void SetDemoFlg(int);													// 体験版ﾌﾗｸﾞ設定
int  GetDemoFlg(void);													// 体験版ﾌﾗｸﾞ取得
void SetInitFlg(int);													// 初期起動状態ﾌﾗｸﾞ設定
int  GetInitFlg(void);													// 初期起動状態ﾌﾗｸﾞ取得
void SetFileFlg(int);													// ﾌｧｲﾙ保存ﾌﾗｸﾞ設定
int  GetFileFlg(void);													// ﾌｧｲﾙ保存ﾌﾗｸﾞ取得
void SetFileName(CString);												// 保存ﾌｧｲﾙ名設定
CString GetFileName(void);												// 保存ﾌｧｲﾙ名取得
void SetSplashMsg(UINT);												// ｽﾌﾟﾗｯｼｭ表示中ﾒｯｾｰｼﾞ設定
void OutSplashMsg(void);												// ｽﾌﾟﾗｯｼｭ表示中ﾒｯｾｰｼﾞ出力
void StartBackUp(int);													// ﾊﾞｯｸｱｯﾌﾟ開始処理
void StartMenu(void);													// ﾒﾆｭｰ開始処理
void EndJMenu1(void);													// ﾒﾆｭｰ終了処理1
void EndJMenu2(void);													// ﾒﾆｭｰ終了処理2
void FileBackUp1(void);													// ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理1
void FileBackUp2(void);													// ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理2
void UndoMenu(int);														// Undoﾒﾆｭｰ処理
void RedoMenu(int);														// Redoﾒﾆｭｰ処理
BOOL RestoreBackUp(void);												// ﾒﾆｭｰ復旧処理
void MemErr(unsigned int);												// ﾒﾓﾘ異常ﾒｯｾｰｼﾞ
double GetIntUse(void);													// 整数ﾊﾟﾗﾒｰﾀ使用率取得
double GetPntUse(void);													// 頂点ﾊﾟﾗﾒｰﾀ使用率取得
int  GetIntUseNum(void);												// 整数ﾊﾟﾗﾒｰﾀ使用数取得
int  GetPntUseNum(void);												// 頂点ﾊﾟﾗﾒｰﾀ使用数取得
void CreatCmd(int, int, int, CMDTYPE**);								// ｺﾏﾝﾄﾞ領域確保
void ChangeCmd(CMDTYPE*, int, int);										// ｺﾏﾝﾄﾞ領域変更
void TempCmd(OBJTYPE*, CMDTYPE*);										// ｺﾏﾝﾄﾞ一時削除
void FreeCmd(OBJTYPE*, CMDTYPE*, int);									// ｺﾏﾝﾄﾞ領域解放
void AppendCmd(OBJTYPE*, CMDTYPE*);										// ｺﾏﾝﾄﾞ&立体接続
void UndoCmd(OBJTYPE*, CMDTYPE*);										// ｺﾏﾝﾄﾞ&立体再接続
void LevelCmd(CMDTYPE*);												// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
void DetachCmd(OBJTYPE*, CMDTYPE*);										// ｺﾏﾝﾄﾞ一時切離し
void AttachCmd(OBJTYPE*);												// ｺﾏﾝﾄﾞ再接続
void InitCmdLvl(CMDTYPE*);												// ｺﾏﾝﾄﾞﾚﾍﾞﾙ初期化
void SetCmdLvl(CMDTYPE*, int);											// ｺﾏﾝﾄﾞﾚﾍﾞﾙ設定
int  GetCmdLvl(CMDTYPE*);												// ｺﾏﾝﾄﾞﾚﾍﾞﾙ取得
void BaseCmdPtr(int, OBJTYPE*, CMDTYPE**);								// 先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
void NextCmdPtr(CMDTYPE**);												// 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
void NextAllCmdPtr(CMDTYPE**);											// 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ(全ｺﾏﾝﾄﾞ対象)取得
void LevelBaseCmdPtr(CMDTYPE**);										// 同一ﾚﾍﾞﾙ内先頭ｺﾏﾝﾄﾞ取得
void LevelNextCmdPtr(CMDTYPE**);										// 同一ﾚﾍﾞﾙ内次ｺﾏﾝﾄﾞ取得
void EditCmdPtr(int, OBJTYPE*, CMDTYPE**);								// 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
void SetEditCmdPtr(int, OBJTYPE*, CMDTYPE*);							// 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ設定
void ResetEditCmdPtr(int, OBJTYPE*);									// 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ解除
void SetCmdPtr(CMDTYPE*);												// 処理対象ｺﾏﾝﾄﾞ設定
void GetCmdPtr(CMDTYPE**);												// 処理対象ｺﾏﾝﾄﾞ取得
void GetCmdObj(CMDTYPE*, OBJTYPE**);									// ｺﾏﾝﾄﾞ対象立体取得
int  GetCmdNo(CMDTYPE*);												// ｺﾏﾝﾄﾞNo取得
int  GetObjCmdNo(int flg, OBJTYPE* op);									// 立体ｺﾏﾝﾄﾞNo取得
int  GetIntNum(CMDTYPE*);												// 整数ﾊﾟﾗﾒｰﾀ数取得
void PutParaInt(CMDTYPE*, int, int);									// 整数ﾊﾟﾗﾒｰﾀ値設定
int  GetParaInt(CMDTYPE*, int);											// 整数ﾊﾟﾗﾒｰﾀ値取得
int  GetPntNum(CMDTYPE*);												// 座標ﾊﾟﾗﾒｰﾀ数取得
void PutParaPnt(CMDTYPE*, int, PNTTYPE);								// 座標ﾊﾟﾗﾒｰﾀ値設定
void GetParaPnt(CMDTYPE*, int, PNTTYPE*);								// 座標ﾊﾟﾗﾒｰﾀ値取得
void OnCmdFlgStp(void);													// 保存ﾌﾗｸﾞ更新中止ON
void OffCmdFlgStp(void);												// 保存ﾌﾗｸﾞ更新中止OFF
void OnCmdFlg(void);													// ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞON
void OffCmdFlg(void);													// ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞOFF
BOOL CheckCmdFlg(void);													// ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞ確認
void OnCmdExeStp(void);													// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞON
void OffCmdExeStp(void);												// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞOFF
int  GetCmdExeStp(void);												// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞ取得
void CreatCopyCmd(int, int, int, CPCTYPE**);							// ｺﾋﾟｰｺﾏﾝﾄﾞ領域確保
void FreeCopyCmd(void);													// ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
void BaseCopyCmdPtr(CPCTYPE**);											// 先頭ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
void NextCopyCmdPtr(CPCTYPE**);											// 次ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
int  GetCopyCmdNo(CPCTYPE*);											// ｺﾋﾟｰｺﾏﾝﾄﾞNo取得
int  GetCopyIntNum(CPCTYPE*);											// ｺﾋﾟｰｺﾏﾝﾄﾞ整数ﾊﾟﾗﾒｰﾀ数取得
void PutCopyParaInt(CPCTYPE*, int, int);								// ｺﾋﾟｰｺﾏﾝﾄﾞ整数ﾊﾟﾗﾒｰﾀ値設定
void GetCopyParaInt(CPCTYPE*, int, int*);								// ｺﾋﾟｰｺﾏﾝﾄﾞ整数ﾊﾟﾗﾒｰﾀ値取得
int  GetCopyPntNum(CPCTYPE*);											// ｺﾋﾟｰｺﾏﾝﾄﾞ座標ﾊﾟﾗﾒｰﾀ数取得
void PutCopyParaPnt(CPCTYPE*, int, PNTTYPE);							// ｺﾋﾟｰｺﾏﾝﾄﾞ座標ﾊﾟﾗﾒｰﾀ値設定
void GetCopyParaPnt(CPCTYPE*, int, PNTTYPE*);							// ｺﾋﾟｰｺﾏﾝﾄﾞ座標ﾊﾟﾗﾒｰﾀ値取得
void OnUndoRedo(void);													// UndoﾌﾗｸﾞON
void OffUndoRedo(void);													// UndoﾌﾗｸﾞOFF
BOOL CheckUndoRedo(void);												// Undoﾌﾗｸﾞ確認
void OnUndoStop(void);													// Undo保存停止ﾌﾗｸﾞON
void OffUndoStop(void);													// Undo保存停止ﾌﾗｸﾞOFF
void InitLevel(void);													// Undo/Redo初期化
void BackLevel(void);													// Undo/Redo戻り
BOOL CheckUndo(void);													// Undoﾁｪｯｸ
void Undo(int*);														// Undo処理
BOOL CheckRedo(void);													// Redoﾁｪｯｸ
void Redo(int*);														// Redo処理

#endif ___DATAACCESS1_H

//============================<EOF:DataAccess1.h>============================