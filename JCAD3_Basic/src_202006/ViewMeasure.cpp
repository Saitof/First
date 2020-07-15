/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewMeasure.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "NCVL32_Api.h"

/***********************************************************************/
void CJcad3GlbView::MouseMeasureDistance(UINT flg, const CPoint& point,
                                         int plane, int ms)				//<<< 距離測定
/***********************************************************************/
{
	PNTTYPE pnt;
	int     i, i1, i2;
	double  dd[3], ln;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==3) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標値保存
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i1 = plane;													// 有効座標軸設定
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si1].p[i1] = pnt.p[i1], Spa[Si1].p[i2] = pnt.p[i2];		// 座標値変更
			for(i=0; i<DIM; i++) {
				dd[i] = fabs(Spa[1].p[i]-Spa[0].p[i]);					// 誤差取得
			}
			ln = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);				// 距離取得
			Tracker1.Setup(Spa, Sia, NULL, 2, 4, 3);					// ﾄﾗｯｶｰ1設定
			m_pNumDisp->SetParam(dd[0], dd[1], dd[2], ln);				// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Si2 = 1;													// ﾏｳｽ移動済み
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {													// <<1回目>>
			CalcScreenWorld(0, &Spa[0], point, plane);					// 座標値保存
			Spa[1] = Spa[0];
			Si1 = 1;													// 移動対象番号
			Sphase = 3;
			Si2 = 0, Sbase = Spa[Si1];									// ﾏｳｽ移動ﾌﾗｸﾞ初期化
		} else if(Sphase==2) {											// <<2回目以降>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標値保存
			if((Si1=Tracker1.HitTracker(1, plane, pnt, Spa, 2))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sphase = 3, Sbase = Spa[Si1];
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		Sphase = (Sphase==3) ? 2 : Sphase;
		Sphase = (!Si2) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL(取消)>>>
		m_pNumDisp->SetParam(0.0, 0.0, 0.0, 0.0);						// 表示値初期化
		for(i=0; i<DIM; i++) {
			Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0;						// 測定点初期化
		}
		Tracker1.Reset(0);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		Sphase = 1;
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			SetNumDisp(1);												// ﾀﾞｲｱﾛｸﾞ表示
			m_pNumDisp->SetParam(0.0, 0.0, 0.0, 0.0);					// 表示値初期化
			for(i=0; i<DIM; i++) {
				Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0;					// 測定点初期化
			}
			Sia[0] = 4, Sia[1] = 4;										// ﾄﾗｯｶｰ色初期化
			Sphase = 1;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		SetNumDisp(0);													// ﾀﾞｲｱﾛｸﾞ消去
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureAngle(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< 角度測定
/********************************************************************/
{
	PNTTYPE pnt;
	VECTYPE v1, v2;
	int     i, i1, i2;
	double	dd[3], th, ln;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==2||Sphase==4) {
			CalcScreenWorld(0, &pnt, point, plane);						// 座標値保存
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			i1 = plane;													// 有効座標軸設定
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si1].p[i1] = pnt.p[i1];									// 座標値変更
			Spa[Si1].p[i2] = pnt.p[i2];
			if(Sphase==2) {
				for(i=0; i<DIM; i++) {
					dd[i] = fabs(Spa[1].p[i]-Spa[0].p[i]);				// 3軸毎の距離
					v1.p[i] = Spa[1].p[i]-Spa[0].p[i];					// ﾍﾞｸﾄﾙ取得
				}
				ln = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);			// 距離取得
				VecN(&v1);												// 単位ﾍﾞｸﾄﾙ化
				th = atan2(v1.p[i2], v1.p[i1]);							// i1軸からの角度
				th = (th<-10||th>10) ? 0.0 : th;
				th += PI/18.0;											// 10度分加算
				Spa[2] = Spa[0];										// 中心点複写
				Spa[2].p[i1] += ln*cos(th);								// i1軸の座標
				Spa[2].p[i2] += ln*sin(th);								// i2軸の座標
				Si2 = 1;												// ﾏｳｽ移動済み
			}
			for(i=0; i<DIM; i++) {
				v1.p[i] = Spa[1].p[i]-Spa[0].p[i];						// 中心点からの
				v2.p[i] = Spa[2].p[i]-Spa[0].p[i];						// ﾍﾞｸﾄﾙ取得
			}
			VecN(&v1);													// 単位ﾍﾞｸﾄﾙ化
			VecN(&v2);
			if(fabs(v1.p[0]-v2.p[0])>EPSILON||							// 2ﾍﾞｸﾄﾙ間の角度
               fabs(v1.p[1]-v2.p[1])>EPSILON||
               fabs(v1.p[2]-v2.p[2])>EPSILON) {
				th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			th = th/PI*180.0;											// 角度
			Tracker1.Setup(Spa, Sia, NULL, 3, 4, 3);					// ﾄﾗｯｶｰ1設定
			m_pNumDisp->SetParam(th);									// ﾏｳｽ角度表示
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		if(Sphase==1) {													// <<1回目>>
			CalcScreenWorld(0, &Spa[0], point, plane);					// 座標値保存
			Spa[1] = Spa[0], Spa[2] = Spa[0];
			Si1 = 1;													// 移動対象番号
			Sphase = 2;
			Si2 = 0, Sbase = Spa[Si1];									// ﾏｳｽ移動ﾌﾗｸﾞ初期化
		} else if(Sphase==3) {											// <<2回目以降>>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標値保存
			if((Si1=Tracker1.HitTracker(1, plane, pnt, Spa, 3))!=-1) {	// ﾄﾗｯｶｰ1ﾁｪｯｸ
				Sphase = 4, Sbase = Spa[Si1];
			}
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		Sphase = (Sphase==2||Sphase==4) ? 3 : Sphase;
		Sphase = (!Si2) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL(取消)>>>
		m_pNumDisp->SetParam(0.0);										// 表示値初期化
		for(i=0; i<DIM; i++) {
			Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0, Spa[2].p[i] = 0.0;	// 測定点初期化
		}
		Tracker1.Reset(0);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		Sphase = 1;
	} else if(ms==0) {													// <<<初期処理>>>
		if(Sphase==0) {
			SetNumDisp(1);												// ﾀﾞｲｱﾛｸﾞ表示
			m_pNumDisp->SetParam(0.0);									// 表示値初期化
			for(i=0; i<DIM; i++) {
				Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0, Spa[2].p[i] = 0.0;// 測定点初期化
			}
			Sia[0] = 5, Sia[1] = 4, Sia[2] = 4;							// ﾄﾗｯｶｰ色初期化
			Sphase = 1;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		SetNumDisp(0);													// ﾀﾞｲｱﾛｸﾞ消去
		Tracker1.Reset(1);												// ﾄﾗｯｶｰ1解除
		Display(0);														// 再描画
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureCurve(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< 曲線長さ測定
/********************************************************************/
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
						if(GetSurfaceCurve(Si1, Si2, Si3, Si4, Si7, Si8,
                                           m_ObjCRpt, Siy[0], Sop1,
                                           &Si11, &Si12, Spw)) {		// 曲面曲線取得
							for(dd=0.0, i=1; i<Si11; i++) {
								dd += PntD(&Spw[i-1], &Spw[i]);			// 頂点間距離集計
							}
							if(Si12) {									// <曲線(閉)の場合>
								dd += PntD(&Spw[Si11-1], &Spw[0]);		// 終点-始点間距離集計
							}
							m_pNumDisp->SetParam(dd);					// 曲線長さ
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
					if(GetSurfaceCurve(Si1, Si2, Si3, Si4, Si7, Si8,
                                       m_ObjCRpt, Siy[0], Sop1,
                                       &Si11, &Si12, Spw)) {			// 曲面曲線取得
						for(dd=0.0, i=1; i<Si11; i++) {
							dd += PntD(&Spw[i-1], &Spw[i]);				// 頂点間距離集計
						}
						if(Si12) {										// <曲線(閉)の場合>
							dd += PntD(&Spw[Si11-1], &Spw[0]);			// 終点-始点間距離集計
						}
						m_pNumDisp->SetParam(dd);						// 曲線長さ
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
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &Sop1, &i1, &i2);								// 編集対象立体取得
			if(Sop1!=NULL) {											// <編集立体有の場合>
				BaseCmdPtr(1, Sop1, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				i3 = GetObjAtr(Sop1);									// 立体属性取得
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <<通常曲面の場合>>
				InitTRLimit();											// ﾄﾗｯｶｰ範囲限定初期化
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// 曲面ｺﾏﾝﾄﾞ取得1
				StartMenu();											// ﾒﾆｭｰ開始処理
				InitLevel();											// Undo/Redo初期化
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
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// ﾄﾗｯｶｰ1設定
				Sphase = 1;												// 制御点指定へ
				SetNumDisp(1);											// ﾀﾞｲｱﾛｸﾞ表示
				m_pNumDisp->SetParam(0.0);								// 表示値初期化
				Display(0);												// 再描画
			} else if(i3/10==1&&cmp!=NULL) {							// <<曲線の場合>>
				OffEdtOtherObject(Sop1);								// 指定外編集対象解除
				Si1 = GetParaInt(cmp, 0);								// 制御点数取得
				Si2 = GetParaInt(cmp, 1);								// 境界ﾓｰﾄﾞ取得
				Si3 = GetVtxNum(Sop1);									// 立体頂点数取得
				for(i=0; i<Si3; i++) {
					GetVtx(Sop1, i, &Spw[i]);							// 指定頂点取得
				}
				for(dd=0.0, i=1; i<Si3; i++) {
					dd += PntD(&Spw[i-1], &Spw[i]);						// 頂点間距離集計
				}
				if(Si2) {												// <曲線(閉)の場合>
					dd += PntD(&Spw[Si3-1], &Spw[0]);					// 終点-始点間距離集計
				}
				SetNumDisp(1);											// ﾀﾞｲｱﾛｸﾞ表示
				m_pNumDisp->SetParam(dd);								// 表示値初期化
				Display(0);												// 再描画
			} else {													// <<編集立体:NG>>
				AfxMessageBox(IDS_ERR_SEL5);							// ｴﾗｰﾒｯｾｰｼﾞ
				ExitMenu(0, 0);											// ﾒﾆｭｰ終了
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(Sphase==1) {													// <曲面指定の場合>
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// 立体削除
			}
			Tracker1.Reset(1);											// ﾄﾗｯｶｰ1解除
			Tracker2.Reset(1);											// ﾄﾗｯｶｰ2解除
			Line.Reset();												// ﾗｲﾝ解除
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			OnUndoRedo();												// Undo/Redo可
			InitLineNo(NULL);											// ﾗｲﾝ番号初期化
			Undo(gno);													// 編集なしはUndo
		}
		SetNumDisp(0);													// ﾀﾞｲｱﾛｸﾞ消去
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		Display(0);														// 再描画
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureScale(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< 定規
/********************************************************************/
{
	int    i1, i2;
	double d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Sbase, point, plane);					// 座標値保存
			if(flg&m_LimKey) {
				LimitPoint(plane, &Sbase, &BaseCursor);					// ﾏｳｽ入力制限座標
			}
			DrawBox(BaseCursor, Sbase, plane);							// BOX表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		i1 = plane;														// 有効座標軸取得
		i2 = (plane+1>2) ? 0 : plane+1;
		d1 = fabs(Sbase.p[i1]-BaseCursor.p[i1]);						// 座標軸方向の長さ
		d2 = fabs(Sbase.p[i2]-BaseCursor.p[i2]);
		if(d1>=1.0&&d2>=1.0) {											// <有効なｻｲｽﾞ>
			m_ScalePlane = plane;										// 表示平面登録
			MeasureSCALE(BaseCursor, Sbase);							// 定規登録
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<初期処理>>>
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==8) {													// <<<他ﾒﾆｭｰ起動>>>
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		DeleteSCALE();													// 定規廃棄
		Display(0);														// 再描画
		m_ActMenu = m_SaveMenu;											// 起動中ﾒﾆｭｰへ戻す
		m_SaveMenu = NONE;												// 保存ﾒﾆｭｰ番号ｸﾘｱ
		m_CursorNo = m_CursorNo_S;										// ｶｰｿﾙNo復元
		m_CursorNo_S = 0;												// 保存ｶｰｿﾙNoｸﾘｱ
		InitChar();														// ｷｰ入力初期化
	}
}

/*******************************************************************/
void CJcad3GlbView::MouseMeasureProt(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< 分度器
/*******************************************************************/
{
	int    i1, i2;
	double ln, l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==2) {															// <<<ﾏｳｽ移動>>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Sbase, point, plane);					// 座標値保存(直径終点)
			if(flg&m_LimKey) {
				LimitPoint(plane, &Sbase, &BaseCursor);					// ﾏｳｽ入力制限座標
			}
			DrawCircle(BaseCursor, Sbase, plane);						// 円表示
			Display(0);													// 再描画
		}
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		i1 = plane;														// 有効座標軸取得
        i2 = (plane+1>2) ? 0 : plane+1;
		l1 = Sbase.p[i1]-BaseCursor.p[i1];								// 座標軸方向の長さ
		l2 = Sbase.p[i2]-BaseCursor.p[i2];
		ln = sqrt(l1*l1+l2*l2);											// 直線の長さ
		if(ln>=2.0) {													// <有効なｻｲｽﾞ>
			m_ProtPlane = plane;										// 表示平面登録
			MeasurePROT(BaseCursor, Sbase);								// 分度器登録
		}
		DeleteCircle();													// 円破棄
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<初期処理>>>
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==8) {													// <<<他ﾒﾆｭｰ起動>>>
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		DeletePROT();													// 分度器廃棄
		Display(0);														// 再描画
		m_ActMenu = m_SaveMenu;											// 起動中ﾒﾆｭｰへ戻す
		m_SaveMenu = NONE;												// 保存ﾒﾆｭｰ番号ｸﾘｱ
		m_CursorNo = m_CursorNo_S;										// ｶｰｿﾙNo復元
		m_CursorNo_S = 0;												// 保存ｶｰｿﾙNoｸﾘｱ
		InitChar();														// ｷｰ入力初期化
	}
}

/*******************************************/
void CJcad3GlbView::MenuMeasureVolume(void)								//<<< 体積測定
/*******************************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      ret, i1, i2;

	BaseObjPtr(2, &op, &i1, &i2);										// 表示立体ﾎﾟｲﾝﾀ取得
	if(op==NULL) {														// <立体無は中止>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// ﾃﾞｰﾀﾍﾞｰｽ先頭ﾎﾟｲﾝﾀ取得
	ret = VolumeObject(op, vtx, flp);									// 立体体積測定
	if(ret==-2) {														// 中止
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(ret==2) {													// ﾒﾓﾘｴﾗｰでｼｽﾃﾑ停止
		MemErr(IDS_MEMERR5);
	}
}
