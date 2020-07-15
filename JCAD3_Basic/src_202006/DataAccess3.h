/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部プロトタイプ宣言
// DataAccess3.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/24 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS3_H
#define ___DATAACCESS3_H

#include "DataType.h"

double ChangeFltToDbl(float);											// float => double
void GetObjBottom(double*);												// 表示立体ﾎﾞﾄﾑ取得
void SetRayTraceEnv(COLORREF, int, int, int, int, double*);				// ﾚｲ･ﾄﾚｰｽ環境設定
void GetRayTraceEnv(COLORREF*, int*, int*, int*, int*, double*);		// ﾚｲ･ﾄﾚｰｽ環境取得
void InitRayTraceEnvFile(void);											// 初期ﾚｲ･ﾄﾚｰｽ環境ﾌｧｲﾙ読込
void GetObjCent(double*);												// 表示立体ｾﾝﾀｰ取得
void SetRayTraceLight(int, LIGHT);										// ﾚｲ･ﾄﾚｰｽ光源設定
void GetRayTraceLight(int, LIGHT*);										// ﾚｲ･ﾄﾚｰｽ光源取得
void InitRayTraceLightFile(void);										// 初期ﾚｲ･ﾄﾚｰｽ光源ﾌｧｲﾙ読込
void SetRayTraceColor(int, ATTRIBT);									// ﾚｲ･ﾄﾚｰｽ表面属性設定
void GetRayTraceColor(int, ATTRIBT*);									// ﾚｲ･ﾄﾚｰｽ表面属性取得
void InitRayTraceColorFile(void);										// 初期ﾚｲ･ﾄﾚｰｽｶﾗｰﾌｧｲﾙ読込
void ReadRayTraceFile(void);											// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ読込
void WriteRayTraceFile(void);											// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ書込
void CreateObjWork(void);												// 立体作業ﾃﾞｰﾀ領域確保
void SetPovData1(CString, CString, CString, int, double, int,
                 double, double, double, int, double, double,
                 double, int, double, double, double, int, int, int);	// POV-Rayﾃﾞｰﾀ設定1
void GetPovData1(CString*, CString*, CString*, int*, double*, int*,
                 double*, double*, double*, int*, double*, double*,
                 double*, int*, double*, double*, double*,
                 int*, int*, int*);										// POV-Rayﾃﾞｰﾀ取得1
void SetPovData2(CString, CString, CString, int, int, int, int);		// POV-Rayﾃﾞｰﾀ設定2
void GetPovData2(CString*, CString*, CString*, int*, int*, int*, int*);	// POV-Rayﾃﾞｰﾀ取得2
void SetPovData3(int*, int*, int*, int*, int*, int*);					// POV-Rayﾃﾞｰﾀ設定3
void GetPovData3(int*, int*, int*, int*, int*, int*);					// POV-Rayﾃﾞｰﾀ取得3
void SetWDefFile1(CString);												// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定1
void SetWDefFile2(CString);												// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定2
void GetWDefFile(CString*, CString*);									// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ取得
void ReadPovDefFile(CString, int*, CString*);							// POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
void ReadPovRayFile(void);												// POV-Ray保存ﾌｧｲﾙ読込
void WritePovRayFile(void);												// POV-Ray保存ﾌｧｲﾙ書込
void GetColorTex(int*, int*, int*, int*, int*, char*, char*,
                 CString*, CString*, CString*, CString*);				// ｶﾗｰ別ﾃｸｽﾁｬ取得
void InitCNVtx(void);													// 頂点ｿｰﾄ初期処理
void SetCNVtx(PNTTYPE, VECTYPE);										// ｿｰﾄ頂点設定
BOOL SortCNVtx(int, double);											// 頂点情報ｿｰﾄ&設定
void EndCNVtx(void);													// 頂点ｿｰﾄ終了処理
void CreateCmdWork(int, CMDTYPE*);										// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ領域確保
void SearchCmdWork(int, CWKTYPE**, CMDTYPE**, OBJTYPE**);				// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ検索
void SetOwkObjPtr(CWKTYPE*, OBJTYPE*);									// 立体作業ﾃﾞｰﾀ立体ﾎﾟｲﾝﾀ設定
void ChangeCwkFlg(CWKTYPE*);											// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀﾌﾗｸﾞ更新
void LastObjWorkProc(void);												// 立体作業ﾃﾞｰﾀ最終処理
void FreeObjCmdWork(int);												// 立体/ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ解放
void OnCurveReptSW(int);												// 曲面細分化SWオン
void OffCurveReptSW(void);												// 曲面細分化SWオフ
BOOL GetCurveReptSW(void);												// 曲面細分化SW取得
int  GetCurveReptNum(void);												// 曲面分割数取得

#endif ___DATAACCESS3_H

//============================<EOF:DataAccess3.h>============================