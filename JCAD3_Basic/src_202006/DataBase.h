/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作ヘッダ
// DataBase.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATABASE_H
#define ___DATABASE_H

#include "DataType.h"

//----- 初期起動操作用 -----//
extern int      Demo_Flg;												// 体験版ﾌﾗｸﾞ
extern int      Init_Flg;												// 初期起動状態ﾌﾗｸﾞ
extern int      File_Flg;												// ﾌｧｲﾙ保存ﾌﾗｸﾞ
extern CString  FileName;												// 保存ﾌｧｲﾙ名
extern int      Spsh_Flg;												// ｽﾌﾟﾗｯｼｭ表示ﾌﾗｸﾞ
extern UINT     Msg_No;													// ﾒｯｾｰｼﾞNo

//----- バックアップ操作用変数 -----//
extern int      Cmd_Lvl;												// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
extern int      Cmd_Lvl_Sv;												// 保存用ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
extern int      Obj_No;													// 立体No

//----- データ領域操作用変数 -----//
extern int*     Int_ary;												// 整数ﾊﾟﾗﾒｰﾀ配列
extern PNTTYPE* Pnt_ary;												// 頂点ﾊﾟﾗﾒｰﾀ配列
extern UNDTYPE* Undo_ary;												// Undo/Redo配列
extern int      Int_adr;												// 整数ﾊﾟﾗﾒｰﾀ配列空先頭番地
extern int      Pnt_adr;												// 頂点ﾊﾟﾗﾒｰﾀ配列空先頭番地
extern int      Undo_adr;												// Undo/Redo配列ｶｳﾝﾀ(ｻｲｸﾘｯｸ)
extern int      Int_max;												// 整数ﾊﾟﾗﾒｰﾀ配列数
extern int      Pnt_max;												// 頂点ﾊﾟﾗﾒｰﾀ配列数
extern int      Undo_max;												// Undo/Redo配列数
extern int      CCtl_max;												// 最大曲線制御点数
extern int      SCtl_max;												// 最大曲面制御点数
extern int      CCtlA_max;												// 最大曲線制御点配列数
extern int      SCtlA_max;												// 最大曲面制御点配列数
extern int      WCtl_max;												// 作業用制御点配列数
extern int      CPnt_max;												// 最大曲線頂点数
extern int      SPnt_max;												// 最大曲面頂点数
extern int      OPnt_max;												// 最大立体頂点数
extern int      BPnt_max;												// 最大断面頂点数

//----- コマンド操作用変数 -----//
extern CMDTYPE* Temp_cmp;												// 一時削除ｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ
extern CMDTYPE* Gc_cmp;													// 再利用ｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ
extern CMDTYPE* Rf_cmp;													// 処理対象ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
extern CMDTYPE* Tach_cmp;												// 一時切離し用ﾎﾟｲﾝﾀ
extern short    Cmd_flg;												// ｺﾏﾝﾄﾞ保存ﾌﾗｸﾞ(0:済,1:未)
extern short    Cmd_upd;												// 保存ﾌﾗｸﾞ更新(0:有,1:無)
extern short    Cmd_stp;												// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞ(0:無,1:有)
extern CPCTYPE* Copy_cmp;												// ｺﾋﾟｰｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ

//----- オブジェクト操作用変数 -----//
extern OBJTYPE* Base_op;												// 有効立体先頭ﾎﾟｲﾝﾀ
extern OBJTYPE* Temp_op;												// 一時削除立体先頭ﾎﾟｲﾝﾀ
extern OBJTYPE* Gc_op;													// 再利用立体先頭ﾎﾟｲﾝﾀ
extern OBJTYPE* Rf_op;													// 処理対象立体ﾎﾟｲﾝﾀ
extern OBJTYPE* Reset_op;												// ﾘｾｯﾄ立体ﾎﾟｲﾝﾀ
extern OBJTYPE* Oset_op;												// ｾｯﾄ立体ﾎﾟｲﾝﾀ
extern BOXTYPE  Scr_Box;												// ｽｸﾘｰﾝﾎﾞｯｸｽ
extern int      SelectNo;												// 立体選択No

//----- 立体構成情報操作用変数 -----//
extern OIFTYPE* Base_oif;												// 有効立体構成先頭ﾎﾟｲﾝﾀ
extern OIFTYPE* Temp_oif;												// 一時削除立体構成先頭ﾎﾟｲﾝﾀ
extern OIFTYPE* Gc_oif;													// 再利用立体構成先頭ﾎﾟｲﾝﾀ

//----- Undo/Redo操作用変数 -----//
extern short    Undo_flg;												// ﾌﾗｸﾞ(0:無,1:有)
extern short    Undo_stp;												// ﾌﾗｸﾞ(0:保存無,1:保存有)

//----- 頂点情報ソート用 -----//
extern int      Now_no;													// 頂点稜線情報設定番号
extern VTYPE*   Vp;														// ｿｰﾄ用頂点配列ﾎﾟｲﾝﾀ
extern NVTYPE*  NVp;													// ｿｰﾄ用頂点配列ﾎﾟｲﾝﾀ

//----- RayTrace環境情報変数 -----//
extern COLORREF BkRGB;                                                  // 背景色
extern short    Max_Lvl;                                                // 最大探査ﾚﾍﾞﾙ
extern short    Anti;                                                   // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
extern short    Edge;                                                   // 稜線ﾌﾗｸﾞ
extern short    StgFlg;                                                 // ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
extern double   StgSize[4];                                             // ｽﾃｰｼﾞｻｲｽﾞ

//----- RayTrace光源情報変数 -----//
extern LIGHT    Lgt_Tbl[LGMAX];                                         // 光源情報配列

//----- RayTrace表面情報変数 -----//
extern ATTRIBT  Atr_Tbl[ATMAX];                                         // 表面属性配列

//----- POV-Ray環境情報変数 -----//
extern CString  PovInFile;												// POV-Ray定義ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)
extern CString  PovInPath;												// POV-Ray定義ﾌｧｲﾙﾊﾟｽ
extern CString  PovOutPath;												// POV-Ray出力ﾌｫﾙﾀﾞﾊﾟｽ
extern BOOL     PovSmooth;												// スムージング有無
extern double   PovAngle;												// 隣接法線ベクトル角度差
extern BOOL     PovTranslate;											// Translate有無
extern double   PovTranslateX;											// Translate量(X)
extern double   PovTranslateY;											// Translate量(Y)
extern double   PovTranslateZ;											// Translate量(Z)
extern BOOL     PovRotate;												// Rotate有無
extern double   PovRotateX;												// Rotate量(X)
extern double   PovRotateY;												// Rotate量(Y)
extern double   PovRotateZ;												// Rotate量(Z)
extern BOOL     PovScale;												// Scale有無
extern double   PovScaleX;												// Scale量(X)
extern double   PovScaleY;												// Scale量(Y)
extern double   PovScaleZ;												// Scale量(Z)
extern BOOL     PovAnima;												// ｱﾆﾒｰｼｮﾝ有無
extern int      PovAnimaZ;												// ｱﾆﾒｰｼｮﾝ分割数(Z)
extern int      PovAnimaY;												// ｱﾆﾒｰｼｮﾝ分割数(Y)
extern CString  PovDefFile1;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(ﾌﾙﾊﾟｽ)
extern CString  PovDefFile2;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(ﾌﾙﾊﾟｽ)
extern CString  PovDefFile1W;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名1WK(ﾌﾙﾊﾟｽ)
extern CString  PovDefFile2W;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名2WK(ﾌﾙﾊﾟｽ)
extern CString  PovDefPath;												// ﾕｰｻﾞ定義ﾌｧｲﾙﾊﾟｽ
extern int      PovTarget;												// photons target 有/無
extern int      PovCollect;												// photons collect on/off
extern int      PovReflection;											// photons reflection on/off
extern int      PovRefraction;											// photons refraction on/off
extern int      PovShape[POVCMAX];										// ｵﾌﾞｼﾞｪｸﾄ形状
extern int      PovTex1[POVCMAX];										// ﾃｸｽﾁｬNo1
extern int      PovTex2[POVCMAX];										// ﾃｸｽﾁｬNo2
extern int      PovTex3[POVCMAX];										// ﾃｸｽﾁｬNo3
extern int      PovTex4[POVCMAX];										// ﾃｸｽﾁｬNo4
extern int      PovTex5[POVCMAX];										// ﾃｸｽﾁｬNo5

//----- 立体･ｺﾏﾝﾄﾞ作業用変数 -----//
extern OWKTYPE* Base_owk;												// 立体作業ﾃﾞｰﾀ先頭ﾎﾟｲﾝﾀ
extern OWKTYPE* Rf_owk;													// 処理対象立体作業ﾃﾞｰﾀﾎﾟｲﾝﾀ

//----- 曲面細分化作業用変数 -----//
extern short    Curve_Rept_SW;											// 曲面細分化SW(0:無,1:有)
extern short    Curve_Rept_Num;											// 曲面細分化数

#endif ___DATABASE_H

//==============================<EOF:DataBase.h>=============================