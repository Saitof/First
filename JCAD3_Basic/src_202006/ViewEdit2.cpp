/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewEdit2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "DataFile2.h"
#include "DataFile3.h"
#include "Command.h"
#include "Select.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "RCommand.h"
#include "EditObject.h"
#include "FaceEdit.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/************************************************************/
void CJcad3GlbView::MouseBend(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< 円筒曲げ[頂点]
/************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, hw, esw=0;
	double  th, l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON&&plane==Si1) {								// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(Sphase==1) {												// <曲げ範囲変更1回目>
				if(abs(BasePoint.x-point.x)>DRAGMIN||
                   abs(BasePoint.y-point.y)>DRAGMIN) {
					l1 = pnt.p[Si2]-Sbase.p[Si2];
					l2 = pnt.p[Si3]-Sbase.p[Si3];
					if(fabs(l1)>fabs(l2)) {								// <i1方向移動が有効>
						esw = (pnt.p[Si2]<Spa[Si1*4].p[Si2]) ? 1 : esw;
						esw = (pnt.p[Si2]>Spa[Si1*4+2].p[Si2]) ? 1 : esw;
					} else {											// <i2方向移動が有効>
						esw = (pnt.p[Si3]<Spa[Si1*4].p[Si3]) ? 1 : esw;
						esw = (pnt.p[Si3]>Spa[Si1*4+2].p[Si3]) ? 1 : esw;
					}
					if(esw==0) {										// <有効移動範囲>
						if(fabs(l1)>fabs(l2)) {							// <i1方向移動が有効>
							if(Si4==0||Si4==1) {						// 対象辺座標設定
								Spb[0]=Spa[Si1*4], Spb[2]=Spa[Si1*4+1];
								Si7 = 1;
							} else {
								Spb[0]=Spa[Si1*4+3], Spb[2]=Spa[Si1*4+2];
								Si7 = 0;
							}
							Tens[0] = 5, Tens[1] = 6, Tens[2] = 5;
							Si4 = (Si4==0||Si4==3) ? 0 : 2;				// 移動対象頂点位置
							Spb[Si4].p[Si2] += l1;						// 移動分補正
							Si5 = Si2, Si6 = Si3;
						} else {										// <i2方向移動が有効>
							if(Si4==0||Si4==3) {						// 対象辺座標設定
								Spb[0]=Spa[Si1*4], Spb[2]=Spa[Si1*4+3];
								Si7 = 1;
							} else {
								Spb[0]=Spa[Si1*4+1], Spb[2]=Spa[Si1*4+2];
								Si7 = 0;
							}
							Tens[0] = 6, Tens[1] = 5, Tens[2] = 6;
							Si4 = (Si4==0||Si4==1) ? 0 : 2;				// 移動対象頂点位置
							Spb[Si4].p[Si3] += l2;						// 移動分補正
							Si5 = Si3, Si6 = Si2;
						}
						Spb[1].p[0] = (Spb[0].p[0]+Spb[2].p[0])/2.0;	// 中間点座標
						Spb[1].p[1] = (Spb[0].p[1]+Spb[2].p[1])/2.0;
						Spb[1].p[2] = (Spb[0].p[2]+Spb[2].p[2])/2.0;
						Tracker1.Setup(Spb, Sib, Tens, 3, Si1, 7);		// ﾄﾗｯｶｰ1設定
						Tracker2.Reset(0);								// ﾄﾗｯｶｰ2解除
						Tracker3.Reset(0);								// ﾄﾗｯｶｰ3解除
						Sphase = 2;
					}
				}
			} else if(Sphase==2&&Si4!=-1) {								// <曲げ範囲変更2回目以降>
				if(Si4==0) {											// 移動範囲ﾁｪｯｸ
					esw = (pnt.p[Si5]<Spa[Si1*4].p[Si5]) ? 1 : esw;
					esw = (pnt.p[Si5]>=Spb[2].p[Si5]) ? 1 : esw;
				} else {
					esw = (pnt.p[Si5]<=Spb[0].p[Si5]) ? 1 : esw;
					esw = (pnt.p[Si5]>Spa[Si1*4+2].p[Si5]) ? 1 : esw;
				}
				if(esw==0) {											// <有効移動範囲>
					Spb[Si4].p[Si5] = pnt.p[Si5];						// 移動分補正
					Spb[1].p[0] = (Spb[0].p[0]+Spb[2].p[0])/2.0;		// 中間点座標
					Spb[1].p[1] = (Spb[0].p[1]+Spb[2].p[1])/2.0;
					Spb[1].p[2] = (Spb[0].p[2]+Spb[2].p[2])/2.0;
					Tracker1.Setup(Spb, Sib, Tens, 3, Si1, 7);			// ﾄﾗｯｶｰ1設定
				}
			} else if(Sphase>2&&(!m_NumInp)) {							// <角度指定>
				l1 = m_Fovy/4000.0;										// 角度計算
				l2 = Sbase.p[Si6]-pnt.p[Si6];
				th = 360.0*l2/l1;
				ObjectBend(Sphase-3, Si5, Si6, th, Spnt1, Spnt2);		// 立体円筒曲げ
				Sphase = (Sphase==3) ? 4 : Sphase;
				m_pNumDisp->SetParam(th);								// 曲げる角度表示
			}
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		Si4 = -1;
		if(Sphase==0) {													// <基準点取得1>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 12))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sbase = Spa[hit];										// 基準点保存
				Si1 = plane;											// 平面保存
				Si2 = Si1;												// 座標軸設定
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				hw = hit/4;												// 指定ﾄﾗｯｶｰ平面
				Si4 = hit%4;											// 指定ﾄﾗｯｶｰ位置
				if(hw==Si1) {
					Sphase = 1;											// <曲げる範囲変更>
				} else {												// <曲げる角度指定>
					if(hw==Si3) {										// 曲げる情報保存
						if(Si4==0) {
							Spnt1 = Spa[Si1*4], Spnt2 = Spa[Si1*4+1];
							Si7 = 1;
						} else {
							Spnt1 = Spa[Si1*4+3], Spnt2 = Spa[Si1*4+2];
							Si7 = 0;
						}
						Si5 = Si2, Si6 = Si3;
					} else {
						if(Si4==0) {
							Spnt1 = Spa[Si1*4], Spnt2 = Spa[Si1*4+3];
							Si7 = 1;
						} else {
							Spnt1 = Spa[Si1*4+1], Spnt2 = Spa[Si1*4+2];
							Si7 = 0;
						}
						Si5 = Si3, Si6 = Si2;
					}
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Display(0);											// 再描画
					Sphase = 3;
				}
			}
		} else if(Sphase==2&&plane==Si1) {								// <基準点取得2>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spb, 3))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sbase = Spb[hit];										// 基準点保存
				Si4 = hit;
				if(hit==1) {											// <曲げる角度指定>
					Spnt1 = Spb[0], Spnt2 = Spb[2];						// 曲げる範囲
					Tracker1.Reset(0);									// ﾄﾗｯｶｰ1解除
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Display(0);											// 再描画
					Sphase = 3;
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(m_NumInp!=0&&Sphase==3) {									// <数値入力の場合>
			CInputBtnDlg3 dlg(1);
			dlg.SetParam1(Si7, Si5, Si6, Spnt1, Spnt2, m_BendAdd);		// ﾊﾟﾗﾒｰﾀ設定
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetParam(&m_BendAdd);									// ﾊﾟﾗﾒｰﾀ取得
			Display(0);													// 再描画
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(m_NumInp==0&&(Sphase==1||Sphase==3||Sphase==4)) {		// <ﾏｳｽ入力の場合>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &cent);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBend(box, cent, Spa, Tens);							// ﾄﾗｯｶｰ取得
			Spd[0]=Spa[0], Spd[1]=Spa[1], Spd[2]=Spa[2], Spd[3]=Spa[3];
			Spd[4]=Spa[8], Spd[5]=Spa[11], Spd[6]=Spa[4], Spd[7]=Spa[5];
			Tracker1.Setup(Spd, Sia, Tens, 8, 0, 7);					// ﾄﾗｯｶｰ1設定
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[6], Spd[3]=Spa[7];
			Spd[4]=Spa[0], Spd[5]=Spa[3], Spd[6]=Spa[8], Spd[7]=Spa[9];
			Tracker2.Setup(Spd, Sia, Tens, 8, 1, 7);					// ﾄﾗｯｶｰ2設定
			Spd[0]=Spa[8], Spd[1]=Spa[9], Spd[2]=Spa[10], Spd[3]=Spa[11];
			Spd[4]=Spa[4], Spd[5]=Spa[7], Spd[6]=Spa[0], Spd[7]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 8, 2, 7);					// ﾄﾗｯｶｰ3設定
			Display(0);													// 再描画
			Sphase = 0;
		}
	} else if(ms==0||ms==11) {											// <<<初期処理/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;					// ﾄﾗｯｶｰ色設定
		Sia[4] = 0, Sia[5] = 0, Sia[6] = 0, Sia[7] = 0;
		Sib[0] = 2, Sib[1] = 0, Sib[2] = 2;
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &cent);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		TrackerBend(box, cent, Spa, Tens);								// ﾄﾗｯｶｰ取得
		Spd[0]=Spa[0], Spd[1]=Spa[1], Spd[2]=Spa[2], Spd[3]=Spa[3];
		Spd[4]=Spa[8], Spd[5]=Spa[11], Spd[6]=Spa[4], Spd[7]=Spa[5];
		Tracker1.Setup(Spd, Sia, Tens, 8, 0, 7);						// ﾄﾗｯｶｰ1設定
		Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[6], Spd[3]=Spa[7];
		Spd[4]=Spa[0], Spd[5]=Spa[3], Spd[6]=Spa[8], Spd[7]=Spa[9];
		Tracker2.Setup(Spd, Sia, Tens, 8, 1, 7);						// ﾄﾗｯｶｰ2設定
		Spd[0]=Spa[8], Spd[1]=Spa[9], Spd[2]=Spa[10], Spd[3]=Spa[11];
		Spd[4]=Spa[4], Spd[5]=Spa[7], Spd[6]=Spa[0], Spd[7]=Spa[1];
		Tracker3.Setup(Spd, Sia, Tens, 8, 2, 7);						// ﾄﾗｯｶｰ3設定
		Display(0);														// 再描画
		SetNumDisp(2);													// 数値表示初期処理
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*************************************************************/
void CJcad3GlbView::MouseTwist(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ひねり[頂点]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, col=0;
	double  th, l11, l12, l21, l22;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(m_NumInp==0&&Sphase>=1&&plane==Si1&&(flg&MK_LBUTTON)) {		// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			l11 = Spnt2.p[Si1]-Spnt1.p[Si1];							// 角度算出
			l12 = Spnt2.p[Si2]-Spnt1.p[Si2];
			l21 = pnt.p[Si1]-Spnt1.p[Si1];
			l22 = pnt.p[Si2]-Spnt1.p[Si2];
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
			ObjectTwist(Sphase-1, Si3, th, Sd3, Spnt1);					// 立体ひねり
			Sphase = (Sphase==1) ? 2 : Sphase;
			m_pNumDisp->SetParam(th);									// ひねり角度表示
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 4))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si1 = plane;											// 平面保存
				Si2 = (Si1+1>2) ? 0 : Si1+1;							// 座標軸設定
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				GetAreaBox(2, &box);									// ﾎﾞｯｸｽｻｲｽﾞ取得
				BoxCenter(box, &Spnt1);									// ﾎﾞｯｸｽｾﾝﾀｰ取得
				Spnt1.p[Si3] = box.bp[0].p[Si3];						// 軸方向最少値設定
				Sd3 = box.bp[1].p[Si3]-box.bp[0].p[Si3];				// 軸方向ﾎﾞｯｸｽ幅
				Spnt2 = Spa[hit];										// 角度基準点設定
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si1, 2);			// ﾄﾗｯｶｰ1設定
				Tracker2.Reset(0);										// ﾄﾗｯｶｰ2解除
				Tracker3.Reset(0);										// ﾄﾗｯｶｰ3解除
				Display(0);												// 再描画
				Sphase = 1, Sd1 = 0.0, Sd2 = 0.0;
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(m_NumInp!=0&&Sphase==1) {									// <数値入力の場合>
			CInputBtnDlg3 dlg(4);
			dlg.SetParam4(Si3, Sd3, Spnt1, m_TwistAdd);					// ﾊﾟﾗﾒｰﾀ設定
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetParam(&m_TwistAdd);									// ﾊﾟﾗﾒｰﾀ取得
			Tracker1.Reset(0);											// ﾄﾗｯｶｰ1解除
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
			Tracker3.Reset(0);											// ﾄﾗｯｶｰ3解除
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(m_NumInp==0) {										// <ﾏｳｽ入力の場合>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &cent);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBox3D(box, Spa);										// ﾄﾗｯｶｰ取得
			Spd[0]=cent, Spd[0].p[2]=Spa[1].p[2];						// ﾄﾗｯｶｰ1座標設定
			Spd[1]=Spa[0], Spd[2]=Spa[1], Spd[3]=Spa[2], Spd[4]=Spa[3];
			Tens[0]=0, Tens[1]=0, Tens[2]=8, Tens[3]=0, Tens[4]=0;		// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
			Tracker1.Setup(Spd, Sia, Tens, 5, 0, 1);					// ﾄﾗｯｶｰ1設定
			Spd[0]=cent, Spd[0].p[0]=Spa[2].p[0];						// ﾄﾗｯｶｰ2座標設定
			Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=8, Tens[4]=0;		// ﾄﾗｯｶｰ2ｵﾌﾟｼｮﾝ設定
			Tracker2.Setup(Spd, Sia, Tens, 5, 1, 1);					// ﾄﾗｯｶｰ2設定
			Spd[0]=cent, Spd[0].p[1]=Spa[3].p[1];						// ﾄﾗｯｶｰ3座標設定
			Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=0, Tens[4]=8;		// ﾄﾗｯｶｰ3ｵﾌﾟｼｮﾝ設定
			Tracker3.Setup(Spd, Sia, Tens, 5, 2, 1);					// ﾄﾗｯｶｰ3設定
			Sphase = 0;
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0, Sia[4] = 0;		// ﾄﾗｯｶｰ色設定
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &cent);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		TrackerBox3D(box, Spa);											// ﾄﾗｯｶｰ取得
		Spd[0]=cent, Spd[0].p[2]=Spa[1].p[2];							// ﾄﾗｯｶｰ1座標設定
		Spd[1]=Spa[0], Spd[2]=Spa[1], Spd[3]=Spa[2], Spd[4]=Spa[3];
		Tens[0]=0, Tens[1]=0, Tens[2]=8, Tens[3]=0, Tens[4]=0;			// ﾄﾗｯｶｰ1ｵﾌﾟｼｮﾝ設定
		Tracker1.Setup(Spd, Sia, Tens, 5, 0, 1);						// ﾄﾗｯｶｰ1設定
		Spd[0]=cent, Spd[0].p[0]=Spa[2].p[0];							// ﾄﾗｯｶｰ2座標設定
		Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=8, Tens[4]=0;			// ﾄﾗｯｶｰ2ｵﾌﾟｼｮﾝ設定
		Tracker2.Setup(Spd, Sia, Tens, 5, 1, 1);						// ﾄﾗｯｶｰ2設定
		Spd[0]=cent, Spd[0].p[1]=Spa[3].p[1];							// ﾄﾗｯｶｰ3座標設定
		Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=0, Tens[4]=8;			// ﾄﾗｯｶｰ3ｵﾌﾟｼｮﾝ設定
		Tracker3.Setup(Spd, Sia, Tens, 5, 2, 1);						// ﾄﾗｯｶｰ3設定
		Display(0);														// 再描画
		SetNumDisp(2);													// 数値表示初期処理
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*************************************************************/
void CJcad3GlbView::MouseShear(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< せん断[頂点]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, i, j, col=4;
	double  l1, l2, dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(m_NumInp==0&&Sphase>=1&&plane==Si1&&(flg&MK_LBUTTON)) {		// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			l1 = Spnt2.p[Si4]-Spnt1.p[Si4];								// せん断垂直方向距離
			l2 = pnt.p[Si5]-Spnt1.p[Si5];								// せん断水平方向距離
			dd = l2/l1;													// せん断傾き
			ObjectShear(Sphase-1, Si4, Si5, dd, Spnt1);					// せん断
			Sphase = (Sphase==1) ? 2 : Sphase;
			Spc[0].p[Si5] = pnt.p[Si5];									// せん断線水平位置変更
			Line.Setup(Spb, Spc, Sib, 1, Si1);							// せん断線(ﾗｲﾝ)設定
			m_pNumDisp->SetParam(l2);									// せん断数値表示
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <基準点取得>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 6))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Si1 = plane;											// 平面保存
				Si2 = (Si1+1>2) ? 0 : Si1+1;							// 座標軸設定
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				i = hit/2;												// 軸No取得
				if(i!=Si3) {											// <有効なﾄﾗｯｶｰ>
					if(i==Si1) {										// せん断垂直･水平方向設定
						Si4 = Si1, Si5 = Si2;
					} else {
						Si4 = Si2, Si5 = Si1;
					}
					j = hit%2;											// 大小No取得
					if(j==0) {
						Spnt1 = Spa[i*2+1], Spnt2 = Spa[i*2];			// せん断基準点･移動元点設定
					} else {
						Spnt1 = Spa[i*2], Spnt2 = Spa[i*2+1];
					}
					Tracker1.Setup(&Spnt1, &col, NULL, 1, Si1, 2);		// ﾄﾗｯｶｰ1設定
					Tracker2.Reset(0);									// ﾄﾗｯｶｰ2解除
					Tracker3.Reset(0);									// ﾄﾗｯｶｰ3解除
					Spb[0] = Spnt1, Spc[0] = Spnt2;
					Line.Setup(Spb, Spc, Sib, 1, Si1);					// せん断線(ﾗｲﾝ)設定
					Display(0);											// 再描画
					Sphase = 1;
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(m_NumInp!=0&&Sphase==1) {									// <数値入力の場合>
			CInputBtnDlg3 dlg(6);
			dlg.SetParam6(Si4, Si5, Si1, Spnt1, Spnt2, m_ShearAdd);		// ﾊﾟﾗﾒｰﾀ設定
			dlg.DoModal();												// 入力ﾀﾞｲｱﾛｸﾞ表示
			dlg.GetParam(&m_ShearAdd);									// ﾊﾟﾗﾒｰﾀ取得
			Tracker1.Reset(0);											// ﾄﾗｯｶｰ1解除
			Tracker2.Reset(0);											// ﾄﾗｯｶｰ2解除
			Tracker3.Reset(0);											// ﾄﾗｯｶｰ3解除
			Line.Reset();												// ﾗｲﾝ解除
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(m_NumInp==0) {										// <ﾏｳｽ入力の場合>
			GetAreaBox(2, &box);										// ﾎﾞｯｸｽｻｲｽﾞ取得
			BoxCenter(box, &cent);										// ﾎﾞｯｸｽｾﾝﾀｰ取得
			TrackerBoxCrs(box, cent, Spa);								// ﾄﾗｯｶｰ取得
			Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// ﾄﾗｯｶｰ1設定
			Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);				// ﾄﾗｯｶｰ2設定
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);					// ﾄﾗｯｶｰ3設定
			Line.Reset();												// ﾗｲﾝ解除
			Sphase = 0;
		}
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;					// ﾄﾗｯｶｰ色設定
		Sib[0] = 0;														// せん断線ｶﾗｰNo
		GetAreaBox(2, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		BoxCenter(box, &cent);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		TrackerBoxCrs(box, cent, Spa);									// ﾄﾗｯｶｰ取得
		Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// ﾄﾗｯｶｰ1設定
		Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);					// ﾄﾗｯｶｰ2設定
		Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
		Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);						// ﾄﾗｯｶｰ3設定
		Display(0);														// 再描画
		SetNumDisp(2);													// 数値表示初期処理
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Display(0);														// 再描画
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*****************************************************************/
void CJcad3GlbView::MouseLinesEdit(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< 曲線指定移動[頂点]
/*****************************************************************/
{
	OBJTYPE* op, * op1;
	PNTTYPE  pnt, ps[3], pe[3];
	int      i, i1, i2, cl[3];
	double   d1, d2, d3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1) {													// <<基準線2回目>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ﾏｳｽ入力制限座標
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 1;
			Line.Setup(ps, pe, cl, 1, 5);								// ﾗｲﾝ設定
			Display(0);													// 再描画
		} else if(Sphase==3) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i1 = plane;													// 有効座標
			i2 = (i1+1>2) ? 0 : i1+1;
			if(plane==Si3) {											// <始点･終点変更の場合>
				if(Si6==0) {
					Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];	// 始点座標値変更
				} else {
					Spnt2.p[i1] = pnt.p[i1], Spnt2.p[i2] = pnt.p[i2];	// 終点座標値変更
				}
			} else {													// <基準線移動の場合>
				if(Si6==0) {
					for(i=0; i<DIM; i++) {
						Spnt2.p[i] -= Spnt1.p[i];						// 始点･終点移動
					}
					Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];
					for(i=0; i<DIM; i++) {
						Spnt2.p[i] += Spnt1.p[i];
					}
				} else {
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] -= Spnt2.p[i];						// 終点･始点移動
					}
					Spnt2.p[i1] = pnt.p[i1], Spnt2.p[i2] = pnt.p[i2];
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] += Spnt2.p[i];
					}
				}
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 1;
			Line.Setup(ps, pe, cl, 1, 5);								// ﾗｲﾝ設定
			Spa[0] = Spnt1, Spa[1] = Spnt2;								// 終点･始点ﾄﾗｯｶｰ
			Tracker1.Setup(Spa, Sia, NULL, 2, 5, 7);					// ﾄﾗｯｶｰ1設定
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <<基準線1回目>>
			if(plane==0) {												// <指定はXY平面のみ有効>
				CalcScreenWorld(0, &Spnt1, point, plane);				// 座標値保存
				Si3 = 0, Si4 = 1, Si5 = 2;								// 対象平面保存
				Spnt2 = Spnt1, Sphase = 1;
			}
		} else if(Sphase==2) {											// <<基準線変更>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if((Si6=Tracker1.HitTracker(1, plane, pnt, Spa, 2))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sphase = 3, Sbase = Spa[Si6];
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase>0) {													// <<基準線決定>>
			d1 = fabs(Spnt1.p[Si3]-Spnt2.p[Si3]);						// 線分両端差
			d2 = fabs(Spnt1.p[Si4]-Spnt2.p[Si4]);
			d3 = m_Fovy/6000.0;
			if(d1>d3||d2>d3) {											// <<有効距離の場合>>
				Spa[0] = Spnt1, Sia[0] = 4;								// 始点ﾄﾗｯｶｰ
				Spa[1] = Spnt2, Sia[1] = 4;								// 終点ﾄﾗｯｶｰ
				Tracker1.Setup(Spa, Sia, NULL, 2, 5, 7);				// ﾄﾗｯｶｰ1設定
				Sphase = 2;
			} else {													// <無効距離の場合>
				Line.Reset();											// ﾗｲﾝ解除
				Sphase = 0;
			}
			Display(0);													// 再描画
		}
	} else if(ms==11) {													// <<<DEL>>>
		Tracker1.Reset(0);												// ﾄﾗｯｶｰ1解除
		Line.Reset();													// ﾗｲﾝ解除
		Sphase = 0;
		Display(0);														// 再描画
	} else if(ms==0) {													// <<<初期処理>>>
		if(GetSelectNo()==0) {											// <<2D曲線OP無>>
			Si7 = (m_FChar&0x1) ? 0 : 1;								// [F5]:削除なし
			Si8 = (m_FChar&0x2) ? 1 : 0;								// [F6]:距離変更あり
			Si1 = 0, i = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// 編集対象立体取得
			while(op!=NULL) {
				i = (GetObjAtr(op)/10!=1) ? 1 : i;						// 曲線以外はｴﾗｰ
				if(i) {
					break;												// ｴﾗｰは中止
				}
				SetSelNo(op, 1);										// 基準曲線立体登録
				Si1++;													// 曲線数ｶｳﾝﾄUP
				NextObjPtr(2, &op, &i1, &i2);							// 次編集立体へ
			}
			if(!i&&Si1>0&&Si1<11) {										// <有効な場合>
				OffEdtAllObject();										// 編集立体全解除
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
			} else {													// <ｴﾗｰの場合>
				ResetSelNo();											// 立体選択ﾘｾｯﾄ
				AfxMessageBox(IDS_ERR_SEL8);							// ｴﾗｰﾒｯｾｰｼﾞ
			}
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
		} else {														// <<2D曲線OP有>>
			Si7 = (m_FChar&0x1) ? 0 : Si7;								// [F5]:削除なし
			Si8 = (m_FChar&0x2) ? 1 : Si8;								// [F6]:距離変更あり
			Si1 = 0, Si2 = 0, i = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// 表示対象立体取得
			while(op!=NULL) {
				if(GetSelNo(op)==1) {
					i = (GetObjAtr(op)/10!=1) ? 1 : i;					// 曲線以外はｴﾗｰ
					Si1++;												// 基準曲線数ｶｳﾝﾄUP
				} else if(i2==1) {
					Si2++;												// 編集対象立体数ｶｳﾝﾄUP
				}
				if(i) {
					break;												// ｴﾗｰは中止
				}
				NextObjPtr(1, &op, &i1, &i2);							// 次表示立体へ
			}
			if(!i&&Si1>0&&Si1<11&&Si2>0) {								// <有効な場合>
				OffUndoRedo();											// Undo/Redo不可
				CheckCtrlKey1();										// 制御ｷｰ確認(起動時)
				Sphase=0;
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ｶｰｿﾙNo設定
			} else {													// <ｴﾗｰの場合>
				ResetSelNo();											// 立体選択ﾘｾｯﾄ
				AfxMessageBox(IDS_ERR_SEL8);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		}
		Display(0);														// 再描画
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==2) {													// <基準線設定済み>
			BaseObjPtr(2, &op, &i1, &i2);								// 編集対象立体取得
			while(op!=NULL) {
				if(GetSelNo(op)!=1) {									// 基準線曲線以外の
					SetSelNo(op, 2);									// 編集対象立体登録
				}
				NextObjPtr(2, &op, &i1, &i2);							// 次編集立体へ
			}
			OffEdtAllObject();											// 編集立体全解除
			ObjectLinesEdit(Si3, Si4, Si5, Si8, Spnt1, Spnt2);			// 曲線指定移動処理
			if(Si7) {													// <基準曲線削除あり>
				SelBaseObjPtr(1, &op);									// 基準曲線立体取得
				while(op!=NULL) {
					op1 = op;											// 削除立体ﾎﾟｲﾝﾀ保存
					SelNextObjPtr(1, &op);								// 次基準曲線立体へ
					LevelObj(1, op1);									// 同一ﾚﾍﾞﾙ削除立体接続
					TempObj(op1);										// 立体一時削除
				}
			}
			SetDataUse();												// ﾃﾞｰﾀ使用率設定
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Line.Reset();													// ﾗｲﾝ解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/*****************************************/
void CJcad3GlbView::MenuSurfaceEdit(void)								//<<< 曲面指定移動[頂点]
/*****************************************/
{
	OBJTYPE* op;
	int      i1, i2, cnt=0, err=0;

	if(GetSelectNo()==0) {												// <<曲面OP無>>
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// 曲線はｴﾗｰ
			if(err) {
				break;													// ｴﾗｰは中止
			}
			if(++cnt>1) {
				break;													// 曲面数ｶｳﾝﾄUP
			}
			SetSelNo(op, 1);											// 基準曲面立体登録
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		if(err==0&&cnt==1) {											// <有効な場合>
			OffEdtAllObject();											// 編集立体全解除
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
		} else {														// <ｴﾗｰの場合>
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	} else {															// <<曲面OP有>>
		BaseObjPtr(1, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// 曲線はｴﾗｰ
			} else if(i2==1) {
				cnt++;													// 編集対象立体数ｶｳﾝﾄUP
				SetSelNo(op, 2);										// 編集対象立体登録
			}
			if(err) {
				break;													// ｴﾗｰは中止
			}
			NextObjPtr(1, &op, &i1, &i2);								// 次編集立体へ
		}
		if(err==0&&cnt>0) {
			ObjectSurfaceEdit();										// 曲面指定移動
		} else {
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
	}
	Display(0);															// 再描画
}

/****************************************************************/
void CJcad3GlbView::MouseLineEdit(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< 基準線指定移動[頂点]
/****************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt, ps[3], pe[3];
	VECTYPE  vec;
	int      i, j, k, l, m, sft, cl[3];
	double   d1, d2, d3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1) {													// <<基準線2点目>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// 座標点取得(Spnt2=2点目)
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ﾏｳｽ入力制限座標
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 0;
			Line.Setup(ps, pe, cl, 1, 5);								// ﾗｲﾝ設定
			Display(0);													// 再描画
		} else if(Sphase==3) {											// <<基準線変更(点列全体移動)>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i = plane;													// 座標変更有効座標
			j = (i+1>2) ? 0 : i+1;
			d1 = pnt.p[i]-Sbase.p[i];									// 座標変更値
			d2 = pnt.p[j]-Sbase.p[j];
			for(k=0; k<Si4; k++) {
				Spy[k] = Spt[k];										// ﾄﾗｯｶｰ座標復元
				Spy[k].p[i] += d1, Spy[k].p[j] += d2;					// 変更値加算
				Spz[k] = Spy[k], Siz[k] = Siy[k];						// 制御点複写
			}
			Si9 = Si4;
			MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);					// S曲線生成
			for(i=0; i<Si9; i++) {
				Spx[i] = Spz[i];										// 基準曲線保存
			}
			InitBasePos(Si14, 0, Si1, Si9, Spx, &Si6, Sia, Sib, Spa);	// 基準移動初期処理
			SetBaseData(1, Si9, 0, Spz, ps, 3);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);					// ﾄﾗｯｶｰ1設定
			for(i=0; i<3; i++) {
				Spw[0].p[i] = (Spy[0].p[i]+Spy[1].p[i])/2.0;			// 点列全体移動用ﾄﾗｯｶｰ
				Spw[1].p[i] = (Spy[Si4-2].p[i]+Spy[Si4-1].p[i])/2.0;
			}
			Siw[0] = 4, Siw[1] = 4;										// 点列全体移動用ﾄﾗｯｶｰ色
			Tracker2.Setup(Spw, Siw, NULL, 2, 5, 7);					// ﾄﾗｯｶｰ2設定
			Display(0);													// 再描画
		} else if(Sphase==4) {											// <<基準線変更(制御点移動)>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i = plane;													// 座標変更有効座標
			j = (i+1>2) ? 0 : i+1;
			Spy[Si5].p[i] = pnt.p[i], Spy[Si5].p[j] = pnt.p[j];			// 座標値変更
			for(i=0; i<Si10; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// 制御点複写
			}
			j = Si10;
			MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);						// S曲線生成
			if(Si13) {													// <曲線:閉の場合>
				Spz[j++] = Spz[0];										// 始点同一点でｸﾛｰｽﾞ
			}
			EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                        Si6, Sia, Sib, Spa);							// 基準移動編集処理
			SetBaseData(1, j, 0, Spz, ps, -1);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);					// ﾄﾗｯｶｰ1設定
			for(i=0; i<Si10-1; i++) {
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
			}
			if(Si13) {													// <曲線:閉の場合>
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
				Si11 = Si10;											// 追加用ﾄﾗｯｶｰ数
			} else {													// <曲線:開の場合>
				Si11 = Si10-1;											// 追加用ﾄﾗｯｶｰ数
			}
			Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);					// ﾄﾗｯｶｰ2設定
			Si8 = 0;													// 全移動ﾄﾗｯｶｰ無
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <<基準線1点目>>
			CalcScreenWorld(0, &Spnt1, point, plane);					// 座標値保存(Spnt1=1点目)
			Si1 = plane;												// 対象平面保存
			Si2 = (Si1+1>2) ? 0 : Si1+1;
			Si3 = (Si2+1>2) ? 0 : Si2+1;
			Spnt2 = Spnt1;
			Sphase = 1;
		} else if(Sphase==2) {											// <<基準線変更>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shiftｷｰ状態
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(Si8) {													// <全移動ﾄﾗｯｶｰ有の場合>
				Si5 = Tracker2.HitTracker(1, plane, pnt, Spw, 2);		// ﾄﾗｯｶｰ2ﾁｪｯｸ
				if(Si5!=-1) {
					Sphase = 3, Sbase = Spw[Si5];
					for(i=0; i<Si4; i++) {
						Spt[i] = Spy[i];								// ﾄﾗｯｶｰ座標保存
					}
				}
			}
			if(Sphase==2) {												// <ﾄﾗｯｶｰ選択無の場合>
				Si5 = Tracker1.HitTracker(1, plane, pnt, Spy, Si10);	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				if(Si5!=-1) {
					Sphase = 4, Sbase = Spy[Si5];
					if(sft) {
						Siy[Si5] = (Siy[Si5]) ? 0 : 1;					// Shift:ONは曲線ﾓｰﾄﾞ反転
					}
					if(!Si13) {											// <曲線:開の場合>
						Siy[0] = 1, Siy[Si10-1] = 1;					// 始点･終点は曲線ﾓｰﾄﾞ:1
					}
				}
			}
			if(Sphase==2&&Si11>0) {										// <ﾄﾗｯｶｰ選択無の場合>
				Si5 = Tracker2.HitTracker(1, plane, pnt, Spw, Si11);	// ﾄﾗｯｶｰ2ﾁｪｯｸ
				if(Si5!=-1) {
					Sbase = Spw[Si5];
					for(i=Si10-1; i>Si5; i--) {
						Spy[i+1] = Spy[i], Siy[i+1] = Siy[i];			// 後ろへｼﾌﾄ
					}
					Spy[Si5+1] = Sbase;									// 指定位置を追加
					Siy[Si5+1] = (sft) ? 1 : 0;
					Si10++;												// 変更後制御点数
					for(i=0; i<Si10; i++) {
						Spz[i] = Spy[i], Siz[i] = Siy[i];				// 制御点複写
					}
					j = Si10;
					MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);				// S曲線生成
					if(Si13) {											// <曲線:閉の場合>
						Spz[j++] = Spz[0];								// 始点同一点でｸﾛｰｽﾞ
					}
					EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                                Si6, Sia, Sib, Spa);					// 基準移動編集処理
					SetBaseData(1, j, 0, Spz, ps, -1);					// 基準線ﾃﾞｰﾀ登録
					Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);			// ﾄﾗｯｶｰ1設定
					for(i=0; i<Si10-1; i++) {
						for(j=0; j<3; j++) {
							Spw[i].p[j]=(Spy[i].p[j]+Spy[i+1].p[j])/2.0;// 追加用ﾄﾗｯｶｰ
						}
						Siw[i] = 5;										// 追加用ﾄﾗｯｶｰ色
					}
					if(Si13) {											// <曲線:閉の場合>
						for(j=0; j<3; j++) {
							Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;// 追加用ﾄﾗｯｶｰ
						}
						Siw[i] = 5;										// 追加用ﾄﾗｯｶｰ色
						Si11 = Si10;									// 追加用ﾄﾗｯｶｰ数
					} else {											// <曲線:開の場合>
						Si11 = Si10-1;									// 追加用ﾄﾗｯｶｰ数
					}
					Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);			// ﾄﾗｯｶｰ2設定
					Display(0);											// 再描画
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <<基準線決定>>
			d1 = Spnt2.p[Si1]-Spnt1.p[Si1];								// 線分両端差(2点目-1点目)
			d2 = Spnt2.p[Si2]-Spnt1.p[Si2];
			d3 = m_Fovy/6000.0;
			if(fabs(d1)>d3||fabs(d2)>d3){								// <<有効距離の場合>>
				if(m_NumInp!=0) {										// <数値入力の場合>
					CInputDlg dlg(13);
					dlg.SetValue(m_LinePNum);							// 既存値表示
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						dlg.GetValue(&m_LinePNum);						// 基準線制御点数取得
						Si4 = m_LinePNum;
					} else {
						Si4 = 0;
						OnUndoRedo();									// Undo/Redo可
						ExitMenu(0, 1);									// ﾒﾆｭｰ終了
					}
				} else {												// <数値入力無の場合>
					Si4 = 5;
				}
				if(Si4>0) {												// <ｷｬﾝｾﾙ以外の場合>
					Si13 = 0;											// 境界:開
					j = Si4-1;
					d1 /= j, d2 /= j;									// 中間制御点計算
					Spy[0] = Spnt1, Spy[j] = Spnt2;
					for(i=1; i<j; i++) {
						Spy[i].p[Si1] = Spnt1.p[Si1]+d1*i;				// 制御点設定
						Spy[i].p[Si2] = Spnt1.p[Si2]+d2*i;
						Spy[i].p[Si3] = Spnt1.p[Si3];
					}
					for(i=0; i<Si4; i++) {
						Spz[i] = Spy[i];								// ﾄﾗｯｶｰ座標複写
						Siz[i] = 0, Siy[i] = 0;							// 曲線ﾓｰﾄﾞ･ﾄﾗｯｶｰ色設定
					}
					Siy[0] = 1, Siy[Si4-1] = 1;
					Siz[0] = 1, Siz[Si4-1] = 1;
					Si9 = Si4;											// 制御点数
					MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);			// S曲線生成
					for(i=0; i<Si9; i++) {
						Spx[i] = Spz[i];								// 基準曲線保存
					}
					InitLevel();										// Undo/Redo初期化
					Si14 = 0;											// 基準線移動ﾓｰﾄﾞ
					InitBasePos(Si14, 1, Si1, Si9, Spx,
                                &Si6, Sia, Sib, Spa);					// 基準移動初期処理
					SetBaseData(1, Si9, 0, Spz, ps, 3);					// 基準線ﾃﾞｰﾀ登録
					Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);			// ﾄﾗｯｶｰ1設定
					Si8 = 1;											// 点列全体移動用ﾄﾗｯｶｰ有
					for(i=0; i<3; i++) {
						Spw[0].p[i] = (Spy[0].p[i]+Spy[1].p[i])/2.0;	// 点列全体移動用ﾄﾗｯｶｰ
						Spw[1].p[i] = (Spy[Si4-2].p[i]+Spy[Si4-1].p[i])/2.0;
					}
					Siw[0] = 4, Siw[1] = 4;								// 点列全体移動用ﾄﾗｯｶｰ色
					Tracker2.Setup(Spw, Siw, NULL, 2, 5, 7);			// ﾄﾗｯｶｰ2設定
					Si10 = Si4;											// 変更後制御点数(仮)
					Si11 = 0;											// 追加ﾄﾗｯｶｰ数
					Sphase = 2;
				}
			} else {													// <無効距離の場合>
				Sphase = 0;
			}
			Line.Reset();												// ﾗｲﾝ解除
			Display(0);													// 再描画
		} else if(Sphase==3) {											// <<点列全体移動>>
			Sphase = 2;
		} else if(Sphase==4) {											// <<制御点移動>>
			for(i=0; i<Si10; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// 制御点複写
			}
			j = Si10;
			MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);						// S曲線生成
			if(Si13) {													// <曲線:閉の場合>
				Spz[j++] = Spz[0];										// 始点同一点でｸﾛｰｽﾞ
			}
			EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                        Si6, Sia, Sib, Spa);							// 基準移動編集処理
			SetBaseData(1, j, 0, Spz, ps, -1);							// 基準線ﾃﾞｰﾀ登録
			Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);					// ﾄﾗｯｶｰ1設定
			for(i=0; i<Si10-1; i++) {
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
			}
			if(Si13) {													// <曲線:閉の場合>
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
				Si11 = Si10;											// 追加用ﾄﾗｯｶｰ数
			} else {													// <曲線:開の場合>
				Si11 = Si10-1;											// 追加用ﾄﾗｯｶｰ数
			}
			Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);					// ﾄﾗｯｶｰ2設定
			Si8 = 0;													// 全移動ﾄﾗｯｶｰ無
			Display(0);													// 再描画
			Sphase = 2;
		}
	} else if(ms==0) {													// <<<初期処理>>>
		if(GetSelectNo()==0) {											// <<立体OP無>>
			Si7 = (m_FChar&0x1) ? 1 : 0;								// [F5]:距離変更無ﾌﾗｸﾞ
			Si12 = (m_FChar&0x2) ? 1 : 0;								// [F6]:曲線使用ﾌﾗｸﾞ
			if(!Si12) {													// <曲線使用なし>
				OffUndoRedo();											// Undo/Redo不可
				StartMenu();											// ﾒﾆｭｰ開始処理
				CheckCtrlKey1();										// 制御ｷｰ確認(起動時)
				Sphase=0;
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ｶｰｿﾙNo設定
			} else {													// <曲線使用あり>
				k = 0;
				BaseObjPtr(2, &Sop1, &i, &j);							// 編集対象立体取得
				while(Sop1!=NULL) {
					SetSelNo(Sop1, 1);									// 編集対象立体登録
					k++;												// 編集対象立体数ｶｳﾝﾄUP
					NextObjPtr(2, &Sop1, &i, &j);						// 次編集立体へ
				}
				if(k>0) {												// <有効な場合>
					OffEdtAllObject();									// 編集立体全解除
					CountEdtObj();										// 編集対象立体ｶｳﾝﾄ
				}
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		} else {														// <<立体OP有>>
			Si7 = (m_FChar&0x1) ? 1 : Si7;								// [F5]:距離変更無ﾌﾗｸﾞ
			k = 0, l = 0, m = 0;
			BaseObjPtr(1, &Sop2, &i, &j);								// 表示対象立体取得
			while(Sop2!=NULL) {
				if(GetSelNo(Sop2)==1) {
					k++;												// 編集対象立体数ｶｳﾝﾄUP
					OffEdtFlg(1, Sop2);									// edtflg:OFF
				} else if(j==1) {
					Sop1 = Sop2;										// 基準曲線OP保存
					l++;												// 基準曲線数ｶｳﾝﾄUP
					m = (GetObjAtr(Sop2)/10!=1) ? 1 : m;				// 曲線以外はｴﾗｰ
				}
				if(m) {
					break;												// ｴﾗｰは中止
				}
				NextObjPtr(1, &Sop2, &i, &j);							// 次表示立体へ
			}
			if(Si12!=1||k==0) {											// <曲線使用で編集立体有でない場合>
				ResetSelNo();											// 立体選択ﾘｾｯﾄ
				AfxMessageBox(IDS_ERR_SEL1);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			} else if(m!=0||l!=1) {										// <基準曲線が1本でない場合>
				ResetSelNo();											// 立体選択ﾘｾｯﾄ
				AfxMessageBox(IDS_ERR_SEL8);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			} else {													// <有効な場合>
				CheckCtrlKey1();										// 制御ｷｰ確認(起動時)
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ｶｰｿﾙNo設定
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				Si4 = GetParaInt(cmp, 0);								// 制御点数取得
				Si13 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				for(i=0; i<Si4; i++) {
					Siy[i] = GetParaInt(cmp, 4+i);						// ﾗｲﾝﾓｰﾄﾞ取得
					GetParaPnt(cmp, i, &Spy[i]);						// 制御点取得
				}
				OffUndoRedo();											// Undo/Redo不可
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
				SetCurve(Si4, Si13, Spy);								// 曲線制御点生成
				GetObjPtr(&Sop2);										// 制御点OP保存
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				for(i=0; i<Si4; i++) {
					GetVtx(Sop2, i, &Spy[i]);							// 立体編集後制御点取得
					Spz[i] = Spy[i];									// ﾄﾗｯｶｰ座標複写
					Siz[i] = Siy[i];									// 曲線ﾓｰﾄﾞをﾄﾗｯｶｰ色へ
				}
				FreeObj(Sop2, 1);										// 編集後立体削除
				Delete();												// 曲線削除
				OffEdtAllObject();										// 編集立体全解除
				BaseObjPtr(1, &Sop1, &i, &j);							// 表示対象立体取得
				while(Sop1!=NULL) {
					if(GetSelNo(Sop1)==1) {								// 最初の選択立体は
						OnEdtFlg(1, Sop1);								// edtflg:ON
					}
					NextObjPtr(1, &Sop1, &i, &j);						// 次表示立体へ
				}
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
				ResetSelNo();											// 立体選択ﾘｾｯﾄ
				j = 0, k = 0, l = 0, m = 0, d1 = 0.0;
				for(i=1; i<Si4; i++) {
					k = (fabs(Spy[0].p[0]-Spy[i].p[0])>EPSILON) ? 1 : k;
					l = (fabs(Spy[0].p[1]-Spy[i].p[1])>EPSILON) ? 1 : l;
					m = (fabs(Spy[0].p[2]-Spy[i].p[2])>EPSILON) ? 1 : m;
				}
				CalcNVec(Spy, Si4, &vec);								// 法線ﾍﾞｸﾄﾙ取得
				if(!k&&l&&m) {											// 2D平面取得
					Si1 = 1;
				} else if(!l&&m&&k) {
					Si1 = 2;
				} else if(!m&&k&&l) {
					Si1 = 0;
				} else if(!k&&!l&&m) {
					Si1 = 1;
				} else if(!m&&!k&&l) {
					Si1 = 0;
				} else if(!l&&!m&&k) {
					Si1 = 0;
				} else {
					for(i=0; i<DIM; i++) {
						if(fabs(vec.p[i])>d1) {							// 法線ﾍﾞｸﾄﾙ最大方向取得
							d1 = fabs(vec.p[i]), j = i;
						}
					}
			        Si1 = (j+1>2) ? 0 : j+1;
				}
				Si2 = (Si1+1>2) ? 0 : Si1+1;
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				Si9 = Si4;												// 制御点数
				MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);				// S曲線生成
				if(Si13) {												// <曲線:閉の場合>
					Spz[Si9++] = Spz[0];								// 始点同一点でｸﾛｰｽﾞ
				}
				for(i=0; i<Si9; i++) {
					Spx[i] = Spz[i];									// 基準曲線保存
				}
				Si14 = 2;												// 基準線移動ﾓｰﾄﾞ
				InitBasePos(Si14, 1, Si1, Si9, Spx,
                            &Si6, Sia, Sib, Spa);						// 基準移動初期処理
				SetBaseData(1, Si9, 0, Spz, ps, 3);						// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);				// ﾄﾗｯｶｰ1設定
				Si8 = 0;												// 点列全体移動用ﾄﾗｯｶｰ無
				for(i=0; i<Si4-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// 追加用ﾄﾗｯｶｰ
					}
					Siw[i] = 5;											// 追加用ﾄﾗｯｶｰ色
				}
				if(Si13) {												// <曲線:閉の場合>
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;	// 追加用ﾄﾗｯｶｰ
					}
					Siw[i] = 5;											// 追加用ﾄﾗｯｶｰ色
					Si11 = Si4;											// 追加用ﾄﾗｯｶｰ数
				} else {												// <曲線:開の場合>
					Si11 = Si4-1;										// 追加用ﾄﾗｯｶｰ数
				}
				Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);				// ﾄﾗｯｶｰ2設定
				Si10 = Si4;												// 変更後制御点数(仮)
				Sphase = 2;
			}
		}
		Display(0);														// 再描画
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==2) {
			EndBasePos();												// 基準移動終了処理
		}
		if(Si12) {														// <曲線使用時>
			CrtCurve(Si10, Si13, 3, 2.0, Spy, Siy);						// 曲線生成(編集後)
			GetObjPtr(&Sop1);											// 曲線立体OP保存
			GetObjOif(Sop1, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			if(Si13) {
				SetOifName(0, oif, OBJ_NAME01);							// 立体構成名称(曲線[閉])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//             (曲線[開])
			}
		}
		DeleteBaseData();												// 基準線ﾃﾞｰﾀ削除
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/*****************************************************************/
static void SetCirclePoint(int fg, PNTTYPE pt, int i1, int i2,
                           int i3, double d1, double d2,
                           PNTTYPE* py, PNTTYPE* pz, PNTTYPE* px)		//<<< 基準円[三角関数]設定
/*****************************************************************/
{
	int    i;
	double th, d11, d12, d21, d22;

	d11 = pt.p[i1]-d1, d12 = pt.p[i1]+d1;								// i1軸最小･最大
	d21 = pt.p[i2]-d2, d22 = pt.p[i2]+d2;								// i2軸最小･最大
	if(fg) {
		for(i=0; i<4; i++) {
			py[i].p[i3] = pt.p[i3];										// ﾄﾗｯｶｰ設定
		}
	}
	py[0].p[i1] = d11, py[0].p[i2] = d21;
	py[1].p[i1] = d12, py[1].p[i2] = d21;
	py[2].p[i1] = d11, py[2].p[i2] = d22;
	py[3].p[i1] = d12, py[3].p[i2] = d22;
	th = PI/36.0;														// 5度
	for(i=0; i<72; i++) {
		pz[i].p[i1] = pt.p[i1]+d1*cos(th*i);							// 基準円座標設定
		pz[i].p[i2] = pt.p[i2]+d2*sin(th*i);
		pz[i].p[i3] = pt.p[i3];
	}
	if(fg) {
		pz[i] = pz[0];													// 始点同一点でｸﾛｰｽﾞ
		pz[i+1] = pt;													// 中心点末尾追加
		for(i=0; i<74; i++) {
			px[i] = pz[i];												// 基準曲線保存
		}
	}
}

/**********************************************************************/
static void SetCircleCtrlP(PNTTYPE pt, int i1, int i2, int i3,
                           int* pn, double d1, double d2, double* da,
                           double* db, PNTTYPE* pw, PNTTYPE* py,
                           PNTTYPE* pz, PNTTYPE* px, int* iy, int* iz)	//<<< 基準円[曲線]設定
/**********************************************************************/
{
	int    i, cn;
	double th;

	cn = *pn;															// 制御点数
	th = 2.0*PI/(double)cn;												// 制御点毎の角度
	for(i=0; i<cn; i++) {
		pw[i].p[i1] = pt.p[i1]+d1*cos(th*i);							// 基準円座標設定
		pw[i].p[i2] = pt.p[i2]+d2*sin(th*i);
		pw[i].p[i3] = pt.p[i3];
		iy[i] = 0;														// 曲線ﾓｰﾄﾞ
	}
	SetCurveMatrix1(1, 2.0, 1, cn, iy, da, db);							// 通過点=>制御点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(1, cn, pw, py, db);									// 通過点=>制御点変換
	for(i=0; i<cn; i++) {
		pz[i] = py[i], iz[i] = iy[i];
	}
	MakeSCurve(&cn, 1, 3, 2.0, pz, iz);									// S曲線生成
	pz[cn] = pz[0];														// 始点同一点でｸﾛｰｽﾞ
	pz[cn+1] = pt;														// 中心点末尾追加
	for(i=0; i<cn+2; i++) {
		px[i] = pz[i];													// 基準曲線保存
	}
	*pn = cn+1;															// 頂点数
}

/******************************************************************/
void CJcad3GlbView::MouseCircleEdit(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< 基準円指定移動[頂点]
/******************************************************************/
{
	PNTTYPE pnt, wk[3];
	int     i, j, k, sft;
	double  th, d1, d2, dd[4];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1) {													// <<基準円2回目>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ﾏｳｽ入力制限座標
			}
			DrawCircle(Spnt1, Spnt2, Si1);								// 円表示
			Display(0);													// 再描画
		} else if(Sphase==3) {											// <<全移動>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i = plane;													// 対象平面
			j = (i+1>2) ? 0 : i+1;
			Spnt1.p[i] = pnt.p[i], Spnt1.p[j] = pnt.p[j];				// 有効座標を変更
			if(Si7==0) {												// <基準円[三角関数]指定>
				SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                               Sd1, Sd2, Spy, Spz, Spx);				// 基準円[三角関数]設定
				InitBasePos(1, 0, Si1, Si4, Spx, &Si6, Sia, Sib, Spa);	// 基準移動初期処理
				SetBaseData(1, Si4, 0, Spz, wk, 3);						// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);				// ﾄﾗｯｶｰ1設定
				Sps[0] = Spz[18], Sps[1] = Spz[54];
				Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);				// ﾄﾗｯｶｰ2設定
				Spt[0] = Spz[0], Spt[1] = Spz[36];
				Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);				// ﾄﾗｯｶｰ3設定
			} else {													// <曲線[閉]指定>
				d1 = pnt.p[i]-Sbase.p[i];								// 座標変更値
				d2 = pnt.p[j]-Sbase.p[j];
				for(k=0; k<Si8; k++) {
					Spy[k] = Spt[k];									// ﾄﾗｯｶｰ座標復元
					Spy[k].p[i] += d1, Spy[k].p[j] += d2;				// 変更値加算
					Spz[k] = Spy[k], Siz[k] = Siy[k];					// 制御点複写
				}
				Si4 = Si8;
				MakeSCurve(&Si4, 1, 3, 2.0, Spz, Siz);					// S曲線生成
				Spz[Si4] = Spz[0];										// 始点同一点でｸﾛｰｽﾞ
				Spz[Si4+1] = Spnt1;										// 中心点末尾追加
				for(k=0; k<Si4+2; k++) {
					Spx[k] = Spz[k];									// 基準曲線保存
				}
				Si4++;													// 頂点数
				InitBasePos(2, 0, Si1, Si4, Spx, &Si6, Sia, Sib, Spa);	// 基準移動初期処理
				SetBaseData(1, Si4, 0, Spz, wk, 3);						// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);				// ﾄﾗｯｶｰ1設定
			}
			Spw[0] = Spnt1;
			Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);					// ﾄﾗｯｶｰ4設定
			Display(0);													// 再描画
		} else if(Sphase>3) {											// <<変形>>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			if(Si7==0) {												// <基準円[三角関数]指定>
				if(Sphase==4) {
					Sd1 = fabs(pnt.p[Si1]-Spnt1.p[Si1]);				// Si1軸半径
					Sd2 = fabs(pnt.p[Si2]-Spnt1.p[Si2]);				// Si2軸半径
				} else if(Sphase==5) {
					if(Si5==0) {
						Sd3 = pnt.p[Si3]-Spnt1.p[Si3];					// Si2平面のSi3軸移動量1
						Sd4 = (flg&m_NLiKey) ? Sd3 : Sd4;				// 均等
					} else {
						Sd4 = pnt.p[Si3]-Spnt1.p[Si3];					// Si2平面のSi3軸移動量2
						Sd3 = (flg&m_NLiKey) ? Sd4 : Sd3;				// 均等
					}
				} else if(Sphase==6) {
					if(Si5==0) {
						Sd5 = pnt.p[Si3]-Spnt1.p[Si3];					// Si3平面のSi3軸移動量1
						Sd6 = (flg&m_NLiKey) ? Sd5 : Sd6;				// 均等
					} else {
						Sd6 = pnt.p[Si3]-Spnt1.p[Si3];					// Si3平面のSi3軸移動量2
						Sd5 = (flg&m_NLiKey) ? Sd6 : Sd5;				// 均等
					}
				}
				SetCirclePoint(0, Spnt1, Si1, Si2, Si3,
                               Sd1, Sd2, Spy, Spz, Spx);				// 基準円[三角関数]設定
				dd[0] = Sd3, dd[1] = Sd4, dd[2] = Sd5, dd[3] = Sd6;		// 最大移動量検索
				for(d1=0.0, j=0, i=0; i<4; i++) {
					if(fabs(dd[i])>d1) {
						d1 = fabs(dd[i]), j = i;
					}
				}
				th = PI/36.0;											// 5度
				if(j>1) {												// <Si1軸方向変位量で調整>
					d1 = Sd5-Sd3;										// 変位量
					for(i=0; i<18; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd5-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd6-Sd3;										// 変位量
					for(i=18; i<36; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd6-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd6-Sd4;										// 変位量
					for(i=36; i<54; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd6-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd5-Sd4;										// 変位量
					for(i=54; i<72; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd5-d1*sin(d2);	// 基準円座標(Si3軸)
					}
				} else {												// <Si2軸方向変位量で調整>
					d1 = Sd3-Sd6;										// 変位量
					for(i=18; i<36; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd3-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd4-Sd6;										// 変位量
					for(i=36; i<54; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd4-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd4-Sd5;										// 変位量
					for(i=54; i<72; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd4-d1*sin(d2);	// 基準円座標(Si3軸)
					}
					d1 = Sd3-Sd5;										// 変位量
					for(i=0; i<18; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1軸方向の比率
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd3-d1*sin(d2);	// 基準円座標(Si3軸)
					}
				}
				Spz[72] = Spz[0];
				EditBasePos(1, Si1, 0, Si4, Spx, Si4, Spz,
                            Si6, Sia, Sib, Spa);						// 基準移動編集処理
				SetBaseData(1, Si4, 0, Spz, wk, -1);					// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);				// ﾄﾗｯｶｰ1設定
				Sps[0] = Spz[18], Sps[1] = Spz[54];
				Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);				// ﾄﾗｯｶｰ2設定
				Spt[0] = Spz[0], Spt[1] = Spz[36];
				Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);				// ﾄﾗｯｶｰ3設定
				Tracker4.Reset(0);										// ﾄﾗｯｶｰ4解除
			} else {													// <曲線[閉]指定>
				i = plane;												// 座標変更有効座標
				j = (i+1>2) ? 0 : i+1;
				Spy[Si5].p[i] = pnt.p[i], Spy[Si5].p[j] = pnt.p[j];		// 座標値変更
				for(i=0; i<Si10; i++) {
					Spz[i] = Spy[i], Siz[i] = Siy[i];					// 制御点複写
				}
				k = Si10;
				MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);					// S曲線生成
				Spz[k++] = Spz[0];										// 始点同一点でｸﾛｰｽﾞ
				EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                            Si6, Sia, Sib, Spa);						// 基準移動編集処理
				SetBaseData(1, k, 0, Spz, wk, -1);						// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);				// ﾄﾗｯｶｰ1設定
				for(i=0; i<Si10-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// 追加用ﾄﾗｯｶｰ
					}
					Siw[i] = 5;											// 追加用ﾄﾗｯｶｰ色
				}
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
				Si11 = Si10;											// 追加用ﾄﾗｯｶｰ数
				Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);				// ﾄﾗｯｶｰ4設定
			}
			Si9 = 0;													// 全移動ﾄﾗｯｶｰ無
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==0) {													// <<基準円1回目>>
			CalcScreenWorld(0, &Spnt1, point, plane);					// 回転中心点座標取得
			Si1 = plane;												// 対象平面保存
			Si2 = (Si1+1>2) ? 0 : Si1+1;
			Si3 = (Si2+1>2) ? 0 : Si2+1;
			Spnt2 = Spnt1;
			if(m_NumInp!=0) {											// <数値入力の場合>
				CInputDlg dlg(14+Si1);
				dlg.SetValue(m_CircleSize1, m_CircleSize2);				// 既存値表示
				if(dlg.DoModal()==IDOK) {								// 入力ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_CircleSize1, &m_CircleSize2);		// 基準円ｻｲｽﾞ取得
					Sd1 = m_CircleSize1*0.5;							// Si1軸半径
					Sd2 = m_CircleSize2*0.5;							// Si2軸半径
					Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Sd6 = 0.0;			// Si3軸移動量
					if(Si7==0) {										// <基準円[三角関数]指定>
						SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                                       Sd1, Sd2, Spy, Spz, Spx);		// 基準円[三角関数]設定
						Si4 = 73;
						InitLevel();									// Undo/Redo初期化
						InitBasePos(1, 1, Si1, Si4,
                                    Spx, &Si6, Sia, Sib, Spa);			// 基準移動初期処理
						SetBaseData(1, Si4, 0, Spz, wk, 3);				// 基準線ﾃﾞｰﾀ登録
						Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);		// ﾄﾗｯｶｰ1設定
						Sps[0] = Spz[18], Sps[1] = Spz[54];
						Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);		// ﾄﾗｯｶｰ2設定
						Spt[0] = Spz[0], Spt[1] = Spz[36];
						Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);		// ﾄﾗｯｶｰ3設定
					} else {											// <曲線[閉]指定>
						Si4 = Si8;										// 指定制御点数
						SetCircleCtrlP(Spnt1, Si1, Si2, Si3, &Si4,
                                       Sd1, Sd2, Sda, Sdb, Spw,
                                       Spy, Spz, Spx, Siy, Siz);		// 基準円[曲線]設定
						InitLevel();									// Undo/Redo初期化
						InitBasePos(2, 1, Si1, Si4,
                                    Spx, &Si6, Sia, Sib, Spa);			// 基準移動初期処理
						SetBaseData(1, Si4, 0, Spz, wk, 3);				// 基準線ﾃﾞｰﾀ登録
						Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);		// ﾄﾗｯｶｰ1設定
						Si10 = Si8;										// 変更後制御点数(仮)
						Si11 = 0;										// 追加ﾄﾗｯｶｰ数
					}
					Si9 = 1;											// 全移動ﾄﾗｯｶｰ有
					Spw[0] = Spnt1;
					Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);			// ﾄﾗｯｶｰ4設定
					Display(0);											// 再描画
					Sphase = 2;
				} else {
					OnUndoRedo();										// Undo/Redo可
					ExitMenu(0, 1);										// ﾒﾆｭｰ終了
				}
			} else {													// <ﾏｳｽ入力の場合>
				Sphase = 1;
			}
		} else if(Sphase==2) {											// <<<基準線変更>>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shiftｷｰ状態
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			if(Si7==0) {												// <<基準円[三角関数]指定>>
				if(Si9) {												// <全移動ﾄﾗｯｶｰ有の場合>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, 1);	// ﾄﾗｯｶｰ4ﾁｪｯｸ
					if(Si5!=-1) {
						Sphase = 3, Sbase = Spw[Si5];
					}
				}
				if(Sphase==2) {											// <全移動ﾄﾗｯｶｰ選択無の場合>
					if(plane==Si1) {
						Si5=Tracker1.HitTracker(1, plane, pnt, Spy, 4);	// ﾄﾗｯｶｰ1ﾁｪｯｸ
						if(Si5!=-1) {
							Sphase = 4, Sbase = Spy[Si5];
						}
					} else if(plane==Si2) {
						Si5=Tracker2.HitTracker(1, plane, pnt, Sps, 2);	// ﾄﾗｯｶｰ2ﾁｪｯｸ
						if(Si5!=-1) {
							Sphase = 5, Sbase = Sps[Si5];
						}
					} else if(plane==Si3) {
						Si5=Tracker3.HitTracker(1, plane, pnt, Spt, 2);	// ﾄﾗｯｶｰ3ﾁｪｯｸ
						if(Si5!=-1) {
							Sphase = 6, Sbase = Spt[Si5];
						}
					}
				}
			} else {													// <<曲線[閉]指定>>
				if(Si9) {												// <全移動ﾄﾗｯｶｰ有の場合>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, 1);	// ﾄﾗｯｶｰ4ﾁｪｯｸ
					if(Si5!=-1) {
						Sphase = 3, Sbase = Spw[Si5];
						for(i=0; i<Si8; i++) {
							Spt[i] = Spy[i];							// ﾄﾗｯｶｰ座標保存
						}
					}
				}
				if(Sphase==2) {											// <ﾄﾗｯｶｰ選択無の場合>
					Si5 = Tracker1.HitTracker(1, plane, pnt, Spy, Si10);// ﾄﾗｯｶｰ1ﾁｪｯｸ
					if(Si5!=-1) {
						Sphase = 4, Sbase = Spy[Si5];
						if(sft) {
							Siy[Si5] = (Siy[Si5]) ? 0 : 1;				// Shift:ONは曲線ﾓｰﾄﾞ反転
						}
					}
				}
				if(Sphase==2&&Si11>0) {									// <ﾄﾗｯｶｰ選択無の場合>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, Si11);// ﾄﾗｯｶｰ4ﾁｪｯｸ
					if(Si5!=-1) {
						Sbase = Spw[Si5];
						for(i=Si10-1; i>Si5; i--) {
							Spy[i+1] = Spy[i], Siy[i+1] = Siy[i];		// 後ろへｼﾌﾄ
						}
						Spy[Si5+1] = Sbase;								// 指定位置を追加
						Siy[Si5+1] = (sft) ? 1 : 0;
						Si10++;											// 変更後制御点数
						for(i=0; i<Si10; i++) {
							Spz[i] = Spy[i], Siz[i] = Siy[i];			// 制御点複写
						}
						k = Si10;
						MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);			// S曲線生成
						Spz[k++] = Spz[0];								// 始点同一点でｸﾛｰｽﾞ
						EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                                    Si6, Sia, Sib, Spa);				// 基準移動編集処理
						SetBaseData(1, k, 0, Spz, wk, -1);				// 基準線ﾃﾞｰﾀ登録
						Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);		// ﾄﾗｯｶｰ1設定
						for(i=0; i<Si10-1; i++) {
							for(j=0; j<3; j++) {
								Spw[i].p[j] = 
                                      (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// 追加用ﾄﾗｯｶｰ
							}
							Siw[i] = 5;									// 追加用ﾄﾗｯｶｰ色
						}
						for(j=0; j<3; j++) {
							Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;// 追加用ﾄﾗｯｶｰ
						}
						Siw[i] = 5;										// 追加用ﾄﾗｯｶｰ色
						Si11 = Si10;									// 追加用ﾄﾗｯｶｰ数
						Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);		// ﾄﾗｯｶｰ4設定
						Display(0);										// 再描画
					}
				}
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		if(Sphase==1) {													// <<基準円決定>>
			Sd1 = Spnt2.p[Si1]-Spnt1.p[Si1];							// 半径の変位量
			Sd2 = Spnt2.p[Si2]-Spnt1.p[Si2];
			Sd3 = m_Fovy/12000.0;
			if(fabs(Sd1)>Sd3||fabs(Sd2)>Sd3){							// <有効距離の場合>
				Sd1 = sqrt(Sd1*Sd1+Sd2*Sd2);							// 半径
				Sd2 = Sd1;
				Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Sd6 = 0.0;				// Si3軸移動量
				if(Si7==0) {											// <基準円[三角関数]指定>
					SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                                   Sd1, Sd2, Spy, Spz, Spx);			// 基準円[三角関数]設定
					Si4 = 73;
					InitLevel();										// Undo/Redo初期化
					InitBasePos(1, 1, Si1, Si4,
                                Spx, &Si6, Sia, Sib, Spa);				// 基準移動初期処理
					SetBaseData(1, Si4, 0, Spz, wk, 3);					// 基準線ﾃﾞｰﾀ登録
					Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);			// ﾄﾗｯｶｰ1設定
					Sps[0] = Spz[18], Sps[1] = Spz[54];
					Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);			// ﾄﾗｯｶｰ2設定
					Spt[0] = Spz[0], Spt[1] = Spz[36];
					Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);			// ﾄﾗｯｶｰ3設定
				} else {												// <曲線[閉]指定>
					Si4 = Si8;											// 指定制御点数
					SetCircleCtrlP(Spnt1, Si1, Si2, Si3, &Si4,
                                   Sd1, Sd2, Sda, Sdb, Spw,
                                   Spy, Spz, Spx, Siy, Siz);			// 基準円[曲線]設定
					InitLevel();										// Undo/Redo初期化
					InitBasePos(2, 1, Si1, Si4,
                                Spx, &Si6, Sia, Sib, Spa);				// 基準移動初期処理
					SetBaseData(1, Si4, 0, Spz, wk, 3);					// 基準線ﾃﾞｰﾀ登録
					Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);			// ﾄﾗｯｶｰ1設定
					Si10 = Si8;											// 変更後制御点数(仮)
					Si11 = 0;											// 追加ﾄﾗｯｶｰ数
				}
				Si9 = 1;												// 全移動ﾄﾗｯｶｰ有
				Spw[0] = Spnt1;
				Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);				// ﾄﾗｯｶｰ4設定
				Sphase = 2;
			} else {													// <無効距離の場合>
				Sphase = 0;
			}
			DeleteCircle();												// 円破棄
			Display(0);													// 再描画
		} else if(Sphase>2) {
			if(Si7==1&&Sphase==4) {										// <曲線[閉]指定で制御点移動>
				for(i=0; i<Si10; i++) {
					Spz[i] = Spy[i], Siz[i] = Siy[i];					// 制御点複写
				}
				k = Si10;
				MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);					// S曲線生成
				Spz[k++] = Spz[0];										// 始点同一点でｸﾛｰｽﾞ
				EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                            Si6, Sia, Sib, Spa);						// 基準移動編集処理
				SetBaseData(1, k, 0, Spz, wk, -1);						// 基準線ﾃﾞｰﾀ登録
				Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);				// ﾄﾗｯｶｰ1設定
				for(i=0; i<Si10-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// 追加用ﾄﾗｯｶｰ
					}
					Siw[i] = 5;											// 追加用ﾄﾗｯｶｰ色
				}
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// 追加用ﾄﾗｯｶｰ
				}
				Siw[i] = 5;												// 追加用ﾄﾗｯｶｰ色
				Si11 = Si10;											// 追加用ﾄﾗｯｶｰ数
				Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);				// ﾄﾗｯｶｰ4設定
				Si9 = 0;												// 全移動ﾄﾗｯｶｰ無
				Display(0);												// 再描画
			}
			Sphase = 2;
		}
	} else if(ms==0) {													// <<<初期処理>>>
		Si7 = (m_FChar&0x1) ? 1 : 0;									// [F5]:曲線[制御点]ﾌﾗｸﾞ
		OffUndoRedo();													// Undo/Redo不可
		StartMenu();													// ﾒﾆｭｰ開始処理
		CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
		Sphase=0;
		Siy[0] = 0, Siy[1] = 0, Siy[2] = 0, Siy[3] = 0;					// ﾄﾗｯｶｰ色設定
		Sis[0] = 0, Sis[1] = 0, Siw[0] = 4;
		Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;				// ﾄﾗｯｶｰｵﾌﾟｼｮﾝ設定
		TensC[0] = 6, TensC[1] = 6, Six[0] = 5, Six[1] = 5;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
		if(Si7!=0) {													// <曲線[制御点]の場合>
			CInputDlg dlg(28);
			dlg.SetValue(m_CirclePNum);									// 既存値表示
			if(dlg.DoModal()==IDOK) {									// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(&m_CirclePNum);							// 基準円制御点数取得
				Si8 = m_CirclePNum;
			} else {
				OnUndoRedo();											// Undo/Redo可
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			}
		}
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==2) {
			EndBasePos();												// 基準移動終了処理
		}
		DeleteBaseData();												// 基準線ﾃﾞｰﾀ削除
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Tracker2.Reset(1);												// ﾄﾗｯｶｰ2解除
		Tracker3.Reset(1);												// ﾄﾗｯｶｰ3解除
		Tracker4.Reset(1);												// ﾄﾗｯｶｰ4解除
		Display(0);														// 再描画
		OnUndoRedo();													// Undo/Redo可
		ExitMenu(0, 1);													// ﾒﾆｭｰ終了
		EndJMenu1();													// ﾒﾆｭｰ終了処理
	}
}

/************************************************************/
void CJcad3GlbView::MousePCut(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< 平面分割
/************************************************************/
{
	OBJTYPE *op;
	CString fname, path;
	int     i1, i2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽの左釦押下中のみ有効
			CalcScreenWorld(0, &Spb[0], point, plane);					// 分割線(ﾗｲﾝ)終点
			if(fabs(Spa[0].p[0]-Spb[0].p[0])>EPSILON||
               fabs(Spa[0].p[1]-Spb[0].p[1])>EPSILON||
               fabs(Spa[0].p[2]-Spb[0].p[2])>EPSILON) {
				if(flg&m_LimKey) {
					LimitPoint(plane, &Spb[0], &Spa[0]);				// ﾏｳｽ入力制限座標
				}
				Line.Setup(Spa, Spb, Sia, 1, plane);					// 分割線(ﾗｲﾝ)設定
				Sphase = 1;
				Display(0);												// 再描画
			}
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(0, &Spa[0], point, plane);						// 分割線(ﾗｲﾝ)始点
	} else if(ms==3) {													// <<ﾏｳｽの左釦解放>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Spb[0], point, plane);					// 分割線(ﾗｲﾝ)終点
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spb[0], &Spa[0]);					// ﾏｳｽ入力制限座標
			}
			Line.Setup(Spa, Spb, Sia, 1, plane);						// 分割線(ﾗｲﾝ)設定
			Display(0);													// 再描画
			if(AfxMessageBox(IDS_RUN_CONFIRM, MB_YESNO)==IDYES) {		// <確認:OK>
				CCurveDiviDlg dlg;
				dlg.SetValue(m_CurveDvd, m_PolygonF, m_FileBUpF);
				if(dlg.DoModal()==IDOK) {								// 曲面細分化設定ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_CurveDvd, &m_PolygonF, &m_FileBUpF);
					if(m_FileBUpF>0&&GetDemoFlg()==0) {					// <ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ有で製品版>
						fname.Format(PCB_NAME, m_PCutCnt);				// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名
						path = m_SaveFilePath+fname;					// ﾌﾙﾊﾟｽ
						if(WriteJgdFile(path, 2)!=0) {					// JGD形式ﾌｧｲﾙ保存
							AfxMessageBox(IDS_ERR_FILEWRITE);			// ｢ﾌｧｲﾙ保存:NG｣
						}
						if(++m_PCutCnt>=m_SaveFileMax) {
							m_PCutCnt = 0;
						}
					}
					if(m_CurveDvd>0) {									// <曲面細分化有>
						OnCurveReptSW(m_CurveDvd);						// 曲面細分化SWｵﾝ
						BaseObjPtr(2, &op, &i1, &i2);					// 編集立体ﾎﾟｲﾝﾀ取得
						while(op!=NULL) {
							RedoCommand(op);							// ｺﾏﾝﾄﾞ再実行
							SetDispList(op);							// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
							NextObjPtr(2, &op, &i1, &i2);				// 次編集立体へ
						}
						Display(0);										// 再描画
					}
					if(m_PolygonF>0) {									// <三角ﾊﾟｯﾁ化有>
						if(MakePolygonData(1)!=0) {						// ﾎﾟﾘｺﾞﾝﾃﾞｰﾀ作成
							AfxMessageBox(IDS_ERR_POLYGON);				// ｢ﾎﾟﾘｺﾞﾝ化:NG｣
						}
					}
					StartMenu();										// ﾒﾆｭｰ開始処理
					InitLevel();										// Undo/Redo初期化
					PCutObject(Spa[0], Spb[0], plane);					// 立体の平面分割
					ExitMenu(0, 0);										// ﾒﾆｭｰ終了
					EndJMenu1();										// ﾒﾆｭｰ終了処理
					if(m_CurveDvd>0) {									// <曲面細分化有>
						OffCurveReptSW();								// 曲面細分化SWｵﾌ
					}
				}
			}
		}
		Line.Reset();													// ﾗｲﾝ解除
		Display(0);														// 再描画
		Sphase = 0;														// ﾌｪｰｽﾞｸﾘｱ
	} else if(ms==0) {													// <<<初期処理>>>
		Sia[0] = 0;														// ﾗｲﾝｶﾗｰNo
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/*************************************/
void CJcad3GlbView::MenuBoolean(void)									//<<< 図形演算
/*************************************/
{
	OBJTYPE* op;
	CString  fname, path;
	int      i1, i2, err=0, cnt1=0, cnt2=0, mode=-1, delb=0;

	if(GetSelectNo()==0) {												// <<図形演算OP無>>
		BaseObjPtr(2, &op, &i1, &i2);									// 編集対象立体取得
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// 曲線はｴﾗｰ
			if(err) {
				break;													// ｴﾗｰは中止
			}
			SetSelNo(op, 1);											// 図形演算立体登録
			cnt1++;														// ｶｳﾝﾄUP
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		if(!err&&cnt1>0) {												// <有効な場合>
			OffEdtAllObject();											// 編集立体全解除
		} else {														// <ｴﾗｰの場合>
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
			AfxMessageBox(IDS_ERR_SEL1);								// ｴﾗｰﾒｯｾｰｼﾞ
		}
	} else {															// <<図形演算OP有>>
		BaseObjPtr(1, &op, &i1, &i2);									// 表示対象立体取得
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// 曲線はｴﾗｰ
				cnt1++;													// 断面曲線ｶｳﾝﾄUP
			} else if(i2==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// 曲線はｴﾗｰ
				cnt2++;													// 掃引線ｶｳﾝﾄUP
			}
			if(err) {
				break;													// ｴﾗｰは中止
			}
			NextObjPtr(1, &op, &i1, &i2);								// 次表示立体へ
		}
		if(!err&&cnt1>0&&cnt2>0) {										// <有効な場合>
			if(m_Char=='1') {											// 1ｷｰ押下は演算1
				mode = 0;
			} else if(m_Char=='2') {									// 2ｷｰ押下は演算2
				mode = 1;
			} else if(m_Char=='3') {									// 3ｷｰ押下は演算3
				mode = 2;
			} else {
				CBooleanDlg dlg;
				if(dlg.DoModal()==IDOK) {								// ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&mode, &delb);							// 図形演算種類取得
				}
			}
			if(mode>=0) {												// <演算指定OK>
				CCurveDiviDlg dlg;
				dlg.SetValue(m_CurveDvd, m_PolygonF, m_FileBUpF);
				if(dlg.DoModal()==IDOK) {								// 曲面細分化設定ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_CurveDvd, &m_PolygonF, &m_FileBUpF);
					if(m_FileBUpF>0&&GetDemoFlg()==0) {					// <ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ有で製品版>
						fname.Format(BLB_NAME, m_BoolCnt);				// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名
						path = m_SaveFilePath+fname;					// ﾌﾙﾊﾟｽ
						if(WriteJgdFile(path, 2)!=0) {					// JGD形式ﾌｧｲﾙ保存
							AfxMessageBox(IDS_ERR_FILEWRITE);			// ｢ﾌｧｲﾙ保存:NG｣
						}
						if(++m_BoolCnt>=m_SaveFileMax) {
							m_BoolCnt = 0;
						}
					}
					if(m_CurveDvd>0) {									// <曲面細分化有>
						OnCurveReptSW(m_CurveDvd);						// 曲面細分化SWｵﾝ
						SelBaseObjPtr(1, &op);							// A側立体取得
						while(op!=NULL) {
							RedoCommand(op);							// ｺﾏﾝﾄﾞ再実行
							SetDispList(op);							// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
							SelNextObjPtr(1, &op);						// 次立体へ
						}
						BaseObjPtr(2, &op, &i1, &i2);					// B側立体ﾎﾟｲﾝﾀ取得
						while(op!=NULL) {
							if(GetSelNo(op)!=1) {
								RedoCommand(op);						// ｺﾏﾝﾄﾞ再実行
								SetDispList(op);						// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
							}
							NextObjPtr(2, &op, &i1, &i2);				// 次編集立体へ
						}
						Display(0);										// 再描画
					}
					if(m_PolygonF>0) {									// <三角ﾊﾟｯﾁ化有>
						if(MakePolygonData(0)!=0) {						// A側ﾎﾟﾘｺﾞﾝﾃﾞｰﾀ作成
							AfxMessageBox(IDS_ERR_POLYGON);				// ｢ﾎﾟﾘｺﾞﾝ化:NG｣
						}
						if(MakePolygonData(1)!=0) {						// B側ﾎﾟﾘｺﾞﾝﾃﾞｰﾀ作成
							AfxMessageBox(IDS_ERR_POLYGON);				// ｢ﾎﾟﾘｺﾞﾝ化:NG｣
						}
					}
					StartMenu();										// ﾒﾆｭｰ開始処理
					InitLevel();										// Undo/Redo初期化
					BooleanObject(mode, delb);							// 立体図形演算
					EndJMenu1();										// ﾒﾆｭｰ終了処理
					if(m_CurveDvd>0) {									// <曲面細分化有>
						OffCurveReptSW();								// 曲面細分化SWｵﾌ
					}
				}
			}
		} else {
			AfxMessageBox(IDS_ERR_SEL1);								// 無効な場合,ｴﾗｰﾒｯｾｰｼﾞ
		}
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
	}
	CountEdtObj();														// 編集対象立体ｶｳﾝﾄ
	Display(0);															// 再描画
}
