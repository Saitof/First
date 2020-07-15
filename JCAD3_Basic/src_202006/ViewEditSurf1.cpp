///////////////////////////////////////////////////////////////////////////// 
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewEditSurf1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputBtnDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "EditCtrlTracker.h"
#include "EditObject.h"
#include "Symmetry.h"
#include "ConvSCurve.h"
#include "NumFunc.h"

/*******************************************************************/
void CJcad3GlbView::MouseCPointSpace(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< ＣＰＳ
/*******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt1, pt2;
	BOXTYPE  box;
	int      hit, i, i1, i2, i3, gno[32];
	double   th, l11, l12, l21, l22;

	if((Sphase!=0&&(ms==2||ms==3)&&Si22)||
       (Sphase>=2&&ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX)) {			// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==3&&plane==Si11&&(flg&MK_LBUTTON)) {			// ﾏｳｽ左釦押下中のみ有効
			if(Si14==0) {												// <移動処理>
				CalcScreenWorld(0, &pnt, point, plane);					// 座標点取得
				for(i=0; i<DIM; i++) {
					Sdb[i] = pnt.p[i]-Spnt4.p[i];						// 立体移動量算出
				}
				if(flg&m_LimKey) {
					LimitValue(plane, Sdb);								// ﾏｳｽ入力制限値
				}
				Sdb[Si13] = 0.0;										// 指定平面外の座標は0
			} else if(Si14==1) {										// <回転処理>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				l11 = Spnt4.p[Si11]-Sbase.p[Si11];						// 角度算出
				l12 = Spnt4.p[Si12]-Sbase.p[Si12];
				l21 = pnt.p[Si11]-Sbase.p[Si11];
				l22 = pnt.p[Si12]-Sbase.p[Si12];
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
				Sdb[Si13+3] = th;
			} else {													// <ｻｲｽﾞ変更処理>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				pt1 = Spnt4, pt1.p[Si13] = 0.0;							// 指定平面以外の座標は0
				pt2 = Sbase, pt2.p[Si13] = 0.0;
				pnt.p[Si13] = 0.0;
				l11 = PntD(&pt1, &pt2);									// 中心点とﾄﾗｯｶｰ位置までの距離
				l12 = PntD(&pnt, &pt2);									// 中心点とｶｰｿﾙ位置までの距離
				if(Si15==0) {											// <ｻｲｽﾞ変更の場合>
					l21 = (l12/l11) - 1.0;								// 増減分の倍率
					Sdb[6] = l21, Sdb[7] = l21, Sdb[8] = l21;
				} else {												// <放射状移動の場合>
					Sdb[9] = l12 - l11;									// 移動距離
				}
			}
			pnt = (Si9==0) ? Spnt1 : ((Si9==1) ? Spnt2 : Spnt3);
			CtrlPointSpace(Spa, Spw, Sib, Si5, Spnt2, pnt, Si15, Si11,
                           Sda[0]+Sdb[0], Sda[1]+Sdb[1], Sda[2]+Sdb[2],
                           Sda[3]+Sdb[3], Sda[4]+Sdb[4], Sda[5]+Sdb[5],
                           Sda[6]+Sdb[6], Sda[7]+Sdb[7], Sda[8]+Sdb[8],
                           Sda[9]+Sdb[9]);								// 制御点ｽﾍﾟｰｽ(CPS)
			MoveCPoint(0, Spw, Sop1, Si1, Si2, Si3, Si4, m_ObjCWgt,
                       Sia, Sib, Spa, Spb, Spc, Tens);					// 制御点移動
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成通過点変換
			for(i=0; i<Si5; i++) {
				Spd[i] = Spc[i];
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			Tracker4.Setup(Spd, Sis, NULL, Si5, 4, 3);					// ﾄﾗｯｶｰ4設定(制御点位置表示)
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			Si6 = 1;													// 実行済み
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spb, Si5, Spx,
                                 Six, Si10, &Si21, &i1, Siy);			// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(i1>0) {												// <制御点選択の場合>
					PickPointMove(0, i1, Siy, Si1, Si2, Si3, Si4,
                                  m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                                  Sop2, Spa, Spb, Sia, Sib, Sic);		// 制御点移動 点選択
					SetParsTracker(0, &Si21, 0, Sib, Spb, Sic,
                                   Si5, Spx, Six, &Si10);				// 透視図ﾄﾗｯｶｰ設定
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					Si23 = 1;
				}
			}
		} else if(Sphase==3) {											// <編集の場合>
			for(i=0; i<10; i++) {
				Sda[i] += Sdb[i], Sdb[i] = 0.0;							// 加減算値を加算
			}
			Sda[6] = (Sda[6]<0.01) ? 0.01 : Sda[6];						// 有効倍率ﾁｪｯｸ
			Sda[7] = (Sda[7]<0.01) ? 0.01 : Sda[7];
			Sda[8] = (Sda[8]<0.01) ? 0.01 : Sda[8];
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &Sbase);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBoxCrs2(box, Sbase, Spy);							// ﾄﾗｯｶｰ取得
			Spz[0] = Spy[0], Spz[1] = Spy[1], Spz[2] = Spy[2];
			Spz[3] = Spy[3], Spz[4] = Spy[4], Spz[5] = Spy[7];
			Tracker1.Setup(Spz, Siz, TensC, 6, 0, 7);					// ﾄﾗｯｶｰ1設定
			Spz[0] = Spy[0], Spz[1] = Spy[3], Spz[2] = Spy[4];
			Spz[3] = Spy[5], Spz[4] = Spy[6], Spz[5] = Spy[7];
			Tracker2.Setup(Spz, Siz, TensC, 6, 1, 7);					// ﾄﾗｯｶｰ2設定
			Spz[0] = Spy[0], Spz[1] = Spy[5], Spz[2] = Spy[6];
			Spz[3] = Spy[1], Spz[4] = Spy[2], Spz[5] = Spy[7];
			Tracker3.Setup(Spz, Siz, TensC, 6, 2, 7);					// ﾄﾗｯｶｰ3設定
			Tracker4.Setup(Spd, Sis, NULL, Si5, 4, 3);					// ﾄﾗｯｶｰ4設定(制御点位置表示)
			Sphase = 2;
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(Sphase==1) {													// <<制御点選択>>
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, Si5,
                            Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <制御点移動の場合>
				Sphase = 2, Si6 = 0;
				if(m_NumInp!=0) {										// <数値入力の場合>
					Sphase = 1;
					if(!Si15) {											// <倍率指定>
						CInputBtnDlg7 dlg(3);
						dlg.SetParam2(Sop1, Sop2, Si21, Si1, Si2, Si3,
                                      Si4, Si5, Si10, &Si6, Spnt1,
                                      Spnt2, Spnt3, Spa, Spb, Spc, Spd,
                                      Spw, Spx, Sia, Sib, Sic, Six,
                                      Tens, m_EarthAdd1, m_EarthAdd2,
                                      m_EarthAdd3, m_ScaleDir,
                                      m_ScalePnt);						// ﾊﾟﾗﾒｰﾀ設定2
						if(dlg.DoModal()==IDOK) {						// 入力ﾀﾞｲｱﾛｸﾞ表示
							if(Si6==1) {								// 移動済みの場合は
								for(i=0; i<Si5; i++) {					// 正式ﾊﾞｯﾌｧ変更
									Spa[i] = Spc[i], Spb[i] = Spd[i];
								}
							}
						}
						dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2,
                                     &m_EarthAdd3, &m_ScaleDir,
                                     &m_ScalePnt);						// ﾊﾟﾗﾒｰﾀ取得
					} else {											// <放射状移動>
						CInputBtnDlg8 dlg;
						dlg.SetParam(Sop1, Sop2, Si21, Si1, Si2, Si3,
                                     Si4, Si5, Si10, plane, &Si6,
                                     Spnt2, Spa, Spb, Spc, Spd, Spw,
                                     Spx, Sia, Sib, Sic, Six, Tens,
                                     m_EarthAdd1, m_EarthAdd2);			// ﾊﾟﾗﾒｰﾀ設定
						if(dlg.DoModal()==IDOK) {						// 入力ﾀﾞｲｱﾛｸﾞ表示
							if(Si6==1) {								// 移動済みの場合は
								for(i=0; i<Si5; i++) {					// 正式ﾊﾞｯﾌｧ変更
									Spa[i] = Spc[i], Spb[i] = Spd[i];
								}
							}
						}
						dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2);		// ﾊﾟﾗﾒｰﾀ取得
					}
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// 曲面生成
					for(i=0; i<Si5; i++) {
						Spb[i] = Spa[i];
					}
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// 曲面制御点生成
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					DispParsTracker(0, Si21, Spb, Sic, Si5,
                                    Spx, Six, Si10);					// 透視図ﾄﾗｯｶｰ表示
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				} else {												// <ﾏｳｽ入力の場合>
					Siz[0] = 0, Siz[1] = 1, Siz[2] = 1;					// ﾄﾗｯｶｰ色設定
					Siz[3] = 1, Siz[4] = 1, Siz[5] = 2;
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					BoxCenter(box, &Sbase);								// ﾎﾞｯｸｽｾﾝﾀｰ取得
					TrackerBoxCrs2(box, Sbase, Spy);					// ﾄﾗｯｶｰ取得
					TensC[0] = 7, TensC[1] = 5, TensC[2] = 5;
					TensC[3] = 6, TensC[4] = 6, TensC[5] = 8;
					Spz[0] = Spy[0], Spz[1] = Spy[1], Spz[2] = Spy[2];
					Spz[3] = Spy[3], Spz[4] = Spy[4], Spz[5] = Spy[7];
					Tracker1.Setup(Spz, Siz, TensC, 6, 0, 7);			// ﾄﾗｯｶｰ1設定
					Spz[0] = Spy[0], Spz[1] = Spy[3], Spz[2] = Spy[4];
					Spz[3] = Spy[5], Spz[4] = Spy[6], Spz[5] = Spy[7];
					Tracker2.Setup(Spz, Siz, TensC, 6, 1, 7);			// ﾄﾗｯｶｰ2設定
					Spz[0] = Spy[0], Spz[1] = Spy[5], Spz[2] = Spy[6];
					Spz[3] = Spy[1], Spz[4] = Spy[2], Spz[5] = Spy[7];
					Tracker3.Setup(Spz, Siz, TensC, 6, 2, 7);			// ﾄﾗｯｶｰ3設定
					Tracker4.Setup(Spb, Sis, NULL, Si5, 4, 3);			// ﾄﾗｯｶｰ4設定(制御点位置表示)
				}
			} else if(i1>0) {											// <制御点選択の場合>
				if(!m_CtlInp) {
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// 全制御点選択解除
					}
				}
				PickPointMove(0, i1, Siy, Si1, Si2, Si3, Si4,
                              m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                              Sop2, Spa, Spb, Sia, Sib, Sic);			// 制御点移動 点選択
				SetParsTracker(0, &Si21, 0, Sib, Spb, Sic, Si5, Spx,
                               Six, &Si10);								// 透視図ﾄﾗｯｶｰ設定
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				Si23 = 1;
			} else if(!m_CtlInp) {										// <ﾄﾗｯｶｰ指定無で追加無>
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 全制御点選択解除
				}
				Si21 = 0, Si10 = 0;										// 透視図選択解除
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ3解除
			}
		} else if(Sphase==2) {											// <<編集選択>>
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spy, 8))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si11 = plane;											// 平面保存
				Si12 = (Si11+1>2) ? 0 : Si11+1;							// 座標軸設定
				Si13 = (Si12+1>2) ? 0 : Si12+1;
				Si14 = -1;												// 処理未定
				if(hit==0) {											// <移動>
					Si14 = 0;											// 移動処理
					Spnt4 = Sbase;
				} else if(hit==7) {										// <回転>
					Si14 = 1;											// 回転処理
					Spnt4 = Spy[7];
				} else {												// <ｻｲｽﾞ変更>
					i = (hit-1)/2;										// 軸No取得
					if(i!=Si13) {										// <有効なﾄﾗｯｶｰ>
						Si14 = 2;										// ｻｲｽﾞ変更処理
						Spnt4 = Spy[hit];
					}
				}
				if(Si14!=-1) {											// <ﾄﾗｯｶｰ選択OK>
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Sphase = 3, Sd1 = 0.0, Sd2 = 0.0;
				}
			}
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si9 = (m_FChar&0x4) ? 0 : 1;								// [F7]:基準点-Z最小指定
			Si9 = (m_FChar&0x8) ? 2 : Si9;								// [F8]:基準点-Z最大指定
			Si15 = (m_FChar&0x10) ? 1 : 0;								// [F9]:放射状移動
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <通常曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 制御点対象ﾌﾗｸﾞｸﾘｱ
					Sis[i] = 4;											// 移動時ﾄﾗｯｶｰ色設定
				}
				for(i=0; i<10; i++) {
					Sda[i] = 0.0, Sdb[i] = 0.0;
				}
				Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;				// 倍率のみ初期値は1.0
				CalcBox(Spa, Si5, &box);								// 図形BOXｻｲｽﾞ取得
				BoxCenter(box, &Spnt2);									// ﾎﾞｯｸｽｾﾝﾀｰ取得
				Spnt1 = Spnt2, Spnt1.p[2] = box.bp[0].p[2];				// ﾎﾞｯｸｽｾﾝﾀｰ(Z最小)
				Spnt3 = Spnt2, Spnt3.p[2] = box.bp[1].p[2];				// ﾎﾞｯｸｽｾﾝﾀｰ(Z最大)
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// 制御点列確認
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
						Tracker2.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ2範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ4解除
				Sphase = 1;												// 制御点指定へ
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==2) {													// <<編集中>>
			if(Si6==1) {												// 移動済みの場合は
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// 正式ﾊﾞｯﾌｧ変更
				}
			}
			RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);				// 曲面生成
			for(i=0; i<Si5; i++) {
				Spb[i] = Spa[i];
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spb);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spb[i]);								// 立体編集後制御点取得
			}
			for(i=0; i<Si5; i++) {
				Sib[i] = 0;												// 全制御点選択解除
			}
			Si21 = 0, Si10 = 0;											// 透視図選択解除
			InitLineNo(Sop2);											// ﾗｲﾝ番号初期化
			Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);					// ﾄﾗｯｶｰ1設定
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
			Tracker3.Reset(0);											// ﾄﾗｯｶｰ3解除
			Tracker4.Reset(0);											// ﾄﾗｯｶｰ4解除
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			Sphase = 1, i1 = 1;
			for(i=0; i<10; i++) {
				Sda[i] = 0.0, Sdb[i] = 0.0;
			}
			Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;					// 倍率のみ初期値は1.0
			Display(0);													// 再描画
		} else {
			i1 = 0;
		}
		if(i1==0||ms==9) {												// <<上記以外/強制終了>>
			if(Si20) {
				AttachObj(Sop1);										// ｺﾏﾝﾄﾞ再接続/再実行
			}
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// 立体削除
			}
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
			Tracker2.Reset(1);											// ﾄﾗｯｶｰ2解除
			Tracker3.Reset(1);											// ﾄﾗｯｶｰ3解除
			Tracker4.Reset(1);											// ﾄﾗｯｶｰ4解除
			Line.Reset();												// ﾗｲﾝ解除
			SetRenderMode(0);											// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			OnUndoRedo();												// Undo/Redo可
			InitLineNo(NULL);											// ﾗｲﾝ番号初期化
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
			if(!Si23) {
				Undo(gno);												// 編集なしはUndo
			}
			Display(0);													// 再描画
		}
	}
}

/*****************************************************************/
void CJcad3GlbView::MousePointMove(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< 制御点移動
/*****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Spnt1);						// ﾏｳｽ入力制限座標
			}
			GetMoveValue(plane, Spnt1, &pnt);							// ﾄﾗｯｶｰ移動量取得
			MoveSymmetryArea(Spnt1, pnt, Si11, Spnt2, 0, Si14, Si15,
                             -1, -1, Si13, 0, Si5, Sib, Spb, Spw);		// 対称領域移動量設定
			MoveCPoint(Si9, Spw, Sop1, Si1, Si2, Si3, Si4, m_ObjCWgt,
                       Sia, Sib, Spa, Spb, Spc, Tens);					// 制御点移動
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成
			CalcSurfacePoint(Si9, 1, 0, Si1, Si2, Si3, Si4, m_ObjCWgt,
                             Sia, Spc, Spd);							// 制御点=>通過点変換
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			DispParsTracker(0, Si21, Spd, Sic, Si5, Spx, Six, Si10);	// 透視図ﾄﾗｯｶｰ表示
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// 移動前点との距離
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Si6 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
			}
		} else if(Sphase==1) {											// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spb, Si5, Spx,
                                 Six, Si10, &Si21, &i1, Siy);			// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(i1>0) {												// <制御点選択の場合>
					PickPointMove(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                                  m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                                  Sop2, Spa, Spb, Sia, Sib, Sic);		// 制御点移動 点選択
					SetParsTracker(0, &Si21, 0, Sib, Spb, Sic,
                                   Si5, Spx, Six, &Si10);				// 透視図ﾄﾗｯｶｰ設定
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					Sphase = 1, Si23 = 1;
				}
			}
		} else if(Sphase==2) {											// <制御点移動の場合>
			if(Si6==1) {												// 移動済みの場合は
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// 正式ﾊﾞｯﾌｧ変更
				}
			}
			Sphase = 1;
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				CalcScreenWorld(0, &Spnt2, point, plane);				// 対称軸中心点取得
				Si12 = plane;											// 対称軸指定平面
				i = (flg&MK_CONTROL) ? 1 : 0;							// 横方向指定
				Si13 = (flg&MK_SHIFT) ? 1 : 0;							// 軸方向反転
				SetSymmetryLine(1,Si11,Spnt2,Si12, i, 0, &Si14, &Si15);	// 対称軸設定
			}
		} else {														// <制御点指定の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, Si5,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <制御点移動の場合>
				Spnt1 = Spb[Siy[0]];									// 移動元点保存
				Sphase = 2, Si6 = 0;
				if(m_NumInp!=0) {										// <数値入力の場合>
					Sphase = 1;
					CInputBtnDlg6 dlg(2);
					dlg.SetParam2(Sop1, Sop2, Si9, Si21, Si1, Si2, Si3,
                                  Si4, m_ObjCWgt, Si5, Si10, &Si6,
                                  Si11, Si14, Si15, Si13, Spnt1, Spnt2,
                                  Spa, Spb, Spc, Spd, Spw, Spx, Sia,
                                  Sib, Sic, Six, Tens, m_PMoveAdd);		// ﾊﾟﾗﾒｰﾀ設定2
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						if(Si6==1) {									// 移動済みの場合は
							for(i=0; i<Si5; i++) {						// 正式ﾊﾞｯﾌｧ変更
								Spa[i] = Spc[i], Spb[i] = Spd[i];
							}
						}
					}
					dlg.GetParam(&m_PMoveAdd);							// ﾊﾟﾗﾒｰﾀ取得
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// 曲面生成
					CalcSurfacePoint(Si9, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// 制御点=>通過点変換
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// 曲面制御点生成
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					DispParsTracker(0, Si21, Spb, Sic, Si5, Spx,
                                    Six, Si10);							// 透視図ﾄﾗｯｶｰ表示
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				}
			} else if(i1>0) {											// <制御点選択の場合>
				if(!m_CtlInp) {
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// 全制御点選択解除
					}
				}
				PickPointMove(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                              m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                              Sop2, Spa, Spb, Sia, Sib, Sic);			// 制御点移動 点選択
				SetParsTracker(0, &Si21, 0, Sib, Spb, Sic, Si5, Spx,
                               Six, &Si10);								// 透視図ﾄﾗｯｶｰ設定
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				Si23 = 1;
			} else if(!m_CtlInp) {										// <ﾄﾗｯｶｰ指定無で追加無>
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 全制御点選択解除
				}
				Si21 = 0, Si10 = 0;										// 透視図選択解除
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
			}
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si11 = (m_FChar&0x8) ? 2 : 0;								// [F8]:入力ﾓｰﾄﾞ:2軸対称
			Si11 = (m_FChar&0x4) ? 1 : Si11;							// [F7]:入力ﾓｰﾄﾞ:1軸対称
			Si9 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <通常曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 制御点対象ﾌﾗｸﾞｸﾘｱ
				}
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				ExecSurface(Si9, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// 制御点列確認
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
						Tracker2.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ2範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				SetNumDisp(2);											// 数値表示初期処理
				if(Si11==0) {											// <通常ﾓｰﾄﾞの場合>
					Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);			// ﾄﾗｯｶｰ1設定
					Sphase = 1;											// 制御点指定へ
				}
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Line.Reset();													// ﾗｲﾝ解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		InitLineNo(NULL);												// ﾗｲﾝ番号初期化
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}

/****************************************************************/
void CJcad3GlbView::MouseLineMove(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< 制御点列移動
/****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Spnt1);						// ﾏｳｽ入力制限座標
			}
			GetMoveValue(plane, Spnt1, &pnt);							// ﾄﾗｯｶｰ移動量取得
			if(Si11) {													// <通常曲面>
				MoveSymmetryArea(Spnt1, pnt, Si14, Spnt2, 0, Si17, Si18,
                                 -1, -1, Si16, 0, Si5, Sib, Spb, Spy);	// 対称領域移動量設定
				MoveCPoint(Si13, Spy, Sop1, Si1, Si2, Si3, Si4,
                           m_ObjCWgt, Sia, Sib, Spa, Spb, Spc, Tens);	// 制御点移動
				RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);			// 曲面生成
				CalcSurfacePoint(Si13, 1,0, Si1, Si2, Si3, Si4,
                                 m_ObjCWgt, Sia, Spc, Spd);				// 制御点=>通過点変換
			} else {													// <掃引曲面>
				MoveSymmetryArea(Spnt1, pnt, Si14, Spnt2, 0, Si17, Si18,
                                 -1, -1, Si16, 0, Si6, Siw, Spz, Spy);	// 対称領域移動量設定
				MoveSweep(Spy, Si12, Si1, Si2, Sps, Si6, Siw, Spt);		// 掃引制御点移動
				RedoChgSweepSurf(Si1, Si2, Si3, Si4, Spt, Sis);			// 掃引曲面変更
				SetSweepCtrl(Si12, Si1, Si2, Si3, Spt, Sis, Spd, Siz);	// 掃引曲面制御点変換
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			TrackerCLine(Si13, Si11, Si1, Si2, Spd, Si7, Si8,
                         &Si6, Spw, Sic);								// 点列ﾄﾗｯｶｰ取得
			for(i=0; i<Si6; i++) {
				Sic[i] = (Siw[i]) ? 4 : Sic[i];							// 指定点色変更
			}
			DispParsTracker(0, Si21, Spw, Sic, Si6, Spx, Six, Si10);	// 透視図ﾄﾗｯｶｰ表示
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// 移動前点との距離
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Si9 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
			}
		} else if(Sphase==1) {											// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spw, Si6, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(i1>0) {												// <制御点選択の場合>
					PickLineMove(Si13, i1, Siy, Si11, Si1, Si2, Si3,
                                 Si4, m_ObjCWgt, Si5, Si7, Si8,
                                 m_CurInp, Si12, &Si6, Sop2, Spa, Spb,
                                 Sps, Spw, Sia, Sib, Sic, Sis, Siw);	// 制御点列移動 列選択
					SetParsTracker(0, &Si21, 0, Siw, Spw, Sic,
                                   Si6, Spx, Six, &Si10);				// 透視図ﾄﾗｯｶｰ設定
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					Si23 = 1;
				}
			}
		} else if(Sphase==2) {											// <制御点移動の場合>
			if(Si9==1) {												// 移動済みの場合は
				for(i=0; i<Si5; i++) {									// 正式ﾊﾞｯﾌｧ変更
					Spa[i] = Spc[i], Spb[i] = Spd[i];
				}
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i];									// ﾄﾗｯｶｰ保存
				}
				if(!Si11) {
					for(i=0; i<Si1+Si2; i++) {
						Sps[i] = Spt[i];
					}
				}
			}
			Sphase = 1;
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				CalcScreenWorld(0, &Spnt2, point, plane);				// 対称軸中心点取得
				Si15 = plane;											// 対称軸指定平面
				i = (flg&MK_CONTROL) ? 1 : 0;							// 横方向指定
				Si16 = (flg&MK_SHIFT) ? 1 : 0;							// 軸方向反転
				SetSymmetryLine(1, Si14, Spnt2, Si15, i, 0,
                                &Si17, &Si18);                          // 対称軸設定
			}
		} else {														// <制御点指定の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si6,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Siw[Siy[0]]==1&&plane!=3) {	// <制御点移動の場合>
				Spnt1 = Spw[Siy[0]];									// 移動元点保存
				Sphase = 2, Si9 = 0;
				if(m_NumInp!=0) {										// <数値入力の場合>
					Sphase = 1;
					CInputBtnDlg6 dlg(3);
					dlg.SetParam3(Sop1, Sop2, Si13, Si11, Si21, Si1,
                                  Si2, Si3, Si4, m_ObjCWgt, Si5, Si7,
                                  Si8, Si10, Si6, Si12, &Si9, Si14,
                                  Si17, Si18, Si16, Spnt1, Spnt2,
                                  Spa, Spb, Spc, Spd, Sps, Spt, Spw,
                                  Spx, Spy, Spz, Sia, Sib, Sic,
                                  Sis, Siw, Six, Tens, m_LMoveAdd);		// ﾊﾟﾗﾒｰﾀ設定3
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						if(Si9==1) {									// 移動済みの場合は
							for(i=0; i<Si5; i++) {						// 正式ﾊﾞｯﾌｧ変更
								Spa[i] = Spc[i], Spb[i] = Spd[i];
							}
							if(!Si11) {
								for(i=0; i<Si1+Si2; i++) {
									Sps[i] = Spt[i];
								}
							}
						}
					}
					dlg.GetParam(&m_LMoveAdd);							// ﾊﾟﾗﾒｰﾀ取得
					if(Si11) {											// <通常曲面>
						RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);	// 曲面生成
						CalcSurfacePoint(Si13, 1, 0, Si1, Si2, Si3,
                                         Si4, m_ObjCWgt, Sia, Spa, Spb);// 制御点=>通過点変換
					} else {											// <掃引曲面>
						RedoChgSweepSurf(Si1, Si2, Si3, Si4, Sps, Sis);	// 掃引曲面変更
						SetSweepCtrl(Si12, Si1, Si2, Si3, Sps,
                                     Sis, Spb, Siz);					// 掃引曲面制御点変換
					}
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// 曲面制御点生成
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7,
                                 Si8, &Si6, Spw, Sic);					// 点列ﾄﾗｯｶｰ取得
					for(i=0; i<Si6; i++) {
						Spz[i] = Spw[i];								// ﾄﾗｯｶｰ保存
						Sic[i] = (Siw[i]) ? 4 : Sic[i];					// 指定点色変更
					}
					DispParsTracker(0, Si21, Spw, Sic, Si6, Spx,
                                    Six, Si10);							// 透視図ﾄﾗｯｶｰ表示
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				}
			} else if(i1>0) {											// <制御点選択の場合>
				if(!m_CtlInp) {
					for(i=0; i<Si6; i++) {
						Siw[i] = 0;										// 指定ﾌﾗｸﾞｸﾘｱ
					}
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// 制御点対象ﾌﾗｸﾞｸﾘｱ
					}
				}
				PickLineMove(Si13, i1, Siy, Si11, Si1, Si2, Si3,
                             Si4, m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                             Si12, &Si6, Sop2, Spa, Spb, Sps, Spw,
                             Sia, Sib, Sic, Sis, Siw);					// 制御点列移動 列選択
				SetParsTracker(0, &Si21, 0, Siw, Spw, Sic, Si6,
                               Spx, Six, &Si10);						// 透視図ﾄﾗｯｶｰ設定
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				Si23 = 1;
			} else if(!m_CtlInp) {										// <ﾄﾗｯｶｰ指定無で追加無>
				for(i=0; i<Si6; i++) {
					Siw[i] = 0;											// 指定ﾌﾗｸﾞｸﾘｱ
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 制御点対象ﾌﾗｸﾞｸﾘｱ
				}
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
			}
		}
		Display(0);														// 再描画
	} else if(ms==11) {													// <<<DEL(削除)>>>
		for(i=0, i1=0; i<Si6; i++) {
			if(Siw[i]==1) {
				i1++;													// 削除対象ｶｳﾝﾄ
			}
		}
		if(i1>0) {
			i1 = Si2, i2 = Si2, i3 = Si1;								// U/V方向有効点列確認
			i1 = (Si7>0) ? i1 - 1 : i1;
			i2 = (Si7>0) ? i2 - 1 : i2;
			i1 = (Si7>2) ? i1 - 1 : i1;
			i2 = (Si7>2) ? i2 - 1 : i2;
			i3 = (Si8>0) ? i3 - 1 : i3;
			i3 = (Si8>2) ? i3 - 1 : i3;
			for(i=0; i<i1; i++) {
				i2 = (Siw[i]==1) ? i2 - 1 : i2;							// V削除対象ｶｳﾝﾄ
			}
			for(i=i1; i<Si6; i++) {
				i3 = (Siw[i]==1) ? i3 - 1 : i3;							// U削除対象ｶｳﾝﾄ
			}
			if(i3<2||i2<3) {											// <制御点下限以下>
				AfxMessageBox(IDS_FEW_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else {													// <制御点下限より大>
				InitLineNo(NULL);										// ﾗｲﾝ番号初期化
				if(Si11) {												// <通常曲面>
					DeleteCLine(Si6, Siw, Si3, Si4, Si7, Si8,
                                &Si1, &Si2, Spa, Sia, Tens);			// 対象制御点列削除
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// 曲面変更
					CalcSurfacePoint(Si13, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// 制御点=>通過点変換
				} else {												// <掃引曲面>
					DeleteSweep(Si6, Siw, Si3, Si4, &Si1, &Si2,
                                Sps, Sis);								// 掃引制御点列削除
					RedoChgSweepSurf(Si1, Si2, Si3, Si4, Sps, Sis);		// 掃引曲面変更
					SetSweepCtrl(Si12, Si1, Si2, Si3, Sps, Sis,
                                 Spb, Siz);								// 掃引曲面制御点変換
				}
				Si5 = Si1*Si2;											// 総制御点数
				RedoSetSurface(Si1, Si2, Si3, Si4, Spb);				// 曲面制御点生成
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2,i,&Spb[i]);								// 立体編集後制御点取得
				}
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i], Siw[i] = 0;						// ﾄﾗｯｶｰ保存/指定ﾌﾗｸﾞｸﾘｱ
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 制御点対象ﾌﾗｸﾞｸﾘｱ
				}
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				Display(0);												// 再描画
			}
			Sphase = 1;
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si14 = (m_FChar&0x8) ? 2 : 0;								// [F8]:入力ﾓｰﾄﾞ:2軸対称
			Si14 = (m_FChar&0x4) ? 1 : Si14;							// [F7]:入力ﾓｰﾄﾞ:1軸対称
			Si13 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==2&&cmp!=NULL) {									// <曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd2(cmp, i3, &Si11, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si12, Sps, Sis);							// 曲面ｺﾏﾝﾄﾞ取得2
				if(!Si11&&Si13) {										// 掃引曲面&通過点ﾓｰﾄﾞは
					AfxMessageBox(IDS_ERR_SWEEP);						// ﾒｯｾｰｼﾞ
					Si13 = 0;											// 掃引曲面は制御点ﾓｰﾄﾞ
				}
				if(!Si11&&!Si20) {										// 掃引曲面&生成時:OFFは
					AfxMessageBox(IDS_SWEEPSURF);						// ﾒｯｾｰｼﾞ
					Si20 = 1;											// 生成時形状:ON
				}
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				if(Si11) {												// <通常曲面>
					ExecSurface(Si13, Sop1, Si1, Si2, Si3, Si4,
                                m_ObjCRpt, m_ObjCWgt, Si5, Spa, Sia,
                                Spb, &Sop2);							// 通常曲面実行
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);	// 制御点列確認
				} else {												// <掃引曲面>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si12, Sps, Sis, Spa,
                                  Spb, &Sop2);							// 掃引曲面実行
				}
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i], Siw[i] = 0;						// ﾄﾗｯｶｰ保存/指定ﾌﾗｸﾞｸﾘｱ
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// 制御点対象ﾌﾗｸﾞｸﾘｱ
				}
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
						Tracker2.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ2範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				SetNumDisp(2);											// 数値表示初期処理
				if(Si14==0) {											// <通常ﾓｰﾄﾞの場合>
					Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);			// ﾄﾗｯｶｰ1設定
					Sphase = 1;											// 制御点指定へ
				}
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Line.Reset();													// ﾗｲﾝ解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		InitLineNo(NULL);												// ﾗｲﾝ番号初期化
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}

/******************************************************************/
void CJcad3GlbView::MouseLineInsert(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 制御点列挿入
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
				if(i1>0) {												// <制御点選択の場合>
					if(!CheckLineCopy(Si9, i1, Siy, Si1, Si2, Si3,
                                      Si4, Si7, &Si11, &m_SecBase4)) {	// 制御点列複写ﾁｪｯｸ
						PickLineInsert(Si9, i1, Siy, &Si1, &Si2, Si3,
                                       Si4, &Si5, Si6, m_CurInp, Si10,
                                       &Si11, m_SecBase4, Sop2, Spnt1,
                                       Spa, Spb, Sps, Sia, Sis);		// 制御点列挿入 列選択
						Si23 = 1;
						ObjectShading(Sop1);							// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					}
					TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11,
                                   Spb, &Si8, Spw, Sib);				// 点列ﾄﾗｯｶｰ取得
					Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);			// ﾄﾗｯｶｰ1設定
				}
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, 0,
                        &Si22, &i1, Siy);								// 全ﾄﾗｯｶｰﾁｪｯｸ2
		if(Si22) {
			return;
		}
		if(i1>0) {														// <ﾄﾗｯｶｰ指定あり>
			if(!CheckLineCopy(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                              Si7, &Si11, &m_SecBase4)) {				// 制御点列複写ﾁｪｯｸ
				PickLineInsert(Si9, i1, Siy, &Si1, &Si2, Si3, Si4, &Si5,
                               Si6, m_CurInp, Si10, &Si11, m_SecBase4,
                               Sop2, Spnt1, Spa, Spb, Sps, Sia, Sis);	// 制御点列挿入 列選択
				Si23 = 1;
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			}
			TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                           &Si8, Spw, Sib);								// 点列ﾄﾗｯｶｰ取得
			Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);					// ﾄﾗｯｶｰ1設定
		}
		Display(0);														// 再描画
	} else if(ms==11) {													// <<<DEL(削除)>>>
		Si11 = -1;														// 複写断面Noﾘｾｯﾄ
		TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                       &Si8, Spw, Sib);									// 点列ﾄﾗｯｶｰ取得
		Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);						// ﾄﾗｯｶｰ1設定
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Sop1 = NULL, Sop2 = NULL;
			Si11 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
                BaseCmdPtr(1, Sop1, &cmp);                              // 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==2&&cmp!=NULL) {									// <曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd2(cmp, i3, &Si9, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si10, Sps, Sis);							// 曲面ｺﾏﾝﾄﾞ取得2
				if(!Si9&&!Si20) {										// 掃引曲面&生成時:OFFは
					AfxMessageBox(IDS_SWEEPSURF);						// ﾒｯｾｰｼﾞ
					Si20 = 1;											// 生成時形状:ON
				}
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				if(Si9) {												// <通常曲面>
					ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);	// 通常曲面実行
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);	// 制御点列確認
				} else {												// <掃引曲面>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si10, Sps, Sis, Spa,
                                  Spb, &Sop2);							// 掃引曲面実行
				}
				TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                               &Si8, Spw, Sib);							// 点列ﾄﾗｯｶｰ取得
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);				// ﾄﾗｯｶｰ1設定
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				Display(0);												// 再描画
				if(Si6==0) {											// <U収束なし>
					CalcCent(Spa, Si5, &Spnt1);
				} else if(Si6==1) {										// <U始点収束>
					Spnt1 = Spa[0];
				} else if(Si6==2) {										// <U終点収束>
					Spnt1 = Spa[Si5-1];
				} else {												// <U始点･終点収束>
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] = (Spa[0].p[i]+Spa[Si5-1].p[i])/2.0;
					}
				}
				Sphase = 1;
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		if(ms==4) {														// <ﾏｳｽ右釦押下>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, 0,
	                        &Si22, &i1, Siy);							// 全ﾄﾗｯｶｰﾁｪｯｸ2
			if(i1==1) {													// <有効な切断位置の場合>
				if(AfxMessageBox(IDS_DIVIDE_CTRLP, MB_YESNO)!=IDYES) {	// 確認:NG
					i1 = 0;
				}
			}
			if(i1==1) {													// <ﾄﾗｯｶｰ指定あり>
				Si23 = (PickLineDivide(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                                       m_ObjCRpt, m_ObjCWgt, Si10, Sop1,
                                       Spa, Sps, Sia, Sis)) ? 1 : Si23;	// 制御点列分割
			}
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}

/******************************************************************/
void CJcad3GlbView::MouseLineRotate(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 制御点列回転
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt[2];
	VECTYPE  vec;
	BOXTYPE  box;
	CPoint   pot;
	int      i, i1, i2, i3, cl[2], ot[2], gno[32];
	double   th;

	if((ms==2||ms==3)&&Si22) {											// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==1) {											// 基準点角度
			if(abs(Si11-point.x)>DRAGMIN||abs(Si12-point.y)>DRAGMIN) {
				th = atan2((double)(point.y-Si12),
                           (double)(point.x-Si11));
				th = (th<-10||th>10) ? 0.0 : th;
				Sd1 = th*180.0/PI, Sphase = 2;
			}
		} else if(Sphase>=2) {											// 現在ｶｰｿﾙ位置角度
			th = atan2((double)(point.y-Si12), (double)(point.x-Si11));
			th = (th<-10||th>10) ? 0.0 : th;
			th = th*180.0/PI;											// 回転角度
			th = th-Sd1;
			th = (th>180.0) ? -(360.0-th) : ((th<-180.0)?360.0+th:th);
			TwistCLine(Spy, Sva, th, Si10, Sib, Si8, Sop1, Si1,
                       Si2, Si3, Si7, Spa, Spb, Spc, Tens);				// 制御点ひねり
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// 点列ﾄﾗｯｶｰ取得
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// 指示済み
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);					// ﾄﾗｯｶｰ1設定
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			m_pNumDisp->SetParam(th);									// ひねり角度表示
			Display(0);													// 再描画
			Sphase = 3;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
				if(i1>0) {												// <制御点選択の場合>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// 制御点列回転 列選択
					if(Si10!=-1) {										// 断面指示済みあり
						Sbase = Spy[Si10];								// 指定断面中心点
						for(i=0; i<DIM; i++) {
							Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;	// 回転用ﾄﾗｯｶｰ
						}
						Siw[Si8] = 0, Si9 = 1, Si23 = 1;
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[2]=Sbase.p[2];
						cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
						Tracker2.Setup(pt, cl, ot, 2, 0, 1);			// ﾄﾗｯｶｰ2設定
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[0]=Sbase.p[0];
						Tracker3.Setup(pt, cl, ot, 2, 1, 1);			// ﾄﾗｯｶｰ3設定
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[1]=Sbase.p[1];
						Tracker4.Setup(pt, cl, ot, 2, 2, 1);			// ﾄﾗｯｶｰ4設定
					} else {											// 断面指示済みなし
						Si9 = 0, Si10 = -1;
						Tracker2.Reset(0);								// ﾄﾗｯｶｰ2解除
						Tracker3.Reset(0);								// ﾄﾗｯｶｰ3解除
						Tracker4.Reset(0);								// ﾄﾗｯｶｰ4解除
					}
					Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);		// ﾄﾗｯｶｰ1設定
				}
			}
		} else if(Sphase==3) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// 正式ﾊﾞｯﾌｧ変更
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);			// 基準点取得
			}
			Sbase = Spy[Si10];											// 指定断面中心点
			for(i=0; i<DIM; i++) {
				Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;				// 回転用ﾄﾗｯｶｰ
			}
			Siw[Si8] = 0, Si9 = 1;
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[2] = Sbase.p[2];
			cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
			Tracker2.Setup(pt, cl, ot, 2, 0, 1);						// ﾄﾗｯｶｰ2設定
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[0] = Sbase.p[0];
			Tracker3.Setup(pt, cl, ot, 2, 1, 1);						// ﾄﾗｯｶｰ3設定
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[1] = Sbase.p[1];
			Tracker4.Setup(pt, cl, ot, 2, 2, 1);						// ﾄﾗｯｶｰ4設定
		}
		Sphase = 0;
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, Si9,
                        &Si22, &i1, Siy);								// 全ﾄﾗｯｶｰﾁｪｯｸ2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <断面ﾄﾗｯｶｰ指定あり>
			if(!m_CtlInp) {
				for(i=0;i<Si8;i++) {
					Sib[i] = 0, Siw[i] = 2;								// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// 制御点列回転 列選択
			if(Si10!=-1) {												// 断面指示済みあり
				Sbase = Spy[Si10];										// 指定断面中心点
				for(i=0; i<DIM; i++) {
					Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;			// 回転用ﾄﾗｯｶｰ
				}
				Siw[Si8] = 0, Si9 = 1, Si23 = 1;
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[2] = Sbase.p[2];
				cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
				Tracker2.Setup(pt, cl, ot, 2, 0, 1);					// ﾄﾗｯｶｰ2設定
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[0] = Sbase.p[0];
				Tracker3.Setup(pt, cl, ot, 2, 1, 1);					// ﾄﾗｯｶｰ3設定
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[1] = Sbase.p[1];
				Tracker4.Setup(pt, cl, ot, 2, 2, 1);					// ﾄﾗｯｶｰ4設定
			} else {													// 断面指示済みなし
				Si9 = 0, Si10 = -1;
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ4解除
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
		} else if(i1==1) {												// <回転ﾄﾗｯｶｰ指定あり>
			i = (plane-1<0) ? 2 : plane-1;								// 法線方向座標
			vec.p[0]=0.0, vec.p[1]=0.0, vec.p[2]=0.0, vec.p[i]=-1.0;	// 法線ﾍﾞｸﾄﾙ設定
			for(i=0; i<Si1; i++) {
				Sva[i] = vec;
			}
			if(m_NumInp!=0) {											// <数値入力の場合>
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
				if(plane==0) {
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Tracker4.Reset(0);									// ﾄﾗｯｶｰ4解除
				} else if(plane==1) {
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker4.Reset(0);									// ﾄﾗｯｶｰ4解除
				} else {
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
				}
				Display(0);												// 再描画
				CInputBtnDlg3 dlg(5);
				dlg.SetParam2(Si1, Si2, Si3, Si4, Si7, Si10, Sib,
                              Si8, Sop1, Spy, Sva, Spa, Sia,
                              Spb, Spc, Tens, m_LRotateAdd);            // ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_LRotateAdd);							// ﾊﾟﾗﾒｰﾀ取得
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// 正式ﾊﾞｯﾌｧ変更
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// 基準点取得
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si9 = 0, Si10 = -1;
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ4解除
			} else {													// <ﾏｳｽ入力の場合>
				CalcWorldScreen(&pot, Sbase, plane);
				Si11 = pot.x, Si12 = pot.y;								// 中心点ｽｸﾘｰﾝ座標保存
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ4解除
				Sphase = 1;
			}
		} else if(!m_CtlInp) {											// <断面指定無の場合>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// 指定ﾌﾗｸﾞｸﾘｱ
			}
			Si9 = 0, Si10 = -1;
			InitLineNo(Sop2);											// ﾗｲﾝ番号初期化
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
			Tracker3.Reset(0);											// ﾄﾗｯｶｰ3解除
			Tracker4.Reset(0);											// ﾄﾗｯｶｰ4解除
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <通常曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// 制御点列確認
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// 基準点取得
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// ﾄﾗｯｶｰ1設定
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指示ﾌﾗｸﾞｸﾘｱ
				}
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				SetNumDisp(2);											// 数値表示初期処理
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Tracker4.Reset(1);												// ﾄﾗｯｶｰ4解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		InitLineNo(NULL);												// ﾗｲﾝ番号初期化
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}

/*****************************************************************/
void CJcad3GlbView::MouseLineTwist(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< ひねり
/*****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   th, l11, l12, l21, l22;

	if((ms==2||ms==3)&&Si22) {											// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==1) {											// ひねり角度算出
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			i1 = plane;													// 更新対象座標
			i2 = (i1+1>2) ? 0 : i1+1;
			l11 = Sbase.p[i1]-Spy[Si10].p[i1];							// 角度算出
			l12 = Sbase.p[i2]-Spy[Si10].p[i2];
			l21 = pnt.p[i1]-Spy[Si10].p[i1];
			l22 = pnt.p[i2]-Spy[Si10].p[i2];
			l11 = atan2(l12, l11);
			l11 = (l11<-10||l11>10) ? 0.0 : l11;
			l11 = l11*180.0/PI;
			l21 = atan2(l22, l21);
			l21 = (l21<-10||l21>10) ? 0.0 : l21;
			l21 = l21*180.0/PI;
			th = -l21+l11;
			Sd2 = (th-Sd1<-180.0)?Sd2+360:((th-Sd1>180.0)?Sd2-360:Sd2);	// 連続回転補正値修正
			Sd1 = th;													// 今回回転角度保存
			th += Sd2;													// 補正値加算
			TwistCLine(Spy, Sva, th, Si10, Sib, Si8, Sop1, Si1,
                       Si2, Si3, Si7, Spa, Spb, Spc, Tens);				// 制御点ひねり
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// 点列ﾄﾗｯｶｰ取得
			i1 = (Si7%2) ? Si10-1 : Si10;
			TrackerVLine(Si1, Si2, Spd, Si7, i1, Si8, &Si9, Spw, Siw);	// 点列ﾄﾗｯｶｰ取得
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// 指示済み
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			m_pNumDisp->SetParam(th);									// ひねり角度表示
			Display(0);													// 再描画
			Si11 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
				if(i1>0) {												// <制御点選択の場合>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// 制御点列回転 列選択
					if(Si10!=-1) {										// 断面指示済みあり
						i2 = (Si7%2) ? Si10-1 : Si10;
						TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                                     &Si9, Spw, Siw);					// 点列ﾄﾗｯｶｰ取得
						Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);	// ﾄﾗｯｶｰ1設定
						Si23 = 1;
					} else {											// 断面指示済みなし
						Si9 = 0, Si10 = -1;
					}
				}
			}
		} else if(Sphase==1&&Si11==1) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// 正式ﾊﾞｯﾌｧ変更
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);		// 基準点取得
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8,
                        Si9, &Si22, &i1, Siy);							// 全ﾄﾗｯｶｰﾁｪｯｸ2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <断面ﾄﾗｯｶｰ指定あり>
			if(!m_CtlInp) {
				for(i=0;i<Si8;i++) {
					Sib[i] = 0, Siw[i] = 2;								// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// 制御点列回転 列選択
			if(Si10!=-1) {												// 断面指示済みあり
				i2 = (Si7%2) ? Si10-1 : Si10;
				TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                             &Si9, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// ﾄﾗｯｶｰ1設定
				Si23 = 1;
			} else {													// 断面指示済みなし
				Si9 = 0, Si10 = -1;
			}
		} else if(i1==1) {												// <回転ﾄﾗｯｶｰ指定あり>
			if(m_NumInp!=0) {											// <数値入力の場合>
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
				Display(0);												// 再描画
				CInputBtnDlg3 dlg(2);
				dlg.SetParam2(Si1, Si2, Si3, Si4, Si7, Si10, Sib,
                              Si8, Sop1, Spy, Sva, Spa, Sia,
                              Spb, Spc, Tens, m_LTwistAdd);             // ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_LTwistAdd);								// ﾊﾟﾗﾒｰﾀ取得
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// 正式ﾊﾞｯﾌｧ変更
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// 基準点取得
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si9 = 0, Si10 = -1;
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// ﾄﾗｯｶｰ1設定
			} else {													// <ﾏｳｽ入力の場合>
				Sbase = Spw[Siy[0]];									// V座標位置保存
				Sphase = 1, Si11 = 0, Sd1 = 0.0, Sd2 = 0.0;
			}
		} else if(!m_CtlInp) {											// <断面指定無の場合>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// 指定ﾌﾗｸﾞｸﾘｱ
			}
			Si9 = 0, Si10 = -1;
			InitLineNo(Sop2);											// ﾗｲﾝ番号初期化
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <通常曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// 制御点列確認
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// 基準点取得
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// ﾄﾗｯｶｰ1設定
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指示ﾌﾗｸﾞｸﾘｱ
				}
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				SetNumDisp(2);											// 数値表示初期処理
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		InitLineNo(NULL);												// ﾗｲﾝ番号初期化
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}

/*****************************************************************/
void CJcad3GlbView::MouseLineScale(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< 拡大/縮小
/*****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   scl, l11, l12, l21, l22;

	if((ms==2||ms==3)&&Si22) {											// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ左釦解放>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==1) {											// 倍率算出
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			i1 = plane;													// 更新対象座標
			i2 = (i1+1>2) ? 0 : i1+1;
			l11 = Sbase.p[i1]-Spy[Si10].p[i1];							// 倍率算出
			l12 = Sbase.p[i2]-Spy[Si10].p[i2];
			l21 = pnt.p[i1]-Spy[Si10].p[i1];
			l22 = pnt.p[i2]-Spy[Si10].p[i2];
			l11 = sqrt(l11*l11+l12*l12);
			l21 = sqrt(l21*l21+l22*l22);
			scl = l21/l11;
            ScaleCLine(Spy, Sva, m_SecBase1a, m_SecBase1b, scl,
                       Si10, Sib, Si8, Sop1, Si1, Si2,
                       Si3, Si7, Spa, Spb, Spc, Tens);                  // 制御点拡大/縮小
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// 点列ﾄﾗｯｶｰ取得
			i1 = (Si7%2) ? Si10-1 : Si10;
			TrackerVLine(Si1, Si2, Spd, Si7, i1, Si8, &Si9, Spw, Siw);	// 点列ﾄﾗｯｶｰ取得
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// 指示済み
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			m_pNumDisp->SetParam(scl);									// 拡大/縮小倍率表示
			Display(0);													// 再描画
			Si13 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
				if(i1>0) {												// <制御点選択の場合>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// 制御点列回転 列選択
					if(Si10!=-1) {										// 断面指示済みあり
						if(!Si14) {
                            Si14 = InitCScale(&m_SecBase1a,
                                              &m_SecBase1b);            // 拡大/縮小初期処理
						}
						if(Si14) {										// <初期処理有効>
							Si23 = 1;									// 処理済
							i2 = (Si7%2) ? Si10-1 : Si10;
							TrackerVLine(Si1, Si2, Spb, Si7, i2,
                                         Si8, &Si9, Spw, Siw);			// 点列ﾄﾗｯｶｰ取得
							Tracker1.Setup(Spw, Siw, NULL,
                                           Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
							Display(0);									// 再描画
						} else {										// 無効は断面未指示
							Si9 = 0, Si10 = -1;
						}
					} else {											// 断面指示済なし
						Si9 = 0, Si10 = -1, Si14 = 0;
					}
				}
			}
		} else if(Sphase==1&&Si13==1) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// 正式ﾊﾞｯﾌｧ変更
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);		// 基準点取得
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8,
                        Si9, &Si22, &i1, Siy);							// 全ﾄﾗｯｶｰﾁｪｯｸ2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <断面ﾄﾗｯｶｰ指定あり>
			if(!m_CtlInp) {
				for(i=0; i<Si8; i++) {
					Sib[i] = 0, Siw[i] = 2;								// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// 制御点列回転 列選択
			if(Si10!=-1) {												// 断面指示済みあり
				if(!Si14) {
                    Si14 = InitCScale(&m_SecBase1a, &m_SecBase1b);      // 拡大/縮小初期処理
				}
				if(Si14) {												// <初期処理有効>
					Si23 = 1;											// 処理済
					i2 = (Si7%2) ? Si10-1 : Si10;
					TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                                 &Si9, Spw, Siw);						// 点列ﾄﾗｯｶｰ取得
					Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);		// ﾄﾗｯｶｰ1設定
					Display(0);											// 再描画
				} else {												// 無効は断面未指示
					Si9 = 0, Si10 = -1;
				}
			} else {													// 断面指示済なし
				Si9 = 0, Si10 = -1, Si14 = 0;
			}
		} else if(i1==1) {												// <回転ﾄﾗｯｶｰ指定あり>
			if(m_NumInp!=0) {											// <数値入力の場合>
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
				Display(0);												// 再描画
                CInputBtnDlg3 dlg(3);
                dlg.SetParam3(Si1, Si2, Si3, Si4, Si7, m_SecBase1a,
                              m_SecBase1b, Si10, Sib, Si8, Sop1,
                              Spy, Sva, Spa, Sia, Spb,
                              Spc, Tens, m_LScaleAdd);                  // ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_LScaleAdd);								// ﾊﾟﾗﾒｰﾀ取得
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// 正式ﾊﾞｯﾌｧ変更
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// 基準点取得
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si9 = 0, Si10 = -1, Si14 = 0;
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// ﾄﾗｯｶｰ1設定
			} else {													// <ﾏｳｽ入力の場合>
				Sbase = Spw[Siy[0]];									// V座標位置保存
				Sphase = 1, Si13 = 0;
			}
		} else if(!m_CtlInp) {											// <断面指定無の場合>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// 指定ﾌﾗｸﾞｸﾘｱ
			}
			Si9 = 0, Si10 = -1, Si14 = 0;
			InitLineNo(Sop2);											// ﾗｲﾝ番号初期化
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// ﾄﾗｯｶｰ1設定
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si14 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <通常曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// 制御点列確認
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// 基準点取得
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(Sop2)) {								// ﾄﾗｯｶｰ範囲限定設定
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// ﾄﾗｯｶｰ1範囲限定
					} else {
						Si24 = 0;										// ｷｬﾝｾﾙは範囲限定中止
					}
				} else {												// <範囲限定なし>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
				}
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// ﾄﾗｯｶｰ1設定
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// 指示ﾌﾗｸﾞｸﾘｱ
				}
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				SetNumDisp(2);											// 数値表示初期処理
				Display(0);												// 再描画
				m_pNumDisp->SetParam(1.0);								// 拡大/縮小倍率表示
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si20) {
			AttachObj(Sop1);											// ｺﾏﾝﾄﾞ再接続/再実行
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		SetRenderMode(0);												// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		InitLineNo(NULL);												// ﾗｲﾝ番号初期化
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(!Si23) {
			Undo(gno);													// 編集なしはUndo
		}
		Display(0);														// 再描画
	}
}
