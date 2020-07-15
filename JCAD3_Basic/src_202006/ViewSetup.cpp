/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewSetup.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "DataFile1.h"
#include "NumFunc.h"

/***************************************/
void CJcad3GlbView::InitWorkTable(void)									//<<< 作業ﾃｰﾌﾞﾙ初期処理
/***************************************/
{
	m_MaxCCtl = GetCCtlMax();											// 最大曲線制御点数取得
	m_MaxCCtA = GetCCtlAMax();											// 曲線制御点配列数取得
	m_MaxSCtl = GetSCtlMax();											// 最大曲面制御点数取得
	m_MaxSCtA = GetSCtlAMax();											// 曲面制御点配列数取得
	m_MaxWCtl = GetWCtlMax();											// 作業用制御点配列数取得
	m_MaxCPnt = GetCPntMax();											// 最大曲線頂点数取得
	m_MaxSPnt = GetSPntMax();											// 最大曲面頂点数取得
	m_MaxOPnt = GetOPntMax();											// 最大立体頂点数取得
	m_MaxBPnt = GetBPntMax();											// 最大断面頂点数取得
	m_ULineNo = (int *)malloc(sizeof(int)*m_MaxCCtA);
	if(m_ULineNo==NULL) {
		MemErr(IDS_MEMERR5);
	}
	m_VLineNo = (int *)malloc(sizeof(int)*m_MaxCCtA);
	if(m_VLineNo==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spa = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Spa==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spb = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Spb==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spc = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Spc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spd = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Spd==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sps = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Sps==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spt = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxSCtA);
	if(Spt==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spw = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxCPnt);
	if(Spw==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spx = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxWCtl);
	if(Spx==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spy = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxWCtl);
	if(Spy==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Spz = (PNTTYPE *)malloc(sizeof(PNTTYPE)*m_MaxWCtl);
	if(Spz==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Ssa = (SCRTYPE *)malloc(sizeof(SCRTYPE)*m_MaxSCtA);
	if(Ssa==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Ssb = (SCRTYPE *)malloc(sizeof(SCRTYPE)*m_MaxSCtA);
	if(Ssb==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Ssc = (SCRTYPE *)malloc(sizeof(SCRTYPE)*m_MaxCPnt);
	if(Ssc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sva = (VECTYPE *)malloc(sizeof(VECTYPE)*m_MaxSCtA);
	if(Sva==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Svb = (VECTYPE *)malloc(sizeof(VECTYPE)*m_MaxSCtA);
	if(Svb==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sia = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(Sia==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sib = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(Sib==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sic = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(Sic==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sis = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(Sis==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Siw = (int *)malloc(sizeof(int)*m_MaxCPnt);
	if(Siw==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Six = (int *)malloc(sizeof(int)*m_MaxWCtl);
	if(Six==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Siy = (int *)malloc(sizeof(int)*m_MaxWCtl);
	if(Siy==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Siz = (int *)malloc(sizeof(int)*m_MaxWCtl);
	if(Siz==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Tens = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(Tens==NULL) {
		MemErr(IDS_MEMERR5);
	}
	TensC = (int *)malloc(sizeof(int)*m_MaxSCtA);
	if(TensC==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sda = (double *)malloc(sizeof(double)*m_MaxCCtA*m_MaxCCtA);
	if(Sda==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Sdb = (double *)malloc(sizeof(double)*m_MaxCCtA*m_MaxCCtA);
	if(Sdb==NULL) {
		MemErr(IDS_MEMERR5);
	}
}

/***************************************/
void CJcad3GlbView::FreeWorkTable(void)									//<<< 作業ﾃｰﾌﾞﾙ解放処理
/***************************************/
{
	free(m_ULineNo);													// 領域解放
	free(m_VLineNo);
	free(Spa);
	free(Spb);
	free(Spc);
	free(Spd);
	free(Sps);
	free(Spt);
	free(Spw);
	free(Spx);
	free(Spy);
	free(Spz);
	free(Ssa);
	free(Ssb);
	free(Ssc);
	free(Sva);
	free(Svb);
	free(Sia);
	free(Sib);
	free(Sic);
	free(Sis);
	free(Siw);
	free(Six);
	free(Siy);
	free(Siz);
	free(Tens);
	free(TensC);
	free(Sda);
	free(Sdb);
}

/**********************************/
void CJcad3GlbView::InitData(void)										//<<< ﾃﾞｰﾀ初期処理
/**********************************/
{
	PNTTYPE p1, p2;
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	int     i;

	ReadRenderFile();													// ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ読込
	ReadSysDataFile();													// ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ読込
	MainWnd->SetCrtPrm(m_CurveRepeat, m_CurveWeight);					// 生成ﾊﾟﾗﾒｰﾀ設定
	ReadScrInfoFile();													// 画面制御情報保存ﾌｧｲﾙ読込
	ScrToPnt(0, &m_EyeSv, &p1);
	ScrToPnt(0, &m_CentPs, &p2);
	VecV(&p1, &p2, &v1);
	VecN(&v1);
	if(fabs(v1.p[0])<EPSILON&&
       fabs(v1.p[1])<EPSILON&&
       fabs(v1.p[2])-1.0<EPSILON) {
		m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
	} else {
		VecO(&v1, &v2, &m_HzVec);
		VecN(&m_HzVec);
	}
	VecO(&v1, &m_HzVec, &m_VtVec);
	VecN(&m_VtVec);
	for(i=0; i<5; i++) {
		ScrToPnt(0, &m_EyeSv_S[i], &p1);
		ScrToPnt(0, &m_CentPs_S[i], &p2);
		VecV(&p1, &p2, &v1);
		VecN(&v1);
		if(fabs(v1.p[0])<EPSILON&&
           fabs(v1.p[1])<EPSILON&&
           fabs(v1.p[2])-1.0<EPSILON) {
			m_HzVec_S[i].p[0] = 0.0;
			m_HzVec_S[i].p[1] = -1.0;
			m_HzVec_S[i].p[2] = 0.0;
		} else {
			VecO(&v1, &v2, &m_HzVec_S[i]);
			VecN(&m_HzVec_S[i]);
		}
		VecO(&v1, &m_HzVec_S[i], &m_VtVec_S[i]);
		VecN(&m_VtVec_S[i]);
	}
	ReadRayTraceFile();													// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ読込
	ReadPovRayFile();													// POV-Ray保存ﾌｧｲﾙ読込
}

/********************************************/
void CJcad3GlbView::InitRColorFile(BOOL upd)							//<<< 初期ﾚﾝﾀﾞﾘﾝｸﾞｶﾗｰﾌｧｲﾙ読込
/********************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, s, er=0;

	path = GetAppPath(0)+COL_NAME;										// 初期ｶﾗｰﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<18; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			m_CColor[i][0] = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			m_CColor[i][1] = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			m_CColor[i][2] = RGB(r, g, b);
			if(fscanf_s(fp, "%d", &s)!=1) {
				er = 1; break;
			}
			m_Shin[i] = s;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	if(upd) {
		for(i=0; i<18; i++) {
			SetGLColor(i);												// OpenGLｶﾗｰ設定
		}
		Display(0);														// 再描画
	}
}

/*****************************************************/
void CJcad3GlbView::InitRenderFile(int flg, BOOL upd)					//<<< 初期ﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ読込
/*****************************************************/
{
	FILE*   fp;
	CString path;
	int     r, g, b, er=0;

	if(flg!=2) {
		path = GetAppPath(0)+REN_NAME;									// 初期ﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ名
		if(fopen_s(&fp, path, "r")==0) {								// ﾌｧｲﾙOPEN
			if(flg==0) {
				m_MapFile = _T("");
			}
			while (TRUE) {
				if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
					er = 1; break;
				}
				m_RColor[0] = RGB(r, g, b);
				if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
					er = 1; break;
				}
				m_RColor[1] = RGB(r, g, b);
				if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
					er = 1; break;
				}
				m_RColor[2] = RGB(r, g, b);
				break;
			}
			fclose(fp);													// ﾌｧｲﾙCLOSE
		} else {
			er = 1;
		}
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		m_MapFile = _T("");
	}
	if(upd) {
		SetGLRender();													// OpenGLｶﾗｰ設定
		SetGLRenderMap();												// OpenGLﾏｯﾌﾟ設定
		SetRenderMode(2);												// ﾚﾝﾀﾞﾘﾝｸﾞﾓｰﾄﾞ
		Display(0);														// 再描画
	}
}

/*************************************/
void CJcad3GlbView::SaveRndEnv1(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理1
/*************************************/
{
	FILE*   fp;
	CString path;
	int     i, j, r, g, b, er=0;

	path = GetAppPath(0)+RN1_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<18; i++) {
			for(j=0; j<3; j++) {
				r = GetRValue(m_CColor[i][j]);
				g = GetGValue(m_CColor[i][j]);
				b = GetBValue(m_CColor[i][j]);
				if(fprintf(fp, "%d %d %d ", r, g, b)<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d\n", m_Shin[i])<0) {
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
}

/*************************************/
void CJcad3GlbView::SaveRndEnv2(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理2
/*************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+RN2_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<3; i++) {
			r = GetRValue(m_RColor[i]);
			g = GetGValue(m_RColor[i]);
			b = GetBValue(m_RColor[i]);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
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
}

/*************************************/
void CJcad3GlbView::SaveRndEnv3(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理3
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+RN3_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		::GetShortPathName(m_MapFile, spath, _MAX_PATH);				// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		if(spath[0]==0x00) {
			::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
		}
		if(fprintf(fp, "%s\n", spath)<0) {
			er = 1;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	delete[] spath;
}

/****************************************/
void CJcad3GlbView::RestoreRndEnv1(void)								//<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理1
/****************************************/
{
	FILE*   fp;
	CString path;
	int     i, j, r, g, b, s, er=0;

	path = GetAppPath(0)+RN1_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<18; i++) {
			for(j=0; j<3; j++) {
				if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
					er = 1; break;
				}
				if(r<0||r>255||g<0||g>255||b<0||b>255) {
					er = 1; break;										// 異常ﾃﾞｰﾀは中止
				}
				m_CColor[i][j] = RGB(r, g, b);							// ｶﾗｰの書き換え
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d", &s)!=1) {
				er = 1; break;
			}
			m_Shin[i] = s;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ｢保存ﾌｧｲﾙなし｣
	}
	for(i=0; i<18; i++) {
		SetGLColor(i);													// OpenGLｶﾗｰ設定
	}
	Display(0);															// 再描画
}

/****************************************/
void CJcad3GlbView::RestoreRndEnv2(void)								//<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理2
/****************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+RN2_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<3; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// 異常ﾃﾞｰﾀは中止
			}
			m_RColor[i] = RGB(r, g, b);									// ｶﾗｰの書き換え
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ｢保存ﾌｧｲﾙなし｣
	}
	SetGLRender();														// OpenGLｶﾗｰ設定
	SetGLRenderMap();													// OpenGLﾏｯﾌﾟ設定
	SetRenderMode(2);													// ﾚﾝﾀﾞﾘﾝｸﾞﾓｰﾄﾞ
	Display(0);															// 再描画
}

/****************************************/
void CJcad3GlbView::RestoreRndEnv3(void)                                //<<< ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理3
/****************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+RN3_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN

		//202006  for blank in path
		fgets(spath, _MAX_PATH, fp);  //202006
		spath[strlen(spath) - 1] = 0; //202006
		if (strlen(spath)) {  //202006
		//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
			memset(lpath, NULL, _MAX_PATH);
			::GetLongPathName(spath, lpath, _MAX_PATH);					// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			m_MapFile.Format("%s", lpath);
			if(m_MapFile=="") {
				m_MapFile = GetAppPath(0);
			}
		} else {
			er = 1;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ｢保存ﾌｧｲﾙなし｣
	}
	SetGLRender();														// OpenGLｶﾗｰ設定
	SetGLRenderMap();													// OpenGLﾏｯﾌﾟ設定
	SetRenderMode(2);													// ﾚﾝﾀﾞﾘﾝｸﾞﾓｰﾄﾞ
	Display(0);															// 再描画
	delete[] spath;
	delete[] lpath;
}

/****************************************/
void CJcad3GlbView::ReadRenderFile(void)								//<<< ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ読込
/****************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString	path;
	int		i, j, r, g, b, s, er=0;

	path = GetAppPath(0)+RDS_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<3; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// 異常ﾃﾞｰﾀは中止
			}
			m_RColor[i] = RGB(r, g, b);									// ｶﾗｰの書き換え
		}
		if(!er) {
			for(i=0; i<18; i++) {
				for(j=0; j<3; j++) {
					if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
						er = 1; break;
					}
					if(r<0||r>255||g<0||g>255||b<0||b>255) {
						er = 1; break;									// 異常ﾃﾞｰﾀは中止
					}
					m_CColor[i][j] = RGB(r, g, b);						// ｶﾗｰの書き換え
				}
				if(er) {
					break;
				}
				if(fscanf_s(fp, "%d", &s)!=1) {
					er = 1; break;
				}
				m_Shin[i] = s;
			}
			if(!er) {
				//202006  for blank in path
				fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
				fgets(spath, _MAX_PATH, fp);  //202006
				spath[strlen(spath) - 1] = 0; //202006
				if (strlen(spath)) {  //202006
				//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
					memset(lpath, NULL, _MAX_PATH);
					::GetLongPathName(spath, lpath, _MAX_PATH);			// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
					m_MapFile.Format("%s", lpath);
					if(m_MapFile=="") {
						m_MapFile = GetAppPath(0);
					}
				} else {
					er = 1;
				}
			}
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	}
	delete[] spath;
	delete[] lpath;
}

/*****************************************/
void CJcad3GlbView::WriteRenderFile(void)								//<<< ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ書込
/*****************************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString	path;
	int		i, j, r, g, b, er=0;

	path = GetAppPath(0)+RDS_NAME;										// ﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		for(i=0; i<3; i++) {
			r = GetRValue(m_RColor[i]);
			g = GetGValue(m_RColor[i]);
			b = GetBValue(m_RColor[i]);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
		}
		for(i=0; i<18; i++) {
			for(j=0; j<3; j++) {
				r = GetRValue(m_CColor[i][j]);
				g = GetGValue(m_CColor[i][j]);
				b = GetBValue(m_CColor[i][j]);
				if(fprintf(fp, "%d %d %d ", r, g, b)<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d\n", m_Shin[i])<0) {
				er = 1; break;
			}
		}
		::GetShortPathName(m_MapFile, spath, _MAX_PATH);				// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		if(spath[0]==0x00) {
			::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
		}
		if(fprintf(fp, "%s\n", spath)<0) {
			er = 1;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	delete[] spath;
}

/*****************************************/
void CJcad3GlbView::ReadSysDataFile(void)								//<<< ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ読込
/*****************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+DTS_NAME;										// ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &m_PrtMode,
                                      &m_PrtColor,
                                      &m_PrtOut)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &m_PrtPos,
                                    &m_PrtSize)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_CapPlane,
                                      &m_CapPos,
                                      &m_CapKind)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_CapWidth,
                                      &m_CapHeight,
                                      &m_CapJComp)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_KeyEnv,
                                      &m_AtrEnv,
                                      &m_SecEnv)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_ShpEnv,
                                      &m_ShdEnv,
                                      &m_ScrEnv1)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_ScrEnv2,
                                      &m_BtnEnv,
                                      &m_SState)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SDataUse,
                                      &m_SNumDisp,
                                      &m_SScrScroll)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SEyeChange,
                                      &m_SOifTree,
                                      &m_SFuncDsp)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SFuncDsp2,
                                      &m_SFuncMode,
                                      &m_DispMode)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%f", &m_CurveRepeat, &fd1)!=2) {
				er = 1; break;
			}
			m_CurveWeight = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d%d", &m_SecBase1a,
                                      &m_SecBase1b,
                                      &m_SecBase2)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SecBase3a,
                                      &m_SecBase3b,
                                      &m_SecBase3c)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SecBase4,
                                      &m_SweepDivi,
                                      &m_RevolveDivi)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%f%f", &m_RingKind, &fd1, &fd2)!=3) {
				er = 1; break;
			}
			m_RingSize1 = ChangeFltToDbl(fd1);
			m_RingSize2 = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_RingSpace1 = ChangeFltToDbl(fd1);
			m_RingSpace2 = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_CurveAngle  = ChangeFltToDbl(fd1);
			m_CurveRadius = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_MoveAdd   = ChangeFltToDbl(fd1);
			m_ScaleAdd1 = ChangeFltToDbl(fd2);
			m_ScaleAdd2 = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_RotateAdd1 = ChangeFltToDbl(fd1);
			m_RotateAdd2 = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_BendAdd   = ChangeFltToDbl(fd1);
			m_LTwistAdd = ChangeFltToDbl(fd2);
			m_LScaleAdd = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_TwistAdd   = ChangeFltToDbl(fd1);
			m_LRotateAdd = ChangeFltToDbl(fd2);
			m_ShearAdd   = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_ArangAdd1 = ChangeFltToDbl(fd1);
			m_ArangAdd2 = ChangeFltToDbl(fd2);
			m_ArangAdd3 = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_CurveAdd = ChangeFltToDbl(fd1);
			m_SweepAdd = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_ECurveAdd = ChangeFltToDbl(fd1);
			m_PMoveAdd  = ChangeFltToDbl(fd2);
			m_LMoveAdd  = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_SMoveAdd = ChangeFltToDbl(fd1);
			m_BMoveAdd = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_ThickValue = ChangeFltToDbl(fd1);
			m_RevolveTh  = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_EarthAdd1 = ChangeFltToDbl(fd1);
			m_EarthAdd2 = ChangeFltToDbl(fd2);
			m_EarthAdd3 = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &m_ScaleDir,
                                      &m_ScalePnt,
                                      &m_ProngNum)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_ProngLn = ChangeFltToDbl(fd1);
			m_ProngDm = ChangeFltToDbl(fd2);
			m_ProngRv = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%d", &fd1, &m_ArrangeNum)!=2) {
				er = 1; break;
			}
			m_ArrangeTh = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d", &m_LinePNum, &m_CirclePNum)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_CircleSize1 = ChangeFltToDbl(fd1);
			m_CircleSize2 = ChangeFltToDbl(fd2);
			m_PastePos    = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &m_PyramidDivi,
                                      &m_InstPNum,
                                      &m_SymRDivi)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%d%d", &fd1, &m_PixelSize1,
                                            &m_PixelSize2)!=3) {
				er = 1; break;
			}
			m_BaseZPos = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d", &m_ObjCopyNum,
                                    &m_ObjSynFlg)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_ObjXScale = ChangeFltToDbl(fd1);
			m_ObjYScale = ChangeFltToDbl(fd2);
			m_ObjZScale = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &m_ObjXPoint,
                                      &m_ObjYPoint,
                                      &m_ObjZPoint)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_SPDiameter = ChangeFltToDbl(fd1);
			m_SPHeight   = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%d%d%d", &m_SPCtrlNum,
                                      &m_SPSprlNum,
                                      &m_TensionNo)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &m_FontNo,
                                    &m_FontCheck)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_TextHeight = ChangeFltToDbl(fd1);
			m_TextDepth  = ChangeFltToDbl(fd2);
			m_TextIntval = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_PrimXSize2D = ChangeFltToDbl(fd1);
			m_PrimYSize2D = ChangeFltToDbl(fd2);
			m_PrimZSize2D = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_PrimXSize3D = ChangeFltToDbl(fd1);
			m_PrimYSize3D = ChangeFltToDbl(fd2);
			m_PrimZSize3D = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_JwlRndSize  = ChangeFltToDbl(fd1);
			m_JwlSqrSize1 = ChangeFltToDbl(fd2);
			m_JwlSqrSize2 = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f", &fd1, &fd2)!=2) {
				er = 1; break;
			}
			m_GemRndSize  = ChangeFltToDbl(fd1);
			m_GemRndHeigt = ChangeFltToDbl(fd2);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_GemSqrSize1 = ChangeFltToDbl(fd1);
			m_GemSqrSize2 = ChangeFltToDbl(fd2);
			m_GemSqrHeigt = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &m_CurveDvd,
                                      &m_PolygonF,
                                      &m_FileBUpF)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_SaveFileMax,
                                      &m_PCutCnt,
                                      &m_BoolCnt)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_XLmt1,
                                      &m_YLmt1,
                                      &m_ZLmt1)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_XLmt2,
                                      &m_YLmt2,
                                      &m_ZLmt2)!=3) {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_PrtPath.Format("%s", lpath);
				if(m_PrtPath=="") {
					m_PrtPath = GetAppPath(0);
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
				m_TmpltPath.Format("%s", lpath);
				if(m_TmpltPath=="") {
					m_TmpltPath = GetAppPath(0);
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
				m_TexPath.Format("%s", lpath);
				if(m_TexPath=="") {
					m_TexPath = GetAppPath(0);
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
				m_MapFilePath.Format("%s", lpath);
				if(m_MapFilePath=="") {
					m_MapFilePath = GetAppPath(0);
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
				m_NCFilePath.Format("%s", lpath);
				if(m_NCFilePath=="") {
					m_NCFilePath = GetAppPath(0);
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
				m_SaveFilePath.Format("%s", lpath);
				if(m_SaveFilePath=="") {
					m_SaveFilePath = GetAppPath(0);
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
		m_PrtMode     = 0,     m_PrtColor    = 0,    m_PrtOut      = 0;
		m_PrtPos      = 0,     m_PrtSize     = 100;
		m_CapPlane    = 3,     m_CapPos      = 0,    m_CapKind     = 0;
		m_CapWidth    = 100,   m_CapHeight   = 100,  m_CapJComp    = 75;
		m_KeyEnv      = 20,    m_AtrEnv      = 1,    m_SecEnv      = 1;
		m_ShpEnv      = 0,     m_ShdEnv      = 0,    m_ScrEnv1     = 0;
		m_ScrEnv2     = 0,     m_BtnEnv      = 0,    m_SState      = 1;
		m_SDataUse    = 1,     m_SNumDisp    = 0,    m_SScrScroll  = 1;
		m_SEyeChange  = 1,     m_SOifTree    = 1,    m_SFuncDsp    = 0;
		m_SFuncDsp2   = 0,     m_SFuncMode   = 6,    m_DispMode    = 0;
		m_CurveRepeat = 3,     m_CurveWeight = 2.0;					//202006 rep 4->3 
		m_SecBase1a   = 4,     m_SecBase1b   = 1,    m_SecBase2    = 4;
		m_SecBase3a   = 3,     m_SecBase3b   = 1,    m_SecBase3c   = 0;
		m_SecBase4    = 1,     m_SweepDivi   = 0,    m_RevolveDivi = 8;
		m_RingKind    = 0,     m_RingSize1   = 12.0, m_RingSize2   = 18.0;
		m_RingSpace1  = 0.0,   m_RingSpace2  = 0.0;
		m_CurveAngle  = 0.0,   m_CurveRadius = 0.0;
		m_MoveAdd     = 0.5,   m_ScaleAdd1   = 0.5,  m_ScaleAdd2   = 0.1;
		m_RotateAdd1  = 5.0,   m_RotateAdd2  = 0.5;
		m_BendAdd     = 5.0,   m_LTwistAdd   = 5.0,  m_LScaleAdd   = 0.1;
		m_TwistAdd    = 5.0,   m_LRotateAdd  = 5.0,  m_ShearAdd    = 0.5;
		m_ArangAdd1   = 0.5,   m_ArangAdd2   = 5.0,  m_ArangAdd3   = 5.0;
		m_CurveAdd    = 0.5,   m_SweepAdd    = 0.5;
		m_ECurveAdd   = 0.5,   m_PMoveAdd    = 0.5,  m_LMoveAdd    = 0.5;
		m_SMoveAdd    = 0.5,   m_BMoveAdd    = 0.5;
		m_ThickValue  = 0.0,   m_RevolveTh   = 360.0;
		m_EarthAdd1   = 0.5,   m_EarthAdd2   = 5.0,  m_EarthAdd3   = 0.05;
		m_ScaleDir    = 0,     m_ScalePnt    = 1,    m_ProngNum    = 4;
		m_ProngLn     = 1.0,   m_ProngDm     = 0.5,  m_ProngRv     = 0.0;
		m_ArrangeTh   = 360.0, m_ArrangeNum  = 4;
		m_LinePNum    = 5,     m_CirclePNum  = 8;
		m_CircleSize1 = 10.0,  m_CircleSize2 = 10.0, m_PastePos    = 0.0;
		m_PyramidDivi = 1,     m_InstPNum    = 1,    m_SymRDivi    = 2;
		m_BaseZPos    = 0.0,   m_PixelSize1  = 350,  m_PixelSize2  = 300;
		m_ObjCopyNum  = 1,     m_ObjSynFlg   = 0;
		m_ObjXScale   = 1.0,   m_ObjYScale   = 1.0,  m_ObjZScale   = 1.0;
		m_ObjXPoint   = 1,     m_ObjYPoint   = 1,    m_ObjZPoint   = 1;
		m_SPDiameter  = 10.0,  m_SPHeight    = 4.0;
		m_SPCtrlNum   = 8,     m_SPSprlNum   = 4,    m_TensionNo   = 0;
		m_FontNo      = 0,     m_FontCheck   = 0;
		m_TextHeight  = 3.0,   m_TextDepth   = 1.0,  m_TextIntval  = 0.5;
		m_PrimXSize2D = 10.0,  m_PrimYSize2D = 10.0, m_PrimZSize2D = 0.0;
		m_PrimXSize3D = 10.0,  m_PrimYSize3D = 10.0, m_PrimZSize3D = 10.0;
		m_JwlRndSize  = 10.0,  m_JwlSqrSize1 = 10.0, m_JwlSqrSize2 = 10.0;
		m_GemRndSize  = 10.0,  m_GemRndHeigt = 0.0;
		m_GemSqrSize1 = 10.0,  m_GemSqrSize2 = 10.0, m_GemSqrHeigt = 0.0;
		m_CurveDvd    = 0,     m_PolygonF    = 0,    m_FileBUpF    = 0;
		m_SaveFileMax = 50,    m_PCutCnt     = 0,    m_BoolCnt     = 0;
        m_XLmt1       = 1,     m_YLmt1       = 1,    m_ZLmt1       = 1;
        m_XLmt2       = 1,     m_YLmt2       = 1,    m_ZLmt2       = 1;
		m_PrtPath     = GetAppPath(0);
		m_TmpltPath   = GetAppPath(0);
		m_TexPath     = GetAppPath(0);
		m_MapFilePath = GetAppPath(0);
		m_NCFilePath  = GetAppPath(0);
		m_SaveFilePath= GetAppPath(0);
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
	delete[] lpath;
}

/******************************************/
void CJcad3GlbView::WriteSysDataFile(void)								//<<< ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ書込
/******************************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+DTS_NAME;										// ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fprintf(fp, "%d %d %d\n", m_PrtMode,
                                         m_PrtColor,
                                         m_PrtOut)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", m_PrtPos,
                                      m_PrtSize)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_CapPlane,
                                         m_CapPos,
                                         m_CapKind)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_CapWidth,
                                         m_CapHeight,
                                         m_CapJComp)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_KeyEnv,
                                         m_AtrEnv,
                                         m_SecEnv)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_ShpEnv,
                                         m_ShdEnv,
                                         m_ScrEnv1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_ScrEnv2,
                                         m_BtnEnv,
                                         m_SState?0:1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SDataUse?0:1,
                                         m_SNumDisp,
                                         m_SScrScroll?0:1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SEyeChange?0:1,
                                         m_SOifTree?0:1,
                                         m_SFuncDsp)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SFuncDsp2,
                                         m_SFuncMode,
                                         m_DispMode)<0) {
				er = 1; break;
			}
			fd1 = (float)m_CurveWeight;
			if(fprintf(fp, "%d %f\n", m_CurveRepeat, fd1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SecBase1a,
                                         m_SecBase1b,
                                         m_SecBase2)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SecBase3a,
                                         m_SecBase3b,
                                         m_SecBase3c)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SecBase4,
                                         m_SweepDivi,
                                         m_RevolveDivi)<0) {
				er = 1; break;
			}
			fd1 = (float)m_RingSize1;
			fd2 = (float)m_RingSize2;
			if(fprintf(fp, "%d %f %f\n", m_RingKind, fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_RingSpace1;
			fd2 = (float)m_RingSpace2;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_CurveAngle;
			fd2 = (float)m_CurveRadius;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_MoveAdd;
			fd2 = (float)m_ScaleAdd1;
			fd3 = (float)m_ScaleAdd2;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_RotateAdd1;
			fd2 = (float)m_RotateAdd2;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_BendAdd;
			fd2 = (float)m_LTwistAdd;
			fd3 = (float)m_LScaleAdd;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_TwistAdd;
			fd2 = (float)m_LRotateAdd;
			fd3 = (float)m_ShearAdd;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ArangAdd1;
			fd2 = (float)m_ArangAdd2;
			fd3 = (float)m_ArangAdd3;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_CurveAdd;
			fd2 = (float)m_SweepAdd;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ECurveAdd;
			fd2 = (float)m_PMoveAdd;
			fd3 = (float)m_LMoveAdd;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_SMoveAdd;
			fd2 = (float)m_BMoveAdd;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ThickValue;
			fd2 = (float)m_RevolveTh;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_EarthAdd1;
			fd2 = (float)m_EarthAdd2;
			fd3 = (float)m_EarthAdd3;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_ScaleDir,
                                         m_ScalePnt,
                                         m_ProngNum)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ProngLn;
			fd2 = (float)m_ProngDm;
			fd3 = (float)m_ProngRv;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ArrangeTh;
			if(fprintf(fp, "%f %d\n", fd1, m_ArrangeNum)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", m_LinePNum, m_CirclePNum)<0) {
				er = 1; break;
			}
			fd1 = (float)m_CircleSize1;
			fd2 = (float)m_CircleSize2;
			fd3 = (float)m_PastePos;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_PyramidDivi,
                                         m_InstPNum,
                                         m_SymRDivi)<0) {
				er = 1; break;
			}
			fd1 = (float)m_BaseZPos;
			if(fprintf(fp, "%f %d %d\n", fd1, m_PixelSize1,
                                              m_PixelSize2)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", m_ObjCopyNum,
                                      m_ObjSynFlg)<0) {
				er = 1; break;
			}
			fd1 = (float)m_ObjXScale;
			fd2 = (float)m_ObjYScale;
			fd3 = (float)m_ObjZScale;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_ObjXPoint,
                                         m_ObjYPoint,
                                         m_ObjZPoint)<0) {
				er = 1; break;
			}
			fd1 = (float)m_SPDiameter;
			fd2 = (float)m_SPHeight;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SPCtrlNum,
                                         m_SPSprlNum,
                                         m_TensionNo)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", m_FontNo,
                                      m_FontCheck)<0) {
				er = 1; break;
			}
			fd1 = (float)m_TextHeight;
			fd2 = (float)m_TextDepth;
			fd3 = (float)m_TextIntval;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_PrimXSize2D;
			fd2 = (float)m_PrimYSize2D;
			fd3 = (float)m_PrimZSize2D;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_PrimXSize3D;
			fd2 = (float)m_PrimYSize3D;
			fd3 = (float)m_PrimZSize3D;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_JwlRndSize;
			fd2 = (float)m_JwlSqrSize1;
			fd3 = (float)m_JwlSqrSize2;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_GemRndSize;
			fd2 = (float)m_GemRndHeigt;
			if(fprintf(fp, "%f %f\n", fd1, fd2)<0) {
				er = 1; break;
			}
			fd1 = (float)m_GemSqrSize1;
			fd2 = (float)m_GemSqrSize2;
			fd3 = (float)m_GemSqrHeigt;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_CurveDvd,
                                         m_PolygonF,
                                         m_FileBUpF)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_SaveFileMax,
                                         m_PCutCnt,
                                         m_BoolCnt)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_XLmt1,
                                         m_YLmt1,
                                         m_ZLmt1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_XLmt2,
                                         m_YLmt2,
                                         m_ZLmt2)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_PrtPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_TmpltPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_TexPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_MapFilePath, spath, _MAX_PATH);		// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_NCFilePath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_SaveFilePath, spath, _MAX_PATH);		// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
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

/*****************************************/
void CJcad3GlbView::ReadScrInfoFile(void)								//<<< 画面制御情報保存ﾌｧｲﾙ読込
/*****************************************/
{
	char*   buf = new char[_MAX_PATH+4];
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     i, j, k, er=0;
	float   f1, f2, f3;

	m_ExactRev = 1.0;
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+SIS_NAME;										// 画面制御情報保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &m_Cent3D.p[0],
                                      &m_Cent3D.p[1],
                                      &m_Cent3D.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_Cent3D_S[i].p[0],
                                          &m_Cent3D_S[i].p[1],
                                          &m_Cent3D_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Fovy,
                                      &m_Fovy_S[0],
                                      &m_Fovy_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Fovy_S[2],
                                      &m_Fovy_S[3],
                                      &m_Fovy_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_CentPs.p[0],
                                      &m_CentPs.p[1],
                                      &m_CentPs.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_CentPs_S[i].p[0],
                                          &m_CentPs_S[i].p[1],
                                          &m_CentPs_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Eye.p[0],
                                      &m_Eye.p[1],
                                      &m_Eye.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_Eye_S[i].p[0],
                                          &m_Eye_S[i].p[1],
                                          &m_Eye_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_EyeSv.p[0],
                                      &m_EyeSv.p[1],
                                      &m_EyeSv.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_EyeSv_S[i].p[0],
                                          &m_EyeSv_S[i].p[1],
                                          &m_EyeSv_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			m_TruckLen      = ChangeFltToDbl(f1);
			m_TruckLen_S[0] = ChangeFltToDbl(f2);
			m_TruckLen_S[1] = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			m_TruckLen_S[2] = ChangeFltToDbl(f1);
			m_TruckLen_S[3] = ChangeFltToDbl(f2);
			m_TruckLen_S[4] = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f", &f1, &f2)!=2) {
				er = 1; break;
			}
			ScrDiv1 = ChangeFltToDbl(f1);
			ScrDiv2 = ChangeFltToDbl(f2);
			if(fscanf_s(fp, "%d%d%d", &m_GridDisp,
                                      &m_GridDisp_S[0],
                                      &m_GridDisp_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridDisp_S[2],
                                      &m_GridDisp_S[3],
                                      &m_GridDisp_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridBind,
                                      &m_GridBind_S[0],
                                      &m_GridBind_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridBind_S[2],
                                      &m_GridBind_S[3],
                                      &m_GridBind_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridSpace,
                                      &m_GridSpace_S[0],
                                      &m_GridSpace_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridSpace_S[2],
                                      &m_GridSpace_S[3],
                                      &m_GridSpace_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridIntvl,
                                      &m_GridIntvl_S[0],
                                      &m_GridIntvl_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GridIntvl_S[2],
                                      &m_GridIntvl_S[3],
                                      &m_GridIntvl_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Bpoint.p[0],
                                      &m_Bpoint.p[1],
                                      &m_Bpoint.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_Bpoint_S[i].p[0],
                                          &m_Bpoint_S[i].p[1],
                                          &m_Bpoint_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Blspnt.p[0],
                                      &m_Blspnt.p[1],
                                      &m_Blspnt.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_Blspnt_S[i].p[0],
                                          &m_Blspnt_S[i].p[1],
                                          &m_Blspnt_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_Blepnt.p[0],
                                      &m_Blepnt.p[1],
                                      &m_Blepnt.p[2])!=3) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_Blepnt_S[i].p[0],
                                          &m_Blepnt_S[i].p[1],
                                          &m_Blepnt_S[i].p[2])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BpDisp,
                                      &m_BpDisp_S[0],
                                      &m_BpDisp_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BpDisp_S[2],
                                      &m_BpDisp_S[3],
                                      &m_BpDisp_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BpBind,
                                      &m_BpBind_S[0],
                                      &m_BpBind_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BpBind_S[2],
                                      &m_BpBind_S[3],
                                      &m_BpBind_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BlDisp,
                                      &m_BlDisp_S[0],
                                      &m_BlDisp_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BlDisp_S[2],
                                      &m_BlDisp_S[3],
                                      &m_BlDisp_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BlBind,
                                      &m_BlBind_S[0],
                                      &m_BlBind_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_BlBind_S[2],
                                      &m_BlBind_S[3],
                                      &m_BlBind_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GlbDisp,
                                      &m_GlbDisp_S[0],
                                      &m_GlbDisp_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GlbDisp_S[2],
                                      &m_GlbDisp_S[3],
                                      &m_GlbDisp_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GlbChange,
                                      &m_GlbChange_S[0],
                                      &m_GlbChange_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_GlbChange_S[2],
                                      &m_GlbChange_S[3],
                                      &m_GlbChange_S[4])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_DspTmpNo,
                                      &m_DspTmpNo_S[0],
                                      &m_DspTmpNo_S[1])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_DspTmpNo_S[2],
                                      &m_DspTmpNo_S[3],
                                      &m_DspTmpNo_S[4])!=3) {
				er = 1; break;
			}
			fgets(buf, _MAX_PATH + 4, fp);  //202006 preread CR/LF
			for(i=0; i<8; i++) {
				for(j=0; j<3; j++) {
					memset(buf, NULL, _MAX_PATH + 4);
					//202006  for blank in path
					fgets(buf, _MAX_PATH+4, fp);  //202006
					buf[strlen(buf) - 1] = 0; //202006
					if (strlen(buf)) {  //202006
					//if(fscanf_s(fp, "%s", buf, _MAX_PATH+4)==1) {  //202006 delete
						if(buf[4]!=NULL){
							memcpy(spath, &buf[4], _MAX_PATH);
							memset(lpath, NULL, _MAX_PATH);
							::GetLongPathName(spath, lpath, _MAX_PATH);	// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
							m_TempPath[i][j].Format("%s", lpath);
						} else {
							m_TempPath[i][j] = _T("");
						}
					} else {
						er = 1; break;
					}
				}
				if(er) {
					break;
				}
			}
			if(er) {
				break;
			}
			for(i=0; i<5; i++) {
				for(j=0; j<8; j++) {
					for(k=0; k<3; k++) {
						memset(buf, NULL, _MAX_PATH+4);
						//202006  for blank in path
						fgets(buf, _MAX_PATH + 4, fp);  //202006
						buf[strlen(buf) - 1] = 0; //202006
						if (strlen(buf)) {  //202006
						//if(fscanf_s(fp, "%s", buf, _MAX_PATH+4)==1) {  //202006 delete
							if(buf[4]!=NULL){
								memcpy(spath, &buf[4], _MAX_PATH);
								memset(lpath, NULL, _MAX_PATH);
								::GetLongPathName(spath, lpath, _MAX_PATH); // ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
								m_TempPath_S[i][j][k].Format("%s", lpath);
							} else {
								m_TempPath_S[i][j][k] = _T("");
							}
						} else {
							er = 1; break;
						}
					}
					if(er) {
						break;
					}
				}
				if(er) {
					break;
				}
			}
			if(er) {
				break;
			}
			for(i=0; i<3; i++) {
				memset(buf, NULL, _MAX_PATH+4);
				//202006  for blank in path
				fgets(buf, _MAX_PATH + 4, fp);  //202006
				buf[strlen(buf) - 1] = 0; //202006
				if (strlen(buf)) {  //202006
				//if(fscanf_s(fp, "%s", buf, _MAX_PATH+4)==1) {  //202006 delete
					if(buf[2]!=NULL){
						memcpy(spath, &buf[2], _MAX_PATH);
						memset(lpath, NULL, _MAX_PATH);
						::GetLongPathName(spath, lpath, _MAX_PATH);		// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
						m_TexFile[i].Format("%s", lpath);
					} else {
						m_TexFile[i] = _T("");
					}
				} else {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_TexFovy[0],
                                      &m_TexFovy[1],
                                      &m_TexFovy[2])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &m_TexDisp[0],
                                      &m_TexDisp[1],
                                      &m_TexDisp[2])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			m_TexMove1[0] = ChangeFltToDbl(f1);
			m_TexMove1[1] = ChangeFltToDbl(f2);
			m_TexMove1[2] = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			m_TexMove2[0] = ChangeFltToDbl(f1);
			m_TexMove2[1] = ChangeFltToDbl(f2);
			m_TexMove2[2] = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%d%d%d", &m_TexRotate[0],
                                      &m_TexRotate[1],
                                      &m_TexRotate[2])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &m_TexScale[0],
                                      &m_TexScale[1],
                                      &m_TexScale[2])!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f", &f1)!=1) {
				er = 1; break;
			}
			m_ExactRev = ChangeFltToDbl(f1);
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
		m_Cent3D.p[0]     = SCT_X, m_Cent3D.p[1]     = SCT_Y, m_Cent3D.p[2]     = SCT_Z;
		m_Cent3D_S[0].p[0]= SCT_X, m_Cent3D_S[0].p[1]= SCT_Y, m_Cent3D_S[0].p[2]= SCT_Z;
		m_Cent3D_S[1].p[0]= SCT_X, m_Cent3D_S[1].p[1]= SCT_Y, m_Cent3D_S[1].p[2]= SCT_Z;
		m_Cent3D_S[2].p[0]= SCT_X, m_Cent3D_S[2].p[1]= SCT_Y, m_Cent3D_S[2].p[2]= SCT_Z;
		m_Cent3D_S[3].p[0]= SCT_X, m_Cent3D_S[3].p[1]= SCT_Y, m_Cent3D_S[3].p[2]= SCT_Z;
		m_Cent3D_S[4].p[0]= SCT_X, m_Cent3D_S[4].p[1]= SCT_Y, m_Cent3D_S[4].p[2]= SCT_Z;
		m_Fovy            = FOVY,  m_Fovy_S[0]       = FOVY,  m_Fovy_S[1]       = FOVY;
		m_Fovy_S[2]       = FOVY,  m_Fovy_S[3]       = FOVY,  m_Fovy_S[4]       = FOVY;
		m_CentPs.p[0]     = SCT_X, m_CentPs.p[1]     = SCT_Y, m_CentPs.p[2]     = SCT_Z;
		m_CentPs_S[0].p[0]= SCT_X, m_CentPs_S[0].p[1]= SCT_Y, m_CentPs_S[0].p[2]= SCT_Z;
		m_CentPs_S[1].p[0]= SCT_X, m_CentPs_S[1].p[1]= SCT_Y, m_CentPs_S[1].p[2]= SCT_Z;
		m_CentPs_S[2].p[0]= SCT_X, m_CentPs_S[2].p[1]= SCT_Y, m_CentPs_S[2].p[2]= SCT_Z;
		m_CentPs_S[3].p[0]= SCT_X, m_CentPs_S[3].p[1]= SCT_Y, m_CentPs_S[3].p[2]= SCT_Z;
		m_CentPs_S[4].p[0]= SCT_X, m_CentPs_S[4].p[1]= SCT_Y, m_CentPs_S[4].p[2]= SCT_Z;
		m_Eye.p[0]        = EYE_X, m_Eye.p[1]        = EYE_Y, m_Eye.p[2]        = EYE_Z;
		m_Eye_S[0].p[0]   = EYE_X, m_Eye_S[0].p[1]   = EYE_Y, m_Eye_S[0].p[2]   = EYE_Z;
		m_Eye_S[1].p[0]   = EYE_X, m_Eye_S[1].p[1]   = EYE_Y, m_Eye_S[1].p[2]   = EYE_Z;
		m_Eye_S[2].p[0]   = EYE_X, m_Eye_S[2].p[1]   = EYE_Y, m_Eye_S[2].p[2]   = EYE_Z;
		m_Eye_S[3].p[0]   = EYE_X, m_Eye_S[3].p[1]   = EYE_Y, m_Eye_S[3].p[2]   = EYE_Z;
		m_Eye_S[4].p[0]   = EYE_X, m_Eye_S[4].p[1]   = EYE_Y, m_Eye_S[4].p[2]   = EYE_Z;
		m_EyeSv.p[0]      = EYE_X, m_EyeSv.p[1]      = EYE_Y, m_EyeSv.p[2]      = EYE_Z;
		m_EyeSv_S[0].p[0] = EYE_X, m_EyeSv_S[0].p[1] = EYE_Y, m_EyeSv_S[0].p[2] = EYE_Z;
		m_EyeSv_S[1].p[0] = EYE_X, m_EyeSv_S[1].p[1] = EYE_Y, m_EyeSv_S[1].p[2] = EYE_Z;
		m_EyeSv_S[2].p[0] = EYE_X, m_EyeSv_S[2].p[1] = EYE_Y, m_EyeSv_S[2].p[2] = EYE_Z;
		m_EyeSv_S[3].p[0] = EYE_X, m_EyeSv_S[3].p[1] = EYE_Y, m_EyeSv_S[3].p[2] = EYE_Z;
		m_EyeSv_S[4].p[0] = EYE_X, m_EyeSv_S[4].p[1] = EYE_Y, m_EyeSv_S[4].p[2] = EYE_Z;
		m_TruckLen        = TRLEN, m_TruckLen_S[0]   = TRLEN, m_TruckLen_S[1]   = TRLEN;
		m_TruckLen_S[2]   = TRLEN, m_TruckLen_S[3]   = TRLEN, m_TruckLen_S[4]   = TRLEN;
		ScrDiv1           = 0.5,   ScrDiv2           = 0.5;
		m_GridDisp        = 0,     m_GridDisp_S[0]   = 0,     m_GridDisp_S[1]   = 0;
		m_GridDisp_S[2]   = 0,     m_GridDisp_S[3]   = 0,     m_GridDisp_S[4]   = 0;
		m_GridBind        = 0,     m_GridBind_S[0]   = 0,     m_GridBind_S[1]   = 0;
		m_GridBind_S[2]   = 0,     m_GridBind_S[3]   = 0,     m_GridBind_S[4]   = 0;
		m_GridSpace       = GRSPC, m_GridSpace_S[0]  = GRSPC, m_GridSpace_S[1]  = GRSPC;
		m_GridSpace_S[2]  = GRSPC, m_GridSpace_S[3]  = GRSPC, m_GridSpace_S[4]  = GRSPC;
		m_GridIntvl       = GRINT, m_GridIntvl_S[0]  = GRINT, m_GridIntvl_S[1]  = GRINT;
		m_GridIntvl_S[2]  = GRINT, m_GridIntvl_S[3]  = GRINT, m_GridIntvl_S[4]  = GRINT;
		m_Bpoint.p[0]     = 0,     m_Bpoint.p[1]     = 0,     m_Bpoint.p[2]     = -8335;
		m_Bpoint_S[0].p[0]= 0,     m_Bpoint_S[0].p[1]= 0,     m_Bpoint_S[0].p[2]= -8335;
		m_Bpoint_S[1].p[0]= 0,     m_Bpoint_S[1].p[1]= 0,     m_Bpoint_S[1].p[2]= -8335;
		m_Bpoint_S[2].p[0]= 0,     m_Bpoint_S[2].p[1]= 0,     m_Bpoint_S[2].p[2]= -8335;
		m_Bpoint_S[3].p[0]= 0,     m_Bpoint_S[3].p[1]= 0,     m_Bpoint_S[3].p[2]= -8335;
		m_Bpoint_S[4].p[0]= 0,     m_Bpoint_S[4].p[1]= 0,     m_Bpoint_S[4].p[2]= -8335;
		m_Blspnt.p[0]     = 0,     m_Blspnt.p[1]     =-10000, m_Blspnt.p[2]     = 0;
		m_Blspnt_S[0].p[0]= 0,     m_Blspnt_S[0].p[1]=-10000, m_Blspnt_S[0].p[2]= 0;
		m_Blspnt_S[1].p[0]= 0,     m_Blspnt_S[1].p[1]=-10000, m_Blspnt_S[1].p[2]= 0;
		m_Blspnt_S[2].p[0]= 0,     m_Blspnt_S[2].p[1]=-10000, m_Blspnt_S[2].p[2]= 0;
		m_Blspnt_S[3].p[0]= 0,     m_Blspnt_S[3].p[1]=-10000, m_Blspnt_S[3].p[2]= 0;
		m_Blspnt_S[4].p[0]= 0,     m_Blspnt_S[4].p[1]=-10000, m_Blspnt_S[4].p[2]= 0;
		m_Blepnt.p[0]     = 0,     m_Blepnt.p[1]     = 10000, m_Blepnt.p[2]     = 0;
		m_Blepnt_S[0].p[0]= 0,     m_Blepnt_S[0].p[1]= 10000, m_Blepnt_S[0].p[2]= 0;
		m_Blepnt_S[1].p[0]= 0,     m_Blepnt_S[1].p[1]= 10000, m_Blepnt_S[1].p[2]= 0;
		m_Blepnt_S[2].p[0]= 0,     m_Blepnt_S[2].p[1]= 10000, m_Blepnt_S[2].p[2]= 0;
		m_Blepnt_S[3].p[0]= 0,     m_Blepnt_S[3].p[1]= 10000, m_Blepnt_S[3].p[2]= 0;
		m_Blepnt_S[4].p[0]= 0,     m_Blepnt_S[4].p[1]= 10000, m_Blepnt_S[4].p[2]= 0;
		m_BpDisp          = 0,     m_BpDisp_S[0]     = 0,     m_BpDisp_S[1]     = 0;
		m_BpDisp_S[2]     = 0,     m_BpDisp_S[3]     = 0,     m_BpDisp_S[4]     = 0;
		m_BpBind          = 0,     m_BpBind_S[0]     = 0,     m_BpBind_S[1]     = 0;
		m_BpBind_S[2]     = 0,     m_BpBind_S[3]     = 0,     m_BpBind_S[4]     = 0;
		m_BlDisp          = 0,     m_BlDisp_S[0]     = 0,     m_BlDisp_S[1]     = 0;
		m_BlDisp_S[2]     = 0,     m_BlDisp_S[3]     = 0,     m_BlDisp_S[4]     = 0;
		m_BlBind          = 0,     m_BlBind_S[0]     = 0,     m_BlBind_S[1]     = 0;
		m_BlBind_S[2]     = 0,     m_BlBind_S[3]     = 0,     m_BlBind_S[4]     = 0;
		m_GlbDisp         = 0,     m_GlbDisp_S[0]    = 0,     m_GlbDisp_S[1]    = 0;
		m_GlbDisp_S[2]    = 0,     m_GlbDisp_S[3]    = 0,     m_GlbDisp_S[4]    = 0;
		m_GlbChange       = 0,     m_GlbChange_S[0]  = 0,     m_GlbChange_S[1]  = 0;
		m_GlbChange_S[2]  = 0,     m_GlbChange_S[3]  = 0,     m_GlbChange_S[4]  = 0;
		m_DspTmpNo        = 0,     m_DspTmpNo_S[0]   = 0,     m_DspTmpNo_S[1]   = 0;
		m_DspTmpNo_S[2]   = 0,     m_DspTmpNo_S[3]   = 0,     m_DspTmpNo_S[4]   = 0;
		for(i=0; i<8; i++) {
			for(j=0; j<3; j++) {
				m_TempPath[i][j]   = _T("");
			}
		}
		for(i=0; i<5; i++) {
			for(j=0; j<8; j++) {
				for(k=0; k<3; k++) {
					m_TempPath_S[i][j][k] = _T("");
				}
			}
		}
		for(i=0; i<3; i++) {
			m_TexFile[i] = _T("");
		}
		m_TexFovy[0]   = FOVY, m_TexFovy[1]   = FOVY, m_TexFovy[2]   = FOVY;
		m_TexDisp[0]   = 0,    m_TexDisp[1]   = 0,    m_TexDisp[2]   = 0;
		m_TexMove1[0]  = 0.0,  m_TexMove1[1]  = 0.0,  m_TexMove1[2]  = 0.0;
		m_TexMove2[0]  = 0.0,  m_TexMove2[1]  = 0.0,  m_TexMove2[2]  = 0.0;
		m_TexRotate[0] = 0,    m_TexRotate[1] = 0,    m_TexRotate[2] = 0;
		m_TexScale[0]  = 1.0f, m_TexScale[1]  = 1.0f, m_TexScale[2]  = 1.0f;
		m_ExactRev     = 1.0;
	}
	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3面図表示状態初期設定
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] buf;
	delete[] spath;
	delete[] lpath;
}

/******************************************/
void CJcad3GlbView::WriteScrInfoFile(void)								//<<< 画面制御情報保存ﾌｧｲﾙ書込
/******************************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     i, j, k, er=0;
	float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	path = GetAppPath(0)+SIS_NAME;										// 画面制御情報保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fprintf(fp, "%d %d %d\n", m_Cent3D.p[0],
                                         m_Cent3D.p[1],
                                         m_Cent3D.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_Cent3D_S[i].p[0],
                                             m_Cent3D_S[i].p[1],
                                             m_Cent3D_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Fovy,
                                         m_Fovy_S[0],
                                         m_Fovy_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Fovy_S[2],
                                         m_Fovy_S[3],
                                         m_Fovy_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_CentPs.p[0],
                                         m_CentPs.p[1],
                                         m_CentPs.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_CentPs_S[i].p[0],
                                             m_CentPs_S[i].p[1],
                                             m_CentPs_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Eye.p[0],
                                         m_Eye.p[1],
                                         m_Eye.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_Eye_S[i].p[0],
                                             m_Eye_S[i].p[1],
                                             m_Eye_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_EyeSv.p[0],
                                         m_EyeSv.p[1],
                                         m_EyeSv.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_EyeSv_S[i].p[0],
                                             m_EyeSv_S[i].p[1],
                                             m_EyeSv_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			f1 = (float)m_TruckLen;
			f2 = (float)m_TruckLen_S[0];
			f3 = (float)m_TruckLen_S[1];
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			f1 = (float)m_TruckLen_S[2];
			f2 = (float)m_TruckLen_S[3];
			f3 = (float)m_TruckLen_S[4];
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			f1 = (float)ScrDiv1;
			f2 = (float)ScrDiv2;
			if(fprintf(fp, "%f %f\n", f1, f2)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridDisp,
                                         m_GridDisp_S[0],
                                         m_GridDisp_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridDisp_S[2],
                                         m_GridDisp_S[3],
                                         m_GridDisp_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridBind,
                                         m_GridBind_S[0],
                                         m_GridBind_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridBind_S[2],
                                         m_GridBind_S[3],
                                         m_GridBind_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridSpace,
                                         m_GridSpace_S[0],
                                         m_GridSpace_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridSpace_S[2],
                                         m_GridSpace_S[3],
                                         m_GridSpace_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridIntvl,
                                         m_GridIntvl_S[0],
                                         m_GridIntvl_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GridIntvl_S[2],
                                         m_GridIntvl_S[3],
                                         m_GridIntvl_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Bpoint.p[0],
                                         m_Bpoint.p[1],
                                         m_Bpoint.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_Bpoint_S[i].p[0],
                                             m_Bpoint_S[i].p[1],
                                             m_Bpoint_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Blspnt.p[0],
                                         m_Blspnt.p[1],
                                         m_Blspnt.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_Blspnt_S[i].p[0],
                                             m_Blspnt_S[i].p[1],
                                             m_Blspnt_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_Blepnt.p[0],
                                         m_Blepnt.p[1],
                                         m_Blepnt.p[2])<0) {
				er = 1; break;
			}
			for(i=0; i<5; i++) {
				if(fprintf(fp, "%d %d %d\n", m_Blepnt_S[i].p[0],
                                             m_Blepnt_S[i].p[1],
                                             m_Blepnt_S[i].p[2])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BpDisp,
                                         m_BpDisp_S[0],
                                         m_BpDisp_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BpDisp_S[2],
                                         m_BpDisp_S[3],
                                         m_BpDisp_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BpBind,
                                         m_BpBind_S[0],
                                         m_BpBind_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BpBind_S[2],
                                         m_BpBind_S[3],
                                         m_BpBind_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BlDisp,
                                         m_BlDisp_S[0],
                                         m_BlDisp_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BlDisp_S[2],
                                         m_BlDisp_S[3],
                                         m_BlDisp_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BlBind,
                                         m_BlBind_S[0],
                                         m_BlBind_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_BlBind_S[2],
                                         m_BlBind_S[3],
                                         m_BlBind_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GlbDisp,
                                         m_GlbDisp_S[0],
                                         m_GlbDisp_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GlbDisp_S[2],
                                         m_GlbDisp_S[3],
                                         m_GlbDisp_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GlbChange,
                                         m_GlbChange_S[0],
                                         m_GlbChange_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_GlbChange_S[2],
                                         m_GlbChange_S[3],
                                         m_GlbChange_S[4])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_DspTmpNo,
                                         m_DspTmpNo_S[0],
                                         m_DspTmpNo_S[1])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_DspTmpNo_S[2],
                                         m_DspTmpNo_S[3],
                                         m_DspTmpNo_S[4])<0) {
				er = 1; break;
			}
			for(i=0; i<8; i++) {
				for(j=0; j<3; j++) {
					memset(spath, NULL, _MAX_PATH);
					::GetShortPathName(m_TempPath[i][j], spath, _MAX_PATH);
					if(fprintf(fp, "%d-%d:%s\n", i, j, spath)<0) {
						er = 1; break;
					}
				}
				if(er) {
					break;
				}
			}
			if(er) {
				break;
			}
			for(i=0; i<5; i++) {
				for(j=0; j<8; j++) {
					for(k=0; k<3; k++) {
						memset(spath, NULL, _MAX_PATH);
						::GetShortPathName(m_TempPath_S[i][j][k], spath, _MAX_PATH);
						if(fprintf(fp, "%d-%d:%s\n", j, k, spath)<0) {
							er = 1; break;
						}
					}
					if(er) {
						break;
					}
				}
				if(er) {
					break;
				}
			}
			if(er) {
				break;
			}
			for(i=0; i<3; i++) {
				memset(spath, NULL, _MAX_PATH);
				::GetShortPathName(m_TexFile[i], spath, _MAX_PATH);
				if(fprintf(fp, "%d:%s\n", i, spath)<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			if(fprintf(fp, "%d %d %d\n", m_TexFovy[0],
                                         m_TexFovy[1],
                                         m_TexFovy[2])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_TexDisp[0],
                                         m_TexDisp[1],
                                         m_TexDisp[2])<0) {
				er = 1; break;
			}
			f1 = (float)m_TexMove1[0];
			f2 = (float)m_TexMove1[1];
			f3 = (float)m_TexMove1[2];
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			f1 = (float)m_TexMove2[0];
			f2 = (float)m_TexMove2[1];
			f3 = (float)m_TexMove2[2];
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_TexRotate[0],
                                         m_TexRotate[1],
                                         m_TexRotate[2])<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%f %f %f\n", m_TexScale[0],
                                         m_TexScale[1],
                                         m_TexScale[2])<0) {
				er = 1; break;
			}
			f1 = (float)m_ExactRev;
			if(fprintf(fp, "%f\n", f1)<0) {
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

/************************************/
void CJcad3GlbView::SaveRender(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞ情報保存
/************************************/
{
	int i, j;

	m_MapFile_S = m_MapFile;
	for(i=0; i<3; i++) {
		m_RColor_S[i] = m_RColor[i];
	}
	for(i=0; i<18; i++) {
		for(j=0; j<3; j++) {
			m_CColor_S[i][j] = m_CColor[i][j];
		}
		m_Shin_S[i] = m_Shin[i];
	}
}

/***************************************/
void CJcad3GlbView::RestoreRender(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞ情報復元
/***************************************/
{
	int i, j;

	m_MapFile = m_MapFile_S;
	for(i=0; i<3; i++) {
		m_RColor[i] = m_RColor_S[i];
	}
	for(i=0; i<18; i++) {
		for(j=0; j<3; j++) {
			m_CColor[i][j] = m_CColor_S[i][j];
		}
		m_Shin[i] = m_Shin_S[i];
		SetGLColor(i);													// OpenGLｶﾗｰ設定
	}
	SetGLRender();														// OpenGLｶﾗｰ設定
	SetGLRenderMap();													// OpenGLﾏｯﾌﾟ設定
}

/***************************************************************/
void CJcad3GlbView::GetColorRGB(int cno, int kind,
                                int* r, int* g, int* b, int* s)			//<<< ｶﾗｰ情報取得
/***************************************************************/
{
	*r = GetRValue(m_CColor[cno][kind]);
	*g = GetGValue(m_CColor[cno][kind]);
	*b = GetBValue(m_CColor[cno][kind]);
	*s = m_Shin[cno];
}

/***********************************************************/
void CJcad3GlbView::SetColorRGB(int cno, int kind,
                                int r, int g, int b, int s)				//<<< ｶﾗｰ情報設定
/***********************************************************/
{
	m_CColor[cno][kind] = RGB(r, g, b);
	m_Shin[cno] = (kind==1) ? s : m_Shin[cno];
	SetGLColor(cno);													// OpenGLｶﾗｰ設定
	Display(0);															// 再描画
}

/******************************************************************/
void CJcad3GlbView::GetRenderRGB(int kind, int* r, int* g, int* b)		//<<< ﾚﾝﾀﾞﾘﾝｸﾞ情報取得
/******************************************************************/
{
	*r = GetRValue(m_RColor[kind]);
	*g = GetGValue(m_RColor[kind]);
	*b = GetBValue(m_RColor[kind]);
}

/***************************************************************/
void CJcad3GlbView::SetRenderRGB(int kind, int r, int g, int b)			//<<< ﾚﾝﾀﾞﾘﾝｸﾞ情報設定
/***************************************************************/
{
	m_RColor[kind] = RGB(r, g, b);
	SetGLRender();														// OpenGLｶﾗｰ設定
	Display(0);															// 再描画
}

/***********************************************/
void CJcad3GlbView::GetRenderMap(CString* file)							//<<< ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ取得
/***********************************************/
{
	*file = m_MapFile;
}

/**********************************************/
void CJcad3GlbView::SetRenderMap(CString file)							//<<< ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ設定
/**********************************************/
{
	m_MapFile = file;
	SetGLRenderMap();													// OpenGLﾏｯﾌﾟ設定
	SetRenderMode(2);													// ﾚﾝﾀﾞﾘﾝｸﾞﾓｰﾄﾞ
	Display(0);															// 再描画
}

/*****************************************/
void CJcad3GlbView::StartRenderTest(void)								//<< ﾚﾝﾀﾞﾘﾝｸﾞﾃｽﾄ開始処理
/*****************************************/
{
	OBJTYPE* op;
	CString  fname;

	m_OpenGLTest = 1;													// OpenGLﾃｽﾄ中
	ChangeTestFlg(0);													// ﾃｽﾄ用ﾌﾗｸﾞ更新(開始)
	SscrA.p[0] = 0, SscrA.p[1] = 0, SscrA.p[2] = 0;						// ﾃｽﾄ環境へ
	SetSeishaei(SscrA, 20000);											// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	SscrA = m_CentPs, SscrB = m_Eye, SscrC = m_EyeSv;					// 現状を保存
	m_CentPs.p[0] = 0, m_CentPs.p[1] = 0, m_CentPs.p[2] = 0;
	m_Eye.p[0] = 22000, m_Eye.p[1] = 0, m_Eye.p[2] = 0;
	m_EyeSv.p[0] = 22000, m_EyeSv.p[1] = 0, m_EyeSv.p[2] = 0;
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	fname = GetAppPath(0)+GLT_NAME;										// OpenGLﾃｽﾄ形状
	OnUndoStop();														// Undo保存停止
	if(!ReadJsdFile(fname, 0, 0)) {										// ﾌﾟﾘﾐﾃｨﾌﾞ入力
		GetObjPtr(&op);													// 立体ﾎﾟｲﾝﾀ取得
		Sop1 = op;														// ﾎﾟｲﾝﾀ保存
	} else {
		AfxMessageBox(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
	}
	OffUndoStop();														// Undo保存再開
	if(ObjectRendering()) {                                             // ﾚﾝﾀﾞﾘﾝｸﾞ処理
		SetRenderMode(2);
	}
	Display(0);															// 再描画
}

/***************************************/
void CJcad3GlbView::EndRenderTest(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞﾃｽﾄ終了処理
/***************************************/
{
	m_OpenGLTest = 0;													// OpenGLﾃｽﾄ終了
	ChangeTestFlg(1);													// ﾃｽﾄ用ﾌﾗｸﾞ更新(終了)
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	m_CentPs = SscrA, m_Eye = SscrB, m_EyeSv = SscrC;					// ﾃｽﾄ前へ復帰
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	SetRenderMode(0);													// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
	FreeObj(Sop1, 0);													// 立体削除
	Sop1 = NULL;
	Display(0);															// 再描画
}

/************************************/
void CJcad3GlbView::SetCtrlKey(void)									//<<< 制御キー設定
/************************************/
{
	if(m_KeyEnv&0x1) {													// ﾏｳｽ入力制限ｷｰ
		m_LimKey = MK_SHIFT, m_NLiKey = MK_CONTROL;
	} else {
		m_LimKey = MK_CONTROL, m_NLiKey = MK_SHIFT;
	}
	if(m_KeyEnv&0x2) {													// 立体選択追加ｷｰ
		m_ObjKey = MK_SHIFT;
	} else {
		m_ObjKey = MK_CONTROL;
	}
	if(m_KeyEnv&0x4) {													// 立体選択解除ｷｰ
		m_NObKey = MK_SHIFT;
	} else {
		m_NObKey = MK_CONTROL;
	}
	if(m_KeyEnv&0x8) {													// 制御点選択追加/解除ｷｰ
		m_CtlKey = MK_SHIFT, m_CurKey = MK_CONTROL;
	} else {
		m_CtlKey = MK_CONTROL, m_CurKey = MK_SHIFT;
	}
	if(m_KeyEnv&0x10) {													// 数値入力ｼｮｰﾄｶｯﾄｷｰ
		m_NumKey = 0x1, m_NNuKey = 0x2;
	} else {
		m_NumKey = 0x2, m_NNuKey = 0x1;
	}
}
