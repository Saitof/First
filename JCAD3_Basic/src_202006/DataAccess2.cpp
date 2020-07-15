/////////////////////////////////////////////////////////////////////////////
// 内部データベース操作部
// DataAccess2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"

/**********************************************************/
/*****                  立体構成情報用                *****/
/**********************************************************/
/***********************************************/
void CreatOif(int flg, OBJTYPE* op, char* name,
              OIFTYPE* bfo, OIFTYPE** oif)								//<<< 立体構成情報領域確保
/***********************************************/
{
	OIFTYPE* optr;

	if(Gc_oif==NULL) {													// <GC無の場合>
		*oif = (OIFTYPE *)malloc(sizeof(OIFTYPE));						// 新規領域確保
		if(*oif==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC有の場合>
		*oif = Gc_oif, Gc_oif = Gc_oif->next;							// GC先頭使用
	}
	(*oif)->op = op;
	memcpy((*oif)->name, name, 24);
	(*oif)->level = 0;
	(*oif)->ono = -1;
	(*oif)->next = NULL;
	if(flg==0) {														// <<末尾接続の場合>>
		if(Base_oif==NULL) {											// <ﾃﾞｰﾀ無の場合>
			Base_oif = (*oif);											// 先頭へ接続
		} else {														// <ﾃﾞｰﾀ有の場合>
			optr = Base_oif;
			while(optr->next!=NULL) {
				optr = optr->next;										// 最終構成検索
			}
			optr->next = (*oif);										// 最終構成へ接続
		}
	} else if(flg==1) {													// <<先頭接続の場合>>
		if(Base_oif==NULL) {											// <ﾃﾞｰﾀ無の場合>
			Base_oif = (*oif);											// 先頭へ接続
		} else {														// <ﾃﾞｰﾀ有の場合>
			(*oif)->next = Base_oif, Base_oif = (*oif);					// 旧先頭を次構成へ
		}
	} else {															// <<中間接続の場合>>
		if(bfo==NULL) {													// <接続ﾎﾟｲﾝﾀ無の場合>
			if(Base_oif==NULL) {										// 末尾接続
				Base_oif = (*oif);
			} else {
				optr = Base_oif;
				while(optr->next!=NULL) {
					optr = optr->next;
				}
				optr->next = (*oif);
			}
		} else {														// <接続ﾎﾟｲﾝﾀ有の場合>
			(*oif)->next = bfo->next, bfo->next = (*oif);				// 指定構成の次構成を接続
		}
	}
}

/*********************************/
static void TempOif(OIFTYPE* oif)										//<<< 立体構成情報一時削除
/*********************************/
{
	OIFTYPE* optr;

	if(oif!=NULL) {
		View->DelOifItem(oif);											// 立体構成情報ｱｲﾃﾑ削除
	}
	if(Base_oif==oif) {													// <先頭構成の場合>
		Base_oif = oif->next;											// 次構成Base接続
	} else {															// <先頭構成以外の場合>
		if((optr=Base_oif)==NULL) {
			return;
		}
		while(optr->next!=NULL&&optr->next!=oif) {
			optr = optr->next;											// 前構成検索
		}
		if(optr->next==NULL) {
			return;														// 指定構成無中止
		}
		optr->next = oif->next;											// 前構成次構成接続
	}
	oif->next = NULL;
	if(Temp_oif==NULL) {
		Temp_oif = oif;
	} else {
		optr = Temp_oif;
		while(optr->next!=NULL) {
			optr = optr->next;											// 最終構成検索
		}
		optr->next = oif;												// 最終構成ﾘｽﾄ接続
	}
}

/***********************************/
void FreeOif(int flg, OIFTYPE* oif)										//<<< 立体構成情報領域解放
/***********************************/
{
	OIFTYPE* optr;
	int      chk=-1;

	if(flg==0&&oif!=NULL) {
		View->DelOifItem(oif);											// 立体構成情報ｱｲﾃﾑ削除
	}
	if(Temp_oif==oif) {													// 構成存在確認
		chk = 0;
	} else if(Base_oif==oif) {
		chk = 1;
	} else {
		if((optr=Temp_oif)!=NULL) {										// Temp確認
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;
			}
			chk = (optr->next==oif) ? 0 : chk;
		}
		if(chk==-1) {
			if((optr=Base_oif)!=NULL) {									// Base確認
				while(optr->next!=NULL&&optr->next!=oif) {
					optr = optr->next;
				}
				chk = (optr->next==oif) ? 1 : chk;
			}
		}
	}
	if(chk==-1) {
		return;															// 構成無中止
	}
	if(!chk) {															// <Temp有の場合>
		if(Temp_oif==oif) {												// <先頭構成の場合>
			Temp_oif = oif->next;										// 次構成Temp接続
		} else {														// <先頭構成以外の場合>
			optr = Temp_oif;
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;										// 前構成検索
			}
			optr->next = oif->next;										// 前構成次構成接続
		}
	} else {															// <Base有の場合>
		if(Base_oif==oif) {												// <先頭構成の場合>
			Base_oif = oif->next;										// 次構成Base接続
		} else {														// <先頭構成以外の場合>
			optr = Base_oif;
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;										// 前構成検索
			}
			optr->next = oif->next;										// 前構成次構成接続
		}
	}
	oif->next = Gc_oif, Gc_oif = oif;									// Gc先頭接続
}

/*********************************/
static void UndoOif(OIFTYPE* oif)										//<<< 立体構成情報復元
/*********************************/
{
	OIFTYPE *optr;

	if(Temp_oif==oif) {													// <先頭構成の場合>
		Temp_oif = oif->next;											// 次構成Temp接続
	} else {															// <先頭構成以外の場合>
		if((optr=Temp_oif)==NULL) {
			return;
		}
		while(optr->next!=NULL&&optr->next!=oif) {
			optr = optr->next;											// 前構成検索
		}
		if(optr->next==NULL) {
			return;														// 指定構成無中止
		}
		optr->next = oif->next;											// 前構成次構成接続
	}
	oif->level = 0, oif->ono = -1, oif->next = NULL;
	if(Base_oif==NULL) {
		Base_oif = oif;
	} else {
		optr = Base_oif;
		while(optr->next!=NULL) {
			optr = optr->next;											// 最終構成検索
		}
		optr->next = oif;												// 最終構成ﾘｽﾄ接続
	}
	if(oif!=NULL) {
		View->SetOifItem(oif);											// 立体構成情報ｱｲﾃﾑ設定
	}
}

/*****************************************/
void SetOifObj(OIFTYPE* oif, OBJTYPE* op)								//<<< 構成情報立体ﾎﾟｲﾝﾀ設定
/*****************************************/
{
	oif->op = op;
	if(op!=NULL) {
		op->oift = oif;
	}
}

/******************************************/
void GetOifObj(OIFTYPE* oif, OBJTYPE** op)								//<<< 構成情報立体ﾎﾟｲﾝﾀ取得
/******************************************/
{
	*op = oif->op;
}

/******************************************/
void GetObjOif(OBJTYPE* op, OIFTYPE** oif)								//<<< 立体構成情報ﾎﾟｲﾝﾀ取得
/******************************************/
{
	*oif = op->oift;
}

/*************************************************/
void BaseOifPtr(int flg, int mode, OIFTYPE** oif)						//<<< 立体情報ﾍﾞｰｽﾎﾟｲﾝﾀ取得
/*************************************************/						// op==NULL mode 0:NG, 1:OK
{
	OIFTYPE* optr;

	optr = Base_oif, *oif = NULL;
	if(flg==0) {														// <全立体指定の場合>
		*oif = Base_oif;
	} else if(flg==1) {													// <dsp:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->dspflg==1) {								// dsp:ON検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	} else if(flg==2) {													// <edt:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1) {								// edt:ON検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	} else {															// <edt:ON/selno=1の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1||optr->op->selno==1) {			// edt:ON/selno=1検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	}
}

/*************************************************/
void NextOifPtr(int flg, int mode, OIFTYPE** oif)						//<<< 次立体情報ﾎﾟｲﾝﾀ移動
/*************************************************/						// op==NULL mode 0:NG, 1:OK
{
	OIFTYPE* optr;

	if(*oif==NULL) {
		return;
	}
	optr = (*oif)->next, *oif = NULL;
	if(flg==0) {														// <全立体指定の場合>
		*oif = optr;
	} else if(flg==1) {													// <dsp:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->dspflg==1) {								// dsp:ON検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	} else if(flg==2) {													// <edt:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1) {								// edt:ON検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	} else {															// <edt:ON/selno=1の場合>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1||optr->op->selno==1) {			// edt:ON/selno=1検索
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULLはOK
				}
			}
			optr = optr->next;
		}
	}
}

/**************************************************/
void SetOifName(int flg, OIFTYPE* oif, char* name)						//<<< 立体構成名称設定
/**************************************************/
{
	memcpy(oif->name, name, 24);
	if(flg==0) {
		if(oif!=NULL) {
			View->SetOifItem(oif);										// 立体構成情報ｱｲﾃﾑ設定
		}
	} else if(flg==1) {
		if(oif!=NULL) {
			View->UpdateOifItem(oif);									// 立体構成情報ｱｲﾃﾑ更新
		}
	}
}

/*****************************************/
void GetOifName(OIFTYPE* oif, char* name)								//<<< 立体構成名称取得
/*****************************************/
{
	memcpy(name, oif->name, ONM_LENG);
}

/***************************************/
void SetOifLevel(OIFTYPE* oif, int lvl)									//<<< 立体構成ﾚﾍﾞﾙ設定
/***************************************/
{
	oif->level = (short)lvl;
}

/*****************************/
int GetOifLevel(OIFTYPE* oif)											//<<< 立体構成ﾚﾍﾞﾙ取得
/*****************************/
{
	return (int)oif->level;
}

/********************/
void InitOifNo(void)													//<<< 立体構成立体No初期化
/********************/
{
	OIFTYPE* oif;

	oif = Base_oif;
	while(oif!=NULL) {
		oif->ono = -1;
		oif = oif->next;
	}
}

/***********************************/
void SetOifNo(OIFTYPE* oif, int no)										//<<< 立体構成立体No設定
/***********************************/
{
	oif->ono = (short)no;
}

/**************************/
int GetOifNo(OIFTYPE* oif)												//<<< 立体構成立体No取得
/**************************/
{
	return (int)oif->ono;
}

/**********************/
void SortOifData(void)													//<<< 立体構成ﾃﾞｰﾀｿｰﾄ
/**********************/
{
	OIFTYPE* oif, * lst, * osv, * owk;
	int      i, max=0;

	oif = Base_oif, lst = NULL, osv = NULL;
	while(oif!=NULL) {
		if(oif->ono>=0) {												// <今回出力ﾃﾞｰﾀ>
			if(max<oif->ono) {
				max = oif->ono;											// 最大立体No取得
			}
			owk = oif;
			oif = oif->next;											// 次ﾃﾞｰﾀへ
			if(Base_oif==owk) {											// 今回ﾃﾞｰﾀを切離
				Base_oif = owk->next;
			} else {
				lst->next = owk->next;
			}
			owk->next = osv;											// 今回ﾃﾞｰﾀのみ別接続
			osv = owk;
		} else {														// <今回出力以外>
			lst = oif;													// 直前ﾃﾞｰﾀを保存
			oif = oif->next;											// 次ﾃﾞｰﾀへ
		}
	}
	for(i=0; i<=max; i++) {												// <<立体No順に繰り返す>>
		oif = osv, owk = NULL;
		while(oif!=NULL) {
			if(oif->ono==i) {											// <該当立体Noに一致>
				if(osv==oif) {											// 今回ﾃﾞｰﾀを切離
					osv = oif->next;
				} else {
					owk->next = oif->next;
				}
				oif->next = NULL;
				break;
			}
			owk = oif;													// 直前ﾃﾞｰﾀを保存
			oif = oif->next;											// 次ﾃﾞｰﾀへ
		}
		if(oif!=NULL) {
			if(Base_oif==NULL) {										// 登録済ﾃﾞｰﾀの末尾に
				Base_oif = oif;											// 立体No順に接続
			} else {
				lst->next = oif;
			}
			lst = oif;
		}
	}
}

/**********************************************************/
/*****                    立体生成用                  *****/
/**********************************************************/
/************************************************************/
void GetDataBasePtr(OBJTYPE** bop, PNTTYPE** vtx, int** flp)			//<<< ﾃﾞｰﾀﾍﾞｰｽ先頭ﾎﾟｲﾝﾀ取得
/************************************************************/
{
	*bop = Base_op;														// 立体ﾃﾞｰﾀの先頭ﾎﾟｲﾝﾀ
	*vtx = Pnt_ary;														// 頂点ﾃﾞｰﾀの先頭ﾎﾟｲﾝﾀ
	*flp = Int_ary;														// 面ﾙｰﾌﾟﾃﾞｰﾀの先頭ﾎﾟｲﾝﾀ
}

/****************************/
void OnResetObj(OBJTYPE* op)											//<<< ﾘｾｯﾄ立体登録
/****************************/
{
	Reset_op = op;
}

/**********************/
void OffResetObj(void)													//<<< ﾘｾｯﾄ立体解除
/**********************/
{
	Reset_op = NULL;
}

/**************************/
BOOL GetResetObjMode(void)												//<<< ﾘｾｯﾄ立体ﾓｰﾄﾞ取得
/**************************/
{
	if(Reset_op!=NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************************************/
void ChangeObj(OBJTYPE* op, int vno, int lno1, int lno2)				//<<< 立体ﾃﾞｰﾀ領域変更
/********************************************************/
{
	int ovn, ova, oln, ola, src, chn, i;

	if(op->vnum==vno&&op->lnum1==lno1&&op->lnum2==lno2) {
		return;
	}
	ovn = op->vnum, ova = Pnt_adr;										// 旧領域数保存
	oln = op->lnum1+op->lnum2, ola = Int_adr;
	op->vnum = vno, Pnt_adr += vno-ovn;									// 頂点ﾃﾞｰﾀ領域確保
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR3);
	}
	op->lnum1 = lno1, op->lnum2 = lno2, Int_adr += lno1+lno2-oln;		// 面ﾙｰﾌﾟﾃﾞｰﾀ領域確保
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR4);
	}
	src = op->vsa+ovn;													// 領域ｼﾌﾄ(頂点ﾃﾞｰﾀ)
	if(ova<Pnt_adr) {
		chn = Pnt_adr-ova;
		if(chn!=0) {
			for(i=ova-1; i>=src; i--) {
				Pnt_ary[i+chn] = Pnt_ary[i];
			}
		}
	} else {
		chn = ova-Pnt_adr;
		if(chn!=0) {
			for(i=src; i<ova; i++) {
				Pnt_ary[i-chn] = Pnt_ary[i];
			}
		}
	}
	src = op->lsa+oln;													// 領域ｼﾌﾄ(面ﾙｰﾌﾟ)
	if(ola<Int_adr) {
		chn = Int_adr-ola;
		if(chn!=0) {
			for(i=ola-1; i>=src; i--) {
				Int_ary[i+chn] = Int_ary[i];
			}
		}
	} else {
		chn = ola-Int_adr;
		if(chn!=0) {
			for(i=src; i<ola; i++) {
				Int_ary[i-chn] = Int_ary[i];
			}
		}
	}
	ChangeAreaAdr(op->lsa, lno1+lno2-oln, op->vsa, vno-ovn);			// 配列ｱﾄﾞﾚｽ変更
}

/********************************************************/
void CreatObj(int vno, int lno1, int lno2, OBJTYPE** op)				//<<< 立体ﾃﾞｰﾀ領域確保
/********************************************************/
{
	OBJTYPE* optr;
	OIFTYPE* oif;

	if(Reset_op!=NULL) {												// <領域確保済み>
		ChangeObj(Reset_op, vno, lno1, lno2);							// 立体ﾃﾞｰﾀ領域変更
		*op = Reset_op;													// ﾎﾟｲﾝﾀｾｯﾄ->中止
		return;
	}  
	if(Gc_op==NULL) {													// <GC無の場合>
		*op = (OBJTYPE *)malloc(sizeof(OBJTYPE));						// 新規領域確保
		if(*op==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC有の場合>
		*op = Gc_op, Gc_op = Gc_op->onxt;								// GC先頭使用
	}
	(*op)->vnum = vno;													// 頂点ﾃﾞｰﾀ領域確保
	(*op)->vsa = Pnt_adr, Pnt_adr += vno;
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR3);
	}
	(*op)->lnum1 = lno1;												// 面ﾙｰﾌﾟﾃﾞｰﾀ領域確保
	(*op)->lnum2 = lno2;
	(*op)->lsa = Int_adr, Int_adr += lno1+lno2;
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR4);
	}
	(*op)->attr = 0;													// 立体情報初期化
	(*op)->part = 0;
	(*op)->color = 0;
	(*op)->dspflg = 1;
	(*op)->edtflg = 0;
	(*op)->lmode = 0;
	(*op)->list = 0;
	(*op)->group = 0;
	(*op)->selno = 0;
	(*op)->ono = ++Obj_No;												// 立体No
	(*op)->bcmp = NULL;													// 先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	(*op)->ccmp = NULL;													// 有効先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	(*op)->rscmp = NULL;												// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	(*op)->rccmp = NULL;												// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	(*op)->ecmp = NULL;													// ｱｰｽｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	(*op)->onxt = NULL;													// 次立体ﾎﾟｲﾝﾀ
	(*op)->cnxt = NULL;													// 同一生成ﾚﾍﾞﾙ内次ﾎﾟｲﾝﾀ
	(*op)->dnxt = NULL;													// 同一削除ﾚﾍﾞﾙ内次ﾎﾟｲﾝﾀ
	(*op)->oset = NULL;													// ｾｯﾄ立体ﾎﾟｲﾝﾀ
	(*op)->oift = NULL;													// 立体情報ﾎﾟｲﾝﾀ
	if(Base_op==NULL) {
		Base_op = (*op);
	} else {
		optr = Base_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// 最終立体検索
		}
		optr->onxt = (*op);												// 最終立体ﾘｽﾄ接続
	}
	CreatOif(0, (*op), OBJ_NAME00, NULL, &oif);							// 立体構成情報領域確保
	(*op)->oift = oif;
}

/*************************/
void TempObj(OBJTYPE* op)												//<<< 立体ﾃﾞｰﾀ一時削除
/*************************/
{
	OBJTYPE* optr;

	if(Base_op==op) {													// <先頭立体の場合>
		Base_op = op->onxt;												// 次立体Base接続
	} else {															// <先頭立体以外の場合>
		if((optr=Base_op)==NULL) {
			return;
		}
		while(optr->onxt!=NULL&&optr->onxt!=op) {
			optr = optr->onxt;											// 前立体検索
		}
		if(optr->onxt==NULL) {
			return;														// 指定立体無中止
		}
		optr->onxt = op->onxt;											// 前立体次立体接続
	}
	op->edtflg = 0;														// edtflg:OFF
	op->dspflg = 1;														// dspflg:ON
	op->selno = 0;														// 選択No初期化
	op->onxt = NULL;
	if(Temp_op==NULL) {
		Temp_op = op;
	} else {
		optr = Temp_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// 最終立体検索
		}
		optr->onxt = op;												// 最終立体ﾘｽﾄ接続
	}
	TempOif(op->oift);													// 立体構成情報一時削除
}

/**********************************/
void FreeObj(OBJTYPE* op, int flg)										//<<< 立体ﾃﾞｰﾀ領域解放
/**********************************/
{
	OBJTYPE* optr;
	CMDTYPE* cptr;
	int      src, i, chk=-1;

	if(Temp_op==op) {													// 立体存在確認
		chk = 0;
	} else if(Base_op==op) {
		chk = 1;
	} else {
		if((optr=Temp_op)!=NULL) {										// Temp確認
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;
			}
			chk = (optr->onxt==op) ? 0 : chk;
		}
		if(chk==-1) {
			if((optr=Base_op)!=NULL) {									// Base確認
				while(optr->onxt!=NULL&&optr->onxt!=op) {
					optr = optr->onxt;
				}
				chk = (optr->onxt==op) ? 1 : chk;
			}
		}
	}
	if(chk==-1) {
		return;															// 立体無中止
	}
	View->DelDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
	while(TRUE) {
		BaseCmdPtr(0, op, &cptr);										// 先頭ｺﾏﾝﾄﾞ取得
		if(cptr==NULL) {
			break;
		}
		FreeCmd(op, cptr, flg);											// ｺﾏﾝﾄﾞ領域解放
	}
	if(!chk) {															// <Temp有の場合>
		if(Temp_op==op) {												// <先頭立体の場合>
			Temp_op = op->onxt;											// 次立体Temp接続
		} else {														// <先頭立体以外の場合>
			optr = Temp_op;
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;										// 前立体検索
			}
			optr->onxt = op->onxt;										// 前立体次立体接続
		}
	} else {															// <Base有の場合>
		if(Base_op==op) {												// <先頭立体の場合>
			Base_op = op->onxt;											// 次立体Base接続
		} else {														// <先頭立体以外の場合>
			optr = Base_op;
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;										// 前立体検索
			}
			optr->onxt = op->onxt;										// 前立体次立体接続
		}
	}
	op->edtflg = 0;														// edtflg:OFF
	op->dspflg = 1;														// dspflg:ON
	op->selno = 0;														// 選択No初期化
	op->onxt = Gc_op, Gc_op = op;										// Gc先頭接続
	if(flg) {															// <Undo/Redo解除>
		if(Undo_ary[Undo_adr].cop==op) {								// <ﾚﾍﾞﾙ先頭の場合>
			Undo_ary[Undo_adr].cop = op->cnxt;							// 次立体先頭接続
		} else {														// <ﾚﾍﾞﾙ先頭でない場合>
			if((optr=Undo_ary[Undo_adr].cop)!=NULL) {					// 先頭より立体検索
				while(optr->cnxt!=NULL&&optr->cnxt!=op) {
					optr = optr->cnxt;
				}
				if(optr->cnxt==op) {
					optr->cnxt = op->cnxt;								// 指定前後接続
				}
			}
		}
	}
	if(op->vnum!=0) {													// 領域解放(頂点ﾃﾞｰﾀ)
		src = op->vsa+op->vnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-op->vnum] = Pnt_ary[i];
		}
		Pnt_adr -= op->vnum;
	}
	if(op->lnum1+op->lnum2!=0) {										// 領域解放(面ﾙｰﾌﾟ)
		src = op->lsa+(op->lnum1+op->lnum2);
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-(op->lnum1+op->lnum2)] = Int_ary[i];
		}
		Int_adr -= (op->lnum1+op->lnum2);
	}
	ChangeAreaAdr(op->lsa, -(op->lnum1+op->lnum2), op->vsa, -op->vnum);	// 配列ｱﾄﾞﾚｽ変更
	FreeOif(0, op->oift);												// 立体構成情報領域解放
}

/*************************/
void UndoObj(OBJTYPE* op)												//<<< 立体ﾃﾞｰﾀ復元
/*************************/
{
	OBJTYPE* optr;

	if(Temp_op==op) {													// <先頭立体の場合>
		Temp_op = op->onxt;												// 次立体Temp接続
	} else {															// <先頭立体以外の場合>
		if((optr=Temp_op)==NULL) {
			return;
		}
		while(optr->onxt!=NULL&&optr->onxt!=op) {
			optr = optr->onxt;											// 前立体検索
		}
		if(optr->onxt==NULL) {
			return;														// 指定立体無中止
		}
		optr->onxt = op->onxt;											// 前立体次立体接続
	}
	op->dspflg = 1;														// dspflg:ON
	op->edtflg = 0;														// edtflg:OFF
	op->onxt = NULL;
	if(Base_op==NULL) {
		Base_op = op, op->oset = NULL;
	} else {
		optr = Base_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// 最終立体検索
		}
		optr->onxt = op;												// 最終立体ﾘｽﾄ接続
		if(op->oset!=optr&&op->oset!=optr->oset) {
			op->oset = NULL;
		}
	}
	UndoOif(op->oift);													// 立体構成情報復元
}

/***********************************/
void LevelObj(int flg, OBJTYPE* op)										//<<< 同一ﾚﾍﾞﾙ立体接続
/***********************************/
{
	OBJTYPE* optr;
	int      wadr;

	if(Undo_stp==1) {
		return;															// Undo保存停止中中止
	}
	if(Reset_op!=NULL) {
		return;															// 接続済中止
	}
	if(!flg) {															// <flg=0 生成立体接続>
		op->cnxt = NULL;
		if(Undo_ary[Undo_adr].cop==NULL) {
			Undo_ary[Undo_adr].cop = op;
		} else {
			optr = Undo_ary[Undo_adr].cop;
			while(optr->cnxt!=NULL) {
				optr = optr->cnxt;										// 最終立体検索
			}
			optr->cnxt = op;											// 最終立体ﾘｽﾄ接続
		}
	} else {															// <flg=1 削除立体接続>
		op->dnxt = NULL;
		if(Undo_ary[Undo_adr].dop==NULL) {
			Undo_ary[Undo_adr].dop = op;
		} else {
			optr = Undo_ary[Undo_adr].dop;
			while(optr->dnxt!=NULL) {
				optr = optr->dnxt;										// 最終立体検索
			}
			optr->dnxt = op;											// 最終立体ﾘｽﾄ接続
		}
	}
	Undo_ary[Undo_adr].urflg = 1;										// 今回ﾒﾆｭｰUndo可
	Undo_ary[Undo_adr].cmflg = 0;										// 前回ﾒﾆｭｰRedo可
	if((wadr=Undo_adr+1)>Undo_max-1) {
		wadr = 0;
	}
	Undo_ary[wadr].cmflg = 1;											// 今回ﾒﾆｭｰRedo不可
}

/**********************************************************/
void BaseObjPtr(int flg, OBJTYPE** op, int* dsp, int* edt)				//<<< 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
/**********************************************************/
{
	OBJTYPE* optr;

	optr = Base_op, *op = NULL;
	if(flg==0) {														// <全立体指定の場合>
		*op = Base_op;
	} else if(flg==1) {													// <dsp:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->dspflg==1) {										// dsp:ON検索
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	} else {															// <edt:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->edtflg==1) {										// edt:ON検索
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	}
	if(*op!=NULL) {
		*dsp = (int)((*op)->dspflg);
		*edt = (int)((*op)->edtflg);
	}
}

/**************************************/
BOOL CheckObjPtr(int flg, OBJTYPE* op)									//<<< 指定立体存在確認
/**************************************/
{
	OBJTYPE* optr;

	optr = Base_op;														// Baseより立体検索
	while(optr!=NULL) {
		if(optr==op) {													// 立体一致
			if(flg==0) {												// <無条件立体の場合>
				return TRUE;
			} else if(flg==1) {											// <dsp:ON立体の場合>
				if(op->dspflg==1) {
					return TRUE;
				} else {
					return FALSE;
				}
			} else {													// <edt:ON立体の場合>
				if(op->edtflg==1) {
					return TRUE;
				} else {
					return FALSE;
				}
			}
		}
		optr = optr->onxt;
	}
	return FALSE;
}

/**********************************************************/
void NextObjPtr(int flg, OBJTYPE** op, int* dsp, int* edt)				//<<< 次立体ﾎﾟｲﾝﾀ移動
/**********************************************************/
{
	OBJTYPE* optr;

	if(*op==NULL) {
		return;
	}
	optr = (*op)->onxt, *op = NULL;
	if(flg==0) {														// <全立体指定の場合>
		*op = optr;
	} else if(flg==1) {													// <dsp:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->dspflg==1) {										// dsp:ON検索
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	} else {															// <edt:ON立体指定の場合>
		while(optr!=NULL) {
			if(optr->edtflg==1) {										// edt:ON検索
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	}
	if(*op!=NULL) {
		*dsp = (int)((*op)->dspflg);
		*edt = (int)((*op)->edtflg);
	}
}

/****************************************/
static unsigned int GetGroupFlg(int gno)								//<<< ｸﾞﾙｰﾌﾟﾌﾗｸﾞ取得
/****************************************/
{
	unsigned int gflg=0;

	if(gno==0) {
		gflg = 0x1;
	} else if(gno==1) {
		gflg = 0x2;
	} else if(gno==2) {
		gflg = 0x4;
	} else if(gno==3) {
		gflg = 0x8;
	} else if(gno==4) {
		gflg = 0x10;
	} else if(gno==5) {
		gflg = 0x20;
	} else if(gno==6) {
		gflg = 0x40;
	} else if(gno==7) {
		gflg = 0x80;
	} else if(gno==8) {
		gflg = 0x100;
	} else if(gno==9) {
		gflg = 0x200;
	} else if(gno==10) {
		gflg = 0x400;
	} else if(gno==11) {
		gflg = 0x800;
	} else if(gno==12) {
		gflg = 0x1000;
	} else if(gno==13) {
		gflg = 0x2000;
	} else if(gno==14) {
		gflg = 0x4000;
	} else if(gno==15) {
		gflg = 0x8000;
	} else if(gno==16) {
		gflg = 0x10000;
	} else if(gno==17) {
		gflg = 0x20000;
	} else if(gno==18) {
		gflg = 0x40000;
	} else if(gno==19) {
		gflg = 0x80000;
	} else if(gno==20) {
		gflg = 0x100000;
	} else if(gno==21) {
		gflg = 0x200000;
	} else if(gno==22) {
		gflg = 0x400000;
	} else if(gno==23) {
		gflg = 0x800000;
	} else if(gno==24) {
		gflg = 0x1000000;
	} else if(gno==25) {
		gflg = 0x2000000;
	} else if(gno==26) {
		gflg = 0x4000000;
	} else if(gno==27) {
		gflg = 0x8000000;
	} else if(gno==28) {
		gflg = 0x10000000;
	} else if(gno==29) {
		gflg = 0x20000000;
	} else if(gno==30) {
		gflg = 0x40000000;
	} else if(gno==31) {
		gflg = 0x80000000;
	}
	return gflg;
}

/*******************************************/
void GroupBaseObjPtr(int gno, OBJTYPE** op)								//<<< 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
/*******************************************/							//    (group)
{
	OBJTYPE* optr;
	unsigned int gflg, grp;

	gflg = GetGroupFlg(gno);											// ｸﾞﾙｰﾌﾟﾌﾗｸﾞ取得
	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		grp = optr->group;
		if(grp&gflg) {													// 指定ｸﾞﾙｰﾌﾟ立体検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void GroupNextObjPtr(int gno, OBJTYPE** op)								//<<< 次立体ﾎﾟｲﾝﾀ移動
/*******************************************/							//    (group)
{
	OBJTYPE* optr;
	unsigned int gflg, grp;

	if(*op==NULL) {
		return;
	}
	gflg = GetGroupFlg(gno);											// ｸﾞﾙｰﾌﾟﾌﾗｸﾞ取得
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		grp = optr->group;
		if(grp&gflg) {													// 指定ｸﾞﾙｰﾌﾟ立体検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void LevelBaseObjPtr(int flg, OBJTYPE** op)								//<<< 同一ﾚﾍﾞﾙ先頭立体取得
/*******************************************/
{
	*op = (!flg) ? Undo_ary[Undo_adr].cop : Undo_ary[Undo_adr].dop;		// 生成立体接続/削除立体接続
}

/*******************************************/
void LevelNextObjPtr(int flg, OBJTYPE** op)								//<<< 同一ﾚﾍﾞﾙ次立体移動
/*******************************************/
{
	if(!flg) {															// <flg=0 生成立体接続>
		if(*op!=NULL) {
			*op = (*op)->cnxt;
		}
	} else {															// <flg=1 削除立体接続>
		if(*op!=NULL) {
			*op = (*op)->dnxt;
		}
	}
}

/*******************************************/
void ColorBaseObjPtr(int col, OBJTYPE** op)								//<<< ｶﾗｰ別立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
/*******************************************/
{
	OBJTYPE* optr;

	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		if(optr->dspflg==1&&optr->color==(BYTE)col) {					// dsp:ONでｶﾗｰ一致検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void ColorNextObjPtr(int col, OBJTYPE** op)								//<<< ｶﾗｰ別次立体ﾎﾟｲﾝﾀ移動
/*******************************************/
{
	OBJTYPE* optr;

	if(*op==NULL) {
		return;
	}
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		if(optr->dspflg==1&&optr->color==(BYTE)col) {					// dsp:ONでｶﾗｰ一致検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*****************************************/
void SelBaseObjPtr(int flg, OBJTYPE** op)								//<<< 立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
/*****************************************/								//    (選択立体)
{
	OBJTYPE* optr;
	int      sno1, sno2;

	if(flg==99) {														//<ﾍﾟｰｽﾄ時は1or3>
		sno1 = 1,   sno2 = 3;
	} else {															//<以外は指定値>
		sno1 = flg, sno2 = flg;
	}
	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		if(sno1==optr->selno||sno2==optr->selno) {						// 指定選択立体検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/****************************************/
void SelNextObjPtr(int flg, OBJTYPE** op)								//<<< 次立体ﾎﾟｲﾝﾀ移動
/****************************************/								//    (選択立体)
{
	OBJTYPE* optr;
	int      sno1, sno2;

	if(*op==NULL) {
		return;
	}
	if(flg==99) {														//<ﾍﾟｰｽﾄ時は1or3>
		sno1 = 1,   sno2 = 3;
	} else {															//<以外は指定値>
		sno1 = flg, sno2 = flg;
	}
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		if(sno1==optr->selno||sno2==optr->selno) {						// 指定選択立体検索
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/***************************/
void SetObjPtr(OBJTYPE* op)												//<<< 処理対象立体設定
/***************************/
{
	Rf_op = op;
}

/****************************/
void GetObjPtr(OBJTYPE** op)											//<<< 処理対象立体取得
/****************************/
{
	*op = Rf_op;
}

/***********************************/
void OnDspFlg(int flg, OBJTYPE* op)										//<<< 表示(有効)ﾌﾗｸﾞON
/***********************************/
{
	op->dspflg = 1;
	if(flg) {
		View->UpdateDspFlg(1, op);										// 立体構成情報表示状態更新
	}
}

/************************************/
void OffDspFlg(int flg, OBJTYPE* op)									//<<< 表示(有効)ﾌﾗｸﾞOFF
/************************************/
{
	op->dspflg = 0;
	if(flg) {
		View->UpdateDspFlg(0, op);										// 立体構成情報表示状態更新
	}
}

/***********************************/
void OnEdtFlg(int flg, OBJTYPE* op)										//<<< 編集対象ﾌﾗｸﾞON
/***********************************/
{
	op->edtflg = 1;
	if(flg) {
		View->UpdateEdtFlg(1, op);										// 立体構成情報選択状態更新
	}
}

/************************************/
void OffEdtFlg(int flg, OBJTYPE* op)									//<<< 編集対象ﾌﾗｸﾞOFF
/************************************/
{
	op->edtflg = 0;
	if(flg) {
		View->UpdateEdtFlg(0, op);										// 立体構成情報選択状態更新
	}
}

/**********************************/
void SetObjNo(OBJTYPE* op, int no)										//<<< 立体No設定
/**********************************/
{
	op->ono = no;
}

/*************************/
int GetObjNo(OBJTYPE* op)												//<<< 立体No取得
/*************************/
{
	return op->ono;
}

/****************************/
void ChangeTestFlg(int mode)											//<<< ﾃｽﾄ用ﾌﾗｸﾞ変更
/****************************/
{
	OBJTYPE* op;

	op = Base_op;
	if(mode==0) {														// <ﾃｽﾄ開始処理>
		while(op!=NULL) {
			op->dspflg = (op->dspflg==1) ? 2 : op->dspflg;
			op->edtflg = (op->edtflg==1) ? 2 : op->edtflg;
			op = op->onxt;
		}
	} else {															// <ﾃｽﾄ終了処理>
		while(op!=NULL) {
			op->dspflg = (op->dspflg==2) ? 1 : op->dspflg;
			op->edtflg = (op->edtflg==2) ? 1 : op->edtflg;
			op = op->onxt;
		}
	}
}

/************************************/
void PutObjAtr(OBJTYPE* op, int atr)									//<<< 属性No設定
/************************************/
{
	if(Reset_op==NULL||op->attr!=1) {
		op->attr = (BYTE)atr;
	}
}

/**************************/
int GetObjAtr(OBJTYPE* op)												//<<< 属性No取得
/**************************/
{
	return (int)op->attr;
}

/*********************************************/
void PutObjPrt(OBJTYPE* op, int prt, int upd)							//<<< ﾊﾟｰﾂNo設定
/*********************************************/
{
	if(Reset_op==NULL) {
		op->part = (BYTE)prt;
		if(prt==1) {													// ﾕﾆｯﾄ･親op保存
			op->oset = NULL;
			Oset_op = op;
		} else if(prt==2) {												// ﾕﾆｯﾄ･子は親op設定
			op->oset = Oset_op;
			op->part = (Oset_op==NULL) ? 0 : 2;							// 親不明はﾕﾆｯﾄ解除
		} else {
			op->oset = NULL;
			if(upd==1) {
				Oset_op = NULL;											// ﾕﾆｯﾄ以外は親opﾘｾｯﾄ
			}
		}
	}
}

/**************************/
int GetObjPrt(OBJTYPE* op)												//<<< ﾊﾟｰﾂNo取得
/**************************/
{
	return (int)op->part;
}

/************************************/
void PutObjCol(OBJTYPE* op, int col)									//<<< ｶﾗｰNo設定
/************************************/
{
	if(Reset_op==NULL) {
		op->color = (BYTE)col;
	}
}

/**************************/
int GetObjCol(OBJTYPE* op)												//<<< ｶﾗｰNo取得
/**************************/
{
	return (int)op->color;
}

/**********************************************/
void PutListNo(OBJTYPE* op, unsigned int list)							//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄNo設定
/**********************************************/
{
	op->list = list;
}

/***********************************/
unsigned int GetListNo(OBJTYPE* op)										//<<< ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄNo取得
/***********************************/
{
	return op->list;
}

/***************************************/
void PutListMode(OBJTYPE* op, int mode)									//<<< ﾘｽﾄﾓｰﾄﾞ設定
/***************************************/
{
	op->lmode = (BYTE)mode;
}

/****************************/
int GetListMode(OBJTYPE* op)											//<<< ﾘｽﾄﾓｰﾄﾞ取得
/****************************/
{
	return (int)op->lmode;
}

/************************************/
void PutGroup(OBJTYPE* op, int* gno)									//<<< 立体ｸﾞﾙｰﾌﾟ情報設定
/************************************/
{
	unsigned int gflg;

	gflg = (gno[0]) ? 0x1 : 0;
	gflg = (gno[1]) ? gflg|0x2 : gflg;
	gflg = (gno[2]) ? gflg|0x4 : gflg;
	gflg = (gno[3]) ? gflg|0x8 : gflg;
	gflg = (gno[4]) ? gflg|0x10 : gflg;
	gflg = (gno[5]) ? gflg|0x20 : gflg;
	gflg = (gno[6]) ? gflg|0x40 : gflg;
	gflg = (gno[7]) ? gflg|0x80 : gflg;
	gflg = (gno[8]) ? gflg|0x100 : gflg;
	gflg = (gno[9]) ? gflg|0x200 : gflg;
	gflg = (gno[10]) ? gflg|0x400 : gflg;
	gflg = (gno[11]) ? gflg|0x800 : gflg;
	gflg = (gno[12]) ? gflg|0x1000 : gflg;
	gflg = (gno[13]) ? gflg|0x2000 : gflg;
	gflg = (gno[14]) ? gflg|0x4000 : gflg;
	gflg = (gno[15]) ? gflg|0x8000 : gflg;
	gflg = (gno[16]) ? gflg|0x10000 : gflg;
	gflg = (gno[17]) ? gflg|0x20000 : gflg;
	gflg = (gno[18]) ? gflg|0x40000 : gflg;
	gflg = (gno[19]) ? gflg|0x80000 : gflg;
	gflg = (gno[20]) ? gflg|0x100000 : gflg;
	gflg = (gno[21]) ? gflg|0x200000 : gflg;
	gflg = (gno[22]) ? gflg|0x400000 : gflg;
	gflg = (gno[23]) ? gflg|0x800000 : gflg;
	gflg = (gno[24]) ? gflg|0x1000000 : gflg;
	gflg = (gno[25]) ? gflg|0x2000000 : gflg;
	gflg = (gno[26]) ? gflg|0x4000000 : gflg;
	gflg = (gno[27]) ? gflg|0x8000000 : gflg;
	gflg = (gno[28]) ? gflg|0x10000000 : gflg;
	gflg = (gno[29]) ? gflg|0x20000000 : gflg;
	gflg = (gno[30]) ? gflg|0x40000000 : gflg;
	gflg = (gno[31]) ? gflg|0x80000000 : gflg;
	if(Reset_op==NULL) {
		op->group = gflg;
	}
}

/************************************/
void GetGroup(OBJTYPE* op, int* gno)									//<<< 立体ｸﾞﾙｰﾌﾟ情報取得
/************************************/
{
	unsigned int gflg;

	gflg = op->group;
	gno[0] = (gflg&0x1) ? 1 : 0;
	gno[1] = (gflg&0x2) ? 1 : 0;
	gno[2] = (gflg&0x4) ? 1 : 0;
	gno[3] = (gflg&0x8) ? 1 : 0;
	gno[4] = (gflg&0x10) ? 1 : 0;
	gno[5] = (gflg&0x20) ? 1 : 0;
	gno[6] = (gflg&0x40) ? 1 : 0;
	gno[7] = (gflg&0x80) ? 1 : 0;
	gno[8] = (gflg&0x100) ? 1 : 0;
	gno[9] = (gflg&0x200) ? 1 : 0;
	gno[10] = (gflg&0x400) ? 1 : 0;
	gno[11] = (gflg&0x800) ? 1 : 0;
	gno[12] = (gflg&0x1000) ? 1 : 0;
	gno[13] = (gflg&0x2000) ? 1 : 0;
	gno[14] = (gflg&0x4000) ? 1 : 0;
	gno[15] = (gflg&0x8000) ? 1 : 0;
	gno[16] = (gflg&0x10000) ? 1 : 0;
	gno[17] = (gflg&0x20000) ? 1 : 0;
	gno[18] = (gflg&0x40000) ? 1 : 0;
	gno[19] = (gflg&0x80000) ? 1 : 0;
	gno[20] = (gflg&0x100000) ? 1 : 0;
	gno[21] = (gflg&0x200000) ? 1 : 0;
	gno[22] = (gflg&0x400000) ? 1 : 0;
	gno[23] = (gflg&0x800000) ? 1 : 0;
	gno[24] = (gflg&0x1000000) ? 1 : 0;
	gno[25] = (gflg&0x2000000) ? 1 : 0;
	gno[26] = (gflg&0x4000000) ? 1 : 0;
	gno[27] = (gflg&0x8000000) ? 1 : 0;
	gno[28] = (gflg&0x10000000) ? 1 : 0;
	gno[29] = (gflg&0x20000000) ? 1 : 0;
	gno[30] = (gflg&0x40000000) ? 1 : 0;
	gno[31] = (gflg&0x80000000) ? 1 : 0;
}

/*********************************************/
void SetGroupNo(OBJTYPE* op, int no, int flg)							//<<< ｸﾞﾙｰﾌﾟ番号設定
/*********************************************/
{
	unsigned int gon, goff, gno;

	if(no==0) {
		gon = 0x1, goff = ~(0x1);
	} else if(no==1) {
		gon = 0x2, goff = ~(0x2);
	} else if(no==2) {
		gon = 0x4, goff = ~(0x4);
	} else if(no==3) {
		gon = 0x8, goff = ~(0x8);
	} else if(no==4) {
		gon = 0x10, goff = ~(0x10);
	} else if(no==5) {
		gon = 0x20, goff = ~(0x20);
	} else if(no==6) {
		gon = 0x40, goff = ~(0x40);
	} else if(no==7) {
		gon = 0x80, goff = ~(0x80);
	} else if(no==8) {
		gon = 0x100, goff = ~(0x100);
	} else if(no==9) {
		gon = 0x200, goff = ~(0x200);
	} else if(no==10) {
		gon = 0x400, goff = ~(0x400);
	} else if(no==11) {
		gon = 0x800, goff = ~(0x800);
	} else if(no==12) {
		gon = 0x1000, goff = ~(0x1000);
	} else if(no==13) {
		gon = 0x2000, goff = ~(0x2000);
	} else if(no==14) {
		gon = 0x4000, goff = ~(0x4000);
	} else if(no==15) {
		gon = 0x8000, goff = ~(0x8000);
	} else if(no==16) {
		gon = 0x10000, goff = ~(0x10000);
	} else if(no==17) {
		gon = 0x20000, goff = ~(0x20000);
	} else if(no==18) {
		gon = 0x40000, goff = ~(0x40000);
	} else if(no==19) {
		gon = 0x80000, goff = ~(0x80000);
	} else if(no==20) {
		gon = 0x100000, goff = ~(0x100000);
	} else if(no==21) {
		gon = 0x200000, goff = ~(0x200000);
	} else if(no==22) {
		gon = 0x400000, goff = ~(0x400000);
	} else if(no==23) {
		gon = 0x800000, goff = ~(0x800000);
	} else if(no==24) {
		gon = 0x1000000, goff = ~(0x1000000);
	} else if(no==25) {
		gon = 0x2000000, goff = ~(0x2000000);
	} else if(no==26) {
		gon = 0x4000000, goff = ~(0x4000000);
	} else if(no==27) {
		gon = 0x8000000, goff = ~(0x8000000);
	} else if(no==28) {
		gon = 0x10000000, goff = ~(0x10000000);
	} else if(no==29) {
		gon = 0x20000000, goff = ~(0x20000000);
	} else if(no==30) {
		gon = 0x40000000, goff = ~(0x40000000);
	} else if(no==31) {
		gon = 0x80000000, goff = ~(0x80000000);
	}
	gno = op->group;
	gno = (!flg) ? gno&goff : gno|gon;
	op->group = gno;
}

/***********************************/
void SetSelNo(OBJTYPE* op, int flg)										//<<< 選択No設定
/***********************************/
{
	op->selno = (BYTE)flg;
	SelectNo = flg;
}

/*************************/
int GetSelNo(OBJTYPE* op)												//<<< 選択No取得
/*************************/
{
	return (int)op->selno;
}

/*********************/
int GetSelectNo(void)													//<<< 立体選択No取得
/*********************/
{
	return SelectNo;
}

/*********************/
void ResetSelNo(void)													//<<< 選択Noﾘｾｯﾄ
/*********************/
{
	OBJTYPE* op;

	op = Base_op;
	while(op!=NULL) {
		op->selno = 0;													// 選択Noﾘｾｯﾄ
		op = op->onxt;
	}
	SelectNo = 0;
}

/**************************/
int GetVtxNum(OBJTYPE* op)												//<<< 立体頂点数取得
/**************************/
{
	return op->vnum;
}

/*********************************************/
void PutVtx(OBJTYPE* op, int no, PNTTYPE pnt)							//<<< 指定頂点設定
/*********************************************/
{
	Pnt_ary[op->vsa+no] = pnt;
}

/**********************************************/
void GetVtx(OBJTYPE* op, int no, PNTTYPE* pnt)							//<<< 指定頂点取得
/**********************************************/
{
	*pnt = Pnt_ary[op->vsa+no];
}

/***************************/
int GetFlpNum1(OBJTYPE* op)												//<<< 立体面ﾙｰﾌﾟ数取得1
/***************************/
{
	return op->lnum1;
}

/***************************************************/
void PutFlp1(OBJTYPE* op, int no, int vno, int flg)						//<<< 指定面ﾙｰﾌﾟ番号設定1
/***************************************************/
{
	Int_ary[op->lsa+no] = (!flg) ? vno+1 : -(vno+1);
}

/*****************************************************/
void GetFlp1(OBJTYPE* op, int no, int* vno, int* flg)					//<<< 指定面ﾙｰﾌﾟ番号取得1
/*****************************************************/
{
	if(Int_ary[op->lsa+no]>0) {
		*flg = 0, *vno = Int_ary[op->lsa+no]-1;
	} else {
		*flg = 1, *vno = -(Int_ary[op->lsa+no]+1);
	}
}

/************************************************************/
void GetFlpVtx1(OBJTYPE* op, int no, PNTTYPE* pnt, int* flg)			//<<< 指定面ﾙｰﾌﾟ座標取得1
/************************************************************/
{
	if(Int_ary[op->lsa+no]>0) {
		*flg = 0, *pnt = Pnt_ary[op->vsa+Int_ary[op->lsa+no]-1];
	} else {
		*flg = 1, *pnt = Pnt_ary[op->vsa-(Int_ary[op->lsa+no]+1)];
	}
}

/***************************/
int GetFlpNum2(OBJTYPE* op)												//<<< 立体面ﾙｰﾌﾟ数取得2
/***************************/
{
	return op->lnum2;
}

/***************************************************/
void PutFlp2(OBJTYPE* op, int no, int vno, int flg)						//<<< 指定面ﾙｰﾌﾟ番号設定2
/***************************************************/
{
	Int_ary[(op->lsa+op->lnum1)+no] = (!flg) ? vno+1 : -(vno+1);
}

/*****************************************************/
void GetFlp2(OBJTYPE* op, int no, int* vno, int* flg)					//<<< 指定面ﾙｰﾌﾟ番号取得2
/*****************************************************/
{
	if(Int_ary[(op->lsa+op->lnum1)+no]>0) {
		*flg = 0, *vno = Int_ary[(op->lsa+op->lnum1)+no]-1;
	} else {
		*flg = 1, *vno = -(Int_ary[(op->lsa+op->lnum1)+no]+1);
	}
}

/************************************************************/
void GetFlpVtx2(OBJTYPE* op, int no, PNTTYPE* pnt, int* flg)			//<<< 指定面ﾙｰﾌﾟ座標取得2
/************************************************************/
{
	if(Int_ary[(op->lsa+op->lnum1)+no]>0) {
		*flg = 0, *pnt = Pnt_ary[op->vsa+Int_ary[(op->lsa+op->lnum1)+no]-1];
	} else {
		*flg = 1, *pnt = Pnt_ary[op->vsa-(Int_ary[(op->lsa+op->lnum1)+no]+1)];
	}
}

/***************************/
void SetObjBox(OBJTYPE* op)												//<<< 立体ﾎﾞｯｸｽ設定
/***************************/
{
	PNTTYPE mip, map, pnt;
	int     i, j, num;

	num = GetVtxNum(op);
	GetVtx(op, 0, &pnt);
	mip = pnt, map = pnt;
	for(i=1; i<num; i++) {
		GetVtx(op, i, &pnt);
		for(j=0; j<DIM; j++) {
			mip.p[j] = (mip.p[j]>pnt.p[j]) ? pnt.p[j] : mip.p[j];
			map.p[j] = (map.p[j]<pnt.p[j]) ? pnt.p[j] : map.p[j];
		}
	}
	op->box.bp[0] = mip, op->box.bp[1] = map;
}

/*****************************************/
void GetObjBox(OBJTYPE* op, BOXTYPE* box)								//<<< 立体ﾎﾞｯｸｽ取得
/*****************************************/
{
	*box = op->box;
}

/********************/
void SetScrBox(void)													//<<< ｽｸﾘｰﾝﾎﾞｯｸｽ設定
/********************/
{
	OBJTYPE* op;
	PNTTYPE  mip, map;
	BOXTYPE  box;
	int      i, dsp, edt;

	for(i=0; i<DIM; i++) {
		Scr_Box.bp[0].p[i] = 0.0;
		Scr_Box.bp[1].p[i] = 0.0;
	}
	BaseObjPtr(0, &op, &dsp, &edt);
	if(op==NULL) {
		return;
	}
	GetObjBox(op, &box);
	mip = box.bp[0], map = box.bp[1];
	NextObjPtr(0, &op, &dsp, &edt);
	while(op!=NULL) {
		GetObjBox(op, &box);
		for(i=0; i<DIM; i++) {
			mip.p[i] = (mip.p[i]>box.bp[0].p[i]) ? box.bp[0].p[i] : mip.p[i];
			map.p[i] = (map.p[i]<box.bp[1].p[i]) ? box.bp[1].p[i] : map.p[i];
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	Scr_Box.bp[0] = mip, Scr_Box.bp[1] = map;
}

/****************************/
void GetScrBox(BOXTYPE* box)											//<<< ｽｸﾘｰﾝﾎﾞｯｸｽ取得
/****************************/
{
	*box = Scr_Box;
}

/**************************************/
void GetAreaBox(int flg, BOXTYPE* box)									//<<< ｴﾘｱﾎﾞｯｸｽ取得
/**************************************/
{
	OBJTYPE* op;
	PNTTYPE  mip, map;
	BOXTYPE  wbx;
	int      i, dsp, edt;

	BaseObjPtr(flg, &op, &dsp, &edt);
	if(op==NULL) {
		return;
	}
	GetObjBox(op, &wbx);
	mip = wbx.bp[0], map = wbx.bp[1];
	NextObjPtr(flg, &op, &dsp, &edt);
	while(op!=NULL) {
		GetObjBox(op, &wbx);
		for(i=0; i<DIM; i++) {
			mip.p[i] = (mip.p[i]>wbx.bp[0].p[i]) ? wbx.bp[0].p[i] : mip.p[i];
			map.p[i] = (map.p[i]<wbx.bp[1].p[i]) ? wbx.bp[1].p[i] : map.p[i];
		}
		NextObjPtr(flg, &op, &dsp, &edt);
	}
	box->bp[0] = mip, box->bp[1] = map;
}

/******************************************/
void GetUnitBox(OBJTYPE* op, BOXTYPE* box)								//<<< ﾕﾆｯﾄ立体ﾎﾞｯｸｽ取得
/******************************************/
{
	OBJTYPE* optr, * oya;
	PNTTYPE  mip, map;
	BOXTYPE  wbx;
	int      i, dsp, edt;

	if(op==NULL) {														// 立体無は終了
		return;
	}
	if(op->edtflg!=1) {													// 編集対象外は終了
		return;
	}
	GetObjBox(op, box);													// 立体ﾎﾞｯｸｽ取得
	if(op->part!=0) {													// <<ﾕﾆｯﾄ有>>
		wbx = *box;
		if(op->part==1) {												// <親の場合>
			oya = op;													// 立体ﾎﾟｲﾝﾀが親ﾎﾟｲﾝﾀ
		} else {														// <子の場合>
			if(op->oset==NULL) {
				oya = op;
			} else {
				oya = op->oset;
			}
		}
		if(oya->edtflg==1) {
			GetObjBox(oya, &wbx);										// 親立体ﾎﾞｯｸｽ取得
		}
		mip = wbx.bp[0], map = wbx.bp[1];
		BaseObjPtr(2, &optr, &dsp, &edt);
		while(optr!=NULL) {
			if(optr->oset==oya) {										// <親立体が登録されている子立体>
				GetObjBox(optr, &wbx);
				for(i=0; i<DIM; i++) {
					mip.p[i] = (mip.p[i]>wbx.bp[0].p[i]) ? wbx.bp[0].p[i] : mip.p[i];
					map.p[i] = (map.p[i]<wbx.bp[1].p[i]) ? wbx.bp[1].p[i] : map.p[i];
				}
			}
			NextObjPtr(2, &optr, &dsp, &edt);
		}
		box->bp[0] = mip, box->bp[1] = map;
	}
}

/********************************************************************/
void ChangeObjVtx(int md, OBJTYPE* op, double x, double y, double z)	//<<< 立体頂点変更
/********************************************************************/
{
	PNTTYPE pnt;
	int     i, num;

	num = GetVtxNum(op);												// 立体頂点数取得
	for(i=0; i<num; i++) {
		GetVtx(op, i, &pnt);											// 頂点座標取得
		if(md==0) {
			pnt.p[0] = pnt.p[0] + x;									// 頂点ﾃﾞｰﾀ変更
			pnt.p[1] = pnt.p[1] + y;
			pnt.p[2] = pnt.p[2] + z;
		} else {
			pnt.p[0] = pnt.p[0] - x;									// 頂点ﾃﾞｰﾀ復旧
			pnt.p[1] = pnt.p[1] - y;
			pnt.p[2] = pnt.p[2] - z;
		}
		PutVtx(op, i, pnt);												// 頂点座標設定
	}
	SetObjBox(op);														// 立体ﾎﾞｯｸｽｻｲｽﾞ設定
}

/*******************/
void InitCVtx(void)														//<<< 頂点ｿｰﾄ初期処理
/*******************/
{
	Vp = NULL;
	Now_no = 0;
}

/************************/
void SetCVtx(PNTTYPE pt)												//<<< ｿｰﾄ頂点設定
/************************/
{
	VTYPE* tv;

	tv = (VTYPE *)malloc(sizeof(VTYPE));								// ｿｰﾄ領域確保
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->next = Vp;
	tv->no = Now_no;
	tv->pt = pt;
	tv->vt = -1;
	tv->flg = FALSE;
	Vp = tv;
	Now_no++;
}

/*******************/
void SetCFace(void)														//<<< 面ﾙｰﾌﾟ終了設定
/*******************/
{
	Vp->flg = TRUE;
}

/*******************************************/
static VTYPE* MergeVx(VTYPE* la, VTYPE* lb)								//<<< 頂点座標:Xｿｰﾄ
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[0]<=lb->pt.p[0]) {									// 頂点座標:X大小比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtx(VTYPE* mlist, int n)							//<<< 頂点座標:Xｿｰﾄ
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// 頂点座標:Xｿｰﾄ
		lm = (VTYPE *)MergeVx(V_Mergsrtx(la, n/2), V_Mergsrtx(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVy(VTYPE* la, VTYPE* lb)								//<<< 頂点座標:Yｿｰﾄ
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[1]<=lb->pt.p[1]) {									// 頂点座標:Y大小比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrty(VTYPE* mlist, int n)							//<<< 頂点座標:Yｿｰﾄ
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// 頂点座標:Yｿｰﾄ
		lm = (VTYPE *)MergeVy(V_Mergsrty(la, n/2), V_Mergsrty(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVz(VTYPE* la, VTYPE* lb)								//<<< 頂点座標:Zｿｰﾄ
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[2]<=lb->pt.p[2]) {									// 頂点座標:Z大小比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtz(VTYPE* mlist, int n)							//<<< 頂点座標:Zｿｰﾄ
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// 頂点座標:Zｿｰﾄ
		lm = (VTYPE *)MergeVz(V_Mergsrtz(la, n/2), V_Mergsrtz(lb,n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVn(VTYPE* la, VTYPE* lb)								//<<< 面ﾙｰﾌﾟ番号ｿｰﾄ
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// 面ﾙｰﾌﾟ番号大小比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtn(VTYPE* mlist, int n)							//<<< 面ﾙｰﾌﾟ番号のｿｰﾄ
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// 面ﾙｰﾌﾟ番号ｿｰﾄ
		lm = (VTYPE *)MergeVn(V_Mergsrtn(la, n/2), V_Mergsrtn(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/******************/
int SortCVtx(void)														//<<< 頂点情報のｿｰﾄ&ｶｳﾝﾄ
/******************/
{
	VTYPE*  wvp;
	PNTTYPE pt;
	int     cnt=0;

	if(Now_no==0) {
		return 0;
	}
	Vp = V_Mergsrtz(Vp, Now_no);										// 頂点座標:Zのｿｰﾄ
	Vp = V_Mergsrty(Vp, Now_no);										// 頂点座標:Yのｿｰﾄ
	Vp = V_Mergsrtx(Vp, Now_no);										// 頂点座標:Xのｿｰﾄ
	pt = Vp->pt;														// 初期値設定
	pt.p[0] = pt.p[0]-1.0;
	wvp = Vp;
	while(wvp!=NULL) {													// 設定済と異なる場合
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {
			wvp->vt = cnt;												// 頂点番号保存
			cnt++;
			pt = wvp->pt;
		} else {
			wvp->vt = cnt-1;											// 同は頂点番号保存
		}
		wvp = wvp->next;
	}
	return cnt;
}

/****************************/
void SetCVtxObj(OBJTYPE* op)											//<<< 頂点情報設定
/****************************/
{
	VTYPE*  wvp, * cvp;
	PNTTYPE pt;
	int     lct=0;

	pt = Vp->pt;														// 初期値設定
	pt.p[0] = pt.p[0]-1.0;
	wvp = Vp;
	while(wvp!=NULL) {													// 設定済と異なる場合
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {
			Pnt_ary[op->vsa+wvp->vt] = wvp->pt;							// 頂点座標設定
			pt = wvp->pt;
		}
		wvp = wvp->next;
	}
	Vp = V_Mergsrtn(Vp, Now_no);										// 面ﾙｰﾌﾟ番号ｿｰﾄ
	wvp = Vp;
	while(wvp!=NULL) {
		Int_ary[op->lsa+lct]=(!wvp->flg) ? wvp->vt+1 : -(wvp->vt+1);	// 面ﾙｰﾌﾟ1設定
		lct++;
		wvp = wvp->next;
	}
	wvp = Vp;
	while(wvp) {														// ｿｰﾄ領域解放
		cvp = wvp->next;
		free(wvp);
		wvp = cvp;
	}
}
