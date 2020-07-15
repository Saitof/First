/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewCurve.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "CreatCurvePoint.h"
#include "EditSurface.h"
#include "EditCtrlTracker.h"
#include "Symmetry.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*************************************************************/
void CJcad3GlbView::MouseCurve(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< 曲線生成
/*************************************************************/
{
	OIFTYPE* oif;
	PNTTYPE  pnt;
	int      i, j, f1, f2, endf=0;
	double   th, dd, d[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2&&HitTestPlane(point)==Si2&&Si1>0) {				// <平面ﾁｪｯｸOK>
			CalcScreenWorld(0, &pnt, point, Si2);						// 座標点取得
			if(CheckNextPoint(Spnt1, pnt)) {							// 直前点との距離OK
				if(m_CurveAFlg&&Si1>1) {								// <角度指定有効>
					th = Sd5-(PI*m_CurveAngle/180.0);					// 補正後角度
					LimitAngle(Si2, &pnt, &Spnt1, th);					// ﾏｳｽ角度指定制限
				} else {												// <角度指定無効>
					if(flg&m_LimKey) {
						LimitPoint(Si2, &pnt, &Spnt1);					// ﾏｳｽ入力制限座標
					}
				}
				Sia[Si1] = 0;											// ﾓｰﾄﾞ設定(仮)
				Spb[Si1] = pnt;											// 制御点設定(仮)
				CalcCurvePoint(Si4, Si1+1, Spb, Spa, Sdb);				// 通過点=>制御点変換
				RedoCrtCurve(Si1+1, 0, Spa, Sia);						// 曲線生成(仮)
				RedoSetCurve(Si1+1, 0, Spb);							// 曲線制御点生成(仮)
				for(i=0; i<DIM; i++) {
					d[i] = pnt.p[i]-Spnt1.p[i];							// 直前点との差
				}
				dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);				// 直前点との距離
				m_pNumDisp->SetParam(d[0], d[1], d[2], dd);				// ﾏｳｽ移動量表示
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// ﾄﾗｯｶｰ1設定
				Display(0);												// 再描画
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		CheckCtrlKey2(flg);												// 制御ｷｰ確認(制御点選択)
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			Sphase = 1;
		} else if(Sphase==1&&(Si2==-1||Si2==plane)) {					// <<初回の1点目>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			i = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                  Si12, Si15, Si16);					// 対称領域ﾁｪｯｸ
			if(Si5!=3||i==1) {											// 回転対称以外かﾁｪｯｸOK
				InitLevel();											// Undo/Redo初期化
				Si2 = (Si2==-1) ? plane : Si2;							// 指定平面
				Sia[Si1] = (m_CurInp) ? 1 : 0;							// Shift折線点ﾓｰﾄﾞ
				Spa[Si1] = pnt;											// 制御点設定(本)
				Spb[Si1++] = pnt;
				Spnt1 = pnt;											// 座標点保存
				CrtCurve(Si1, 0, m_CurveRepeat, m_CurveWeight,
                         Spa, Sia);										// 曲線生成(本)
				GetObjPtr(&Sop1);										// 曲線立体OP保存
				SetCurve(Si1, 0, Spb);									// 曲線制御点生成(本)
				GetObjPtr(&Sop2);										// 制御点OP保存
				SetSelNo(Sop2, 2);										// 黄色表示OP設定
				for(i=0; i<Si1; i++) {
					Sib[i] = Sia[i]*2;
				}
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// ﾄﾗｯｶｰ1設定
				Sphase = 2;
				Sd3 = (m_CurveRFlg) ? m_CurveRadius : 0.0;				// 終点R指定保存
				ClearCurveData();										// 曲面制御ﾃﾞｰﾀｸﾘｱ
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				if(m_NumInp!=0) {										// <数値入力の場合>
					Display(0);											// 再描画
					CInputBtnDlg5 dlg(1);
					dlg.SetParam1(Si4, Si5, Si13, Sbase, Si12, Si15,
                                  Si16, Si3, m_CurveWeight, &Si1, Spa,
                                  Spb, Sia, Sib, Sda, Sdb, m_CurveAdd);	// ﾊﾟﾗﾒｰﾀ設定1
					dlg.DoModal();										// 入力ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetParam(&m_CurveAdd);							// ﾊﾟﾗﾒｰﾀ取得
					endf = 2;											// ﾒﾆｭｰ終了
				}
			}
		} else if(Sphase==2&&Si2==plane) {								// <<2点目以降で同一平面>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(Si1==0) {												// <1点目まで取り消された場合>
				i = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                      Si12, Si15, Si16);				// 対称領域ﾁｪｯｸ
				if(i==1) {												// 回転対称以外かﾁｪｯｸOK
					Sia[Si1] = (m_CurInp) ? 1 : 0;						// Shift折線点ﾓｰﾄﾞ
					Spa[Si1] = pnt;										// 制御点設定(本)
					Spb[Si1++] = pnt;
					Spnt1 = pnt;										// 座標点保存
					CrtCurve(Si1, 0, m_CurveRepeat, m_CurveWeight,
                             Spa, Sia);									// 曲線生成(本)
					GetObjPtr(&Sop1);									// 曲線立体OP保存
					SetCurve(Si1, 0, Spb);								// 曲線制御点生成(本)
					GetObjPtr(&Sop2);									// 制御点OP保存
					SetSelNo(Sop2, 2);									// 黄色表示OP設定
					for(i=0; i<Si1; i++) {
						Sib[i] = Sia[i]*2;
					}
					Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);		// ﾄﾗｯｶｰ1設定
					Sd3 = (m_CurveRFlg) ? m_CurveRadius : 0.0;			// 終点R指定保存
					ClearCurveData();									// 曲面制御ﾃﾞｰﾀｸﾘｱ
					SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                    Sia, Sda, Sdb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				}
			} else {													// <2点目以降>
				i = CheckNextPoint(Spnt1, pnt);							// 直前点との距離ﾁｪｯｸ
				j = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                      Si12, Si15, Si16);				// 対称領域ﾁｪｯｸ
				if(i&&j) {												// ﾁｪｯｸOK
					if(m_CurveAFlg&&Si1>1) {							// <角度指定有効>
						th = Sd5-(PI*m_CurveAngle/180.0);				// 補正後角度
						LimitAngle(Si2, &pnt, &Spnt1, th);				// ﾏｳｽ角度指定制限
					} else if(Si1>0) {									// <角度指定無効>
						if(flg&m_LimKey) {								// ﾏｳｽ入力制限座標
							LimitPoint(Si2, &pnt, &Spnt1);
						}
					}
					if((Si1+1)*Si12<=m_MaxCCtl) {						// <制御点上限未満>
						Sia[Si1] = (m_CurInp) ? 1 : 0;					// Shift折線点ﾓｰﾄﾞ
						Spb[Si1++] = pnt;								// 制御点設定(本)
						Spnt1 = pnt;									// 座標点保存
						dd = (m_CurveRFlg) ? m_CurveRadius : 0.0;
						if(dd>1.0&&Si1>2) {								// <R指定有効>
							CalcRPoint(0, Si2, Si12, dd, &Si1, Spb,Sia);// R頂点位置算出
						}
					} else {
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1,
                                        Sia, Sda, Sdb);					// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					}
					CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);			// 通過点=>制御点変換
					RedoCrtCurve(Si1, 0, Spa, Sia);						// 曲線生成(本)
					RedoSetCurve(Si1, 0, Spb);							// 曲線制御点生成(本)
					for(i=0; i<Si1; i++) {
						Sib[i] = Sia[i]*2;
					}
					Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);		// ﾄﾗｯｶｰ1設定
					if(Si1>1) {
						Sd5=CalcBaseAngle(Spb[Si1-2], Spb[Si1-1], Si2);	// 基準角度取得
					}
				}
				if(Si1==2) {
					Sd4 = (m_CurveRFlg) ? m_CurveRadius : 0.0;			// 始点R指定保存
				}
				ClearCurveData();										// 曲面制御ﾃﾞｰﾀｸﾘｱ
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			}
		}
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(Si5==4) {												// <入力ﾓｰﾄﾞ:螺旋>
				if(Si8*Si9+1<=m_MaxCCtl) {								// 総制御点数
					InitLevel();										// Undo/Redo初期化
					Si1 = Si8*Si9+1;
					CalcSpiralPoint(pnt, plane, Sd1, Sd2, Si8,
                                    Si9, Spb, Sia);						// 螺旋制御点計算
					SetCurveMatrix1(Si4, m_CurveWeight, Si3, Si1,
                                    Sia, Sda, Sdb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);			// 通過点=>制御点変換
					CrtCurve(Si1, Si3, m_CurveRepeat, m_CurveWeight,
                             Spa, Sia);									// 曲線生成(本)
					GetObjPtr(&Sop1);									// 曲線立体OP保存
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// ｴﾗｰﾒｯｾｰｼﾞ
				}
				endf = 2;												// ﾒﾆｭｰ終了
			} else {													// <入力ﾓｰﾄﾞ:螺旋以外>
				Si2 = plane;											// 指定平面
				ScrToPnt(0, &m_Cent3D, &Sbase);							// 3面図中心点
				Si14 = (flg&MK_CONTROL) ? 1 : 0;						// 横方向指定
				SetSymmetryLine(0, Si5, Sbase, Si2, Si14, Si12,
                                &Si15, &Si16);							// 対称軸設定
			}
		}
	} else if(ms==11) {													// <<<DEL(取消)>>>
		if(Sphase==2&&Si1>0) {											// <制御点を登録済み>
			Si1--;														// 制御点数減算
			if(Si1>0) {
				Spnt1 = Spb[Si1-1];										// 座標点保存
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1,
                                Sia, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
				CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);				// 通過点=>制御点変換
				RedoCrtCurve(Si1, 0, Spa, Sia);							// 曲線生成(本)
				RedoSetCurve(Si1, 0, Spb);								// 曲線制御点生成(本)
				for(i=0; i<Si1; i++) {
					Sib[i] = Sia[i]*2;
				}
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// ﾄﾗｯｶｰ1設定
				Sd5 = CalcBaseAngle(Spb[Si1-2], Spb[Si1-1], Si2);		// 基準角度取得
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			} else {
				Spnt1.p[0]=DBL_MAX, Spnt1.p[1]=DBL_MAX, Spnt1.p[2]=DBL_MAX;
				Si13 = 0;												// 対称領域未決定
				if(Sop1!=NULL) {
					FreeObj(Sop1, 1);									// 立体削除
				}
				if(Sop2!=NULL) {
					FreeObj(Sop2, 1);									// 立体削除
				}
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
			}
			Display(0);													// 再描画
		}
		ClearCurveData();												// 曲面制御ﾃﾞｰﾀｸﾘｱ
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			i = (m_FChar&0x1) ? 1 : 0;									// [F5]:入力制限有
			Si4 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			Si5 = 0;													// 入力ﾓｰﾄﾞ:通常
			Si5 = (m_FChar&0x2) ? 3 : Si5;								// [F6]:入力ﾓｰﾄﾞ:回転対称
			Si5 = (m_FChar&0x8) ? 2 : Si5;								// [F8]:入力ﾓｰﾄﾞ:2軸対称
			Si5 = (m_FChar&0x4) ? 1 : Si5;								// [F7]:入力ﾓｰﾄﾞ:1軸対称
			Si5 = (m_FChar&0x20) ? 4 : Si5;								// [F10]:入力ﾓｰﾄﾞ:螺旋
			if(Si5==4) {												// <入力ﾓｰﾄﾞ:螺旋>
				i = 0, Si4 = 1, Si3 = 0, m_NumInp = 0;					// 制限無/通過点/境界開/数値無
				CSpiralDlg dlg;
				dlg.SetValue(m_SPDiameter, m_SPHeight,
                             m_SPCtrlNum, m_SPSprlNum);					// 既存値表示
				if(dlg.DoModal()==IDOK) {								// 螺旋情報設定ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_SPDiameter, &m_SPHeight,
                                 &m_SPCtrlNum, &m_SPSprlNum);			// 螺旋情報取得
					Sd1 = m_SPDiameter, Sd2 = m_SPHeight;
					Si8 = m_SPCtrlNum,  Si9 = m_SPSprlNum;
				} else {
					ExitMenu(0, 1);                                     // ﾒﾆｭｰ終了
				}
			} else {													// <入力ﾓｰﾄﾞ:螺旋以外>
				if(m_Char=='O') {										// Oｷｰ押下:開
					Si3 = 0;
				} else if(m_Char=='C') {								// Cｷｰ押下:閉
					Si3 = 1;
				} else {
					CCurveDlg dlg;
					if(dlg.DoModal()==IDOK) {							// 開/閉設定ﾀﾞｲｱﾛｸﾞ表示
						dlg.GetValue(&Si3);
						Si3 = (Si3==0) ? 1 : 0;
					} else {
						ExitMenu(0, 1);									// ﾒﾆｭｰ終了
					}
				}
			}
			if(Si5==3) {												// <入力ﾓｰﾄﾞ:回転対称>
				Si12 = (InputRevolveDivi(&m_SymRDivi)) ? m_SymRDivi : 0;// 回転対称分割数取得
				if(Si12<0) {
					ExitMenu(0, 1);										// ﾒﾆｭｰ終了
				}
			} else if(Si5==1) {
				Si12 = 2;												// ｴﾘｱ分割数
			} else if(Si5==2) {
				Si12 = 4;
			} else { Si12 = 1;
			}
			if(m_ActMenu!=NONE) {										// <中止でない場合>
				i = (m_NumInp!=0) ? 0 : i;								// 数値入力:有は制限無
				Sop1 = NULL, Sop2 = NULL;
				Si1 = 0, Si2 = -1, Si13 = 0;
				Sd3 = 0.0, Sd4 = 0.0;
				OffEdtAllObject();										// 全立体edtflg:OFF
				OffUndoRedo();											// Undo/Redo不可
				Display(0);												// 再描画
				Sphase = (Si5==0) ? 1 : Sphase;							// 通常ﾓｰﾄﾞは制御点指定へ
				m_CursorNo = 1;											// ｶｰｿﾙNo設定
				if(i) {
					ShowCurveCtrl();									// 曲線制御ﾀﾞｲｱﾛｸﾞ表示
				} else {
					ClearCurveData();									// 曲面制御ﾃﾞｰﾀｸﾘｱ
				}
				StartMenu();											// ﾒﾆｭｰ開始処理
				SetNumDisp(2);											// 数値表示初期処理
			}
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		endf = (ms==9) ? 1 : 2;											// ﾒﾆｭｰ終了
	}
	if(endf>0) {														// <ﾒﾆｭｰ終了の場合>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		if(Si1==2&&Si3==0&&Si5==0) {									// 2点&開&通常ﾓｰﾄﾞの場合
			InsertCurvePoint(&m_InstPNum, &Si1, Spb, Sia);				// S曲線中間制御点挿入
		}
		if(Si1>0) {														// 1点以上指定の場合
			i = Si1;													// 複写前制御点数保存
			j = CopySymmetryArea(Si5, Sbase, Si12, Si15, Si16, &Si1,
                                 &Si3, Spb, Sia, &f1, &f2);				// 対称領域複写
			if(j>0) {													// <2軸対称で1軸のみ有効>
				Si5 = 1;												// 入力ﾓｰﾄﾞを1軸対称へ
				Si14 = (Si2==1) ? ((j==1)?0:1) : ((j==1)?1:0);			// 横方向指定ﾌﾗｸﾞ設定
			}
		}
		if(Si1>2) {														// <<<正常な曲線>>>
			if(Si3) {													// <<曲線閉の場合>>
				th = (m_CurveRFlg) ? m_CurveRadius : 0.0;
				dd = (th>1.0) ? th : Sd3;
				if(dd>1.0) {
					CalcRPoint(2, Si2, Si12, dd, &Si1, Spb, Sia);		// 終点R頂点位置算出
				}
				if(Sd4>1.0) {
					CalcRPoint(1, Si2, Si12, Sd4, &Si1, Spb, Sia);		// 始点R頂点位置算出
				}
			}
			SetCurveMatrix1(Si4, m_CurveWeight, Si3, Si1,
                            Sia, Sda, Sdb);								// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
			CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);					// 通過点=>制御点変換
			RedoCrtCurve(Si1, Si3, Spa, Sia);							// 曲線生成(本)
			GetObjOif(Sop1, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(Si3) {
				SetOifName(0, oif, OBJ_NAME01);							// 立体構成名称(曲線[閉])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//             (曲線[開])
			}
			OnEdtFlg(1, Sop1);											// 曲線edtflg:ON
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		} else if(Sphase==2) {											// <異常な曲線>
			AfxMessageBox(IDS_FEW_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
			FreeObj(Sop1, 1);											// 曲線削除
			BackLevel();												// ﾚﾍﾞﾙ戻
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Line.Reset();													// ﾗｲﾝ解除
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		ClearCurveData();												// 曲面制御ﾃﾞｰﾀｸﾘｱ
		HideCurveCtrl();												// 曲線制御ﾀﾞｲｱﾛｸﾞ消去
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
		if(endf==2&&Si1>2) {											// <自ﾒﾆｭｰ内で終了>
			Wi0 = m_NumInp;												// 数値入力ﾓｰﾄﾞ継続
			Wi4 = 0;													// 対称以外は0
			m_FChar = (Si4) ? m_FChar|0x10 : m_FChar;					// [F9]:通過点ﾓｰﾄﾞ継続
			if(Si5>0&&Si5<4) {											// 1軸･2軸･回転の場合
				Wi4 = i;												// 複写前制御点数
				if(Si5==1) {											// [F7]:1軸対称継続
					m_FChar = m_FChar|0x4;
				} else if(Si5==2) {										// [F8]:2軸対称継続
					m_FChar = m_FChar|0x8;
				} else if(Si5==3) {										// [F6]:回転対称継続
					m_FChar = m_FChar|0x2;
				}
				Wi1 = Si12, Wi2 = Si2, Wi3 = Si14, Wi5 = f1, Wi6 = f2;	// 対称軸情報継続
				SpntA = Sbase;
			} else {
				Wi1 = 0, Wi2 = 0, Wi3 = 0, Wi5 = 0, Wi6 = 0;
			}
			SetActMenu(M_EDIT_CURVE);									// 自動的に｢曲線編集｣起動
			InitActMenu(0);
		}
	}
}

/**************************************************************/
void CJcad3GlbView::MouseECurve(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< 曲線編集
/**************************************************************/
{
	OIFTYPE* oif;
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
		if(flg&MK_LBUTTON&&(Sphase==1||Sphase==3)&&plane!=3) {			// ﾄﾞﾗｯｸﾞ
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			GetMoveValue(plane, Sbase, &pnt);							// ﾄﾗｯｶｰ移動量取得
			MoveSymmetryArea(Sbase, pnt, Si6, Spnt1, Si7, Si11, Si12,
                             Si14, Si15, Si9, Si13, Si1, Sic, Spy, Spw);// 対称領域移動量設定
			MoveECurve(Si5, Sdb, Spw, Sop1, Si1, Si2, Spa, Spb,
                       Sic, Spc, Tens);									// 制御点/通過点移動
			RedoChgCurve(Si1, Si2, Spc, Sia);							// 曲線生成
			TrackerECurve(Si5, Sda, Si2, Si1, Spc, Sia, Sic,
                          &Si3, Spd, Sib);								// 曲線編集ﾄﾗｯｶｰ取得
			RedoSetCurve(Si3, Si2, Spd);								// 曲線制御点生成
			for(i=0; i<Si3; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// 立体編集後制御点取得
			}
			i = (Si13>0) ? Si13*2-1 : Si3;								// 継続は表示数変更
			DispParsTracker(0, Si21, Spd, Sib, i, Spx, Six, Si10);		// 透視図ﾄﾗｯｶｰ表示
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// 移動前点との距離
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Si4 = 1;
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				i = (Si13>0) ? Si13*2-1 : Si3;							// 継続は表示数変更
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
			}
		} else if(Sphase==1||Sphase==3) {								// <ﾄﾗｯｶｰ選択の場合>
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				box.bp[0] = BaseCursor;
				box.bp[1] = pnt;
				i = (Si13>0) ? Si13*2-1 : Si3;							// 継続は表示数変更
				DragCtrlTracker1(plane, box, Spb, i, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(Si13>0) {											// 継続の場合は
					PickSymmetryArea(Si6, Si7, Si13*2-1, Si3,
                                     Si14, Si15, &i1, Siy);				// 対称領域ﾄﾗｯｶｰ選択
				}
				if(i1>0) {												// <制御点選択の場合>
					for(i=0, i2=0; i<i1; i++) {
						if(Siy[i]%2!=0) {
							i2++;										// 制御点挿入数ｶｳﾝﾄ
						} else {
							i2 = 0; break;								// 既存の選択は挿入NG
						}
					}
					if(Si1+i2>m_MaxCCtl) {								// <制御点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						Sphase = 1;
					} else {											// <制御点上限未満>
						if(Si13>0) {									// 生成継続ﾓｰﾄﾞの場合
							for(i=0, i2=0; i<i1; i++) {
								if(Siy[i]>=Si13*2-1) {
									break;								// 生成領域外は終了
								}
								if(Siy[i]%2!=0) {
									i2++;								// 生成領域挿入対象ｶｳﾝﾄ
								} else {
									i2 = 0; break;						// 既存の選択は挿入NG
								}
							}
							Si13 += i2;									// 生成領域制御点数加算
						}
						PickECurve(i1, Siy, Si5, Sda, Sdb, &Si1, Si2,
                                   m_ObjCRpt, m_ObjCWgt, &Si3, Sop2,
                                   m_CurInp, Spa, Spb, Sia, Sib, Sic);	// 曲線編集 点選択
						i = (Si13>0) ? Si13*2-1 : Si3;					// 継続は表示数変更
						SetParsTracker(0, &Si21, 1, Sic, Spb, Sib, i,
                                       Spx, Six, &Si10);				// 透視図ﾄﾗｯｶｰ設定
						Sphase = 3;
						Si23 = 1;
					}
				}
			}
		} else if(Sphase==2) {											// <制御点移動の場合>
			if(Si4==1) {												// 移動済みの場合は
				for(i=0; i<Si1; i++) {									// 正式にﾊﾞｯﾌｧ変更
					Spa[i] = Spc[i], Spy[i] = Spd[i*2];
				}
				for(i=0; i<Si3; i++) {
					Spb[i] = Spd[i];
				}
			}
			Sphase = 3;
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <入力ﾓｰﾄﾞ:通常以外>
			if(plane<3) {												// 3面図の場合
				CalcScreenWorld(0, &Spnt1, point, plane);				// 対称軸中心点取得
				Si8 = plane;											// 対称軸指定平面
				i = (flg&MK_CONTROL) ? 1 : 0;							// 横方向指定
				Si9 = (flg&MK_SHIFT) ? 1 : 0;							// 軸方向反転
				SetSymmetryLine(1, Si6, Spnt1, Si8, i, Si7,
                                &Si11, &Si12);							// 対称軸設定
			}
		} else {														// <制御点指定の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			i = (Si13>0) ? Si13*2-1 : Si3;								// 継続は表示数変更
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, i,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Siy[0]%2==0&&
               Sic[Siy[0]/2]==1&&plane!=3) {							// <制御点移動の場合>
				Sbase = Spb[Siy[0]];									// 移動元点保存
				Sphase = 2;
				Si4 = 0;
				if(m_NumInp!=0) {										// <数値入力の場合>
					Sphase = 3;
					CInputBtnDlg6 dlg(1);
					dlg.SetParam1(Sop1, Sop2, Si5, Si21, Si1, Si2, Si10,
                                  &Si4, Si13, Si6, Si7, Si11, Si12, Si14,
                                  Si15, Si9, Sbase, Spnt1, Spa, Spb, Spc,
                                  Spd, Spw, Spx, Spy, Sia, Sib, Sic, Six,
                                  Tens, Sda, Sdb, m_ECurveAdd);         // ﾊﾟﾗﾒｰﾀ設定1
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						if(Si4==1) {									// 移動済みの場合は
							for(i=0; i<Si1; i++) {						// 正式にﾊﾞｯﾌｧ変更
								Spa[i] = Spc[i], Spy[i] = Spd[i*2];
							}
							for(i=0; i<Si3; i++) {
								Spb[i] = Spd[i];
							}
						}
					}
					dlg.GetParam(&m_ECurveAdd);							// ﾊﾟﾗﾒｰﾀ取得
					RedoChgCurve(Si1, Si2, Spa, Sia);					// 曲線生成
					TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia,
                                  Sic, &Si3, Spb, Sib);					// 曲線編集ﾄﾗｯｶｰ取得
					RedoSetCurve(Si3, Si2, Spb);						// 曲線制御点生成
					for(i=0; i<Si3; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					i = (Si13>0) ? Si13*2-1 : Si3;						// 継続は表示数変更
					DispParsTracker(0, Si21, Spb, Sib, i,
                                    Spx, Six, Si10);					// 透視図ﾄﾗｯｶｰ表示
				}
			} else if(i1>0) {											// <制御点選択の場合>
				if(!m_CtlInp) {
					for(i=0; i<Si1; i++) {
						Sic[i] = 0;										// 全制御点選択解除
					}
				}
				if(Si13>0) {											// 継続の場合は
					PickSymmetryArea(Si6, Si7, Si13*2-1, Si3,
                                     Si14, Si15, &i1, Siy);				// 対称領域ﾄﾗｯｶｰ選択
				}
				for(i=0, i2=0; i<i1; i++) {
					if(Siy[i]%2!=0) {
						i2++;											// 制御点挿入数ｶｳﾝﾄ
					} else {
						i2 = 0; break;									// 既存の選択は挿入NG
					}
				}
				if(Si1+i2>m_MaxCCtl) {									// <制御点上限以上>
					AfxMessageBox(IDS_MANY_CTRLP);						// ｴﾗｰﾒｯｾｰｼﾞ
					Sphase = 1;
				} else {												// <制御点上限未満>
					if(Si13>0) {										// 生成継続ﾓｰﾄﾞの場合
						for(i=0, i2=0; i<i1; i++) {
							if(Siy[i]>=Si13*2-1) {
								break;									// 生成領域外は終了
							}
							if(Siy[i]%2!=0) {
								i2++;									// 生成領域挿入対象ｶｳﾝﾄ
							} else {
								i2 = 0; break;							// 既存の選択は挿入NG
							}
						}
						Si13 += i2;										// 生成領域制御点数加算
					}
					PickECurve(i1, Siy, Si5, Sda, Sdb, &Si1, Si2,
                               m_ObjCRpt, m_ObjCWgt, &Si3, Sop2,
                               m_CurInp, Spa, Spb, Sia, Sib, Sic);		// 曲線編集 点選択
					for(i=0; i<Si1; i++) {
						Spy[i] = Spb[i*2];								// 立体編集後制御点取得
					}
					i = (Si13>0) ? Si13*2-1 : Si3;						// 継続は表示数変更
					SetParsTracker(0, &Si21, 1, Sic, Spb, Sib, i,
                                   Spx, Six, &Si10);					// 透視図ﾄﾗｯｶｰ設定
					Sphase = 3, Si23 = 1;
				}
			} else if(!m_CtlInp) {										// <ﾄﾗｯｶｰ指定無で追加無>
				for(i=0; i<Si1; i++) {
					Sic[i] = 0;											// 全制御点選択解除
				}
				RedoChgCurve(Si1, Si2, Spa, Sia);						// 曲線生成
				TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia, Sic,
                              &Si3, Spb, Sib);							// 曲線編集ﾄﾗｯｶｰ取得
				RedoSetCurve(Si3, Si2, Spb);							// 曲線制御点生成
				for(i=0; i<Si3; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si1; i++) {
					Spy[i] = Spb[i*2];									// 立体編集後制御点取得
				}
				i = (Si13>0) ? Si13*2-1 : Si3;							// 継続は表示数変更
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Sphase = 1, Si21 = 0, Si10 = 0;							// 透視図選択解除
			}
		}
		Display(0);														// 再描画
	} else if(ms==11) {													// <<<DEL(削除)>>>
		if(Sphase==2||Sphase==3) {
			for(i=0, i1=0; i<Si1; i++) {
				i1 = (Sic[i]) ? i1 + 1 : i1;							// 削除対象ｶｳﾝﾄ
			}
			if(Si1-i1<3) {												// <制御点下限以下>
				AfxMessageBox(IDS_FEW_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else {													// <制御点下限より大>
				if(Si13>0) {											// 生成継続ﾓｰﾄﾞの場合
					for(i=0, i2=0; i<Si13; i++) {
						i2 = (Sic[i]) ? i2 + 1 : i2;					// 生成領域削除対象ｶｳﾝﾄ
					}
					Si13 -= i2;											// 生成領域制御点数減算
				}
				DeleteCurvePoint(Si5, Sda, Sdb, Si2, m_ObjCWgt, Sic,
                                 Si1, Spa, Sia, &i1);					// 制御点･通過点削除
				Si1 = i1;												// 制御点数再設定
				RedoChgCurve(Si1, Si2, Spa, Sia);						// 曲線生成
				TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia, Sic,
                              &Si3, Spb, Sib);							// 曲線編集ﾄﾗｯｶｰ取得
				RedoSetCurve(Si3, Si2, Spb);							// 曲線制御点生成
				for(i=0; i<Si3; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				for(i=0; i<Si1; i++) {
					Spy[i] = Spb[i*2];									// 立体編集後制御点取得
				}
				i = (Si13>0) ? Si13*2-1 : Si3;							// 継続は表示数変更
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 1;
			}
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			m_NumInp = (m_NumInp==0&&Wi0==1) ? 1 : m_NumInp;			// ｢曲面生成｣から継続
			Sop1 = NULL, Sop2 = NULL;
			Si10 = 0, Si13 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si5 = (m_FChar&0x10) ? 1 : 0;								// [F9]:通過点ﾓｰﾄﾞ
			Si6 = (m_FChar&0x2) ? 3 : 0;								// [F6]:入力ﾓｰﾄﾞ:回転対称
			Si6 = (m_FChar&0x8) ? 2 : Si6;								// [F8]:入力ﾓｰﾄﾞ:2軸対称
			Si6 = (m_FChar&0x4) ? 1 : Si6;								// [F7]:入力ﾓｰﾄﾞ:1軸対称
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if(i3/10==1&&cmp!=NULL) {									// <<曲線の場合>>
				if(Wi4>0) {
					Si7 = Wi1;											// 曲面生成の継続ﾃﾞｰﾀ
				} else {
					if(Si6==3) {										// 回転対称分割数取得
						Si7 = (InputRevolveDivi(&m_SymRDivi))?m_SymRDivi:0;
					} else if(Si6==1) {
						Si7 = 2;										// 1軸対称ｴﾘｱ分割数(2)
					} else if(Si6==2) {
						Si7 = 4;										// 2軸対称ｴﾘｱ分割数(4)
					} else {
						Si7 = 1;										// 通常ｴﾘｱ分割数(1)
					}
				}
				if(Si7>0) {												// <ｴﾘｱ分割数OK>
					InitTRLimit();										// ﾄﾗｯｶｰ範囲限定初期化
					OffEdtOtherObject(Sop1);							// 指定外編集対象解除
					Si1 = GetParaInt(cmp, 0);							// 制御点数取得
					Si2 = GetParaInt(cmp, 1);							// 境界ﾓｰﾄﾞ取得
					m_ObjCRpt = GetParaInt(cmp, 2);						// 曲線分割数取得
					m_ObjCWgt = (double)GetParaInt(cmp, 3)/1000.0;		// 曲線重み係数取得
					for(i=0; i<Si1; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// ﾗｲﾝﾓｰﾄﾞ取得
						GetParaPnt(cmp, i, &Spa[i]);					// 制御点取得
						Sic[i] = 0;										// 対象ﾌﾗｸﾞ:OFF
					}
					SetCurveMatrix3(Si5, m_ObjCWgt, Si2, Si1,
                                    Sia, Sda, Sdb);						// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
					StartMenu();										// ﾒﾆｭｰ開始処理
					InitLevel();										// Undo/Redo初期化
					ChgCurve(NULL, Si1, Si2, m_ObjCRpt, m_ObjCWgt,
                             Spa, Sia);									// 曲線変更(初回)
					TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia,
                                  Sic, &Si3, Spb, Sib);					// 曲線編集ﾄﾗｯｶｰ取得
					SetCurve(Si3, Si2, Spb);							// 曲線制御点生成(初回)
					GetObjPtr(&Sop2);									// 制御点OP保存
					SetSelNo(Sop2, 2);									// 黄色表示OP設定
					BaseCmdPtr(1, Sop1, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					NextCmdPtr(&cmp);									// 曲線編集後ｺﾏﾝﾄﾞ
					while(cmp!=NULL) {
						RedoCommandCopy(Sop2, cmp);						// ｺﾏﾝﾄﾞ複写実行
						NextCmdPtr(&cmp);								// 次ｺﾏﾝﾄﾞへ
					}
					SetDispList(Sop2);									// ﾃﾞｨｽﾌﾟﾚｲ・ﾘｽﾄ登録
					for(i=0; i<Si3; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// 立体編集後制御点取得
					}
					for(i=0; i<Si1; i++) {
						Spy[i] = Spb[i*2];								// 立体編集後制御点取得
					}
					OffUndoRedo();										// Undo/Redo不可
					SetNumDisp(2);										// 数値表示初期処理
					if(Si6!=0&&Wi4>0) {									// <通常ﾓｰﾄﾞ以外で生成継続>
						Si13 = Wi4;										// 複写前制御点数
						Si14 = Wi5;										// 始点接軸ﾌﾗｸﾞ
						Si15 = Wi6;										// 終点接軸ﾌﾗｸﾞ
						Si8 = Wi2;										// 対称軸指定平面
						Spnt1 = SpntA;									// 対称軸中心点
						i = Wi3;										// 横方向指定
						Si9 = 0;										// 軸方向反転なし
						SetSymmetryLine(1, Si6, Spnt1, Si8, i,
                                        Si7, &Si11, &Si12);				// 対称軸設定
						Sphase = 1;
					}
					Sphase = (Si6==0) ? 1 : Sphase;						// <通常ﾓｰﾄﾞ>
					if(Sphase==1) {
						i = (Si13>0) ? Si13*2-1 : Si3;					// 継続は表示数変更
						Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);		// ﾄﾗｯｶｰ1設定
					}
					Display(0);											// 再描画
				}
			} else {													// <編集立体:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
			Wi0 = 0, Wi1 = 0, Wi2 = 0, Wi3 = 0, Wi4 = 0, Wi5 = 0, Wi6 = 0;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// 立体削除
		}
		if(ms==4) {														// <ﾏｳｽ右釦押下>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			i = (Si13>0) ? Si13*2-1 : Si3;								// 継続は表示数変更
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, i,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(i1>0) {													// <制御点選択の場合>
				i1 = (Siy[0]%2!=0) ? (Siy[0]+1)/2 : 0;					// 追加位置選択のみ有効
			}
			if(i1>0) {													// <<有効な切断位置の場合>>
				if(AfxMessageBox(IDS_DIVIDE_CTRLP, MB_YESNO)!=IDYES) {	// 確認:NG
					i1 = 0;
				}
			}
			if(i1>0) {													// <<有効な切断位置の場合>>
				i2 = 0, i3 = 0;;
				if(Si2) {												// <<境界:閉の場合>>
					for(i=i1; i<Si1; i++) {
						Spb[i2] = Spa[i], Sib[i2++] = Sia[i];			// 切断位置以降を複写
					}
					for(i=0; i<i1; i++) {
						Spb[i2] = Spa[i], Sib[i2++] = Sia[i];			// 連続して切断位置より前を複写
					}
				} else {												// <<境界:開の場合>>
					if(i1<3&&Si1-i1<3) {								// <切断位置前後が制御点数不足>
						AfxMessageBox(IDS_FEW_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(i1>=3&&Si1-i1<3) {						// <切断位置前が有効>
						for(i=0; i<i1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// 切断位置より前を複写
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(i1<3&&Si1-i1>=3) {						// <切断位置後が有効>
						for(i=i1; i<Si1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// 切断位置以降を複写
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else {											// <切断位置前後が有効>
						for(i=0; i<i1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// 切断位置より前を複写
						}
						for(i=i1; i<Si1; i++) {
							Spc[i3] = Spa[i], Sic[i3++] = Sia[i];		// 切断位置以降を複写
						}
					}
				}
				if(i2>0) {												// <Redo:OK>
					RedoChgCurve(i2, 0, Spb, Sib);						// 曲線編集(開)
					GetObjOif(Sop1, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					SetOifName(1, oif, OBJ_NAME02);						// 立体構成名称(曲線[開])
					Si23 = 1;
				}
				if(i3>0) {												// <Create:OK>
					CrtCurve(i3, 0, m_ObjCRpt, m_ObjCWgt, Spc, Sic);	// 曲線生成(開)
					GetObjPtr(&Sop2);									// 制御点OP保存
					BaseCmdPtr(1, Sop1, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					NextCmdPtr(&cmp);									// 曲線編集後ｺﾏﾝﾄﾞ
					while(cmp!=NULL) {
						RedoCommandCopy(Sop2, cmp);						// ｺﾏﾝﾄﾞ複写実行
						NextCmdPtr(&cmp);								// 次ｺﾏﾝﾄﾞへ
					}
					SetDispList(Sop2);									// ﾃﾞｨｽﾌﾟﾚｲ・ﾘｽﾄ登録
					OnEdtFlg(1, Sop2);									// edtflg:ON
					GetObjOif(Sop2, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					SetOifName(0, oif, OBJ_NAME02);						// 立体構成名称(曲線[開])
					Si23 = 1;
				}
			}
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Line.Reset();													// ﾗｲﾝ解除
		ResetSelNo();													// 立体選択Noﾘｾｯﾄ
		OnUndoRedo();													// Undo/Redo可
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
void CJcad3GlbView::MouseExtrCurve(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< 曲線抽出
/*****************************************************************/
{
	OIFTYPE* oif;
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
				DragCtrlTracker1(plane, box, Spw, Si6, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// ﾄﾗｯｶｰﾄﾞﾗｯｸﾞ1
				if(i1>0) {												// <制御点選択の場合>
					for(i=0; i<i1; i++) {
						Siw[Siy[i]] = 1;								// 選択状態
					}
					SetLineNo2(Si2, Si6, Si7, Si8, Siw);				// ﾗｲﾝ番号設定2
					TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                                 &Si6, Spw, Sic);						// 点列ﾄﾗｯｶｰ取得
					for(i=0; i<Si6; i++) {
						Sic[i] = (Siw[i]) ? 4 : Sic[i];					// 指定点色変更
					}
					SetParsTracker(7, &Si21, 0, Siw, Spw, Sic,
                                   Si6, Spx, Six, &Si10);				// 透視図ﾄﾗｯｶｰ設定
					if(i1==1) {											// <<1制御点選択>>
						Display(0);										// 再描画
						if(AfxMessageBox(IDS_OUTPUT_CURVE,
                                         MB_YESNO)==IDYES) {			// <抽出OK?>
							if(ExtractionCurve(Si1, Si2, Si3, Si4, Si7,
                                               Si8, Siy[0], Spb, Sia,
                                               &Si11, &Si12, Sps, Sis)){// 曲線抽出
								CrtCurve(Si11, Si12, m_ObjCRpt,
                                         m_ObjCWgt, Sps, Sis);			// 曲線生成
								GetObjPtr(&Sop3);						// 曲線立体OP保存
								GetObjOif(Sop3, &oif);					// 立体構成情報ﾎﾟｲﾝﾀ取得
								if(Si12) {
									SetOifName(0, oif, OBJ_NAME01);		// 立体構成名称(曲線[閉])
								} else {
									SetOifName(0, oif, OBJ_NAME02);		//             (曲線[開])
								}
								Si23 = 1;
							}
						}
					}
				}
			}
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {													// <入力ﾓｰﾄﾞ:通常>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			BaseCursor = pnt;											// 押下位置保存
			CheckCtrlKey2(flg);											// 制御ｷｰ確認(制御点選択)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si6,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// 全ﾄﾗｯｶｰﾁｪｯｸ1
			if(Si22) {
				return;
			}
			for(i=0; i<Si6; i++) {
				Siw[i] = 0;												// 指定ﾌﾗｸﾞｸﾘｱ
			}
			if(i1>0) {													// <制御点選択の場合>
				for(i=0; i<i1; i++) {
					Siw[Siy[i]] = 1;									// 選択状態
				}
				SetLineNo2(Si2, Si6, Si7, Si8, Siw);					// ﾗｲﾝ番号設定2
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				for(i=0; i<Si6; i++) {
					Sic[i] = (Siw[i]) ? 4 : Sic[i];						// 指定点色変更
				}
				SetParsTracker(7, &Si21, 0, Siw, Spw, Sic, Si6,
                               Spx, Six, &Si10);						// 透視図ﾄﾗｯｶｰ設定
				Display(0);												// 再描画
				if(i1==1) {												// <<1制御点選択>>
					if(AfxMessageBox(IDS_OUTPUT_CURVE,
                                     MB_YESNO)==IDYES) {				// <抽出OK?>
						if(ExtractionCurve(Si1, Si2, Si3, Si4, Si7,
                                           Si8, Siy[0], Spb, Sia,
                                           &Si11, &Si12, Sps, Sis)) {	// 曲線抽出
							CrtCurve(Si11, Si12, m_ObjCRpt, m_ObjCWgt,
                                     Sps, Sis);							// 曲線生成
							GetObjPtr(&Sop3);							// 曲線立体OP保存
							GetObjOif(Sop3, &oif);						// 立体構成情報ﾎﾟｲﾝﾀ取得
							if(Si12) {
								SetOifName(0, oif, OBJ_NAME01);			// 立体構成名称(曲線[閉])
							} else {
								SetOifName(0, oif, OBJ_NAME02);			//             (曲線[開])
							}
							Si23 = 1;
						}
					}
				}
			} else {													// <ﾄﾗｯｶｰ指定無>
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ﾗｲﾝ番号初期化
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
			}
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Sop1 = NULL, Sop2 = NULL;
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// 生成時形状状態
			if(m_FChar&0x1) {											// [F5]:生成時形状反転
				Si20 = (Si20) ? 0 : 1;
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
				SetSurface(Si1, Si2, Si3, Si4, Spa);					// 曲面制御点生成(初回)
				GetObjPtr(&Sop2);										// 制御点OP保存
				SetSelNo(Sop2, 2);										// 黄色表示OP設定
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲面編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)!=THICKNESS) {						// <厚みづけ以外の場合>
						RedoCommandCopy(Sop2, cmp);						// ｺﾏﾝﾄﾞ複写実行
					}
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// 立体編集後制御点取得
				}
				SetDispList(Sop2);										// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// 制御点列確認
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// 点列ﾄﾗｯｶｰ取得
				for(i=0; i<Si6; i++) {
					Siw[i] = 0;											// 指定ﾌﾗｸﾞｸﾘｱ
				}
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
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
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

/***************************************/
void CJcad3GlbView::MenuCentCurve(void)									//<<< 立体中心点曲線生成
/***************************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i1, i2, i3=0, i4=0, i5=0, i6=0, i7=0;
	CString  str;

	if(GetSelectNo()==0) {												// <<2D曲線OP無>>
		Si1 = (m_FChar&0x1) ? 1 : 0;									// [F5]:中心点Z最小
		Si2 = (m_FChar&0x2) ? 1 : 0;									// [F6]:中心点Z最大
		Si3 = (m_FChar&0x4) ? 1 : 0;									// [F7]:生成終了
		Si4 = (m_FChar&0x8) ? 1 : 0;									// [F8]:閉曲線
		Si5 = (m_FChar&0x10) ? 0 : 1;									// [F9]:制御点
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			i3 = (GetObjAtr(op)/10==1) ? 1 : i3;						// 曲線はｴﾗｰ
			if(i3==1) {
				break;													// ｴﾗｰは中止
			}
			if(GetObjPrt(op)!=2) {										// <ﾕﾆｯﾄ:子以外の場合>
				Sop1 = op;												// 立体OP保存
				i4++;													// 立体数ｶｳﾝﾄUP
			}
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		if(i3==0&&i4==1) {												// <<有効な場合>>
			if(Si3==1) {												// <生成終了>
				AfxMessageBox(IDS_FEW_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else {													// <生成継続>
				GetObjBox(Sop1, &box);									// 立体BOX取得
				BoxCenter(box, &pnt);									// BOXｾﾝﾀｰ取得
				if(Si1==1) {
					pnt.p[2] = box.bp[0].p[2];							// 中心点Z最小指定
				} else if(Si2==1) {
					pnt.p[2] = box.bp[1].p[2];							// 中心点Z最大指定
				}
				Spa[0] = pnt;											// 中心点保存
				Sia[0] = 0;												// 曲線点
				Si6 = 1;												// 保存数
				SetSelNo(Sop1, 1);										// 立体登録
				OffEdtAllObject();										// 編集立体全解除
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
				Si7 = 1;												// 中心点登録中
			}
		} else {														// <ｴﾗｰの場合>
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
	} else if(Si7==1) {													// <<2D曲線OP有で登録中>>
		Si1 = (m_FChar&0x1) ? 1 : 0;									// [F5]:中心点Z最小
		Si2 = (m_FChar&0x2) ? 1 : 0;									// [F6]:中心点Z最大
		Si3 = (m_FChar&0x4) ? 1 : 0;									// [F7]:生成終了
		Si4 = (m_FChar&0x8) ? 1 : Si4;									// [F8]:閉曲線
		Si5 = (m_FChar&0x10) ? 0 : Si5;									// [F9]:制御点
		BaseObjPtr(1, &op, &i1, &i2);									// 表示対象立体取得
		while(op!=NULL) {
			if(GetSelNo(op)==1) {										// <<登録済み立体>>
				i3 = (GetObjAtr(op)/10==1) ? 1 : i3;					// 曲線はｴﾗｰ
				i4++;													// 登録済立体数ｶｳﾝﾄUP
			} else if(i2==1) {											// <<編集対象立体>>
				i7++;
				i5 = (GetObjAtr(op)/10==1) ? 1 : i5;					// 曲線はｴﾗｰ
				if(i5==1) {
					break;												// ｴﾗｰは中止
				}
				if(GetObjPrt(op)!=2) {									// <ﾕﾆｯﾄ:子以外の場合>
					Sop1 = op;											// 立体OP保存
					i6++;												// 立体数ｶｳﾝﾄUP
				}
			}
			NextObjPtr(1, &op, &i1, &i2);								// 次表示立体へ
		}
		if(i3==1||Si6!=i4) {											// <登録済に異常の場合>
			Si6 = 0, Si7 = 0;											// ﾃﾞｰﾀﾘｾｯﾄ
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else if(i5==0&&i6==1) {										// <有効な場合>
			GetObjBox(Sop1, &box);										// 立体BOX取得
			BoxCenter(box, &pnt);										// BOXｾﾝﾀｰ取得
			if(Si1==1) {
				pnt.p[2] = box.bp[0].p[2];								// 中心点Z最小指定
			} else if(Si2==1) {
				pnt.p[2] = box.bp[1].p[2];								// 中心点Z最大指定
			}
			if(Si6>m_MaxCCtl) {											// <制御点上限以上>
				AfxMessageBox(IDS_MANY_CTRLP);							// ｴﾗｰﾒｯｾｰｼﾞ
			} else {
				Spa[Si6] = pnt;											// 中心点保存
				Sia[Si6++] = 0;											// 曲線点
				SetSelNo(Sop1, 1);										// 立体登録
			}
			OffEdtAllObject();											// 編集立体全解除
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		} else if(i7==0) {												// <登録済み立体のみ>
			Si3 = 1;													// 生成終了
		} else {														// <今回のみｴﾗｰの場合>
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
		if(Si3==1) {													// <<<生成終了>>>
			str.Format(IDS_CURVE_CONFIRM, Si6);
			if(AfxMessageBox(str, MB_YESNO)==IDYES) {					// <<生成OK?>>
				if(Si6>2) {												// <生成可能な点数>
					OffEdtAllObject();									// 編集立体全解除
					Display(0);											// 再描画
					if(Si4==0) {										// 開曲線は
						Sia[0] = 1, Sia[Si6-1] = 1;						// 始点/終点は折線点
					}
					StartMenu();										// ﾒﾆｭｰ開始処理
					InitLevel();										// Undo/Redo初期化
					SetCurveMatrix1(Si5, m_CurveWeight, Si4, Si6,
                                    Sia, Sda, Sdb);						// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
					CalcCurvePoint(Si5, Si6, Spa, Spb, Sdb);			// 通過点=>制御点変換
					CrtCurve(Si6, Si4, m_CurveRepeat, m_CurveWeight,
                             Spb, Sia);									// 曲線生成
					GetObjPtr(&Sop1);									// 曲線立体OP保存
					GetObjOif(Sop1, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					if(Si4==1) {
						SetOifName(0, oif, OBJ_NAME01);					// 立体構成名称(曲線[閉])
					} else {
						SetOifName(0, oif, OBJ_NAME02);					//             (曲線[開])
					}
					OnEdtFlg(1, Sop1);									// 曲線edtflg:ON
					CountEdtObj();										// 編集対象立体ｶｳﾝﾄ
					EndJMenu1();										// ﾒﾆｭｰ終了処理
					SetDataUse();										// ﾃﾞｰﾀ使用率設定
					ResetSelNo();										// 立体選択ﾘｾｯﾄ
				} else {												// <制御点不足>
					Si6 = 0, Si7 = 0;									// ﾃﾞｰﾀﾘｾｯﾄ
					ResetSelNo();										// 立体選択ﾘｾｯﾄ
					AfxMessageBox(IDS_FEW_CTRLP);						// ｴﾗｰﾒｯｾｰｼﾞ
				}
			}
		}
	}
	ExitMenu(0, 0);														// ﾒﾆｭｰ終了
	Display(0);															// 再描画
}
