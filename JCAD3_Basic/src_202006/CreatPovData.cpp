/////////////////////////////////////////////////////////////////////////////
// POV-Rayデータ作成部
// CreatPovData.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <direct.h>
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

#define	MAXFVTX	1000													// 最大面頂点数

typedef	struct {
	int    vno;															// 3次元上の頂点番号
	double x;															// 2次元上のX座標
	double y;															//    〃    Y座標
} VERTEX;

CProgressDlg* m_pProgressP=NULL;										// ﾌﾟﾛｸﾞﾚｽｲﾝｼﾞｹｰﾀｰ

/*********************************************************************/
static void PutTriangle(FILE* fp, int shape, OBJTYPE* op, VERTEX* pa)	//<<< 三角形ﾊﾟｯﾁ出力
/*********************************************************************/
{
	PNTTYPE pt[3];
	VECTYPE vec;
	CString sbuf;
	int     i;

	for(i=0; i<3; i++) {
		GetVtx(op, pa[i].vno, &pt[i]);									// 頂点座標取得
	}
	if(shape==0) {														// <Smooth Triangle>
		CalcNVec(pt, 3, &vec);											// 法線ﾍﾞｸﾄﾙ取得
		SetCNVtx(pt[0], vec);											// ｿｰﾄ頂点設定
		SetCNVtx(pt[1], vec);											// ｿｰﾄ頂点設定
		SetCNVtx(pt[2], vec);											// ｿｰﾄ頂点設定
	} else {															// <Triangle>
		sbuf.LoadString(IDS_STR121);
		fprintf(fp, "%s\n", (LPCTSTR)sbuf);  //##
		sbuf.Format(IDS_STR116, pt[0].p[0], pt[0].p[1], pt[0].p[2],
								pt[1].p[0], pt[1].p[1], pt[1].p[2],
								pt[2].p[0], pt[2].p[1], pt[2].p[2]);
		fprintf(fp, "%s\n\t}\n", (LPCTSTR)sbuf);  //##
	}
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

/************************************************************/
static void DivisionPolygon(FILE* fp, int shape,
                            OBJTYPE* op, VERTEX* pa, int pn)			//<<< 多角形を三角形に分割
/************************************************************/
{
	VERTEX* pa1 = new VERTEX[MAXFVTX];
	int     vm, vf, vb, vs, i, lc, pn1;
	double  fa;

	if(pn==3) {
		PutTriangle(fp, shape, op, pa);									// 三角形ﾊﾟｯﾁ出力
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
			DivisionPolygon(fp, shape, op, pa1, pn1);					// 多角形分割
			MakeDivPolygon(vf, vb, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, shape, op, pa1, pn1);					// 多角形分割
		} else {														// <頂点有の場合>
			MakeDivPolygon(vm, vs, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, shape, op, pa1, pn1);					// 多角形分割
			MakeDivPolygon(vs, vm, pa, pn, pa1, &pn1);					// 新多角形生成
			DivisionPolygon(fp, shape, op, pa1, pn1);					// 多角形分割
		}
	}
	delete[] pa1;
}

/*******************************************************************/
static void MakePolygon(PNTTYPE* pnt, int* vno, int pn, VERTEX* pa)		//<<< 分割面ﾃﾞｰﾀ作成
/*******************************************************************/
{
	VECTYPE vec, rvc;
	double  rot[3][3], th;
	int     i;

	CalcNVec(pnt, pn, &vec);											// 法線ﾍﾞｸﾄﾙ取得
	th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);						// 角度,回転軸取得
	if(th<EPSILON) {
		rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (vec.p[2]<0.0) ? PI : 0.0;
	} else {
		rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (fabs(vec.p[2])>EPSILON) ? atan2(th, vec.p[2]) : PI/2.0;
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

/**************************************************************/
int OutputPovObj(int cno, int ono, CString path, CString name,
				 int shape, int smoth, double angle,
				 double sper, double per)								//<<< POV-Rayｵﾌﾞｼﾞｪｸﾄ出力
/**************************************************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	CString  ofile, sbuf;
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op;
	NVTYPE*  wvp, * wv1, * wv2, * wv3;
	int      ret=0, i, j, lp, pn, sln=0, cln=0, spr=0;
	double   dpr;

	sbuf.Format(IDS_STR118, name, ono);									// ｲﾝｸﾙｰﾄﾞﾌｧｲﾙ名
	ofile = path  + "\\" + sbuf;
	if(fopen_s(&fp, ofile, "w")!=0) {									// 出力ﾌｧｲﾙOpen
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		return 1;
	}
	sbuf.Format(IDS_STR112, ono);
	fprintf(fp, "%s\n", (LPCTSTR)sbuf);											// DECLARE宣言出力
	InitCNVtx();														// 頂点ｿｰﾄ初期処理
	ColorBaseObjPtr(cno, &op);											// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
	while(op) {
		sln += GetFlpNum1(op);											// 面ﾙｰﾌﾟ数加算
		ColorNextObjPtr(cno, &op);										// ｶﾗｰ別次立体ﾎﾟｲﾝﾀ移動
	}
	ColorBaseObjPtr(cno, &op);											// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
	while(op) {
		lp = GetFlpNum1(op);											// 面ﾙｰﾌﾟ数取得
		for(i=0, pn=0; i<lp; i++) {										// 全面を三角形分割
			GetFlp1(op, i, &vno[pn], &j);								// 面ﾙｰﾌﾟ番号取得
			GetVtx(op, vno[pn], &pnt[pn]);								// 頂点座標取得
			pn++;
			if(j) {														// <面ﾙｰﾌﾟ終了>
				MakePolygon(pnt, vno, pn, pa);							// 分割面ﾃﾞｰﾀ作成
				if(CheckPolygon(pa, &pn)) {								// 多角形頂点確認
					DivisionPolygon(fp, shape, op, pa, pn);				// 多角形分割
				}
				pn = 0;
			}
			cln++;
			if(shape==0) {												// <Smooth Triangle>
				dpr = (per/2.0)*((double)cln/(double)sln)+sper;			// 現在ﾊﾟｰｾﾝﾄ
			} else {													// <Triangle>
				dpr = per*((double)cln/(double)sln)+sper;				// 現在ﾊﾟｰｾﾝﾄ
			}
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressP->SetProgress(spr);							// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressP->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressP->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
		}
		if(ret) {														// 強制終了時は中止
			break;
		}
		ColorNextObjPtr(cno, &op);										// ｶﾗｰ別次立体ﾎﾟｲﾝﾀ移動
	}
	if(shape==0) {														// <Smooth Triangle>
		if(SortCNVtx(smoth, angle)) {									// 頂点情報ｿｰﾄ&設定
			cln = 0;
			wvp = NVp;
			while(wvp) {
				wv1 = wvp;												// 1点目
				if((wvp=wvp->next)==NULL) {
					break;
				}
				wv2 = wvp;												// 2点目
				if((wvp=wvp->next)==NULL) {
					break;
				}
				wv3 = wvp;												// 3点目
				wvp = wvp->next;										// 次面へ
				sbuf.LoadString(IDS_STR122);
				fprintf(fp, "%s\n", (LPCTSTR)sbuf);
				sbuf.Format(IDS_STR117, wv1->pt.p[0], wv1->pt.p[1], wv1->pt.p[2],
                                        wv1->vc.p[0], wv1->vc.p[1], wv1->vc.p[2],
                                        wv2->pt.p[0], wv2->pt.p[1], wv2->pt.p[2],
                                        wv2->vc.p[0], wv2->vc.p[1], wv2->vc.p[2],
                                        wv3->pt.p[0], wv3->pt.p[1], wv3->pt.p[2],
                                        wv3->vc.p[0], wv3->vc.p[1], wv3->vc.p[2]);
				fprintf(fp, "%s\n\t}\n", (LPCTSTR)sbuf);
				cln += 3;
				dpr = (per/2.0)*((double)cln/(double)Now_no)+sper+(per/2.0);
				if(spr<(int)dpr) {										// <ﾊﾟｰｾﾝﾄUP>
					spr = (int)dpr;
					m_pProgressP->SetProgress(spr);						// 進捗表示
				}
				if(PeekMessage(&msg, m_pProgressP->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressP->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// 強制終了
						ret = -1; break;
					}
				}
			}
		}
		EndCNVtx();														// 頂点ｿｰﾄ設定終了
	}
	fprintf(fp, "}\n");
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	return ret;
}

/*******************************/
int CreatePovData(CString name)											//<<< POV-Rayﾃﾞｰﾀ作成
/*******************************/
{
	WIN32_FIND_DATA	wfd;
	OBJTYPE* op;
	CString  ifile, opath, ofile, def1, def2, dname, sbuf;
	CString  stex2[POVCMAX], stex3[POVCMAX], stex4[POVCMAX], stex5[POVCMAX];
	int      i, j, cnt, ret, smot, tran, rota, scal;
	int      anim, aniz, aniy, targ, colc, refl, refr;
	int      shape[POVCMAX], itex1[POVCMAX], itex2[POVCMAX];
	int      itex3[POVCMAX], itex4[POVCMAX], itex5[POVCMAX];
	double   angl, trnx, trny, trnz, rotx, roty, rotz, scax, scay, scaz; 
	double   sper, per;
	char     buff[256], buff2[256], ftex1[POVCMAX], ftex2[POVCMAX];

	// パラメータ取得
	GetPovData1(&ifile, &sbuf, &opath, &smot, &angl,
                &tran, &trnx, &trny, &trnz,
                &rota, &rotx, &roty, &rotz,
                &scal, &scax, &scay, &scaz,
                &anim, &aniz, &aniy);									// POV-Rayﾃﾞｰﾀ取得1
	GetPovData2(&def1, &def2, &sbuf, &targ, &colc, &refl, &refr);		// POV-Rayﾃﾞｰﾀ取得2
	GetPovData3(shape, itex1, itex2, itex3, itex4, itex5);				// POV-Rayﾃﾞｰﾀ取得3
	GetColorTex(itex1, itex2, itex3, itex4, itex5,
                ftex1, ftex2, stex2, stex3, stex4, stex5);				// ｶﾗｰ別ﾃｸｽﾁｬ取得
	opath = opath + name;												// 出力ﾌｫﾙﾀﾞ(ﾌﾙﾊﾟｽ)
	ofile = opath + "\\" + name + ".pov";								// 出力ﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
	// 出力フォルダチェック
	if(FindFirstFile(opath, &wfd)==INVALID_HANDLE_VALUE) {				// <ﾌｫﾙﾀﾞ無の場合>
		if(_mkdir(opath)!=0) {											// ﾌｫﾙﾀﾞ作成
			AfxMessageBox(IDS_ERR_MKDIR);								// ｢ﾌｫﾙﾀﾞ作成NG｣
		}
	}
	// ファイルチェック
	std::fstream istrm(ifile, std::ios::in);							// 定義ﾌｧｲﾙOpen
	if(!istrm.rdbuf()->is_open()) {										// 定義ﾌｧｲﾙOpenﾁｪｯｸ
		AfxMessageBox(IDS_ERR_DEFFILE);									// ｢定義ﾌｧｲﾙに異常あり｣
		return 0;
	}
	istrm.getline(buff, sizeof(buff));									// 1行目入力
	sbuf.LoadString(IDS_STR96);										// 正規の1行目ﾃﾞｰﾀ
	i = sbuf.GetLength();
	memset(buff2, 0x0, sizeof(buff2));
	memcpy(buff2, sbuf.GetBuffer(i), i);
	if(strncmp(buff, buff2, i)) {										// <一致しない場合>
		AfxMessageBox(IDS_ERR_DEFFILE);									// ｢定義ﾌｧｲﾙに異常あり｣
		return 0;
	}
	std::fstream ostrm(ofile, std::ios::out);							// 出力ﾌｧｲﾙOpen
	if(!ostrm.rdbuf()->is_open()) {										// 出力ﾌｧｲﾙOpenﾁｪｯｸ
		AfxMessageBox(IDS_ERR_MKFILE);									// ｢ﾌｧｲﾙ作成NG｣
		return 0;
	}
	// POV-Rayファイル(.pov)編集
	m_pProgressP = new CProgressDlg(6);
	m_pProgressP->Create();												// ﾀﾞｲｱﾛｸﾞ表示
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	strcpy_s(buff, "");
	while(istrm.getline(buff, sizeof(buff))) {
		if(!strncmp(buff, "/*", 2)) {									// ｽﾗｯｼｭ+ｱｽﾀﾘｽｸ行は無視
			continue;
		}
		if(!strncmp(buff, "[[[A]]]", 7)) {								// <ｲﾝｸﾙｰﾄﾞ宣言位置>
			if(def1!="") {												// ﾕｰｻﾞ定義ﾌｧｲﾙ名1
				i = def1.ReverseFind('\\');
				j = def1.ReverseFind('.');
				dname = def1.Mid(i+1, j-i-1);							// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(拡張子なし)
				sbuf.Format(IDS_STR111, dname);							// ｲﾝｸﾙｰﾄﾞ宣言
				ostrm<<sbuf;
			}
			if(def2!="") {												// ﾕｰｻﾞ定義ﾌｧｲﾙ名2
				i = def2.ReverseFind('\\');
				j = def2.ReverseFind('.');
				dname = def2.Mid(i+1, j-i-1);							// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(拡張子なし)
				sbuf.Format(IDS_STR111, dname);							// ｲﾝｸﾙｰﾄﾞ宣言
				ostrm<<sbuf;
			}
			for(cnt=0, i=0; i<POVCMAX; i++) {							// 全ｶﾗｰ繰り返す
				ColorBaseObjPtr(i, &op);								// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
				if(op!=NULL) {											// <ｶﾗｰ別の有効立体がある場合>
					sbuf.Format(IDS_STR110, name, cnt);					// ｲﾝｸﾙｰﾄﾞ宣言
					ostrm<<sbuf;
					cnt++;
				}
			}
			if(anim) {													// <ｱﾆﾒｰｼｮﾝあり>
				sbuf.LoadString(IDS_STR109);							// 空行
				ostrm<<sbuf;
				sbuf.LoadString(IDS_STR123);							// ｺﾒﾝﾄ行(説明)
				ostrm<<sbuf;
				sbuf.Format(IDS_STR124, aniz*(aniy+1), aniz*(aniy+1)-1);// ｺﾒﾝﾄ行(ｱﾆﾒｰｼｮﾝｺﾏ数)
				ostrm<<sbuf;
			}
		} else if(!strncmp(buff, "[[[B]]]", 7)) {						// <ｵﾌﾞｼﾞｪｸﾄ情報位置>
			for(cnt=0, i=0; i<POVCMAX; i++) {							// 全ｶﾗｰ繰り返す
				ColorBaseObjPtr(i, &op);								// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
				if(op!=NULL) {											// <ｶﾗｰ別の有効立体がある場合>
					sbuf.Format(IDS_STR119, cnt);						// ｵﾌﾞｼﾞｪｸﾄ情報
					ostrm<<sbuf;
					if(ftex1[i]=='H') {									// <ﾃｸｽﾁｬ材質区分:ｶﾞﾗｽ2>
						sbuf.Format(IDS_STR105, stex3[i]);
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR106, stex4[i], stex5[i]);
						ostrm<<"  "<<sbuf;
					} else if(ftex1[i]=='I') {							// <ﾃｸｽﾁｬ材質区分:ｶﾞﾗｽ3>
						sbuf.Format(IDS_STR107, stex5[i], stex3[i]);
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR108, stex4[i]);
						ostrm<<"  "<<sbuf;
					} else {											// <ﾃｸｽﾁｬ材質区分:その他>
						if(ftex2[i]=='A') {
							sbuf.Format(IDS_STR101, stex2[i]);
						} else if(ftex2[i]=='B') {
							sbuf.Format(IDS_STR102, stex2[i]);
						} else if(ftex2[i]=='C') {
							sbuf.Format(IDS_STR103, stex2[i]);
						} else {
							sbuf.Format(IDS_STR104, stex2[i]);
						}
						ostrm<<"  "<<sbuf;
						if(ftex2[i]=='E') {								// <ﾃｸｽﾁｬﾌｫｰﾏｯﾄ区分:ﾕｰｻﾞ定義2>
							sbuf.LoadString(IDS_STR120);				// photons
							ostrm<<"  "<<sbuf;
							if(targ) {									// target有
								sbuf.LoadString(IDS_STR97);
								ostrm<<"    "<<sbuf;
							}
							if(colc) {									// collect on
								sbuf.Format(IDS_STR98, "on");
							} else {									// collect off
								sbuf.Format(IDS_STR98, "off");
							}
							ostrm<<"    "<<sbuf;
							if(refl) {									// reflection on
								sbuf.Format(IDS_STR99, "on");
							} else {									// reflection off
								sbuf.Format(IDS_STR99, "off");
							}
							ostrm<<"    "<<sbuf;
							if(refr) {									// refraection on
								sbuf.Format(IDS_STR100, "on");
							} else {									// refraction off
								sbuf.Format(IDS_STR100, "off");
							}
							ostrm<<"    "<<sbuf;
							sbuf.LoadString(IDS_STR109);
							ostrm<<"  }"<<sbuf;
						}
					}
					if(tran) {											// <Translateあり>
						sbuf.Format(IDS_STR113, trnx, trny, trnz);
						ostrm<<"  "<<sbuf;
					}
					if(rota) {											// <Rotateあり>
						sbuf.Format(IDS_STR114, rotx, roty, rotz);
						ostrm<<"  "<<sbuf;
					}
					if(scal) {											// <Scaleあり>
						sbuf.Format(IDS_STR115, scax, scay, scaz);
						ostrm<<"  "<<sbuf;
					}
					if(anim) {											// <ｱﾆﾒｰｼｮﾝあり>
						sbuf.Format(IDS_STR125, 360*(aniy+1));
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR126, aniy+1, 180.0/aniy);
						ostrm<<"  "<<sbuf;
					}
					sbuf.LoadString(IDS_STR109);
					ostrm<<"}"<<sbuf;
					cnt++;
				}
			}
		} else {														// <POV-Ray記述>
			sbuf.LoadString(IDS_STR109);
			ostrm<<buff<<sbuf;											// そのままｺﾋﾟｰ
		}
	}
	// ユーザ定義ファイル(.inc)コピー
	if(def1!="") {
		i = def1.ReverseFind('\\');
		dname = def1.Mid(i+1);											// ﾕｰｻﾞ定義ﾌｧｲﾙ名1
		sbuf = opath  + "\\" + dname;
		CopyFile(def1, sbuf, FALSE);									// ﾌｧｲﾙｺﾋﾟｰ
	}
	if(def2!="") {
		i = def2.ReverseFind('\\');
		dname = def2.Mid(i+1);											// ﾕｰｻﾞ定義ﾌｧｲﾙ名2
		sbuf = opath  + "\\" + dname;
		CopyFile(def2, sbuf, FALSE);									// ﾌｧｲﾙｺﾋﾟｰ
	}
	// オブジェクトデータ変換
	per = 100.0/(double)cnt;											// 1立体毎ﾊﾟｰｾﾝﾄ
	for(cnt=0, i=0; i<POVCMAX; i++) {									// 全ｶﾗｰ繰り返す
		ColorBaseObjPtr(i, &op);										// ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
		if(op!=NULL) {													// <ｶﾗｰ別の有効立体がある場合>
			sper = per*cnt;
			ret = OutputPovObj(i, cnt, opath, name,
                               shape[i], smot, angl, sper, per);		// POV-Rayｵﾌﾞｼﾞｪｸﾄ出力
			if(ret!=0) {												// 異常は処理中止
				break;
			}
			cnt++;
		}
	}
	m_pProgressP->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressP;
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	return 0;
}
