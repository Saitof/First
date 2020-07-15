/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewEdit1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputBtnDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "EditObject.h"
#include "NumFunc.h"

/*************************************************************/
void CJcad3GlbView::MouseSpace(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ｽﾍﾟｰｽ[頂点]
/*************************************************************/
{
	PNTTYPE pnt, pt1, pt2;
	BOXTYPE box;
	int     hit, i;
	double  th, l11, l12, l21, l22;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1&&plane==Si5&&(flg&MK_LBUTTON)) {					// ﾏｳｽ左釦押下中のみ有効
			if(Si8==0) {												// <移動処理>
				CalcScreenWorld(0, &pnt, point, plane);					// 座標点取得
				for(i=0; i<DIM; i++) {
					Sdb[i] = pnt.p[i]-Spnt1.p[i];						// 立体移動量算出
				}
				if(flg&m_LimKey) {
					LimitValue(plane, Sdb);								// ﾏｳｽ入力制限値
				}
				Sdb[Si7] = 0.0;											// 指定平面外の座標は0
			} else if(Si8==1) {											// <回転処理>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				l11 = Spnt1.p[Si5]-Sbase.p[Si5];						// 角度算出
				l12 = Spnt1.p[Si6]-Sbase.p[Si6];
				l21 = pnt.p[Si5]-Sbase.p[Si5];
				l22 = pnt.p[Si6]-Sbase.p[Si6];
				l11 = atan2(l12, l11);
				l11 = (l11<-10||l11>10) ? 0.0 : l11;
				l11 = l11*180.0/PI;
				l21 = atan2(l22, l21);
				l21 = (l21<-10||l21>10) ? 0.0 : l21;
				l21 = l21*180.0/PI;
				th = -l21+l11;
				Sd2 = (th-Sd1<-180.0) ? Sd2+360 :
                      ((th-Sd1>180.0) ? Sd2-360 : Sd2);					// 連続回転補正値修正
				Sd1 = th;												// 今回回転角度保存
				th += Sd2;												// 補正値加算
				Sdb[Si7+3] = th;
			} else {													// <ｻｲｽﾞ変更処理>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				pt1 = Spnt1, pt1.p[Si7] = 0.0;							// 指定平面以外の座標は0
				pt2 = Sbase, pt2.p[Si7] = 0.0;
				pnt.p[Si7] = 0.0;
				l11 = PntD(&pt1, &pt2);									// 中心点とﾄﾗｯｶｰ位置までの距離
				l12 = PntD(&pnt, &pt2);									// 中心点とｶｰｿﾙ位置までの距離
				l21 = (l12/l11) - 1.0;									// 増減分の倍率
				Sdb[6] = l21, Sdb[7] = l21, Sdb[8] = l21;
			}
			ObjectSpace(Si1, Si4, Si3, Si2, 0, Sda[0]+Sdb[0],
                        Sda[1]+Sdb[1], Sda[2]+Sdb[2], Sda[3]+Sdb[3],
                        Sda[4]+Sdb[4], Sda[5]+Sdb[5], Sda[6]+Sdb[6],
                        Sda[7]+Sdb[7], Sda[8]+Sdb[8]);					// ｽﾍﾟｰｽ
			Si1 = 1;
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 8))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si5 = plane;											// 平面保存
				Si6 = (Si5+1>2) ? 0 : Si5+1;							// 座標軸設定
				Si7 = (Si6+1>2) ? 0 : Si6+1;
				Si8 = -1;												// 処理未定
				if(hit==0) {											// <移動>
					Si8 = 0;											// 移動処理
					Spnt1 = Sbase;
				} else if(hit==7) {										// <回転>
					Si8 = 1;											// 回転処理
					Spnt1 = Spa[7];
				} else {												// <ｻｲｽﾞ変更>
					i = (hit-1)/2;										// 軸No取得
					if(i!=Si7) {										// <有効なﾄﾗｯｶｰ>
						Si8 = 2;										// ｻｲｽﾞ変更処理
						Spnt1 = Spa[hit];
					}
				}
				if(Si8!=-1) {											// <ﾄﾗｯｶｰ選択OK>
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Display(0);											// 再描画
					Sphase = 1, Sd1 = 0.0, Sd2 = 0.0;
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		for(i=0; i<9; i++) {
			Sda[i] += Sdb[i], Sdb[i] = 0.0;								// 加減算値を加算
		}
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &Sbase);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		TrackerBoxCrs2(box, Sbase, Spa);								// ﾄﾗｯｶｰ取得
		Spb[0]=Spa[0], Spb[1]=Spa[1], Spb[2]=Spa[2], Spb[3]=Spa[3];
		Spb[4]=Spa[4], Spb[5]=Spa[7];
		Tracker1.Setup(Spb, Sia, Tens, 6, 0, 7);						// ﾄﾗｯｶｰ1設定
		Spb[0]=Spa[0], Spb[1]=Spa[3], Spb[2]=Spa[4], Spb[3]=Spa[5];
		Spb[4]=Spa[6], Spb[5]=Spa[7];
		Tracker2.Setup(Spb, Sia, Tens, 6, 1, 7);						// ﾄﾗｯｶｰ2設定
		Spb[0]=Spa[0], Spb[1]=Spa[5], Spb[2]=Spa[6], Spb[3]=Spa[1];
		Spb[4]=Spa[2], Spb[5]=Spa[7];
		Tracker3.Setup(Spb, Sia, Tens, 6, 2, 7);						// ﾄﾗｯｶｰ3設定
		Sphase = 0;
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si1 = 0;													// 初期ﾓｰﾄﾞ
			Si2 = (m_FChar&0x1) ? 0 : 1;								// [F5]:基準点-Z最小指定
			Si2 = (m_FChar&0x2) ? 2 : Si2;								// [F6]:基準点-Z最大指定
			Si3 = (m_FChar&0x4) ? 0 : 2;								// [F7]:基準点-立体中心指定
			Si3 = (m_FChar&0x8) ? 1 : Si3;								// [F8]:基準点-ﾕﾆｯﾄ中心指定
			Si4 = (m_FChar&0x4) ? 0 : 2;								// [F7]:中心点-立体中心指定
			Si4 = (m_FChar&0x8) ? 1 : Si4;								// [F8]:中心点-ﾕﾆｯﾄ中心指定
		}
		for(i=0; i<9; i++) {
			Sda[i] = 0.0, Sdb[i] = 0.0;
		}
		Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;						// 倍率のみ初期値は1.0
		if(m_NumInp!=0) {												// <数値入力の場合>
			CInputBtnDlg7 dlg(1);
			dlg.SetParam(Sda[0], Sda[1], Sda[2], Sda[3], Sda[4],
                         Sda[5], Sda[6], m_EarthAdd1, m_EarthAdd2,
                         m_EarthAdd3, m_ScaleDir, m_ScalePnt, Si3, Si4);// ﾊﾟﾗﾒｰﾀ設定
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2, &m_EarthAdd3,
                         &m_ScaleDir, &m_ScalePnt);						// ﾊﾟﾗﾒｰﾀ取得
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else {														// <ﾏｳｽ入力の場合>
			OffUndoRedo();												// Undo/Redo不可
			Sia[0]=0, Sia[1]=1, Sia[2]=1, Sia[3]=1, Sia[4]=1, Sia[5]=2;	// ﾄﾗｯｶｰ色設定
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &Sbase);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBoxCrs2(box, Sbase, Spa);							// ﾄﾗｯｶｰ取得
			Tens[0]=7, Tens[1]=5, Tens[2]=5, Tens[3]=6, Tens[4]=6, Tens[5]=8;
			Spb[0]=Spa[0], Spb[1]=Spa[1], Spb[2]=Spa[2], Spb[3]=Spa[3];
			Spb[4]=Spa[4], Spb[5]=Spa[7];
			Tracker1.Setup(Spb, Sia, Tens, 6, 0, 7);					// ﾄﾗｯｶｰ1設定
			Spb[0]=Spa[0], Spb[1]=Spa[3], Spb[2]=Spa[4], Spb[3]=Spa[5];
			Spb[4]=Spa[6], Spb[5]=Spa[7];
			Tracker2.Setup(Spb, Sia, Tens, 6, 1, 7);					// ﾄﾗｯｶｰ2設定
			Spb[0]=Spa[0], Spb[1]=Spa[5], Spb[2]=Spa[6], Spb[3]=Spa[1];
			Spb[4]=Spa[2], Spb[5]=Spa[7];
			Tracker3.Setup(Spb, Sia, Tens, 6, 2, 7);					// ﾄﾗｯｶｰ3設定
		}
		Display(0);														// 再描画
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/************************************************************/
void CJcad3GlbView::MouseMove(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< 移動[頂点]
/************************************************************/
{
	PNTTYPE pnt;
	int     i;
	double  d[3], dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			for(i=0; i<DIM; i++) {
				d[i] = pnt.p[i]-Sbase.p[i];								// 立体移動量算出
			}
			if(flg&m_LimKey) {
				LimitValue(plane, d);									// ﾏｳｽ入力制限値
			}
			ObjectMove(Sphase, m_CopyNum, Spnt1, Sscr1,
                       d[0], d[1], d[2]);								// 立体平行移動
			Sphase = (Sphase==0) ? 1 : Sphase;
			dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
			m_pNumDisp->SetParam(d[0], d[1], d[2], dd);					// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Sd1 = d[0], Sd2 = d[1], Sd3 = d[2];							// 移動量保存
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(0, &Sbase, point, plane);						// 移動元点の座標取得
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <2回目以降>
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, Sd1, Sd2, Sd3);		// 平行移動終了処理
			Display(0);													// 再描画
			m_CopyNum = 0, Sphase = 0;									// 複写数/ﾌｪｰｽﾞｸﾘｱ
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			if(m_CopyInp==2) {											// <<複写ﾓｰﾄﾞ:ON(ﾀﾞｲｱﾛｸﾞ表示)>>
				m_CopyNum  = m_ObjCopyNum;								// 既存値設定
				Spnt1.p[0] = m_ObjXScale;
				Spnt1.p[1] = m_ObjYScale;
				Spnt1.p[2] = m_ObjZScale;
				Si1        = m_ObjSynFlg;
				Sscr1.p[0] = m_ObjXPoint;
				Sscr1.p[1] = m_ObjYPoint;
				Sscr1.p[2] = m_ObjZPoint;
				if(InputCopyNum(&m_CopyNum, &Spnt1, &Si1, &Sscr1)) {	// 複写情報入力
					m_ObjCopyNum = m_CopyNum;							// 既存値保存
					m_ObjXScale  = Spnt1.p[0];
					m_ObjYScale  = Spnt1.p[1];
					m_ObjZScale  = Spnt1.p[2];
					m_ObjSynFlg  = Si1;
					m_ObjXPoint  = Sscr1.p[0];
					m_ObjYPoint  = Sscr1.p[1];
					m_ObjZPoint  = Sscr1.p[2];
				} else {
					m_ObjCopyNum = 0;
				}
				if(m_CopyNum==0) {
					ExitMenu(1, 1);										// 複写数無はﾒﾆｭｰ終了
				}
			} else if(m_CopyInp==1) {									//<<複写ﾓｰﾄﾞ:ON(ﾀﾞｲｱﾛｸﾞなし)>>
				m_CopyNum = 1;											// 複写数:1
				Si1 = 0;												// 位置モード:モード1
				for(i=0; i<DIM; i++) {
					Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;					// 倍率:1.0倍/基準点:中間
				}
			}
			if(m_NumInp!=0) {											// <数値入力の場合>
				CInputBtnDlg1 dlg(1);
				dlg.SetParam1(m_CopyNum, Spnt1, Si1, Sscr1, m_MoveAdd);	// ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_MoveAdd);								// ﾊﾟﾗﾒｰﾀ取得
				m_CopyNum = 0;											// 複写数ｸﾘｱ
				ExitMenu(1, 1);											// ﾒﾆｭｰ終了
				EndJMenu1();											// ﾒﾆｭｰ終了処理
			}
		}
		Display(0);														// 再描画
		SetNumDisp(2);													// 数値表示初期処理
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		ExitMenu(1, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/**************************************************************/
void CJcad3GlbView::MouseRotate(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< 回転[頂点]
/**************************************************************/
{
	int    i, col=0;
	double th;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&!m_NumInp) {									// ﾏｳｽ左釦押下中のみ有効
			if(Sphase==1) {
				if(abs(Si2-point.x)>DRAGMIN||abs(Si3-point.y)>DRAGMIN) {// 角度算出基準点角度
					th = atan2((double)(point.y-Si3),
                               (double)(point.x-Si2));
					th = (th<-10||th>10) ? 0.0 : th;
					Sd1 = th*180.0/PI;
					Sphase = 2;
				}
			} else if(Sphase>=2) {										// 現在ｶｰｿﾙ位置角度
				th = atan2((double)(point.y-Si3),
                           (double)(point.x-Si2));
				th = (th<-10||th>10) ? 0.0 : th;
				th = th*180.0/PI;
				Sd2 = th - Sd1;											// 回転角度
				Sd2 = (Sd2>180.0)?-(360.0-Sd2):((Sd2<-180.0)?360.0+Sd2:Sd2);
				ObjectRotate(Sphase-2, m_CopyNum, Spnt1,
                             Sscr1, Sbase, plane, Sd2, 0.0);			// 立体回転移動
				m_pNumDisp->SetParam(Sd2);								// 回転角度表示
				Display(0);												// 再描画
				Sphase = 3;
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {
			CalcScreenWorld(0, &Sbase, BasePoint, plane);				// 回転中心点座標取得
			CalcWorldScreen(&BasePoint, Sbase, plane);
			Si2 = BasePoint.x, Si3 = BasePoint.y;						// 中心点ｽｸﾘｰﾝ座標保存
			Tracker1.Setup(&Sbase, &col, NULL, 1, plane, 2);			// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
			Sphase = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(m_NumInp!=0) {												// <数値入力の場合>
			CInputBtnDlg2 dlg;
			dlg.SetParam(m_CopyNum, Spnt1, Si1, Sscr1,  Sbase,
                         plane, m_RotateAdd1, m_RotateAdd2);			// ﾊﾟﾗﾒｰﾀ設定
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetParam(&m_RotateAdd1, &m_RotateAdd2);                 // ﾊﾟﾗﾒｰﾀ取得
			Tracker1.Reset(0);											// ﾄﾗｯｶｰ1解除
			m_CopyNum = 0;												// 複写数ｸﾘｱ
			ExitMenu(1, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(Sphase==3) {											// <ﾏｳｽ入力の2回目以降>
			EndRotate(m_CopyNum, Spnt1, Si1, Sscr1, Sbase, plane,
                      Sd2, 0.0);										// 回転移動終了処理
			m_CopyNum = 0;												// 複写数ｸﾘｱ
		}
		Display(0);														// 再描画
		Sphase = (m_ActMenu!=NONE) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL>>>
		Tracker1.Reset(0);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			if(m_CopyInp==2) {											// <<複写ﾓｰﾄﾞ:ON(ﾀﾞｲｱﾛｸﾞ表示)>>
				m_CopyNum  = m_ObjCopyNum;								// 既存値設定
				Spnt1.p[0] = m_ObjXScale;
				Spnt1.p[1] = m_ObjYScale;
				Spnt1.p[2] = m_ObjZScale;
				Si1        = m_ObjSynFlg;
				Sscr1.p[0] = m_ObjXPoint;
				Sscr1.p[1] = m_ObjYPoint;
				Sscr1.p[2] = m_ObjZPoint;
				if(InputCopyNum(&m_CopyNum, &Spnt1, &Si1, &Sscr1)) {	// 複写情報入力
					m_ObjCopyNum = m_CopyNum;							// 既存値保存
					m_ObjXScale  = Spnt1.p[0];
					m_ObjYScale  = Spnt1.p[1];
					m_ObjZScale  = Spnt1.p[2];
					m_ObjSynFlg  = Si1;
					m_ObjXPoint  = Sscr1.p[0];
					m_ObjYPoint  = Sscr1.p[1];
					m_ObjZPoint  = Sscr1.p[2];
				} else {
					m_ObjCopyNum = 0;
				}
				if(m_CopyNum==0) {
					ExitMenu(1, 1);										// 複写数無はﾒﾆｭｰ終了
				}
			} else if(m_CopyInp==1) {									//<<複写ﾓｰﾄﾞ:ON(ﾀﾞｲｱﾛｸﾞなし)>>
				m_CopyNum = 1;											// 複写数:1
				Si1 = 0;												// 位置モード:モード1
				for(i=0; i<DIM; i++) {
					Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;					// 倍率:1.0倍/基準点:中間
				}
			}
		}
		SetNumDisp(2);													// 数値表示初期処理
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(1, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/**************************************************************/
void CJcad3GlbView::MouseMirror(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< 鏡像[頂点]
/**************************************************************/
{
	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(0, &Spb[0], point, plane);					// 鏡像軸(ﾗｲﾝ)終点
			if(fabs(Spa[0].p[0]-Spb[0].p[0])>EPSILON||
               fabs(Spa[0].p[1]-Spb[0].p[1])>EPSILON||
               fabs(Spa[0].p[2]-Spb[0].p[2])>EPSILON) {
				if(flg&m_LimKey) {
					LimitPoint(plane, &Spb[0], &Spa[0]);				// ﾏｳｽ入力制限座標
				}
				Line.Setup(Spa, Spb, Sia, 1, plane);					// 鏡像軸(ﾗｲﾝ)設定
				ObjectMirror(Sphase, m_CopyNum, Spa[0], Spb[0], plane);	// 立体鏡像移動
				Sphase = (Sphase==0) ? 1 : Sphase;
				Display(0);												// 再描画
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(0, &Spa[0], point, plane);						// 鏡像軸(ﾗｲﾝ)始点
	} else if(ms==3) {													// <<ﾏｳｽの左釦解放>>
		Line.Reset();													// ﾗｲﾝ解除
		Display(0);														// 再描画
		m_CopyNum = 0, Sphase = 0;										// 複写数/ﾌｪｰｽﾞｸﾘｱ
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			m_CopyNum = (m_CopyInp) ? 1 : 0;							// 複写ﾓｰﾄﾞ:ON
			Sia[0] = 0;													// ﾗｲﾝｶﾗｰNo登録
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		ExitMenu(1, 0);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*************************************************************/
void CJcad3GlbView::MouseScale(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ｻｲｽﾞ変更[頂点]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, i, esw=0, col=0, smd;
	double  dd[3], l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&!m_NumInp&&Sphase>1) {						// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			smd = (flg&m_LimKey) ? LimitPoint(plane, &pnt, &Spnt1) : 0;	// ﾏｳｽ入力制限座標
			smd = (flg&m_NLiKey) ? 2 : smd;								// ﾓｰﾄﾞ:3D均等
			if(Si7==-1) {
				esw = (pnt.p[Si3]>=Sbase.p[Si3]) ? 1 : esw;
			} else if(Si7==1) {
				esw = (pnt.p[Si3]<=Sbase.p[Si3]) ? 1 : esw;
			} else {
				pnt.p[Si3] = Sbase.p[Si3]+1.0;
				Spnt1.p[Si3] = Sbase.p[Si3]+1.0;
			}
			if(Si8==-1) {
				esw = (pnt.p[Si4]>=Sbase.p[Si4]) ? 1 : esw;
			} else if(Si8==1) {
				esw = (pnt.p[Si4]<=Sbase.p[Si4]) ? 1 : esw;
			} else {
				pnt.p[Si4] = Sbase.p[Si4]+1.0;
				Spnt1.p[Si4] = Sbase.p[Si4]+1.0;
			}
			if(esw==0) {												// <正常な場合>
				l1 = Spnt1.p[Si3]-Sbase.p[Si3];							// 倍率計算
				l2 = pnt.p[Si3]-Sbase.p[Si3];
				dd[Si3] = l2/l1;
				l1 = Spnt1.p[Si4]-Sbase.p[Si4];
				l2 = pnt.p[Si4]-Sbase.p[Si4];
				dd[Si4] = l2/l1;
				dd[Si5] = 1.0;
				if(smd==1) {											// <ﾓｰﾄﾞ:2D均等>
					if(dd[Si3]>dd[Si4]) {								// 大倍率採用
						dd[Si4] = dd[Si3];
					} else {
						dd[Si3] = dd[Si4];
					}
				} else if(smd==2) {										// <ﾓｰﾄﾞ:3D均等>
					if(dd[Si3]>dd[Si4]) {								// 大倍率採用
						dd[Si4] = dd[Si3], dd[Si5] = dd[Si3];
					} else {
						dd[Si3] = dd[Si4], dd[Si5] = dd[Si4];
					}
				}
				ObjectScale(Sphase-2, Sbase, dd[0], dd[1], dd[2]);		// 立体拡大/縮小
				Sphase = (Sphase==2) ? 3 : Sphase;

				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                     box.bp[1].p[1]-box.bp[0].p[1],
                                     box.bp[1].p[2]-box.bp[0].p[2]);
				Display(0);												// 再描画
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 13))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si2 = plane;											// 平面保存
				Si3 = Si2;												// 座標軸設定
				Si4 = (Si3+1>2) ? 0 : Si3+1;
				Si5 = (Si4+1>2) ? 0 : Si4+1;
				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				BoxCenter(box, &cent);									// ﾎﾞｯｸｽｾﾝﾀｰ取得
				Sbase = Spa[hit];										// ﾘｻｲｽﾞ基準点設定
				Sbase.p[Si5] = cent.p[Si5];
				if(m_NumInp==0) {										// <ﾏｳｽ入力の場合>
					TrackerScale2(Si2, box, Sbase, Spa, Tens, &Si6);	// ﾄﾗｯｶｰ取得2
					Tracker1.Setup(Spa, Sib, Tens, Si6, Si2, 1);		// ﾄﾗｯｶｰ1設定
				} else {												// <数値入力の場合>
					Tracker1.Setup(&Sbase, &col, NULL, 1, Si2, 2);		// ﾄﾗｯｶｰ1設定
				}
				Display(0);												// 再描画
				Sphase = 1;
			}
		} else if(Sphase==1&&plane==Si2) {								// <更新点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, Si2, pnt, Spa, Si6))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Spnt1 = Spa[hit];										// ﾘｻｲｽﾞ更新点設定
				Si7 = (fabs(Spnt1.p[Si3]-Sbase.p[Si3])<EPSILON) ? 0		// 基準点&更新点位置関係
                                  : ((Spnt1.p[Si3]<Sbase.p[Si3])?-1:1);
				Si8 = (fabs(Spnt1.p[Si4]-Sbase.p[Si4])<EPSILON) ? 0
                                  : ((Spnt1.p[Si4]<Sbase.p[Si4])?-1:1);
				Tracker1.Setup(&Sbase, &col, NULL, 1, Si2, 2);			// ﾄﾗｯｶｰ1設定
				Display(0);												// 再描画
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(m_NumInp!=0&&Sphase==1) {									// <数値入力の場合>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			for(i=0; i<DIM; i++) {
				dd[i] = box.bp[1].p[i]-box.bp[0].p[i];
			}
			CInputBtnDlg1 dlg(Si1);
			if(Si1==2) {												// <ｻｲｽﾞ指定>
				dlg.SetParam2(Sbase, dd[0], dd[1], dd[2], m_ScaleAdd1);	// ﾊﾟﾗﾒｰﾀ設定
			} else {													// <倍率指定>
				dlg.SetParam2(Sbase, dd[0], dd[1], dd[2], m_ScaleAdd2);	// ﾊﾟﾗﾒｰﾀ設定
			}
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			if(Si1==2) {												// <ｻｲｽﾞ指定>
				dlg.GetParam(&m_ScaleAdd1);								// ﾊﾟﾗﾒｰﾀ取得
			} else {													// <倍率指定>
				dlg.GetParam(&m_ScaleAdd2);								// ﾊﾟﾗﾒｰﾀ取得
			}
			Tracker1.Reset(0);											// ﾄﾗｯｶｰ1解除
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(m_NumInp==0&&(Sphase==2||Sphase==3)) {				// <ﾏｳｽ入力の場合>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &cent);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerScale1(box, cent, Spa);								// ﾄﾗｯｶｰ取得
			Tracker1.Setup(Spa, Sia, NULL, 13, 5, 7);					// ﾄﾗｯｶｰ1設定
			Sphase = 0;
		}
		Display(0);														// 再描画
	} else if(ms==0||ms==11) {											// <<<初期処理/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si1 = (m_FChar&0x1) ? 3 : 2;								// [F5]:倍率指定
		}
		for(i=0; i<13; i++) {
			Sia[i] = 0;													// ﾄﾗｯｶｰ色設定
		}
		Sib[0] = 0, Sib[1] = 2, Sib[2] = 2, Sib[3] = 2, Sib[4] = 2;
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &cent);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		TrackerScale1(box, cent, Spa);									// ﾄﾗｯｶｰ取得1
		Tracker1.Setup(Spa, Sia, NULL, 13, 5, 7);						// ﾄﾗｯｶｰ1設定
		Display(0);														// 再描画
		SetNumDisp(2);													// 数値表示初期処理
		m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                             box.bp[1].p[1]-box.bp[0].p[1],
                             box.bp[1].p[2]-box.bp[0].p[2]);
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/***************************************************************/
void CJcad3GlbView::MouseResize3(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 直線指定ﾘｻｲｽﾞ2[頂点]
/***************************************************************/
{
	PNTTYPE pnt, p, p1, p2, p3;
	BOXTYPE box;
	int     hit, i, i0, i1, i2, i3, i4, ok=1, esw=0, col=0;
	double  d1, d2, d3, d4, d5;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&plane==Si1&&(Sphase==1||Sphase==3)) {		// <<範囲指定>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(Sphase==1) {												// 初回のみ有効移動確認
				ok = (abs(BasePoint.x-point.x)<=DRAGMIN&&
                      abs(BasePoint.y-point.y)<=DRAGMIN) ? 0 : ok;
			}
			if(ok==1) {
				if(Sphase==1) {
					d4 = pnt.p[Si1]-Sbase.p[Si1];
					d5 = pnt.p[Si2]-Sbase.p[Si2];						// 初回のみ移動方向確認
					Si5 = (fabs(d4)>fabs(d5)) ? Si1 : Si2;				// Si1/Si2方向移動有効
				}
				esw = (pnt.p[Si5]<Spa[0].p[Si5]) ? 1 : esw;
				esw = (pnt.p[Si5]>Spa[2].p[Si5]) ? 1 : esw;
				if(Sphase==3) {											// 2回目以降移動済点確認
					if(Si4==0||(Si5==Si1&&Si4==3)||(Si5==Si2&&Si4==1)) {
						esw = (pnt.p[Si5]>Spc[2].p[Si5]) ? 1 : esw;
					} else {
						esw = (pnt.p[Si5]<Spc[0].p[Si5]) ? 1 : esw;
					}
				}
				if(esw==0) {											// <<有効移動範囲>>
					if(Sphase==1) {
						for(i=0; i<4; i++) {
							Spc[i] = Spa[i];
						}
					}
					if(Si5==Si1) {										// <Si1方向移動有効>
						if(Si4==0||Si4==3) {
							i0 = 0, i1 = 0, i2 = 3, i3 = 1, i4 = 2;
							Tens[i3] = 2, Tens[i4] = 2;
						} else {
							i0 = 1, i1 = 1, i2 = 2, i3 = 0, i4 = 3;
							Tens[i3] = 1, Tens[i4] = 1;
						}
						Tens[i1] = 5, Tens[i2] = 5;
						TensC[0] = 6, TensC[1] = 6, TensC[2] = 6;
					} else {											// <Si2方向移動有効>
						if(Si4==0||Si4==1) {
							i0 = 0, i1 = 0, i2 = 1, i3 = 3, i4 = 2;
							Tens[i3] = 4, Tens[i4] = 4;
						} else {
							i0 = 1, i1 = 3, i2 = 2, i3 = 0, i4 = 1;
							Tens[i3] = 3, Tens[i4] = 3;
						}
						Tens[i1] = 6, Tens[i2] = 6;
						TensC[0] = 5, TensC[1] = 5, TensC[2] = 5;
					}
					Spc[i1].p[Si5] = pnt.p[Si5];
					Spc[i2].p[Si5] = pnt.p[Si5];
					if(Si6&&Si7) {
						for(i=0; i<DIM; i++) {							// ﾄﾗｯｶｰ2設定
							Spb[0].p[i] = (Spc[i1].p[i]+Spc[i3].p[i])/2.0;
							Spb[1].p[i] = (Spc[i2].p[i]+Spc[i4].p[i])/2.0;
						}
						Sib[2] = 2;
						Tens[i3] = Tens[i1], Tens[i4] = Tens[i1];
					} else {
						Spb[0] = Spa[i1], Spb[1] = Spa[i2];
						Sib[2] = (Si9) ? 2 : 0;
						TensC[0] = 7, TensC[1] = 7, TensC[2] = 7;
					}
					for(i=0; i<DIM; i++) {
						Spb[2].p[i] = (Spb[0].p[i]+Spb[1].p[i])/2.0;
					}
					if(i0==0) {
						Si6 = 1;										// <下限設定>
						Si12 = (Si12<0) ? Si11++ : Si12;				// 初期はﾗｲﾝNo取得
						Sps[Si12] = Spc[i1], Spt[Si12] = Spc[i2];		// 下限線情報設定
						Sis[Si12] = 0;
					} else {
						Si7 = 1;										// <上限設定>
						Si13 = (Si13<0) ? Si11++ : Si13;				// 初期はﾗｲﾝNo取得
						Sps[Si13] = Spc[i1], Spt[Si13] = Spc[i2];		// 上限線情報設定
						Sis[Si13] = 0;
					}
					if(Si9) {
						if(Si6&&Si7) {
							p1 = Spc[0], p2 = Spc[2];
						} else if(Si6) {
							p1 = Spa[0], p2 = Spc[0];
						} else {
							p1 = Spc[2], p2 = Spa[2];
						}
						if(Si5==Si1) {
							p1.p[Si2] = Spb[2].p[Si2];
							p2.p[Si2] = Spb[2].p[Si2];
						} else {
							p1.p[Si1] = Spb[2].p[Si1];
							p2.p[Si1] = Spb[2].p[Si1];
						}
						Si14 = (Si14<0) ? Si11++ : Si14;				// 初期はﾗｲﾝNo取得
						Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;	// 基準線情報設定
					}
					Sphase = (Sphase==1) ? 3 : Sphase;
					Tracker1.Setup(Spc, Sia, Tens, 4, Si1, 7);			// ﾄﾗｯｶｰ1設定
					Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);			// ﾄﾗｯｶｰ2設定
					Line.Setup(Sps, Spt, Sis, Si11, Si1);				// ﾗｲﾝ表示
				}
			}
			Display(0);													// 再描画
		} else if(flg&MK_LBUTTON&&plane==Si1&&(Sphase==4||Sphase==5)){	// <ｻｲｽﾞ指定>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(!Si10) {													// 初回のみ有効移動確認
				ok = (abs(BasePoint.x-point.x)<=DRAGMIN&&
                      abs(BasePoint.y-point.y)<=DRAGMIN) ? 0 : ok;
			}
			if(ok==1) {
				if(!Si10) {
					d4 = pnt.p[Si1]-Sbase.p[Si1];
					d5 = pnt.p[Si2]-Sbase.p[Si2];						// 初回のみ移動方向確認
					Si8 = (fabs(d4)>fabs(d5)) ? Si1 : Si2;				// Si1/Si2方向移動有効
				}
				if(Si6&&Si7) {
					if(Si8==Si5) {										// 有効範囲確認
						esw = 1;
					} else {
						if(Si4==0) {
							esw = (pnt.p[Si8]>Spc[2].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]>Spb[2].p[Si8]) ? 1 : esw;
						} else if(Si4==1) {
							esw = (pnt.p[Si8]<Spc[0].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]<Spb[2].p[Si8]) ? 1 : esw;
						} else {
							if(pnt.p[Si8]<Spc[0].p[Si8]||pnt.p[Si8]>Spc[2].p[Si8]) {
								esw = 1;
							} else {
								Si9 = 1;
							}
						}
					}
				} else {
					if(Si8==Si5) {
						if(Si6) {
							esw = (pnt.p[Si8]>Spc[0].p[Si8]) ? 1 : esw;
						} else {
							esw = (pnt.p[Si8]<Spc[2].p[Si8]) ? 1 : esw;
						}
						esw = (Si9&&Si4==2) ? 1 : esw;
					} else {
						if(Si4==0) {
							esw = (pnt.p[Si8]>Spc[2].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]>Spb[2].p[Si8]) ? 1 : esw;
						} else if(Si4==1) {
							esw = (pnt.p[Si8]<Spc[0].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]<Spb[2].p[Si8]) ? 1 : esw;
						} else {
							if(pnt.p[Si8]<Spc[0].p[Si8]||pnt.p[Si8]>Spc[2].p[Si8]) {
								esw = 1;
							} else {
								Si9 = 1;
							}
						}
					}
				}
				if(esw==0&&Si4==2&&Si9) {
					Spb[2].p[Si8] = pnt.p[Si8];
					TensC[2] = (Si8==Si1) ? 5 : 6;						// 移動軸方向確認
					Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);			// ﾄﾗｯｶｰ2表示
					if(Si6&&Si7) {
						p1 = Spc[0], p2 = Spc[2];
					} else if(Si6) {
						p1 = Spa[0], p2 = Spc[0];
					} else {
						p1 = Spc[2], p2 = Spa[2];
					}
					if(Si5==Si1) {
						p1.p[Si2] = Spb[2].p[Si2];
						p2.p[Si2] = Spb[2].p[Si2];
					} else {
						p1.p[Si1] = Spb[2].p[Si1];
						p2.p[Si1] = Spb[2].p[Si1];
					}
					Si14 = (Si14<0) ? Si11++ : Si14;					// 初期はﾗｲﾝNo取得
					Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;		// 基準線情報設定
					Line.Setup(Sps, Spt, Sis, Si11, Si1);				// ﾗｲﾝ表示
					Si10 = 1;
				} else if(esw==0) {										// <有効移動範囲>
					Spnt1 = Sbase;
					Spnt1.p[Si8] = pnt.p[Si8];
					d1 = 1.0, d3 = 1.0, d2 = -0.001;					// 基準点等設定
					for(i=0; i<DIM; i++) {
						p1.p[i] = 0.0, p2.p[i] = 0.0, p3.p[i] = 0.0;
					}
					if(Si6&&Si7) {
						if(Si4==0) {
							i0 = 0, p3 = Spc[2];
							p1 = (Si5==Si1) ? Spc[3] : Spc[1];
						} else {
							i0 = 1, p1 = Spc[0];
							p3 = (Si5==Si1) ? Spc[1] : Spc[3];
						}
						if(Si9) {
							if(Si5==Si1) {
								p1.p[Si2] = Spb[2].p[Si2];
								p3.p[Si2] = Spb[2].p[Si2];
							} else {
								p1.p[Si1] = Spb[2].p[Si1];
								p3.p[Si1] = Spb[2].p[Si1];
							}
							i0 = (flg&m_NLiKey) ? 2 : i0;
						}
						for(i=0; i<DIM; i++) {
							p2.p[i] = (p1.p[i]+p3.p[i])/2.0;
						}
						d4 = Spnt1.p[Si8]-p1.p[Si8];
						d5 = Sbase.p[Si8]-p1.p[Si8];
						d2 = d4/d5;
					} else if(Si6) {
						if(Si8==Si5) {
							i0 = 0, p1 = Spc[0];
							if(Si5==Si1) {
								p3 = Spc[3];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si2] = Spb[2].p[Si2];
									} else {
										p = p1, p1.p[Si2] = Spb[2].p[Si2];
									}
								}
							} else {
								p3 = Spc[1];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si1] = Spb[2].p[Si1];
									} else {
										p = p1, p1.p[Si1] = Spb[2].p[Si1];
									}
								}
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							if(Si4==0) {
								d1 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p3, p3 = p, d2 = 1.0, d3 = d1;
								}
							} else if(Si4==1) {
								d3 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p1, p1 = p, d2 = 1.0, d1 = d3;
								}
							} else {
								d1 = d4/d5, d3 = d4/d5;
							}
						} else {
							if(Si4==0) {
								i0 = 0;
								if(Si5==Si1) {
									p1 = Spa[3], p3 = Spc[3];
								} else {
									p1 = Spa[1], p3 = Spc[1];
								}
							} else {
								i0 = 1, p1 = Spa[0], p3 = Spc[0];
							}
							if(Si9) {
								if(Si5==Si1) {
									p1.p[Si2] = Spb[2].p[Si2];
									p3.p[Si2] = Spb[2].p[Si2];
								} else {
									p1.p[Si1] = Spb[2].p[Si1];
									p3.p[Si1] = Spb[2].p[Si1];
								}
								i0 = (flg&m_NLiKey) ? 2 : i0;
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							d1 = d4/d5;
						}
					} else {
						if(Si8==Si5) {
							i0 = 1, p3 = Spc[2];
							if(Si5==Si1) {
								p1 = Spc[1];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si2] = Spb[2].p[Si2];
									} else {
										p = p1, p1.p[Si2] = Spb[2].p[Si2];
									}
								}
							} else {
								p1 = Spc[3];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si1] = Spb[2].p[Si1];
									} else {
										p = p1, p1.p[Si1] = Spb[2].p[Si1];
									}
								}
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							if(Si4==0) {
								d1 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p3, p3 = p, d2 = 1.0, d3 = d1;
								}
							} else if(Si4==1) {
								d3 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p1, p1 = p, d2 = 1.0, d1 = d3;
								}
							} else {
								d1 = d4/d5, d3 = d4/d5;
							}
						} else {
							if(Si4==0) {
								i0 = 0, p1 = Spc[2], p3 = Spa[2];
							} else if(Si5==Si1) {
								i0 = 1, p1 = Spc[1], p3 = Spa[1];
							} else {
								i0 = 1, p1 = Spc[3], p3 = Spa[3];
							}
							if(Si9) {
								if(Si5==Si1) {
									p1.p[Si2] = Spb[2].p[Si2];
									p3.p[Si2] = Spb[2].p[Si2];
								} else {
									p1.p[Si1] = Spb[2].p[Si1];
									p3.p[Si1] = Spb[2].p[Si1];
								}
								i0 = (flg&m_NLiKey) ? 2 : i0;
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							d3 = d4/d5;
						}
					}
					d4 = Spw[0].p[Si3];									// 垂直方向下限
					d5 = Spw[1].p[Si3];									// 垂直方向上限
					i1 = Si16+(2*Si17);									// 垂直方向限定状況
					ObjectResize3(Sphase-4, Si1, i0, d1, d2, d3,
                                  i1, d4, d5, p1, p2, p3);				// 直線指定ﾘｻｲｽﾞ2
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Sphase = (Sphase = 4) ? 5 : Sphase;
					Si10 = 1;
				}
			}
			Display(0);													// 再描画
		} else if(flg&MK_LBUTTON&&plane==Si2&&Sphase==6) {				// <<垂直範囲限定>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			d1 = pnt.p[Si3];											// 垂直方向座標
			if(Si4==0) {												// <下限>
				if(d1>=Spd[0].p[Si3]&&d1<Spw[1].p[Si3]) {				// 立体最小～
					Spw[0].p[Si3] = d1;									// 指定上限は更新
					Si16 = 1;											// 下限設定済み
				}
			} else {													// <上限>
				if(d1>Spw[0].p[Si3]&&d1<=Spd[1].p[Si3]) {				// 指定下限～
					Spw[1].p[Si3] = d1;									// 立体最大は更新
					Si17 = 1;											// 上限設定済み
				}
			}
			Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);					// ﾄﾗｯｶｰ3設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		Si10 = 0;
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		if(Sphase==0) {													// <範囲指定1回目>
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 4))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sbase = Spa[hit];										// 基準点保存
				Si1 = plane;											// 座標軸設定
				Si2 = (Si1+1>2) ? 0 : Si1+1;
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				BoxCenter(box, &pnt);									// BOXｾﾝﾀｰ取得
				Spd[0] = pnt, Spd[0].p[Si3] = box.bp[0].p[Si3];			// 垂直方向の両端
				Spd[1] = pnt, Spd[1].p[Si3] = box.bp[1].p[Si3];
				Spw[0] = Spd[0], Spw[1] = Spd[1];
				Si16 = 0, Si17 = 0;										// 垂直方向限定済みﾌﾗｸﾞ
				TrackerBox2D(Si1, box, Spa, Tens);						// ﾄﾗｯｶｰ取得
				for(Si4=0; Si4<4; Si4++) {
					if(fabs(Sbase.p[Si1]-Spa[Si4].p[Si1])<EPSILON&&
                       fabs(Sbase.p[Si2]-Spa[Si4].p[Si2])<EPSILON) {	// 押下点ﾄﾗｯｶｰ検索
						break;
					}
				}
				Tracker1.Setup(Spa, Sia, Tens, 4, Si1, 7);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				if(Si15) {												// <垂直限定有>
					Sic[0] = 6, Sic[1] = 6;
					Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);			// ﾄﾗｯｶｰ3設定
				} else {												// <垂直限定無>
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
				}
				Display(0);												// 再描画
				Sphase = 1, Si6 = 0, Si7 = 0;
			}
		} else if(Sphase==2) {											// <<範囲指定2回目以降>>
			if((Si4=Tracker1.HitTracker(1, Si1, pnt, Spc, 4))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sbase = Spc[Si4], Sphase = 3;							// 基準点保存
			} else if((Si4=
                       Tracker2.HitTracker(1, Si1, pnt, Spb, 3))!=-1) {	// ﾄﾗｯｶｰ2ﾁｪｯｸ
				Sbase = Spb[Si4], Sphase = 4;							// 基準点保存
			} else if(Si15) {											// <垂直範囲限定有>
				if((Si4=Tracker3.HitTracker(1, Si2, pnt, Spw, 2))!=-1) {// ﾄﾗｯｶｰ3ﾁｪｯｸ
					Sbase = Spw[Si4], Sphase = 6;						// 基準点保存
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==3||Sphase==4||Sphase==6) {
			if(Sphase==4&&Si4==2) {
				Si9 = 1, Sib[2] = 2;
				if(Si6&&Si7) {
					p1 = Spc[0], p2 = Spc[2];
				} else if(Si6) {
					p1 = Spa[0], p2 = Spc[0];
				} else {
					p1 = Spc[2], p2 = Spa[2];
				}
				if(Si5==Si1) {
					p1.p[Si2] = Spb[2].p[Si2];
					p2.p[Si2] = Spb[2].p[Si2];
				} else {
					p1.p[Si1] = Spb[2].p[Si1];
					p2.p[Si1] = Spb[2].p[Si1];
				}
				Si14 = (Si14<0) ? Si11++ : Si14;						// 初期はﾗｲﾝNo取得
				Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;			// 基準線情報設定
				Line.Setup(Sps, Spt, Sis, Si11, Si1);					// ﾗｲﾝ表示
			}
			Tracker1.Setup(Spc, Sia, Tens, 4, Si1, 7);					// ﾄﾗｯｶｰ1設定
			Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);					// ﾄﾗｯｶｰ2設定
			if(Si15) {													// <垂直限定有>
				Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);				// ﾄﾗｯｶｰ3設定
			} else {													// <垂直限定無>
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
			}
			Display(0);													// 再描画
			Sphase = 2;
		} else if(Sphase==1||Sphase==5) {
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			TrackerBox3D(box, Spa);										// ﾄﾗｯｶｰ取得
			Tens[0] = 9, Tens[1] = 10, Tens[2] = 11, Tens[3] = 12;		// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// ﾄﾗｯｶｰ1設定
			Tens[0] = 9, Tens[1] = 12, Tens[2] = 10, Tens[3] = 11;		// ﾄﾗｯｶｰ2ｵﾌﾟｼｮﾝ設定
			Tracker2.Setup(Spa, Sia, Tens, 4, 1, 7);					// ﾄﾗｯｶｰ2設定
			Tens[0] = 9, Tens[1] = 11, Tens[2] = 12, Tens[3] = 10;		// ﾄﾗｯｶｰ3ｵﾌﾟｼｮﾝ設定
			Tracker3.Setup(Spa, Sia, Tens, 4, 2, 7);					// ﾄﾗｯｶｰ3設定
			Line.Reset();												// ﾗｲﾝ解除
			Si11 = 0, Si12 = -1, Si13 = -1, Si14 = -1;					// ﾗｲﾝ情報初期化
			Display(0);													// 再描画
			Sphase = 0, Si9 = 0;
		}
	} else if(ms==0||ms==11) {											// <<<初期処理/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si15 = (m_FChar&0x1) ? 1 : 0;								// [F5]:垂直限定有
		}
		Sia[0] = 2, Sia[1] = 2, Sia[2] = 2, Sia[3] = 2;					// ﾄﾗｯｶｰ1色設定
		Sib[0] = 0, Sib[1] = 0, Sib[2] = 0;								// ﾄﾗｯｶｰ2色設定
		Siw[0] = 5, Siw[1] = 5;											// ﾄﾗｯｶｰ3色設定
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		TrackerBox3D(box, Spa);											// ﾄﾗｯｶｰ取得
		Tens[0] = 9, Tens[1] = 10, Tens[2] = 11, Tens[3] = 12;			// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// ﾄﾗｯｶｰ1設定
		Tens[0] = 9, Tens[1] = 12, Tens[2] = 10, Tens[3] = 11;			// ﾄﾗｯｶｰ2ｵﾌﾟｼｮﾝ設定
		Tracker2.Setup(Spa, Sia, Tens, 4, 1, 7);						// ﾄﾗｯｶｰ2設定
		Tens[0] = 9, Tens[1] = 11, Tens[2] = 12, Tens[3] = 10;			// ﾄﾗｯｶｰ3ｵﾌﾟｼｮﾝ設定
		Tracker3.Setup(Spa, Sia, Tens, 4, 2, 7);						// ﾄﾗｯｶｰ3設定
		Line.Reset();													// ﾗｲﾝ解除
		Si11 = 0, Si12 = -1, Si13 = -1, Si14 = -1;						// ﾗｲﾝ情報初期化
		Display(0);														// 再描画
		Sphase = 0, Si9 = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Line.Reset();													// ﾗｲﾝ解除
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/***************************************************************/
void CJcad3GlbView::MouseResize4(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 曲線指定ﾘｻｲｽﾞ2[頂点]
/***************************************************************/
{
	PNTTYPE pnt, wk[3];
	BOXTYPE box;
	int     hit, sft, i, j;
	double  d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2&&plane==Si4&&(flg&MK_LBUTTON)) {					// <<基準線指定>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			pnt.p[Si8] = (pnt.p[Si8]<Sd1) ? Sd1 : pnt.p[Si8];			// ｻｲｽﾞ指定方向位置
			pnt.p[Si8] = (pnt.p[Si8]>Sd2) ? Sd2 : pnt.p[Si8];			// ｻｲｽﾞ指定方向位置
			Spnt1.p[Si8] = pnt.p[Si8], Spnt2.p[Si8] = pnt.p[Si8];		// 基準線座標更新
			Spb[0] = Spnt1, Spc[0] = Spnt2;
			Line.Setup(Spb, Spc, Sib, 1, Si4);							// 基準線(ﾗｲﾝ)設定
			Display(0);													// 再描画
		} else if(Sphase==4&&plane==Si4&&(flg&MK_LBUTTON)) {			// <<基準線指定>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if((Si2&&pnt.p[Si8]<Spnt1.p[Si8])||							// 正側で基準点以下か
               (!Si2&&pnt.p[Si8]>Spnt1.p[Si8])) {						// 負側で基準点以上の場合は
				pnt.p[Si8] = Spnt1.p[Si8];								// 基準点のｻｲｽﾞ指定方向値
			}
			Spy[Si12].p[Si8] = pnt.p[Si8];								// ｻｲｽﾞ指定方向変更
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// 曲線情報複写
			}
			d1 = Spw[0].p[Si6];											// 垂直方向下限
			d2 = Spw[1].p[Si6];											// 垂直方向上限
			i = Si16+(2*Si17);											// 垂直方向限定状況
			ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// 曲線指定ﾘｻｲｽﾞ2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S曲線生成
			SetBaseData(1, i, 0, Spz, wk, -1);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// ﾄﾗｯｶｰ1設定
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Six[i] = 5;												// 追加用ﾄﾗｯｶｰ色
			}
			Si9 = Si1-1;												// 追加用ﾄﾗｯｶｰ数
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// ﾄﾗｯｶｰ2設定
			Display(0);													// 再描画
		} else if(Sphase==5&&plane==Si5&&(flg&MK_LBUTTON)) {			// <<垂直範囲限定>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			d1 = pnt.p[Si6];											// 垂直方向座標
			if(Si12==0) {												// <下限>
				if(d1>=Spd[0].p[Si6]&&d1<Spw[1].p[Si6]) {				// 立体最小～
					Spw[0].p[Si6] = d1;									// 指定上限は更新
					Si16 = 1;											// 下限設定済み
				}
			} else {													// <上限>
				if(d1>Spw[0].p[Si6]&&d1<=Spd[1].p[Si6]) {				// 指定下限～
					Spw[1].p[Si6] = d1;									// 立体最大は更新
					Si17 = 1;											// 上限設定済み
				}
			}
			Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);					// ﾄﾗｯｶｰ3設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {													// <<基準点指定>>
			Si2 = (flg&MK_SHIFT) ? 0 : 1;								// 正負ﾌﾗｸﾞ
			Si3 = (flg&MK_CONTROL) ? 1 : 0;								// 両側ﾌﾗｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 6))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si4 = plane;											// 平面保存
				Si5 = (Si4+1>2) ? 0 : Si4+1;							// 座標軸設定
				Si6 = (Si5+1>2) ? 0 : Si5+1;
				i = hit/2;												// 軸No取得
				if(i!=Si6) {											// <有効なﾄﾗｯｶｰ>
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					BoxCenter(box, &pnt);								// BOXｾﾝﾀｰ取得
					Spd[0] = pnt, Spd[0].p[Si6] = box.bp[0].p[Si6];		// 垂直方向の両端
					Spd[1] = pnt, Spd[1].p[Si6] = box.bp[1].p[Si6];
					Spw[0] = Spd[0], Spw[1] = Spd[1];
					Si16 = 0, Si17 = 0, Si18 = 0;						// 垂直方向限定済みﾌﾗｸﾞ
					if(i==Si4) {										// 長さ方向･ｻｲｽﾞ指定方向設定
						Si7 = Si4, Si8 = Si5;
						for(j=0; j<Si1; j++) {
							Tens[j] = 6;
						}
					} else {
						Si7 = Si5, Si8 = Si4;
						for(j=0; j<Si1; j++) {
							Tens[j] = 5;
						}
					}
					Spnt1 = Spa[Si7*2];									// 長さ方向の基準点
					Spnt2 = Spa[Si7*2+1];
					Sd1 = Spa[Si8*2].p[Si8];							// ｻｲｽﾞ指定方向の移動範囲
					Sd2 = Spa[Si8*2+1].p[Si8];
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					if(Si15) {											// <垂直限定有>
						Siw[0] = 5, Siw[1] = 5, Sic[0] = 6, Sic[1] = 6;	// ﾄﾗｯｶｰ3色設定
						Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);		// ﾄﾗｯｶｰ3設定
					} else {											// <垂直限定無>
						Tracker3.Reset(0);								// ﾄﾗｯｶｰ3解除
					}
					Spb[0] = Spnt1, Spc[0] = Spnt2;
					Line.Setup(Spb, Spc, Sib, 1, Si4);					// 基準線(ﾗｲﾝ)設定
					Display(0);											// 再描画
					Sphase = 2;
				}
			}
		} else if(Sphase==3&&plane==Si4) {								// <<制御点指定>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shiftｷｰ状態
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			Si12 = Tracker1.HitTracker(1, plane, pnt, Spy, Si1);		// ﾄﾗｯｶｰ1ﾁｪｯｸ
			if(Si12!=-1) {
				if(sft) {
					Siy[Si12] = (Siy[Si12]) ? 0 : 1;					// Shift:ONは曲線ﾓｰﾄﾞ反転
				}
				Siy[0] = 1, Siy[Si1-1] = 1;								// 始点･終点は曲線ﾓｰﾄﾞ:1
				Sphase = 4;
				Si18 = 1;												// ｻｲｽﾞ変更済み
			}
			if(Sphase!=4) {												// <ﾄﾗｯｶｰ選択無の場合>
				Si12 = Tracker2.HitTracker(1, plane, pnt, Spx, Si9);	// ﾄﾗｯｶｰ2ﾁｪｯｸ
				if(Si12!=-1) {
					pnt = Spx[Si12];
					for(i=Si1-1; i>Si12; i--) {
						Spy[i+1] = Spy[i];								// 後ろへｼﾌﾄ
						Siy[i+1] = Siy[i];
						Tens[i+1] = Tens[i];
					}
					Spy[Si12+1] = pnt;									// 指定位置を追加
					Siy[Si12+1] = (sft) ? 1 : 0;
					Tens[Si12+1] = Tens[0];
					Si1++;												// 変更後制御点数
					for(i=0; i<Si1; i++) {
						Spz[i] = Spy[i], Siz[i] = Siy[i];				// 曲線情報複写
					}
					d1 = Spw[0].p[Si6];									// 垂直方向下限
					d2 = Spw[1].p[Si6];									// 垂直方向上限
					i = Si16+(2*Si17);									// 垂直方向限定状況
					ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                                  i, d1, d2, Spnt1, Si1, Spy, Siy);		// 曲線指定ﾘｻｲｽﾞ2
					i = Si1;
					MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);				// S曲線生成
					SetBaseData(1, i, 0, Spz, wk, -1);					// 基準線ﾃﾞｰﾀ登録
					Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);		// ﾄﾗｯｶｰ1設定
					for(i=0; i<Si1-1; i++) {
						for(j=0; j<3; j++) {
							Spx[i].p[j]=(Spy[i].p[j]+Spy[i+1].p[j])/2.0;// 追加用ﾄﾗｯｶｰ
						}
						Six[i] = 5;										// 追加用ﾄﾗｯｶｰ色
					}
					Si9 = Si1-1;										// 追加用ﾄﾗｯｶｰ数
					Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);		// ﾄﾗｯｶｰ2設定
					Si18 = 1;											// ｻｲｽﾞ変更済み
					Display(0);											// 再描画
				}
			}
		} else if(Sphase==3&&plane==Si5) {								// <<垂直範囲限定>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(Si15&&!Si18) {											// <ｻｲｽﾞ変更未の場合>
				Si12 = Tracker3.HitTracker(1, plane, pnt, Spw, 2);		// ﾄﾗｯｶｰ3ﾁｪｯｸ
				if(Si12!=-1) {
					Sphase = 5;
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==2) {													// <<基準線指定>>
			d1 = Spnt2.p[Si7]-Spnt1.p[Si7];								// 線分両端差
			j = Si1-1, d1 /= j;											// 中間制御点計算
			Spy[0] = Spnt1, Spy[j] = Spnt2;
			Siy[0] = 1, Siy[j] = 1;
			for(i=1; i<j; i++) {
				Spy[i] = Spnt1, Siy[i] = 0;
				Spy[i].p[Si7] = Spnt1.p[Si7]+d1*i;
			}
			Si2 = (Spnt1.p[Si8]<=Sd1) ? 1 : Si2;						// 正負ﾌﾗｸﾞ補正
			Si2 = (Spnt1.p[Si8]>=Sd2) ? 0 : Si2;
			if(Si2) {													// <正側の場合>
				Sd3 = Sd2 - Spnt1.p[Si8];								// 基準長さ
				for(i=0; i<Si1; i++) {
					Spy[i].p[Si8] = Sd2;								// 最大値設定
				}
			} else {													// <負側の場合>
				Sd3 = Spnt1.p[Si8] - Sd1;								// 基準長さ
				for(i=0; i<Si1; i++) {
					Spy[i].p[Si8] = Sd1;								// 最小値設定
				}
			}
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// 曲線情報複写
			}
			d1 = Spw[0].p[Si6];											// 垂直方向下限
			d2 = Spw[1].p[Si6];											// 垂直方向上限
			i = Si16+(2*Si17);											// 垂直方向限定状況
			ObjectResize4(0, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// 曲線指定ﾘｻｲｽﾞ2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S曲線生成
			SetBaseData(1, i, 0, Spz, wk, Si4);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// ﾄﾗｯｶｰ1設定
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Six[i] = 5;												// 追加用ﾄﾗｯｶｰ色
			}
			Si9 = Si1-1;												// 追加用ﾄﾗｯｶｰ数
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// ﾄﾗｯｶｰ2設定
			Sphase = 3;
			Display(0);													// 再描画
		} else if(Sphase==4||Sphase==5) {								// <<制御点指定>>
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// 曲線情報複写
			}
			d1 = Spw[0].p[Si6];											// 垂直方向下限
			d2 = Spw[1].p[Si6];											// 垂直方向上限
			i = Si16+(2*Si17);											// 垂直方向限定状況
			ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// 曲線指定ﾘｻｲｽﾞ2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S曲線生成
			SetBaseData(1, i, 0, Spz, wk, -1);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// ﾄﾗｯｶｰ1設定
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Six[i] = 5;												// 追加用ﾄﾗｯｶｰ色
			}
			Si9 = Si1-1;												// 追加用ﾄﾗｯｶｰ数
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// ﾄﾗｯｶｰ2設定
			if(Si15) {													// <垂直限定有>
				Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);				// ﾄﾗｯｶｰ3設定
			} else {													// <垂直限定無>
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
			}
			Sphase = 3;
			Display(0);													// 再描画
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			OffUndoRedo();												// Undo/Redo不可
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si15 = (m_FChar&0x1) ? 1 : 0;								// [F5]:垂直限定有
		}
		if(m_NumInp!=0) {												// <数値入力の場合>
			CInputDlg dlg(13);
			dlg.SetValue(m_LinePNum);									// 既存値表示
			if(dlg.DoModal()==IDOK) {									// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(&m_LinePNum);								// 基準線制御点数取得
				Si1 = m_LinePNum;
			} else {
				Si1 = 0;
			}
		} else {														// <数値入力無の場合>
			Si1 = 2;
		}
		if(Si1>0) {
			Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;				// ﾄﾗｯｶｰ色設定
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &pnt);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBoxCrs(box, pnt, Spa);								// ﾄﾗｯｶｰ取得
			Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// ﾄﾗｯｶｰ1設定
			Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);				// ﾄﾗｯｶｰ2設定
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);					// ﾄﾗｯｶｰ3設定
			Display(0);													// 再描画
			Sphase = 1;
			m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;			// ｶｰｿﾙNo設定
		} else {
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		DeleteBaseData();												// 基準線ﾃﾞｰﾀ削除
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Line.Reset();													// ﾗｲﾝ解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}
