/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewJewelry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputBtnDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "RCommand.h"
#include "Select.h"
#include "CreatCtrlPoint1.h"
#include "EditJewelry.h"
#include "InputBtnDlg2.h"
#include "EditObject.h"
#include "NumFunc.h"

/*************************************/
void CJcad3GlbView::MenuJwlRing(void)									//<<< ﾘﾝｸﾞｻｲｽﾞ合わせ
/*************************************/
{
	OBJTYPE* op;
	PNTTYPE  cp, pa[2];
	BOXTYPE  box;
	int      kind, mode, i, i1, i2;
	double   size=0.0, space, rr, th, sy, wk, mat[3][3];

	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	if(op!=NULL) {														// <編集立体有の場合>
		m_RingKind = (m_FChar&0x1) ? 0 : m_RingKind;					// [F5]:号数指定
		m_RingKind = (m_FChar&0x2) ? 1 : m_RingKind;					// [F6]:直径指定
		mode = (m_FChar&0x8) ? 1 : 0;									// [F8]:接合間隔角度指定
		kind = m_RingKind;
		size = (!kind) ? m_RingSize1 : m_RingSize2;						// ｻｲｽﾞ(号数/直径)
		space = (!mode) ? m_RingSpace1 : m_RingSpace2;					// 接合間隔(mm/度)
		if(KeyboardJwlRing(kind, mode, &size, &space)) {				// ｻｲｽﾞ,接合間隔の入力
			if(!kind) {
				m_RingSize1 = size;
			} else {
				m_RingSize2 = size;
			}
			if(!mode) {
				m_RingSpace1 = space;
			} else {
				m_RingSpace2 = space;
			}
		} else {
			size = 0.0, space = 0.0;
		}
	}
	if(size>0.0) {														// <<ｻｲｽﾞ指定の場合>>
		if((m_GlbDisp==1||m_GlbDisp==2)&&m_GlbChange) {					// <GLB1/GLB2でﾊﾟﾗﾒｰﾀ更新あり>
			if(ReadGLB()!=TRUE) {										// GLB再入力
				AfxMessageBox(IDS_CANT_READ_FILE);						// ｢ﾃﾞｰﾀﾌｧｲﾙ読込NG｣
				DeleteGLB();											// GLB破棄
				m_GlbDisp = 0;
			}
		}
		if(m_DspTmpNo&&m_GlbChange) {									// <ﾃﾝﾌﾟﾚｰﾄ表示でﾊﾟﾗﾒｰﾀ更新あり>
			if(ReadTEMP()!=TRUE) {										// ﾃﾝﾌﾟﾚｰﾄ入力
				AfxMessageBox(IDS_CANT_READ_FILE);						// ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
				DeleteTEMP();											// ﾃﾝﾌﾟﾚｰﾄ破棄
				m_DspTmpNo = 0;
			}
		}
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {												// <編集立体有の場合>
			RedoRingBeforeCommand(op);									// ﾘﾝｸﾞｻｲｽﾞ前ｺﾏﾝﾄﾞ再実行
			NextObjPtr(2, &op, &i1, &i2);								// 次立体へ
		}
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		for(i=0; i<DIM; i++) {
			cp.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;				// ﾎﾞｯｸｽ中心取得
		}
		rr = (!kind) ? 15.0+(size-7.0)/3.0 : size;						// ﾘﾝｸﾞ直径計算
		th = (!mode) ? asin(space/rr) : space*PI/360.0;					// 接合間隔分の角度
		th = (th<-10||th>10) ? 0.0 : th;
		sy = (rr*PI)/(box.bp[1].p[1]-box.bp[0].p[1]);
		sy = (1.0-th/PI)*sy;											// 接合間隔分減算
		th = 360.0-180.0/PI*th*2.0;										// 円筒曲げの角度
		pa[0].p[0] = 0.0, pa[1].p[0] = 0.0;								// 円筒曲げ基準点設定
		pa[0].p[1] = box.bp[0].p[1], pa[1].p[1] = box.bp[1].p[1];
		pa[0].p[2] = box.bp[0].p[2], pa[1].p[2] = box.bp[0].p[2];
		wk = (int)(sy*1000.0)/1000.0;									// 1/1000単位に変換
		mat[0][0] = 1.0, mat[0][1] = 0.0, mat[0][2] = 0.0;				// ﾏﾄﾘｯｸｽ設定
		mat[1][0] = 0.0, mat[1][1] = wk,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
		AffineVtx(2, pa, cp, mat);										// 頂点座標ｱﾌｨﾝ変換
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		Ring(cp, 1.0, sy, 1.0, 1, 2, th, pa[0], pa[1]);					// ﾘﾝｸﾞｻｲｽﾞ合わせ
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		Display(0);														// 再描画
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
	}
}

/****************************************************************/
void CJcad3GlbView::MouseJwlStone(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< 石枠生成
/****************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp=NULL;
	PNTTYPE  pnt, cpt, mip, map;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, i4, ht, es=0, col=0, endf=0, ok=0, ng=0;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			if(Sphase==2) {												// 初期以外は入力ﾁｪｯｸ
				CalcScreenWorld(0, &pnt, point, plane);					// 座標点取得
				if(flg&m_LimKey) {
					LimitPoint(plane, &pnt, &Spnt1);					// ﾏｳｽ入力制限座標
				}
				if(!Si15) {												// <ｻｲｽﾞ変更>
					if(Si16==-1) {
						es = (pnt.p[0]>=Sbase.p[0]) ? 1 : es;
					} else if(Si16==1) {
						es = (pnt.p[0]<=Sbase.p[0]) ? 1 : es;
					}
					if(Si17==-1) {
						es = (pnt.p[1]>=Sbase.p[1]) ? 1 : es;
					} else if(Si17==1) {
						es = (pnt.p[1]<=Sbase.p[1]) ? 1 : es;
					}
				} else {												// <四隅変更>
					if(!Si17) {											// 変更可能範囲取得
						if(Si16==0||Si16==1) {
							i1 = 0, i2 = 1;
						} else {
							i1 = 3, i2 = 2;
						}
					} else {
						if(Si16==0||Si16==3) {
							i1 = 0, i2 = 3;
						} else {
							i1 = 1, i2 = 2;
						}
					}
					for(i=0; i<DIM; i++) {
						cpt.p[i] = (Spc[i1].p[i]+Spc[i2].p[i])/2.0;		// 中間点取得
					}
					if(!Si17) {
						if(Si16==0||Si16==3) {
							mip = Spc[Si16], map = cpt;
						} else {
							mip = cpt, map = Spc[Si16];
						}
					} else {
						if(Si16==0||Si16==1) {
							mip = Spc[Si16], map = cpt;
						} else {
							mip = cpt, map = Spc[Si16];
						}
					}
					es = (pnt.p[Si17]<=mip.p[Si17]) ? 1 : es;
					es = (pnt.p[Si17]>=map.p[Si17]) ? 1 : es;
				}
				if(es==0) {												// <<ｴﾗｰ無の場合>>
					if(Si5==2) {										// <ｽｸｴｱの場合>
						if(!Si15) {										// 変更後のｻｲｽﾞ
							Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
							Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
						} else {										// 四隅の変更
							if(!Si17) {
								Sd5 = fabs(pnt.p[0]-Spc[Si16].p[0]);
							} else {
								Sd6 = fabs(pnt.p[1]-Spc[Si16].p[1]);
							}
						}
					} else {											// <ｽｸｴｱ以外の場合>
						if(Si5==0) {									// 変更後のｻｲｽﾞ
							if(!Si18) {
								Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
								Sd4 = fabs(pnt.p[0]-Sbase.p[0]);
							} else {
								Sd3 = fabs(pnt.p[1]-Sbase.p[1]);
								Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
							}
						} else {
							Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
							Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
						}
					}
					Si2 = 0;
					for(i=0; i<Si1; i++) {								// <断面別に繰り返す>
						i1 = Sis[i];									// 断面別制御点数
						for(j=0; j<i1; j++) {
							Spa[j] = Spw[Si2+j];						// 断面制御点抽出
							Sia[j] = Siw[Si2+j];						// 断面曲線ﾓｰﾄﾞ抽出
						}
						Si2 += i1;										// 合計断面制御点数
						if(Si5==2) {									// <ｽｸｴｱの場合>
							SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                                        i1, Spa, Sia, Spb, Sib);		// ｽｸｴｱ石枠生成
						} else {										// <ｽｸｴｱ以外の場合>
							RoundStone(Sbase, Sd7, Sd3, Sd4, Si3, i1,
                                       Spa, Sia, Spb, Sib);				// ﾗｳﾝﾄﾞ石枠生成
						}
						RedoCrtSurface2(i, Si3, i1, 1, 1, Spb, Sib);	// 曲面変更
					}
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1]);
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spc, Si14))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Spnt1 = Spc[ht];										// 変更基準点保存
				if(ht<4) {												// <<ｻｲｽﾞ変更の場合>>
					Si15 = 0;											// ｻｲｽﾞ変更ﾓｰﾄﾞ
					Si16 = (fabs(Spnt1.p[0]-Sbase.p[0])<EPSILON) ? 0	// 中心点と更新点位置関係
                                      : ((Spnt1.p[0]<Sbase.p[0])?-1:1);
					Si17 = (fabs(Spnt1.p[1]-Sbase.p[1])<EPSILON) ? 0
                                      : ((Spnt1.p[1]<Sbase.p[1])?-1:1);
					Si18 = (Si5==0) ? ht%2 : Si18;						// 変更方向
				} else {												// <<四隅変更の場合>>
					Si15 = 1;											// 四隅変更ﾓｰﾄﾞ
					Si16 = (ht-4)/2;									// 編集情報保存
					Si17 = (ht==5||ht==6||ht==9||ht==10) ? 0 : 1;
				}
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// ﾄﾗｯｶｰ1設定
				Display(0);												// 再描画
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6, &Si14,
                     Spc, Sic, Tens);									// 石枠ﾄﾗｯｶｰ作成
		Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);						// ﾄﾗｯｶｰ1設定
		Display(0);														// 再描画
		Sphase = 1;
	} else if(ms==11) {													// <<<DEL>>>
		if(Si5==2) {													// <ｽｸｴｱの場合>
			Sd5 = 0.0, Sd6 = 0.0;
			Si2 = 0;
			for(i=0; i<Si1; i++) {										// <断面別に繰り返す>
				i1 = Sis[i];											// 断面別制御点数
				for(j=0; j<i1; j++) {
					Spa[j] = Spw[Si2+j];								// 断面制御点抽出
					Sia[j] = Siw[Si2+j];								// 断面曲線ﾓｰﾄﾞ抽出
				}
				Si2 += i1;												// 合計断面制御点数
				SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                            i1, Spa, Sia, Spb, Sib);					// ｽｸｴｱ石枠生成
				RedoCrtSurface2(i, Si3, i1, 1, 1, Spb, Sib);			// 曲面変更
			}
			TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6,
                         &Si14, Spc, Sic, Tens);						// 石枠ﾄﾗｯｶｰ作成
			Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);					// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
			Sphase = 1;
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			Soif = NULL, Sop2 = NULL;
			Si1 = 0, Si2 = 0, Si3 = 0;
			Sd5 = 0.0, Sd6 = 0.0, Sd7 = DBL_MAX;
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si19 = (m_FChar&0x80) ? 1 : 0;								// [F12]:分轄ﾀﾞｲｱﾛｸﾞ無
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			while(Sop1!=NULL) {
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
				GetObjBox(Sop1, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				if(i3==10&&cmp!=NULL) {									// 曲線(閉)は正常
					ok = 1;
				} else {												// 曲線(開)はｴﾗｰ
					ok = 0, ng = 1;
					break;
				}
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON) {		// YZ平面以外はｴﾗｰ
					ok = 0, ng = 1;
					break;
				}
				if(Sd7>box.bp[0].p[1]) {
					Sd7 = box.bp[0].p[1];								// Y最小位置検索
					Sop2 = Sop1;										// Y最小曲線OP保存
				}
				if(++Si1>5) {											// 断面曲線立体数が6以上はｴﾗｰ
					ok = 0, ng = 1;
					break;
				}
				NextObjPtr(2, &Sop1, &i1, &i2);							// 次編集立体へ
			}
			if(ok) {													// <<有効ﾃﾞｰﾀの場合>>
				if(m_Char=='R') {										// Rｷｰ押下はﾗｳﾝﾄﾞ
					Si5 = 0;
				} else if(m_Char=='O') {								// Oｷｰ押下はｵｰﾊﾞﾙ
					Si5 = 1;
				} else if(m_Char=='S') {								// Sｷｰ押下はｽｸｴｱ
					Si5 = 2;
				} else {
					ok = (!KeyboardStoneForm(&Si5)) ? 0 : ok;			// 石枠形状の入力
				}
			}
			if(ok) {													// <<有効ﾃﾞｰﾀの場合>>
				if(m_NumInp!=0) {										// <数値入力指定の場合>
					Si6 = 1;
					if(Si5==0) {										// 石枠直径の入力
						if(KeyboardRoundSize(&m_JwlRndSize)) {
							Sd3 = m_JwlRndSize, Sd4 = m_JwlRndSize;
						} else {
							ok = 0;
						}
					} else {											// 石枠縦横ｻｲｽﾞの入力
						if(KeyboardSquareSize(&m_JwlSqrSize1,
                                              &m_JwlSqrSize2)) {
							Sd3 = m_JwlSqrSize1, Sd4 = m_JwlSqrSize2;
						} else {
							ok = 0;
						}
					}
					ok = (fabs(Sd3)<EPSILON) ? 0 : ok;					// ｷｬﾝｾﾙの場合
				} else {												// <ﾏｳｽ入力指定の場合>
					Si6 = 0;
					Sd3 = m_Fovy/2000.0, Sd4 = m_Fovy/2000.0;			// 画面の長さの半分取得
				}
				Sd3 /= 2.0, Sd4 /= 2.0;									// 半径算出
			}
			if(ok&&Si5==0) {											// <ﾗｳﾝﾄﾞの場合>
				if(!KeyboardProng(&m_ProngNum, &m_ProngLn,
                                  &m_ProngDm, &m_ProngRv)) {			// 爪情報の入力
					ok = 0;
				}
			}
			if(ok) {													// <<有効ﾃﾞｰﾀの場合>>
				if(Si5==2) {
					Si3 = 8;											// ｽｸｴｱの分割数
				} else {
					if(!Si19) {											// ﾀﾞｲｱﾛｸﾞ省略無の場合
						CInputDlg dlg(11);
						dlg.SetValue(m_RevolveDivi);					// 既存値表示
						if(dlg.DoModal()==IDOK) {						// ﾀﾞｲｱﾛｸﾞ表示
							dlg.GetValue(&m_RevolveDivi);				// 回転分割数取得
							Si3 = m_RevolveDivi;
						} else {										// ｷｬﾝｾﾙは中止
							ok = 0;
						}
					} else {											// ﾀﾞｲｱﾛｸﾞ省略は8
						Si3 = 8;
					}
				}
			}
			if(ok) {													// <<有効ﾃﾞｰﾀの場合>>
				ScrToPnt(0, &m_Cent3D, &Sbase);							// 画面の中心取得
				Si1 = 0;
				if(Sop2!=NULL) {										// <最大幅立体(先頭:親)>
					BaseCmdPtr(1, Sop2, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					i3 = GetParaInt(cmp, 0);							// 制御点数取得
					i4 = GetParaInt(cmp, 1);							// 境界ﾓｰﾄﾞ取得
					for(i=0; i<i3; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// ﾗｲﾝﾓｰﾄﾞ取得
						GetParaPnt(cmp, i, &Spa[i]);					// 曲線制御点取得
					}
					SetCurve(i3, i4, Spa);								// 曲線制御点生成
					GetObjPtr(&Sop3);									// 制御点OPの保存
					BaseCmdPtr(1, Sop2, &cmp);							// 有効先頭ｺﾏﾝﾄﾞ取得
					NextCmdPtr(&cmp);									// 曲線編集後のｺﾏﾝﾄﾞ
					while(cmp!=NULL) {
						RedoCommandCopy(Sop3, cmp);						// ｺﾏﾝﾄﾞの複写実行
						NextCmdPtr(&cmp);								// 次ｺﾏﾝﾄﾞへ
					}
					for(i=0; i<i3; i++) {
						GetVtx(Sop3, i, &Spa[i]);						// 立体編集後制御点取得
					}
					FreeObj(Sop3, 1);									// 編集後立体の削除
					if(i3*Si3>m_MaxSCtl) {								// 制御点上限ｵｰﾊﾞｰはｴﾗｰ
						ok = 0, ng = 2;
					}
					for(i=0; i<i3; i++) {
						Spw[Si2+i] = Spa[i];							// 断面制御点保存(累積)
						Siw[Si2+i] = Sia[i];							// 断面曲線ﾓｰﾄﾞ保存(累積)
					}
					Si2 += i3;											// 合計断面制御点数
					Sis[Si1] = i3;										// 断面別制御点数
					Si1++;												// 断面数加算
				}
				BaseObjPtr(2, &Sop1, &i1, &i2);							// 編集対象立体取得
				while(Sop1!=NULL) {
					if(Sop1!=Sop2) {									// <最大幅立体(親)以外>
						BaseCmdPtr(1, Sop1, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						i3 = GetParaInt(cmp, 0);						// 制御点数取得
						i4 = GetParaInt(cmp, 1);						// 境界ﾓｰﾄﾞ取得
						for(i=0; i<i3; i++) {
							Sia[i] = GetParaInt(cmp, 4+i);				// ﾗｲﾝﾓｰﾄﾞ取得
							GetParaPnt(cmp, i, &Spa[i]);				// 曲線制御点取得
						}
						SetCurve(i3, i4, Spa);							// 曲線制御点生成
						GetObjPtr(&Sop3);								// 制御点OPの保存
						BaseCmdPtr(1, Sop1, &cmp);						// 有効先頭ｺﾏﾝﾄﾞ取得
						NextCmdPtr(&cmp);								// 曲線編集後のｺﾏﾝﾄﾞ
						while(cmp!=NULL) {
							RedoCommandCopy(Sop3, cmp);					// ｺﾏﾝﾄﾞの複写実行
							NextCmdPtr(&cmp);							// 次ｺﾏﾝﾄﾞへ
						}
						for(i=0; i<i3; i++) {
							GetVtx(Sop3, i, &Spa[i]);					// 立体編集後制御点取得
						}
						FreeObj(Sop3, 1);								// 編集後立体の削除
						if(i3*Si3>m_MaxSCtl) {							// 制御点上限ｵｰﾊﾞｰはｴﾗｰ
							ok = 0, ng = 2;
							break;
						}
						for(i=0; i<i3; i++) {
							Spw[Si2+i] = Spa[i];						// 断面制御点保存(累積)
							Siw[Si2+i] = Sia[i];						// 断面曲線ﾓｰﾄﾞ保存(累積)
						}
						Si2 += i3;										// 合計断面制御点数
						Sis[Si1] = i3;									// 断面別制御点数
						Si1++;											// 断面数加算
					}
					NextObjPtr(2, &Sop1, &i1, &i2);						// 次編集立体へ
				}
			}
			if(ok) {													// <<有効ﾃﾞｰﾀの場合>>
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				CreatOif(0, NULL, OBJ_NAME00, NULL, &Soif);				// 立体構成情報作成
				SetOifName(0, Soif, OBJ_NAME36);						// 構成名(石枠)
				Delete();												// 2D図形削除
				Si2 = 0;
				for(i=0; i<Si1; i++) {									// <断面別に繰り返す>
					i1 = Sis[i];										// 断面別制御点数
					for(j=0; j<i1; j++) {
						Spa[j] = Spw[Si2+j];							// 断面制御点抽出
						Sia[j] = Siw[Si2+j];							// 断面曲線ﾓｰﾄﾞ抽出
					}
					InitCreat(1, 1, i1, Spa, Sia, &i2);					// 生成初期処理
					for(j=0; j<i1; j++) {
						Spw[Si2+j] = Spa[j];							// 断面制御点再設定
						Siw[Si2+j] = Sia[j];							// 断面曲線ﾓｰﾄﾞ再設定
					}
					Si2 += i1;											// 合計断面制御点数
					if(Si5==2) {										// <ｽｸｴｱの場合>
						SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                                    i1, Spa, Sia, Spb, Sib);			// ｽｸｴｱ石枠生成
					} else {											// <ｽｸｴｱ以外の場合>
						RoundStone(Sbase, Sd7, Sd3, Sd4, Si3, i1,
                                   Spa, Sia, Spb, Sib);					// ﾗｳﾝﾄﾞ石枠生成
					}
					CrtSurface(Si3, i1, 1, 1, m_CurveRepeat,
                               m_CurveWeight, 1, Spb, Sib);				// 曲面生成(初回)
					GetObjPtr(&Sop1);									// 曲面OPの保存
					GetObjOif(Sop1, &oif);								// 立体構成情報ﾎﾟｲﾝﾀ取得
					SetOifLevel(oif, 1);								// 構成ﾚﾍﾞﾙ(1)
					SetOifName(0, oif, OBJ_NAME30);						// 構成名(石座)
					OnEdtFlg(1, Sop1);									// 曲面のedtflg:ON
					if(m_ProngNum>0||Si1>0) {							// <爪/複数断面がある場合>
						if(i==0) {										// 最大幅(先頭)の石座は
							PutObjPrt(Sop1, 1, 1);						// ﾊﾟｰﾂNo設定(親)
						} else {										// 以外の石座は
							PutObjPrt(Sop1, 2, 1);						// ﾊﾟｰﾂNo設定(子)
						}
					}
				}
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
				if((!Si6)||Si5==2) {									// <ﾏｳｽ入力/ｽｸｴｱ>
					OffUndoRedo();										// Undo/Redo不可
					TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6,
                                 &Si14, Spc, Sic, Tens);				// 石枠ﾄﾗｯｶｰ作成
					Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);			// ﾄﾗｯｶｰ1設定
					SetNumDisp(2);										// 数値表示初期処理
					GetAreaBox(2, &box);								// ﾎﾞｯｸｽｻｲｽﾞ取得
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1]);
					Sphase = 1;
				} else {												// <ｷｰﾎﾞｰﾄﾞ入力>
					endf = 1;											// 終了ﾌﾗｸﾞON
				}
				Display(0);												// 再描画
			} else {													// <編集立体:NG>
				if(ng==1) {
					AfxMessageBox(IDS_ERR_SEL7);						// ｴﾗｰﾒｯｾｰｼﾞ
				} else if(ng==2) {
					AfxMessageBox(IDS_MANY_CTRLP);
				}
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
				Sop1 = NULL;
				Sop2 = NULL;
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右押下/終了>>>
		endf = 1;														// 終了ﾌﾗｸﾞON
	}
	if(endf&&Si5==0&&m_ProngNum>0) {									// <終了で爪有の場合>
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		SetProng(m_ProngNum, m_ProngLn, m_ProngDm, m_ProngRv,
                 box, Si2, Spw);										// 石枠爪設定
		CountEdtObj();													// 編集対象立体ｶｳﾝﾄ
	}
	if(endf) {
		if(Soif!=NULL) {
			SelectOifItem(Soif);										// 立体構成ｱｲﾃﾑ選択
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		OnUndoRedo();													// Undo/Redo可
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		SetDataUse();													// ﾃﾞｰﾀ使用率設定
	}
}

/******************************************************************/
void CJcad3GlbView::MouseJwlArrange(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 石枠自動配置
/******************************************************************/
{
	PNTTYPE pnt;
	int     i, ht, col=0;
	double  th, d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			if(Sphase==3) {												// <XY移動>
				CalcScreenWorld(0, &pnt, point, plane);					// 座標点取得
				Sd3 = PntD(&Spnt1, &pnt);								// ﾄﾗｯｶｰ間距離
				d1 = Sd3 - Sd2;											// 基準距離との差
				ArrangeMove(d1, Si6, Si1, Sia, Sib, Spa, Spb, Sva);		// 石枠移動
			} else if(Sphase==4) {										// <水平回転>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				th = atan2(pnt.p[Si4]-Spnt1.p[Si4],
                           pnt.p[Si3]-Spnt1.p[Si3]);
				th = (th<-10||th>10) ? 0.0 : th;
				Sd4 = th*180.0/PI;										// 回転角度
				ArrangeRotate1(Sd4, Si6, Si1, Sia, Sib, Spb);			// 石枠水平回転
			} else if(Sphase==5) {										// <垂直回転>
				CalcScreenWorld(1, &pnt, point, plane);					// 座標点取得
				d1 = pnt.p[Si5]-Spnt3.p[Si5];
				d2 = m_Fovy/4000.0;
				Sd5 = 180.0*d1/d2;										// 回転角度
				ArrangeRotate2(Sd5, Si6, Si1, Sia, Sib, Spb, Svb);		// 石枠垂直回転
			}
			m_pNumDisp->SetParam(Sd3, Sd4, Sd5);
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==2) {
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((ht=Tracker1.HitTracker(1, Si3, pnt, Spy, 3))!=-1) {		// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = (ht==1) ? 3 : ((ht==2)?4:Sphase);
			} else if(Tracker2.HitTracker(1, Si4, pnt, Spz, 1)!=-1) {	// ﾄﾗｯｶｰ2ﾁｪｯｸ
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Display(0);												// 再描画
				Sphase = 5;
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <回転複写中心点>
			Si3 = plane;												// 座標軸1(水平平面)
			Si4 = (Si3+1>2) ? 0 : Si3+1;								// 座標軸2(垂直平面)
			Si5 = (Si4+1>2) ? 0 : Si4+1;								// 座標軸3
			CalcScreenWorld(0, &Spnt1, point, plane);					// 回転中心点座標取得
			InitLevel();												// Undo/Redo初期化
			InitArrange(Si3, Si1, Sd1, Spnt1 ,&Si6, Sia, Sib,
                        Spa, Sva, Svb);									// 石枠配置初期処理
			for(i=0; i<Si1; i++) {
				Spb[i] = Spa[i];										// 石枠中心点複写
			}
			Spnt1.p[Si5] = Spa[0].p[Si5];								// 中心点のSi5座標補正
			Spnt2 = Spa[0];												// 半径調節用ﾄﾗｯｶｰ位置
			Sd2 = PntD(&Spnt1, &Spnt2);									// ﾄﾗｯｶｰ間距離(基準)
			AutoZoom();													// 3面図の自動倍率
			if(m_NumInp!=0) {											// <数値入力の場合>
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// ﾄﾗｯｶｰ1設定
				Display(0);												// 再描画
				CInputBtnDlg4 dlg;
				dlg.SetParam(Si3, Si1, Si6, Sd2, Sia, Sib,
                             Spa, Spb, Sva, Svb, 
                             m_ArangAdd1, m_ArangAdd2, m_ArangAdd3);	// ﾊﾟﾗﾒｰﾀ設定
				dlg.DoModal();											// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetParam(&m_ArangAdd1, &m_ArangAdd2, &m_ArangAdd3);	// ﾊﾟﾗﾒｰﾀ取得
				Tracker1.Reset(0);										// ﾄﾗｯｶｰ1解除
				OnUndoRedo();											// Undo/Redo可
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
				EndJMenu1();											// ﾒﾆｭｰ終了処理
			} else {													// <ﾏｳｽ入力の場合>
				pnt = Spnt1;											// 水平回転用ﾄﾗｯｶｰ
				pnt.p[Si3] += m_Fovy/14000.0;
				Spy[0] = Spnt1, Spy[1] = Spnt2, Spy[2] = pnt;
				Siy[0] = 0, Siy[1] = 1, Siy[2] = 2;
				Tens[0] = 0, Tens[1] = 7, Tens[2] = 8;
				Tracker1.Setup(Spy, Siy, Tens, 3, Si3, 1);				// ﾄﾗｯｶｰ1設定
				Spnt3 = Spnt1;											// 垂直回転用ﾄﾗｯｶｰ
				Spnt3.p[Si5] = Spnt2.p[Si5];
				Spz[0] = Spnt3, Siz[0] = 0, TensC[0] = 6;
				Tracker2.Setup(Spz, Siz, TensC, 1, Si4, 7);				// ﾄﾗｯｶｰ2設定
				Sd3 = Sd2, Sd4 = 0.0, Sd5 = 0.0;						// 現半径,角度設定
				m_pNumDisp->SetParam(Sd3, 0, 0);
				Sphase = 2;
			}
		} else if(Sphase>=2) {											// <編集終了>
			if(Sphase==3) {
				for(i=0; i<DIM; i++) {
					Spy[1].p[i] = Spnt1.p[i]+Sva[0].p[i]*Sd3;
				}
			} else if(Sphase==4) {
				th = PI*Sd4/180.0;
				Spy[2].p[Si3] = Spnt1.p[Si3]+(m_Fovy/14000.0)*cos(th);
				Spy[2].p[Si4] = Spnt1.p[Si4]+(m_Fovy/14000.0)*sin(th);
			} else if(Sphase==5) {
				d1 = m_Fovy/4000.0;
				d2 = d1*Sd5/180.0;
				Spz[0].p[Si5] = Spnt3.p[Si5]+d2;
			}
			Tracker1.Setup(Spy, Siy, Tens, 3, Si3, 1);					// ﾄﾗｯｶｰ1設定
			Tracker2.Setup(Spz, Siz, TensC, 1, Si4, 7);					// ﾄﾗｯｶｰ2設定
			Sphase = 2;
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			Si1 = (KeyboardArrange(&m_ArrangeTh, &m_ArrangeNum)) ?
                                                  m_ArrangeNum : 0;		// 配置情報入力
			if(Si1>0) {													// <OK>
				Sd1 = (fabs((m_ArrangeTh)-360.0)<EPSILON) ?
                             360.0/Si1 : m_ArrangeTh/(Si1-1);			// 1個あたりの角度
				OffUndoRedo();											// Undo/Redo不可
				SetNumDisp(2);											// 数値表示初期処理
				m_pNumDisp->SetParam(0.0, 0.0, 0.0);
				Sphase = 1;
				StartMenu();											// ﾒﾆｭｰ開始処理
			} else {													// <ｷｬﾝｾﾙ>
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase!=0) {
			EndArrange(Si6);											// 石枠配置終了処理
		}
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*************************************/
void CJcad3GlbView::MenuSetUnit(void)									//<<< ﾕﾆｯﾄ･ｵﾌﾞｼﾞｪｸﾄ
/*************************************/
{
	OBJTYPE* op, * wop=NULL, * sop=NULL;
	BOXTYPE  box;
	int      dsp, edt;
	double   d1, d2, mx=0.0;

	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		PutObjPrt(op, 0, 1);											// ﾊﾟｰﾂNo解除
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	if(!(m_FChar&0x1)) {												// <[F5]:解除以外の場合>
		ResetSelNo();													// 選択Noﾘｾｯﾄ
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {
			SetSelNo(op, 1);											// 選択No設定
			GetObjBox(op, &box);										// ﾎﾞｯｸｽ取得
			d1 = box.bp[1].p[0]-box.bp[0].p[0];
			d2 = box.bp[1].p[1]-box.bp[0].p[1];
			d1 = (d1<d2) ? d2 : d1;										// XY最大幅取得
			if(d1>mx) {
				mx = d1, wop = op;										// 石座(最大幅)立体保存
			}
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
		if(wop==NULL) {
			return;														// 立体無は処理中止
		}
		OffEdtAllObject();												// 編集立体全解除
		SetSelNo(wop, 0);												// 選択No解除
		OnEdtFlg(1, wop);												// 立体edtflg:ON
		Copy();															// 立体複写
		GetObjPtr(&sop);												// 立体ﾎﾟｲﾝﾀ取得
		SetSelNo(sop, 2);												// 選択No設定
		SetDispList(sop);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		Delete();														// 旧立体削除
		while(TRUE) {
			wop = NULL;
			SelBaseObjPtr(1, &op);										// 先頭立体取得
			while(op!=NULL) {
				if(GetObjPrt(op)==0) {
					wop = op; break;									// 未ﾕﾆｯﾄ立体を検索
				}
				SelNextObjPtr(1, &op);									// 次立体へ
			}
			if(wop==NULL) {												// 未ﾕﾆｯﾄ立体無終了
				break;
			} else {
				SetSelNo(wop, 0);										// 選択No解除
				OnEdtFlg(1, wop);										// 立体edtflg:ON
				Copy();													// 立体複写
				GetObjPtr(&op);											// 立体ﾎﾟｲﾝﾀ取得
				SetSelNo(op, 2);										// 選択No設定
				SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
				Delete();												// 旧立体削除
			}
		}
		SelBaseObjPtr(2, &op);											// 先頭立体取得
		while(op!=NULL) {
			if(op==sop) {
				PutObjPrt(op, 1, 1);									// ﾊﾟｰﾂNo設定(親)
			} else {
				PutObjPrt(op, 2, 1);									// ﾊﾟｰﾂNo設定(子)
			}
			OnEdtFlg(1, op);											// 立体edtflg:ON
			SelNextObjPtr(2, &op);										// 次立体へ
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
		CountEdtObj();													// 編集対象立体ｶｳﾝﾄ
	}
}

/*************************************************************/
void CJcad3GlbView::MouseEarth(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ｱｰｽ
/*************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt1, pt2;
	BOXTYPE  box;
	int      hit, i, i1, i2;
	double   th, l11, l12, l21, l22;

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
			ObjectEarth(Si1, Si4, Si3, Si2, 0, Sda[0]+Sdb[0],
                        Sda[1]+Sdb[1], Sda[2]+Sdb[2], Sda[3]+Sdb[3],
                        Sda[4]+Sdb[4], Sda[5]+Sdb[5], Sda[6]+Sdb[6],
                        Sda[7]+Sdb[7], Sda[8]+Sdb[8]);					// ｱｰｽ
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
			Si3 = (m_FChar&0x4) ? 0 : 1;								// [F7]:基準点-立体中心指定
			Si3 = (m_FChar&0x8) ? 2 : Si3;								// [F8]:基準点-ｴﾘｱ中心指定
			Si4 = (m_FChar&0x4) ? 0 : 1;								// [F7]:中心点-立体中心指定
			Si4 = (m_FChar&0x8) ? 2 : Si4;								// [F8]:中心点-ｴﾘｱ中心指定
		}
		for(i=0; i<9; i++) {
			Sia[i] = 0, Sda[i] = 0.0, Sdb[i] = 0.0;
		}
		BaseObjPtr(2, &Sop1, &i1, &i2);									// 編集対象立体取得
		while(Sop1!=NULL) {
			EditCmdPtr(2, Sop1, &cmp);									// ｱｰｽｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
			if(cmp!=NULL) {												// <ｱｰｽｺﾏﾝﾄﾞ有>
				for(i=0; i<9; i++) {
					Sia[i] = GetParaInt(cmp, i);						// 整数ﾊﾟﾗﾒｰﾀ取得
				}
				break;
			}
			NextObjPtr(2, &Sop1, &i1, &i2);								// 次立体へ
		}
		i1 = (Sia[6]>Sia[7]) ? Sia[6] : Sia[7];							// 設定済の最大倍率を使用
		i1 = (Sia[8]>i1) ? Sia[8] : i1;
		i1 = (i1==0) ? 1000 : i1;										// 0の場合は1000(1.0倍)へ
		Sia[6] = i1, Sia[7] = i1, Sia[8] = i1;
		for(i=0; i<9; i++) {
			Sda[i] = (double)Sia[i]/1000.0;
		}
		if(m_NumInp!=0) {												// <数値入力の場合>
			CInputBtnDlg7 dlg(2);
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
