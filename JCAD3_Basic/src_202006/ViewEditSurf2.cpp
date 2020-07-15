/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewEditSurf2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlTracker.h"
#include "FaceEdit.h"
#include "Symmetry.h"
#include "ConvSCurve.h"

/******************************************************************/
void CJcad3GlbView::MouseSPointMove(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 断面制御点移動
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

	if(Sphase==1&&(ms==2||ms==3)&&Si22) {								// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	} else if(Sphase!=1&&ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {
		EyeChange(flg, point, plane, ms);
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase>0&&Sphase<3&&plane!=3) {				// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==3) {											// 制御点移動
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			GetMoveValue(plane, Sbase, &pnt);							// ﾄﾗｯｶｰ移動量取得
			MoveSymmetryArea(Sbase, pnt, Si15, Spnt4, 0, Si16, Si17, -1,
                             -1, Si21, 0, Si2, Six, Spz, Spy);			// 対称領域移動量設定
			MoveSCurve(Si14, Sdb, Spy, Sop3, Si2, Si4, Spz, Spw,
                       Six, Spx, TensC);								// 断面制御点移動
			RedoCrtCurve(Si2, Si4, Spx, Siz);							// 曲線生成
			CalcCurvePoint(Si14, Si2, Spx, Spd, Sda);					// 制御点=>通過点変換
			RedoSetCurve(Si2, Si4, Spd);								// 曲線制御点生成
			for(i=0; i<Si2; i++) {
				Siw[i] = (Six[i])?4:Siz[i]*2;							// ﾄﾗｯｶｰ色設定
			}
			Tracker1.Setup(Spd, Siw, NULL, Si2, Si11, 0);				// ﾄﾗｯｶｰ1設定
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// 移動前点との距離
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Si9 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0&&Si16>-1) {										// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);			// ﾄﾗｯｶｰ1設定
				Sphase = 2;												// 制御点指定へ
			}
		} else if(Sphase<3) {											// <ﾄﾞﾗｯｸﾞ選択の場合>
			i1 = 0;
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt, i1 = 1;
			}
		}
		if(Sphase==1&&i1==1) {											// <断面番号指定>
			DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);			// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
			if(i1>0) {													// 断面選択あり
				Si23 = 1;
				Si15 = (m_FChar&0x8) ? 2 : 0;							// [F8]:入力ﾓｰﾄﾞ:2軸対称
				Si15 = (m_FChar&0x4) ? 1 : Si15;						// [F7]:入力ﾓｰﾄﾞ:1軸対称
				m_FChar = m_FChar&~(0x4);								// [F7]OFF
				m_FChar = m_FChar&~(0x8);								// [F8]OFF
				Si10 = Siy[0];											// U断面位置保存
				PickSPointMove1(Si10, Si14, Sda, Sdb, Si12, Si19, Si1,
                                Si2, Si4, m_ObjCRpt, m_ObjCWgt, Si7,
                                Si13, &Si11, Spb, Sps, Spz, Spw, &Spnt1,
                                &Svec1, &Spnt3, Sia, Sis, Siz, Siw, Six,
                                Tens, TensC, Sop1, Sop2, &Sop3, &Sop4);	// 断面制御点移動 列選択
				if(Si15==0) {											// <通常ﾓｰﾄﾞの場合>
					Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);		// ﾄﾗｯｶｰ1設定
					Sphase = 2;											// 制御点指定へ
				} else {												// <対称ﾓｰﾄﾞの場合>
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Sphase = 0, Si16 = -1;								// 対称軸指定へ
				}
			}
		} else if(Sphase==2&&plane==Si11&&i1==1) {						// <断面制御点指定>
			DragCtrlTracker2(plane, box, Spw, Si2, &i1, Siy);			// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ2
			if(i1>0) {													// 制御点選択あり
				PickSPointMove2(i1, Siy, Si14, Sda, Sdb, Si2, Si4,
                                m_ObjCWgt, m_CurInp, Spz, Spw,
                                Siz, Siw, Six);							// 断面制御点移動 点選択
				Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);			// ﾄﾗｯｶｰ1設定
			}
		} else if(Sphase==3&&Si9==1) {
			for(i=0; i<Si2; i++) {
				Spz[i] = Spx[i], Spw[i] = Spd[i];						// 正式ﾊﾞｯﾌｧ変更
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
		Sphase = (Sphase==3) ? 2 : Sphase;
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				CalcScreenWorld(0, &Spnt4, point, plane);				// 対称軸中心点取得
				i1 = plane;												// 対称軸指定平面
				i2 = (flg&MK_CONTROL) ? 1 : 0;							// 横方向指定
				Si21 = (flg&MK_SHIFT) ? 1 : 0;							// 軸方向反転
				SetSymmetryLine(1, Si15, Spnt4, i1, i2, 0,
                                &Si16, &Si17);							// 対称軸設定
			}
		} else {														// <制御点指定の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(Sphase==1) {												// <<断面番号指定>>
				HitCtrlTracker2(flg, point, plane, ms, pnt, Spw,
                                Si8, 0, &Si22, &i1, Siy);				// 全ﾄﾗｯｶｰﾁｪｯｸ2
				if(Si22) {
					return;
				}
				if(i1>0) {												// <断面ﾄﾗｯｶｰ指定あり>
					Si23 = 1;
					Si15 = (m_FChar&0x8) ? 2 : 0;						// [F8]:入力ﾓｰﾄﾞ:2軸対称
					Si15 = (m_FChar&0x4) ? 1 : Si15;					// [F7]:入力ﾓｰﾄﾞ:1軸対称
					m_FChar = m_FChar&~(0x4);							// [F7]OFF
					m_FChar = m_FChar&~(0x8);							// [F8]OFF
					Si10 = Siy[0];										// U断面位置保存
					PickSPointMove1(Si10, Si14, Sda, Sdb, Si12, Si19,
                                    Si1, Si2, Si4, m_ObjCRpt, m_ObjCWgt,
                                    Si7, Si13, &Si11, Spb, Sps, Spz,
                                    Spw, &Spnt1, &Svec1, &Spnt3, Sia,
                                    Sis, Siz, Siw, Six, Tens, TensC,
                                    Sop1, Sop2, &Sop3, &Sop4);			// 断面制御点移動 列選択
					if(Si15==0) {										// <通常ﾓｰﾄﾞの場合>
						Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);	// ﾄﾗｯｶｰ1設定
						Sphase = 2;										// 制御点指定へ
					} else {											// <対称ﾓｰﾄﾞの場合>
						Tracker1.Reset(0);								// ﾄﾗｯｶｰ1解除
						Sphase = 0, Si16 = -1;							// 対称軸指定へ
					}
				}
			} else if(Sphase==2&&plane==Si11) {							// <<断面制御点指定>>
				i1 = Tracker1.HitTracker(1, Si11, pnt, Spw, Si2);		// ﾄﾗｯｶｰ1ﾁｪｯｸ
				if(i1!=-1) {
					if(!m_CurInp&&!m_CtlInp&&Six[i1]==1) {				// <制御点移動の場合>
						Sbase = Spw[i1];								// 基準点保存
						Sphase = 3;
						Si9 = 0;
						if(m_NumInp!=0) {								// <数値入力の場合>
							Sphase = 2;
							CInputBtnDlg6 dlg(4);
							dlg.SetParam4(Sop3, Si14, Si2, Si4, Si11,
                                          &Si9, Si15, Si16, Si17, Si21,
                                          Sbase, Spnt4, Spz, Spx, Spw,
                                          Spd, Spy, Siz, Six, Siw, TensC,
                                          Sda, Sdb, m_SMoveAdd);        // ﾊﾟﾗﾒｰﾀ設定4
							if(dlg.DoModal()==IDOK) {					// 入力ﾀﾞｲｱﾛｸﾞ表示
								if(Si9==1) {							// 移動済みの場合は
									for(i=0; i<Si2; i++) {
										Spz[i] = Spx[i];				// 正式ﾊﾞｯﾌｧ変更
										Spw[i] = Spd[i];
									}
								}
							}
							dlg.GetParam(&m_SMoveAdd);					// ﾊﾟﾗﾒｰﾀ取得
							RedoCrtCurve(Si2, Si4, Spz, Siz);			// 曲線生成
							CalcCurvePoint(Si14, Si2, Spz, Spw, Sda);	// 制御点=>通過点変換
							RedoSetCurve(Si2, Si4, Spw);				// 曲線制御点生成
							for(i=0; i<Si2; i++) {
								Siw[i] = (Six[i])?4:Siz[i]*2;			// ﾄﾗｯｶｰ色設定
							}
							Tracker1.Setup(Spw, Siw, NULL, Si2, Si11,0);// ﾄﾗｯｶｰ1設定
						}
					} else {											// <制御点選択の場合>
						if(!m_CtlInp){
							for(i=0; i<Si2; i++) {
								Six[i] = 0;								// 全制御点選択解除
							}
						}
						Siy[0] = i1;
						PickSPointMove2(1, Siy, Si14, Sda, Sdb, Si2,
                                        Si4, m_ObjCWgt, m_CurInp, Spz,
                                        Spw, Siz, Siw, Six);			// 断面制御点移動 点選択
						Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);	// ﾄﾗｯｶｰ1設定
					}
				} else if(!m_CtlInp) {									// <<ﾄﾗｯｶｰ選択無>>
					for(i=0; i<Si2; i++) {
						Siw[i] = Siz[i]*2, Six[i] = 0;					// ﾄﾗｯｶｰ/指示ﾌﾗｸﾞ設定
					}
					Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);		// ﾄﾗｯｶｰ1設定
				}
			}
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL, Sop3 = NULL, Sop4 = NULL;
			Scmp = NULL;
			Si6 = 0, Si7 = 0, Si16 = -1, Si18 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			m_Shading = m_ShdEnv;										// ｼｪﾃﾞｨﾝｸﾞ表示状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:ｼｪﾃﾞｨﾝｸﾞ表示反転
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si19 = (m_FChar&0x4) ? 1 : 0;								// [F7]:断面時立体消去
			Si14 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			m_FChar = m_FChar&~(0x4);									// [F7]OFF
			m_FChar = m_FChar&~(0x8);									// [F8]OFF
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==2&&cmp!=NULL) {									// <曲面の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				if(Si19) {
					EraseObjSPointMove(&Si18, Sic, Sop1);				// 他立体消去
				}
				GetSurfCmd2(cmp, i3, &Si12, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si13, Sps, Sis);							// 曲面ｺﾏﾝﾄﾞ取得2
				if(!Si12&&Si14) {										// 掃引曲面&通過点ﾓｰﾄﾞは
					AfxMessageBox(IDS_ERR_SWEEP);						// ﾒｯｾｰｼﾞ
					Si14 = 0;											// 掃引曲面は制御点ﾓｰﾄﾞ
				}
				if(!Si12&&!Si20) {										// 掃引曲面&生成時:OFFは
					AfxMessageBox(IDS_SWEEPSURF);						// ﾒｯｾｰｼﾞ
					Si20 = 1;											// 生成時形状:ON
				}
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ｺﾏﾝﾄﾞ一時切離
				}
				if(Si12) {												// <通常曲面>
					ExecSurface(Si14, Sop1, Si1, Si2, Si3, Si4,
                                m_ObjCRpt, m_ObjCWgt, Si5, Spa, Sia,
                                Spb, &Sop2);							// 通常曲面実行
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);	// 制御点列確認
				} else {												// <掃引曲面>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si13, Sps, Sis, Spa,
                                  Spb, &Sop2);							// 掃引曲面実行
				}
				BaseCmdPtr(1, Sop1, &Scmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				TrackerULine(Si14, Si1, Si2, Spb, Si6, Si7,
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
				ObjectShading(Sop1);									// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				OffUndoRedo();											// Undo/Redo不可
				SetNumDisp(2);											// 数値表示初期処理
				Display(0);												// 再描画
				Sphase = 1;
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==2) {													// <<断面曲線編集中>>
			ChangeSPointMove(Si12, Si14, Si19, Si20, Si10, Si1, Si2,
                             Si3, Si4, m_ObjCWgt, Si5, Si7, Si13,
                             Spnt1, Svec1, Spnt3, Spa,Spb, Spc, Sps,
                             Spz, Sia, Sis, Siz, Tens, Scmp, Sop1,
                             Sop2, Sop3, Sop4);							// 断面制御点移動 断面変更
			TrackerULine(Si14, Si1, Si2, Spb, Si6, Si7, &Si8, Spw, Siw);// 点列ﾄﾗｯｶｰ取得
			Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);					// ﾄﾗｯｶｰ1設定
			Line.Reset();												// ﾗｲﾝ解除
			ObjectShading(Sop1);										// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			Display(0);													// 再描画
			Sphase = 1, i3 = 1;
		} else {
			i3 = 0;
		}
		if(i3==0||ms==9) {												// <<上記以外/強制終了>>
			if(Si20) {
				AttachObj(Sop1);										// ｺﾏﾝﾄﾞ再接続/再実行
			}
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// 立体削除
			}
			if(Sop3!=NULL) {
				FreeObj(Sop3, 1);										// 立体削除
			}
			if(Sop4!=NULL) {
				FreeObj(Sop4, 1);										// 立体削除
			}
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
			SetRenderMode(0);											// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			OnUndoRedo();												// Undo/Redo可
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
			if(Si19) {
				DispObjSPointMove(Si18, Sic, Sop1);						// 他立体表示
			}
			if(!Si23) {
				Undo(gno);												// 編集なしはUndo
			}
			Display(0);													// 再描画
		}
	}
}

/******************************************************************/
void CJcad3GlbView::MouseBPointMove(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< ﾍﾞｰｽ線移動
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	VECTYPE  vec;
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
			MoveSymmetryArea(Spnt1, pnt, Si12, Spnt2, 0, Si15, Si16,
                             -1, -1, Si14, 0, Si8, Sib, Spz, Sps);		// 対称領域移動量設定
			MoveBPoint(Si11, Spy, Sps, Sop1, Si1, Si2, Si3,
                       Si4, m_ObjCWgt, Si7, Si8, Sia,
                       Sib, Spa, Spb, Spc, Tens);						// ﾍﾞｰｽ線移動
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// 曲面生成
			CalcSurfacePoint(Si11, 1, 0, Si1, Si2, Si3, Si4, m_ObjCWgt,
                             Sia, Spc, Spd);							// 制御点=>通過点変換
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// 曲面制御点生成
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			TrackerULine(Si11, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);// 点列ﾄﾗｯｶｰ取得
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// 指定点色変更
			}
			DispParsTracker(0, Si21, Spw, Siw, Si8, Spx, Six, Si10);	// 透視図ﾄﾗｯｶｰ表示
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
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
			}
		} else if(Sphase==1) {											// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spw, Si8, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(i1>0) {												// <制御点選択の場合>
					PickBPointMove(i1, Siy, Si7, Si8, Sop2, Sib, Siw);	// ﾍﾞｰｽ線移動 列選択
					SetParsTracker(0, &Si21, 0, Sib, Spw, Siw, Si8,
                                   Spx, Six, &Si10);					// 透視図ﾄﾗｯｶｰ設定
					Sphase = 1, Si23 = 1;
				}
			}
		} else if(Sphase==2) {
			if(Si9==1) {												// <制御点移動の場合>
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// 正式ﾊﾞｯﾌｧ変更
				}
				for(i=0; i<Si8; i++) {
					Spz[i] = Spw[i];									// ﾄﾗｯｶｰ保存
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// 基準点取得
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
				Si13 = plane;											// 対称軸指定平面
				i = (flg&MK_CONTROL) ? 1 : 0;							// 横方向指定
				Si14 = (flg&MK_SHIFT) ? 1 : 0;							// 軸方向反転
				SetSymmetryLine(1, Si12, Spnt2, Si13, i, 0,
                                &Si15, &Si16);							// 対称軸設定
			}
		} else {														// <制御点指定の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si8, Spx,
                            Six, Si10, &Si21, &Si22, &i1, Siy);			// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <制御点移動の場合>
				Spnt1 = Spw[Siy[0]];									// 移動元点保存
				Sphase = 2, Si9 = 0;
				if(m_NumInp!=0) {										// <数値入力の場合>
					Sphase = 1;
					CInputBtnDlg6 dlg(5);
					dlg.SetParam5(Sop1, Sop2, Si11, Si21, Si1, Si2, Si3,
                                  Si4, m_ObjCWgt, Si5, Si6, Si7, Si8,
                                  Si10, &Si9, Si12, Si15, Si16, Si14,
                                  Spnt1, Spnt2, Spy, Spa, Spb, Spc, Spd,
                                  Sps, Spw, Spx, Spz, Sia, Sib, Siw,
                                  Six, Tens, m_BMoveAdd);				// ﾊﾟﾗﾒｰﾀ設定5
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						if(Si9==1) {									// 移動済みの場合は
							for(i=0; i<Si5; i++) {						// 正式ﾊﾞｯﾌｧ変更
								Spa[i] = Spc[i], Spb[i] = Spd[i];
								
							}
							for(i=0; i<Si1; i++) {
								GetBasePoint(Si1, Si2, Spb, i,
                                             &Spy[i], &vec);			// 基準点取得
							}
						}
					}
					dlg.GetParam(&m_BMoveAdd);							// ﾊﾟﾗﾒｰﾀ取得
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// 曲面生成
					CalcSurfacePoint(Si11, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// 制御点=>通過点変換
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// 曲面制御点生成
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					TrackerULine(Si11, Si1, Si2, Spb, Si6, Si7,
                                 &Si8, Spw, Siw);						// 点列ﾄﾗｯｶｰ取得
					for(i=0; i<Si8; i++) {
						Spz[i] = Spw[i];								// ﾄﾗｯｶｰ保存
						Siw[i] = (Sib[i]) ? 4 : Siw[i];					// 指定点色変更
					}
					DispParsTracker(0, Si21, Spw, Siw, Si8,
                                    Spx, Six, Si10);					// 透視図ﾄﾗｯｶｰ表示
					ObjectShading(Sop1);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
				}
			} else if(i1>0) {											// <制御点選択の場合>
				if(!m_CtlInp) {
					for(i=0;i<Si8;i++) {
						Sib[i] = 0, Siw[i] = 2;							// 指定ﾌﾗｸﾞｸﾘｱ
					}
				}
				PickBPointMove(i1, Siy, Si7, Si8, Sop2, Sib, Siw);		// ﾍﾞｰｽ線移動 列選択
				SetParsTracker(0, &Si21, 0, Sib, Spw, Siw, Si8,
                               Spx, Six, &Si10);						// 透視図ﾄﾗｯｶｰ設定
				Si23 = 1;
			} else if(!m_CtlInp) {										// <ﾄﾗｯｶｰ指定無で追加無>
				for(i=0; i<Si8; i++) {
					Sib[i] = 0, Siw[i] = 2;								// 指定ﾌﾗｸﾞｸﾘｱ
				}
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);				// ﾄﾗｯｶｰ1設定
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
			Si12 = (m_FChar&0x8) ? 2 : 0;								// [F8]:入力ﾓｰﾄﾞ:2軸対称
			Si12 = (m_FChar&0x4) ? 1 : Si12;							// [F7]:入力ﾓｰﾄﾞ:1軸対称
			Si11 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
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
				ExecSurface(Si11, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// 通常曲面実行
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// 制御点列確認
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// 基準点取得
				}
				TrackerULine(Si11, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// 点列ﾄﾗｯｶｰ取得
				for(i=0; i<Si8; i++) {
					Spz[i] = Spw[i], Sib[i] = 0;						// ﾄﾗｯｶｰ保存/指示ﾌﾗｸﾞｸﾘｱ
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
				if(Si12==0) {											// <通常ﾓｰﾄﾞの場合>
					Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);			// ﾄﾗｯｶｰ1設定
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

/******************************************************************/
void CJcad3GlbView::MouseLineAdjust(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 制御点曲線合わせ
/******************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	SCRTYPE  scr;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, ct1, ct2, er, un, vn, ub, vb;

	if((ms==2||ms==3)&&Si22) {											// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&plane!=3) {									// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(abs(BasePoint.x-point.x)>=DRAGMIN||
           abs(BasePoint.y-point.y)>=DRAGMIN) {
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			box.bp[0] = BaseCursor, box.bp[1] = pnt;
			DragCtrlTracker1(plane, box, Spx, Si6, Spy, Siy,
                             Si7, &Si21, &i1, Siz);						// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
			if(i1>0) {													// <制御点選択の場合>
				for(i=0; i<i1; i++) {
					i2 = Siz[i], pnt = Spx[i2];							// 選択ﾄﾗｯｶｰNo/座標
					Six[i2] = (Six[i2]==4) ? 0 : 4;						// 選択状況反転
					for(j=0; j<Si6; j++) {								// <指定点同一座標>
						if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                           fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                           fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
							Six[j] = (Six[j]==4) ? 0 : 4;				// 選択状況反転
						}
					}
				}
				SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                               Spy, Siy, &Si7);							// 透視図ﾄﾗｯｶｰ設定
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker1(flg, point, plane, ms, pnt, Spx, Si6,
                        Spy, Siy, Si7, &Si21, &Si22, &i1, Siz);			// 全ﾄﾗｯｶｰﾁｪｯｸ1
		if(Si22) {
			return;
		}
		if(i1>0) {														// <制御点選択の場合>
			if(!m_CtlInp) {
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// 全制御点選択解除
				}
			}
			for(i=0; i<i1; i++) {
				i2 = Siz[i], pnt = Spx[i2];								// 選択ﾄﾗｯｶｰNo/座標
				Six[i2] = (Six[i2]==4) ? 0 : 4;							// 選択状況反転
				for(j=0; j<Si6; j++) {									// <指定点同一座標>
					if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                       fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                       fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
						Six[j] = (Six[j]==4) ? 0 : 4;					// 選択状況反転
					}
				}
			}
			SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                           Spy, Siy, &Si7);								// 透視図ﾄﾗｯｶｰ設定
		} else if(!m_CtlInp) {											// <ﾄﾗｯｶｰ指定無で追加無>
			for(i=0; i<Si6; i++) {
				Six[i] = 0;												// 全制御点選択解除
			}
			Si21 = 0, Si7 = 0;											// 透視図選択解除
			Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);					// ﾄﾗｯｶｰ1設定
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(GetSelectNo()==0) {											// <<2D曲線OP無>>
			if(m_FChar&0x1) {											// [F5]:合せ方向(X)
				Si1 = 0;
			} else if(m_FChar&0x2) {									// [F6]:   〃   (Y)
				Si1 = 1;
			} else if(m_FChar&0x4) {									// [F7]:   〃   (Z)
				Si1 = 2;
			} else {													// 未指定(Z)
				Si1 = 2;
			}
			Si2 = (m_FChar&0x8) ? 0 : 1;								// [F8]:削除なし
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			ct1 = 0, er = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// 編集対象立体取得
			while(op!=NULL) {
				if(GetObjAtr(op)/10!=1) {								// 曲線以外はｴﾗｰ
					er = 1; break;
				}
				op1 = op;												// 曲線OP保存
				if(++ct1>1) {											// 曲線数ｶｳﾝﾄUP
					er = 1; break;
				}
				NextObjPtr(2, &op, &i1, &i2);							// 次編集立体へ
			}
			if(er==0) {													// <正常な場合>
				GetObjBox(op1, &box);									// BOXｻｲｽﾞ取得
				for(i=0, i1=0; i<DIM; i++) {
					scr.p[i] = 0;
					if(fabs(box.bp[0].p[i]-box.bp[1].p[i])>EPSILON) {	// 有効ｻｲｽﾞ方向検索
						scr.p[i] = 1;
						i1++;
					}
				}
				if(i1==2) {												// <有効ｻｲｽﾞ方向が2>
					if(scr.p[Si1]==0) {									// 合せ方向が無効はｴﾗｰ
						er = 3;
					} else {
						for(i=0; i<DIM; i++) {
							Si3 = (i!=Si1&&scr.p[i]==1) ? i : Si3;		// 位置確認方向
						}
					}
				} else if(i1==1) {										// <有効ｻｲｽﾞ方向が1>
					if(scr.p[Si1]==1) {									// 合せ方向が有効はｴﾗｰ
						er = 3;
					} else {
						for(i=0; i<DIM; i++) {
							Si3 = (scr.p[i]==1) ? i : Si3;				// 位置確認方向
						}
					}
				} else {												// 有効ｻｲｽﾞが全/0はｴﾗｰ
					er = 2;
				}
			}
			if(er==0) {													// <正常な場合>
				BaseCmdPtr(1, op1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				if(cmp!=NULL) {
					i1 = GetParaInt(cmp, 1);							// 曲線境界ﾓｰﾄﾞ取得
				} else {
					er = 1;
				}
			}
			if(er==0&&ct1==1) {											// <正常な場合>
				Si4 = m_SurfTension;									// 張力係数保存
				SetSelNo(op1, 1);										// 断面曲線立体登録
				Si5 = GetVtxNum(op1);									// 曲線頂点数
				for(i=0; i<Si5; i++) {
					GetVtx(op1, i, &Spw[i]);							// 基準曲線頂点取得
				}
				if(i1) {												// 基準曲線:閉は始点追加
					Spw[Si5] = Spw[0], Si5++;
				}
				OffEdtAllObject();										// 編集立体全解除
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
			} else if(er==1||ct1!=1) {
				AfxMessageBox(IDS_ERR_SEL8);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else if(er==2) {
				AfxMessageBox(IDS_ERR_PLANE);
			} else {
				AfxMessageBox(IDS_ERR_DIRECTION);
			}
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
		} else {														// <<2D曲線OP有>>
			Si2 = (m_FChar&0x8) ? 0 : Si2;								// [F8]:削除なし
			Si24 = (m_FChar&0x80) ? 1 : Si24;							// [F12]:範囲限定
			ct1 = 0, ct2 = 0, er = 0, Si6 = 0, Si7 = 0, Si8 = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// 表示対象立体取得
			while(op!=NULL) {											// <<編集立体有の場合>>
				i3 = GetObjAtr(op);										// 立体属性取得
				if(GetSelNo(op)==1) {									// <基準曲線の場合>
					er = (i3/10!=1) ? 3 : er;							// 曲線以外はｴﾗｰ
					ct1++;
				} else if(i2==1) {										// <編集対象の場合>
					ct2++;
					BaseCmdPtr(1, op, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					if((i3==10||i3==11)&&cmp!=NULL) {					// <曲線の場合>
						vn = GetParaInt(cmp, 0);						// 制御点数取得
						vb = GetParaInt(cmp, 1);						// 境界ﾓｰﾄﾞ取得
						if(vn+Si6>m_MaxWCtl) {							// 制御点合計ｵｰﾊﾞｰは中止
							er = 2; break;
						}
						for(i=0; i<vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// 制御点取得
						}
						SetCurve(vn, vb, Spa);							// 曲線制御点生成
						GetObjPtr(&op1);								// 制御点OP保存
						BaseCmdPtr(1, op, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 曲面編集後ｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							RedoCommandCopy(op1, cmp);					// ｺﾏﾝﾄﾞ複写実行
							NextCmdPtr(&cmp);							// 次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// 立体編集後制御点取得
						}
						FreeObj(op1, 1);								// 編集後立体削除
						Si6 += vn;										// 制御点数合計
						if(++Si8==1) {									// 編集対象曲線数
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// 張力係数設定
							}
						}
					} else if((i3==20||i3==21)&&cmp!=NULL) {			// <通常曲面の場合>
						un = GetParaInt(cmp, 0);						// U制御点数取得
						vn = GetParaInt(cmp, 1);						// V制御点数取得
						ub = GetParaInt(cmp, 2);						// U境界ﾓｰﾄﾞ取得
						vb = GetParaInt(cmp, 3);						// V境界ﾓｰﾄﾞ取得
						if(un*vn+Si6>m_MaxWCtl) {						// 制御点合計ｵｰﾊﾞｰは中止
							er = 2; break;
						}
						for(i=0; i<un*vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// 制御点取得
						}
						SetSurface(un, vn, ub, vb, Spa);				// 曲面制御点生成
						GetObjPtr(&op1);								// 制御点OP保存
						BaseCmdPtr(1, op, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 曲面編集後ｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							if(GetCmdNo(cmp)!=THICKNESS) {				// <厚みづけ以外の場合>
								RedoCommandCopy(op1, cmp);				// ｺﾏﾝﾄﾞ複写実行
							}
							NextCmdPtr(&cmp);							// 次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<un*vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// 立体編集後制御点取得
						}
						FreeObj(op1, 1);								// 編集後立体削除
						Si6 += un*vn;									// 制御点数合計
						if(++Si8==1) {									// 編集対象曲面数
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// 張力係数設定
							}
						}
					} else {											// 編集立体:NG
						er = 1; break;
					}
				}
				NextObjPtr(1, &op, &i1, &i2);							// 次表示立体へ
			}
			if(er==0&&ct1==1&&ct2>0) {
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(NULL)) {								// ﾄﾗｯｶｰ範囲限定設定
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
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// ﾄﾗｯｶｰ色/指示ﾌﾗｸﾞ初期化
				}
				Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);				// ﾄﾗｯｶｰ1設定
			} else {
				if(er==3||ct1!=1) {
					AfxMessageBox(IDS_ERR_SEL8);
				} else if(er==2) {
					AfxMessageBox(IDS_MANY_ECTLP);
				} else {
					AfxMessageBox(IDS_ERR_SEL5);						// ｴﾗｰﾒｯｾｰｼﾞ
				}
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
		Display(0);														// 再描画
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		for(i=0, i1=0; i<Si6; i++) {
			i1 = (Six[i]>0) ? i1+1 : i1;								// 指示済数ｶｳﾝﾄ
		}
		if(i1>0) {														// <指示ありの場合>
			StartMenu();												// ﾒﾆｭｰ開始処理
			InitLevel();												// Undo/Redo初期化
			m_SurfTension = Si4;
			AdjustCPoint(Si1, Si3, Si5, Spw, Si8, Si6, Six,
                         Spa, Spb, Spc, Spd, Sia, Sib, Sic);			// 制御点曲線合わせ
			if(Si2) {													// <断面登録立体削除あり>
				SelBaseObjPtr(1, &op);									// 断面登録立体取得
				if(op!=NULL) {
					LevelObj(1, op);									// 同一ﾚﾍﾞﾙ削除立体接続
					TempObj(op);										// 立体一時削除
				}
			}
			EndJMenu1();												// ﾒﾆｭｰ終了処理
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		Display(0);														// 再描画
	}
}

/********************************************************************/
void CJcad3GlbView::MouseObjectAdjust(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< 制御点立体合わせ
/********************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, ct1, ct2, er, un, vn, ub, vb;

	if((ms==2||ms==3)&&Si22) {											// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&plane!=3) {									// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(abs(BasePoint.x-point.x)>=DRAGMIN||
           abs(BasePoint.y-point.y)>=DRAGMIN) {
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			box.bp[0] = BaseCursor, box.bp[1] = pnt;
			DragCtrlTracker1(plane, box, Spx, Si6, Spy, Siy,
                             Si7, &Si21, &i1, Siz);						// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
			if(i1>0) {													// <制御点選択の場合>
				for(i=0; i<i1; i++) {
					i2 = Siz[i], pnt = Spx[i2];							// 選択ﾄﾗｯｶｰNo/座標
					Six[i2] = (Six[i2]==4) ? 0 : 4;						// 選択状況反転
					for(j=0; j<Si6; j++) {								// <指定点同一座標>
						if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                           fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                           fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
							Six[j] = (Six[j]==4) ? 0 : 4;				// 選択状況反転
						}
					}
				}
				SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                               Spy, Siy, &Si7);							// 透視図ﾄﾗｯｶｰ設定
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		HitCtrlTracker1(flg, point, plane, ms, pnt, Spx, Si6,
                        Spy, Siy, Si7, &Si21, &Si22, &i1, Siz);			// 全ﾄﾗｯｶｰﾁｪｯｸ1
		if(Si22) {
			return;
		}
		if(i1>0) {														// <制御点選択の場合>
			if(!m_CtlInp) {
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// 全制御点選択解除
				}
			}
			for(i=0; i<i1; i++) {
				i2 = Siz[i], pnt = Spx[i2];								// 選択ﾄﾗｯｶｰNo/座標
				Six[i2] = (Six[i2]==4) ? 0 : 4;							// 選択状況反転
				for(j=0; j<Si6; j++) {									// <指定点同一座標>
					if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                       fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                       fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
						Six[j] = (Six[j]==4) ? 0 : 4;					// 選択状況反転
					}
				}
			}
			SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                           Spy, Siy, &Si7);								// 透視図ﾄﾗｯｶｰ設定
		} else if(!m_CtlInp) {											// <ﾄﾗｯｶｰ指定無で追加無>
			for(i=0; i<Si6; i++) {
				Six[i] = 0;												// 全制御点選択解除
			}
			Si21 = 0, Si7 = 0;											// 透視図選択解除
			Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);					// ﾄﾗｯｶｰ1設定
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(GetSelectNo()==0) {											// <<立体OP無>>
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			ct1 = 0, er = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// 編集対象立体取得
			while(op!=NULL) {
				if(GetObjAtr(op)/10==1) {								// 曲線はｴﾗｰ
					er = 1; break;
				}
				op1 = op;												// 立体OP保存
				if(++ct1>1) {											// 立体数ｶｳﾝﾄUP
					er = 1; break;
				}
				NextObjPtr(2, &op, &i1, &i2);							// 次編集立体へ
			}
			if(er==0&&ct1==1) {											// <正常な場合>
				Si4 = m_SurfTension;									// 張力係数保存
				SetSelNo(op1, 1);										// 断面曲線立体登録
				OffEdtAllObject();										// 編集立体全解除
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
			} else {
				AfxMessageBox(IDS_ERR_SEL1);							// ｴﾗｰﾒｯｾｰｼﾞ
			}
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
		} else {														// <<2D曲線OP有>>
			Si24 = (m_FChar&0x80) ? 1 : Si24;							// [F12]:範囲限定
			ct1 = 0, ct2 = 0, er = 0, Si6 = 0, Si7 = 0, Si8 = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// 表示対象立体取得
			while(op!=NULL) {											// <<表示立体有の場合>>
				i3 = GetObjAtr(op);										// 立体属性取得
				if(GetSelNo(op)==1) {									// <基準面の場合>
					er = (i3/10==1) ? 3 : er;							// 曲線はｴﾗｰ
					ct1++;
				} else if(i2==1) {										// <編集対象の場合>
					ct2++;
					BaseCmdPtr(1, op, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					if((i3==10||i3==11)&&cmp!=NULL) {					// <曲線の場合>
						vn = GetParaInt(cmp, 0);						// 制御点数取得
						vb = GetParaInt(cmp, 1);						// 境界ﾓｰﾄﾞ取得
						if(vn+Si6>m_MaxWCtl) {							// 制御点合計ｵｰﾊﾞｰは中止
							er = 2; break;
						}
						for(i=0; i<vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// 制御点取得
						}
						SetCurve(vn, vb, Spa);							// 曲線制御点生成
						GetObjPtr(&op1);								// 制御点OP保存
						BaseCmdPtr(1, op, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 曲面編集後ｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							RedoCommandCopy(op1, cmp);					// ｺﾏﾝﾄﾞ複写実行
							NextCmdPtr(&cmp);							// 次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// 立体編集後制御点取得
						}
						FreeObj(op1, 1);								// 編集後立体削除
						Si6 += vn;										// 制御点数合計
						if(++Si8==1) {									// 編集対象曲線数
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// 張力係数設定
							}
						}
					} else if((i3==20||i3==21)&&cmp!=NULL) {			// <通常曲面の場合>
						un = GetParaInt(cmp, 0);						// U制御点数取得
						vn = GetParaInt(cmp, 1);						// V制御点数取得
						ub = GetParaInt(cmp, 2);						// U境界ﾓｰﾄﾞ取得
						vb = GetParaInt(cmp, 3);						// V境界ﾓｰﾄﾞ取得
						if(un*vn+Si6>m_MaxWCtl) {						// 制御点合計ｵｰﾊﾞｰは中止
							er = 2; break;
						}
						for(i=0; i<un*vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// 制御点取得
						}
						SetSurface(un, vn, ub, vb, Spa);				// 曲面制御点生成
						GetObjPtr(&op1);								// 制御点OP保存
						BaseCmdPtr(1, op, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 曲面編集後ｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							if(GetCmdNo(cmp)!=THICKNESS) {				// <厚みづけ以外の場合>
								RedoCommandCopy(op1, cmp);				// ｺﾏﾝﾄﾞ複写実行
							}
							NextCmdPtr(&cmp);							// 次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<un*vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// 立体編集後制御点取得
						}
						FreeObj(op1, 1);								// 編集後立体削除
						Si6 += un*vn;									// 制御点数合計
						if(++Si8==1) {									// 編集対象曲面数
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// 張力係数設定
							}
						}
					} else {											// 編集立体:NG
						er = 1; break;
					}
				}
				NextObjPtr(1, &op, &i1, &i2);							// 次表示立体へ
			}
			if(er==0&&ct1==1&&ct2>0) {
				if(Si24) {												// <範囲限定あり>
					if(SetTRLimit(NULL)) {								// ﾄﾗｯｶｰ範囲限定設定
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
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// ﾄﾗｯｶｰ色/指示ﾌﾗｸﾞ初期化
				}
				Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);				// ﾄﾗｯｶｰ1設定
			} else {
				if(er==3||ct1!=1) {
					AfxMessageBox(IDS_ERR_SEL1);
				} else if(er==2) {
					AfxMessageBox(IDS_MANY_ECTLP);
				} else {
					AfxMessageBox(IDS_ERR_SEL5);						// ｴﾗｰﾒｯｾｰｼﾞ
				}
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
		Display(0);														// 再描画
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		for(i=0, i1=0; i<Si6; i++) {
			i1 = (Six[i]>0) ? i1+1 : i1;								// 指示済数ｶｳﾝﾄ
		}
		if(i1>0) {														// <指示ありの場合>
			m_SurfTension = Si4;
			ObjectAdjustCP(Si8, Si6, Spx, Six, Spa, Spb, Spc, Spd,
                           Spw, Sia, Sib, Sic);							// 制御点立体合わせ
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		Display(0);														// 再描画
	}
}

/******************************************/
void CJcad3GlbView::MenuObjectAdjust(void)								//<<< 曲面立体合わせ
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i1, i2, atr, ct1=0, ct2=0, err=0;

	if(GetSelectNo()==0) {												// <<曲面OP無>>
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// 曲線はｴﾗｰ
			if(err) {
				break;													// ｴﾗｰは中止
			}
			if(++ct1>1) {												// 曲面数ｶｳﾝﾄUP
				break;
			}
			SetSelNo(op, 1);											// 基準曲面立体登録
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		if(err==0&&ct1==1) {											// <有効な場合>
			OffEdtAllObject();											// 編集立体全解除
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		} else {														// <ｴﾗｰの場合>
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	} else {															// <<曲面OP有>>
		BaseObjPtr(1, &op, &i1, &i2);									// 表示対象立体取得
		while(op!=NULL) {
			atr = GetObjAtr(op);										// 立体属性取得
			if(GetSelNo(op)==1) {										// <基準面の場合>
				err = (atr/10==1) ? 1 : err;							// 曲線はｴﾗｰ
				ct1++;
			} else if(i2==1) {											// <編集対象の場合>
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				while(cmp!=NULL) {
					i1 = GetCmdNo(cmp);									// ｺﾏﾝﾄﾞNo取得
					if(i1!=CRTSURF&&i1!=CHGSURF) {						// 曲面生成以外ｴﾗｰ
						err = 2; break;
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				err = (atr!=20&&atr!=21) ? 3 : err;						// 通常曲面以外はｴﾗｰ
				ct2++;													// 編集対象立体数ｶｳﾝﾄUP
				SetSelNo(op, 2);										// 編集対象立体登録
			}
			if(err||ct1>1) {
				break;													// ｴﾗｰは中止
			}
			NextObjPtr(1, &op, &i1, &i2);								// 次表示立体へ
		}
		if(err==0&&ct1==1&&ct2>0) {
			ObjectAdjust();												// 曲面指定移動
		} else if(err==1||ct1!=1) {
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else if(err==3||ct2==0) {
			AfxMessageBox(IDS_ERR_SEL3);
		} else {
			AfxMessageBox(IDS_ERR_OADJUST);
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
	}
	Display(0);															// 再描画
}

/**********************************************/
void CJcad3GlbView::MenuChgSurfParam(int mode)							//<<< 曲線ﾊﾟﾗﾒｰﾀ変更
/**********************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, cnt=0, cno, mx, i, stp=0;
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
		return;
	}
	if(!mode) {															// <通常ﾓｰﾄﾞ>
		CInputDlg dlg(9);
		dlg.SetValue(mx, wt);											// 曲線分割数,曲線重み係数取得
		if(dlg.DoModal()==IDOK) {										// ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&mx, &wt);
		} else {
			stp = 1;													// 中止
		}
	} else {															// <入力なしﾓｰﾄﾞ>
		if(mx!=m_CurveRepeat||wt!=m_CurveWeight) {						// ｼｽﾃﾑ登録値と立体登録値が
			mx = m_CurveRepeat;											// 異なる場合はｼｽﾃﾑ登録値を採用
			wt = m_CurveWeight;
		} else {														// ｼｽﾃﾑ登録値と立体登録値が
			stp = 1;													// 一致の場合は中止
		}
	}
	if(!stp) {															// <<中止なし>>
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		BaseObjPtr(2, &op, &dsp, &edt);									// 先頭立体のﾎﾟｲﾝﾀ(編集対象)
		while(op) {														// 指定立体が存在する
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			cno = GetCmdNo(cmp);										// ｺﾏﾝﾄﾞNo取得
			if(cno==CRTCURVE||cno==CHGCURVE) {							// <曲線生成/変更>
				Si1 = GetParaInt(cmp, 0);								// 制御点数取得
				Si2 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 制御点取得
				}
				ChgCurve(op, Si1, Si2, mx, wt, Spa, Sia);				// 曲線変更
			} else if(cno==CRTSURF||cno==CHGSURF) {						// <曲面生成/編集>
				Si1 = GetParaInt(cmp, 0);								// U制御点数取得
				Si2 = GetParaInt(cmp, 1);								// V制御点数取得
				Si3 = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
				Si4 = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
				Si5 = Si1*Si2;											// 総制御点数
				for(i=0; i<Si5; i++) {
					Sia[i] = GetParaInt(cmp, 7+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 制御点取得
				}
				ChgSurface(op, Si1, Si2, Si3, Si4, mx, wt, Spa, Sia);	// 曲面変更
			} else if(cno==CRTSWEEP||cno==CHGSWEEP) {					// <掃引曲面生成/編集>
				Si1 = GetParaInt(cmp, 0);								// U制御点数取得
				Si2 = GetParaInt(cmp, 1);								// V制御点数取得
				Si3 = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
				Si4 = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
				for(i=0; i<Si1+Si2; i++) {
					Sia[i] = GetParaInt(cmp, 8+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 制御点取得
				}
				ChgSweepSurf(op, Si1, Si2, Si3, Si4, mx, wt, Spa, Sia);	// 掃引曲面変更
			}
			NextObjPtr(2, &op, &dsp, &edt);
		}
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		Display(0);														// 再描画
		MainWnd->SetCrtPrm(mx, wt);										// 生成ﾊﾟﾗﾒｰﾀ設定
	}
}

/***************************************************************/
void CJcad3GlbView::MouseTension(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 張力係数設定
/***************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  area;
	int      i, i1, i2, i3;

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&Sphase==2&&plane!=3) {						// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==2) {
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				area.bp[0] = BaseCursor, area.bp[1] = pnt;
				SelAreaCtrl(plane, area, Si5, Spa, Tens, m_CtlInp);		// 制御点選択(ｴﾘｱ)
			}
			for(i=0; i<Si5; i++) {
				Sia[i] = 2*Tens[i];
			}
			Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);					// ﾄﾗｯｶｰ1設定
			DeleteBox();												// 矩形破棄
			Display(0);													// 再描画
			Sphase = 1;
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		Sphase = 1, i1 = -1, i2 = -1;
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(plane==3) {													// <透視図選択>
			if((i1=Tracker1.HitTracker3D(0, point, Spa, Si5))==-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				EyeChange(flg, point, plane, ms);						// 視点変更
				Si22 = 1;												// 視点変更ﾓｰﾄﾞ
				return;
			}
		} else {														// <3面図選択>
			i2 = Tracker1.HitTrackerS(0, plane, pnt, Spa, Si5, Sia);	// ﾄﾗｯｶｰ1ﾁｪｯｸ
		}
		if(!m_CtlInp) {
			for(i=0; i<Si5; i++) {
				Tens[i] = 0;											// 選択全解除
			}
		}
		if(i1!=-1) {													// <透視図選択OK>
			Tens[i1] = (Tens[i1]) ? 0 : 1;
			for(i=0; i<Si5; i++) {
				if(fabs(Spa[i].p[0]-Spa[i1].p[0])<EPSILON&&
                   fabs(Spa[i].p[1]-Spa[i1].p[1])<EPSILON&&
                   fabs(Spa[i].p[2]-Spa[i1].p[2])<EPSILON&&i!=i1) {		// 同一座標点の場合
					Tens[i] = (Tens[i]) ? 0 : 1;						// 選択反転
				}
			}
		} else if(i2>0) {												// <3面図選択OK>
			for(i=0; i<i2; i++) {
				Tens[Sia[i]] = (Tens[Sia[i]]) ? 0 : 1;					// 選択反転
			}
		} else {														// <ﾄﾞﾗｯｸﾞ開始>
			Sphase = 2;
		}
		for(i=0; i<Si5; i++) {
			Sia[i] = 2*Tens[i];
		}
		Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);						// ﾄﾗｯｶｰ1設定
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:範囲限定
			Sop1 = NULL, Sop2 = NULL;
			Si22 = 0;
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==10||i3==11||i3==20||i3==21)&&cmp!=NULL) {			// <<曲線･通常曲面の場合>>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				Display(0);												// 再描画
				if(i3==20||i3==21) {									// <曲面>
					Si1 = GetParaInt(cmp, 0);							// U制御点数取得
					Si2 = GetParaInt(cmp, 1);							// V制御点数取得
					Si3 = GetParaInt(cmp, 2);							// U境界ﾓｰﾄﾞ取得
					Si4 = GetParaInt(cmp, 3);							// V境界ﾓｰﾄﾞ取得
				} else {												// <曲線>
					Si2 = GetParaInt(cmp, 0);							// 制御点数取得
					Si4 = GetParaInt(cmp, 1);							// 境界ﾓｰﾄﾞ取得
					Si1 = 1, Si3 = 0;
				}
				Si5 = Si1*Si2;											// 総制御点数
				CTensionDlg dlg;
				dlg.SetValue(m_TensionNo);								// 既存値表示
				if(dlg.DoModal()==IDOK) {								// ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_SurfTension, &i1);					// 張力係数設定
					m_TensionNo = m_SurfTension;
					if(m_SurfTension==0) {								// <張力設定無の場合>
						for(i=0; i<Si5; i++) {
							Tens[i] = 0;								// 範囲指定全解除
						}
						ExitMenu(0, 0);									// ﾒﾆｭｰ終了
					} else if(!i1) {									// <設定有&限定無の場合>
						for(i=0; i<Si5; i++) {
							Tens[i] = 1;								// 範囲指定全設定
						}
						ExitMenu(0, 0);									// ﾒﾆｭｰ終了
					} else {											// <設定有&限定有の場合>
						for(i=0; i<Si5; i++) {
							Tens[i] = 0;								// 範囲指定全解除
						}
					}
				} else {
					ExitMenu(0, 0);										// ｷｬﾝｾﾙはﾒﾆｭｰ終了
				}
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL5);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
			if(m_ActMenu!=NONE) {										// <範囲限定有の場合>
				for(i=0; i<Si5; i++) {
					GetParaPnt(cmp, i, &Spa[i]);						// 制御点取得
				}
				if(i3==20||i3==21) {									// <曲面>
					SetSurface(Si1, Si2, Si3, Si4, Spa);				// 曲面制御点生成
				} else {												// <曲線>
					SetCurve(Si2, Si4, Spa);							// 曲線制御点生成
				}
				GetObjPtr(&Sop2);										// 制御点OP保存
				SetSelNo(Sop2, 2);										// 黄色表示OP設定
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲面編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)!=THICKNESS) {
						RedoCommandCopy(Sop2, cmp);						// ｺﾏﾝﾄﾞ複写実行
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				SetDispList(Sop2);										// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si5; i++) {
					Sia[i] = 2*Tens[i];
				}
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
				Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);				// ﾄﾗｯｶｰ1設定
				OffUndoRedo();											// Undo/Redo不可
				Display(0);												// 再描画
				m_CursorNo = 1;											// ｶｰｿﾙNo設定
			}
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		for(i1=0, i=0; i<Si5; i++) {
			if(Tens[i]) {
				i1 = 1; break;
			}
		}
		if(!i1) {
			m_SurfTension = 0;											// 範囲無は張力設定無効
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}
