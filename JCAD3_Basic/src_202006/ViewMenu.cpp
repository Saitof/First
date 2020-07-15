/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewMenu.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "ButtonDlg.h"
#include "InputsDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"
#include "LCheck.h"

/****************************************************/
int CJcad3GlbView::HitTestPlane(const CPoint& point)					//<<< 平面位置確認
/****************************************************/
{
	if(PlRect[XY].PtInRect(point)) {
		return XY;
	}
	if(PlRect[YZ].PtInRect(point)) {
		return YZ;
	}
	if(PlRect[ZX].PtInRect(point)) {
		return ZX;
	}
	if(PlRect[PRS].PtInRect(point)) {
		return PRS;
	}
	return -1;
}

/***************************************************************/
void CJcad3GlbView::GetViewport(int plane,
                                int* x, int* y, int* w, int* h)			//<<< ﾋﾞｭｰﾎﾟｰﾄ取得
/***************************************************************/
{
	*x = PlRect[plane].left;
	*y = ViewSize.cy-PlRect[plane].bottom-1;
	*w = PlRect[plane].Width();
	*h = PlRect[plane].Height();
}

/*******************************************************************/
BOOL CJcad3GlbView::CalcScreenWorld(int bind, PNTTYPE* pnt,
                                    const CPoint& point, int plane)		//<<< Screen->World座標変換
/*******************************************************************/
{
	CPoint  cp;
	CSize   dist;
	PNTTYPE spt, bp1, bp2, bp3;
	BOOL    ret=TRUE;
	int     grid, bpoint, bline, id;
	double  ln0, ln1, ln2, ln3;

	if(bind==1) {														// <ｸﾞﾘｯﾄﾞ拘束OFF指定時>
		grid = m_GridBind, m_GridBind = 0;								// 拘束状態保存後非拘束
	} else if(bind==2) {												// <ｸﾞﾘｯﾄﾞ拘束ON指定時>
		grid = m_GridBind, bpoint = m_BpBind, bline = m_BlBind;			// 拘束状態保存
		m_GridBind = 1, m_BpBind = 0, m_BlBind = 0;						// ｸﾞﾘｯﾄﾞ分移動
	}
	if(plane==XY) {
		cp = CPoint((PlRect[XY].left+PlRect[XY].right)/2,
					(PlRect[XY].top+PlRect[XY].bottom)/2);
		dist = point - cp;												// XY平面中心→指定点の距離
		pnt->p[0] = (m_Cent3D.p[0]+(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[1] = (m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[2] = m_Cent3D.p[2]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ｸﾞﾘｯﾄﾞ拘束
			id = m_Cent3D.p[0]+(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[0] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[1] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// 基準点拘束
				ScrToPnt(0, &m_Bpoint, &bp1);							// 基準点位置
				bp1.p[2] = m_Cent3D.p[2]/1000.0;
			}
			if(m_BlBind) {												// 基準線拘束
				ScrToPnt(0, &m_Blspnt, &bp2);							// 基準線始点位置
				bp2.p[2] = m_Cent3D.p[2]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// 基準線終点位置
				bp3.p[2] = m_Cent3D.p[2]/1000.0;
			}
		}
	} else if(plane==YZ) {
		cp = CPoint((PlRect[YZ].left+PlRect[YZ].right)/2,
					(PlRect[YZ].top+PlRect[YZ].bottom)/2);
		dist = point - cp;												// YZ平面中心→指定点の距離
		pnt->p[1] = (m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[2] = (m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[0] = m_Cent3D.p[0]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ｸﾞﾘｯﾄﾞ拘束
			id = m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[1] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[2] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// 基準点拘束
				ScrToPnt(0, &m_Bpoint, &bp1);							// 基準点位置
				bp1.p[0] = m_Cent3D.p[0]/1000.0;
			}
			if(m_BlBind) {												// 基準線拘束
				ScrToPnt(0, &m_Blspnt, &bp2);							// 基準線始点位置
				bp2.p[0] = m_Cent3D.p[0]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// 基準線終点位置
				bp3.p[0] = m_Cent3D.p[0]/1000.0;
			}
		}
	} else if(plane==ZX) {
		cp = CPoint((PlRect[ZX].left+PlRect[ZX].right)/2,
					(PlRect[ZX].top+PlRect[ZX].bottom)/2);
		dist = point - cp;												// ZX平面中心→指定点の距離
		pnt->p[2] = (m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[0] = (m_Cent3D.p[0]-(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[1] = m_Cent3D.p[1]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ｸﾞﾘｯﾄﾞ拘束
			id = m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[2] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[0]-(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[0] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// 基準点拘束
				ScrToPnt(0, &m_Bpoint, &bp1);							// 基準点位置
				bp1.p[1] = m_Cent3D.p[1]/1000.0;
			}
			if(m_BlBind) {												// 基準線拘束
				ScrToPnt(0, &m_Blspnt, &bp2);							// 基準線始点位置
				bp2.p[1] = m_Cent3D.p[1]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// 基準線終点位置
				bp3.p[1] = m_Cent3D.p[1]/1000.0;
			}
		}
	} else {
		ret = FALSE;
	}
	if(ret&&m_GridBind) {												// 正常ｸﾞﾘｯﾄﾞ拘束時
		if(m_BpBind||m_BlBind) {										// 基準点/線拘束有
			ln0 = PntD(&spt, pnt);										// ｸﾘｯｸ点-ｸﾞﾘｯﾄﾞ点距離
			ln1 = (m_BpBind) ? PntD(&spt, &bp1) : DBL_MAX;				// ｸﾘｯｸ点-基準点距離
			ln2 = (m_BlBind) ? PntD(&spt, &bp2) : DBL_MAX;				// ｸﾘｯｸ点-基準線距離
			ln3 = (m_BlBind) ? PntD(&spt, &bp3) : DBL_MAX;
			if(ln0<ln1) {												// ln0-ln3の最小検索
				id = 0;
			} else {
				id = 1, ln0 = ln1;
			}
			if(ln0>ln2) {
				id = 2, ln0 = ln2;
			}
			if(ln0>ln3) {
				id = 3;
			}
			if(id==1) {													// 最小の座標を設定
				*pnt = bp1;
			} else if(id==2) {
				*pnt = bp2;
			} else if(id==3) {
				*pnt = bp3;
			}
		}
	}
	if(bind==1) {														// <ｸﾞﾘｯﾄﾞ拘束OFF指定時>
		m_GridBind = grid;												// 拘束状態復元
	} else if(bind==2) {												// <ｸﾞﾘｯﾄﾞ拘束ON指定時>
		m_GridBind = grid, m_BpBind = bpoint, m_BlBind = bline;			// 拘束状態復元
	}
	return ret;
}

/******************************************************************/
BOOL CJcad3GlbView::CalcWorldScreen(CPoint* point,
                                    const PNTTYPE& pnt, int plane)		//<<< World->Screen座標変換
/******************************************************************/
{
	CPoint cp;
	CSize  dist;
	BOOL   ret=TRUE;

	if(plane==XY) {
		cp = CPoint((PlRect[XY].left+PlRect[XY].right)/2,
					(PlRect[XY].top+PlRect[XY].bottom)/2);
		dist.cy = ((int)(pnt.p[0]*1000.0)-m_Cent3D.p[0])*PlSize/m_Fovy;	// 3D変換
		dist.cx = ((int)(pnt.p[1]*1000.0)-m_Cent3D.p[1])*PlSize/m_Fovy;
		*point = dist+cp;
	} else if(plane==YZ) {
		cp = CPoint((PlRect[YZ].left+PlRect[YZ].right)/2,
					(PlRect[YZ].top+PlRect[YZ].bottom)/2);
		dist.cx = ((int)(pnt.p[1]*1000.0)-m_Cent3D.p[1])*PlSize/m_Fovy;	// 3D変換
		dist.cy = (m_Cent3D.p[2]-(int)(pnt.p[2]*1000.0))*PlSize/m_Fovy;
		*point = dist+cp;
	} else if(plane==ZX) {
		cp = CPoint((PlRect[ZX].left+PlRect[ZX].right)/2,
					(PlRect[ZX].top+PlRect[ZX].bottom)/2);
		dist.cy = (m_Cent3D.p[2]-(int)(pnt.p[2]*1000.0))*PlSize/m_Fovy;	// 3D変換
		dist.cx = (m_Cent3D.p[0]-(int)(pnt.p[0]*1000.0))*PlSize/m_Fovy;
		*point = dist+cp;
	} else {
		ret = FALSE;
	}
	return ret;
}

/*****************************************************/
void CJcad3GlbView::LimitValue(int plane, double* dd)					//<<< ﾏｳｽ入力制限値
/*****************************************************/
{
	int    i1, i2;
	double d1, d2;

	i1 = plane;															// 指定平面有効座標軸
	i2 = (plane+1>2) ? 0 : plane+1;
	d1 = dd[i1]/dd[i2];													// 垂直方向の座標軸に
	d2 = dd[i2]/dd[i1];													// 対する比率
	if(fabs(d1)>1.732) {												// <i1軸に近い(30度以内)>
		dd[i2] = 0.0;
	} else if(fabs(d2)>1.732) {											// <i2軸に近い(30度以内)>
		dd[i1] = 0.0;
	} else {															// <i1,i2軸の中間>
		if(fabs(dd[i1])>fabs(dd[i2])) {									// 大きい方の座標を採用
			dd[i2] = (dd[i2]>0) ? fabs(dd[i1]) : -fabs(dd[i1]);
		} else {
			dd[i1] = (dd[i1]>0) ? fabs(dd[i2]) : -fabs(dd[i2]);
		}
	}
}

/*******************************************************************/
BOOL CJcad3GlbView::LimitPoint(int plane, PNTTYPE* pa, PNTTYPE* pb)		//<<< ﾏｳｽ入力制限座標
/*******************************************************************/
{
	int    i, i1, i2;
	double dd[DIM], d1, d2;
	BOOL   ret=FALSE;

	i1 = plane;															// 指定平面有効座標軸
	i2 = (plane+1>2) ? 0 : plane+1;
	for(i=0; i<DIM; i++) {
		dd[i] = pa->p[i]-pb->p[i];										// 前点との誤差算出
	}
	d1 = dd[i1]/dd[i2];													// 垂直方向の座標軸に
	d2 = dd[i2]/dd[i1];													// 対する比率
	if(fabs(d1)>1.732) {												// <i1軸に近い(30度以内)>
		dd[i2] = 0.0;
	} else if(fabs(d2)>1.732) {											// <i2軸に近い(30度以内)>
		dd[i1] = 0.0;
	} else {															// <i1,i2軸の中間>
		if(fabs(dd[i1])>fabs(dd[i2])) {									// 大きい方の座標を採用
			dd[i2] = (dd[i2]>0) ? fabs(dd[i1]) : -fabs(dd[i1]);
		} else {
			dd[i1] = (dd[i1]>0) ? fabs(dd[i2]) : -fabs(dd[i2]);
		}
		ret = TRUE;
	}
	for(i=0; i<DIM; i++) {
		pa->p[i] = dd[i]+pb->p[i];										// 今回点復元
	}
	return ret;
}

/******************************************************/
void CJcad3GlbView::LimitAngle(int plane, PNTTYPE* pa,
                               PNTTYPE* pb, double th)					//<<< ﾏｳｽ角度指定制限
/******************************************************/
{
	int    i, i1, i2;
	double dd[DIM], d1, d2;

	i1 = plane;															// 指定平面有効座標軸
	i2 = (plane+1>2) ? 0 : plane+1;
	for(i=0; i<DIM; i++) {
		dd[i] = pa->p[i]-pb->p[i];										// 前点との誤差算出
	}
	d1 = cos(th);														// d1軸方向の比率
	d2 = sin(th);														// d2軸方向の比率
	if(fabs(dd[i1])>fabs(dd[i2])) {										// ﾏｳｽ移動大方向採用
		if(fabs(d1)>EPSILON) {
			dd[i2] = dd[i1]*d2/d1;
		} else {
			dd[i1] = dd[i2]*d1/d2;
		}
	} else {
		if(fabs(d2)>EPSILON) {
			dd[i1] = dd[i2]*d1/d2;
		} else {
			dd[i2] = dd[i1]*d2/d1;
		}
	}
	for(i=0; i<DIM; i++) {
		pa->p[i] = dd[i]+pb->p[i];										// 今回点復元
	}
}

/************************************/
int CJcad3GlbView::CountEdtObj(void)									//<<< 編集対象立体ｶｳﾝﾄ
/************************************/
{
	OBJTYPE* op;
	int      dsp, edt, cnt1=0, cnt2=0;

	BaseObjPtr(1, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ(表示対象)
	while(op) {															// 指定立体が存在する
		cnt1++;
		NextObjPtr(1, &op, &dsp, &edt);
	}
	m_DspCount = cnt1;
	BaseObjPtr(2, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ(編集対象)
	while(op) {															// 指定立体が存在する
		cnt2++;
		NextObjPtr(2, &op, &dsp, &edt);
	}
	m_EdtCount = cnt2;
	return cnt2;
}

/****************************************/
void CJcad3GlbView::CheckEdtObjPrm(void)								//<<< 編集対象立体生成ﾊﾟﾗﾒｰﾀﾁｪｯｸ
/****************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, cnt=0, cno, mx;
	double   wt;

	BaseObjPtr(2, &op, &dsp, &edt);										// 先頭立体のﾎﾟｲﾝﾀ(編集対象)
	while(op) {															// 指定立体が存在する
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		cno = GetCmdNo(cmp);											// ｺﾏﾝﾄﾞNo取得
		if(cno==CRTCURVE||cno==CHGCURVE) {								// <曲線生成/変更>
			mx = GetParaInt(cmp, 2);									// 曲線化繰返数
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// 曲線重み係数
			cnt++; break;
		}
		if(cno==CRTSURF||cno==CHGSURF||									// <曲面生成/編集>
		   cno==CRTSWEEP||cno==CHGSWEEP) {								// <掃引曲面生成/編集>
			mx = GetParaInt(cmp, 4);									// 曲線化繰返数
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// 曲線重み係数
			cnt++; break;
		}
		NextObjPtr(2, &op, &dsp, &edt);
	}
	if(cnt==0) {														// <対象編集立体なし>
		mx = m_CurveRepeat;												// 曲線化繰返数取得
		wt = m_CurveWeight;												// 曲線重み係数取得
	}
	MainWnd->SetCrtPrm(mx, wt);											// 生成ﾊﾟﾗﾒｰﾀ設定
}

/*****************************************/
void CJcad3GlbView::InitActMenu(int mode)								//<<< ﾒﾆｭｰ初期処理
/*****************************************/
{
	if(m_ActMenu==M_JWL_GEM) {											// <宝石ｶｯﾄの場合>
		if(m_Char=='R') {												// Rｷｰ押下:ﾗｳﾝﾄﾞ
			m_GemNo = 0;
		} else if(m_Char=='O') {										// Oｷｰ押下:ｵｰﾊﾞﾙ
			m_GemNo = 1;
		} else if(m_Char=='M') {										// Mｷｰ押下:ﾏｰｷｰｽﾞ
			m_GemNo = 2;
		} else if(m_Char=='P') {										// Pｷｰ押下:ﾍﾟｱｼｪｰﾌﾟ
			m_GemNo = 3;
		} else if(m_Char=='H') {										// Hｷｰ押下:ﾊｰﾄｼｪｰﾌﾟ
			m_GemNo = 4;
		} else if(m_Char=='S') {										// Sｷｰ押下:ｼﾝｸﾞﾙ
			m_GemNo = 5;
		} else if(m_Char=='E') {										// Eｷｰ押下:ｴﾒﾗﾙﾄﾞ
			m_GemNo = 6;
		} else if(m_Char=='B') {										// Bｷｰ押下:ﾊﾞｹﾞｯﾄ
			m_GemNo = 7;
		} else if(m_Char=='T') {										// Tｷｰ押下:ﾃｰﾊﾟｰﾄﾞ
			m_GemNo = 8;
		} else if(m_Char=='U') {										// Uｷｰ押下:ﾛｰｽﾞ
			m_GemNo = 9;
		} else if(m_Char=='C') {										// Cｷｰ押下:ｶﾎﾞｼｮﾝ
			m_GemNo = 10;
		} else {
			CGemDlg dlg;
			if(dlg.DoModal()==IDOK) {									// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(&m_GemNo);									// ｶｯﾄ種類設定
			} else {
				m_ActMenu = NONE;										// ｷｬﾝｾﾙはﾒﾆｭｰ中止
				return;
			}
		}
	}
	if(m_ActMenu!=M_SURF_CPS&&m_ActMenu!=M_SURF_P_MOVE&&
       m_ActMenu!=M_SURF_L_MOVE&&m_ActMenu!=M_SURF_L_ROTATE&&
       m_ActMenu!=M_SURF_TWIST&&m_ActMenu!=M_SURF_SCALE&&
       m_ActMenu!=M_SURF_S_MOVE&&m_ActMenu!=M_SURF_B_MOVE&&
       m_ActMenu!=M_SURF_P_ADJUST&&m_ActMenu!=M_SURF_S_ADJUST&&
       m_ActMenu!=M_EDIT_CURVE) {										// 張力有効ﾒﾆｭｰ以外は
		m_SurfTension = 0;												// 張力係数初期化
	}
	m_InitMenu = mode;
	MenuCall(m_ActMenu, 0, 0, 0, 0);
	m_Char = 0;
}

/*****************************************/
void CJcad3GlbView::ExitActMenu(int menu)								//<<< ﾒﾆｭｰ後処理
/*****************************************/
{
	int flg=9;

	if(m_SaveMenu!=NONE&&m_ActMenu!=menu) {								// 保存ﾒﾆｭｰを終了の場合
		m_ActMenu = m_SaveMenu;											// 保存ﾒﾆｭｰを起動ﾒﾆｭｰへ
		m_SaveMenu = NONE;
		m_CursorNo_S = 0;
	}
	if(m_ActMenu==M_MEASURE_SCALE||m_ActMenu==M_MEASURE_GRAD) {			// 定規/分度器
		if(m_ActMenu!=menu) {											// 別ﾒﾆｭｰによる終了
			flg = 8;
		}
	}
	MenuCall(m_ActMenu, 0, 0, 0, flg);
}

/********************************************************************/
void CJcad3GlbView::MenuCall(int menu, UINT nflg,
                             const CPoint& point, int plane, int ev)	//<<< ﾒﾆｭｰ呼び出し
/********************************************************************/
{
	if(GetSelectNo()==1) {
		if(menu!=NONE&&
           menu!=M_SELECT_OBJ&&menu!=M_SCROLL&&
           menu!=M_ZOOM_BOX&&menu!=M_CRT_SETSECTION&&
           menu!=M_EDIT_LINES&&menu!=M_EDIT_LINE&&
           menu!=M_SURF_P_ADJUST&&menu!=M_SURF_S_ADJUST&&
           menu!=M_EDIT_PASTEOBJ) {
			ResetSelNo();
		}
	}
	if(menu==M_SURF_CPS) {
		MouseCPointSpace(nflg, point, plane, ev);						// 曲編 CPS
	} else if(menu==M_SURF_P_MOVE) {
		MousePointMove(nflg, point, plane, ev);							// 曲編 制御点移動
	} else if(menu==M_SURF_L_MOVE) {
		MouseLineMove(nflg, point, plane, ev);							// 曲編 制御点列移動
	} else if(menu==M_SURF_L_INST) {
		MouseLineInsert(nflg, point, plane, ev);						// 曲編 制御点列挿入
	} else if(menu==M_SURF_L_ROTATE) {
		MouseLineRotate(nflg, point, plane, ev);						// 曲編 制御点列回転
	} else if(menu==M_SURF_TWIST) {
		MouseLineTwist(nflg, point, plane, ev);							// 曲編 ひねり
	} else if(menu==M_SURF_SCALE) {
		MouseLineScale(nflg, point, plane, ev);							// 曲編 拡大/縮小
	} else if(menu==M_SURF_S_MOVE) {
		MouseSPointMove(nflg, point, plane, ev);						// 曲編 断面制御点移動
	} else if(menu==M_SURF_B_MOVE) {
		MouseBPointMove(nflg, point, plane, ev);						// 曲編 ﾍﾞｰｽ線移動
	} else if(menu==M_SURF_P_ADJUST) {
		MouseLineAdjust(nflg, point, plane, ev);						// 曲編 制御点曲線合わせ
	} else if(menu==M_SURF_S_ADJUST) {
		MouseObjectAdjust(nflg, point, plane, ev);						// 曲編 制御点立体合わせ
	} else if(menu==M_SURF_TENSION) {
		MouseTension(nflg, point, plane, ev);							// 曲編 張力係数
	} else if(menu==M_CRT_CURVE) {
		MouseCurve(nflg, point, plane, ev);								// 生成 曲線生成
	} else if(menu==M_EDIT_CURVE) {
		MouseECurve(nflg, point, plane, ev);							// 生成 曲線編集
	} else if(menu==M_CRT_EXTRCURVE) {
		MouseExtrCurve(nflg, point, plane, ev);							// 生成 曲線抽出
	} else if(menu==M_CRT_SWEEP) {
		MouseSweep(nflg, point, plane, ev);								// 生成 掃引体
	} else if(menu==M_CRT_REVOLVE) {
		MouseRevolve(nflg, point, plane, ev);							// 生成 回転体
	} else if(menu==M_CRT_PYRAMID) {
		MousePyramid(nflg, point, plane, ev);							// 生成 錘
	} else if(menu==M_CRT_SETSECTION) {
		MouseSetSection(nflg, point, plane, ev);						// 生成 曲線断面割付
	} else if(menu==M_EDIT_SPACE) {
		MouseSpace(nflg, point, plane, ev);								// 立編 ｽﾍﾟｰｽ
	} else if(menu==M_EDIT_MOVE) {
		MouseMove(nflg, point, plane, ev);								// 立編 移動
	} else if(menu==M_EDIT_ROTATE) {
		MouseRotate(nflg, point, plane, ev);							// 立編 回転
	} else if(menu==M_EDIT_MIRROR) {
		MouseMirror(nflg, point, plane, ev);							// 立編 鏡像
	} else if(menu==M_EDIT_SCALE) {
		MouseScale(nflg, point, plane, ev);								// 立編 ｻｲｽﾞ変更
	} else if(menu==M_EDIT_RESIZE) {
		MouseResize3(nflg, point, plane, ev);							// 立編 直線指定ﾘｻｲｽﾞ2
	} else if(menu==M_EDIT_RESIZE2) {
		MouseResize4(nflg, point, plane, ev);							// 立編 曲線指定ﾘｻｲｽﾞ2
	} else if(menu==M_EDIT_BEND) {
		MouseBend(nflg, point, plane, ev);								// 立編 円筒曲げ
	} else if(menu==M_EDIT_TWIST) {
		MouseTwist(nflg, point, plane, ev);								// 立編 立体ひねり
	} else if(menu==M_EDIT_SHEAR) {
		MouseShear(nflg, point, plane, ev);								// 立編 せん断
	} else if(menu==M_EDIT_LINES) {
		MouseLinesEdit(nflg, point, plane, ev);							// 立編 曲線指定移動
	} else if(menu==M_EDIT_LINE) {
		MouseLineEdit(nflg, point, plane, ev);							// 立編 基準線指定移動
	} else if(menu==M_EDIT_CIRCLE) {
		MouseCircleEdit(nflg, point, plane, ev);						// 立編 基準円指定移動
	} else if(menu==M_EDIT_PCUT) {
		MousePCut(nflg, point, plane, ev);								// 立編 平面分割
	} else if(menu==M_EDIT_PASTEOBJ) {
		MousePasteObject(nflg, point, plane, ev);						// 編集 ﾍﾟｰｽﾄ
	} else if(menu==M_SELECT_OBJ) {
		ObjChoose(nflg, point, plane, ev);								// 編集 立体選択
	} else if(menu==M_ZOOM_BOX) {
		ZoomBox(nflg, point, plane, ev);								// 指定倍率(BOX表示)
	} else if(menu==M_SCROLL) {
		Scroll(nflg, point, plane, ev);									// 画面ｽｸﾛｰﾙ
	} else if(menu==M_BASE_SET) {
		BasePointSet(nflg, point, plane, ev);							// 基準点登録
	} else if(menu==M_ALLSHADING) {
		MenuAllShad(nflg, point, plane, ev);							// ｼｪｰﾃﾞｨﾝｸﾞ
	} else if(menu==M_RENDERING) {
		MenuRender(nflg, point, plane, ev);								// ﾚﾝﾀﾞﾘﾝｸﾞ
	} else if(menu==M_JWL_STONE) {
		MouseJwlStone(nflg, point, plane, ev);							// 石枠生成
	} else if(menu==M_JWL_ARRANGE) {
		MouseJwlArrange(nflg, point, plane, ev);						// 石枠自動配置
	} else if(menu==M_JWL_GEM) {
		if(m_GemNo==0||m_GemNo==5||m_GemNo==9) {
			MouseGemSquare(nflg, point, plane, ev);						// 宝石ｶｯﾄ1(ﾗｳﾝﾄﾞ系)
		} else if(m_GemNo==1||m_GemNo==2||m_GemNo==3||
                  m_GemNo==4||m_GemNo==6||m_GemNo==7||m_GemNo==10) {
			MouseGemRect(nflg, point, plane, ev);						// 宝石ｶｯﾄ2(ｵｰﾊﾞﾙ系)
		} else if(m_GemNo==8) {
			MouseGemRect2(nflg, point, plane, ev);						// 宝石ｶｯﾄ3(ﾃｰﾊﾟｰﾄﾞ)
		}
	} else if(menu==M_JWL_EARTH) {
		MouseEarth(nflg, point, plane, ev);								// ｱｰｽ
	} else if(menu==M_MEASURE_DIST) {
		MouseMeasureDistance(nflg, point, plane, ev);					// 距離測定
	} else if(menu==M_MEASURE_ANGLE) {
		MouseMeasureAngle(nflg, point, plane, ev);						// 角度測定
	} else if(menu==M_MEASURE_CURVE) {
		MouseMeasureCurve(nflg, point, plane, ev);						// 曲線長さ測定
	} else if(menu==M_MEASURE_SCALE) {
		MouseMeasureScale(nflg, point, plane, ev);						// 定規
	} else if(menu==M_MEASURE_GRAD) {
		MouseMeasureProt(nflg, point, plane, ev);						// 分度器
	} else {
		EyeChange(nflg, point, plane, ev);								// 透視図視点変更
		ObjSelect(nflg, point, plane, ev);								// 立体選択
	}
}

/****************************************/
void CJcad3GlbView::InitializeMenu(void)								//<<< ﾒﾆｭｰ初期処理
/****************************************/
{
	if(m_SFuncMode<3) {													// <詳細>
		m_pFunction->MessageOut(-1, 0);									// ﾌｧﾝｸｼｮﾝﾒｯｾｰｼﾞ初期化
		if(m_SFuncMode>0) {												// <詳細[常駐以外]>
			HideFunction();												// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ消去
		}
	} else if(m_SFuncMode==4||m_SFuncMode==5) {							// <簡易[常駐以外]>
		HideFunction2();												// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2消去
	}
}

/********************************************/
void CJcad3GlbView::ExitMenu(int f1, int f2)							//<<< ﾒﾆｭｰ終了処理
/********************************************/
{
	if(m_ActMenu!=M_SURF_TENSION) {										// 張力係数初期化
		m_SurfTension = 0;
	}
	m_ActMenu = NONE;													// ﾒﾆｭｰ番号ｸﾘｱ
	Sphase = 0;															// ﾌｪｰｽﾞｸﾘｱ
	m_CursorNo = 0;														// ｶｰｿﾙNoｸﾘｱ
	if(f1&&!(m_BtnEnv&0x1)) {											// 複写ﾎﾞﾀﾝｸﾘｱ
		m_CopyBtn = 0;
	}
	m_CopyInp = 0;
	m_CopyNum = 0;
	if(f2&&!(m_BtnEnv&0x2)) {											// 数値ﾎﾞﾀﾝｸﾘｱ
		m_NumBtn = 0;
	}
	m_NumInp = 0;
	InitChar();															// ｷｰ入力初期化
	CheckEdtObjPrm();													// 編集対象立体生成ﾊﾟﾗﾒｰﾀﾁｪｯｸ
}

/*************************************/
void CJcad3GlbView::InitTRLimit(void)									//<<< ﾄﾗｯｶｰ範囲限定初期化
/*************************************/
{
	m_XLMin = 1.0-DBL_MAX, m_XLMax = DBL_MAX;							// X 限定なし
	m_YLMin = 1.0-DBL_MAX, m_YLMax = DBL_MAX;							// Y 限定なし
	m_ZLMin = 1.0-DBL_MAX, m_ZLMax = DBL_MAX;							// Z 限定なし
}

/*******************************************/
BOOL CJcad3GlbView::SetTRLimit(OBJTYPE* op)								//<<< ﾄﾗｯｶｰ範囲限定設定
/*******************************************/
{
	PNTTYPE cpt;
	BOXTYPE box;

	if(op!=NULL) {														// <OP指定あり>
		GetObjBox(op, &box);											// 立体BOX取得
	} else {															// <OP指定なし>
		GetAreaBox(2, &box);											// 選択立体BOX取得
	}
	BoxCenter(box, &cpt);												// BOXｾﾝﾀｰ取得
	m_XLmtR = (double)((int)(cpt.p[0]*10000))/10000.0;					// 編集立体の中心
	m_YLmtR = (double)((int)(cpt.p[1]*10000))/10000.0;
	m_ZLmtR = (double)((int)(cpt.p[2]*10000))/10000.0;
	CLimitTrackerDlg dlg;
	dlg.SetValue(m_XLmtR, m_YLmtR, m_ZLmtR, m_XLmt1,
                 m_XLmt2, m_YLmt1, m_YLmt2, m_ZLmt1, m_ZLmt2);
	if(dlg.DoModal()==IDOK) {											// ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&m_XLmtR, &m_YLmtR, &m_ZLmtR, &m_XLmt1,
                     &m_XLmt2, &m_YLmt1, &m_YLmt2, &m_ZLmt1, &m_ZLmt2);
		m_XLMin = (m_XLmt1) ? 1.0-DBL_MAX : m_XLmtR-0.0001;				// X 最小側限定
		m_XLMax = (m_XLmt2) ? DBL_MAX : m_XLmtR+0.0001;					// X 最大側限定
		m_YLMin = (m_YLmt1) ? 1.0-DBL_MAX : m_YLmtR-0.0001;				// Y 最小側限定
		m_YLMax = (m_YLmt2) ? DBL_MAX : m_YLmtR+0.0001;					// Y 最大側限定
		m_ZLMin = (m_ZLmt1) ? 1.0-DBL_MAX : m_ZLmtR-0.0001;				// Z 最小側限定
		m_ZLMax = (m_ZLmt2) ? DBL_MAX : m_ZLmtR+0.0001;					// Z 最大側限定
		return TRUE;
	} else {
		InitTRLimit();													// ﾄﾗｯｶｰ範囲限定初期化
		return FALSE;
	}
}

/******************************************************************/
void CJcad3GlbView::GetTRLimit(double* x1, double* x2, double* y1,
                               double* y2, double* z1, double* z2)		//<<< ﾄﾗｯｶｰ範囲限定取得
/******************************************************************/
{
	*x1 = m_XLMin, *x2 = m_XLMax, *y1 = m_YLMin;
	*y2 = m_YLMax, *z1 = m_ZLMin, *z2 = m_ZLMax;
}
