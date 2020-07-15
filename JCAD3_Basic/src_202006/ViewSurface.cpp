/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/06/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "RCommand.h"
#include "Select.h"
#include "CreatCtrlPoint1.h"
#include "CreatCtrlPoint2.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*************************************************************/
void CJcad3GlbView::MouseSweep(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< 曲面生成(掃引体)
/*************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, endf=0;
	double   d[3], dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(Sphase>0&&Si3>0) {
		i = (m_FChar&0x20) ? 1 : 0;										// [F10]:断面90度回転
		i = (m_FChar&0x40) ? 2 : i;										// [F11]:断面180度回転
		if(i>=0) {
			ChangeRotate2D(i, Si6, Si1, Spa);							// 2D断面回転更新
		}
		m_FChar = m_FChar&~(0x20);										// [F10]OFF
		m_FChar = m_FChar&~(0x40);										// [F11]OFF
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2&&HitTestPlane(point)==Si5&&Si3>0) {				// <平面ﾁｪｯｸOK>
			CalcScreenWorld(0, &pnt, point, Si5);						// 座標点取得
			if(CheckNextPoint(Spnt1, pnt)) {							// 直前点との距離OK
				if(flg&m_LimKey) {
					LimitPoint(Si5, &pnt, &Spnt1);						// ﾏｳｽ入力制限座標
				}
				Sib[Si3] = 1, Spb[Si3] = pnt;							// ﾓｰﾄﾞ/制御点設定(仮)
				CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3+1,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// 掃引体制御点計算
				RedoSetSurface(Si3+1, Si1, 0, Si2, Spc);				// 曲面制御点生成(仮)
				if(Si3>0) {
					Tracker1.Setup(Spc, Sic, NULL, (Si3+1)*Si1, 5, 0);	// ﾄﾗｯｶｰ1設定
				} else {
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
				}
				for(i=0; i<DIM; i++) {
					d[i] = pnt.p[i]-Spnt1.p[i];							// 直前点との差
				}
				dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);				// 直前点との距離
				m_pNumDisp->SetParam(d[0], d[1], d[2], dd);				// ﾏｳｽ移動量表示
				Display(0);												// 再描画
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(Sphase==1) {													// <<初回の1点目>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			Si5 = plane;
			Sib[Si3] = 1;												// 始点折線点ﾓｰﾄﾞ
			Spb[Si3++] = pnt;											// 制御点設定(本)
			Spnt1 = pnt;												// 座標点保存
			InitLevel();												// Undo/Redo初期化
			InitSweep(Sbase, Si6, Si4, Si1, Spa);						// 掃引初期処理
			SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3, Sib, Sda, Sdb);	// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			CalcSweepPoint(0, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                           Spa, Sia, Spb, Sib, Spc, Sic, Sdb);			// 掃引体制御点計算
			GetObjPtr(&Sop1);											// 曲面OP保存
			SetSurface(Si3, Si1, 0, Si2, Spc);							// 曲面制御点生成(初回)
			GetObjPtr(&Sop2);											// 制御点OP保存
			SetSelNo(Sop2, 2);											// 黄色表示OP設定
			Sphase = 2;
			SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                            Sib, Sda, Sdb);								// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			Display(0);													// 再描画
			if(m_NumInp!=0) {											// <数値入力の場合>
				CInputBtnDlg5 dlg(2);
				dlg.SetParam2(m_SweepFlg, Si7, Si6, Si2,
                              m_CurveWeight, Si1, &Si3, Spa, Spb, Spc,
                              Sia, Sib, Sic, Sda, Sdb, m_SweepAdd);		// ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_SweepAdd);								// ﾊﾟﾗﾒｰﾀ取得
				endf = 1;												// ﾒﾆｭｰ終了
			}
		} else if(plane==Si5) {											// <<2点目以降で同一平面>>
			CalcScreenWorld(0, &pnt, point, Si5);						// 座標点取得
			if(Si3==0) {												// <1点目まで取り消された場合>
				Sib[Si3] = 1;											// 始点折線点ﾓｰﾄﾞ
				Spb[Si3++] = pnt;										// 制御点設定(本)
				Spnt1 = pnt;											// 座標点保存
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                Sib, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				CalcSweepPoint(0, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// 掃引体制御点計算
				GetObjPtr(&Sop1);										// 曲面OP保存
				SetSurface(Si3, Si1, 0, Si2, Spc);						// 曲面制御点生成(初回)
				GetObjPtr(&Sop2);										// 制御点OP保存
				SetSelNo(Sop2, 2);										// 黄色表示OP設定
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                Sib, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				Display(0);												// 再描画
			} else {													// <2点目以降>
				if(CheckNextPoint(Spnt1, pnt)) {						// 直前点との距離OK
					if(Si3>0&&(flg&m_LimKey)) {
						LimitPoint(Si5, &pnt, &Spnt1);					// ﾏｳｽ入力制限座標
					}
					if(Si1*(2*Si6+Si3+1)<=m_MaxSCtl&&
                       2*Si6+Si3+1<=m_MaxCCtl) {						// <制御点上限未満>
						Sib[Si3] = (m_CurInp) ? 1 : 0;					// Shift折線点ﾓｰﾄﾞ
						Spb[Si3++] = pnt;								// 制御点設定(本)
						Spnt1 = pnt;									// 座標点保存
					} else {											// <制御点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                        Sib, Sda, Sdb);					// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					}
					CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                                   Spa, Sia, Spb, Sib, Spc, Sic, Sdb);	// 掃引体制御点計算
					RedoSetSurface(Si3, Si1, 0, Si2, Spc);				// 曲面制御点生成(本)
					if(Si3>1) {
						Tracker1.Setup(Spc, Sic, NULL, Si3*Si1, 5, 0);	// ﾄﾗｯｶｰ1設定
					} else {
						Tracker1.Reset(0);								// ﾄﾗｯｶｰ1解除
					}
					SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                    Sib, Sda, Sdb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==11) {													// <<<DEL(取消)>>>
		if(Sphase==2&&Si3>0) {											// <制御点を登録済>
			Si3--;														// 制御点数減算
			if(Si3>0) {
				Spnt1 = Spb[Si3-1];										// 座標点保存
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                Sib, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// 掃引体制御点計算
				RedoSetSurface(Si3, Si1, 0, Si2, Spc);					// 曲面制御点生成(本)
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                Sib, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			} else {
				Spnt1.p[0]=DBL_MAX, Spnt1.p[1]=DBL_MAX, Spnt1.p[2]=DBL_MAX;
				if(Sop1!=NULL) {
					FreeObj(Sop1, 1);									// 立体削除
				}
				if(Sop2!=NULL) {
					FreeObj(Sop2, 1);									// 立体削除
				}
			}
			if(Si3>1) {
				Tracker1.Setup(Spc, Sic, NULL, Si3*Si1, 5, 0);			// ﾄﾗｯｶｰ1設定
			} else {
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
			}
			Display(0);													// 再描画
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si6 = 0;
			m_SolidFlg = m_AtrEnv;										// 生成曲面属性
			Si8 = m_SecEnv;												// 生成端面形状
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			if(m_FChar&0x1) {											// [F5]:生成属性反転
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:端面形状反転
				Si8 = (Si8) ? 0 : 1;
			}
			m_SweepFlg = (m_FChar&0x4) ? 1 : 0;							// [F7]:掃引曲面
			Si7 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			Si9 = (m_FChar&0x80) ? 1 : 0;								// [F12]:分轄ﾀﾞｲｱﾛｸﾞ無
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==1&&cmp!=NULL) {									// <曲線の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				Display(0);												// 再描画
				GetObjBox(Sop1, &box);									// 立体BOXｻｲｽﾞ取得
				Si1 = GetParaInt(cmp, 0);								// 制御点数取得
				Si2 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 曲線制御点取得
				}
				SetCurve(Si1, Si2, Spa);								// 曲線制御点生成
				GetObjPtr(&Sop2);										// 制御点OP保存
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// 立体編集後制御点取得
				}
				FreeObj(Sop2, 1);										// 編集後立体削除
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si4)) {			// 生成初期処理
					if(InputBasePoint1(&m_SecBase2)) {					// 断面生成位置取得
						SetBasePoint(Si4, m_SecBase2, box, &Sbase);		// 断面生成座標設定
						StartMenu();									// ﾒﾆｭｰ開始処理
						if(!m_SweepFlg) {								// <通常曲面>
							i1 = GetVtxNum(Sop1);						// 曲面頂点数取得
							for(i=0; i<i1; i++) {
								GetVtx(Sop1, i, &Spb[i]);				// 曲面頂点座標取得
							}
							Si6 = GetCurveCenter(Si4, Si2, i1,
                                                 Spb, &pnt);			// 2D図形中心取得
							Si6 = ((!m_SolidFlg)||Si8) ? 0 : Si6;		// OFFは断面無
							if(Si6) {
								Spa[Si1] = pnt;							// 2D中心追加
							}
						} else {										// <掃引曲面>
							for(i=0; i<Si1; i++) {
								Sia[i] /= 2;							// 曲線ﾓｰﾄﾞ変換(0/1)
							}
						}
						OffUndoRedo();									// Undo/Redo不可
						SetNumDisp(2);									// 数値表示初期処理
						Sphase = 1;
						if(m_SweepFlg&&Si7) {							// 掃引曲面&通過点ﾓｰﾄﾞ
							AfxMessageBox(IDS_ERR_SWEEP);				// ｴﾗｰﾒｯｾｰｼﾞ
							Si7 = 0;									// 掃引曲面は制御点ﾓｰﾄﾞ
						}
					} else {											// <ｷｬﾝｾﾙ>
						ExitMenu(0, 1);									// ﾒﾆｭｰ終了
					}
				} else {												// <存在平面:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// ｴﾗｰﾒｯｾｰｼﾞ
					ExitMenu(0, 1);										// ﾒﾆｭｰ終了
				}
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
			Sop1 = NULL, Sop2 = NULL;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		endf = 1;														// ﾒﾆｭｰ終了
	}
	if(endf) {															// <ﾒﾆｭｰ終了の場合>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		if(Sphase==2) {
			if(Si3>1) {													// <<<正常な曲線>>>
				Sib[0] = 1, Sib[Si3-1] = 1;								// 始点/終点折線点ﾓｰﾄﾞ
				if(Si3==2) {											// <始点/終点のみの場合>
					i1 = InsertSweepPoint(Si9, Si1, Si6, &m_SweepDivi,
                                          &Si3, Spb, Sib);				// 掃引体中間制御点挿入
				} else {
					i1 = TRUE;
				}
				if(i1) {												// <ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙなし>
					Delete();											// 2D図形削除
					SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                    Sib, Sda, Sdb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					CalcSweepPoint(1, m_SweepFlg, Si7, Si6,
                                   Si1, Si2, Si3, Spa, Sia,
                                   Spb, Sib, Spc, Sic, Sdb);			// 掃引体制御点計算
					i = GetObjAtr(Sop1);								// 立体属性取得
					GetObjOif(Sop1, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					if(i==20) {
						SetOifName(0,oif,OBJ_NAME03);					// 体構成名(通常掃引体)
					} else if(i==21) {
						SetOifName(0,oif,OBJ_NAME04);					//         (通常掃引体[面体])
					} else if(i==22) {
						SetOifName(0,oif,OBJ_NAME05);					//         (掃引掃引体)
					} else {
						SetOifName(0,oif,OBJ_NAME06);					//         (掃引掃引体[面体])
					}
					OnEdtFlg(1, Sop1);									// 曲面edtflg:ON
					CountEdtObj();										// 編集対象立体ｶｳﾝﾄ
				} else {												// <ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙあり>
					FreeObj(Sop1, 1);									// 曲線削除
					BackLevel();										// ﾚﾍﾞﾙ戻
				}
			} else {													// <<<有効制御点数未満>>>
				AfxMessageBox(IDS_FEW_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
				FreeObj(Sop1, 1);										// 曲線削除
				BackLevel();											// ﾚﾍﾞﾙ戻
			}
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
		}
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
    }
}

/***************************************************************/
void CJcad3GlbView::MouseRevolve(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 曲面生成(回転体)
/***************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt, ps[3], pe[3];
	BOXTYPE  box;
	int      i, j, i1, i2, i3, cl[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if((Sphase==2||Sphase==3)&&(flg&MK_LBUTTON)) {					// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(0, &Spnt2, point, Si6);						// 回転軸(ﾗｲﾝ)終点
			if(CheckNextPoint(Spnt1, Spnt2)) {							// 1点目との距離OK
				if(flg&m_LimKey) {
					LimitPoint(Si6, &Spnt2, &Spnt1);					// ﾏｳｽ入力制限座標
				}
				ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 0;
				Line.Setup(ps, pe, cl, 1, Si6);							// 回転軸(ﾗｲﾝ)設定
				if(Sphase==2) {											// <1回目>
					InitLevel();										// Undo/Redo初期化
					CalcRevolvePoint(0, m_SweepFlg, Si10, Si9, Si6, 
                                     Si1, Si2, Si3, Si4, Sd1, Spnt1,
                                     Spnt2, Spnt3, Spa, Sia);			// 回転体制御点計算
					GetObjPtr(&Sop1);									// 曲面OP保存
					Sphase = 3;
				} else {												// <2回目以降>
					CalcRevolvePoint(1, m_SweepFlg, Si10, Si9, Si6,
                                     Si1, Si2, Si3, Si4, Sd1, Spnt1,
                                     Spnt2, Spnt3, Spa, Sia);			// 回転体制御点計算
				}
				Display(0);												// 再描画
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1&&HitTestPlane(point)==Si6) {						// <平面ﾁｪｯｸOK>
			i1 = -1;
			if(Si8>0) {
				CalcScreenWorld(1, &Spnt1, point, Si6);					// 座標点取得
				i1 = Tracker1.HitTracker(1, Si6, Spnt1, Spb, Si8);		// ﾄﾗｯｶｰ1ﾁｪｯｸ
			}
			if(i1==-1) {												// <ﾄﾗｯｶｰ無/非選択>
				CalcScreenWorld(0, &Spnt1, point, Si6);					// 回転軸(ﾗｲﾝ)始点
				if(Si8>0) {
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
				}
				Sphase = 2;
			} else {													// <ﾄﾗｯｶｰ選択>
				InitLevel();											// Undo/Redo初期化
				SetSRevolvePoint(Si10, Si6, i1, Si1, Si3, Sd1,
                                 Spa, Sia, Spnt1);						// 回転体ｿﾘｯﾄﾞ制御点作成
				GetObjPtr(&Sop1);										// 曲面OP保存
				Sphase = 4;
			}
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si4 = 0;
			Si8 = 0, Si9 = 0, Si10 = 0;
			m_SolidFlg = m_AtrEnv;										// 生成曲面属性
			Si11 = m_SecEnv;											// 生成端面形状
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			if(m_FChar&0x1) {											// [F5]:生成属性反転
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:端面形状反転
				Si11 = (Si11) ? 0 : 1;
			}
			m_SweepFlg = (m_FChar&0x4) ? 1 : 0;							// [F7]:掃引曲面
			Si12 = (m_FChar&0x80) ? 1 : 0;								// [F12]:分轄ﾀﾞｲｱﾛｸﾞ無
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==1&&cmp!=NULL) {									// <曲線の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				Display(0);												// 再描画
				GetObjBox(Sop1, &box);									// 立体BOXｻｲｽﾞ取得
				Si1 = GetParaInt(cmp, 0);								// 制御点数取得
				Si2 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 曲線制御点取得
				}
				SetCurve(Si1, Si2, Spa);								// 曲線制御点生成
				GetObjPtr(&Sop2);										// 制御点OP保存
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ｺﾏﾝﾄﾞの複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// 立体編集後制御点取得
				}
				FreeObj(Sop2, 1);										// 編集後立体削除
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si6)) {			// 生成初期処理
					if(GetRevolveInfo(m_NumInp, Si12, &m_RevolveTh,
                                      &m_RevolveDivi)) {				// 回転体情報取得
						Sd1 = (m_NumInp) ? m_RevolveTh : 360.0;
						Si3 = (!Si12) ? m_RevolveDivi : 8;
					} else {
						Sd1 = 0.0;
					}
				} else {												// <存在平面:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// ｴﾗｰﾒｯｾｰｼﾞ
					Sd1 = 0.0;
				}
				if(Sd1>0.0) {											// <有効生成角度>
					if(fabs(Sd1-360.0)<EPSILON) {						// 360度は境界:閉
						Si4 = 1;
					}
					if(!m_SweepFlg) {									// <<通常曲線>>
						if(fabs(Sd1-360.0)<EPSILON) {					// <360度>
							InitRevolve(Si2, Si1, Spa, Sia,
                                        &Si8, Spb, Sib);				// 回転初期初期
							if((!Si2)&&m_SolidFlg&&(!Si11)) {			// ONで開は断面有
								Si9 = 1;
							}
						} else {										// <360度未満>
							i1 = GetVtxNum(Sop1);						// 曲面頂点数取得
							for(i=0; i<i1; i++) {
								GetVtx(Sop1, i, &Spb[i]);				// 曲面頂点座標取得
							}
							Si10 = GetCurveCenter(Si6, Si2, i1,
                                                  Spb, &pnt);			// 2D図形中心取得
							Si10 = ((!m_SolidFlg)||Si11) ? 0 : Si10;	// OFFは断面無
							if(Si10) {
								Spa[Si1] = pnt;							// 2D中心追加
							}
						}
					} else {											// <<掃引曲線>>
						for(i=0; i<DIM; i++) {							// 2D頂点中心点取得
							Spnt3.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
						}
						for(i=0; i<Si1; i++) {
							for(j=0; j<DIM; j++) {
								Spa[i].p[j] -= Spnt3.p[j];				// 2Dを中心点へ移動
							}
						}
						for(i=0; i<Si1; i++) {
							Sia[i] /= 2;								// 曲線ﾓｰﾄﾞ変換(0/1)
						}
					}
					if((2*(Si9+Si10)+Si1)*Si3>m_MaxSCtl) {				// <制御点上限より上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ExitMenu(0, 1);									// ﾒﾆｭｰ終了
					} else {											// <制御点上限以下>
						OffUndoRedo();									// Undo/Redo不可
						StartMenu();									// ﾒﾆｭｰ開始処理
						if(Si8>0) {
							Tracker1.Setup(Spb, Sib, NULL, Si8, Si6, 7);// ﾄﾗｯｶｰ1設定
							Display(0);									// 再描画
						}
						Sphase = 1;
					}
				} else {
					ExitMenu(0, 1);										// ﾒﾆｭｰ終了
				}
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
			Sop1 = NULL, Sop2 = NULL;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if((ms==3&&Sphase>=3)||ms==4||ms==9) {						// <<<左解放/右押下/終了>>>
		if(ms==3&&Sphase==3) {											// <2回目以降>
			CalcRevolvePoint(2, m_SweepFlg, Si10, Si9, Si6,
                             Si1, Si2, Si3, Si4, Sd1, Spnt1,
                             Spnt2, Spnt3, Spa, Sia);					// 回転体制御点計算
		}
		if(Sphase>=3) {
			i1 = GetObjAtr(Sop1);										// 立体属性取得
			GetObjOif(Sop1, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME07);							// 体構成名(通常回転体)
			} else if(i1==21) {
				SetOifName(0, oif, OBJ_NAME08);							//         (通常回転体[面体])
			} else if(i1==22) {
				SetOifName(0, oif, OBJ_NAME09);							//         (掃引回転体)
			} else {
				SetOifName(0, oif, OBJ_NAME10);							//         (掃引回転体[面体])
			}
			Delete();													// 2D図形削除
			OnEdtFlg(1, Sop1);											// 曲面edtflg:ON
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		}
		Line.Reset();													// ﾗｲﾝ解除
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		OnUndoRedo();													// Undo/Redo可
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
	}
}

/***************************************************************/
void CJcad3GlbView::MousePyramid(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< 曲面生成(錐)
/***************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, i1, i2, i3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2&&(flg&MK_LBUTTON)) {								// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 錘頂点座標取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Spnt2);						// ﾏｳｽ入力制限座標
			}
			i1 = plane;													// 指定平面有効座標
			i2 = (i1+1>2) ? 0 : i1+1;
			Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];			// 有効座標更新
			Pyramid(0, Si5, Si1, Spa, Sia, Spnt1, Si3, Spb, Sib);		// 錘制御点生成(仮)
			RedoCrtSurface((Si3+2), Si1, 0, Si2, Spb, Sib);				// 曲面生成(仮)
			RedoSetSurface((Si3+2), Si1, 0, Si2, Spb);					// 曲面制御点生成(仮)
			Tracker1.Setup(&Spnt1, &Si4, NULL, 1, 5, 0);				// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			Sphase=(Tracker1.HitTracker(1, plane, pnt, &Spnt1, 1)!=-1)
                                                         ? 2 : Sphase;	// ﾄﾗｯｶｰ1ﾁｪｯｸ
		} 
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		Sphase = (Sphase==2) ? 1 : Sphase;
		Spnt2 = Spnt1;													// 錘頂点座標保存
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si4 = 0, Si6 = 0;
			m_SolidFlg = m_AtrEnv;										// 生成曲面属性
			Si7 = m_SecEnv;												// 生成端面形状
			if(m_FChar&0x1) {											// [F5]:生成属性反転
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:端面形状反転
				Si7 = (Si7) ? 0 : 1;
			}
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==1&&cmp!=NULL) {									// <曲線の場合>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				Display(0);												// 再描画
				Si1 = GetParaInt(cmp, 0);								// 制御点数取得
				Si2 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spa[i]);						// 曲線制御点取得
				}
				SetCurve(Si1, Si2, Spa);								// 曲線制御点生成
				GetObjPtr(&Sop2);										// 制御点OP保存
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// 立体編集後制御点取得
				}
				FreeObj(Sop2, 1);										// 編集後立体の削除
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si5)) {			// 生成初期処理
					CheckCtrlKey1();									// 制御ｷｰ確認(起動時)
					if(m_NumInp!=0) {									// <数値入力の場合>
						CInputDlg dlg(22);
						dlg.SetValue(m_PyramidDivi);					// 既存値表示
						if(dlg.DoModal()==IDOK) {						// ﾀﾞｲｱﾛｸﾞ表示
							dlg.GetValue(&m_PyramidDivi);				// 錐分割数取得
							Si3 = m_PyramidDivi;
						} else {
							Si3 = 0;
						}
					} else {
						Si3 = 1;										// 指定無は錐分割数:1
					}
					if(Si3>0) {
						OffUndoRedo();									// Undo/Redo不可
						StartMenu();									// ﾒﾆｭｰ開始処理
						InitLevel();									// Undo/Redo初期化
						InitPyramid(Si5, Si1, Spa, &Spnt1);				// 錘初期初期
						i1 = GetVtxNum(Sop1);							// 曲面頂点数取得
						for(i=0; i<i1; i++) {
							GetVtx(Sop1, i, &Spb[i]);					// 曲面頂点座標取得
						}
						Si6 = GetCurveCenter(Si5, Si2, i1, Spb, &pnt);	// 2D図形中心取得
						Si6 = ((!m_SolidFlg)||Si7) ? 0 : Si6;			// OFFは断面無
						if(Si6) {
							Spa[Si1] = pnt;								// 2D中心追加
						}
						Pyramid(0, Si5, Si1, Spa, Sia, Spnt1,
                                Si3, Spb, Sib);							// 錘制御点生成(初回)
						Spnt2 = Spnt1;									// 錘頂点座標保存
						CrtSurface(Si3+2, Si1, 0, Si2, m_CurveRepeat,
                                   m_CurveWeight, m_SolidFlg, Spb, Sib);// 曲面生成(初回)
						GetObjPtr(&Sop1);								// 曲面OP保存
						SetSurface(Si3+2, Si1, 0, Si2, Spb);			// 曲面制御点生成(初回)
						GetObjPtr(&Sop2);								// 制御点OP保存
						SetSelNo(Sop2, 2);								// 黄色表示OP設定
						Tracker1.Setup(&Spnt1, &Si4, NULL, 1, 5, 0);	// ﾄﾗｯｶｰ1設定
						Display(0);										// 再描画
						Sphase = 1;
					} else {											// 中止の場合
						ExitMenu(0, 0);									// ﾒﾆｭｰ終了
						Sop1 = NULL, Sop2 = NULL;
					}
				} else {												// <存在平面:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// ｴﾗｰﾒｯｾｰｼﾞ
					ExitMenu(0, 0);										// ﾒﾆｭｰ終了
					Sop1 = NULL, Sop2 = NULL;
				}
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
				Sop1 = NULL, Sop2 = NULL;
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==1) {
			i1 = Si6+1;
			Pyramid(i1, Si5, Si1, Spa, Sia, Spnt1, Si3, Spb, Sib);		// 錘制御点生成(本)
			RedoCrtSurface(Si3+Si6+2, Si1, 0, Si2, Spb, Sib);			// 曲面生成(本)
			i1 = GetObjAtr(Sop1);										// 立体属性取得
			GetObjOif(Sop1, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME11);							// 体構成名(通常錐体)
			} else {
				SetOifName(0, oif, OBJ_NAME12);							//         (通常錐体[面体])
			}
			FreeObj(Sop2, 1);											// 立体削除
			Delete();													// 2D図形削除
			OnEdtFlg(1, Sop1);											// 曲面edtflg:ON
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			OnUndoRedo();												// Undo/Redo可
			Display(0);													// 再描画
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
		}
	}
}

/******************************************************************/
void CJcad3GlbView::MouseSetSection(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 曲面生成(曲線断面割付)
/******************************************************************/
{
	OBJTYPE* op, * op1, * op2;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	VECTYPE  vc1, vc2;
	BOXTYPE  box;
	int      i, i1, i2, i3, err=0;
	double   d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(!Si4) {														// 立体選択
			ObjSelect(flg, point, plane, ms);
		} else if(Sphase==1&&(Si5==0||Si13==plane)) {					// <制御点移動の場合>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			if(plane==0) {
				pnt.p[2] = Spx[Si12].p[2];
			} else if(plane==1) {
				pnt.p[0] = Spx[Si12].p[0];
			} else if(plane==2) {
				pnt.p[1] = Spx[Si12].p[1];
			}
			Spx[Si12] = pnt;											// 通過点位置変更
			if(Si12<Si7) {												// <掃引線1側の変更>
				CalcCurvePoint(1, Si7, Spx, Spa, Sdb);					// 通過点=>制御点変換
				for(i1=0; i1<Si7; i1++) {
					Spy[i1] = Spa[i1], Siy[i1] = Sia[i1];				// 制御点情報複写
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spy, Siy);									// 曲線生成
			} else {													// <掃引線2側の変更>
				CalcCurvePoint(1, Si7, &Spx[Si7], Spb, Sdb);			// 通過点=>制御点変換
				for(i1=0; i1<Si7; i1++) {
					Spz[i1] = Spb[i1], Siy[i1] = Sib[i1];				// 制御点情報複写
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spz, Siy);									// 曲線生成
			}
			Si3 = 0, Si9 = 0, Si10 = 0;
			for(i=0; i<Si7; i++) {
				Siz[i] = 0;												// 割付ﾌﾗｸﾞ初期化
			}
			for(i=0; i<Si11; i++) {
				Six[i] = 3;												// ﾄﾗｯｶｰ色初期化
			}
			SetBaseData(Si6, i1, Si8, Spy, Spz, 3);						// 基準線情報登録
			Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);					// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		if((Si12=Tracker1.HitTracker(1, plane, pnt, Spx, Si11))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
			Si4 = 1, i1 = 0;											// ﾄﾗｯｶｰｸﾘｯｸあり
			BaseObjPtr(2, &op, &i2, &i3);								// 編集対象立体取得
			while(op!=NULL) {											// <編集立体有の場合>
				op1 = op;												// OP保存&ｶｳﾝﾄ
				i1++;
				NextObjPtr(2, &op, &i2, &i3);							// 次立体取得
			}
			if(i1==1) {													// <断面立体数:1の場合>
				BaseCmdPtr(1, op1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i2 = GetObjAtr(op1);									// 立体属性取得
				if(i2/10==1&&cmp!=NULL) {
					if(Si3==0) {										// <1回目の断面割付の場合>
						Si9 = GetParaInt(cmp, 0);						// 制御点数取得
						Si10 = GetParaInt(cmp, 1);						// 境界ﾓｰﾄﾞ取得
						err = (Si7*Si9>m_MaxSCtl) ? 2 : err;			// 制御点数ｵｰﾊﾞｰ
						err = (Si5&&Si10==1) ? 1 : err;					// ｿﾘｯﾄﾞで境界:閉はｴﾗｰ
					} else {											// <2度目以降の断面割付の場合>
						err = (GetParaInt(cmp, 0)!=Si9) ? 1 : err;		// 制御点数ﾁｪｯｸ
						err = (GetParaInt(cmp, 1)!=Si10) ? 1 : err;		// 境界ﾓｰﾄﾞﾁｪｯｸ
					}
					if(!err) {											// <正常な曲面の場合>
						for(i=0; i<Si9; i++) {
							Siw[i] = GetParaInt(cmp, 4+i);				// 曲線ﾓｰﾄﾞ取得
							GetParaPnt(cmp, i, &Spw[i]);				// 曲線制御点取得
						}
						SetCurve(Si9, Si10, Spw);						// 断面曲線制御点生成
						GetObjPtr(&op);									// 断面制御点OP保存
						BaseCmdPtr(1, op1, &cmp);						// 断面有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 断面曲線編集後ｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							RedoCommandCopy(op, cmp);					// 断面ｺﾏﾝﾄﾞ複写実行
							NextCmdPtr(&cmp);							// 断面次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<Si9; i++) {
							GetVtx(op,i,&Spw[i]);						// 断面立体編集後制御点取得
						}
						FreeObj(op, 1);									// 断面編集後立体削除
					}
				} else {
					err = 1;											// 正常な曲線以外はｴﾗｰ
				}
			}
			if(i1==1&&err==0) {											// <<正常な1本の曲線の場合>>
				GetObjBox(op1, &box);									// 立体BOXｻｲｽﾞ取得
				err = (fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON&&
                       fabs(box.bp[0].p[1]-box.bp[1].p[1])>EPSILON&&
                       fabs(box.bp[0].p[2]-box.bp[1].p[2])>EPSILON)
                      ? 3 : err;										// 3面図上に存在なしはｴﾗｰ
			}
			if(i1==0) {													// <<断面選択なしの場合>>
				Sbase = Spx[Si12], Sphase = 1;							// 制御点移動処理へ
			} else if(i1==1&&err==0) {									// <<正常な1本の曲線の場合>>
				if(Si6==1&&Si5!=1) {									// 掃引線1本の場合
					i2 = InputBasePoint1(&m_SecBase2);					// 断面生成位置設定1
				} else if(Si6==2) {										// 掃引線2本の場合
					i2 = InputBasePoint2(&m_SecBase3a,
                                         &m_SecBase3b, &m_SecBase3c);	// 断面生成位置設定2
				} else {
					i2 = 1;
				}
				i1 = (!i2) ? 0 : i1;									// ｷｬﾝｾﾙは処理中止
			}
			if(i1==1&&err==0) {											// <<正常な1本の曲線の場合>>
				SetSelNo(op1, 9);										// 断面曲線登録
				Si12 = (Si12>=Si7) ? Si12 - Si7 : Si12;					// 2本目位置は1本目位置に変換
				SetCurveSection(Si6, Si5, Si12, m_SecBase2, m_SecBase3a,
                                m_SecBase3b, box, Si9, Si10,
                                Spw, Siw, Sps, Sis);					// 曲線断面割付
				Si3 = 1, Siz[Si12] = m_SecBase3c+1;						// 断面割付済み
				Six[Si12] = 5;											// ﾄﾗｯｶｰ色変更
				Six[Si7+Si12] = (Si6>1) ? 5 : Six[Si7+Si12];
				Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);				// ﾄﾗｯｶｰ1設定
				Display(0);												// 再描画
			} else if(i1>1||err==1) {									// <<2本以上か、ｴﾗｰ発生>>
				AfxMessageBox(IDS_ERR_SEL9);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else if(err==2) {											// <<制御点上限ｵｰﾊﾞｰ>>
				AfxMessageBox(IDS_MANY_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else if(err==3) {											// <<3面図上に存在なし>>
				AfxMessageBox(IDS_ERR_PLANE);							// ｴﾗｰﾒｯｾｰｼﾞ
			}
		} else {
			Si4 = 0;													// ﾄﾗｯｶｰｸﾘｯｸなし
			ObjSelect(flg, point, plane, ms);							// 立体選択
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(!Si4) {
			ObjSelect(flg, point, plane, ms);							// 立体選択
		} else if(Sphase==1) {
			Sphase = 0;
		}
		m_EdtCount += Si6;												// 編集対象立体数加算(仮)
	} else if(ms==11) {													// <<<DEL>>>
		Si3  = 0, Si9  = 0, Si10 = 0;
		for(i=0; i<Si7; i++) {
			Siz[i] = 0;													// 割付ﾌﾗｸﾞ初期化
		}
		for(i=0; i<Si11; i++) {
			Six[i] = 3;													// ﾄﾗｯｶｰ色初期化
		}
		SelBaseObjPtr(9, &op);											// 断面登録立体取得
		while(op!=NULL) {
			SetSelNo(op, 0);											// 断面登録立体解除
			SelNextObjPtr(9, &op);										// 次登録立体へ
		}
		Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);						// ﾄﾗｯｶｰ1設定
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
		Si2 = 0, Si3 = 0, Si4 = 0, Si5 = 0, Si6 = 0;
		Si7 = 0, Si8 = 0, Si9 = 0, Si10 = 0, Si11 = 0;
		m_SolidFlg = m_AtrEnv;											// 生成曲面属性
		Si1 = m_SecEnv;													// 生成端面形状
		if(m_FChar&0x1) {												// [F5]:生成属性反転
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {												// [F6]:端面形状反転
			Si1 = (Si1) ? 0 : 1;
		}
		Si14 = (m_FChar&0x8) ? 0 : 1;									// [F8]:削除なし
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {												// <<編集立体有の場合>>
			if(Si6>1) {													// 掃引線3本以上はｴﾗｰ
				err = 1; break;
			}
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			i3 = GetObjAtr(op);											// 立体属性取得
			if(i3/10==1&&cmp!=NULL) {
				if(Si6==0) {											// <1本目の曲線の場合>
					op1 = op;											// 1本目掃引線曲線立体
					Si7 = GetParaInt(cmp, 0);							// 制御点数取得
					Si8 = GetParaInt(cmp, 1);							// 境界ﾓｰﾄﾞ取得
					for(i=0; i<Si7; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i, &Spa[i]);					// 曲線制御点取得
					}
				} else {												// <2本目の曲線の場合>
					op2 = op;											// 2本目掃引線曲線立体
					if(GetParaInt(cmp, 0)!=Si7) {						// 制御点数ﾁｪｯｸ
						err = 1; break;
					}
					if(GetParaInt(cmp, 1)!=Si8) {						// 境界ﾓｰﾄﾞﾁｪｯｸ
						err = 1; break;
					}
					for(i=0; i<Si7; i++) {
						Sib[i] = GetParaInt(cmp, 4+i);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i, &Spb[i]);					// 曲線制御点取得
					}
				}
			} else {													// 曲線以外はｴﾗｰ
				err = 1; break;
			}
			NextObjPtr(2, &op, &i1, &i2);								// 次立体取得
			Si6++;
		}
		if(!err&&Si6==1&&Si8==1) {										// <<掃引線が1本で閉の場合>>
			CRevolveDlg dlg;
			if(dlg.DoModal()==IDOK) {									// 回転体種類設定
				dlg.GetValue(&Si5);
			}
			if(Si5==1) {												// <ｿﾘｯﾄﾞの場合>
				GetObjBox(op1, &box);									// 立体BOXｻｲｽﾞ取得
				err = (fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON&&
                       fabs(box.bp[0].p[1]-box.bp[1].p[1])>EPSILON&&
                       fabs(box.bp[0].p[2]-box.bp[1].p[2])>EPSILON)
                      ? 2 : err;										// 3面図上に存在なしはｴﾗｰ
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {		// ｿﾘｯﾄﾞ掃引線指定平面
					Si13 = 1;
				} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
					Si13 = 2;
				} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
					Si13 = 0;
				}
			}
		}
		if(!err) {														// <<正常な曲線の場合>>
			SetCurve(Si7, Si8, Spa);									// 1本目曲線制御点生成
			GetObjPtr(&op);												// 1本目制御点OP保存
			BaseCmdPtr(1, op1, &cmp);									// 1本目有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 1本目曲線編集後ｺﾏﾝﾄﾞ
			while(cmp!=NULL) {
				RedoCommandCopy(op, cmp);								// 1本目ｺﾏﾝﾄﾞ複写実行
				NextCmdPtr(&cmp);										// 1本目次ｺﾏﾝﾄﾞへ
			}
			for(i=0; i<Si7; i++) {
				GetVtx(op, i, &Spa[i]);									// 1本目立体編集後制御点取得
			}
			FreeObj(op, 1);												// 1本目編集後立体削除
			if(Si6>1) {													// <2本目がある場合>
				SetCurve(Si7, Si8, Spb);								// 2本目曲線制御点生成
				GetObjPtr(&op);											// 2本目制御点OP保存
				BaseCmdPtr(1, op2, &cmp);								// 2本目有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 2本目曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(op, cmp);							// 2本目ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 2本目次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si7; i++) {
					GetVtx(op, i, &Spb[i]);								// 2本目立体編集後制御点取得
				}
				FreeObj(op, 1);											// 2本目編集後立体削除
			}
		}
		if(!err&&Si5==1) {												// <正常なｿﾘｯﾄﾞの場合>
			CalcNVec(Spa, Si7, &vc1);									// ｿﾘｯﾄﾞ曲線法線ﾍﾞｸﾄﾙ取得
			i = (Si13-1<0) ? 2 : Si13-1;								// 法線ﾍﾞｸﾄﾙ方向
			if(vc1.p[i]>0) {
				ChangeLoop(Si7, Spa, Sia);								// 面ﾙｰﾌﾟ反転
			}
		}
		if(!err&&Si6>1) {												// <<正常な曲線で2本の場合>>
			i1 = 0;
			if(!Si8) {													// <境界:開の場合>
				i1 = (PntD(&Spa[0], &Spb[0])>PntD(&Spa[0], &Spb[Si7-1]))// 1本目の始点に対し
					 ? 1 : i1;											// 2本目の始点より終点が近い
			} else {													// <境界:閉の場合>
				CalcNVec(Spa, Si7, &vc1);								// 1本目曲線法線ﾍﾞｸﾄﾙ取得
				CalcNVec(Spb, Si7, &vc2);								// 2本目曲線法線ﾍﾞｸﾄﾙ取得
				for(i=0, i2=0, d1=0.0; i<DIM; i++) {
					if((d2=fabs(vc1.p[i]))>d1) {						// 1本目法線ﾍﾞｸﾄﾙの最大方向
						d1 = d2, i2 = i;
					}
				}
				i1 = ((vc1.p[i2]>0&&vc2.p[i2]<0)||
                      (vc1.p[i2]<0&&vc2.p[i2]>0)) ? 1 : i1;				// 法線ﾍﾞｸﾄﾙ方向が異なる
			}
			if(i1) {
				ChangeLoop(Si7, Spb, Sib);								// 向きが逆は2本目面ﾙｰﾌﾟ反転
			}
			for(i=0; i<Si7; i++) {
				if(Sia[i]!=Sib[i]) {									// 曲線ﾓｰﾄﾞﾁｪｯｸ
					err = 1; break;
				}
			}
		}
		if(!err) {														// <<正常な曲線の場合>>
			Si2 = 1;													// 処理開始済み
			OffUndoRedo();												// Undo/Redo不可
			SetSelNo(op1, 1);											// 1本目掃引線立体登録
			OffEdtFlg(1, op1);											// 1本目掃引線立体非選択
			OffDspFlg(1, op1);											// 1本目掃引線立体非表示
			for(i1=0; i1<Si7; i1++) {
				Spy[i1] = Spa[i1], Siy[i1] = Sia[i1];					// 制御点情報複写
			}
			MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                       Spy, Siy);										// 曲線生成
			SetCurveMatrix3(1, m_CurveWeight, Si8, Si7, Sia, Sda, Sdb);	// 制御点=>通過点変換ﾏﾄﾘｯｸｽ
			CalcCurvePoint(1, Si7, Spa, Spx, Sda);						// 制御点=>通過点変換
			for(i=0; i<Si7; i++) {
				Six[i] = 3, Siz[i] = 0;									// ﾄﾗｯｶｰ色&割付ﾌﾗｸﾞ初期化
			}
			Si11 = Si7;													// ﾄﾗｯｶｰ数設定
			if(Si6>1) {													// <2本目がある場合>
				SetSelNo(op2, 1);										// 2本目掃引線立体登録
				OffEdtFlg(1, op2);										// 2本目掃引線立体非選択
				OffDspFlg(1, op2);										// 2本目掃引線立体非表示
				for(i1=0; i1<Si7; i1++) {
					Spz[i1] = Spb[i1], Siy[i1] = Sib[i1];				// 制御点情報複写
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spz, Siy);									// 曲線生成
				CalcCurvePoint(1, Si7, Spb, &Spx[Si7], Sda);			// 制御点=>通過点変換
				for(i=0; i<Si7; i++) {
					Six[i+Si7] = 3;										// ﾄﾗｯｶｰ色初期化
				}
				Si11 += Si7;											// ﾄﾗｯｶｰ数加算
			}
			SetBaseData(Si6, i1, Si8, Spy, Spz, 3);						// 基準線情報登録
			Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);					// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
			m_CursorNo = 1;												// ｶｰｿﾙNo設定
		} else {														// <<異常な立体の場合>>
			if(err==1) {
				AfxMessageBox(IDS_ERR_SEL9);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else {
				AfxMessageBox(IDS_ERR_PLANE);
			}
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Si2==1) {													// <処理開始済みの場合>
			OffEdtAllObject();											// 編集立体全解除
			if(!Si3) {													// <断面未割付の場合>
				if(Si5) {												// 回転体(ｿﾘｯﾄﾞ)はV:開
					Si9  = 5, Si10 = 0;
				} else {												// 以外はV:閉
					Si9  = 8, Si10 = 1;
				}
				SetCircleSection(Si6, Si5, m_Fovy/12000.0, Si7, Si9,
                                 Spa, Spb, Sps, Sis);					// 円断面割付
			} else {													// <断面割付済の場合>
				CalcCurveSection(Si6, Si5, Si7, Si8, Si9, Si10,
                                 Spa, Spb, Siz, Sps, Sis);				// 曲線割付断面計算
			}
			CrtSectionSweep(Si6, Si5, Si7, Si8, Si9, Si10, Spa,
                            Spb, Sia, Sps, Sis, Spc, Sic);				// 割付断面掃引体生成
			if(!Si8&&Si10&&m_SolidFlg&&!Si1) {							// <U:開,V:閉,ｿﾘｯﾄﾞ:ON,1端面:OFF>
				AddCenterPoint(&Si7, Si9, Si10, Spc, Sic);				// 端面中心点追加
			}
			StartMenu();												// ﾒﾆｭｰ開始処理
			InitLevel();												// Undo/Redo初期化
			SelBaseObjPtr(1, &op);										// 掃引線立体取得
			while(op!=NULL) {
				op1 = op;												// 削除立体ﾎﾟｲﾝﾀ保存
				SelNextObjPtr(1, &op);									// 次掃引線立体へ
				LevelObj(1, op1);										// 同一ﾚﾍﾞﾙ削除立体接続
				TempObj(op1);											// 立体一時削除
			}
			if(Si14) {													// <断面登録立体削除あり>
				SelBaseObjPtr(9, &op);									// 断面登録立体取得
				while(op!=NULL) {
					op1 = op;											// 削除立体ﾎﾟｲﾝﾀ保存
					SelNextObjPtr(9, &op);								// 次断面登録立体へ
					LevelObj(1, op1);									// 同一ﾚﾍﾞﾙ削除立体接続
					TempObj(op1);										// 立体一時削除
				}
			}
			CrtSurface(Si7, Si9, Si8, Si10, m_CurveRepeat,
                       m_CurveWeight, m_SolidFlg, Spc, Sic);			// 通常曲線生成
			GetObjPtr(&op);												// 通常曲面OP取得
			i1 = GetObjAtr(op);											// 立体属性取得
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME03);							// 体構成名(通常掃引体)
			} else {
				SetOifName(0, oif, OBJ_NAME04);							//         (通常掃引体[面体])
			}
			OnEdtFlg(1, op);											// 通常曲面edtflg:ON
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			DeleteBaseData();											// 基準線情報削除
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			OnUndoRedo();												// Undo/Redo可
			Display(0);													// 再描画
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		}
	}
}

/**************************************/
void CJcad3GlbView::MenuSetCurve(void)									//<<< 曲面生成(曲線組合せ)
/**************************************/
{
	OBJTYPE* op, * dop;
	BOXTYPE  box;
	int      i, i1, i2, cnt1=0, cnt2=0, err=0, gno[32];

	if(GetSelectNo()==0) {												// <<2D曲線OP無>>
		m_SolidFlg = m_AtrEnv;                                          // 生成曲面属性
		Si1 = m_SecEnv;												    // 生成端面形状
		if(m_FChar&0x1) {											    // [F5]:生成属性反転
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {											    // [F6]:端面形状反転
			Si1 = (Si1) ? 0 : 1;
		}
		m_SweepFlg = (m_FChar&0x4) ? 1 : 0;								// [F7]:掃引曲面
		Si3 = (m_FChar&0x8) ? 0 : 1;									// [F8]:削除なし
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			GetObjBox(op, &box);										// 立体BOXｻｲｽﾞ取得
			SetSelNo(op, 1);											// 断面曲線立体登録
			err = (GetObjAtr(op)/10!=1) ? 1 : err;						// 曲線以外はｴﾗｰ
			if(cnt1==0) {
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {		// 曲線存在平面ﾁｪｯｸ
					i = 0, Si2 = 1;
				} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
					i = 1, Si2 = 2;
				} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
					i = 2, Si2 = 0;
				} else {
					err = 1;
				}
			} else {
				err = (fabs(box.bp[0].p[i]-box.bp[1].p[i])>EPSILON)		// 曲線存在平面ﾁｪｯｸ
                      ? 1 : err;
			}
			if(err) {													// ｴﾗｰは中止
				break;
			}
			cnt1++;														// ｶｳﾝﾄUP
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		if(!err&&cnt1>0&&cnt1<101) {									// <有効な場合>
			OffEdtAllObject();											// 編集立体全解除
		} else {														// <ｴﾗｰの場合>
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
			AfxMessageBox(IDS_ERR_SEL8);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
	} else {															// <<2D曲線OP有>>
		m_SolidFlg = m_AtrEnv;											// 生成曲面属性
		Si1 = m_SecEnv;													// 生成端面形状
		if(m_FChar&0x1) {												// [F5]:生成属性反転
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {												// [F6]:端面形状反転
			Si1 = (Si1) ? 0 : 1;
		}
		m_SweepFlg = (m_FChar&0x4) ? 1 : m_SweepFlg;					// [F7]:掃引曲面
		Si3 = (m_FChar&0x8) ? 0 : Si3;									// [F8]:削除なし
		BaseObjPtr(1, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10!=1) ? 1 : err;					// 曲線以外はｴﾗｰ
				cnt1++;													// 断面曲線ｶｳﾝﾄUP
			} else if(i2==1) {
				SetSelNo(op, 2);										// 掃引曲線立体登録
				err = (GetObjAtr(op)/10!=1) ? 1 : err;					// 曲線以外はｴﾗｰ
				cnt2++;													// 掃引線ｶｳﾝﾄUP
			}
			if(err) {													// ｴﾗｰは中止
				break;
			}
			NextObjPtr(1, &op, &i1, &i2);								// 次編集立体へ
		}
		if(cnt1>0&&cnt2>0&&err==0) {
			OffEdtAllObject();											// 編集立体全解除
			i = 0;
			if(InputBasePoint1(&m_SecBase2)) {							// 断面生成位置取得
				while(TRUE) {
					StartMenu();										// ﾒﾆｭｰ開始処理
					InitLevel();										// Undo/Redo初期化
					CreatSetCurve(i, m_SolidFlg, Si1, m_SweepFlg,
                                  Si2, m_SecBase2);						// 曲線組合せ
					Display(0);											// 再描画
					if(AfxMessageBox(IDS_ROTATE_2D,MB_YESNO)==IDYES){	// 2D断面回転確認?
						EndJMenu1();									// ﾒﾆｭｰ終了処理
						Undo(gno);										// ﾘﾄﾗｲはUndo
						i++;											// 90度回転
					} else {											// 終了
						break;
					}
				}
				BaseObjPtr(1, &op, &i1, &i2);							// 処理対象立体取得
				while(op!=NULL) {
					dop = op;											// 削除立体ﾎﾟｲﾝﾀ保存
					i = GetSelNo(op);									// 選択No取得
					NextObjPtr(1, &op, &i1, &i2);						// 次立体へ
					if((Si3==1&&i==1)||i==2) {							// 断面曲線/掃引曲線
						LevelObj(1, dop);								// 同一ﾚﾍﾞﾙ削除立体接続
						TempObj(dop);									// 立体一時削除
					}
				}
				EndJMenu1();											// ﾒﾆｭｰ終了処理
			}
		} else {
			AfxMessageBox(IDS_ERR_SEL8);								// 曲線以外,ｴﾗｰﾒｯｾｰｼﾞ
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
	}
	CountEdtObj();														// 編集対象立体ｶｳﾝﾄ
	Display(0);															// 再描画
}

/*************************************/
void CJcad3GlbView::MenuSection(void)									//<<< 曲面生成(断面指定)
/*************************************/
{
	OBJTYPE** sop = new OBJTYPE*[1000];
	OBJTYPE*  op, * wop;
	OIFTYPE*  oif;
	CMDTYPE*  cmp;
	VECTYPE   vc1, vc2;
	int       i, j, k, i1, i2, i3, i4, er=0, oct=0, pct=0;

	m_SolidFlg = m_AtrEnv;												// 生成曲面属性
	Si1 = m_SecEnv;														// 生成端面形状
	if(m_FChar&0x1) {													// [F5]:生成属性反転
		m_SolidFlg = (m_SolidFlg) ? 0 : 1;
	}
	if(m_FChar&0x2) {													// [F6]:端面形状反転
		Si1 = (Si1) ? 0 : 1;
	}
	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		i3 = GetObjAtr(op);												// 立体属性取得
		if(i3/10==1&&cmp!=NULL) {										// <曲線の場合>
			if(oct>=1000) {												// 最大曲線数以上中止
				er = 1; break;
			}
			i1 = GetParaInt(cmp, 0);									// 制御点数取得
			i2 = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			if(oct==0) {												// 初回は
				Si2 = i1, Si3 = i2;										// 制御点数/境界ﾓｰﾄﾞ保存
			} else  {
				if(Si2!=i1||Si3!=i2) {									// 制御点数,境界ﾓｰﾄﾞが
					er = 1; break;										// 異なる場合は中止
				}
			}
			for(i=0; i<Si2; i++) {
				Sia[pct+i] = GetParaInt(cmp, 4+i);						// 曲線ﾓｰﾄﾞ取得
				GetParaPnt(cmp, i, &Sps[i]);							// 曲線制御点取得
			}
			SetCurve(Si2, Si3, Sps);									// 作業立体生成
			GetObjPtr(&wop);											// 作業立体OP取得
			NextCmdPtr(&cmp);											// 曲線後ｺﾏﾝﾄﾞ
			while(cmp!=NULL) {
				RedoCommandCopy(wop, cmp);								// ｺﾏﾝﾄﾞ複写実行
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			for(i=0; i<Si2; i++) {
				GetVtx(wop, i, &Spa[pct+i]);							// 立体編集後制御点取得
			}
			FreeObj(wop, 1);											// 作業立体削除
			CalcCent(&Spa[pct], Si2, &Spb[oct]);						// 曲線中心点取得
			Sib[oct] = pct;												// 制御点開始位置
			Sic[oct] = Si2;												// 制御点数
			sop[oct] = op;												// 立体ﾎﾟｲﾝﾀ保存
			pct += Si2;													// 累積制御点数
			oct++;														// 登録曲線数
		} else {														// <以外の場合は中止>
			er = 1; break;
		}
		NextObjPtr(2, &op, &i1, &i2);									// 次編集立体へ
	}
	if(oct>1&&er==0) {													// <<正常なﾃﾞｰﾀ>>
		er = (ConctSEPoint(oct, Spb, Spb, Ssa)) ? 2 : er;				// 始点終点制御点連結
		er = (GetConctObj(oct, Ssa, Ssb)) ? 2 : er;						// 連結立体順取得
	}
	if(oct>1&&er==0) {													// <<正常なﾃﾞｰﾀ>>
		for(i=0; i<DIM; i++) {
			vc1.p[i] = Spb[Ssb[1].p[0]].p[i]-Spb[Ssb[0].p[0]].p[i];		// 開始断面と次断面の方向
		}
		j = Sib[Ssb[0].p[0]];											// 開始断面の制御点開始位置
		CalcNVec(&Spa[j], Si2, &vc2);									// 開始断面法線取得
		if(fabs(vc2.p[0])>0.5) {										// 開始断面法線/平面方向取得
			i1 = 0, i2 = 1;
		} else if(fabs(vc2.p[1])>0.5) {
			i1 = 1, i2 = 2;
		} else {
			i1 = 2, i2 = 0;
		}
		i1 = ((vc1.p[i1]>0&&vc2.p[i1]>0)||(vc1.p[i1]<0&&vc2.p[i1]<0))	// 断面方向と法線方向が
             ? 1 : 0;													// 同じ場合は面を変転
		i4 = GetVtxNum(sop[Ssb[0].p[0]]);								// 開始断面曲面頂点数取得
		for(i=0; i<i4; i++) {
			GetVtx(sop[Ssb[0].p[0]], i, &Sps[i]);						// 開始断面曲面頂点座標取得
		}
		i2 = GetCurveCenter(i2, Si3, i4, Sps, &Spb[Ssb[0].p[0]]);		// 開始断面2D図形中心取得
		j = Sib[Ssb[oct-1].p[0]];										// 終了断面の制御点開始位置
		CalcNVec(&Spa[j], Si2, &vc2);									// 終了断面法線取得
		i3 = (fabs(vc2.p[0])>0.5) ? 1 : ((fabs(vc2.p[1])>0.5) ? 2 : 0);	// 終了断面平面方向取得
		i4 = GetVtxNum(sop[Ssb[oct-1].p[0]]);							// 終了断面曲面頂点数取得
		for(i=0; i<i4; i++) {
			GetVtx(sop[Ssb[oct-1].p[0]], i, &Sps[i]);					// 終了断面曲面頂点座標取得
		}
		i3 = GetCurveCenter(i3, Si3, i4, Sps, &Spb[Ssb[oct-1].p[0]]);	// 終了断面2D図形中心取得
		if((!m_SolidFlg)||Si1) {										// OFFは開始終了断面無
			i2 = 0, i3 = 0;
		}
		for(i=0; i<oct; i++) {											// 各断面毎(反転は開始と同)
			j = Sib[i];													// 各断面の制御点開始位置
			InitCreat2(i1, Si3, Si2, &Spa[j], &Sia[j]);					// 生成初期処理2
		}
		i1 = oct;														// 指定断面数
		i1 = (i2) ? i1+1 : i1;											// 開始/終了断面が中心有は+1づつ
		i1 = (i3) ? i1+1 : i1;
		if(Si2*i1>m_MaxSCtl) {											// <制御点上限より上>
			AfxMessageBox(IDS_MANY_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else {														// <制御点上限以下>
			for(i=0, pct=0; i<Si2; i++) {								// V方向
				if(i2) {												// <開始断面の中心有>
					Sps[pct] = Spb[Ssb[0].p[0]];						// 開始断面中心点
					k = Sib[Ssb[0].p[0]];								// 開始断面の制御点開始位置
					Sis[pct++] = Sia[k+i]+1;							// ﾓｰﾄﾞ設定
				}
				for(j=0; j<oct; j++) {									// U方向
					k = Sib[Ssb[j].p[0]];								// 各断面の制御点開始位置
					Sps[pct] = Spa[k+i];								// 小さい順に制御点設定
					Sis[pct++] = Sia[k+i];								// 小さい順にﾓｰﾄﾞ設定
					if(j==0||j==oct-1) {								// U両端ﾓｰﾄﾞは1
						Sis[pct-1]++;
					}
				}
				if(i3) {												// <終了断面の中心有>
					Sps[pct] = Spb[Ssb[oct-1].p[0]];					// 終了断面中心点
					k = Sib[Ssb[oct-1].p[0]];							// 終了断面の制御点開始位置
					Sis[pct++] = Sia[k+i]+1;							// ﾓｰﾄﾞ設定
				}
			}
			StartMenu();												// ﾒﾆｭｰ開始処理
			InitLevel();												// Undo/Redo初期化
			CrtSurface(i1, Si2, 0, Si3, m_CurveRepeat,
                       m_CurveWeight, m_SolidFlg, Sps, Sis);			// 曲面生成
			GetObjPtr(&op);												// 曲面立体OP取得
			i = GetObjAtr(op);											// 立体属性取得
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(i==20) {
				SetOifName(0, oif, OBJ_NAME03);							// 体構成名(通常掃引体)
			} else if(i==21) {
				SetOifName(0, oif, OBJ_NAME04);							//         (通常掃引体[面体])
			} else if(i==22) {
				SetOifName(0, oif, OBJ_NAME05);							//         (掃引掃引体)
			} else {
				SetOifName(0, oif, OBJ_NAME06);							//         (掃引掃引体[面体])
			}
			Delete();													// 2D図形削除
			OnEdtFlg(1, op);											// 曲面edtflg:ON
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			Display(0);													// 再描画
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		}
	} else if(er==2) {
		AfxMessageBox(IDS_ERR_CONECT);									// ｴﾗｰﾒｯｾｰｼﾞ
	} else {
		AfxMessageBox(IDS_ERR_SEL9);									// ｴﾗｰﾒｯｾｰｼﾞ
	}
	delete[] sop;
}

/************************************/
void CJcad3GlbView::MenuSpiral(void)									//<<< 曲面生成(螺旋)
/************************************/
{
	OBJTYPE* op, * op2;
	CMDTYPE* cmp;
	BOXTYPE  box;
	int      i, i1, i2, i3, cnt=0, err=0;

	m_SolidFlg = m_AtrEnv;												// 生成曲面属性
	Si1 = m_SecEnv;														// 生成端面形状
	if(m_FChar&0x1) {													// [F5]:生成属性反転
		m_SolidFlg = (m_SolidFlg) ? 0 : 1;
	}
	if(m_FChar&0x2) {													// [F6]:端面形状反転
		Si1 = (Si1) ? 0 : 1;
	}
	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	while(op!=NULL) {
		GetObjBox(op, &box);											// 立体BOXｻｲｽﾞ取得
		err = (GetObjAtr(op)/10!=1) ? 1 : err;							// 曲線以外はｴﾗｰ
		if(cnt==0) {
			if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {			// 曲線存在平面ﾁｪｯｸ
				Si2 = 1;
			} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
				Si2 = 2;
			} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
				Si2 = 0;
			} else {
				err = 2;
			}
		} else {
			err = 1;
		}
		if(err) {														// ｴﾗｰは中止
			break;
		}
		cnt++;															// ｶｳﾝﾄUP
		NextObjPtr(2, &op, &i1, &i2);									// 次編集立体へ
	}
	if(err==0) {														// <有効な場合>
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		if(op!=NULL) {													// <編集立体有の場合>
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			i3 = GetObjAtr(op);											// 立体属性取得
		}
		if(i3/10==1&&cmp!=NULL) {										// <曲線の場合>
			Si3 = GetParaInt(cmp, 0);									// 制御点数取得
			Si4 = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			for(i=0; i<Si3; i++) {
				Sia[i] = GetParaInt(cmp, 4+i);							// ﾗｲﾝﾓｰﾄﾞ取得
				GetParaPnt(cmp, i, &Spa[i]);							// 曲線制御点取得
			}
			i1 = GetVtxNum(op);											// 曲面頂点数取得
			for(i=0; i<i1; i++) {
				GetVtx(op, i, &Spb[i]);									// 曲面頂点座標取得
			}
			Si5 = GetCurveCenter(Si2, Si4, i1, Spb, &Spnt1);			// 2D図形中心取得
			Si5 = ((!m_SolidFlg)||Si1) ? 0 : Si5;						// OFFは断面無
			SetCurve(Si3, Si4, Spa);									// 曲線制御点生成
			GetObjPtr(&op2);											// 制御点OP保存
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 曲線編集後ｺﾏﾝﾄﾞ
			while(cmp!=NULL) {
				RedoCommandCopy(op2, cmp);								// ｺﾏﾝﾄﾞ複写実行
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			for(i=0; i<Si3; i++) {
				GetVtx(op2, i, &Spa[i]);								// 立体編集後制御点取得
			}
			FreeObj(op2, 1);											// 編集後立体削除
		} else {
			err = 1;													// 曲線ｴﾗｰ
		}
	}
	if(err==1) {
		AfxMessageBox(IDS_ERR_SEL8);									// ｴﾗｰﾒｯｾｰｼﾞ
	} else if(err==2) {
		AfxMessageBox(IDS_ERR_PLANE);
	} else {															// <有効な場合>
		CSpiralDlg dlg;
		dlg.SetValue(m_SPDiameter, m_SPHeight,
                     m_SPCtrlNum, m_SPSprlNum);							// 既存値表示
		while(TRUE) {
			if(dlg.DoModal()==IDOK) {									// 螺旋情報設定ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(&m_SPDiameter, &m_SPHeight,
                             &m_SPCtrlNum, &m_SPSprlNum);				// 螺旋情報取得
				Sd1 = m_SPDiameter, Sd2 = m_SPHeight;
				Si8 = m_SPCtrlNum, Si9 = m_SPSprlNum;
			} else {
				err = 3; break;											// 処理中止
			}
			if(2*Si5+Si8*Si9+1>m_MaxCCtl||
               Si3*(2*Si5+Si8*Si9+1)>m_MaxSCtl) {						// 制御点数ｵｰﾊﾞｰは
				AfxMessageBox(IDS_MANY_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ後にﾘﾄﾗｲ
			} else {
				break;													// 正常は終了
			}
		}
	}
	if(err==0) {														// <有効な場合>
		GetCentPnt(&Spnt2);												// 画面の中心取得
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		Delete();														// 2D図形削除
		InitCreat(1, Si4, Si3, Spa, Sia, &i1);							// 生成初期処理
		CreatSpiralObject(Si2, Si3, Si4, Si5, Sd1, Sd2, Si8, Si9,
                          Spnt1, Spnt2, Spa, Sia);						// 螺旋曲面生成
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		CountEdtObj();													// 編集対象立体ｶｳﾝﾄ
		Display(0);														// 再描画
	}
}

/******************************************/
void CJcad3GlbView::MenuResetSurface(void)								//<<<  曲面生成(再作成)
/******************************************/
{
	char*    name = new char[24];
	OBJTYPE* op, * cop, * dop;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	int      cpf, cmf, un, vn, ub, vb, mx, wt, fg, ct, i, j, i1, i2, i3, mir, err=0;
	double   dwt;

	cpf = (m_FChar&0x1) ? 1 : 0;										// [F5]:生成時形状
	cpf = (m_FChar&0x2) ? 2 : cpf;										// [F6]:生成位置
	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		i3 = GetObjAtr(op);												// 立体属性取得
		err = (cmp==NULL) ? 1 : err;									// ｺﾏﾝﾄﾞ無効はｴﾗｰ
		err = (i3/10!=1&&i3!=20&&i3!=21) ? 1 : err;						// 曲線･通常曲面以外はｴﾗｰ
		if(err) {														// ｴﾗｰは中止
			break;
		}
		SetSelNo(op, 1);												// 再作成立体登録
		NextObjPtr(2, &op, &i1, &i2);									// 次編集立体へ
	}
	if(!err) {															// <<正常な場合>>
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		SelBaseObjPtr(1, &op);											// 再作成立体先頭ﾎﾟｲﾝﾀ取得
		while(op!=NULL) {
			cmf = (cpf==1) ? 0 : 1;										// ｺﾏﾝﾄﾞ検索(0:先頭,1:有効先頭)
			BaseCmdPtr(cmf, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			i3 = GetObjAtr(op);											// 立体属性取得
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			GetOifName(oif, name);										// 立体構成名取得
			if(i3==10||i3==11) {										// <曲線の場合>
				un = GetParaInt(cmp, 0);								// 制御点数取得
				ub = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				mx = GetParaInt(cmp, 2);								// 曲線化繰返数
				wt = GetParaInt(cmp, 3);								// 曲線重み係数
				dwt = wt/1000.0;
				for(i=0, ct=0; i<un; i++) {
					GetParaPnt(cmp, i, &Spa[ct]);						// 制御点取得
					Sia[ct++] = GetParaInt(cmp, i+4);					// ﾗｲﾝﾓｰﾄﾞ取得
				}
				if(cpf==0) {											// <現在位置で再生成>
					SetCurve(un, ub, Spa);								// 曲線制御点生成
					GetObjPtr(&dop);									// 制御点OP保存
					NextCmdPtr(&cmp);									// 曲線編集後ｺﾏﾝﾄﾞ
					while(cmp!=NULL) {
						RedoCommandCopy(dop, cmp);						// ｺﾏﾝﾄﾞ複写実行
						NextCmdPtr(&cmp);								// 次ｺﾏﾝﾄﾞへ
					}
					for(i=0; i<un; i++) {
						GetVtx(dop, i, &Spa[i]);						// 立体編集後制御点取得
					}
					FreeObj(dop, 1);									// 立体削除
				}
				CrtCurve(un, ub, mx, dwt, Spa, Sia);					// 曲線生成
				GetObjPtr(&cop);										// 生成立体取得
			} else {													// <通常曲面の場合>
				un = GetParaInt(cmp, 0);								// U制御点数取得
				vn = GetParaInt(cmp, 1);								// V制御点数取得
				ub = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
				vb = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
				mx = GetParaInt(cmp, 4);								// 曲線化繰返数
				wt = GetParaInt(cmp, 5);								// 曲線重み係数
				dwt = wt/1000.0;
				fg = GetParaInt(cmp, 6);								// ｿﾘｯﾄﾞﾓｰﾄﾞ
				for(i=0, ct=0; i<vn; i++) {
					for(j=0; j<un; j++) {
						GetParaPnt(cmp, un*i+j, &Spa[ct]);				// 制御点取得
						Sia[ct++] = GetParaInt(cmp, un*i+j+7);			// ﾓｰﾄﾞ取得
					}
				}
				if(cpf==0) {											// <現在位置で再生成>
					SetSurface(un, vn, ub, vb, Spa);					// 曲面制御点生成
					GetObjPtr(&dop);									// 制御点OP保存
					NextCmdPtr(&cmp);									// 曲面編集後ｺﾏﾝﾄﾞ
					mir = 0;
					while(cmp!=NULL) {
						if(GetCmdNo(cmp)==MIRROR) {						// 鏡像ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
							mir++;
						}
						if(GetCmdNo(cmp)!=THICKNESS) {					// 厚み付け以外の
							RedoCommandCopy(dop, cmp);					// ｺﾏﾝﾄﾞ複写実行
						}
						NextCmdPtr(&cmp);								// 次ｺﾏﾝﾄﾞへ
					}
					j = un*vn;
					for(i=0; i<j; i++) {
						GetVtx(dop, i, &Spa[i]);						// 立体編集後制御点取得
					}
					FreeObj(dop, 1);									// 立体削除
					if(mir%2==1) {
						ReverseSurf(un, vn, Spa, Sia);					// 曲面制御点反転
					}
				}
				CrtSurface(un, vn, ub, vb, mx, dwt, fg, Spa, Sia);		// 曲面生成
				GetObjPtr(&cop);										// 生成立体取得
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)==THICKNESS) {						// 厚み付けの
						RedoCommandCopy(cop, cmp);						// ｺﾏﾝﾄﾞ複写実行
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
			}
			GetObjOif(cop, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			SetOifName(0, oif, name);									// 立体構成名設定
			OnEdtFlg(1, cop);											// edtflg:ON
			dop = op;													// 削除立体ﾎﾟｲﾝﾀ保存
			SelNextObjPtr(1, &op);										// 次再作成立体
			LevelObj(1, dop);											// 同一ﾚﾍﾞﾙ削除立体接続
			TempObj(dop);												// 立体一時削除
		}
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	} else {
		AfxMessageBox(IDS_ERR_SEL5);									// ｴﾗｰﾒｯｾｰｼﾞ
	}
	ResetSelNo();														// 立体選択ﾘｾｯﾄ
	CountEdtObj();														// 編集対象立体ｶｳﾝﾄ
	SetDataUse();														// ﾃﾞｰﾀ使用率設定
	Display(0);															// 再描画
	delete[] name;
}

/*************************************/
void CJcad3GlbView::MenuConnect(void)									//<<<  曲面生成(連結)
/*************************************/
{
	OBJTYPE** sop = new OBJTYPE*[1000];
	OBJTYPE*  op;
	OIFTYPE*  oif;
	CMDTYPE*  cmp;
	int       i, j, i1, i2, i3, i4, clf, cn, ct1=0, ct2=0, err=0;

	clf = (m_FChar&0x1) ? 1 : 0;										// [F5]:閉連結ﾌﾗｸﾞ
	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		i3 = GetObjAtr(op);												// 立体属性取得
		if(i3/10==1&&cmp!=NULL) {										// <曲線の場合>
			sop[ct1] = op;												// 立体ﾎﾟｲﾝﾀ保存
			Sia[ct1] = GetParaInt(cmp, 0);								// 制御点数取得
			Siw[ct1] = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
			if(Siw[ct1]==1) {											// 境界ﾓｰﾄﾞ:閉はｴﾗｰ
				err = 1; break;
			}
			if(++ct1>1000) {											// 曲線ｶｳﾝﾄ
				err = 1; break;
			}
		} else if((i3==20||i3==21)&&cmp!=NULL) {						// <通常曲面の場合>
			sop[ct2] = op;												// 立体ﾎﾟｲﾝﾀ保存
			Sia[ct2] = GetParaInt(cmp, 0);								// U制御点数取得
			Sib[ct2] = GetParaInt(cmp, 1);								// V制御点数取得
			Siw[ct2] = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
			Six[ct2] = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
			Sic[ct2] = i3;												// 立体属性保存
			if(Siw[ct2]==1&&Six[ct2]==1) {								// 境界ﾓｰﾄﾞ:閉はｴﾗｰ
				err = 1; break;
			}
			if(++ct2>1000) {											// 曲面ｶｳﾝﾄ
				err = 1; break;
			}
		} else {														// 曲線･曲面以外はｴﾗｰ
			err = 1; break;
		}
		if(ct1>0&&ct2>0) {												// 曲線･曲面混在はｴﾗｰ
			err = 1; break;
		}
		while(cmp!=NULL) {
			i = GetCmdNo(cmp);											// ｺﾏﾝﾄﾞNo取得
			if(i!=CRTCURVE&&i!=CRTSURF&&i!=CRTSWEEP&&
               i!=CHGCURVE&&i!=CHGSURF&&i!=CHGSWEEP) {					// 曲線･曲面生成以外ｴﾗｰ
				err = 2; break;
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		if(err) {
			break;
		}
		NextObjPtr(2, &op, &i1, &i2);									// 次編集立体へ
	}
	err = (!err&&ct1+ct2<1) ? 1 : err;									// ﾃﾞｰﾀ無はｴﾗｰ
	if(!err) {
		if(ct2>0) {														// <正常曲面な場合>
			err = CheckSurface(ct2, Sia, Sib, Siw, Six, Sic, &i2);		// 曲面連結ﾁｪｯｸ
			i1 = 1;
		} else {
			i1 = 0;
		}
	}
	if(!err) {															// <正常な場合>
		cn = (ct1>ct2) ? ct1 : ct2;										// 連結立体数
		GetSEPoint(i1, i2, cn, sop, Sia, Sib, Siw, Six,
                   Spa, Spb, Ssa, &i3, &i4);							// 始点終点制御点取得
		if(cn>1) {
			err = (ConctSEPoint(cn, Spa, Spb, Ssb)) ? 4 : err;			// 始点終点制御点連結
		}
	}
	if(!err&&cn>1) {													// <正常な複数立体の場合>
		for(i=0, j=0; i<cn; i++) {
			if(Ssb[i].p[0]<0) {
				j++;													// 始点側未連結数ｶｳﾝﾄ
			} else if(Ssa[i].p[0]) {
				i3--;													// 始点側収束点分減算
			}
			if(Ssb[i].p[1]<0) {
				j++;													// 終点側未連結数ｶｳﾝﾄ
			} else if(Ssa[i].p[1]) {
				i3--;													// 終点側収束点分減算
			}
		}
		err = (i3>m_MaxCCtl||i3*i4>m_MaxSCtl) ? 3 : err;				// 制御点数上限ｵｰﾊﾞｰ
		err = (j!=2) ? 4 : err;											// 未連結個所が2以外はｴﾗｰ
	}
	if(!err) {															// <正常な場合>
		if(cn>1) {
			err = (GetConctObj(cn, Ssb, Ssc)) ? 4 : err;				// 連結立体順取得
		} else {
			Ssc[0].p[0] = 0, Ssc[0].p[1] = 0;
		}
	}
	if(!err) {															// <<正常な場合>>
		clf = (cn==1) ? 1 : clf;										// 1立体は必ず,境界:閉
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		if(i1==0) {
			ConctCurve(clf, cn, sop, Ssc);								// 曲線連結
		} else {
			ConctSurface(clf, i2, cn, sop, Ssc, Ssa);					// 通常曲面連結
		}
		GetObjPtr(&op);													// 生成立体取得
		GetObjOif(op, &oif);											// 立体構成情報ﾎﾟｲﾝﾀ取得
		if(i1==0) {
			if(clf) {
				SetOifName(0, oif, OBJ_NAME01);							// 体構成名(曲線[閉])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//         (曲線[開])
			}
		} else {
			SetOifName(0, oif, OBJ_NAME03);								//         (通常掃引体)
		}
		Delete();														// 選択立体削除
		OnEdtFlg(1, op);												// edtflg:ON
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		Display(0);														// 再描画
	} else if(err==2) {													// 立体編集ｺﾏﾝﾄﾞ有の場合
		AfxMessageBox(IDS_ERR_EDIT);
	} else if(err==3) {													// 制御点数が多すぎる場合
		AfxMessageBox(IDS_MANY_CTRLP);
	} else if(err==4) {													// 連結不可能な場合
		AfxMessageBox(IDS_ERR_CONECT);
	} else {															// 曲線･曲面以外か属性異常
		AfxMessageBox(IDS_ERR_SEL5);
	}
	delete[] sop;
}

/***************************************/
void CJcad3GlbView::MenuThickness(void)									//<<< 曲面生成(厚みづけ)
/***************************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	int      i1, i2, i3=0;
	double   d1=0.0;

	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	if(op!=NULL) {
		i3 = GetObjAtr(op);												// 立体属性取得
	}
	if(i3==21||i3==23) {												// <曲面(面体)の場合>
		OffEdtOtherObject(op);											// 指定外編集対象解除
		Display(0);														// 再描画
		CInputDlg dlg(5);
		dlg.SetValue(m_ThickValue);										// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetValue(&m_ThickValue);								// 厚み幅入力
			d1 = m_ThickValue;
		}
		if(fabs(d1)>EPSILON) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			InitLevel();												// Undo/Redo初期化
			Thickness(op, d1);											// 曲面厚みづけ
			i2 = GetObjAtr(op);											// 立体属性取得
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(i2==20) {
				SetOifName(1, oif, OBJ_NAME03);							// 体構成名(通常掃引体)
			} else {
				SetOifName(1, oif, OBJ_NAME05);							//         (掃引掃引体)
			}
			Display(0);													// 再描画
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		}
	} else {
		AfxMessageBox(IDS_ERR_SEL6);									// ｴﾗｰﾒｯｾｰｼﾞ
	}
}
