///////////////////////////////////////////////////////////////////////////// 
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewScrCtrl.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*******************************************/
void MouseBind(CPoint& point, CPoint& base)								//<<< ﾏｳｽ入力座標拘束
/*******************************************/
{
	double hi;
	int    x=point.x-base.x, y=point.y-base.y;

	hi = (double)y/(double)x;											// Xに対するYの比を算出
	if(fabs(hi)<=1.0) {													// X軸に近い(45度以内)
		point.y = base.y;
	} else {															// Y軸に近い(45度以内)
		point.x = base.x;
	}
}

/********************************/
void CJcad3GlbView::ZoomIn(void)										//<<< ｽﾞｰﾑ･ｲﾝ(3面図)
/********************************/
{
	if(m_Fovy>200) {													// 表示高が規定値(μm)以上
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3面図表示状態保存
		m_Fovy = (int)(m_Fovy*0.8+0.5);
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列の再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		Display(1);														// 再描画
	}
}

/*********************************/
void CJcad3GlbView::ZoomOut(void)										//<<< ｽﾞｰﾑ･ｱｳﾄ(3面図)
/*********************************/
{
	if(m_Fovy<1000000) {												// 表示高が規定値(μm)以下
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3面図表示状態保存
		m_Fovy = (int)(m_Fovy/0.8+0.5);
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列の再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		Display(1);														// 再描画
	}
}

/**********************************/
void CJcad3GlbView::AutoZoom(void)										//<<< 自動倍率
/**********************************/
{
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, cnt=0;
	double   x, y, z, fvy1, fvy2, fvy3, fvy4, fvy5, fvy6;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// 表示立体無は中止
		return;
	}
	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3面図表示状態保存
	GetAreaBox(1, &box);												// 全立体のBOXの最大値取得
	if(m_DAraFlg) {														// <表示範囲指定の場合>
		for(i=0; i<DIM; i++) {
			if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
				box.bp[0].p[i] = m_DAra.bp[0].p[i];
			}
			if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
				box.bp[1].p[i] = m_DAra.bp[1].p[i];
			}
		}
	}
	BoxCenter(box, &cpt);												// ﾎﾞｯｸｽｾﾝﾀｰを3面中心
	ScrToPnt(1, &m_Cent3D, &cpt);										// PNTTYPE => SCRTYPE
	x = box.bp[1].p[0]-box.bp[0].p[0];									// x,y,zｻｲｽﾞ取得
	y = box.bp[1].p[1]-box.bp[0].p[1];
	z = box.bp[1].p[2]-box.bp[0].p[2];
	fvy1 = -1.0, fvy2 = -1.0, fvy3 = -1.0;
	fvy4 = -1.0 ,fvy5 = -1.0, fvy6 = -1.0;
	if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {									// ｽｸﾘｰﾝ表示ｻｲｽﾞ
		cnt++;
		fvy1 = x*0.5/ScrDiv2;
		fvy2 = y*0.5*PlSizeH[XY]/(ScrDiv2*PlSizeW[XY]);
	}
	if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
		cnt++;
		fvy3 = z*0.5/(1.0-ScrDiv2);
		fvy4 = y*0.5*PlSizeH[YZ]/((1.0-ScrDiv2)*PlSizeW[YZ]);
	}
	if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
		cnt++;
		fvy5 = z*0.5/(1.0-ScrDiv2);
		fvy6 = x*0.5*PlSizeH[ZX]/((1.0-ScrDiv2)*PlSizeW[ZX]);
	}
	if(cnt>0) {															// <3面の何れかが有効>
		fvy1 = (fvy1<fvy2) ? fvy2 : fvy1;
		fvy1 = (fvy1<fvy3) ? fvy3 : fvy1;
		fvy1 = (fvy1<fvy4) ? fvy4 : fvy1;
		fvy1 = (fvy1<fvy5) ? fvy5 : fvy1;
		fvy1 = (fvy1<fvy6) ? fvy6 : fvy1;
		m_Fovy = (int)(1000.0*fvy1/0.8+0.5);							// 画面表示幅更新
	}
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	if(m_GridDisp) {
		SetGrid();														// ｸﾞﾘｯﾄﾞ設定
	}
	Display(1);															// 再描画
}

/***********************************/
void CJcad3GlbView::ExactZoom(void)										//<<< 実寸表示
/***********************************/
{
	CWnd*    m_pwnd;
	CDC*     m_pcdc;
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, sh, sw, px, py;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// 表示立体無は中止
		return;
	}
	if(m_FChar&0x1) {													// <[F5]:倍率補正>
		CInputDlg dlg(27);
		dlg.SetValue(m_ExactRev);										// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&m_ExactRev);									// 補正倍率入力
		} else {														// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙ
			return;
		}
	}
	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3面図表示状態保存
	SetScreenNo(0);														// 表示範囲設定
	GetAreaBox(1, &box);												// 全立体のBOXの最大値取得
	if(m_DAraFlg) {														// <表示範囲指定の場合>
		for(i=0; i<DIM; i++) {
			if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
				box.bp[0].p[i] = m_DAra.bp[0].p[i];
			}
			if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
				box.bp[1].p[i] = m_DAra.bp[1].p[i];
			}
		}
	}
	BoxCenter(box, &cpt);												// ﾎﾞｯｸｽｾﾝﾀｰを3面中心
	ScrToPnt(1, &m_Cent3D, &cpt);										// PNTTYPE => SCRTYPE
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	sw = PlRect[XY].Width(), sh = PlRect[XY].Height();					// 透視図転送元ｻｲｽﾞ
	m_pwnd = GetDesktopWindow();										// ﾃﾞｽｸﾄｯﾌﾟｳｨﾝﾄﾞｳ
	m_pcdc = m_pwnd->GetDC();											// ｱｸﾃｨﾌﾞｳｨﾝﾄﾞｳのｸﾗｲｱﾝﾄDC
	px = m_pcdc->GetDeviceCaps(LOGPIXELSX);								// X方向画面dpi
	py = m_pcdc->GetDeviceCaps(LOGPIXELSY);								// Y方向画面dpi
	m_Fovy = (int)((25.4*1000.0*sh)/(py*m_ExactRev));					// 画面表示幅更新
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	if(m_GridDisp) {
		SetGrid();														// ｸﾞﾘｯﾄﾞ設定
	}
	Display(0);															// 再描画
}

/**********************************************************/
void CJcad3GlbView::ZoomBox(UINT flg, const CPoint& point,
                            int plane, int ms)							//<<< 画面指定倍率
/**********************************************************/
{
	PNTTYPE pnt;
	CPoint  cent;
	CSize   size;
	int     cx, cy, len, h;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図上の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// ﾏｳｽ左釦押しながらﾄﾞﾗｯｸﾞ
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		CalcScreenWorld(0, &pnt, point, plane);							// 座標点取得
		DrawBox(BaseCursor, pnt, plane);								// BOX表示
		Display(0);														// 再描画
	} else if(ms==3) {													// BOXの決定
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		cent = BasePoint + point;
		cent.x = (int)((cent.x/2.0)+0.5);								// BOXの中心位置算出
		cent.y = (int)((cent.y/2.0)-0.5);								//　 （四捨五入付）
		CalcScreenWorld(0, &pnt, cent, Plane);							// 中心点を3D変換
		m_pState->SetCent3D(pnt);
		ScrToPnt(1, &m_Cent3D, &pnt);									// PNTTYPE => SCRTYPE
		size = point - BasePoint;
		cx = abs(size.cx), cy = abs(size.cy);
		len = cy<cx ? cx : cy;
		if(len>2) {
			h = (int)(m_Fovy*len/PlSizeH[plane]/0.9+0.5);				// 表示高算出
			m_Fovy = (h>200&&h<1000000) ? h : m_Fovy;					// 表示高が規定値内は表示高設定
		}
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列の再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		DeleteBox();													// 矩形破棄
		Display(1);														// 再描画
	} else if(ms==0) {													// ﾒﾆｭｰ開始
		m_CursorNo = (m_ActMenu!=NONE) ? 3 : m_CursorNo;				// ｶｰｿﾙNo設定
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3面図表示状態保存
	} else if(ms==4||ms==9) {											// ﾒﾆｭｰ終了
		m_ActMenu = m_SaveMenu;											// 起動中ﾒﾆｭｰへ戻す
		m_SaveMenu = NONE;												// 保存ﾒﾆｭｰ番号ｸﾘｱ
		m_CursorNo = m_CursorNo_S;										// ｶｰｿﾙNo復元
		m_CursorNo_S = 0;												// 保存ｶｰｿﾙNoｸﾘｱ
		InitChar();														// ｷｰ入力初期化
	}
}

/*********************************************************/
void CJcad3GlbView::Scroll(UINT flg, const CPoint& point,
                           int plane, int ms)							//<<< 画面ｽｸﾛｰﾙ
/*********************************************************/
{
	PNTTYPE pnt, cpt;
	int     pln2=(plane+1)%3;
	double  dd[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図上の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		m_Cent3D = SscrA;
		CalcScreenWorld(2, &pnt, point, plane);							// 座標点取得
		for(int i=0; i<DIM; i++) {
			dd[i] = pnt.p[i]-SpntA.p[i];								// 最初の点と現在点との差
		}
		if(flg&m_LimKey) {
			LimitValue(plane, dd);										// ﾏｳｽ入力制限値
		}
		m_Cent3D.p[plane] = SscrA.p[plane]-(int)(dd[plane]*1000.0);
		m_Cent3D.p[pln2] = SscrA.p[pln2]-(int)(dd[pln2]*1000.0);
		ScrToPnt(0, &m_Cent3D, &cpt);									// SCRTYPE => PNTTYPE
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列の再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		Display(1);														// 再描画
	} else if(ms==1) {													// ﾏｳｽ左釦押下
		CalcScreenWorld(2, &SpntA, point, plane);						// ｽｸﾛｰﾙ元点の座標取得
		SscrA = m_Cent3D;												// 3面図の参照点保存
	} else if(ms==3) {													// ﾏｳｽ左釦解放
		m_Cent3D = SscrA;
		CalcScreenWorld(2, &pnt, point, plane);							// 座標点取得
		for(int i=0; i<DIM; i++) {
			dd[i] = pnt.p[i]-SpntA.p[i];								// 最初の点と現在点との差
		}
		if(flg&m_LimKey) {
			LimitValue(plane, dd);										// ﾏｳｽ入力制限値
		}
		m_Cent3D.p[plane] = SscrA.p[plane]-(int)(dd[plane]*1000.0);
		m_Cent3D.p[pln2] = SscrA.p[pln2]-(int)(dd[pln2]*1000.0);
		ScrToPnt(0, &m_Cent3D, &cpt);									// SCRTYPE => PNTTYPE
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列の再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		Display(1);														// 再描画
	} else if(ms==0) {													// ﾒﾆｭｰ開始
		m_CursorNo = (m_ActMenu!=NONE) ? 2 : m_CursorNo;				// ｶｰｿﾙNo設定
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3面図表示状態保存
	} else if(ms==4||ms==9) {											// ﾒﾆｭｰ終了
		m_ActMenu = m_SaveMenu;											// 起動中ﾒﾆｭｰへ戻す
		m_SaveMenu = NONE;												// 保存ﾒﾆｭｰ番号ｸﾘｱ
		m_CursorNo = m_CursorNo_S;										// ｶｰｿﾙNo復元
		m_CursorNo_S = 0;												// 保存ｶｰｿﾙNoｸﾘｱ
		InitChar();														// ｷｰ入力初期化
	}
}

/*********************************************/
void CJcad3GlbView::Scroll(int axis, int dir)							//<<< 画面ｽｸﾛｰﾙ･ﾂｰﾙ
/*********************************************/
{
	PNTTYPE cpt;

	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3面図表示状態保存
	m_Cent3D.p[axis] = m_Cent3D.p[axis]+(dir*m_GridSpace);				// 3面図の中心点変更
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	if(m_GridDisp) {
		SetGrid();														// ｸﾞﾘｯﾄﾞ設定
	}
	Display(1);															// 再描画
}

/*****************************************/
void CJcad3GlbView::BackScreenState(void)								//<<< 3面図表示状態戻し
/*****************************************/
{
	m_Cent3D = m_Cent3D_SS, m_Fovy = m_Fovy_SS;							// 3面図表示状態戻し
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	if(m_GridDisp) {
		SetGrid();														// ｸﾞﾘｯﾄﾞ設定
	}
	Display(1);															// 再描画
}

/***************************************************************/
void CJcad3GlbView::BasePointSet(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 基準点/線登録
/***************************************************************/
{
	PNTTYPE pnt, cpt;
	BOXTYPE box;
	int     i1, i2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i1 = plane;													// 有効座標軸設定
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si6].p[i1] = pnt.p[i1], Spa[Si6].p[i2] = pnt.p[i2];		// 座標値変更
			if(Si1==0) {												// 基準点の場合
				ScrToPnt(1, &m_Bpoint, &Spa[0]);
			} else {													// 基準線の場合
				ScrToPnt(1, &m_Blspnt, &Spa[0]);
				ScrToPnt(1, &m_Blepnt, &Spa[1]);
			}
			SetBase();													// 基準点/線設定
			Tracker1.Setup(Spa, Sia, NULL, Si2, 5, 0);					// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((Si6=Tracker1.HitTracker(1, plane, pnt, Spa, Si2))!=-1) {// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sphase = 2, Sbase = Spa[Si6];
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			if(m_Char=='P') {											// Pｷｰ押下は基準点
				Si1 = 0;
			} else if(m_Char=='L') {									// Lｷｰ押下は基準線
				Si1 = 1;
			} else {
				CBasesetDlg dlg;
				if(dlg.DoModal()==IDOK) {								// 点/線選択ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&Si1);									// 基準点/線選択
				} else {
					ExitMenu(0, 0);										// ﾒﾆｭｰ終了
				}
			}
			if(m_ActMenu!=NONE) {										// <中止でない場合>
				if(Si1==0) {											// 基準点の場合
					Si2 = 1;
					ScrToPnt(0, &m_Bpoint, &Spa[0]);
					Si3 = m_BpDisp, m_BpDisp = 1;						// 基準点表示状態保存
					m_Cent3D = m_Bpoint;								// 基準点を3D中心点
				} else {												// 基準線の場合
					Si2 = 2;
					ScrToPnt(0, &m_Blspnt, &Spa[0]);
					ScrToPnt(0, &m_Blepnt, &Spa[1]);
					Si3 = m_BlDisp, m_BlDisp = 1;						// 基準線表示状態保存
					box.bp[0] = Spa[0], box.bp[1] = Spa[1];				// 基準線始終点でBOX
					if(m_Blspnt.p[0]>m_Blepnt.p[0]) {
						box.bp[0].p[0] = Spa[1].p[0];
					}
					if(m_Blspnt.p[1]>m_Blepnt.p[1]) {
						box.bp[0].p[1] = Spa[1].p[1];
					}
					if(m_Blspnt.p[2]>m_Blepnt.p[2]) {
						box.bp[0].p[2] = Spa[1].p[2];
					}
					if(m_Blspnt.p[0]<m_Blepnt.p[0]) {
						box.bp[1].p[0] = Spa[1].p[0];
					}
					if(m_Blspnt.p[1]<m_Blepnt.p[1]) {
						box.bp[1].p[1] = Spa[1].p[1];
					}
					if(m_Blspnt.p[2]<m_Blepnt.p[2]) {
						box.bp[1].p[2] = Spa[1].p[2];
					}
					m_Fovy=(int)((BoxCenter(box, &cpt)*1000.0)/0.8+0.5);// ﾎﾞｯｸｽのｾﾝﾀｰ&最大幅取得
					ScrToPnt(1, &m_Cent3D, &cpt);
				}
				Si4 = m_BpBind, m_BpBind = 0;							// 基準点拘束状態保存
				Si5 = m_BlBind, m_BlBind = 0;							// 基準線拘束状態保存
				Sia[0] = 4, Sia[1] = 4;
				Tracker1.Setup(Spa, Sia, NULL, Si2, 5, 0);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;
				m_pState->SetCent3D(cpt);
				SetSeishaei(m_Cent3D, m_Fovy);							// 正射影行列の再構築
				ChangeTexPixels();										// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
				if(m_GridDisp) {
					SetGrid();											// ｸﾞﾘｯﾄﾞ設定
				}
				Display(0);												// 再描画
				m_CursorNo = 1;											// ｶｰｿﾙNo設定
			}
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si1==0) {
			m_BpDisp = Si3;												// 基準点/線表示状態を戻す
		} else {
			m_BlDisp = Si3;
		}
		m_BpBind = Si4, m_BlBind = Si5;									// 基準点/線拘束状態を戻す
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/**********************************/
void CJcad3GlbView::CentPars(void)										//<<< 参照点指定(透視図)
/**********************************/
{
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	PNTTYPE ept, cpt;

	m_EyeSv = m_Eye;													// 開始視点位置
	m_CentPs = m_Cent3D;												// 3面図中心点を透視図参照点
	ScrToPnt(0, &m_Eye, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	m_TruckLen = PntD(&ept, &cpt)*0.2;									// 視点→参照点距離
	m_TruckLen = (m_TruckLen<0.1) ? 0.1 : m_TruckLen;
	VecV(&ept, &cpt, &v1);												// 視線ﾍﾞｸﾄﾙ取得
	VecN(&v1);															// <視点ﾍﾞｸﾄﾙがZ軸と同>
	if(fabs(v1.p[0])<EPSILON&&
       fabs(v1.p[1])<EPSILON&&
       fabs(v1.p[2])-1.0<EPSILON) {
		m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
	} else {															// <視点ﾍﾞｸﾄﾙがZ軸と異>
		VecO(&v1, &v2, &m_HzVec);										// 横方向ﾍﾞｸﾄﾙ取得
		VecN(&m_HzVec);
	}
	VecO(&v1, &m_HzVec, &m_VtVec);										// 縦方向ﾍﾞｸﾄﾙ取得
	VecN(&m_VtVec);
	m_pState->SetCentPRS(cpt);
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	Display(2);															// 再描画
}

/*********************************/
void CJcad3GlbView::TruckUp(void)										//<<< ﾄﾗｯｸ･ｱｯﾌﾟ(透視図)
/*********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE vec;
	SCRTYPE len;
	int     i, ln, sa;

	ScrToPnt(0, &m_EyeSv, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	VecV(&ept, &cpt, &vec);												// 開始視線ﾍﾞｸﾄﾙ取得
	VecN(&vec);
	for(i=0; i<DIM; i++) {
		ln = (int)(vec.p[i]*m_TruckLen*1000.0);
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln>=0.5) ? 1 : 0;			// 誤差の補正
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln<=-0.5) ? -1 : sa;
		len.p[i] = ln+sa;												// 視点の移動距離算出
	}
	for(i=0; i<DIM; i++) {
		m_Eye.p[i] = m_Eye.p[i]+len.p[i];								// 視点を移動
	}
	if(m_Eye.p[0]==m_Cent3D.p[0]&&
       m_Eye.p[1]==m_Cent3D.p[1]&&
       m_Eye.p[2]==m_Cent3D.p[2]) {
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] = m_Eye.p[i]+len.p[i];
		}
	}
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	Display(2);															// 再描画
}

/***********************************/
void CJcad3GlbView::TruckDown(void)										//<<< ﾄﾗｯｸ･ﾀﾞｳﾝ(透視図)
/***********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE vec;
	SCRTYPE len;
	int     i, ln, sa;

	ScrToPnt(0, &m_EyeSv, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	VecV(&ept, &cpt, &vec);												// 開始視線ﾍﾞｸﾄﾙ取得
	VecN(&vec);
	for(i=0; i<DIM; i++) {
		ln = (int)(vec.p[i]*m_TruckLen*1000.0);
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln>=0.5) ? 1 : 0;			// 誤差の補正
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln<=-0.5) ? -1 : sa;
		len.p[i] = ln+sa;												// 視点の移動距離算出
	}
	for(i=0; i<DIM; i++) {
		m_Eye.p[i] = m_Eye.p[i]-len.p[i];								// 視点を移動
	}
	if(m_Eye.p[0]==m_Cent3D.p[0]&&
       m_Eye.p[1]==m_Cent3D.p[1]&&
       m_Eye.p[2]==m_Cent3D.p[2]) {
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] = m_Eye.p[i]-len.p[i];
		}
	}
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	Display(2);															// 再描画
}

/***********************************/
void CJcad3GlbView::EyeSet(int flg)										//<<< 透視図変更
/***********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	double  len;

	ScrToPnt(0, &m_Eye, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	len = PntD(&ept, &cpt);												// 視点→参照点距離
	m_Eye = m_CentPs;													// 視点を参照点に合わせる
	if(flg==0) {														// <XY平面(表)>
		m_Eye.p[2] += (int)(len*1000.0);
	} else if(flg==1) {													// <YZ平面(表)>
		m_Eye.p[0] += (int)(len*1000.0);
	} else if(flg==2) {													// <ZX平面(表)>
		m_Eye.p[1] += (int)(len*1000.0);
	} else if(flg==3) {													// <XY平面(裏)>
		m_Eye.p[2] -= (int)(len*1000.0);
	} else if(flg==4) {													// <YZ平面(裏)>
		m_Eye.p[0] -= (int)(len*1000.0);
	} else {															// <ZX平面(裏)>
		m_Eye.p[1] -= (int)(len*1000.0);
	}
	m_EyeSv = m_Eye;													// 開始視点位置
	ScrToPnt(0, &m_Eye, &ept);
	m_TruckLen = PntD(&ept, &cpt)*0.2;									// 視点→参照点距離X0.2
	m_TruckLen = (m_TruckLen<0.1) ? 0.1 : m_TruckLen;
	VecV(&ept, &cpt, &v1);												// 視線ﾍﾞｸﾄﾙ取得
	VecN(&v1);															// <視点ﾍﾞｸﾄﾙがZ軸と同>
	if(fabs(v1.p[0])<EPSILON&&
       fabs(v1.p[1])<EPSILON&&
       fabs(v1.p[2])-1.0<EPSILON) {
		m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
	} else {															// <視点ﾍﾞｸﾄﾙがZ軸と異>
		VecO(&v1, &v2, &m_HzVec);										// 横方向ﾍﾞｸﾄﾙ取得
		VecN(&m_HzVec);
	}
	VecO(&v1, &m_HzVec, &m_VtVec);										// 縦方向ﾍﾞｸﾄﾙ取得
	VecN(&m_VtVec);
	m_pState->SetEye(ept);
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	Display(2);															// 再描画
}

/************************************************************/
void CJcad3GlbView::EyeChange(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< 視点変更
/************************************************************/
{
	PNTTYPE ept;
	CPoint  npoint(point);
	double  hzth, vtth;
	int     sax, say;

	if(plane!=PRS) {
		return;
	}
	if(ms==2&&flg&MK_LBUTTON&&Sphase2==1) {								// ﾏｳｽの左釦押下中のみ有効
		if(flg&m_LimKey) {
			MouseBind(npoint, BasePoint);								// 回転方向拘束
			m_Eye = SscrA, m_EyeSv = SscrB;
			m_VtVec = SvecA, m_HzVec = SvecB;
			sax = npoint.x - BasePoint.x;								// 基準点と現在点との差
			say = BasePoint.y - npoint.y;
		} else {
			sax = npoint.x - Spoint.x;									// 1点前と現在点との差
			say = Spoint.y - npoint.y;
		}
		Spoint = npoint;
		hzth = -(PI*2.0*sax/PlRect[PRS].Width());						// 水平方向回転角算出
		vtth = -(PI*2.0*say/PlRect[PRS].Height());						// 垂直方向回転角算出
		EyeMove(0, hzth);
		EyeMove(1, vtth);
		ScrToPnt(0, &m_Eye, &ept);
		m_pState->SetEye(ept);
		SetPerspective();												// 透視法射影行列の再構築
		SetXYZGuide();													// XYZｶﾞｲﾄﾞ設定
		Display(2);														// 再描画
	} else if(ms==1) {													// ﾏｳｽ左釦押下
		Spoint = point;
		SscrA = m_Eye, SscrB = m_EyeSv;
		SvecA = m_VtVec, SvecB = m_HzVec;
		Sphase2 = 1;
	} else if(ms==3) {													// ﾏｳｽ左釦解放
		Sphase2 = 0;
	}
}

/*************************************************/
void CJcad3GlbView::EyeChange(int dir, double th)						//<<< 視点回転
/*************************************************/						// dir:0→横，1→縦
{
	PNTTYPE ept;

	EyeMove(dir, th);
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// 透視法射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	Display(2);															// 再描画
}

/***********************************************/
void CJcad3GlbView::EyeMove(int dir, double th)							//<<< 視点移動
/***********************************************/						// dir:0→横，1→縦
{
	PNTTYPE pt1, pt2;
	VECTYPE v1;
	int     i;
	double  rot[3][3];

	if(dir==1) {
		SetRotateMatrix(m_HzVec, th, rot);								// 縦方向回転行列を取得
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] -= m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] -= m_CentPs.p[i];
		}
		ScrToPnt(0, &m_Eye, &pt1);
		ScrToPnt(0, &m_EyeSv, &pt2);
		VecRotate(&pt1, rot);											// 視点回転
		VecRotate(&pt2, rot);											// 開始視点回転
		ScrToPnt(1, &m_Eye, &pt1);
		ScrToPnt(1, &m_EyeSv, &pt2);
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] += m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] += m_CentPs.p[i];
		}
		ScrToPnt(0, &m_EyeSv, &pt1);
		ScrToPnt(0, &m_CentPs, &pt2);
		VecV(&pt1, &pt2, &v1);											// 視線ﾍﾞｸﾄﾙ取得
		VecN(&v1);
		VecO(&v1, &m_HzVec, &m_VtVec);									// 縦方向ﾍﾞｸﾄﾙ取得
		VecN(&m_VtVec);
	} else {
		SetRotateMatrix(m_VtVec, th, rot);								// 横方向回転行列取得
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] -= m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] -= m_CentPs.p[i];
		}
		ScrToPnt(0, &m_Eye, &pt1);
		ScrToPnt(0, &m_EyeSv, &pt2);
		VecRotate(&pt1, rot);											// 視点回転
		VecRotate(&pt2, rot);											// 開始視点回転
		ScrToPnt(1, &m_Eye, &pt1);
		ScrToPnt(1, &m_EyeSv, &pt2);
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] += m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] += m_CentPs.p[i];
		}
		ScrToPnt(0, &m_EyeSv, &pt1);
		ScrToPnt(0, &m_CentPs, &pt2);
		VecV(&pt1, &pt2, &v1);											// 視線ﾍﾞｸﾄﾙ取得
		VecN(&v1);
		VecO(&m_VtVec, &v1, &m_HzVec);									// 横方向ﾍﾞｸﾄﾙ取得
		VecN(&m_HzVec);
	}
}

/*******************************************/
void CJcad3GlbView::SetHomePosition(int no)								//<<< 画面状態保存
/*******************************************/
{
	CString str;
	int     i, j, n;

	str.Format(IDS_SAVE_SCRN, no);
	if(AfxMessageBox(str, MB_YESNO)==IDYES) {							// <保存OK>
		n = no - 1;
		m_Cent3D_S[n]    = m_Cent3D;									// 画面表示状態保存
		m_Fovy_S[n]      = m_Fovy;
		m_CentPs_S[n]    = m_CentPs;
		m_Eye_S[n]       = m_Eye;
		m_EyeSv_S[n]     = m_EyeSv;
		m_TruckLen_S[n]  = m_TruckLen;
		m_HzVec_S[n]     = m_HzVec;
		m_VtVec_S[n]     = m_VtVec;
		m_GridDisp_S[n]  = m_GridDisp;
		m_GridBind_S[n]  = m_GridBind;
		m_GridSpace_S[n] = m_GridSpace;
		m_GridIntvl_S[n] = m_GridIntvl;
		m_Bpoint_S[n]    = m_Bpoint;
		m_Blspnt_S[n]    = m_Blspnt;
		m_Blepnt_S[n]    = m_Blepnt;
		m_BpDisp_S[n]    = m_BpDisp;
		m_BpBind_S[n]    = m_BpBind;
		m_BlDisp_S[n]    = m_BlDisp;
		m_BlBind_S[n]    = m_BlBind;
		m_GlbDisp_S[n]   = m_GlbDisp;
		m_GlbChange_S[n] = m_GlbChange;
		m_DspTmpNo_S[n]  = m_DspTmpNo;
		for(i=0; i<8; i++) {
			for(j=0; j<3; j++) {
				m_TempPath_S[n][i][j] = m_TempPath[i][j];
			}
		}
	}
}

/***********************************************/
void CJcad3GlbView::RestoreHomePosition(int no)							//<<< 画面状態復帰
/***********************************************/
{
	PNTTYPE p1, p2;
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	int     i, j, n, hm;

	if(no>0) {															// <ﾒﾆｭｰから起動の場合>
		hm = 1, n = no - 1;
	} else {															// <新規起動の場合>
		hm = 0, n = 0;
	}
	if(hm||(m_ScrEnv2&0x1)) {											// <Home復帰>
		m_Cent3D = m_Cent3D_S[n];										// 3面図表示状態復帰
		m_Fovy   = m_Fovy_S[n];
	} else if(!(m_ScrEnv1&0x1)) {										// <新規>
		m_Cent3D.p[0]=SCT_X, m_Cent3D.p[1]=SCT_Y, m_Cent3D.p[2]=SCT_Z;	// 3面図表示状態初期化
		m_Fovy   = FOVY;													
	}
	if(hm||(m_ScrEnv2&0x2)) {											// <Home復帰>
		m_CentPs   = m_CentPs_S[n];										// 透視図表示状態復帰
		m_Eye      = m_Eye_S[n];
		m_EyeSv    = m_EyeSv_S[n];
		m_TruckLen = m_TruckLen_S[n];
		m_HzVec    = m_HzVec_S[n];
		m_VtVec    = m_VtVec_S[n];
	} else if(!(m_ScrEnv1&0x2)) {										// <新規>
		m_CentPs.p[0]=SCT_X, m_CentPs.p[1]=SCT_Y, m_CentPs.p[2]=SCT_Z;	// 透視図表示状態初期化
		m_Eye.p[0] = EYE_X, m_Eye.p[1] = EYE_Y, m_Eye.p[2] = EYE_Z;
		m_EyeSv.p[0]=EYE_X, m_EyeSv.p[1]=EYE_Y, m_EyeSv.p[2]=EYE_Z;
		m_TruckLen = TRLEN;
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
	}
	if(hm||(m_ScrEnv2&0x4)) {											// <Home復帰>
		m_GridDisp  = m_GridDisp_S[n];									// ｸﾞﾘｯﾄﾞ状態復帰
		m_GridBind  = m_GridBind_S[n];
		m_GridSpace = m_GridSpace_S[n];
		m_GridIntvl = m_GridIntvl_S[n];
	} else if(!(m_ScrEnv1&0x4)) {										// <新規>
		m_GridDisp  = 0;												// ｸﾞﾘｯﾄﾞ状態初期化
		m_GridBind  = 0;
		m_GridSpace = GRSPC;
		m_GridIntvl = GRINT;
	}
	if(hm||(m_ScrEnv2&0x10)) {											// <Home復帰>
		m_BpDisp = m_BpDisp_S[n];										// 基準点状態復帰
		m_BpBind = m_BpBind_S[n];
		m_Bpoint = m_Bpoint_S[n];
		m_BlDisp = m_BlDisp_S[n];										// 基準線状態復帰
		m_BlBind = m_BlBind_S[n];
		m_Blspnt = m_Blspnt_S[n];
		m_Blepnt = m_Blepnt_S[n];
	} else if(!(m_ScrEnv1&0x10)) {										// <新規>
		m_BpDisp = 0;													// 基準点状態初期化
		m_BpBind = 0;
		m_BlDisp = 0;													// 基準線状態初期化
		m_BlBind = 0;
	}
	ScrToPnt(0, &m_Cent3D, &p1);
	m_pState->SetCent3D(p1);											// 画面状態表示変更
	ScrToPnt(0, &m_Eye, &p1);
	m_pState->SetEye(p1);
	ScrToPnt(0, &m_CentPs, &p1);
	m_pState->SetCentPRS(p1);
	SetPerspective();													// 透視法射影行列の再構築
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列の再構築
	SetXYZGuide();														// XYZｶﾞｲﾄﾞ設定
	SetGrid();															// ｸﾞﾘｯﾄﾞ設定
	SetBase();															// 基準点/線設定
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	if(hm||(m_ScrEnv2&0x8)) {											// <Home復帰>
		m_GlbDisp   = m_GlbDisp_S[n];									// GLB表示状態復帰
		m_GlbChange = m_GlbChange_S[n];
	} else if(!(m_ScrEnv1&0x8)) {										// <新規>
		m_GlbDisp   = 0;												// GLB表示状態初期化
		m_GlbChange = 0;
	}
	if(ReadGLB()!=TRUE) {												// GLB入力
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙ読込NG｣
		DeleteGLB();													// GLB破棄
		m_GlbDisp = 0;
	}
	if(hm||(m_ScrEnv2&0x8)) {											// <Home復帰>
		m_DspTmpNo = m_DspTmpNo_S[n];									// ﾃﾝﾌﾟﾚｰﾄ表示状態復帰
		for(i=0; i<8; i++) {
			for(j=0; j<3; j++) {
				m_TempPath[i][j] = m_TempPath_S[n][i][j];
			}
		}
	} else if(!(m_ScrEnv1&0x8)) {										// <新規>
		m_DspTmpNo = 0;													// ﾃﾝﾌﾟﾚｰﾄ表示状態初期化
	}
	if(ReadTEMP()!=TRUE) {												// ﾃﾝﾌﾟﾚｰﾄ入力
		SetSplashMsg(IDS_CANT_READ_FILE);								// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		View->DeleteTEMP();												// ﾃﾝﾌﾟﾚｰﾄ破棄
		m_DspTmpNo = 0;
	}
	Display(0);															// 再描画
}

/********************************************/
void CJcad3GlbView::GetCentPnt(PNTTYPE* pnt)							//<<< 画面ｾﾝﾀｰ取得
/********************************************/
{
	ScrToPnt(0, &m_Cent3D, pnt);										// SCRTYPE => PNTTYPE
}

/*******************************************/
void CJcad3GlbView::GetEyePnt(PNTTYPE* pnt)								//<<< 視点取得
/*******************************************/
{
	ScrToPnt(0, &m_Eye, pnt);                                           // SCRTYPE => PNTTYPE
}

/*********************************************/
void CJcad3GlbView::GetEyeSvPnt(PNTTYPE* pnt)							//<<< 視点SV取得
/*********************************************/
{
	ScrToPnt(0, &m_EyeSv, pnt);                                         // SCRTYPE => PNTTYPE
}

/**********************************************/
void CJcad3GlbView::GetCentPsPnt(PNTTYPE* pnt)							//<<< 参照点取得
/**********************************************/
{
	ScrToPnt(0, &m_CentPs, pnt);                                        // SCRTYPE => PNTTYPE
}
