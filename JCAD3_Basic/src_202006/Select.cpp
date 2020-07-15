/////////////////////////////////////////////////////////////////////////////
// 立体選択部
// Select.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*************************/
void OnDspAllObject(void)												//<<< 立体表示(全立体)
/*************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(0, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OnDspFlg(1, op);												// dspflg:ON
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
}

/**************************/
void OffDspAllObject(void)												//<<< 立体非表示(全立体)
/**************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(0, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OffDspFlg(1, op);												// dspflg:OFF
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
}

/******************************/
void OnDspGroupObject(int gno)											//<<< 立体表示(ｸﾞﾙｰﾌﾟ)
/******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(0, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		GetGroup(op, grp);												// ｸﾞﾙｰﾌﾟ情報取得
		if(grp[gno]) {
			OnDspFlg(1, op);											// dspflg:ON
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
}

/*******************************/
void OffDspGroupObject(int gno)											//<<< 立体非表示(ｸﾞﾙｰﾌﾟ)
/*******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(0, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		GetGroup(op, grp);												// ｸﾞﾙｰﾌﾟ情報取得
		if(grp[gno]) {
			OffDspFlg(1, op);											// dspflg:OFF
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ｽｸﾘｰﾝﾎﾞｯｸｽ設定
}

/*****************************************************/
void EdtPointObject(int plane, PNTTYPE pnt, int ckey)					//<<< 編集立体選択(ﾎﾟｲﾝﾄ)
/*****************************************************/
{
	OBJTYPE* op, * sop=NULL;
	BOXTYPE  box;
	int      dsp, edt, prt, oed, chk=0;

	BaseObjPtr(1, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		if(ckey==0) {
			OffEdtFlg(1, op);											// edtflg:OFF
			edt = 0;
		}
		prt = GetObjPrt(op);											// ﾊﾟｰﾂNo取得
		GetObjBox(op, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		if(BoxCheck(plane, box, pnt)) {									// <<選択立体>>
			if(ckey==1) {												// <ｷｰ押下(追加=解除)>
				if(!edt) {
					OnEdtFlg(1, op);									// edtflg:ON
					oed = 1;
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
					oed = 0;
				}
			} else if(ckey==2||ckey==0) {								// <ｷｰ押下(追加)/ｷｰ押下無>
				OnEdtFlg(1, op);										// edtflg:ON
				oed = 1;
			} else if(ckey==3) {										// <ｷｰ押下(解除)>
				OffEdtFlg(1, op);										// edtflg:OFF
				oed = 0;
			}
			if(prt==1) {												// ﾕﾆｯﾄ･親
				chk = 1, sop = op;
			}
		}
		if(prt==2) {													// <<ﾕﾆｯﾄ･子>>
			if(op->oset!=sop) {
				chk = 0;
			}
			if(chk) {
				if(oed) {
					OnEdtFlg(1, op);									// edtflg:ON
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
				}
			}
		} else {														// <<ﾕﾆｯﾄ･子以外>>
			if(op!=sop) {
				chk = 0;
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/*****************************************************/
void EdtAreaObject(int plane, BOXTYPE area, int ckey)					//<<< 編集立体選択(ｴﾘｱ)
/*****************************************************/
{
	OBJTYPE* op, * sop=NULL;
	PNTTYPE  pnt;
	BOXTYPE  box, abox;
	int      i, dsp, edt, prt, oed, chk=0;

	for(i=0; i<DIM; i++) {												// ﾎﾞｯｸｽｻｲｽﾞ位置補正
		if(area.bp[0].p[i]<area.bp[1].p[i]) {
			abox.bp[0].p[i] = area.bp[0].p[i];
			abox.bp[1].p[i] = area.bp[1].p[i];
		} else {
			abox.bp[0].p[i] = area.bp[1].p[i];
			abox.bp[1].p[i] = area.bp[0].p[i];
		}
	}
	BaseObjPtr(1, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		if(!ckey) {
			OffEdtFlg(1, op);											// edtflg:OFF
			edt = 0;
		}
		prt = GetObjPrt(op);											// ﾊﾟｰﾂNo取得
		GetObjBox(op, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &pnt);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		if(BoxCheck(plane, abox, pnt)) {								// <<選択立体>>
			if(ckey==1) {												// <ｷｰ押下(追加=解除)>
				if(!edt) {
					OnEdtFlg(1, op);									// edtflg:ON
					oed = 1;
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
					oed = 0;
				}
			} else if(ckey==2||ckey==0) {								// <ｷｰ押下(追加)/ｷｰ押下無>
				OnEdtFlg(1, op);										// edtflg:ON
				oed = 1;
			} else if(ckey==3) {										// <ｷｰ押下(解除)>
				OffEdtFlg(1, op);										// edtflg:OFF
				oed = 0;
			}
			if(prt==1) {												// ﾕﾆｯﾄ･親
				chk = 1, sop = op;
			}
		}
		if(prt==2) {													// <<ﾕﾆｯﾄ･子>>
			if(op->oset!=sop) {
				chk = 0;
			}
			if(chk) {
				if(oed) {
					OnEdtFlg(1, op);									// edtflg:ON
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
				}
			}
		} else {														// <<ﾕﾆｯﾄ･子以外>>
			if(op!=sop) {
				chk = 0;
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/******************************/
void OnEdtGroupObject(int gno)											//<<< 編集立体選択(ｸﾞﾙｰﾌﾟ)
/******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(1, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		GetGroup(op, grp);												// ｸﾞﾙｰﾌﾟ情報取得
		if(grp[gno]) {
			OnEdtFlg(1, op);											// edtflg:ON
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/*************************/
void OnEdtAllObject(void)												//<<< 編集立体全選択(表示立体)
/*************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(1, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OnEdtFlg(1, op);												// edtflg:ON
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/**************************/
void OffEdtAllObject(void)												//<<< 編集立体全解除
/**************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(2, &op, &dsp, &edt);
	}
}

/************************************/
void OffEdtOtherObject(OBJTYPE* sop)									//<<< 指定外編集対象解除
/************************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		if(op!=sop) {
			OffEdtFlg(1, op);											// edtflg:OFF
		}
		NextObjPtr(2, &op, &dsp, &edt);
	}
}
