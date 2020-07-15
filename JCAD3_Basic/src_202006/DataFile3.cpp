/////////////////////////////////////////////////////////////////////////////
// ﾌｧｲﾙ入出力部3
// DataFile3.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "Command.h"
#include "CCommand.h"
#include "RCommand.h"
#include "NumFunc.h"
#include "CFAC32_Api.h"
#include "SLCA32_Api.h"

#define	MAXFVTX	1000													// 最大面頂点数
#define	MAXPART	10000													// 最大ﾊﾟｰﾂ数

typedef	struct {
	int    vno;															// 3次元上の頂点番号
	double x;															// 2次元上のX座標
	double y;															//    〃    Y座標
} VERTEX;

CProgressDlg* m_pProgressF3=NULL;										// ﾌﾟﾛｸﾞﾚｽｲﾝｼﾞｹｰﾀｰ
int	FacNum;																// 面数
int	ObjNum;																// 立体数
int PolyFlg;															// ﾎﾟﾘｺﾞﾝﾌﾗｸﾞ(0:ﾌｧｲﾙ主力,1:ﾎﾟﾘｺﾞﾝ作成)

/***********************************************************************/
static void ReadCommonFileParam(int* ver, int* out, int* typ, int* fac)	//<<< 共通ﾌｧｲﾙﾊﾟﾗﾒｰﾀ保存ﾌｧｲﾙ読込
/***********************************************************************/
{
	FILE*    fp;
	CString  path;
	int      er=0;

	path = GetAppPath(0)+CFAC_NAME;										// CFAC32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		if(fscanf_s(fp, "%d%d%d%d", ver, out, typ, fac)!=4) {
			er = 1;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		er = 1;
	}
	if(er) {
		*ver = 0, *out = 0, *typ = 0, *fac = 0;
	}
}

/********************************************************************/
static void WriteCommonFileParam(int ver, int out, int typ, int fac)	//<<< 共通ﾌｧｲﾙﾊﾟﾗﾒｰﾀ保存ﾌｧｲﾙ書込
/********************************************************************/
{
	FILE*    fp;
	CString  path;
	int      er=0;

	path = GetAppPath(0)+CFAC_NAME;										// CFAC32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		if(fprintf(fp, "%d %d %d %d\n", ver, out, typ, fac)<0) {
			er = 1;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
}

/*********************************************************************/
static void PutTriangle(FILE* fp, OBJTYPE* op, VECTYPE v, VERTEX* pa)	//<<< 三角形ﾊﾟｯﾁ出力
/*********************************************************************/
{
	PNTTYPE pt;
	int     i, flg;

	if(!PolyFlg) {														// <ﾌｧｲﾙ出力の場合>
		fprintf(fp, "%d %f %f %f\n", 2, v.p[0], v.p[1], v.p[2]);		// 法線ﾍﾞｸﾄﾙ出力
	}
	for(i=0; i<3; i++) {
		GetVtx(op, pa[i].vno, &pt);										// 頂点座標取得
		flg = (i==2) ? 1 : 0;											// 面ﾙｰﾌﾟ中間/終了
		fprintf(fp, "%d %f %f %f\n", flg, pt.p[0], pt.p[1], pt.p[2]);	// 頂点座標出力
	}
	FacNum++;															// 面数加算
}

/**************************************************************/
static double TriangleArea(int p1, int p2, int p3, VERTEX* pa)			//<<< 三角形符号付き面積
/**************************************************************/
{
	return ((pa[p1].x-pa[p3].x)*(pa[p2].y-pa[p3].y)
            -(pa[p2].x-pa[p3].x)*(pa[p1].y-pa[p3].y));
}

/*****************************************************************/
static BOOL CheckArea(int p1, int p2, int p3, int pp, VERTEX* pa)		//<<< 三角形内側確認
/*****************************************************************/
{
	if(TriangleArea(pp, p1, p2, pa)<0.0) {								// 三角形面積
		return FALSE;
	}
	if(TriangleArea(pp, p2, p3, pa)<0.0) {								// 三角形面積
		return FALSE;
	}
	if(TriangleArea(pp, p3, p1, pa)<0.0) {								// 三角形面積
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
static int CheckInsideTriangle(int vm, int vf, int vb,VERTEX* pa,int pn)//<<< 三角ﾊﾟｯﾁ数検定
/************************************************************************/
{
	int pp, lp=-1;

    pp = (vf+1==pn) ? 0 : vf+1;
	while(pp!=vb) {
		if(CheckArea(vm, vf, vb, pp, pa)) {								// 三角形内側確認
			if(lp==-1) {
				lp = pp;
			} else {
				if(pa[pp].y<pa[lp].y) {
					lp = pp;
				}
			}
		}
        pp = (pp+1==pn) ? 0 : pp+1;
	}
	return lp;
}

/**************************************************************/
static void MakeDivPolygon(int sp, int ep, VERTEX* pa, int pn,
                           VERTEX* pa1, int* pn1)						//<<< 新多角形生成
/**************************************************************/
{
	int ps, pd=0;

	ps = sp;
	while(ps!=ep) {
		pa1[pd++] = pa[ps++];
		if(ps==pn) {
			ps = 0;
		}
	}
	pa1[pd++] = pa[ps++], *pn1 = pd;
}

/******************************************/
static int FindYminVtx(VERTEX* pa, int pn)								//<<< 最小Y座標値取得
/******************************************/
{
	int    i, lp=0;
	double miny;

	miny = pa[0].y;
	for(i=1; i<pn; i++) {
		if(pa[i].y<miny) {
			miny = pa[i].y, lp = i;
		}
	}
	return lp;
}

/***************************************************************/
static void DivisionPolygon(FILE* fp, OBJTYPE* op, VECTYPE vec, 
                            VERTEX* pa, int pn)							//<<< 多角形を三角形に分割
/***************************************************************/
{
	VERTEX* pa1 = new VERTEX[MAXFVTX];
	int     vm, vf, vb, vs, i, lc, pn1;
	double  fa;

	if(pn==3) {
		PutTriangle(fp, op, vec, pa);									// 三角形ﾊﾟｯﾁ出力
	} else {
		lc = 0;
		do {
			vm = FindYminVtx(pa, pn);									// 最小Y座標値取得
			if((vf=vm+1)>=pn) {
				vf = 0;
			}
			if((vb=vm-1)<0) {
				vb = pn-1;
			}
			fa = TriangleArea(vb, vm, vf, pa);							// 三角形面積
			if(fa<EPSILON) {
				pa[pn] = pa[0];
				for(i=0; i<pn; ++i) {
					pa[i] = pa[i+1];
				}
			}
			if(lc++>pn) {
				break;
			}
		} while(fa<EPSILON);
		vs = CheckInsideTriangle(vm, vf, vb, pa, pn);					// 三角ﾊﾟｯﾁ数検定
		if(vs<0) {														// <頂点無の場合>
			MakeDivPolygon(vb, vf, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, op, vec, pa1, pn1);						// 多角形分割
			MakeDivPolygon(vf, vb, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, op, vec, pa1, pn1);						// 多角形分割
		} else {														// <頂点有の場合>
			MakeDivPolygon(vm, vs, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, op, vec, pa1, pn1);						// 多角形分割
			MakeDivPolygon(vs, vm, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, op, vec, pa1, pn1);						// 多角形分割
		}
	}
	delete[] pa1;
}

/*******************************************************/
static void MakePolygon(PNTTYPE* pnt, int* vno, int pn,
                        VERTEX* pa, VECTYPE* vec)						//<<< 分割面ﾃﾞｰﾀ作成
/*******************************************************/
{
	VECTYPE rvc;
	double  rot[3][3], th;
	int     i;

	CalcNVec(pnt, pn, vec);												// 法線ﾍﾞｸﾄﾙ取得
	th = sqrt(vec->p[0]*vec->p[0]+vec->p[1]*vec->p[1]);					// 角度,回転軸取得
	if(th<EPSILON) {
		rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (vec->p[2]<0.0) ? PI : 0.0;
	} else {
		rvc.p[0] = -vec->p[1], rvc.p[1] = vec->p[0], rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (fabs(vec->p[2])>EPSILON) ? atan2(th, vec->p[2]) : PI/2.0;
		th = (th<-10||th>10) ? 0.0 : th;
	}
	SetRotateMatrix(rvc, -th, rot);										// 回転ﾏﾄﾘｯｸｽ作成
	for(i=0; i<pn; i++) {
		VecRotate(&pnt[i], rot);										// 面を回転(XY平面と平行)
		pa[i].vno = vno[i], pa[i].x = pnt[i].p[0], pa[i].y = pnt[i].p[1];
	}
}

/*********************************************/
static BOOL CheckPolygon(VERTEX* pa, int* pn)							//<<< 多角形頂点確認
/*********************************************/
{
	int i, j, pnum;

	pnum = *pn;
	pa[pnum] = pa[0];
	for(i=0; i<pnum; i++) {
		if(fabs(pa[i].x-pa[i+1].x)<EPSILON&&
           fabs(pa[i].y-pa[i+1].y)<EPSILON) {
			if(i!=(pnum-1)) {
				for(j=i+1; j<pnum; ++j) {
					pa[j] = pa[j+1];
				}
				pnum--;
			} else {
				pnum--; break;
			}
		}
	}
	if(pnum<3) {
		return FALSE;
	}
	*pn = pnum;
	return TRUE;
}

/************************************************/
int WriteCommonFile(int mode, const char* fname)						//<<< 共通ﾌｧｲﾙ出力
/************************************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	char*    tname = new char[_MAX_PATH];
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op;
	VECTYPE  vec;
	CString  pname;
	int      ret=0, ver, out, typ, fac, i, i1, i2, lp, pn, sln=0, cln=0, spr=0;
	double   dpr;

	PolyFlg = 0;														// ﾎﾟﾘｺﾞﾝﾌﾗｸﾞ(0:ﾌｧｲﾙ主力)
	ReadCommonFileParam(&ver, &out, &typ, &fac);						// 共通ﾌｧｲﾙﾊﾟﾗﾒｰﾀ保存ﾌｧｲﾙ読込
	if(mode==0) {														// <DXFﾌｧｲﾙの場合>
		ret = InpDXFParam(&ver, &out, &fac);							// DXFﾊﾟﾗﾒｰﾀ入力
	} else {															// <STLﾌｧｲﾙの場合>
		ret = InpSTLParam(&typ, &fac);									// STLﾊﾟﾗﾒｰﾀ入力
	}
	if(!ret) {															// <ﾀﾞｲｱﾛｸﾞ正常終了>
		WriteCommonFileParam(ver, out, typ, fac);						// 共通ﾌｧｲﾙﾊﾟﾗﾒｰﾀ保存ﾌｧｲﾙ書込
	} else {															// <ﾀﾞｲｱﾛｸﾞ取消終了>
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] tname;
		return 0;
	}
	FacNum = 0, ObjNum = 0;												// 面数･立体数ｸﾘｱ
	i = strlen(fname)-3;												// ﾌｧｲﾙ名長(拡張子なし)
	memset(tname, 0x0, _MAX_PATH);
	memcpy(tname, fname, i);											// ﾌｧｲﾙ名複写(拡張子なし)
	memcpy(&tname[i], "tmp", 3);										// 拡張子(tmp)
	if(fopen_s(&fp, tname, "w")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] tname;
		return 1;
	}
	m_pProgressF3 = new CProgressDlg(3);
	m_pProgressF3->Create();											// ﾀﾞｲｱﾛｸﾞ表示
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	if(fac>0) {															// <曲面細分化あり>
		OnCurveReptSW(fac);												// 曲面細分化SWオン
		BaseObjPtr(1, &op, &i1, &i2);									// 表示立体ﾎﾟｲﾝﾀ取得
		while(op!=NULL) {
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			NextObjPtr(1, &op, &i1, &i2);								// 次編集立体へ
		}
		View->Display(0);												// 再描画
	}
	BaseObjPtr(1, &op, &i1, &i2);										// 表示対象立体三角形分割
	while(op) {
		sln += GetFlpNum1(op);											// 面ﾙｰﾌﾟ数加算
		NextObjPtr(1, &op, &i1, &i2);									// 次立体へ
	}
	fprintf(fp, "%d %08d %08d\n", 9, FacNum, ObjNum);					// 面数･立体数出力(仮)
	BaseObjPtr(1, &op, &i1, &i2);										// 表示対象立体三角形分割
	while(op) {
		lp = GetFlpNum1(op);											// 面ﾙｰﾌﾟ数取得
		for(i=0, pn=0; i<lp; i++) {										// 全面を三角形分割
			GetFlp1(op, i, &vno[pn], &i1);								// 面ﾙｰﾌﾟ番号取得
			GetVtx(op, vno[pn], &pnt[pn]);								// 頂点座標取得
			pn++;
			if(i1) {													// <面ﾙｰﾌﾟ終了>
				MakePolygon(pnt, vno, pn, pa, &vec);					// 分割面ﾃﾞｰﾀ作成
				if(CheckPolygon(pa, &pn)) {								// 多角形頂点確認
					DivisionPolygon(fp, op, vec, pa, pn);				// 多角形分割
				}
				pn = 0;
			}
			cln++;
			dpr = 100.0*((double)cln/(double)sln);						// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF3->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
		}
		if(ret) {														// 強制終了時は中止
			break;
		}
		if(mode==0) {													// <DXFﾌｧｲﾙの場合>
			fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);			// 立体終了出力
			ObjNum++;													// 立体数加算
		}
		NextObjPtr(1, &op, &i1, &i2);									// 次立体へ
	}
	if(!ret&&mode==1) {													// <STLﾌｧｲﾙの場合>
		fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);				// 立体終了出力
		ObjNum++;														// 立体数加算
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	if(!ret) {															// <正常処理中>
		if(fopen_s(&fp, tname, "r+")==0) {								// 作業ﾌｧｲﾙOPEN
			fprintf(fp, "%d %08d %08d\n", 9, FacNum, ObjNum);			// 面数･立体数出力(本)
			fclose(fp);													// 作業ﾌｧｲﾙCLOSE
		}
	}
	m_pProgressF3->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressF3;
	pname = GetAppPath(0)+CFAC_NAME;									// ﾃﾞｰﾀ保存ﾌｧｲﾙPATH取得
	if(!ret) {															// <正常処理中>
		if(mode==0) {													// <DXFﾌｧｲﾙの場合>
			ret = ChgWDXFFile(fname, tname, ver, out);					// DXFﾌｧｲﾙ変換(Write)
		} else {														// <STLﾌｧｲﾙの場合>
			ret = ChgWSTLFile(fname, tname, typ);						// STLﾌｧｲﾙ変換(Write)
		}
	}
	if(fac>0) {															// <曲面細分化あり>
		OffCurveReptSW();												// 曲面細分化SWオフ
		BaseObjPtr(1, &op, &i1, &i2);									// 表示立体ﾎﾟｲﾝﾀ取得
		while(op!=NULL) {
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			NextObjPtr(1, &op, &i1, &i2);								// 次編集立体へ
		}
		View->Display(0);												// 再描画
	}
	remove(tname);														// 作業ﾌｧｲﾙ削除
	if(!ret) {
		OffCmdFlg();													// ｺﾏﾝﾄﾞ未保存OFF
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	delete[] tname;
	return ret;
}

/***********************************************/
int ReadCommonFile(int mode, const char* fname)							//<<< 共通ﾌｧｲﾙ入力
/***********************************************/
{
	int*     ln = new int[MAXPART];
	int*     fn = new int[MAXPART];
	char*    tname = new char[_MAX_PATH];
	char*    b1 = new char[100];
	char*    b2 = new char[100];
	char*    b3 = new char[100];
	MSG      msg;
	OBJTYPE* op, * sop;
	OIFTYPE* oif;
	PNTTYPE  pa;
	FILE*    fp;
	int      ret, i, j, on, flg, pn, tvn, psw=0, cln=0, spr=0;
	double   dpr;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	i = strlen(fname)-3;												// ﾌｧｲﾙ名長(拡張子なし)
	memset(tname, 0x0, _MAX_PATH);
	memcpy(tname, fname, i);											// ﾌｧｲﾙ名複写(拡張子なし)
	memcpy(&tname[i], "tmp", 3);										// 拡張子(tmp)
	if(mode==0) {														// <DXFﾌｧｲﾙの場合>
		ret = ChgRDXFFile(fname, tname);								// DXFﾌｧｲﾙ変換(Read)
	} else {															// <STLﾌｧｲﾙの場合>
		ret = ChgRSTLFile(fname, tname);								// STLﾌｧｲﾙ変換(Read)
	}
	if(!ret) {															// <正常処理中>
		View->Display(0);												// 再描画
		if(fopen_s(&fp, tname, "r")!=0) {								// 作業ﾌｧｲﾙOPEN
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		psw = 1;														// ﾌﾟﾛｸﾞﾚｽ表示中
		m_pProgressF3 = new CProgressDlg(4);
		m_pProgressF3->Create();										// ﾀﾞｲｱﾛｸﾞ表示
		on = 0, pn = 0, ln[0] = 0, fn[0] = 0, sop = NULL;				// ﾊﾟｰﾂ情報初期化
		FacNum = 0, ObjNum = 0;
		fscanf_s(fp, "%d%d%d", &flg, &FacNum, &ObjNum);					// 面数･立体数取得
		while(TRUE) {													// EOFまで
			fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100);
			if(feof(fp)) {
				break;
			}
			if(flg<0) {													// <立体終了>
				on++;													// ﾊﾟｰﾂｶｳﾝﾀUP
				pn = 0, ln[on] = 0, fn[on] = 0;							// ﾊﾟｰﾂ情報初期化
			} else {													// <面ﾙｰﾌﾟ>
				pn++;													// 頂点数加算
				if(flg==1) {											// <面ﾙｰﾌﾟ終了>
					ln[on] += pn, pn = 0;
					fn[on]++, cln++;									// 面ﾙｰﾌﾟ数･面数ｾｯﾄ
					dpr = 5.0*((double)cln/(double)FacNum);				// 現在ﾊﾟｰｾﾝﾄ
					if(spr<(int)dpr) {									// <ﾊﾟｰｾﾝﾄUP>
						spr = (int)dpr;
						m_pProgressF3->SetProgress(spr);				// 進捗表示
					}
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// 強制終了
						ret = -1; break;
					}
				}
			}
		}
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
		if(!ret&&ln[0]==0) {											// ﾃﾞｰﾀ無
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		if(fopen_s(&fp, tname, "r")!=0) {								// 作業ﾌｧｲﾙOPEN
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		if(on==0) {														// ﾃﾞｰﾀ有は立体ｶｳﾝﾀを1以上
			on = 1;
		}
		fscanf_s(fp, "%d%d%d", &flg, &FacNum, &ObjNum);					// 面数･立体数取得
		OnCmdFlgStp();													// 未保存ﾌﾗｸﾞ更新中止ON
		for(i=0, cln=0; i<on; i++) {									// ﾊﾟｰﾂ毎に行う
			InitCVtx();													// 頂点ｿｰﾄ初期処理
			for(j=0; j<fn[i]; j++) {									// 面数分行う
				do {	                                                // 頂点座標入力
					if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
						ret = 1; break;
					}
					pa.p[0] = atof(b1);									// 実数変換
					pa.p[1] = atof(b2);
					pa.p[2] = atof(b3);
					SetCVtx(pa);										// ｿｰﾄ頂点設定
				} while(flg==0);
				if(ret) {												// 作業中止
					break;
				}
				SetCFace();												// 面ﾙｰﾌﾟ終了設定
				cln++;
				dpr = 95.0*((double)cln/(double)FacNum)+5.0;			// 現在ﾊﾟｰｾﾝﾄ
				if(spr<(int)dpr) {										// <ﾊﾟｰｾﾝﾄUP>
					spr = (int)dpr;
					m_pProgressF3->SetProgress(spr);					// 進捗表示
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// 強制終了
						ret = -1; break;
					}
				}
			}			                                                // 立体終了入力
			if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
				ret = 1;
			}
			if(ret) {													// 作業中止
				break;
			}
			if((tvn=SortCVtx())>0) {									// 頂点情報のｿｰﾄ&ｶｳﾝﾄ
				CreatObj(tvn, ln[i], 0, &op);							// 立体領域確保
				LevelObj(0, op);										// 同一ﾚﾍﾞﾙ生成立体接続
				SetCVtxObj(op);											// 頂点情報設定
				PutObjAtr(op, 0);										// 属性No設定(DIRECT)
				PutObjPrt(op, 0, 1);									// ﾊﾟｰﾂNo設定(通常)
				PutObjCol(op, 0);										// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
				SetObjBox(op);											// ﾎﾞｯｸｽｻｲｽﾞ設定
				MakeDirectCommand(op);									// 直接生成ｺﾏﾝﾄﾞ生成
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
				if(sop==NULL) {
					sop = op;
				}
			}
		}
		if(!ret&&sop!=NULL) {
			GetObjOif(sop, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			View->SelectOifItem(oif);									// 立体構成情報ｱｲﾃﾑ選択
		}
		OffCmdFlgStp();													// 未保存ﾌﾗｸﾞ更新中止OFF
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
	}
	remove(tname);														// 作業ﾌｧｲﾙ削除
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	if(psw) {															// <ﾌﾟﾛｸﾞﾚｽ表示中>
		m_pProgressF3->DestroyWindow();									// ﾀﾞｲｱﾛｸﾞ廃棄
		delete m_pProgressF3;
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] ln;
	delete[] fn;
	delete[] tname;
	delete[] b1;
	delete[] b2;
	delete[] b3;
	return ret;
}

/*****************************/
int MakePolygonData(int mode)											//<<< ﾎﾟﾘｺﾞﾝﾃﾞｰﾀ作成
/*****************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	int*     ln = new int[MAXPART];
	int*     fn = new int[MAXPART];
	char*    b1 = new char[100];
	char*    b2 = new char[100];
	char*    b3 = new char[100];
	CString  path;
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op, * sop;
	OIFTYPE* oif;
	PNTTYPE  pt;
	VECTYPE  vec;
	int      ret=0, i, j, i1, i2, lp, pn, on, flg, tvn, sln, cln, spr=0;
	double   dpr;

	PolyFlg = 1;														// ﾎﾟﾘｺﾞﾝﾌﾗｸﾞ(1:ﾎﾟﾘｺﾞﾝ作成)
	FacNum = 0, ObjNum = 0;												// 面数･立体数ｸﾘｱ
	path = GetAppPath(0)+TMP_NAME;										// ﾃﾞｰﾀﾌｧｲﾙPATH取得
	if(fopen_s(&fp, path, "w")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] ln;
		delete[] fn;
		delete[] b1;
		delete[] b2;
		delete[] b3;
		return 1;
	}
	m_pProgressF3 = new CProgressDlg(5);
	m_pProgressF3->Create();											// ﾀﾞｲｱﾛｸﾞ表示
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	sln = 0, cln = 0;
	if(!mode) {
		SelBaseObjPtr(1, &op);											// A側立体取得
	} else {
		BaseObjPtr(2, &op, &i1, &i2);									// B側立体取得
	}
	while(op) {
		sln += GetFlpNum1(op);											// 面ﾙｰﾌﾟ数加算
		if(!mode) {
			SelNextObjPtr(1, &op);										// 次A側立体へ
		} else {
			NextObjPtr(2, &op, &i1, &i2);								// 次B側立体へ
		}
	}
	if(!mode) {
		SelBaseObjPtr(1, &op);											// A側立体取得
	} else {
		BaseObjPtr(2, &op, &i1, &i2);									// B側立体取得
	}
	while(op) {
		lp = GetFlpNum1(op);											// 面ﾙｰﾌﾟ数取得
		for(i=0, pn=0; i<lp; i++) {										// 全面を三角形分割
			GetFlp1(op, i, &vno[pn], &i1);								// 面ﾙｰﾌﾟ番号取得
			GetVtx(op, vno[pn], &pnt[pn]);								// 頂点座標取得
			pn++;
			if(i1) {													// <面ﾙｰﾌﾟ終了>
				MakePolygon(pnt, vno, pn, pa, &vec);					// 分割面ﾃﾞｰﾀ作成
				if(CheckPolygon(pa, &pn)) {								// 多角形頂点確認
					DivisionPolygon(fp, op, vec, pa, pn);				// 多角形分割
				}
				pn = 0;
			}
			cln++;
			dpr = 50.0*((double)cln/(double)sln);						// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF3->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
		}
		if(ret) {														// 強制終了時は中止
			break;
		}
		fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);				// 立体終了出力
		ObjNum++;														// 立体数加算
		if(!mode) {
			SelNextObjPtr(1, &op);										// 次A側立体へ
		} else {
			NextObjPtr(2, &op, &i1, &i2);								// 次B側立体へ
		}
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	if(!ret) {															// <正常処理中>
		if(fopen_s(&fp, path, "r")!=0) {								// 作業ﾌｧｲﾙOPEN
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		if(!mode) {
			SelBaseObjPtr(1, &op);										// A側立体取得
			while(op!=NULL) {
				sop = op;												// 削除立体ﾎﾟｲﾝﾀ保存
				SelNextObjPtr(1, &op);									// 次立体へ
				LevelObj(1, sop);										// 同一ﾚﾍﾞﾙ削除立体接続
				TempObj(sop);											// 立体一時削除
			}
		} else {
			Delete();													// B側立体の削除
		}
		cln = 0, on = 0, pn = 0, ln[0] = 0, fn[0] = 0, sop = NULL;		// ﾊﾟｰﾂ情報初期化
		while(TRUE) {													// EOFまで
			fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100);
			if(feof(fp)) {
				break;
			}
			if(flg<0) {													// <立体終了>
				on++;													// ﾊﾟｰﾂｶｳﾝﾀUP
				pn = 0, ln[on] = 0, fn[on] = 0;							// ﾊﾟｰﾂ情報初期化
			} else {													// <面ﾙｰﾌﾟ>
				pn++;													// 頂点数加算
				if(flg==1) {											// <面ﾙｰﾌﾟ終了>
					ln[on] += pn, pn = 0;
					fn[on]++, cln++;									// 面ﾙｰﾌﾟ数･面数ｾｯﾄ
					dpr = 5.0*((double)cln/(double)FacNum)+50.0;		// 現在ﾊﾟｰｾﾝﾄ
					if(spr<(int)dpr) {									// <ﾊﾟｰｾﾝﾄUP>
						spr = (int)dpr;
						m_pProgressF3->SetProgress(spr);				// 進捗表示
					}
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// 強制終了
						ret = -1; break;
					}
				}
			}
		}
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
		if(!ret&&ln[0]==0) {											// ﾃﾞｰﾀ無
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		if(fopen_s(&fp, path, "r")!=0) {								// 作業ﾌｧｲﾙOPEN
			ret = 1;
		}
	}
	if(!ret) {															// <正常処理中>
		if(on==0) {														// ﾃﾞｰﾀ有は立体ｶｳﾝﾀを1以上
			on = 1;
		}
		for(i=0, cln=0; i<on; i++) {									// ﾊﾟｰﾂ毎に行う
			InitCVtx();													// 頂点ｿｰﾄ初期処理
			for(j=0; j<fn[i]; j++) {									// 面数分行う
				do {	                                                // 頂点座標入力
					if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
						ret = 1; break;
					}
					pt.p[0] = atof(b1);									// 実数変換
					pt.p[1] = atof(b2);
					pt.p[2] = atof(b3);
					SetCVtx(pt);										// ｿｰﾄ頂点設定
				} while(flg==0);
				if(ret) {												// 作業中止
					break;
				}
				SetCFace();												// 面ﾙｰﾌﾟ終了設定
				cln++;
				dpr = 45.0*((double)cln/(double)FacNum)+55.0;			// 現在ﾊﾟｰｾﾝﾄ
				if(spr<(int)dpr) {										// <ﾊﾟｰｾﾝﾄUP>
					spr = (int)dpr;
					m_pProgressF3->SetProgress(spr);					// 進捗表示
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// 強制終了
						ret = -1; break;
					}
				}
			}			                                                // 立体終了入力
			if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
				ret = 1;
			}
			if(ret) {													// 作業中止
				break;
			}
			if((tvn=SortCVtx())>0) {									// 頂点情報のｿｰﾄ&ｶｳﾝﾄ
				CreatObj(tvn, ln[i], 0, &op);							// 立体領域確保
				LevelObj(0, op);										// 同一ﾚﾍﾞﾙ生成立体接続
				SetCVtxObj(op);											// 頂点情報設定
				PutObjAtr(op, 0);										// 属性No設定(DIRECT)
				PutObjPrt(op, 0, 1);									// ﾊﾟｰﾂNo設定(通常)
				PutObjCol(op, 0);										// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
				SetObjBox(op);											// ﾎﾞｯｸｽｻｲｽﾞ設定
				MakeDirectCommand(op);									// 直接生成ｺﾏﾝﾄﾞ生成
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
				if(!mode) {
					SetSelNo(op, 1);									// A側図形演算立体登録
				} else {
					OnEdtFlg(1, op);									// B側図形演算立体登録
				}
				if(sop==NULL) {
					sop = op;
				}
			}
		}
		if(!ret&&sop!=NULL) {
			GetObjOif(sop, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			View->SelectOifItem(oif);									// 立体構成情報ｱｲﾃﾑ選択
		}
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
	}
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	m_pProgressF3->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressF3;
	remove(path);														// 作業ﾌｧｲﾙ削除
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	delete[] ln;
	delete[] fn;
	delete[] b1;
	delete[] b2;
	delete[] b3;
	return ret;
}

/********************************/
void ExportSLCFile(CString file)										//<<< ｴｸｽﾎﾟｰﾄSLC
/********************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      ret, i1, i2;
	CString  pname;

	BaseObjPtr(1, &op, &i1, &i2);										// 表示立体ﾎﾟｲﾝﾀ取得
	if(op==NULL) {														// <立体無は中止>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// ﾃﾞｰﾀﾍﾞｰｽ先頭ﾎﾟｲﾝﾀ取得
	pname = GetAppPath(0)+SLCA_NAME;									// ﾃﾞｰﾀ保存ﾌｧｲﾙPATH取得
	ret = MakeSLCFile(op, vtx, flp, file, pname);						// SLCﾌｧｲﾙ作成
	if(ret==1) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ﾌｧｲﾙ出力ｴﾗｰ
	} else if(ret==2) {
		MemErr(IDS_MEMERR5);											// ﾒﾓﾘｴﾗｰ
	} else if(ret<0) {
		AfxMessageBox(IDS_STOP_PROCESS);								// 中止
	}
}
