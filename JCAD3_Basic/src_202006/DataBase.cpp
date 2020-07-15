/////////////////////////////////////////////////////////////////////////////
// 内部データベース
// DataBase.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataBase.h"

//----- 初期起動操作用 -----//
int      Demo_Flg;														// 体験版ﾌﾗｸﾞ
int      Init_Flg;														// 初期起動状態ﾌﾗｸﾞ
int      File_Flg;														// ﾌｧｲﾙ保存ﾌﾗｸﾞ
CString  FileName;														// 保存ﾌｧｲﾙ名
int      Spsh_Flg;														// ｽﾌﾟﾗｯｼｭ表示ﾌﾗｸﾞ
UINT     Msg_No;														// ﾒｯｾｰｼﾞNo

//----- バックアップ操作用変数 -----//
int      Cmd_Lvl;														// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
int      Cmd_Lvl_Sv;													// 保存用ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
int      Obj_No;														// 立体No

//----- データ領域操作用変数 -----//
int*     Int_ary;														// 整数ﾊﾟﾗﾒｰﾀ配列
PNTTYPE* Pnt_ary;														// 頂点ﾊﾟﾗﾒｰﾀ配列
UNDTYPE* Undo_ary;														// Undo/Redo配列
int      Int_adr;														// 整数ﾊﾟﾗﾒｰﾀ配列空先頭番地
int      Pnt_adr;														// 頂点ﾊﾟﾗﾒｰﾀ配列空先頭番地
int      Undo_adr;														// Undo/Redo配列ｶｳﾝﾀ(ｻｲｸﾘｯｸ)
int      Int_max;														// 整数ﾊﾟﾗﾒｰﾀ配列数
int      Pnt_max;														// 頂点ﾊﾟﾗﾒｰﾀ配列数
int      Undo_max;														// Undo/Redo配列数
int      CCtl_max;														// 最大曲線制御点数
int      SCtl_max;														// 最大曲面制御点数
int      CCtlA_max;														// 最大曲線制御点配列数
int      SCtlA_max;														// 最大曲面制御点配列数
int      WCtl_max;														// 作業用制御点配列数
int      CPnt_max;														// 最大曲線頂点数
int      SPnt_max;														// 最大曲面頂点数
int      OPnt_max;														// 最大立体頂点数
int      BPnt_max;														// 最大断面頂点数

//----- コマンド操作用変数 -----//
CMDTYPE* Temp_cmp;														// 一時削除ｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ
CMDTYPE* Gc_cmp;														// 再利用ｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ
CMDTYPE* Rf_cmp;														// 処理対象ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
CMDTYPE* Tach_cmp;														// 一時切離し用ﾎﾟｲﾝﾀ
short    Cmd_flg;														// ｺﾏﾝﾄﾞ保存ﾌﾗｸﾞ(0:済,1:未)
short    Cmd_upd;														// 保存ﾌﾗｸﾞ更新(0:有,1:無)
short    Cmd_stp;														// ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞ(0:無,1:有)
CPCTYPE* Copy_cmp;														// ｺﾋﾟｰｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ

//----- オブジェクト操作用変数 -----//
OBJTYPE* Base_op;														// 有効立体先頭ﾎﾟｲﾝﾀ
OBJTYPE* Temp_op;														// 一時削除立体先頭ﾎﾟｲﾝﾀ
OBJTYPE* Gc_op;															// 再利用立体先頭ﾎﾟｲﾝﾀ
OBJTYPE* Rf_op;															// 処理対象立体ﾎﾟｲﾝﾀ
OBJTYPE* Reset_op;														// ﾘｾｯﾄ立体ﾎﾟｲﾝﾀ
OBJTYPE* Oset_op;														// ｾｯﾄ立体ﾎﾟｲﾝﾀ
BOXTYPE  Scr_Box;														// ｽｸﾘｰﾝﾎﾞｯｸｽ
int      SelectNo;														// 立体選択No

//----- 立体構成情報操作用変数 -----//
OIFTYPE* Base_oif;														// 有効立体構成先頭ﾎﾟｲﾝﾀ
OIFTYPE* Temp_oif;														// 一時削除立体構成先頭ﾎﾟｲﾝﾀ
OIFTYPE* Gc_oif;														// 再利用立体構成先頭ﾎﾟｲﾝﾀ

//----- Undo/Redo操作用変数 -----//
short    Undo_flg;														// ﾌﾗｸﾞ(0:無,1:有)
short    Undo_stp;														// ﾌﾗｸﾞ(0:保存無,1:保存有)

//----- 頂点情報ソート用 -----//
int      Now_no;														// 頂点稜線情報設定番号
VTYPE*   Vp;															// ｿｰﾄ用頂点配列ﾎﾟｲﾝﾀ
NVTYPE*  NVp;															// ｿｰﾄ用頂点配列ﾎﾟｲﾝﾀ

//----- RayTrace環境情報変数 -----//
COLORREF BkRGB;                                                         // 背景色
short    Max_Lvl;                                                       // 最大探査ﾚﾍﾞﾙ
short    Anti;                                                          // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
short    Edge;                                                          // 稜線ﾌﾗｸﾞ
short    StgFlg;                                                        // ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
double   StgSize[4];                                                    // ｽﾃｰｼﾞｻｲｽﾞ

//----- RayTrace光源情報変数 -----//
LIGHT    Lgt_Tbl[LGMAX];                                                // 光源情報配列

//----- RayTrace表面情報変数 -----//
ATTRIBT  Atr_Tbl[ATMAX];                                                // 表面属性配列

//----- POV-Ray環境情報変数 -----//
CString  PovInFile;														// POV-Ray定義ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)
CString  PovInPath;														// POV-Ray定義ﾌｧｲﾙﾊﾟｽ
CString  PovOutPath;													// POV-Ray出力ﾌｫﾙﾀﾞﾊﾟｽ
BOOL     PovSmooth;														// スムージング有無
double   PovAngle;														// 隣接法線ベクトル角度差
BOOL     PovTranslate;													// Translate有無
double   PovTranslateX;													// Translate量(X)
double   PovTranslateY;													// Translate量(Y)
double   PovTranslateZ;													// Translate量(Z)
BOOL     PovRotate;														// Rotate有無
double   PovRotateX;													// Rotate量(X)
double   PovRotateY;													// Rotate量(Y)
double   PovRotateZ;													// Rotate量(Z)
BOOL     PovScale;														// Scale有無
double   PovScaleX;														// Scale量(X)
double   PovScaleY;														// Scale量(Y)
double   PovScaleZ;														// Scale量(Z)
BOOL     PovAnima;														// ｱﾆﾒｰｼｮﾝ有無
int      PovAnimaZ;														// ｱﾆﾒｰｼｮﾝ分割数(Z)
int      PovAnimaY;														// ｱﾆﾒｰｼｮﾝ分割数(Y)
CString  PovDefFile1;													// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(ﾌﾙﾊﾟｽ)
CString  PovDefFile2;													// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(ﾌﾙﾊﾟｽ)
CString  PovDefFile1W;													// ﾕｰｻﾞ定義ﾌｧｲﾙ名1WK(ﾌﾙﾊﾟｽ)
CString  PovDefFile2W;													// ﾕｰｻﾞ定義ﾌｧｲﾙ名2WK(ﾌﾙﾊﾟｽ)
CString  PovDefPath;													// ﾕｰｻﾞ定義ﾌｧｲﾙﾊﾟｽ
int      PovTarget;														// photons target 有/無
int      PovCollect;													// photons collect on/off
int      PovReflection;													// photons reflection on/off
int      PovRefraction;													// photons refraction on/off
int      PovShape[POVCMAX];												// ｵﾌﾞｼﾞｪｸﾄ形状
int      PovTex1[POVCMAX];												// ﾃｸｽﾁｬNo1
int      PovTex2[POVCMAX];												// ﾃｸｽﾁｬNo2
int      PovTex3[POVCMAX];												// ﾃｸｽﾁｬNo3
int      PovTex4[POVCMAX];												// ﾃｸｽﾁｬNo4
int      PovTex5[POVCMAX];												// ﾃｸｽﾁｬNo5

//----- 立体･ｺﾏﾝﾄﾞ作業用変数 -----//
OWKTYPE* Base_owk;														// 立体作業ﾃﾞｰﾀ先頭ﾎﾟｲﾝﾀ
OWKTYPE* Rf_owk;														// 処理対象立体作業ﾃﾞｰﾀﾎﾟｲﾝﾀ

//----- 曲面細分化作業用変数 -----//
short    Curve_Rept_SW;													// 曲面細分化SW(0:無,1:有)
short    Curve_Rept_Num;												// 曲面細分化数

//=============================<EOF:DataBase.cpp>============================