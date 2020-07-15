/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部
// DataAccess3.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "Command.h"
#include "NumFunc.h"
#include "Texture.h"

/*********************************************************/
/*****             ﾚｲ･ﾄﾚｰｼﾝｸﾞ環境変数操作            *****/
/*********************************************************/
/********************************/
double ChangeFltToDbl(float num)										//<<< float => double
/********************************/
{
	double w1;
	int    w2;

	w1 = (num>=0.0) ? num*10000.0+0.000999 : num*10000.0-0.000999;
	w2 = (int)w1;
	return (double)w2/10000.0;
}

/******************************/
void GetObjBottom(double* btm)											//<<< 表示立体ﾎﾞﾄﾑ取得
/******************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j;

	BaseObjPtr(1, &op, &i, &j);											// 表示立体取得
	if(!op) {															// <表示立体無>
		btm[0] = 0.0, btm[1] = 0.0, btm[2] = 0.0;						// ﾎﾞﾄﾑは原点
	} else {															// <表示立体有>
		GetAreaBox(1, &box);											// 全立体BOX取得
		BoxCenter(box, &pnt);											// BOXｾﾝﾀｰ取得
		btm[0] = pnt.p[0], btm[1] = pnt.p[1], btm[2] = box.bp[0].p[2];	// ﾎﾞﾄﾑ位置(Z)設定
	}
}

/***********************************************************/
void SetRayTraceEnv(COLORREF bk, int max, int ant, int edg,
                    int stf, double* sts)								//<<< ﾚｲ･ﾄﾚｰｽ環境設定
/***********************************************************/
{
	int i;

	BkRGB = bk;															// 背景色設定
	Max_Lvl = (short)max;												// 最大探査ﾚﾍﾞﾙ
	Anti = (short)ant;													// ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
	Edge = (short)edg;													// 稜線ﾌﾗｸﾞ
	StgFlg = (short)stf;												// ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
	for(i=0; i<4; i++) {
		StgSize[i] = sts[i];											// ｽﾃｰｼﾞｻｲｽﾞ設定
	}
}

/***************************************************************/
void GetRayTraceEnv(COLORREF* bk, int* max, int* ant, int* edg,
                    int* stf, double* sts)								//<<< ﾚｲ･ﾄﾚｰｽ環境取得
/***************************************************************/
{
	int i;

	*bk = BkRGB;														// 背景色取得
	*max = (int)Max_Lvl;												// 最大探査ﾚﾍﾞﾙ
	*ant = (int)Anti;													// ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
	*edg = (int)Edge;													// 稜線ﾌﾗｸﾞ
	*stf = (int)StgFlg;													// ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
	for(i=0; i<4; i++) {
		sts[i] = StgSize[i];											// ｽﾃｰｼﾞｻｲｽﾞ取得
	}
}

/******************************/
void InitRayTraceEnvFile(void)											//<<< 初期ﾚｲ･ﾄﾚｰｽ環境ﾌｧｲﾙ読込
/******************************/
{
	FILE*   fp;
	CString path;
	int     i1, i2, i3, i4, er=0;
	float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+TEN_NAME;										// ﾚｲ･ﾄﾚｰｽ初期環境ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 背景色設定
				er = 1; break;
			}
			BkRGB = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d%d", &i1, &i2, &i3, &i4)!=4) {
				er = 1; break;
			}
			Max_Lvl = (short)i1;										// 最大探査ﾚﾍﾞﾙ
			Anti = (short)i2;											// ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
			Edge = (short)i3;											// 稜線ﾌﾗｸﾞ
			StgFlg = (short)i4;											// ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
			if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {		// ｽﾃｰｼﾞｻｲｽﾞ設定
				er = 1; break;
			}
			StgSize[0] = ChangeFltToDbl(f1);
			StgSize[1] = ChangeFltToDbl(f2);
			StgSize[2] = ChangeFltToDbl(f3);
			StgSize[3] = ChangeFltToDbl(f4);
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/****************************/
void GetObjCent(double* oct)											//<<< 表示立体ｾﾝﾀｰ取得
/****************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j;

	BaseObjPtr(1, &op, &i, &j);											// 表示立体取得
	if(!op) {															// <表示立体無>
		oct[0] = 0.0, oct[1] = 0.0, oct[2] = 0.0;						// ｾﾝﾀｰは原点
	} else {															// <表示立体有>
		GetAreaBox(1, &box);											// 全立体BOX取得
		BoxCenter(box, &pnt);											// BOXｾﾝﾀｰ取得
		oct[0] = pnt.p[0], oct[1] = pnt.p[1], oct[2] = pnt.p[2];
	}
}

/****************************************/
void SetRayTraceLight(int no, LIGHT lgt)								//<<< ﾚｲ･ﾄﾚｰｽ光源設定
/****************************************/
{
	Lgt_Tbl[no] = lgt;													// 光源情報設定
}

/*****************************************/
void GetRayTraceLight(int no, LIGHT* lgt)								//<<< ﾚｲ･ﾄﾚｰｽ光源取得
/*****************************************/
{
	*lgt = Lgt_Tbl[no];													// 光源情報取得
}

/********************************/
void InitRayTraceLightFile(void)										//<<< 初期ﾚｲ･ﾄﾚｰｽ光源ﾌｧｲﾙ読込
/********************************/
{
	FILE*   fp;
	LIGHT   lgt;
	CString path;
	int     i, i1, i2, i3, er=0;
	float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+LGT_NAME;										// ﾚｲ･ﾄﾚｰｽ初期光源ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<LGMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {
				er = 1; break;
			}
			lgt.flg = i1;												// 有効ﾌﾗｸﾞ
			lgt.type = i2;												// 光源ﾀｲﾌﾟ
			lgt.k = i3;													// 光源減衰ﾀｲﾌﾟ
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 光源RGB
				er = 1; break;
			}
			lgt.lc = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// 光源座標位置
				er = 1; break;
			}
			lgt.x1 = ChangeFltToDbl(f1);
			lgt.y1 = ChangeFltToDbl(f2);
			lgt.z1 = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// 光源視線位置
				er = 1; break;
			}
			lgt.x2 = ChangeFltToDbl(f1);
			lgt.y2 = ChangeFltToDbl(f2);
			lgt.z2 = ChangeFltToDbl(f3);
			Lgt_Tbl[i] = lgt;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/******************************************/
void SetRayTraceColor(int no, ATTRIBT atr)								//<<< ﾚｲ･ﾄﾚｰｽ表面属性設定
/******************************************/
{
	Atr_Tbl[no] = atr;													// 表面属性設定
}

/*******************************************/
void GetRayTraceColor(int no, ATTRIBT* atr)								//<<< ﾚｲ･ﾄﾚｰｽ表面属性取得
/*******************************************/
{
	*atr = Atr_Tbl[no];													// 表面属性取得
}

/********************************/
void InitRayTraceColorFile(void)										//<<< 初期ﾚｲ･ﾄﾚｰｽｶﾗｰﾌｧｲﾙ読込
/********************************/
{
	FILE*   fp;
	ATTRIBT atr;
	CString path;
	int     i, i1, i2, i3, er=0;
	float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+TCL_NAME;										// ﾚｲ･ﾄﾚｰｽ初期ｶﾗｰﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<ATMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 環境光
				er = 1; break;
			}
			atr.ka = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 拡散反射光
				er = 1; break;
			}
			atr.kd = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 鏡面反射係数
				er = 1; break;
			}
			atr.ks = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// 透過調整係数
				er = 1; break;
			}
			atr.ct = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%f", &i1, &f1)!=2) {
				er = 1; break;
			}
			atr.trace = i1;												// 映り込み有無
			atr.n = ChangeFltToDbl(f1);									// 視線屈折率
			if(fscanf_s(fp, "%d%d%f", &i1, &i2, &f1)!=3) {
				er = 1; break;
			}
			atr.model = i1;												// ﾓﾃﾞﾙ(0:Phong, 1:Cook-T)
			atr.m1 = i2;												// ﾊｲﾗｲﾄの広がり1(Phong)
			atr.m2 = ChangeFltToDbl(f1);								// ﾊｲﾗｲﾄの広がり2(Cook-T)
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// 分光屈折率
				er = 1; break;
			}
			atr.nr = ChangeFltToDbl(f1);
			atr.ng = ChangeFltToDbl(f2);
			atr.nb = ChangeFltToDbl(f3);
			Atr_Tbl[i] = atr;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/***************************/
void ReadRayTraceFile(void)												//<<< ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ読込
/***************************/
{
	FILE*    fp;
	CString  path;
	LIGHT    lgt;
	ATTRIBT  atr;
	COLORREF bk;
	int      i, r, g, b, max, ant, edg, stf, er=0;
	float    f1, f2, f3, f4;
	double   sts[4];

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+RTS_NAME;										// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			bk = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d%d", &max, &ant, &edg, &stf)!=4) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {
				er = 1; break;
			}
			sts[0] = ChangeFltToDbl(f1);
			sts[1] = ChangeFltToDbl(f2);
			sts[2] = ChangeFltToDbl(f3);
			sts[3] = ChangeFltToDbl(f4);
			SetRayTraceEnv(bk, max, ant, edg, stf, sts);
			break;
		}
		for(i=0; i<LGMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &lgt.flg, &lgt.type, &lgt.k)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			lgt.lc = RGB(r, g, b);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			lgt.x1 = ChangeFltToDbl(f1);
			lgt.y1 = ChangeFltToDbl(f2);
			lgt.z1 = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			lgt.x2 = ChangeFltToDbl(f1);
			lgt.y2 = ChangeFltToDbl(f2);
			lgt.z2 = ChangeFltToDbl(f3);
			SetRayTraceLight(i, lgt);
		}
		for(i=0; i<ATMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ka = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.kd = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ks = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ct = RGB(r, g, b);
			if(fscanf_s(fp, "%d%f", &atr.trace, &f1)!=2) {
				er = 1; break;
			}
			atr.n = ChangeFltToDbl(f1);
			if(fscanf_s(fp, "%d%d%f", &atr.model, &atr.m1, &f1)!=3) {
				er = 1; break;
			}
			atr.m2 = ChangeFltToDbl(f1);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			atr.nr = ChangeFltToDbl(f1);
			atr.ng = ChangeFltToDbl(f2);
			atr.nb = ChangeFltToDbl(f3);
			SetRayTraceColor(i, atr);
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*****************************/
void WriteRayTraceFile(void)											//<<< ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ書込
/****************************/
{
	FILE*    fp;
	CString  path;
	LIGHT    lgt;
	ATTRIBT  atr;
	COLORREF bk;
	int      i, r, g, b, max, ant, edg, stf, er=0;
	float    f1, f2, f3, f4;
	double   sts[4];

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+RTS_NAME;										// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			GetRayTraceEnv(&bk, &max, &ant, &edg, &stf, sts);
			r = GetRValue(bk), g = GetGValue(bk), b = GetBValue(bk);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d\n", max, ant, edg, stf)<0) {
				er = 1; break;
			}
			f1 = (float)sts[0], f2 = (float)sts[1];
			f3 = (float)sts[2], f4 = (float)sts[3];
			if(fprintf(fp, "%f %f %f %f\n", f1, f2, f3, f4)<0) {
				er = 1; break;
			}
			break;
		}
		for(i=0; i<LGMAX; i++) {
			GetRayTraceLight(i, &lgt);
			if(fprintf(fp, "%d %d %d\n", lgt.flg, lgt.type, lgt.k)<0) {
				er = 1; break;
			}
			r = GetRValue(lgt.lc), g = GetGValue(lgt.lc), b = GetBValue(lgt.lc);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			f1 = (float)lgt.x1, f2 = (float)lgt.y1, f3 = (float)lgt.z1;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			f1 = (float)lgt.x2, f2 = (float)lgt.y2, f3 = (float)lgt.z2;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
		}
		for(i=0; i<ATMAX; i++) {
			GetRayTraceColor(i, &atr);
			r = GetRValue(atr.ka), g = GetGValue(atr.ka), b = GetBValue(atr.ka);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.kd), g = GetGValue(atr.kd), b = GetBValue(atr.kd);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.ks), g = GetGValue(atr.ks), b = GetBValue(atr.ks);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.ct), g = GetGValue(atr.ct), b = GetBValue(atr.ct);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			f1 = (float)atr.n;
			if(fprintf(fp, "%d %f\n", atr.trace, f1)<0) {
				er = 1; break;
			}
			f1 = (float)atr.m2;
			if(fprintf(fp, "%d %d %f\n", atr.model, atr.m1, f1)<0) {
				er = 1; break;
			}
			f1 = (float)atr.nr, f2 = (float)atr.ng, f3 = (float)atr.nb;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/******************************************************/
/*****             POV-Ray環境変数操作            *****/
/******************************************************/
/*************************************************************/
void SetPovData1(CString ifile, CString ipath, CString opath,
                 int smt, double agl,
                 int trs, double tsx, double tsy, double tsz,
                 int rot, double rox, double roy, double roz,
                 int scl, double scx, double scy, double scz,
                 int ani, int aniz, int aniy)							//<<< POV-Rayﾃﾞｰﾀ設定1
/*************************************************************/
{
	PovInFile     = ifile;												// POV-Ray定義ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)
	PovInPath     = ipath;												// POV-Ray定義ﾌｧｲﾙﾊﾟｽ
	PovOutPath    = opath;												// POV-Ray出力ﾌｫﾙﾀﾞﾊﾟｽ
	PovSmooth     = (BOOL)smt;											// スムージング有無
	PovAngle      = agl;												// 隣接法線ベクトル角度差
	PovTranslate  = (BOOL)trs;											// Translate有無
	PovTranslateX = tsx;												// Translate量(X)
	PovTranslateY = tsy;												// Translate量(Y)
	PovTranslateZ = tsz;												// Translate量(Z)
	PovRotate     = (BOOL)rot;											// Rotate有無
	PovRotateX    = rox;												// Rotate量(X)
	PovRotateY    = roy;												// Rotate量(Y)
	PovRotateZ    = roz;												// Rotate量(Z)
	PovScale      = (BOOL)scl;											// Scale有無
	PovScaleX     = scx;												// Scale量(X)
	PovScaleY     = scy;												// Scale量(Y)
	PovScaleZ     = scz;												// Scale量(Z)
	PovAnima      = (BOOL)ani;											// ｱﾆﾒｰｼｮﾝ有無
	PovAnimaZ     = aniz;												// ｱﾆﾒｰｼｮﾝ分割数(Z)
	PovAnimaY     = aniy;												// ｱﾆﾒｰｼｮﾝ分割数(Y)
}

/*****************************************************************/
void GetPovData1(CString* ifile, CString* ipath, CString* opath,
                 int* smt, double* agl,
                 int* trs, double* tsx, double* tsy, double* tsz,
                 int* rot, double* rox, double* roy, double* roz,
                 int* scl, double* scx, double* scy, double* scz,
                 int* ani, int* aniz, int* aniy)						//<<< POV-Rayﾃﾞｰﾀ取得1
/*****************************************************************/
{
	*ifile = PovInFile;													// POV-Ray定義ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)
	*ipath = PovInPath;													// POV-Ray定義ﾌｧｲﾙﾊﾟｽ
	*opath = PovOutPath;												// POV-Ray出力ﾌｫﾙﾀﾞﾊﾟｽ
	*smt = (int)PovSmooth;												// スムージング有無
	*agl = PovAngle;													// 隣接法線ベクトル角度差
	*trs = (int)PovTranslate;											// Translate有無
	*tsx = PovTranslateX;												// Translate量(X)
	*tsy = PovTranslateY;												// Translate量(Y)
	*tsz = PovTranslateZ;												// Translate量(Z)
	*rot = (int)PovRotate;												// Rotate有無
	*rox = PovRotateX;													// Rotate量(X)
	*roy = PovRotateY;													// Rotate量(Y)
	*roz = PovRotateZ;													// Rotate量(Z)
	*scl = (int)PovScale;												// Scale有無
	*scx = PovScaleX;													// Scale量(X)
	*scy = PovScaleY;													// Scale量(Y)
	*scz = PovScaleZ;													// Scale量(Z)
	*ani = (int)PovAnima;												// ｱﾆﾒｰｼｮﾝ有無
	*aniz = PovAnimaZ;													// ｱﾆﾒｰｼｮﾝ分割数(Z)
	*aniy = PovAnimaY;													// ｱﾆﾒｰｼｮﾝ分割数(Y)
}

/************************************************************/
void SetPovData2(CString file1, CString file2, CString path,
                 int targ, int colc, int refl, int refr)				//<<< POV-Rayﾃﾞｰﾀ設定2
/************************************************************/
{
	PovDefFile1   = file1;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(ﾌﾙﾊﾟｽ)
	PovDefFile2   = file2;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(ﾌﾙﾊﾟｽ)
	PovDefPath    = path;												// ﾕｰｻﾞ定義ﾌｧｲﾙﾊﾟｽ
	PovTarget     = targ;												// photons target 有/無
	PovCollect    = colc;												// photons collect on/off
	PovReflection = refl;												// photons reflection on/off
	PovRefraction = refr;												// photons refraction on/off
}

/***************************************************************/
void GetPovData2(CString* file1, CString* file2, CString* path,
                 int* targ, int* colc, int* refl, int* refr)			//<<< POV-Rayﾃﾞｰﾀ取得2
/***************************************************************/
{
	*file1 = PovDefFile1;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(ﾌﾙﾊﾟｽ)
	*file2 = PovDefFile2;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(ﾌﾙﾊﾟｽ)
	*path  = PovDefPath;												// ﾕｰｻﾞ定義ﾌｧｲﾙﾊﾟｽ
	*targ  = PovTarget;													// photons target 有/無
	*colc  = PovCollect;												// photons collect on/off
	*refl  = PovReflection;												// photons reflection on/off
	*refr  = PovRefraction;												// photons refraction on/off
	PovDefFile1W = PovDefFile1;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK1(ﾌﾙﾊﾟｽ)
	PovDefFile2W = PovDefFile2;											// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK2(ﾌﾙﾊﾟｽ)
}

/*************************************************/
void SetPovData3(int* shp, int* tex1, int* tex2,
                 int* tex3, int* tex4, int* tex5)						//<<< POV-Rayﾃﾞｰﾀ設定3
/*************************************************/
{
	int i;

	for(i=0; i<POVCMAX; i++) {
		PovShape[i] = shp[i];											// ｵﾌﾞｼﾞｪｸﾄ形状
		PovTex1[i] = tex1[i];											// ﾃｸｽﾁｬNo
		PovTex2[i] = tex2[i];
		PovTex3[i] = tex3[i];
		PovTex4[i] = tex4[i];
		PovTex5[i] = tex5[i];
	}
}

/*************************************************/
void GetPovData3(int* shp, int* tex1, int* tex2,
                 int* tex3, int* tex4, int* tex5)						//<<< POV-Rayﾃﾞｰﾀ取得3
/*************************************************/
{
	int i;

	for(i=0; i<POVCMAX; i++) {
		shp[i] = PovShape[i];											// ｵﾌﾞｼﾞｪｸﾄ形状
		tex1[i] = PovTex1[i];											// ﾃｸｽﾁｬNo
		tex2[i] = PovTex2[i];
		tex3[i] = PovTex3[i];
		tex4[i] = PovTex4[i];
		tex5[i] = PovTex5[i];
	}
}

/*******************************/
void SetWDefFile1(CString file)											//<<< POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定1
/*******************************/
{
	PovDefFile1W = file;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK1(ﾌﾙﾊﾟｽ)
}

/*******************************/
void SetWDefFile2(CString file)											//<<< POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定2
/*******************************/
{
	PovDefFile2W = file;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK2(ﾌﾙﾊﾟｽ)
}

/************************************************/
void GetWDefFile(CString* file1, CString* file2)						//<<< POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ取得
/************************************************/
{
	*file1 = PovDefFile1W;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK1(ﾌﾙﾊﾟｽ)
	*file2 = PovDefFile2W;												// ﾕｰｻﾞ定義ﾌｧｲﾙ名WK2(ﾌﾙﾊﾟｽ)
}

/**********************************************************/
void ReadPovDefFile(CString path, int* cnt, CString* data)				//<<< POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
/**********************************************************/
{
	char* cdmy = new char[256];
	FILE* fp;

	*cnt = 0;
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOpen
		while(TRUE) {													// EOFまで繰返
			fscanf_s(fp, "%s", cdmy, 256);
			if(feof(fp)) {
				break;
			}
			if(!strcmp(cdmy, "#declare")) {								// declare宣言文
				fscanf_s(fp, "%s", cdmy, 256);
				if(feof(fp)) {
					break;
				}
				data[*cnt] = CString(cdmy);								// 宣言名を配列に保存
				(*cnt)++;
			}
		}
		fclose(fp);														// ﾌｧｲﾙClose
	}
	delete[] cdmy;
}

/*************************/
void ReadPovRayFile(void)												//<<< POV-Ray保存ﾌｧｲﾙ読込
/*************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+POV_NAME;										// POV-Ray保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%f", &PovSmooth, &fd1)!=2) {
				er = 1; break;
			}
			PovAngle = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d%d", &PovTranslate,
                                      &PovRotate,
                                      &PovScale)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovTranslateX = ChangeFltToDbl(fd1);
			PovTranslateY = ChangeFltToDbl(fd2);
			PovTranslateZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovRotateX = ChangeFltToDbl(fd1);
			PovRotateY = ChangeFltToDbl(fd2);
			PovRotateZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovScaleX = ChangeFltToDbl(fd1);
			PovScaleY = ChangeFltToDbl(fd2);
			PovScaleZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &PovAnima,
                                      &PovAnimaZ,
                                      &PovAnimaY)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &PovTarget,
                                    &PovCollect)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &PovReflection,
                                    &PovRefraction)!=2) {
				er = 1; break;
			}
			for(i=0; i<POVCMAX; i++) {
				if(fscanf_s(fp, "%d%d%d", &PovShape[i],
                                          &PovTex1[i],
                                          &PovTex2[i])!=3) {
					er = 1; break;
				}
				if(fscanf_s(fp, "%d%d%d", &PovTex3[i],
                                          &PovTex4[i],
                                          &PovTex5[i])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovInFile.Format("%s", lpath);
				if(PovInFile=="") {
					PovInFile = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovInPath.Format("%s", lpath);
				if(PovInPath=="") {
					PovInPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovOutPath.Format("%s", lpath);
				if(PovOutPath=="") {
					PovOutPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovDefFile1.Format("%s", lpath);
				if(PovDefFile1=="") {
					PovDefFile1 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovDefFile2.Format("%s", lpath);
				if(PovDefFile2=="") {
					PovDefFile2 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				PovDefPath.Format("%s", lpath);
				if(PovDefPath=="") {
					PovDefPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		er = 1;
	}
	if(er) {															// ｴﾗｰ発生時は初期値
		PovSmooth     = 0,   PovAngle      = 45.0;
		PovTranslate  = 0,   PovRotate     = 0,   PovScale      = 0;
		PovTranslateX = 0.0, PovTranslateY = 0.0, PovTranslateZ = 0.0;
		PovRotateX    = 0.0, PovRotateY    = 0.0, PovRotateZ    = 0.0;
		PovScaleX     = 0.0, PovScaleY     = 0.0, PovScaleZ     = 0.0;
		PovAnima      = 0,   PovAnimaZ     = 15,  PovAnimaY     = 6;
		PovTarget     = 1,   PovCollect    = 0;
		PovReflection = 1,   PovRefraction = 1;
		PovShape[0]   = 0,   PovTex1[0]    = 0,   PovTex2[0]    = 0;	// ｺﾞｰﾙﾄﾞ
		PovShape[1]   = 0,   PovTex1[1]    = 0,   PovTex2[1]    = 2;	// ｼﾙﾊﾞｰ
		PovShape[2]   = 1,   PovTex1[2]    = 1,   PovTex2[2]    = 0;	// ｸﾘｽﾀﾙ
		PovShape[3]   = 1,   PovTex1[3]    = 1,   PovTex2[3]    = 2;	// ﾙﾋﾞｰ
		PovShape[4]   = 1,   PovTex1[4]    = 1,   PovTex2[4]    = 1;	// ｴﾒﾗﾙﾄﾞ
		PovShape[5]   = 1,   PovTex1[5]    = 1,   PovTex2[5]    = 3;	// ｻﾌｧｲｱ
		PovShape[6]   = 1,   PovTex1[6]    = 1,   PovTex2[6]    = 4;	// ﾕｰｻﾞｰ1
		PovShape[7]   = 1,   PovTex1[7]    = 1,   PovTex2[7]    = 5;	// ﾕｰｻﾞｰ2
		PovShape[8]   = 1,   PovTex1[8]    = 1,   PovTex2[8]    = 6;	// ﾕｰｻﾞｰ3
		PovShape[9]   = 1,   PovTex1[9]    = 1,   PovTex2[9]    = 7;	// ﾕｰｻﾞｰ4
		PovShape[10]  = 1,   PovTex1[10]   = 1,   PovTex2[10]   = 8;	// ﾕｰｻﾞｰ5
		PovShape[11]  = 1,   PovTex1[11]   = 1,   PovTex2[11]   = 9;	// ﾕｰｻﾞｰ6
		PovShape[12]  = 1,   PovTex1[12]   = 1,   PovTex2[12]   = 10;	// ﾕｰｻﾞｰ7
		PovShape[13]  = 1,   PovTex1[13]   = 1,   PovTex2[13]   = 11;	// ﾕｰｻﾞｰ8
		PovShape[14]  = 1,   PovTex1[14]   = 1,   PovTex2[14]   = 12;	// ﾕｰｻﾞｰ9
		PovShape[15]  = 1,   PovTex1[15]   = 1,   PovTex2[15]   = 13;	// ﾕｰｻﾞｰ10
		PovShape[16]  = 1,   PovTex1[16]   = 1,   PovTex2[16]   = 14;	// ﾕｰｻﾞｰ11
		PovShape[17]  = 1,   PovTex1[17]   = 1,   PovTex2[17]   = 15;	// ﾕｰｻﾞｰ12
		for(i=0; i<POVCMAX; i++) {
			PovTex3[i] = 0, PovTex4[i] = 0, PovTex5[i] = 0;
		}
		PovInFile   = GetAppPath(0)+POV_DEF1;
		PovInPath   = GetAppPath(0);
		PovOutPath  = GetAppPath(0);
		PovDefFile1 = GetAppPath(0)+POV_DEF2;
		PovDefFile2 = GetAppPath(0)+POV_DEF3;
		PovDefPath  = GetAppPath(0);
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
	delete[] lpath;
}

/**************************/
void WritePovRayFile(void)												//<<< POV-Ray保存ﾌｧｲﾙ書込
/**************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+POV_NAME;										// POV-Ray保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			fd1 = (float)PovAngle;
			if(fprintf(fp, "%d %f\n", PovSmooth, fd1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", PovTranslate,
                                         PovRotate,
                                         PovScale)<0) {
				er = 1; break;
			}
			fd1 = (float)PovTranslateX;
			fd2 = (float)PovTranslateY;
			fd3 = (float)PovTranslateZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)PovRotateX;
			fd2 = (float)PovRotateY;
			fd3 = (float)PovRotateZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)PovScaleX;
			fd2 = (float)PovScaleY;
			fd3 = (float)PovScaleZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", PovAnima,
                                         PovAnimaZ,
                                         PovAnimaY)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", PovTarget,
                                      PovCollect)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", PovReflection,
                                      PovRefraction)<0) {
				er = 1; break;
			}
			for(i=0; i<POVCMAX; i++) {
				if(fprintf(fp, "%d %d %d\n", PovShape[i],
                                             PovTex1[i],
                                             PovTex2[i])<0) {
					er = 1; break;
				}
				if(fprintf(fp, "%d %d %d\n", PovTex3[i],
                                             PovTex4[i],
                                             PovTex5[i])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovInFile, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovInPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovOutPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefFile1, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefFile2, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
}

/**********************************************************/
static char GetTexFlg1(CString def1, CString def2, int no)				//<<< ﾃｸｽﾁｬﾌﾗｸﾞ1取得
/**********************************************************/
{
	int i, cnt=0, pos;

	if(def1!="") {														// <ﾕｰｻﾞ定義1あり>
		if(no==cnt) {
			return '1';
		}
		cnt++;
	}
	if(def2!="") {														// <ﾕｰｻﾞ定義2あり>
		if(no==cnt) {
			return '2';
		}
		cnt++;
	}
	for(i=0; i<MATERIAL_CNT; i++) {										// <ﾃｸｽﾁｬ材質>
		if(no==cnt+i) {
			pos = T_Material[i].Find("|");								// 区切文字(|)
			return T_Material[i].GetAt(pos+1);
		}
	}
	return '0';
}

/***************************************************************/
static void GetTexture2(CString* name1, CString* name2, int no,
                        char flg1, CString* tex2, char* flg2)			//<<< ﾃｸｽﾁｬ2取得
/***************************************************************/
{
	int pos;

	if(flg1=='A') {														// <金>
		pos = T_Gold[no].Find("|");										// 区切文字(|)
		*tex2 = T_Gold[no].Left(pos);
		*flg2 = T_Gold[no].GetAt(pos+1);
	} else if(flg1=='B') {												// <銀>
		pos = T_Silver[no].Find("|");									// 区切文字(|)
		*tex2 = T_Silver[no].Left(pos);
		*flg2 = T_Silver[no].GetAt(pos+1);
	} else if(flg1=='C') {												// <銅>
		pos = T_Copper[no].Find("|");									// 区切文字(|)
		*tex2 = T_Copper[no].Left(pos);
		*flg2 = T_Copper[no].GetAt(pos+1);
	} else if(flg1=='D') {												// <真鍮>
		pos = T_Brass[no].Find("|");									// 区切文字(|)
		*tex2 = T_Brass[no].Left(pos);
		*flg2 = T_Brass[no].GetAt(pos+1);
	} else if(flg1=='E') {												// <ｸﾛﾑ>
		pos = T_Chrome[no].Find("|");									// 区切文字(|)
		*tex2 = T_Chrome[no].Left(pos);
		*flg2 = T_Chrome[no].GetAt(pos+1);
	} else if(flg1=='F') {												// <金属>
		pos = T_Metal[no].Find("|");									// 区切文字(|)
		*tex2 = T_Metal[no].Left(pos);
		*flg2 = T_Metal[no].GetAt(pos+1);
	} else if(flg1=='G') {												// <ｶﾞﾗｽ1>
		pos = T_Glass1[no].Find("|");									// 区切文字(|)
		*tex2 = T_Glass1[no].Left(pos);
		*flg2 = T_Glass1[no].GetAt(pos+1);
	} else if(flg1=='J') {												// <木>
		pos = T_Wood[no].Find("|");										// 区切文字(|)
		*tex2 = T_Wood[no].Left(pos);
		*flg2 = T_Wood[no].GetAt(pos+1);
	} else if(flg1=='K') {												// <石>
		pos = T_Stone[no].Find("|");									// 区切文字(|)
		*tex2 = T_Stone[no].Left(pos);
		*flg2 = T_Stone[no].GetAt(pos+1);
	} else if(flg1=='L') {												// <空>
		pos = T_Sky[no].Find("|");										// 区切文字(|)
		*tex2 = T_Sky[no].Left(pos);
		*flg2 = T_Sky[no].GetAt(pos+1);
	} else if(flg1=='1') {												// <ﾕｰｻﾞ定義1>
		*tex2 = name1[no];
		*flg2 = 'D';
	} else {															// <ﾕｰｻﾞ定義2>
		*tex2 = name2[no];
		*flg2 = 'E';
	}
}

/**********************************/
static CString GetTexture3(int no)										//<<< ﾃｸｽﾁｬ3取得
/**********************************/
{
	return G_Out_Prpt[no];
}

/*********************************************/
static CString GetTexture4(int no, char flg1)							//<<< ﾃｸｽﾁｬ4取得
/*********************************************/
{
	if(flg1=='I') {														// <ｶﾞﾗｽ3>
		return G_In_Prpt2[no];
	} else {															// <ｶﾞﾗｽ2>
		return G_In_Prpt1[no];
	}
}

/*********************************************/
static CString GetTexture5(int no, char flg1)							//<<< ﾃｸｽﾁｬ5取得
/*********************************************/
{
	if(flg1=='I') {														// <ｶﾞﾗｽ3>
		return G_Color2[no];
	} else {															// <ｶﾞﾗｽ2>
		return G_Color1[no];
	}
}

/******************************************************************/
void GetColorTex(int* it1, int* it2, int* it3, int* it4, int* it5,
                 char* ft1, char* ft2, CString* st2,
                 CString* st3, CString* st4, CString* st5)				// ｶﾗｰ別ﾃｸｽﾁｬ取得
/******************************************************************/
{
	CString	def1, def2, path, dname1[128], dname2[128];
	int     i, targ, colc, refl, refr, dcnt1=0, dcnt2=0;

	GetPovData2(&def1, &def2, &path, &targ, &colc, &refl, &refr);		// POV-Rayﾃﾞｰﾀ取得2
	if(def1!="") {
		ReadPovDefFile(def1, &dcnt1, dname1);							// POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
		if(dcnt1==0) {
			def1 = _T("");
		}
	}
	if(def2!="") {
		ReadPovDefFile(def2, &dcnt2, dname2);							// POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
		if(dcnt2==0) {
			def2 = _T("");
		}
	}
	for(i=0; i<POVCMAX; i++) {											// <ｶﾗｰ数分>
		ft1[i] = GetTexFlg1(def1, def2, it1[i]);						// ﾃｸｽﾁｬﾌﾗｸﾞ1取得
		GetTexture2(dname1, dname2, it2[i], ft1[i], &st2[i], &ft2[i]);	// ﾃｸｽﾁｬ2取得
		st3[i] = GetTexture3(it3[i]);									// ﾃｸｽﾁｬ3取得
		st4[i] = GetTexture4(it4[i], ft1[i]);							// ﾃｸｽﾁｬ4取得
		st5[i] = GetTexture5(it5[i], ft1[i]);							// ﾃｸｽﾁｬ5取得
	}
}

/********************/
void InitCNVtx(void)													//<<< 頂点ｿｰﾄ初期処理
/********************/
{
	NVp = NULL;
	Now_no = 0;
}

/*************************************/
void SetCNVtx(PNTTYPE pt, VECTYPE vc)									//<<< ｿｰﾄ頂点設定
/*************************************/
{
	NVTYPE* tv;

	tv = (NVTYPE *)malloc(sizeof(NVTYPE));								// ｿｰﾄ領域確保
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->next = NVp;
	tv->no   = Now_no;
	tv->pt   = pt;
	tv->vc   = vc;
	tv->fg1  = 0;
	tv->fg2  = 0;
	NVp      = tv;
	Now_no++;
}

/***********************************************/
static NVTYPE* MergeNVx(NVTYPE* la, NVTYPE* lb)							//<<< 頂点座標:Xｿｰﾄ
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[0]<=lb->pt.p[0]) {									// 頂点座標:X大小で比較
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtx(NVTYPE* mlist, int n)						//<<< 頂点座標:Xｿｰﾄ
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVx(NV_Mergsrtx(la, n/2),
                                NV_Mergsrtx(lb, n-n/2));				// 頂点座標:Xｿｰﾄ
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVy(NVTYPE* la, NVTYPE* lb)							//<<< 頂点座標:Yｿｰﾄ
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[1]<=lb->pt.p[1]) {									// 頂点座標:Y大小で比較
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrty(NVTYPE* mlist, int n)						//<<< 頂点座標:Yｿｰﾄ
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVy(NV_Mergsrty(la, n/2),
                                NV_Mergsrty(lb, n-n/2));				// 頂点座標:Yｿｰﾄ
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVz(NVTYPE* la, NVTYPE* lb)							//<<< 頂点座標:Zｿｰﾄ
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[2]<=lb->pt.p[2]) {									// 頂点座標:Z大小で比較
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtz(NVTYPE* mlist, int n)						//<<< 頂点座標:Zｿｰﾄ
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVz(NV_Mergsrtz(la, n/2),
                                NV_Mergsrtz(lb, n-n/2));				// 頂点座標:Zｿｰﾄ
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVn(NVTYPE* la, NVTYPE* lb)							//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// 面ﾙｰﾌﾟ番号大小で比較
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtn(NVTYPE* mlist, int n)						//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVn(NV_Mergsrtn(la, n/2),
                                NV_Mergsrtn(lb, n-n/2));				// 面ﾙｰﾌﾟ番号ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/*****************************************************/
static void CorrectionNVec1(NVTYPE* svp, NVTYPE* evp)					//<<< 法線ﾍﾞｸﾄﾙ補正1
/*****************************************************/					//   (同一法線ﾍﾞｸﾄﾙ検索)
{
	NVTYPE* wvp, * cvp;
	VECTYPE vec;

	wvp = svp;
	while(wvp!=NULL) {													// <同一頂点(svp～evp)>
		if(wvp==evp) {													// 最終頂点はﾁｪｯｸ対象外
			break;
		}
		if(wvp->fg1==0) {												// <未処理のみ対象>
			wvp->fg1 = 1;												// 単独面/代表面
			vec = wvp->vc;
			cvp = wvp->next;											// 次頂点以降とﾁｪｯｸ
			while(cvp!=NULL) {
				if(cvp->fg1==0) {										// <<未処理のﾃﾞｰﾀ>>
					if(fabs(CalcVecAngle(&wvp->vc, &cvp->vc))<PI/360) {	// 法線ﾍﾞｸﾄﾙ角度差0.5以内は
						cvp->fg1 = 2;									// 同一平面とし、付属面
					}
				}
				if(cvp==evp) {											// 最終頂点で終了
					break;
				}
				cvp = cvp->next;
			}
		}
		wvp = wvp->next;												// 次頂点へ
	}
}

/*****************************************************/
static void CorrectionNVec2(NVTYPE* svp, NVTYPE* evp,
                            int smooth, double angle)					//<<< 法線ﾍﾞｸﾄﾙ補正2
/*****************************************************/					//   (法線ﾍﾞｸﾄﾙ平均化)
{
	NVTYPE* wvp, * cvp;
	VECTYPE vec;
	int     i;

	wvp = svp;
	while(wvp!=NULL) {													// <同一頂点(svp～evp)>
		if(wvp==evp) {													// 最終頂点はﾁｪｯｸ対象外
			break;
		}
		if(wvp->fg2==0) {												// <未処理のみ対象>
			wvp->fg2 = 1;												// 補正中
			vec = wvp->vc;
			cvp = wvp->next;											// 次頂点以降とﾁｪｯｸ
			while(cvp!=NULL) {
				if(cvp->fg2==0) {										// <<未処理のﾃﾞｰﾀ>>
					if(!smooth||fabs(CalcVecAngle(&wvp->vc, &cvp->vc))
                                                      <PI*angle/180) {	// <ｽﾑｰｼﾞﾝｸﾞ有/指定角度未満>
						if(cvp->fg1<2) {								// 単独面/代表面の場合
							for(i=0; i<DIM; i++) {
								vec.p[i] += cvp->vc.p[i];				// 対象頂点ﾍﾞｸﾄﾙ加算
							}
						}
						cvp->fg2 = 1;									// 補正中
					}
				}
				if(cvp==evp) {											// 最終頂点で終了
					break;
				}
				cvp = cvp->next;
			}
			VecN(&vec);													// 単位ﾍﾞｸﾄﾙ化
			cvp = wvp;
			while(cvp!=NULL) {
				if(cvp->fg2==1) {										// <<処理中のﾃﾞｰﾀ>>
					cvp->fg2 = 2;										// 処理済
					cvp->vc = vec;										// 補正済法線ﾍﾞｸﾄﾙ
				}
				if(cvp==evp) {											// 最終頂点で終了
					break;
				}
				cvp = cvp->next;
			}
		}
		wvp = wvp->next;												// 次頂点へ
	}
}

/****************************************/
BOOL SortCNVtx(int smooth, double angle)								//<<< 頂点情報ｿｰﾄ&設定
/****************************************/
{
	NVTYPE* wvp, * svp=NULL, * cvp;
	PNTTYPE pt;

	if(Now_no==0) {
		return FALSE;
	}
	NVp = NV_Mergsrtz(NVp, Now_no);										// 頂点座標:Zｿｰﾄ
	NVp = NV_Mergsrty(NVp, Now_no);										// 頂点座標:Yｿｰﾄ
	NVp = NV_Mergsrtx(NVp, Now_no);										// 頂点座標:Xｿｰﾄ
	pt = NVp->pt;
	pt.p[0] = pt.p[0] - 1.0f;											// 初期値設定
	wvp = NVp;
	while(wvp!=NULL) {													// 設定済と異なる場合
		if(pt.p[0]!=wvp->pt.p[0]||
           pt.p[1]!=wvp->pt.p[1]||
           pt.p[2]!=wvp->pt.p[2]) {										// <頂点座標が異なる>
			if(svp!=NULL) {
				CorrectionNVec1(svp, cvp);								// 法線ﾍﾞｸﾄﾙ補正1
				CorrectionNVec2(svp, cvp, smooth, angle);				// 法線ﾍﾞｸﾄﾙ補正2
			}
			svp = wvp;													// 先頭ﾎﾟｲﾝﾀ保存
			pt = wvp->pt;
		}
		cvp = wvp;														// 直前のﾎﾟｲﾝﾀ保存
		wvp = wvp->next;
	}
	if(svp!=NULL) {
		CorrectionNVec1(svp, cvp);										// 法線ﾍﾞｸﾄﾙ補正1
		CorrectionNVec2(svp, cvp, smooth, angle);						// 法線ﾍﾞｸﾄﾙ補正2
	}
	NVp = NV_Mergsrtn(NVp, Now_no);										// 面ﾙｰﾌﾟ番号ｿｰﾄ
	return TRUE;
}

/*******************/
void EndCNVtx(void)														//<<< 頂点ｿｰﾄ終了処理
/*******************/
{
	NVTYPE* wvp, * cvp;

	wvp = NVp;
	while(wvp) {														// ｿｰﾄ領域解放
		cvp = wvp->next;
		free(wvp);
		wvp  = cvp;
	}
}

/*********************************************************/
/*****            立体･ｺﾏﾝﾄﾞ作業用変数操作           *****/
/*********************************************************/
/************************/
void CreateObjWork(void)												//<<< 立体作業ﾃﾞｰﾀ領域確保
/************************/
{
	OWKTYPE* optr;

	Rf_owk = (OWKTYPE *)malloc(sizeof(OWKTYPE));						// 新規領域確保
	if(Rf_owk==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Rf_owk->next = NULL;												// 次ﾃﾞｰﾀへﾎﾟｲﾝﾀ
	Rf_owk->op = NULL;													// 立体ﾎﾟｲﾝﾀ
	Rf_owk->cwk = NULL;													// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀﾎﾟｲﾝﾀ
	if(Base_owk==NULL) {
		Base_owk = Rf_owk;
	} else {
		optr = Base_owk;
		while(optr->next!=NULL) {
			optr = optr->next;											// 最終立体検索
		}
		optr->next = Rf_owk;											// 最終立体ﾘｽﾄ接続
	}
}

/******************************************/
void CreateCmdWork(int clvl, CMDTYPE* cmp)								//<<< ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ領域確保
/******************************************/
{
	CWKTYPE* cwkp, * cptr;

	cwkp = (CWKTYPE *)malloc(sizeof(CWKTYPE));							// 新規領域確保
	if(cwkp==NULL) {
		MemErr(IDS_MEMERR5);
	}
	cwkp->next = NULL;													// 次ﾃﾞｰﾀへﾎﾟｲﾝﾀ
	cwkp->owk = Rf_owk;													// 立体作業ﾃﾞｰﾀﾎﾟｲﾝﾀ
	cwkp->cmp = cmp;													// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	cwkp->clvl = clvl;													// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
	cwkp->flg = 0;														// 処理ﾌﾗｸﾞ(0:未)
	if(Rf_owk->cwk==NULL) {
		Rf_owk->cwk = cwkp;
	} else {
		cptr = Rf_owk->cwk;
		while(cptr->next!=NULL) {
			cptr = cptr->next;											// 最終ｺﾏﾝﾄﾞ検索
		}
		cptr->next = cwkp;												// 最終ｺﾏﾝﾄﾞﾘｽﾄ接続
	}
}

/***********************************************************************/
void SearchCmdWork(int lvl, CWKTYPE** cwk, CMDTYPE** cmp, OBJTYPE** op)	//<<< ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ検索
/***********************************************************************/
{
	OWKTYPE* optr;
	CWKTYPE* cptr;

	*cwk = NULL, *cmp = NULL, *op = NULL;								// ﾎﾟｲﾝﾀ初期化
	optr = Base_owk;													// 開始立体作業ﾎﾟｲﾝﾀ
	while(optr!=NULL) {
		cptr = optr->cwk;												// 立体別開始ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
		while(cptr!=NULL) {
			if(cptr->flg==0&&cptr->clvl==lvl) {							// <未処理の同一ｺﾏﾝﾄﾞﾚﾍﾞﾙNo>
				cptr->flg = 1;											// 処理済み
				*cwk = cptr;											// 対象ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
				*cmp = cptr->cmp;										// 対象ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
				*op = (cptr->owk)->op;									// 対象立体ﾎﾟｲﾝﾀ
				return;
			}
			cptr = cptr->next;											// 次ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
		}
		optr = optr->next;												// 次立体作業ﾎﾟｲﾝﾀ
	}
}

/********************************************/
void SetOwkObjPtr(CWKTYPE* cwk, OBJTYPE* op)							//<<< 立体作業ﾃﾞｰﾀ立体ﾎﾟｲﾝﾀ設定
/********************************************/
{
	(cwk->owk)->op = op;												// 立体ﾎﾟｲﾝﾀ設定
}

/*******************************/
void ChangeCwkFlg(CWKTYPE* cwk)											//<<< ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀﾌﾗｸﾞ更新
/*******************************/
{
	cwk->flg = 2;														// ｺﾏﾝﾄﾞ実行済み
}

/**************************/
void LastObjWorkProc(void)												//<<< 立体作業ﾃﾞｰﾀ最終処理
/**************************/
{
	OWKTYPE* optr;
	OIFTYPE* oif;
	int      oct=0;

	optr = Base_owk;													// 開始立体作業ﾎﾟｲﾝﾀ
	while(optr!=NULL) {
		if(optr->op!=NULL) {											// <立体ﾎﾟｲﾝﾀ登録済み>
			RedoCommand(optr->op);										// ｺﾏﾝﾄﾞ再実行
			GetObjOif(optr->op, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
			SetOifNo(oif, oct++);										// 立体構成立体No設定
			View->SetDispList(optr->op);								// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		optr = optr->next;												// 次立体作業ﾎﾟｲﾝﾀ
	}
}

/****************************/
void FreeObjCmdWork(int flg)											//<<< 立体/ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ解放
/****************************/
{
	OWKTYPE* optr, * dop;
	CWKTYPE* cptr, * dcp;

	optr = Base_owk;													// 開始立体作業ﾎﾟｲﾝﾀ
	while(optr!=NULL) {
		cptr = optr->cwk;												// 立体別開始ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
		while(cptr!=NULL) {
			if(flg==1) {												// <ｺﾏﾝﾄﾞ削除指示有>
				FreeCmd(NULL, cptr->cmp, 0);							// ｺﾏﾝﾄﾞ削除
			}
			dcp = cptr, cptr = cptr->next;								// 次ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
			free(dcp);													// ｺﾏﾝﾄﾞ作業領域解放
		}
		dop = optr, optr = optr->next;									// 次立体作業ﾎﾟｲﾝﾀ
		free(dop);														// 立体作業領域解放
	}
	Base_owk = NULL, Rf_owk = NULL;										// 初期化
}

/*********************************************************/
/*****            曲面細分化作業用変数操作           *****/
/*********************************************************/
/****************************/
void OnCurveReptSW(int mode)											//<<< 曲面細分化SWオン
/****************************/
{
	Curve_Rept_SW = 1;													// 曲面細分化作業あり
	if(mode==1) {
		Curve_Rept_Num = 3;												// 曲線分割数:3
	} else if(mode==2) {
		Curve_Rept_Num = 4;												// 曲線分割数:4
	} else if(mode==3) {
		Curve_Rept_Num = 5;												// 曲線分割数:5
	} else {
		Curve_Rept_Num = 2;												// 曲線分割数:2
	}
}

/*************************/
void OffCurveReptSW(void)												//<<< 曲面細分化SWオフ
/*************************/
{
	Curve_Rept_SW = 0;													// 曲面細分化作業なし
	Curve_Rept_Num = 3;		//202006 2->3					　			// 曲線分割数:4
}

/*************************/
BOOL GetCurveReptSW(void)												//<<< 曲面細分化SW取得
/*************************/
{
	return (BOOL)Curve_Rept_SW;											// 曲面細分化SW
}

/*************************/
int GetCurveReptNum(void)												//<<< 曲面分割数取得
/*************************/
{
	return (int)Curve_Rept_Num;											// 曲線分割数
}
