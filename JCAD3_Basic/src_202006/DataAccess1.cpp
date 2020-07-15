/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部
// DataAccess1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

/**********************************************************/
/*****                   領域初期化                   *****/
/**********************************************************/
/**************************/
void ReadMemDataFile(void)												//<<< ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ読込
/**************************/
{
	FILE*   fp;
	CString path;
	int     er=0, upd=0;

	path = GetAppPath(0)+MES_NAME;										// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &Int_max, &Pnt_max, &Undo_max)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &CCtl_max, &SCtl_max)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &CCtlA_max, &SCtlA_max, &WCtl_max)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &CPnt_max, &SPnt_max)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &OPnt_max, &BPnt_max)!=2) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
		upd = 1;														// ﾌｧｲﾙなし
	}
	if(upd||er) {														// <初期値設定>
		Int_max   = MAXINTAN, Pnt_max  = MAXPNTAN, Undo_max  = MAXUNDON;
		CCtl_max  = MAXCCTLN, SCtl_max = MAXSCTLN, CCtlA_max = MAXCCTLA;
		SCtlA_max = MAXSCTLA, WCtl_max = MAXWCTLN, CPnt_max  = MAXCPNTN;
		SPnt_max  = MAXSPNTN, OPnt_max = MAXOPNTN, BPnt_max  = MAXBPNTN;
		WriteMemDataFile();												// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ書込
	}
}

/***************************/
void WriteMemDataFile(void)												//<<< ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ書込
/***************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+MES_NAME;										// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ名
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fprintf(fp, "%d %d %d\n", Int_max, Pnt_max, Undo_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", CCtl_max, SCtl_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", CCtlA_max, SCtlA_max, WCtl_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", CPnt_max, SPnt_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", OPnt_max, BPnt_max)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;															// ﾌｧｲﾙなし
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
}

/*******************/
int GetIntMax(void)														//<<< 整数ﾊﾟﾗﾒｰﾀ配列数取得
/*******************/
{
	return Int_max;
}

/***********************/
void SetIntMax(int max)													//<<< 整数ﾊﾟﾗﾒｰﾀ配列数設定
/***********************/
{
	Int_max = max;
}

/*******************/
int GetPntMax(void)														//<<< 座標ﾊﾟﾗﾒｰﾀ配列数取得
/*******************/
{
	return Pnt_max;
}

/***********************/
void SetPntMax(int max)													//<<< 座標ﾊﾟﾗﾒｰﾀ配列数設定
/***********************/
{
	Pnt_max = max;
}

/********************/
int GetUndoMax(void)													//<<< Undo/Redoﾊﾟﾗﾒｰﾀ配列数取得
/********************/
{
	return Undo_max;
}

/************************/
void SetUndoMax(int max)												//<<< Undo/Redoﾊﾟﾗﾒｰﾀ配列数設定
/************************/
{
	Undo_max = max;
}

/********************/
int GetCCtlMax(void)													//<<< 最大曲線制御点数取得
/********************/
{
	return CCtl_max;
}

/************************/
void SetCCtlMax(int max)												//<<< 最大曲線制御点数設定
/************************/
{
	CCtl_max = max, CCtlA_max = max + 2;
}

/********************/
int GetCCtlAMax(void)													//<<< 曲線制御点配列数取得
/********************/
{
	return CCtlA_max;
}

/********************/
int GetSCtlMax(void)													//<<< 最大曲面制御点数取得
/********************/
{
	return SCtl_max;
}

/************************/
void SetSCtlMax(int max)												//<<< 最大曲面制御点数設定
/************************/
{
	SCtl_max = max, SCtlA_max = (int)(max*1.1);
}

/*********************/
int GetSCtlAMax(void)													//<<< 曲面制御点配列数取得
/*********************/
{
	return SCtlA_max;
}

/********************/
int GetWCtlMax(void)													//<<< 作業用制御点配列数取得
/********************/
{
	return WCtl_max;
}

/************************/
void SetWCtlMax(int max)												//<<< 作業用制御点配列数設定
/************************/
{
	WCtl_max = max;
}

/********************/
int GetCPntMax(void)													//<<< 最大曲線頂点数取得
/********************/
{
	return CPnt_max;
}

/************************/
void SetCPntMax(int max)												//<<< 最大曲線頂点数設定
/************************/
{
	CPnt_max = max;
}

/********************/
int GetSPntMax(void)													//<<< 最大曲面頂点数取得
/********************/
{
	return SPnt_max;
}

/************************/
void SetSPntMax(int max)												//<<< 最大曲面頂点数設定
/************************/
{
	SPnt_max = max;
}

/********************/
int GetOPntMax(void)													//<<< 最大立体頂点数取得
/********************/
{
	return OPnt_max;
}

/************************/
void SetOPntMax(int max)												//<<< 最大立体頂点数設定
/************************/
{
	OPnt_max = max;
}

/********************/
int GetBPntMax(void)													//<<< 最大断面頂点数取得
/********************/
{
	return BPnt_max;
}

/************************/
void SetBPntMax(int max)												//<<< 最大断面頂点数設定
/************************/
{
	BPnt_max = max;
}

/*******************************/
static void CreateMemArea(void)											//<<< ﾃﾞｰﾀ領域確保
/*******************************/
{
	Int_ary = (int *)malloc(sizeof(int)*Int_max);						// 整数ﾊﾟﾗﾒｰﾀ配列領域確保
	if(Int_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Pnt_ary = (PNTTYPE *)malloc(sizeof(PNTTYPE)*Pnt_max);				// 座標ﾊﾟﾗﾒｰﾀ配列領域確保
	if(Pnt_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Undo_ary = (UNDTYPE *)malloc(sizeof(UNDTYPE)*Undo_max);				// Undo/Redo配列領域確保
	if(Undo_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
}

/**************************/
static void StartVar(void)												//<<< 開始変数初期化
/**************************/
{
	int	i;

	Temp_cmp = NULL, Rf_cmp = NULL, Tach_cmp = NULL, Copy_cmp = NULL;	// ｺﾏﾝﾄﾞ操作用変数初期化
	Int_adr = 0, Pnt_adr = 0, Cmd_flg = 0, Cmd_upd = 0, Cmd_stp = 0;
	Base_op = NULL, Temp_op = NULL, Rf_op = NULL, Reset_op = NULL;		// 立体操作用変数初期化
	Oset_op = NULL, SelectNo = 0, Base_oif = NULL, Temp_oif = NULL;
	for(i=0; i<Undo_max; i++) {
		Undo_ary[i].urflg = -1, Undo_ary[i].cmflg = -1;					// Undo操作用変数初期化
		Undo_ary[i].cmp = NULL, Undo_ary[i].cop = NULL;
		Undo_ary[i].dop = NULL;
	}
	Undo_adr = 0, Undo_flg = 1, Undo_stp = 0;
	for(i=0; i<DIM; i++) {
		Scr_Box.bp[0].p[i] = 0.0, Scr_Box.bp[1].p[i] = 0.0;
	}
	Base_owk = NULL, Rf_owk = NULL;										// 立体･ｺﾏﾝﾄﾞ作業用変数初期化
	Curve_Rept_SW = 0;
	Curve_Rept_Num = 3;	//202006 2 -> 3 に変更							// 曲面細分化作業用変数初期化
}

/******************/
void InitVar(void)														//<<< 変数初期化(ｼｽﾃﾑ起動時)
/******************/
{
	Demo_Flg = 0, Init_Flg = 0, File_Flg = 0;							// 初期起動制御用変数初期化
	Spsh_Flg = 0, Msg_No = 0;											// ｽﾌﾟﾗｯｼｭ関連変数初期化
	Gc_cmp = NULL, Gc_op  = NULL, Gc_oif = NULL;						// 再利用変数初期化
	ReadMemDataFile();													// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ読込
	CreateMemArea();													// ﾃﾞｰﾀ領域確保
	StartVar();															// 開始変数初期化
}

/*******************/
void ResetVar(void)														//<<< 変数初期化(ｼｽﾃﾑ稼働中)
/*******************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	CPCTYPE* cpc, * dcpc;

	op = Base_op;														// <有効立体領域部>
	while(op!=NULL) {													// ｺﾏﾝﾄﾞ領域解放
		if((cmp=op->bcmp)!=NULL) {
			while(cmp->onxt!=NULL) {
				cmp = cmp->onxt;
			}
			cmp->onxt = Gc_cmp, Gc_cmp = op->bcmp;
		}
		op = op->onxt;
	}
	if(Base_op!=NULL) {													// 立体領域解放
		op = Base_op;
		while(op->onxt!=NULL) {
			op = op->onxt;
		}
		op->onxt = Gc_op, Gc_op = Base_op;
	}
	if(Base_oif!=NULL) {												// <有効構成領域部>
		oif = Base_oif;
		while(oif->next!=NULL) {
			oif = oif->next;											// 領域解放
		}
		oif->next = Gc_oif, Gc_oif = Base_oif;
	}
	op = Temp_op;														// <一時削除立体領域部>
	while(op!=NULL) {													// ｺﾏﾝﾄﾞ領域解放
		if((cmp=op->bcmp)!=NULL) {
			while(cmp->onxt!=NULL) {
				cmp = cmp->onxt;
			}
			cmp->onxt = Gc_cmp, Gc_cmp = op->bcmp;
		}
		op = op->onxt;
	}
	if(Temp_op!=NULL) {													// 立体領域解放
		op = Temp_op;
		while(op->onxt!=NULL) {
			op = op->onxt;
		}
		op->onxt = Gc_op, Gc_op = Temp_op;
	}
	if(Temp_oif!=NULL) {												// <一時削除構成領域部>
		oif = Temp_oif;
		while(oif->next!=NULL) {
			oif = oif->next;											// 領域解放
		}
		oif->next = Gc_oif, Gc_oif = Temp_oif;
	}
	if(Temp_cmp!=NULL) {												// <一時削除ｺﾏﾝﾄﾞ領域部>
		cmp = Temp_cmp;
		while(cmp->onxt!=NULL) {
			cmp = cmp->onxt;											// ｺﾏﾝﾄﾞ領域解放
		}
		cmp->onxt = Gc_cmp, Gc_cmp = Temp_cmp;
	}
	cpc = Copy_cmp;														// <ｺﾋﾟｰｺﾏﾝﾄﾞ領域部>
	while(cpc!=NULL) {
		dcpc = cpc, cpc = cpc->next;
		free(dcpc);														// ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
	}
	StartVar();															// 開始変数初期化
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/******************/
void FreeMem(void)														//<<< ﾒﾓﾘ解放(ｼｽﾃﾑ終了時)
/******************/
{
	OBJTYPE* op, * dop;
	OIFTYPE* oif, * doif;
	CMDTYPE* cmp, * dcmp;
	CPCTYPE* cpc, * dcpc;

	op = Base_op;														// <有効立体領域部>
	while(op!=NULL) {
		cmp = op->bcmp;
		while(cmp!=NULL) {
			dcmp = cmp, cmp = cmp->onxt;
			free(dcmp);													// ｺﾏﾝﾄﾞ領域解放
		}
		dop = op, op = op->onxt;
		free(dop);														// 立体領域解放
	}
	oif = Base_oif;														// <有効構成領域部>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// 領域解放
	}
	op = Temp_op;														// <一時削除立体領域部>
	while(op!=NULL) {
		cmp = op->bcmp;
		while(cmp!=NULL) {
			dcmp = cmp, cmp = cmp->onxt;
			free(dcmp);													// ｺﾏﾝﾄﾞ領域解放
		}
		dop = op, op = op->onxt;
		free(dop);														// 立体領域解放
	}
	oif = Temp_oif;														// <一時削除構成領域部>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// 領域解放
	}
	cmp = Temp_cmp;														// <一時削除ｺﾏﾝﾄﾞ領域部>
	while(cmp!=NULL) {
		dcmp = cmp, cmp = cmp->onxt;
		free(dcmp);														// ｺﾏﾝﾄﾞ領域解放
	}
	op = Gc_op;															// <削除立体領域部>
	while(op!=NULL) {
		dop = op, op = op->onxt;
		free(dop);														// 立体領域解放
	}
	oif = Gc_oif;														// <削除構成領域部>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// 領域解放
	}
	cmp = Gc_cmp;														// <一時削除ｺﾏﾝﾄﾞ領域部>
	while(cmp!=NULL) {
		dcmp = cmp, cmp = cmp->onxt;
		free(dcmp);														// ｺﾏﾝﾄﾞ領域解放
	}
	cpc = Copy_cmp;														// <ｺﾋﾟｰｺﾏﾝﾄﾞ領域部>
	while(cpc!=NULL) {
		dcpc = cpc, cpc = cpc->next;
		free(dcpc);														// ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
	}
}

/**********************************************************/
/*****                 領域アドレス変更               *****/
/**********************************************************/
/******************************************************/
void ChangeAreaAdr(int isa, int icn, int psa, int pcn)					//<<< 配列ｱﾄﾞﾚｽ変更
/******************************************************/
{
	OBJTYPE* optr;
	CMDTYPE* cptr;
	CPCTYPE* pptr;
	OWKTYPE* owptr;
	CWKTYPE* cwptr;

	optr = Base_op;														// Base開始番地変更
	while(optr!=NULL) {
		optr->vsa = (optr->vsa>psa) ? optr->vsa+pcn : optr->vsa;
		optr->lsa = (optr->lsa>isa) ? optr->lsa+icn : optr->lsa;
		cptr = optr->bcmp;
		while(cptr!=NULL) {
			cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
			cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			cptr = cptr->onxt;
		}
		optr = optr->onxt;
	}
	optr = Temp_op;														// Temp開始番地変更
	while(optr!=NULL) {
		optr->vsa = (optr->vsa>psa) ? optr->vsa+pcn : optr->vsa;
		optr->lsa = (optr->lsa>isa) ? optr->lsa+icn : optr->lsa;
		cptr = optr->bcmp;
		while(cptr!=NULL) {
			cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
			cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			cptr = cptr->onxt;
		}
		optr = optr->onxt;
	}
	cptr = Temp_cmp;													// Temp開始番地変更
	while(cptr!=NULL) {
		cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
		cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
		cptr = cptr->onxt;
	}
	pptr = Copy_cmp;													// Copy開始番地変更
	while(pptr!=NULL) {
		pptr->isa = (pptr->isa>isa) ? pptr->isa+icn : pptr->isa;
		pptr->psa = (pptr->psa>psa) ? pptr->psa+pcn : pptr->psa;
		pptr = pptr->next;
	}
	cptr = Tach_cmp;													// 一時切離し中開始番地変更
	while(cptr!=NULL) {
		cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
		cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
		cptr = cptr->onxt;
	}
	owptr = Base_owk;													// 開始立体作業ﾎﾟｲﾝﾀ
	while(owptr!=NULL) {
		cwptr = owptr->cwk;												// 立体別開始ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
		while(cwptr!=NULL) {
			if(cwptr->flg!=2) {											// <未実行のｺﾏﾝﾄﾞ>
				cptr = cwptr->cmp;										// 対象ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
				cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
				cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			}
			cwptr = cwptr->next;										// 次ｺﾏﾝﾄﾞ作業ﾎﾟｲﾝﾀ
		}
		owptr = owptr->next;											// 次立体作業ﾎﾟｲﾝﾀ
	}

}

/**********************************************************/
/*****                  初期起動制御用                *****/
/**********************************************************/
/************************/
void SetDemoFlg(int flg)												//<<< 体験版ﾌﾗｸﾞ設定
/************************/
{
	Demo_Flg = flg;
}

/********************/
int GetDemoFlg(void)													//<<< 体験版ﾌﾗｸﾞ取得
/********************/
{
	return Demo_Flg;
}

/************************/
void SetInitFlg(int flg)												//<<< 初期起動状態ﾌﾗｸﾞ設定
/************************/
{
	Init_Flg = flg;
}

/********************/
int GetInitFlg(void)													//<<< 初期起動状態ﾌﾗｸﾞ取得
/********************/
{
	return Init_Flg;
}

/************************/
void SetFileFlg(int flg)												//<<< ﾌｧｲﾙ保存ﾌﾗｸﾞ設定
/************************/
{
	File_Flg = flg;
}

/********************/
int GetFileFlg(void)													//<<< ﾌｧｲﾙ保存ﾌﾗｸﾞ取得
/********************/
{
	return File_Flg;
}

/*******************************/
void SetFileName(CString fname)											//<<< 保存ﾌｧｲﾙ名設定
/*******************************/
{
	FileName = fname;
}

/*************************/
CString GetFileName(void)												//<<< 保存ﾌｧｲﾙ名取得
/*************************/
{
	return FileName;
}

/***************************/
void SetSplashMsg(UINT mno)												//<<< ｽﾌﾟﾗｯｼｭ表示中ﾒｯｾｰｼﾞ設定
/***************************/
{
	if(Spsh_Flg==0) {													// <ｽﾌﾟﾗｯｼｭ表示中>
		Msg_No = mno;													// ﾒｯｾｰｼﾞNo保存
	} else {															// <ｽﾌﾟﾗｯｼｭ非表示>
		AfxMessageBox(mno);												// ﾒｯｾｰｼﾞ表示
	}
}

/***********************/
void OutSplashMsg(void)													//<<< ｽﾌﾟﾗｯｼｭ表示中ﾒｯｾｰｼﾞ出力
/***********************/
{
	if(Msg_No!=0) {														// <ﾒｯｾｰｼﾞNoあり>
		AfxMessageBox(Msg_No);											// ﾒｯｾｰｼﾞ表示
	}
	Spsh_Flg = 1;														// ｽﾌﾟﾗｯｼｭ表示終了
	Msg_No = 0;															// ﾒｯｾｰｼﾞNoｸﾘｱ
}

/**********************************************************/
/*****              バックアップファイル用            *****/
/**********************************************************/
/**************************/
void StartBackUp(int mode)												//<<< ﾊﾞｯｸｱｯﾌﾟ開始処理
/**************************/
{
	FILE*   fp;
	CString bfile, ofile;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	Cmd_Lvl = 0, Cmd_Lvl_Sv = 0, Obj_No = 0;							// ﾊﾞｯｸｱｯﾌﾟ用変数初期化
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	ofile = GetAppPath(0)+BOD_NAME;										// ﾊﾞｯｸｱｯﾌﾟ保存ﾌｧｲﾙ名取得
	if(mode) {															// <初期起動時のみ>
		remove(ofile);													// 保存ﾌｧｲﾙ削除
		rename(bfile, ofile);											// ﾊﾞｯｸｱｯﾌﾟ->保存ﾌｧｲﾙ
	}
	if(fopen_s(&fp, bfile, "w")==0) {
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ初期化
	} else {
		SetSplashMsg(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
}

/********************/
void StartMenu(void)													//<<< ﾒﾆｭｰ開始処理
/********************/
{
	Cmd_Lvl_Sv = Cmd_Lvl;												// 開始時ｺﾏﾝﾄﾞﾚﾍﾞﾙNo保存
}

/********************/
void EndJMenu1(void)													//<<< ﾒﾆｭｰ終了処理1
/********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, df, ef, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ｺﾏﾝﾄﾞ実行無中止
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {	    							// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <今回ｺﾏﾝﾄﾞ有>
			LevelBaseObjPtr(1, &op);									// 一時削除立体取得
			while(op!=NULL) {
				fprintf(fp, "%d %d\n", clvl, op->ono);					// ﾍｯﾀﾞｰ情報書込
				fprintf(fp, "C%02d %d %d\n", DELCMD, 0, 0);				// ｺﾏﾝﾄﾞ情報書込
				LevelNextObjPtr(1, &op);
			}
		}
		BaseObjPtr(2, &op, &df, &ef);									// 有効立体先頭取得
		while(op) {
			BaseCmdPtr(0, op, &cmp);									// ｺﾏﾝﾄﾞ先頭取得
			while(cmp) {
				clvl = cmp->clvl;										// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
				if(clvl>Cmd_Lvl_Sv) {									// <今回ｺﾏﾝﾄﾞ有>
					cn = GetCmdNo(cmp);									// ｺﾏﾝﾄﾞNo取得
					in = GetIntNum(cmp);								// 整数ﾊﾟﾗﾒｰﾀ数取得
					pn = GetPntNum(cmp);								// 座標ﾊﾟﾗﾒｰﾀ数取得
					fprintf(fp, "%d %d\n", clvl, op->ono);				// ﾍｯﾀﾞｰ情報書込
					fprintf(fp, "C%02d %d %d\n", cn, in, pn);			// ｺﾏﾝﾄﾞ情報書込
					for(i=0; i<in; i++) {
						fprintf(fp, "%d ", GetParaInt(cmp, i));			// 整数ﾊﾟﾗﾒｰﾀ書込
					}
					fprintf(fp, "\n");									// 改行
					for(i=0; i<pn; i++) {
						GetParaPnt(cmp, i, &p);							// 座標ﾊﾟﾗﾒｰﾀ取得&書込
						fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
					}
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			NextObjPtr(2, &op, &df, &ef);								// 次有効立体へ
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
	if(Cmd_Lvl>10000) {
		AfxMessageBox(IDS_LEVEL_MAX);									// 警告ﾒｯｾｰｼﾞ出力
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/********************/
void EndJMenu2(void)													//<<< ﾒﾆｭｰ終了処理2
/********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, df, ef, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ｺﾏﾝﾄﾞ実行無中止
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <今回ｺﾏﾝﾄﾞ有>
			LevelBaseObjPtr(1, &op);									// 一時削除立体取得
			while(op!=NULL) {
				fprintf(fp, "%d %d\n", clvl, op->ono);					// ﾍｯﾀﾞｰ情報書込
				fprintf(fp, "C%02d %d %d\n", DELCMD, 0, 0);				// ｺﾏﾝﾄﾞ情報書込
				LevelNextObjPtr(1, &op);
			}
		}
		BaseObjPtr(1, &op, &df, &ef);									// 表示立体先頭取得
		while(op) {
			BaseCmdPtr(0, op, &cmp);									// ｺﾏﾝﾄﾞ先頭取得
			while(cmp) {
				clvl = cmp->clvl;										// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
				if(clvl>Cmd_Lvl_Sv) {									// <今回ｺﾏﾝﾄﾞ有>
					cn = GetCmdNo(cmp);									// ｺﾏﾝﾄﾞNo取得
					in = GetIntNum(cmp);								// 整数ﾊﾟﾗﾒｰﾀ数取得
					pn = GetPntNum(cmp);								// 座標ﾊﾟﾗﾒｰﾀ数取得
					fprintf(fp, "%d %d\n", clvl, op->ono);				// ﾍｯﾀﾞｰ情報書込
					fprintf(fp, "C%02d %d %d\n", cn, in, pn);			// ｺﾏﾝﾄﾞ情報書込
					for(i=0; i<in; i++) {
						fprintf(fp, "%d ", GetParaInt(cmp, i));			// 整数ﾊﾟﾗﾒｰﾀ書込
					}
					fprintf(fp, "\n");									// 改行
					for(i=0; i<pn; i++) {
						GetParaPnt(cmp, i, &p);							// 座標ﾊﾟﾗﾒｰﾀ取得&書込
						fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
					}
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			NextObjPtr(1, &op, &df, &ef);								// 次表示立体へ
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
	if(Cmd_Lvl>10000) {
		AfxMessageBox(IDS_LEVEL_MAX);									// 警告ﾒｯｾｰｼﾞ出力
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************/
void FileBackUp1(void)													//<<< ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理1
/**********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ｺﾏﾝﾄﾞ実行無中止
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {									// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <今回ｺﾏﾝﾄﾞ有>
			LevelBaseObjPtr(0, &op);									// 生成立体取得
			while(op) {
				BaseCmdPtr(0, op, &cmp);								// ｺﾏﾝﾄﾞ先頭取得
				while(cmp) {
					clvl = cmp->clvl;									// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
					if(clvl>Cmd_Lvl_Sv) {								// <今回ｺﾏﾝﾄﾞ有>
						cn = GetCmdNo(cmp);								// ｺﾏﾝﾄﾞNo取得
						in = GetIntNum(cmp);							// 整数ﾊﾟﾗﾒｰﾀ数取得
						pn = GetPntNum(cmp);							// 座標ﾊﾟﾗﾒｰﾀ数取得
						fprintf(fp, "%d %d\n", clvl, op->ono);			// ﾍｯﾀﾞｰ情報書込
						fprintf(fp, "C%02d %d %d\n", cn, in, pn);		// ｺﾏﾝﾄﾞ情報書込
						for(i=0; i<in; i++) {
							fprintf(fp, "%d ", GetParaInt(cmp, i));		// 整数ﾊﾟﾗﾒｰﾀ書込
						}
						fprintf(fp, "\n");								// 改行
						for(i=0; i<pn; i++) {
							GetParaPnt(cmp, i, &p);						// 座標ﾊﾟﾗﾒｰﾀ取得&書込
							fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
						}
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				LevelNextObjPtr(0, &op);
			}
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************/
void FileBackUp2(void)													//<<< ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理2
/**********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*    fp;
	OWKTYPE* owk;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ｺﾏﾝﾄﾞ実行無中止
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {									// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		owk = Base_owk;													// 開始立体作業ﾎﾟｲﾝﾀ
		while(owk!=NULL) {
			if(owk->op!=NULL) {											// <立体ﾎﾟｲﾝﾀ登録済み>
				op = owk->op;
				BaseCmdPtr(0, op, &cmp);								// ｺﾏﾝﾄﾞ先頭取得
				while(cmp) {
					clvl = cmp->clvl;									// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
					if(clvl>Cmd_Lvl_Sv) {								// <今回ｺﾏﾝﾄﾞ有>
						cn = GetCmdNo(cmp);								// ｺﾏﾝﾄﾞNo取得
						in = GetIntNum(cmp);							// 整数ﾊﾟﾗﾒｰﾀ数取得
						pn = GetPntNum(cmp);							// 座標ﾊﾟﾗﾒｰﾀ数取得
						fprintf(fp, "%d %d\n", clvl, op->ono);			// ﾍｯﾀﾞｰ情報書込
						fprintf(fp, "C%02d %d %d\n", cn, in, pn);		// ｺﾏﾝﾄﾞ情報書込
						for(i=0; i<in; i++) {
							fprintf(fp, "%d ", GetParaInt(cmp, i));		// 整数ﾊﾟﾗﾒｰﾀ書込
						}
						fprintf(fp, "\n");								// 改行
						for(i=0; i<pn; i++) {
							GetParaPnt(cmp, i, &p);						// 座標ﾊﾟﾗﾒｰﾀ取得&書込
							fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
						}
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
			}
			owk = owk->next;											// 次立体作業ﾎﾟｲﾝﾀ
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/***********************/
void UndoMenu(int clvl)													//<<< Undoﾒﾆｭｰ処理
/***********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*   fp;
	CString bfile;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		fprintf(fp, "%d %d\n", clvl, 0);								// ﾍｯﾀﾞｰ情報書込
		fprintf(fp, "C%02d %d %d\n", UNDOCMD, 0, 0);					// ｺﾏﾝﾄﾞ情報書込
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
}

/***********************/
void RedoMenu(int clvl)													//<<< Redoﾒﾆｭｰ処理
/***********************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	FILE*   fp;
	CString bfile;

	if(Demo_Flg!=0) {
		return;															// 体験版は中止
	}
	bfile = GetAppPath(0)+BUP_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		fprintf(fp, "%d %d\n", clvl, 0);								// ﾍｯﾀﾞｰ情報書込
		fprintf(fp, "C%02d %d %d\n", REDOCMD, 0, 0);					// ｺﾏﾝﾄﾞ情報書込
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// ｴﾗｰﾒｯｾｰｼﾞ出力
	}
}

/***************************************************/
static void UpdateCheck(BYTE* ary, int no, int chk)						//<<< ﾁｪｯｸ更新
/***************************************************/
{
	BYTE bt;
	int  i, j;

	i = no/8, j = no%8;
	bt = 0x1,  bt <<= 7-j;
	if(chk) {
		ary[i] |= bt;
	} else {
		bt ^= 0xff, ary[i] &= bt;
	}
}

/**************************************/
static int GetCheck(BYTE* ary, int no)									//<<< ﾁｪｯｸ取得
/**************************************/
{
	BYTE bt;
	int  i, j;

	i = no/8, j = no%8;
	bt = ary[i], bt >>= 7-j, i = bt & 0x1;
	return i;
}

/***********************************/
static BOOL CheckCmdArea(BYTE* cmd)										//<<< ｺﾏﾝﾄﾞﾚﾍﾞﾙNoﾁｪｯｸ
/***********************************/
{
	char*   cc = new char[100];
	FILE*   fp;
	CString bfile;
	BOOL    ret=FALSE;
	int     i, dmy1, clvl, ono, cn, in, pn;
	float   dmy2;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BOD_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "r")==0) {									// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ﾍｯﾀﾞｰ情報取得
			} else {
				ret = TRUE;												// ｺﾏﾝﾄﾞ情報取得
			}
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(cc[0]!='C') {
				ret = FALSE; break;										// ﾃﾞｰﾀ異常で終了
			}
			sscanf_s(&cc[1], "%d", &cn);								// 数値変換
			for(i=0; i<in; i++) {
				if(fscanf_s(fp, "%d", &dmy1)!=1) {						// 整数ﾊﾟﾗﾒｰﾀ取得
					ret = FALSE; break;
				}
			}
			for(i=0; i<pn; i++) {										// 座標ﾊﾟﾗﾒｰﾀ取得
				if(fscanf_s(fp, "%f%f%f", &dmy2, &dmy2, &dmy2)!=3) {
					ret = FALSE; break;
				}
			}
			if(cn==UNDOCMD) {
				UpdateCheck(cmd, clvl, 0);								// ｺﾏﾝﾄﾞ無効の場合
			} else if(cn==REDOCMD) {
				UpdateCheck(cmd, clvl, 1);								// ｺﾏﾝﾄﾞ有効の場合
			}
			if(!ret) {
				break;
			}
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] cc;
	return ret;
}

/**********************************************/
static BOOL CheckObjArea(BYTE* cmd, BYTE* obj)							//<<< 立体Noﾁｪｯｸ
/**********************************************/
{
	char*   cc = new char[100];
	FILE*   fp;
	CString bfile;
	BOOL    ret=FALSE;
	int     i, dmy1, clvl, ono, cn, in, pn;
	float   dmy2;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BOD_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "r")==0) {									// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ﾍｯﾀﾞｰ情報取得
			} else {
				ret = TRUE;												// ｺﾏﾝﾄﾞ情報取得
			}
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(cc[0]!='C') {
				ret = FALSE; break;										// ﾃﾞｰﾀ異常で終了
			}
			sscanf_s(&cc[1], "%d", &cn);								// 数値変換
			for(i=0; i<in; i++) {
				if(fscanf_s(fp, "%d", &dmy1)!=1) {						// 整数ﾊﾟﾗﾒｰﾀ取得
					ret = FALSE; break;
				}
			}
			for(i=0; i<pn; i++) {										// 座標ﾊﾟﾗﾒｰﾀ取得
				if(fscanf_s(fp, "%f%f%f", &dmy2, &dmy2, &dmy2)!=3) {
					ret = FALSE; break;
				}
			}
			if(cn==CRTSURF||cn==DIRECT||cn==CRTCURVE||cn==SETSURF||
				cn==SETCURVE||cn==THICKNESS||cn==CRTSWEEP) {			// <生成ｺﾏﾝﾄﾞ>
				if(GetCheck(cmd, clvl)) {
					UpdateCheck(obj, ono, 1);							// 立体有効
				}
			}
			if(cn==DELCMD) {											// 立体削除で
				if(GetCheck(cmd, clvl)) {
					UpdateCheck(obj, ono, 0);							// 立体無効
				}
			}
			if(!ret) {
				break;
			}
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] cc;
	return ret;
}

/*********************************************/
static BOOL RestoreObject(int no, BYTE* cmd)							//<<< 立体復元
/*********************************************/
{
    char*    cc = new char[100];
    char*    b1 = new char[100];
    char*    b2 = new char[100];
    char*    b3 = new char[100];
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	BOOL     ret=FALSE;
	int      i, j, clvl, ono, cn, in, pn, sw, ok=0;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	bfile = GetAppPath(0)+BOD_NAME;										// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名取得
	if(fopen_s(&fp, bfile, "r")==0) {									// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙOPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ﾍｯﾀﾞｰ情報取得
			} else {
				ret=TRUE;												// ｺﾏﾝﾄﾞ情報取得
			}
			sw = (no==ono) ? 1 : 0;										// 対象立体ﾁｪｯｸ
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(sw) {
				if(cc[0]!='C') {
					ret=FALSE; break;									// ﾃﾞｰﾀ異常で終了
				}
				sscanf_s(&cc[1], "%d", &cn);							// 数値変換
				sw = (cn>MAXCMD) ? 0 : sw;								// 立体ｺﾏﾝﾄﾞﾁｪｯｸ
				if(sw) {
					sw = (!GetCheck(cmd, clvl)) ? 0 : sw;				// 有効ｺﾏﾝﾄﾞﾁｪｯｸ
				}
			}
			if(sw) {													// <ｺﾏﾝﾄﾞ出力OK>
				CreatCmd(cn, in, pn, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<in; i++) {
					if(fscanf_s(fp, "%d", &j)!=1) {						// 整数ﾊﾟﾗﾒｰﾀ取得
						ret = FALSE; break;
					}
					PutParaInt(cmp, i, j);								// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				for(i=0; i<pn; i++) {                                   // 座標ﾊﾟﾗﾒｰﾀ取得
					if(fscanf_s(fp, "%s%s%s", b1, 100, b2, 100, b3, 100)!=3) {
						ret = FALSE; break;
					}
					p.p[0] = atof(b1), p.p[1] = atof(b2), p.p[2] = atof(b3);
					PutParaPnt(cmp, i, p);								// 座標ﾊﾟﾗﾒｰﾀ設定
				}
				if(!ret) {
					break;
				}
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
				ok = 1;
			} else {													// <ｺﾏﾝﾄﾞ出力NG>
				for(i=0; i<in; i++) {
					if(fscanf_s(fp, "%d", &j)!=1) {						// 整数ﾊﾟﾗﾒｰﾀ取得
						ret = FALSE; break;
					}
				}
				for(i=0; i<pn; i++) {			                        // 座標ﾊﾟﾗﾒｰﾀ取得
					if(fscanf_s(fp, "%s%s%s", b1, 100, b2, 100, b3, 100)!=3) {
						ret = FALSE; break;
					}
				}
				if(!ret) {
					break;
				}
			}
		}
		fclose(fp);														// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙCLOSE
	}
	if(ok) {															// <ｺﾏﾝﾄﾞ出力済>
		GetObjPtr(&op);													// 立体ﾎﾟｲﾝﾀ取得
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
    delete[] cc;
    delete[] b1;
    delete[] b2;
    delete[] b3;
	return ret;
}

/************************/
BOOL RestoreBackUp(void)												//<<< ﾒﾆｭｰ復旧処理
/************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	BYTE* cmd = new BYTE[1500];
	BYTE* obj = new BYTE[1500];
	int   i;

	if(Demo_Flg!=0) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// 体験版は中止
	}
	for(i=0; i<1500; i++) {
		cmd[i] = 0xff;													// ｺﾏﾝﾄﾞﾁｪｯｸ初期化(All-OK)
		obj[i] = 0x0;													// 立体ﾁｪｯｸ初期化(All-NG)
	}
	if(!CheckCmdArea(cmd)) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// ｺﾏﾝﾄﾞﾚﾍﾞﾙNoﾁｪｯｸ
	}
	if(!CheckObjArea(cmd, obj)) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// 立体Noﾁｪｯｸ
	}
	for(i=0; i<12000; i++) {
		if(GetCheck(obj, i)) {											// 有効立体の場合
			if(!RestoreObject(i, cmd)) {								// 立体復元
				delete[] cmd;
				delete[] obj;
				return FALSE;
			}
			View->Display(0);											// 再描画
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	delete[] cmd;
	delete[] obj;
	return TRUE;
}

/**********************************************************/
/*****                  データ使用率用                *****/
/**********************************************************/
/****************************/
void MemErr(unsigned int id)											//<<< ﾒﾓﾘ異常ﾒｯｾｰｼﾞ
/****************************/
{
	AfxMessageBox(id);													// ｴﾗｰﾒｯｾｰｼﾞ出力
	MainWnd->DestroyWindow();											// JCAD3終了
	exit(1);															// 終了
}

/**********************/
double GetIntUse(void)													//<<< 整数ﾊﾟﾗﾒｰﾀ使用率取得
/**********************/
{
	double par;

	if((par=((double)Int_adr/Int_max)*100.0)>=100.0) {
		MemErr(IDS_MEMERR1);
	}
	return par;
}

/**********************/
double GetPntUse(void)													//<<< 頂点ﾊﾟﾗﾒｰﾀ使用率取得
/**********************/
{
	double par;

	if((par=((double)Pnt_adr/Pnt_max)*100.0)>=100.0) {
		MemErr(IDS_MEMERR2);
	}
	return par;
}

/**********************/
int GetIntUseNum(void)													//<<< 整数ﾊﾟﾗﾒｰﾀ使用数取得
/**********************/
{
	return Int_adr;
}

/**********************/
int GetPntUseNum(void)													//<<< 頂点ﾊﾟﾗﾒｰﾀ使用数取得
/**********************/
{
	return Pnt_adr;
}

/**********************************************************/
/*****                    コマンド用                  *****/
/**********************************************************/
/*******************************************************/
void CreatCmd(int cno, int ino, int pno, CMDTYPE** cmp)					//<<< ｺﾏﾝﾄﾞ領域確保
/*******************************************************/
{
	if(Gc_cmp==NULL) {													// <GC無の場合>
		*cmp = (CMDTYPE *)malloc(sizeof(CMDTYPE));						// 新規領域確保
		if(*cmp==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC有の場合>
		*cmp = Gc_cmp, Gc_cmp = Gc_cmp->onxt;							// GC先頭使用
	}
	(*cmp)->inum = ino;													// 整数ﾊﾟﾗﾒｰﾀ領域確保
	(*cmp)->isa = Int_adr, Int_adr += ino;
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR1);
	}
	(*cmp)->pnum = pno;													// 座標ﾊﾟﾗﾒｰﾀ領域確保
	(*cmp)->psa = Pnt_adr, Pnt_adr += pno;
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR2);
	}
	(*cmp)->cno = (BYTE)cno;											// ｺﾏﾝﾄﾞNo設定
	(*cmp)->clvl = Cmd_Lvl;												// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo設定
	(*cmp)->onxt = NULL;												// ﾎﾟｲﾝﾀ初期化
	(*cmp)->lnxt = NULL;
	(*cmp)->op = NULL;
}

/**********************************************/
void ChangeCmd(CMDTYPE* cmp, int ino, int pno)							//<<< ｺﾏﾝﾄﾞ領域変更
/**********************************************/
{
	int oin, oia, opn, opa, src, chn, i;

	if(cmp->inum==ino&&cmp->pnum==pno) {
		return;															// 領域変更無中止
	}
	oin = cmp->inum, oia = Int_adr;										// 旧領域数保存
	opn = cmp->pnum, opa = Pnt_adr;
	cmp->inum = ino, Int_adr += ino-oin;								// 整数ﾊﾟﾗﾒｰﾀ領域確保
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR1);
	}
	cmp->pnum = pno, Pnt_adr += pno-opn;								// 座標ﾊﾟﾗﾒｰﾀ領域確保
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR2);
	}
	src = cmp->isa+oin;													// 領域ｼﾌﾄ(整数ﾊﾟﾗﾒｰﾀ)
	if(oia<Int_adr) {
		chn = Int_adr-oia;
		if(chn!=0) {
			for(i=oia-1; i>=src; i--) {
				Int_ary[i+chn] = Int_ary[i];
			}
		}
	} else {
		chn = oia-Int_adr;
		if(chn!=0) {
			for(i=src; i<oia; i++) {
				Int_ary[i-chn] = Int_ary[i];
			}
		}
	}
	src = cmp->psa+opn;													// 領域ｼﾌﾄ(座標ﾊﾟﾗﾒｰﾀ)
	if(opa<Pnt_adr) {
		chn = Pnt_adr-opa;
		if(chn!=0) {
			for(i=opa-1; i>=src; i--) {
				Pnt_ary[i+chn] = Pnt_ary[i];
			}
		}
	} else {
		chn = opa-Pnt_adr;
		if(chn!=0) {
			for(i=src; i<opa; i++) {
				Pnt_ary[i-chn] = Pnt_ary[i];
			}
		}
	}
	ChangeAreaAdr(cmp->isa, ino-oin, cmp->psa, pno-opn);				// 配列ｱﾄﾞﾚｽ変更
}

/***************************************/
void TempCmd(OBJTYPE* op, CMDTYPE* cmp)									//<<< ｺﾏﾝﾄﾞ一時削除
/***************************************/
{
	CMDTYPE* cptr;

	if(op->bcmp==cmp) {													// <先頭ｺﾏﾝﾄﾞの場合>
		op->bcmp = cmp->onxt;											// 次ｺﾏﾝﾄﾞ立体接続
		if(op->ccmp==cmp) {
			op->ccmp = cmp->onxt;										// 有効先頭ｺﾏﾝﾄﾞ->次ｺﾏﾝﾄﾞ
		}
	} else {															// <先頭以外ｺﾏﾝﾄﾞの場合>
		if((cptr=op->bcmp)==NULL) {
			return;
		}
		while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
			cptr = cptr->onxt;											// 前ｺﾏﾝﾄﾞ検索
		}
		if(cptr->onxt==NULL) {
			return;														// 指定ｺﾏﾝﾄﾞ無中止
		}
		cptr->onxt = cmp->onxt;											// 前ｺﾏﾝﾄﾞ次ｺﾏﾝﾄﾞ接続
		if(op->ccmp==cmp) {
			op->ccmp = cptr;											// 有効先頭ｺﾏﾝﾄﾞ->前ｺﾏﾝﾄﾞ
		}
		if(op->rscmp==cmp) {											// <ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ設定有の場合>
			op->rscmp = NULL;											// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==RING) {
					op->rscmp = cptr;									// 最初のﾘﾝｸﾞｺﾏﾝﾄ設定
					break;
				}
				cptr = cptr->onxt;
			}
		}
		if(op->rccmp==cmp) {											// <ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ設定有の場合>
			op->rccmp = NULL;											// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==RING) {
					op->rccmp = cptr;									// 最後のﾘﾝｸﾞｺﾏﾝﾄ設定
				}
				cptr = cptr->onxt;
			}
		}
		if(op->ecmp==cmp) {												// <ｱｰｽｺﾏﾝﾄﾞ設定有の場合>
			op->ecmp = NULL;											// ｱｰｽｺﾏﾝﾄﾞ初期化
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==EARTH) {
					op->ecmp = cptr;									// 最後のｱｰｽｺﾏﾝﾄﾞ設定
				}
				cptr = cptr->onxt;
			}
		}
	}
	cmp->onxt = Temp_cmp, Temp_cmp = cmp;								// Temp先頭接続
}

/************************************************/
void FreeCmd(OBJTYPE* op, CMDTYPE* cmp, int flg)						//<<< ｺﾏﾝﾄﾞ領域解放
/************************************************/
{
	CMDTYPE* cptr;
	int      src, i;

	if(op!=NULL) {														// <立体指定の場合>
		if(op->bcmp==cmp) {												// <先頭ｺﾏﾝﾄﾞの場合>
			op->bcmp = cmp->onxt;										// 次ｺﾏﾝﾄﾞ立体接続
			if(op->ccmp==cmp) {
				op->ccmp = cmp->onxt;									// 有効先頭ｺﾏﾝﾄﾞ->次ｺﾏﾝﾄﾞ
			}
		} else {														// <先頭以外ｺﾏﾝﾄﾞの場合>
			if((cptr=op->bcmp)==NULL) {
				return;
			}
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;										// 前ｺﾏﾝﾄﾞ検索
			}
			if(cptr->onxt==NULL) {
				return;													// 指定ｺﾏﾝﾄﾞ無中止
			}
			cptr->onxt = cmp->onxt;										// 前ｺﾏﾝﾄﾞ次ｺﾏﾝﾄﾞ接続
			if(op->ccmp==cmp) {
				op->ccmp = cptr;										// 有効先頭ｺﾏﾝﾄﾞ->前ｺﾏﾝﾄﾞ
			}
			if(op->rscmp==cmp) {										// <ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ設定有の場合>
				op->rscmp = NULL;										// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==RING) {
						op->rscmp = cptr;								// 最初のﾘﾝｸﾞｺﾏﾝﾄ設定
						break;
					}
					cptr = cptr->onxt;
				}
			}
			if(op->rccmp==cmp) {										// <ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ設定有の場合>
				op->rccmp = NULL;										// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==RING) {
						op->rccmp = cptr;								// 最後のﾘﾝｸﾞｺﾏﾝﾄ設定
					}
					cptr = cptr->onxt;
				}
			}
			if(op->ecmp==cmp) {											// <ｱｰｽｺﾏﾝﾄﾞ設定有の場合>
				op->ecmp = NULL;										// ｱｰｽｺﾏﾝﾄﾞ初期化
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==EARTH) {
						op->ecmp = cptr;								// 最後のｱｰｽｺﾏﾝﾄﾞ設定
					}
					cptr = cptr->onxt;
				}
			}
		}
	} else {															// <立体無指定の場合>
		if(Temp_cmp==cmp) {												// <Temp先頭の場合>
			Temp_cmp = cmp->onxt;										// 次ｺﾏﾝﾄﾞTemp接続
		} else {														// <Temp先頭でない場合>
			if((cptr=Temp_cmp)==NULL) {
				return;													// Tempよりｺﾏﾝﾄﾞ検索
			}
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;
			}
			if(cptr->onxt==NULL) {
				return;													// 指定ｺﾏﾝﾄﾞ無中止
			}
			cptr->onxt = cmp->onxt;										// 指定前後接続
		}
	}
	cmp->onxt = Gc_cmp, Gc_cmp = cmp;									// GC先頭接続
	if(flg) {															// <Undo/Redo解除>
		if(Undo_ary[Undo_adr].cmp==cmp) {								// <ﾚﾍﾞﾙ先頭の場合>
			Undo_ary[Undo_adr].cmp = cmp->lnxt;							// 次ｺﾏﾝﾄﾞ先頭接続
		} else {														// <ﾚﾍﾞﾙ先頭でない場合>
			if((cptr=Undo_ary[Undo_adr].cmp)!=NULL) {					// 先頭よりｺﾏﾝﾄﾞ検索
				while(cptr->lnxt!=NULL&&cptr->lnxt!=cmp) {
					cptr = cptr->lnxt;
				}
				if(cptr->lnxt==cmp) {
					cptr->lnxt = cmp->lnxt;								// 指定前後接続
				}
			}
		}
	}
	if(cmp->inum!=0) {													// 整数ﾊﾟﾗﾒｰﾀ領域解放
		src = cmp->isa+cmp->inum;
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-cmp->inum] = Int_ary[i];
		}
		Int_adr -= cmp->inum;
	}
	if(cmp->pnum!=0) {													// 座標ﾊﾟﾗﾒｰﾀ領域解放
		src = cmp->psa+cmp->pnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-cmp->pnum] = Pnt_ary[i];
		}
		Pnt_adr -= cmp->pnum;
	}
	ChangeAreaAdr(cmp->isa, -cmp->inum, cmp->psa, -cmp->pnum);			// 配列ｱﾄﾞﾚｽ変更
}

/*****************************************/
void AppendCmd(OBJTYPE* op, CMDTYPE* cmp)								//<<< ｺﾏﾝﾄﾞ&立体接続
/*****************************************/
{
	CMDTYPE* cptr;

	if(Reset_op!=NULL) {
		return;															// 接続済みは中止する
	}
	cmp->op = op;														// OP設定
	if(op->bcmp==NULL) {												// <生成ｺﾏﾝﾄﾞの場合>
		op->bcmp = cmp, op->ccmp = cmp;									// 立体ｺﾏﾝﾄﾞ接続
	} else {															// <編集ｺﾏﾝﾄﾞの場合>
		if(cmp->cno==CHGSURF||cmp->cno==CHGCURVE||cmp->cno==CHGSWEEP) {	// <曲面/曲線変更の場合>
			cptr = op->ccmp;											// 有効先頭ｺﾏﾝﾄﾞ次接続
			cmp->onxt = cptr->onxt, cptr->onxt = cmp;
			op->ccmp = cmp;												// 有効先頭ｺﾏﾝﾄﾞ変更
		} else {														// <曲面編集以外の場合>
			cptr = op->bcmp;
			while(cptr->onxt!=NULL) {
				cptr = cptr->onxt;										// 最終ｺﾏﾝﾄﾞ検索
			}
			cptr->onxt = cmp;											// 最終接続
			if(cmp->cno==RING) {										// <ﾘﾝｸﾞｺﾏﾝﾄﾞの場合>
				if(op->rscmp==NULL) {
					op->rscmp = cmp;									// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ変更
				}
				op->rccmp = cmp;										// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ変更
			}
			if(cmp->cno==EARTH) {										// <ｱｰｽｺﾏﾝﾄﾞの場合>
				op->ecmp = cmp;											// ｱｰｽｺﾏﾝﾄﾞ変更
			}
		}
	}
}

/***************************************/
void UndoCmd(OBJTYPE* op, CMDTYPE* cmp)									//<<< ｺﾏﾝﾄﾞ&立体再接続
/***************************************/
{
	CMDTYPE* cptr;

	if(Temp_cmp==cmp) {													// <Temp先頭の場合>
		Temp_cmp = cmp->onxt;											// 次ｺﾏﾝﾄﾞTemp接続
	} else {															// <Temp先頭でない場合>
		if((cptr=Temp_cmp)==NULL) {
			return;
		}
		while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
			cptr = cptr->onxt;											// Tempよりｺﾏﾝﾄﾞ検索
		}
		if(cptr->onxt==NULL) {
			return;														// 指定ｺﾏﾝﾄﾞ無中止
		}
		cptr->onxt = cmp->onxt;											// 指定前後接続
	}
	cmp->onxt = NULL;
	cmp->op = op;														// OP設定
	if(op->bcmp==NULL) {												// <生成ｺﾏﾝﾄﾞの場合>
		op->bcmp = cmp, op->ccmp = cmp;									// 立体ｺﾏﾝﾄﾞ接続
	} else {															// <編集ｺﾏﾝﾄﾞの場合>
		if(cmp->cno==CHGSURF||cmp->cno==CHGCURVE||cmp->cno==CHGSWEEP) {	// <曲面/曲線変更の場合>
			cptr = op->ccmp;											// 有効先頭ｺﾏﾝﾄﾞ次接続
			cmp->onxt = cptr->onxt, cptr->onxt = cmp;
			op->ccmp = cmp;												// 有効先頭ｺﾏﾝﾄﾞ変更
		} else {														// <曲面編集以外の場合>
			cptr = op->bcmp;
			while(cptr->onxt!=NULL) {
				cptr = cptr->onxt;										// 最終ｺﾏﾝﾄﾞ検索
			}
			cptr->onxt = cmp;											// 最終接続
			if(cmp->cno==RING) {										// <ﾘﾝｸﾞｺﾏﾝﾄﾞの場合>
				if(op->rscmp==NULL) {
					op->rscmp = cmp;									// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ変更
				}
				op->rccmp = cmp;										// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ変更
			}
			if(cmp->cno==EARTH) {										// <ｱｰｽｺﾏﾝﾄﾞの場合>
				op->ecmp = cmp;											// ｱｰｽｺﾏﾝﾄﾞ変更
			}
		}
	}
}

/***************************/
void LevelCmd(CMDTYPE* cmp)												//<<< 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
/***************************/
{
	CMDTYPE* cptr;
	int      wadr;

    if(Undo_stp==1) {
		return;															// Undo保存停止中中止
	}
	if(Reset_op!=NULL) {
		return;															// 接続済中止
	}
	if(Undo_ary[Undo_adr].cmp==NULL) {									// <先頭ｺﾏﾝﾄﾞの場合>
		Undo_ary[Undo_adr].cmp = cmp;									// 配列接続
		Undo_ary[Undo_adr].urflg = 1;									// 今回ﾒﾆｭｰUndo可
		Undo_ary[Undo_adr].cmflg = 0;									// 前回ﾒﾆｭｰRedo可
		wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;
		Undo_ary[wadr].cmflg = 1;										// 今回ﾒﾆｭｰRedo不可
	} else {															// <先頭以外ｺﾏﾝﾄﾞの場合>
		cptr = Undo_ary[Undo_adr].cmp;
		while(cptr->lnxt!=NULL) {
			cptr = cptr->lnxt;											// 最終ｺﾏﾝﾄﾞ検索
		}
		cptr->lnxt = cmp;												// 最終接続
	}
}

/*****************************************/
void DetachCmd(OBJTYPE* op, CMDTYPE* cmp)								//<<< ｺﾏﾝﾄﾞ一時切離し
/*****************************************/
{
	CMDTYPE* cptr;

	if(op->bcmp!=NULL) {												// <ｺﾏﾝﾄﾞ有の場合>
		cptr = op->bcmp;
		if(cptr==cmp) {													// <先頭指定の場合>
			Tach_cmp = cptr->onxt;										// 指定次ﾎﾟｲﾝﾀ保存
			cptr->onxt = NULL;											// ﾎﾟｲﾝﾀ切離し
		} else {														// <先頭指定以外の場合>
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;
			}
			if(cptr->onxt==cmp) {										// <指定ﾎﾟｲﾝﾀ有の場合>
				cptr = cptr->onxt;										// 指定ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
				Tach_cmp = cptr->onxt;									// 指定次ﾎﾟｲﾝﾀ保存
				cptr->onxt = NULL;										// ﾎﾟｲﾝﾀ切離し
			}
		}
		op->rscmp = NULL;												// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rscmp = cptr;										// 最初のﾘﾝｸﾞｺﾏﾝﾄ設定
				break;
			}
			cptr = cptr->onxt;
		}
		op->rccmp = NULL;												// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rccmp = cptr;										// 最後のﾘﾝｸﾞｺﾏﾝﾄ設定
			}
			cptr = cptr->onxt;
		}
		op->ecmp = NULL;												// ｱｰｽｺﾏﾝﾄﾞ初期化
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==EARTH) {
				op->ecmp = cptr;										// 最後のｱｰｽｺﾏﾝﾄﾞ設定
			}
			cptr = cptr->onxt;
		}
	}
}

/***************************/
void AttachCmd(OBJTYPE* op)												//<<< ｺﾏﾝﾄﾞ再接続
/***************************/
{
	CMDTYPE* cptr;

	if(op->bcmp!=NULL) {												// <ｺﾏﾝﾄﾞ有の場合>
		cptr = op->bcmp;
		while(cptr->onxt!=NULL) {
			cptr = cptr->onxt;											// 最終ｺﾏﾝﾄﾞ検索
		}
		cptr->onxt = Tach_cmp;											// 最終再接続
	}
	Tach_cmp = NULL;													// 保存領域初期化
	op->rscmp = NULL;													// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==RING) {
			op->rscmp = cptr;											// 最初のﾘﾝｸﾞｺﾏﾝﾄ設定
			break;
		}
		cptr = cptr->onxt;
	}
	op->rccmp = NULL;													// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==RING) {
			op->rccmp = cptr;											// 最後のﾘﾝｸﾞｺﾏﾝﾄ設定
		}
		cptr = cptr->onxt;
	}
	op->ecmp = NULL;													// ｱｰｽｺﾏﾝﾄﾞ初期化
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==EARTH) {
			op->ecmp = cptr;											// 最後のｱｰｽｺﾏﾝﾄﾞ設定
		}
		cptr = cptr->onxt;
	}
}

/*****************************/
void InitCmdLvl(CMDTYPE* cmp)											//<<< ｺﾏﾝﾄﾞﾚﾍﾞﾙ初期化
/*****************************/
{
	cmp->clvl = Cmd_Lvl;
}

/**************************************/
void SetCmdLvl(CMDTYPE* cmp, int clvl)									//<<< ｺﾏﾝﾄﾞﾚﾍﾞﾙ設定
/**************************************/
{
	cmp->clvl = clvl;
}

/***************************/
int GetCmdLvl(CMDTYPE* cmp)												//<<< ｺﾏﾝﾄﾞﾚﾍﾞﾙ取得
/***************************/
{
	return cmp->clvl;
}

/****************************************************/
void BaseCmdPtr(int flg, OBJTYPE* op, CMDTYPE** cmp)					//<<< 先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
/****************************************************/
{
	*cmp = (!flg) ? op->bcmp : op->ccmp;								// ﾍﾞｰｽｺﾏﾝﾄﾞ/有効先頭ｺﾏﾝﾄﾞ
}

/******************************/
void NextCmdPtr(CMDTYPE** cmp)											//<<< 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
/******************************/
{
	OBJTYPE* op;
	BOOL     nxt=FALSE;

	if(*cmp!=NULL) {
		op = (*cmp)->op;
		if(*cmp==op->rccmp) {											// 現ｺﾏﾝﾄﾞがﾘﾝｸﾞ現在ｺﾏﾝﾄﾞの場合は
			*cmp = op->rscmp;											// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞへ置き換える
		}
		if(*cmp==op->ccmp) {											// <現ｺﾏﾝﾄﾞが有効先頭ｺﾏﾝﾄﾞの場合>
			if(op->ecmp!=NULL) {										// ｱｰｽｺﾏﾝﾄﾞが有効で
				if(op->ecmp!=op->ccmp) {								// 有効先頭ｺﾏﾝﾄﾞと異なる場合は
					*cmp = op->ecmp;									// 次ｺﾏﾝﾄﾞはｱｰｽｺﾏﾝﾄﾞ
				} else {												// 有効先頭ｺﾏﾝﾄﾞと同じ場合は
					nxt = TRUE;											// 次ｺﾏﾝﾄﾞを検索へ
				}
			} else {													// ｱｰｽｺﾏﾝﾄﾞが無効な場合は
				nxt = TRUE;												// 次ｺﾏﾝﾄﾞを検索へ
			}
		} else if(*cmp==op->ecmp) {										// <現ｺﾏﾝﾄﾞがｱｰｽｺﾏﾝﾄﾞの場合>
			*cmp = (op->ccmp)->onxt;									// 次ｺﾏﾝﾄﾞは有効先頭ｺﾏﾝﾄﾞの次
			if((*cmp)!=NULL) {
				if(*cmp==op->ecmp||
                   ((*cmp)->cno==RING&&*cmp!=op->rscmp)) {				// 次がｱｰｽ又は無効なﾘﾝｸﾞｺﾏﾝﾄﾞの場合は
					nxt = TRUE;											// 次ｺﾏﾝﾄﾞを検索へ
				}
			}
		} else {														// <現ｺﾏﾝﾄﾞが上記以外の場合>
			nxt = TRUE;													// 次ｺﾏﾝﾄﾞを検索へ
		}
		while(nxt&&(*cmp)!=NULL) {										// <次ｺﾏﾝﾄﾞ検索あり>
			*cmp = (*cmp)->onxt;										// 次ｺﾏﾝﾄﾞ
			if(*cmp==NULL) {
				break;
			}
			if(((*cmp)->cno!=EARTH&&(*cmp)->cno!=RING)||				// 次ｺﾏﾝﾄﾞがｱｰｽ&ﾘﾝｸﾞ以外か
               (*cmp)==op->rscmp) {										// 有効なﾘﾝｸﾞｺﾏﾝﾄﾞは検索終了
				break;
			}
		}
		if(*cmp!=NULL) {
			if(*cmp==op->rscmp) {										// 次ｺﾏﾝﾄﾞがﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞの場合は
				*cmp = op->rccmp;										// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞへ置き換える
			}
		}
	}
}

/*********************************/
void NextAllCmdPtr(CMDTYPE** cmp)										//<<< 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ(全ｺﾏﾝﾄﾞ対象)取得
/*********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->onxt;											// 次ｺﾏﾝﾄﾞ(無条件)
	}
}

/***********************************/
void LevelBaseCmdPtr(CMDTYPE** cmp)										//<<< 同一ﾚﾍﾞﾙ内先頭ｺﾏﾝﾄﾞ取得
/***********************************/
{
	*cmp = Undo_ary[Undo_adr].cmp;
}

/***********************************/
void LevelNextCmdPtr(CMDTYPE** cmp)										//<<< 同一ﾚﾍﾞﾙ内次ｺﾏﾝﾄﾞ取得
/***********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->lnxt;											// 次ｺﾏﾝﾄﾞ設定
	}
}

/****************************************************/
void EditCmdPtr(int flg, OBJTYPE* op, CMDTYPE** cmp)					//<<< 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
/****************************************************/					// flg 0:ﾘﾝｸﾞ先頭, 1:ﾘﾝｸﾞ現在, 2:ｱｰｽ
{
	*cmp = (flg==0) ? op->rscmp : ((flg==1) ? op->rccmp : op->ecmp);	// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ/ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ/ｱｰｽｺﾏﾝﾄﾞ
}

/******************************************************/
void SetEditCmdPtr(int flg, OBJTYPE* op, CMDTYPE* cmp)					//<<< 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ設定
/******************************************************/				// flg 0:ﾘﾝｸﾞ先頭, 1:ﾘﾝｸﾞ現在, 2:ｱｰｽ
{
	CMDTYPE* cptr;

	cptr = op->ccmp;
	if(flg==0) {														// <ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞの場合>
		op->rscmp = NULL;												// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rscmp = cptr;										// 最初のﾘﾝｸﾞｺﾏﾝﾄ設定
				break;
			}
			cptr = cptr->onxt;
		}
	} else if(flg==1) {													// <ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞの場合>
		op->rccmp = NULL;												// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rccmp = cptr;										// 最後のﾘﾝｸﾞｺﾏﾝﾄ設定
			}
			cptr = cptr->onxt;
		}
	} else {															// <ｱｰｽｺﾏﾝﾄﾞの場合>
		op->ecmp = NULL;												// ｱｰｽｺﾏﾝﾄﾞ初期化
		while(cptr!=NULL) {
			if(cptr->cno==EARTH) {
				op->ecmp = cptr;										// 最後のｱｰｽｺﾏﾝﾄﾞ設定
			}
			cptr = cptr->onxt;
		}
	}
}

/******************************************/
void ResetEditCmdPtr(int flg, OBJTYPE* op)								//<<< 編集ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ解除
/******************************************/							// flg 0:ﾘﾝｸﾞ先頭, 1:ﾘﾝｸﾞ現在, 2:ｱｰｽ
{
	if(flg==0) {
		op->rscmp = NULL;												// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞ初期化
	} else if(flg==1) {
		op->rccmp = NULL;												// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞ初期化
	} else {
		op->ecmp = NULL;												// ｱｰｽｺﾏﾝﾄﾞ初期化
	}
}

/****************************/
void SetCmdPtr(CMDTYPE* cmp)											//<<< 処理対象ｺﾏﾝﾄﾞ設定
/****************************/
{
	Rf_cmp = cmp;
}

/*****************************/
void GetCmdPtr(CMDTYPE** cmp)											//<<< 処理対象ｺﾏﾝﾄﾞ取得
/*****************************/
{
	*cmp = Rf_cmp;
}

/******************************************/
void GetCmdObj(CMDTYPE* cmp, OBJTYPE** op)								//<<< ｺﾏﾝﾄﾞ対象立体取得
/******************************************/
{
	*op = cmp->op;
}

/**************************/
int GetCmdNo(CMDTYPE* cmp)												//<<< ｺﾏﾝﾄﾞNo取得
/**************************/
{
	return (int)cmp->cno;
}

/*************************************/
int GetObjCmdNo(int flg, OBJTYPE* op)									//<<< 立体ｺﾏﾝﾄﾞNo取得
/*************************************/									// 0:先頭ｺﾏﾝﾄﾞ, 1:有効先頭ｺﾏﾝﾄﾞ
{
	CMDTYPE* cmp;
	int      cno=0;

	if(op!=NULL) {
		cmp = (flg==0) ? op->bcmp : op->ccmp;
		cno = (cmp!=NULL) ? (int)cmp->cno : 0;
	}
	return cno;
}

/***************************/
int GetIntNum(CMDTYPE *cmp)												//<<< 整数ﾊﾟﾗﾒｰﾀ数取得
/***************************/
{
	return cmp->inum;
}

/***********************************************/
void PutParaInt(CMDTYPE* cmp, int no, int data)							//<<< 整数ﾊﾟﾗﾒｰﾀ値設定
/***********************************************/
{
	Int_ary[cmp->isa+no] = data;
}

/************************************/
int GetParaInt(CMDTYPE* cmp, int no)									//<<< 整数ﾊﾟﾗﾒｰﾀ値取得
/************************************/
{
	return Int_ary[cmp->isa+no];
}

/***************************/
int GetPntNum(CMDTYPE* cmp)												//<<< 座標ﾊﾟﾗﾒｰﾀ数取得
/***************************/
{
	return cmp->pnum;
}

/***************************************************/
void PutParaPnt(CMDTYPE* cmp, int no, PNTTYPE data)						//<<< 座標ﾊﾟﾗﾒｰﾀ値設定
/***************************************************/
{
	Pnt_ary[cmp->psa+no] = data;
}

/****************************************************/
void GetParaPnt(CMDTYPE* cmp, int no, PNTTYPE* data)					//<<< 座標ﾊﾟﾗﾒｰﾀ値取得
/****************************************************/
{
	*data = Pnt_ary[cmp->psa+no];
}

/**********************/
void OnCmdFlgStp(void)													//<<< 保存ﾌﾗｸﾞ更新中止ON
/**********************/
{
	Cmd_upd = 1;
}

/***********************/
void OffCmdFlgStp(void)													//<<< 保存ﾌﾗｸﾞ更新中止OFF
/***********************/
{
	Cmd_upd = 0;
}

/*******************/
void OnCmdFlg(void)														//<<< ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞON
/*******************/
{
	Cmd_flg = (Cmd_upd!=1) ? 1 : Cmd_flg;
}

/********************/
void OffCmdFlg(void)													//<<< ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞOFF
/********************/
{
	Cmd_flg = 0;
}

/**********************/
BOOL CheckCmdFlg(void)													//<<< ｺﾏﾝﾄﾞ未保存ﾌﾗｸﾞ確認
/**********************/
{
	if(Cmd_flg==1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**********************/
void OnCmdExeStp(void)													//<<< ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞON
/**********************/
{
	Cmd_stp = 1;
}

/***********************/
void OffCmdExeStp(void)													//<<< ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞOFF
/***********************/
{
	Cmd_stp = 0;
}

/**********************/
int GetCmdExeStp(void)													//<<< ｺﾏﾝﾄﾞ実行停止ﾌﾗｸﾞ取得
/**********************/
{
	return (int)Cmd_stp;
}

/**********************************************************/
/*****                コピー コマンド用               *****/
/**********************************************************/
/*************************************************************/
void CreatCopyCmd(int cno, int inum, int pnum, CPCTYPE** cmp)			//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ領域確保
/*************************************************************/
{
	CPCTYPE* cpc;

	*cmp = (CPCTYPE *)malloc(sizeof(CPCTYPE));							// 新規領域確保
	if(*cmp==NULL) {
		MemErr(IDS_MEMERR5);
	}
	(*cmp)->inum = inum;												// 整数ﾊﾟﾗﾒｰﾀ領域確保
	if(inum) {
		(*cmp)->isa = Int_adr, Int_adr += inum;
		if(Int_adr>=Int_max) {
			MemErr(IDS_MEMERR1);
		}
	} else {
		(*cmp)->isa = 0;
	}
	(*cmp)->pnum = pnum;												// 座標ﾊﾟﾗﾒｰﾀ領域確保
	if(pnum) {
		(*cmp)->psa = Pnt_adr, Pnt_adr += pnum;
		if(Pnt_adr>=Pnt_max) {
			MemErr(IDS_MEMERR2);
		}
	} else {
		(*cmp)->psa = 0;
	}
	(*cmp)->cno = (BYTE)cno;											// ｺﾏﾝﾄﾞNo設定
	(*cmp)->next = NULL;												// ﾎﾟｲﾝﾀ初期化
	if(Copy_cmp==NULL) {												// 取得領域をCopy_cmp
		Copy_cmp = *cmp;												// に接続
	} else {
		cpc = Copy_cmp;
		while(cpc->next!=NULL) {
			cpc = cpc->next;
		}
		cpc->next = *cmp;
	}
}

/**************************************/
static void FreeParaArea(CPCTYPE* cmp)									//<<< ﾊﾟﾗﾒｰﾀ領域解放
/**************************************/
{
	int src, i;

	if(cmp->inum!=0) {													// 整数ﾊﾟﾗﾒｰﾀ領域解放
		src = cmp->isa+cmp->inum;
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-cmp->inum] = Int_ary[i];
		}
		Int_adr -= cmp->inum;
	}
	if(cmp->pnum!=0) {													// 座標ﾊﾟﾗﾒｰﾀ領域解放
		src = cmp->psa+cmp->pnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-cmp->pnum] = Pnt_ary[i];
		}
		Pnt_adr -= cmp->pnum;
	}
	ChangeAreaAdr(cmp->isa, -cmp->inum, cmp->psa, -cmp->pnum);			// 配列ｱﾄﾞﾚｽ変更
}

/**********************/
void FreeCopyCmd(void)													//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
/**********************/
{
	CPCTYPE* cpc;

	while(Copy_cmp!=NULL) {
		cpc = Copy_cmp, Copy_cmp = Copy_cmp->next;						// Copy_cmpから切り離し
		FreeParaArea(cpc);												// ﾊﾟﾗﾒｰﾀ領域解放
		free(cpc);														// ﾒﾓﾘ解放
	}
	Copy_cmp = NULL;
}

/**********************************/
void BaseCopyCmdPtr(CPCTYPE** cmp)										//<<< 先頭ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
/**********************************/
{
	*cmp = Copy_cmp;													// ﾍﾞｰｽｺﾏﾝﾄﾞ
}

/**********************************/
void NextCopyCmdPtr(CPCTYPE** cmp)										//<<< 次ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
/**********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->next;											// 次ｺﾏﾝﾄﾞ設定
	}
}

/******************************/
int GetCopyCmdNo(CPCTYPE* cmp)											//<<< ｺﾋﾟｰｺﾏﾝﾄﾞNo取得
/******************************/
{
	return (int)cmp->cno;
}

/*******************************/
int GetCopyIntNum(CPCTYPE* cmp)											//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/*******************************/										//    整数ﾊﾟﾗﾒｰﾀ数取得
{
	return cmp->inum;
}

/***************************************************/
void PutCopyParaInt(CPCTYPE* cmp, int no, int data)						//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/***************************************************/					//    整数ﾊﾟﾗﾒｰﾀ値設定
{
	Int_ary[cmp->isa+no] = data;
}

/****************************************************/
void GetCopyParaInt(CPCTYPE* cmp, int no, int* data)					//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/****************************************************/					//    整数ﾊﾟﾗﾒｰﾀ値取得
{
	*data = Int_ary[cmp->isa+no];
}

/*******************************/
int GetCopyPntNum(CPCTYPE* cmp)											//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/*******************************/										//    座標ﾊﾟﾗﾒｰﾀ数取得
{
	return cmp->pnum;
}

/*******************************************************/
void PutCopyParaPnt(CPCTYPE* cmp, int no, PNTTYPE data)					//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/*******************************************************/				//    座標ﾊﾟﾗﾒｰﾀ値設定
{
	Pnt_ary[cmp->psa+no] = data;
}

/********************************************************/
void GetCopyParaPnt(CPCTYPE* cmp, int no, PNTTYPE* data)				//<<< ｺﾋﾟｰｺﾏﾝﾄﾞ
/********************************************************/				//    座標ﾊﾟﾗﾒｰﾀ値取得
{
	*data = Pnt_ary[cmp->psa+no];
}

/*********************************************************/
/*****              Ｕｎｄｏ／Ｒｅｄｏ用             *****/
/*********************************************************/
/*********************/
void OnUndoRedo(void)													//<<< UndoﾌﾗｸﾞON
/*********************/
{
	Undo_flg = 1;
}

/**********************/
void OffUndoRedo(void)													//<<< UndoﾌﾗｸﾞOFF
/**********************/
{
	Undo_flg = 0;
}

/************************/
BOOL CheckUndoRedo(void)												//<<< Undoﾌﾗｸﾞ確認
/************************/
{
	return Undo_flg;
}

/*********************/
void OnUndoStop(void)													//<<< Undo保存停止ﾌﾗｸﾞON
/*********************/
{
	Undo_stp = 1;
}

/**********************/
void OffUndoStop(void)													//<<< Undo保存停止ﾌﾗｸﾞOFF
/**********************/
{
	Undo_stp = 0;
}

/********************/
void InitLevel(void)													//<<< Undo/Redo初期化
/********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      wadr;

    Undo_adr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;				// ｶｳﾝﾀUP
	if(Undo_ary[Undo_adr].urflg==0) {									// <Undo状態の場合>
		LevelBaseObjPtr(0, &op);										// 生成立体取得
		while(op!=NULL) {
			FreeObj(op, 0);												// 全生成立体削除
			LevelNextObjPtr(0, &op);
		}
		LevelBaseCmdPtr(&cmp);											// 編集ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			FreeCmd(op, cmp, 0);										// 全ｺﾏﾝﾄﾞ削除
			LevelNextCmdPtr(&cmp);
		}
	} else if(Undo_ary[Undo_adr].urflg==1) {							// <Undo無状態の場合>
		LevelBaseObjPtr(1, &op);										// 一時削除立体取得
		while(op!=NULL) {
			FreeObj(op, 0);												// 全一時削除立体削除
			LevelNextObjPtr(1, &op);
		}
	}
	Undo_ary[Undo_adr].urflg = -1;										// Undo領域初期化
	Undo_ary[Undo_adr].cmflg = -1;
	Undo_ary[Undo_adr].cmp = NULL;
	Undo_ary[Undo_adr].cop = NULL;
	Undo_ary[Undo_adr].dop = NULL;
	Undo_ary[Undo_adr].clvl = ++Cmd_Lvl;
    wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;
	Undo_ary[wadr].cmflg = -1;
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/********************/
void BackLevel(void)													//<<< Undo/Redo戻り
/********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// 生成立体取得
	while(op!=NULL) {
		FreeObj(op, 0);													// 全生成立体削除
		LevelNextObjPtr(0, &op);
	}
	LevelBaseCmdPtr(&cmp);												// 編集ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		FreeCmd(op, cmp, 0);											// 全ｺﾏﾝﾄﾞ削除
		LevelNextCmdPtr(&cmp);
	}
    Undo_adr = (Undo_adr-1<0) ? Undo_max-1 : Undo_adr-1;				// ｶｳﾝﾀDOWN
    Cmd_Lvl = (Cmd_Lvl-1<0) ? 0 : Cmd_Lvl-1;							// ｺﾏﾝﾄﾞﾚﾍﾞﾙNoDOWN
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/********************/
BOOL CheckUndo(void)													//<<< Undo確認
/********************/
{
	if(Undo_flg==0) {
		return FALSE;
	}
	if(Undo_ary[Undo_adr].urflg==1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************/
void Undo(int* gno)														//<<< Undo処理
/*******************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, ogn[32];

	for(i=0; i<32; i++) {
		gno[i] = 0;
	}
	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		TempObj(op);													// 立体一時削除
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
	LevelBaseCmdPtr(&cmp);												// 編集ｺﾏﾝﾄﾞ検索
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// ｺﾏﾝﾄﾞ対象立体取得
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		TempCmd(op, cmp);												// ｺﾏﾝﾄﾞ一時削除
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextCmdPtr(&cmp);											// 次編集ｺﾏﾝﾄﾞ検索
	}
	LevelBaseObjPtr(1, &op);											// 削除立体検索
	while(op!=NULL) {
		UndoObj(op);													// 立体再接続
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(1, &op);										// 次削除立体検索
	}
	UndoMenu(Undo_ary[Undo_adr].clvl);									// Undoﾒﾆｭｰ処理
	Undo_ary[Undo_adr].urflg = 0;										// Undo済
    Undo_adr = (Undo_adr-1<0) ? Undo_max-1 : Undo_adr-1;				// ｶｳﾝﾀDOWN
}

/********************/
BOOL CheckRedo(void)													//<<< Redo確認
/********************/
{
	int wadr;

	if(Undo_flg==0) {
		return FALSE;
	}
	wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;					// 次領域を確認
	if(Undo_ary[wadr].urflg!=0) {
		return FALSE;
	}
	if(Undo_ary[wadr].cmflg!=0&&Undo_ary[Undo_adr].urflg!=0) {
		return FALSE;
	}
	return TRUE;
}

/*******************/
void Redo(int* gno)														//<<< Redo処理
/*******************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, ogn[32];

	for(i=0; i<32; i++) {
		gno[i] = 0;
	}
    Undo_adr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;				// ｶｳﾝﾀUP
	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		UndoObj(op);													// 立体再接続
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
	LevelBaseCmdPtr(&cmp);												// 編集ｺﾏﾝﾄﾞ検索
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// ｺﾏﾝﾄﾞ対象立体取得
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		UndoCmd(op, cmp);												// ｺﾏﾝﾄﾞ再接続
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextCmdPtr(&cmp);											// 次編集ｺﾏﾝﾄﾞ検索
	}
	LevelBaseObjPtr(1, &op);											// 削除立体検索
	while(op!=NULL) {
		TempObj(op);													// 立体一時削除
		GetGroup(op, ogn);												// ｸﾞﾙｰﾌﾟ情報取得
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// 対象ｸﾞﾙｰﾌﾟON
		}
		LevelNextObjPtr(1, &op);										// 次削除立体検索
	}
	RedoMenu(Undo_ary[Undo_adr].clvl);									// Redoﾒﾆｭｰ処理
	Undo_ary[Undo_adr].urflg = 1;										// Redo済
}
