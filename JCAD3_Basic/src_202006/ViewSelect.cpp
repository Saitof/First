/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewSelect.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataFile1.h"
#include "ECommand.h"
#include "RCommand.h"
#include "Select.h"
#include "InputDlg.h"
#include "EditObject.h"
#include "NumFunc.h"

/*******************************************************************/
void CJcad3GlbView::MousePasteObject(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< ﾍﾟｰｽﾄ
/*******************************************************************/
{
	OBJTYPE* op;
	CPCTYPE* cmp;
	PNTTYPE  pnt, crs, mov, vpt;
	VECTYPE  vec;
	BOXTYPE  box;
	int      i, i1, i2;
	double   th;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<透視図の場合>>>
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==5) {															// <<<英字ｷｰ押下>>>
		if(Si1==1) {													// <<ﾄﾞﾗｯｸﾞ中の場合>>
			if(flg=='Z') {												// <10度回転>
				Sd2 += 10.0;
			} else if(flg=='X') {										// <5度回転>
				Sd2 += 5.0;
			} else if(flg=='C') {										// <0.5度回転>
				Sd2 += 0.5;
			} else if(flg=='A') {										// <-10度回転>
				Sd2 -= 10.0;
			} else if(flg=='S') {										// <-5度回転>
				Sd2 -= 5.0;
			} else if(flg=='D') {										// <-0.5度回転>
				Sd2 -= 0.5;
			}
			EditPasteObject2(Si2, Sd2);									// 立体ﾍﾟｰｽZ軸回転
			Display(0);													// 再描画
		}
	} else if(ms==2) {													// <<<ﾏｳｽ移動>>>
		if(flg&MK_LBUTTON) {											// ﾏｳｽ左釦押下中のみ有効
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ﾏｳｽ入力制限座標
			}
			if(!GetObjCrossPnt(plane, pnt, &crs, &vec, &th)) {			// <立体交点なし>
				for(i=0; i<DIM; i++) {
					mov.p[i] = pnt.p[i]-Spnt1.p[i];						// 立体移動量算出
				}
				EditPasteObject1(Si2, mov, 0.0, pnt, Spnt3);			// 立体ﾍﾟｰｽﾄ配置(移動･回転)
			} else {													// <立体交点あり>
				for(i=0; i<DIM; i++) {
					mov.p[i] = crs.p[i]-Spnt2.p[i];						// 立体移動量算出
					vpt.p[i] = vec.p[i];								// 法線ﾍﾞｸﾄﾙ変換
				}
				EditPasteObject1(Si2, mov, th, crs, vpt);				// 立体ﾍﾟｰｽﾄ配置(移動･回転)
			}
			Display(0);													// 再描画
		}
	} else if(ms==1) {													// <<<ﾏｳｽ左釦押下>>>
		CalcScreenWorld(0, &Sbase, point, plane);						// 移動元点の座標取得
		OffEdtAllObject();												// 編集立体全解除
		InitPasteObject(&Si2);											// 立体ﾍﾟｰｽﾄ初期処理
		GetAreaBox(2, &box);											// BOXｻｲｽﾞ取得
		BoxCenter(box, &Spnt1);											// BOXｾﾝﾀｰ取得
		Spnt2 = Spnt1;
		if(plane==0) {
			Spnt2.p[2] = box.bp[0].p[2]-Sd1;							// 指定平面上の
		} else if(plane==1) {											// BOXｾﾝﾀｰ底面点
			Spnt2.p[0] = box.bp[0].p[0]-Sd1;
		} else {
			Spnt2.p[1] = box.bp[0].p[1]-Sd1;
		}
		Spnt3.p[0] = 0.0, Spnt3.p[1] = 0.0, Spnt3.p[2] = -1.0;			// 仮回転軸ﾍﾞｸﾄﾙ
		if(!GetObjCrossPnt(plane, Sbase, &crs, &vec, &th)) {			// <立体交点なし>
			for(i=0; i<DIM; i++) {
				mov.p[i] = Sbase.p[i] - Spnt1.p[i];						// 立体移動量算出
			}
			EditPasteObject1(Si2, mov, 0.0, Sbase, Spnt3);				// 立体ﾍﾟｰｽﾄ配置(移動･回転)
		} else {														// <立体交点あり>
			for(i=0; i<DIM; i++) {
				mov.p[i] = crs.p[i]-Spnt2.p[i];							// 立体移動量算出
				vpt.p[i] = vec.p[i];									// 法線ﾍﾞｸﾄﾙ変換
			}
			EditPasteObject1(Si2, mov, th, crs, vpt);					// 立体ﾍﾟｰｽﾄ配置(移動･回転)
		}
		Display(0);														// 再描画
		Si1 = 1, Sd2 = 0.0;
	} else if(ms==3) {													// <<<ﾏｳｽ左釦解放>>>
		EndPasteObject(Si2);											// 立体ﾍﾟｰｽﾄ終了処理
		Si1 = 0, Sd2 = 0.0;
	} else if(ms==0) {													// <<<初期処理>>>
		if(!m_InitMenu) {
			Si1 = 0, Sd1 = 0.0, Sd2 = 0.0, i = 0;
			StartMenu();												// ﾒﾆｭｰ開始処理
			CheckCtrlKey1();											// 制御ｷｰ確認(起動時)
			FreeCopyCmd();												// ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
			ReadCopyFile(m_Char);										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ入力
			BaseCopyCmdPtr(&cmp);										// 先頭ｺﾋﾟｰｺﾏﾝﾄﾞﾎﾟｲﾝﾀ取得
			if(cmp==NULL) {												// <ｺﾋﾟｰｺﾏﾝﾄﾞなし>
				CountEdtObj();											// 編集対象立体ｶｳﾝﾄ
				ExitMenu(0, 1);											// ﾒﾆｭｰ終了
			} else {													// <ｺﾋﾟｰｺﾏﾝﾄﾞあり>
				BaseObjPtr(2, &op, &i1, &i2);							// 編集対象立体取得
				while(op!=NULL) {
					if(m_FChar&0x1) {									// [F5]:ｼｪｰﾃﾞｨﾝｸﾞ表示
						m_Shading = 2;									// ｼｪﾃﾞｨﾝｸﾞ表示状態
						ObjectShading(op);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					} else {											// 通常表示
						SetSelNo(op, 1);								// 選択No設定
					}
					OffEdtFlg(1, op);									// 編集立体解除
					i++;												// 立体ｶｳﾝﾄ
					NextObjPtr(2, &op, &i1, &i2);						// 次立体取得
				}
				if(i>0&&m_NumInp) {										// <選択立体有で数値入力>
				    CInputDlg dlg(21);
					dlg.SetValue(m_PastePos);							// 既存値表示
					if(dlg.DoModal()==IDOK) {							// 入力ﾀﾞｲｱﾛｸﾞ表示
						dlg.GetValue(&m_PastePos);						// ﾍﾟｰｽﾄ基準位置幅入力
						Sd1 = m_PastePos;
					} else {											// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙ
						m_Shading = 0;									// ｼｪﾃﾞｨﾝｸﾞ表示なし
						SetRenderMode(0);								// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
						ResetSelNo();									// 立体選択Noﾘｾｯﾄ
						CountEdtObj();									// 編集対象立体ｶｳﾝﾄ
						ExitMenu(0, 1);									// ﾒﾆｭｰ終了
					}
				}
			}
		}
		Display(0);														// 再描画
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ｶｰｿﾙNo設定
	} else if(ms==4||ms==9) {											// <<<ﾏｳｽ右釦押下/終了>>>
		if(ms==4&&(m_CChar&0x1||m_CChar&0x2||m_Char!=0)) {				// <右釦ｸﾘｯｸでｷｰ押下中>
			ReadCopyFile(m_Char);										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ入力
		} else {														// <上記以外>
			m_Shading = 0;												// ｼｪﾃﾞｨﾝｸﾞ表示なし
			SetRenderMode(0);											// ﾜｲﾔｰﾌﾚｰﾑﾓｰﾄﾞ
			ResetSelNo();												// 立体選択Noﾘｾｯﾄ
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			Display(0);													// 再描画
			ExitMenu(0, 1);												// ﾒﾆｭｰ終了
			EndJMenu2();												// ﾒﾆｭｰ終了処理
		}
	}
}

/************************************************************/
void CJcad3GlbView::ObjSelect(UINT flg, const CPoint& point,
                              int plane, int ms)						// 立体の選択
/************************************************************/
{
	BOXTYPE	area;
	PNTTYPE	cpnt, pnt;
	int     i, ckey;
	double  d[3], dd, dx, dy, dz;

	if(plane!=XY&&plane!=YZ&&plane!=ZX) {
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// <<ﾏｳｽ左釦押しながらﾄﾞﾗｯｸﾞ>>
		if(Sphase==2) {													// <立体選択の場合>
			CalcScreenWorld(1, &pnt, point, plane);						// 座標点取得
			DrawBox(BaseCursor, pnt, plane);							// BOX表示
			Display(0);													// 再描画
		} else if(Sphase==3||Sphase==4) {								// <立体移動の場合>
			CalcScreenWorld(0, &pnt, point, plane);						// 座標点取得
			for(i=0; i<DIM; i++) {
				d[i] = pnt.p[i]-Sbase.p[i];								// 立体移動量算出
			}
			if(flg&m_LimKey) {
				LimitValue(plane, d);									// ﾏｳｽ入力制限値
			}
			if(Sphase==3) {												// <移動初期処理>
				StartMenu();											// ﾒﾆｭｰ開始処理
			}
			ObjectMove(Sphase-3, m_CopyNum, Spnt1, Sscr1,
                       d[0], d[1], d[2]);								// 立体平行移動
			Sphase = (Sphase==3) ? 4 : Sphase;
			dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
			m_pNumDisp->SetParam(d[0], d[1], d[2], dd);					// ﾏｳｽ移動量表示
			Display(0);													// 再描画
			Sd1 = d[0], Sd2 = d[1], Sd3 = d[2];							// 移動量保存
		}
	} else if(ms==3) {													// <<ﾏｳｽ左釦解放>>
		if(Sphase==2) {													// <立体選択の場合>
			if((flg&m_ObjKey)&&(flg&m_NObKey)) {						// 追加=解除又は両方押下
				ckey = 1;
			} else if(flg&m_ObjKey) {									// 追加押下
				ckey = 2;
			} else if(flg&m_NObKey) {									// 解除押下
				ckey = 3;
			} else {													// 押下無
				ckey = 0;
			}
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &cpnt, point, plane);				// 座標点取得
				area.bp[0] = BaseCursor, area.bp[1] = cpnt;
				EdtAreaObject(plane, area, ckey);
			} else {
				EdtPointObject(plane, BaseCursor, ckey);
			}
			CountEdtObj();
			m_SurfTension = (m_EdtCount==0) ? 0 : m_SurfTension;		// 編集対象立体無の場合
			CheckEdtObjPrm();											// 編集対象立体生成ﾊﾟﾗﾒｰﾀﾁｪｯｸ
		} else if(Sphase==4) {											// <立体移動(M)の場合>
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, Sd1, Sd2, Sd3);		// 平行移動終了処理
			ExitMenu(1, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		} else if(Sphase==1) {											// <立体移動(矢印)の場合>
			m_CopyNum = 0, Si1 = 0;										// 複写数:0/位置モード:モード1
			for(i=0; i<DIM; i++) {
				Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;						// 倍率:1.0倍/基準点:中間
			}
			dx = 0.0, dy = 0.0, dz = 0.0;
			if(m_DChar&0x1) {											// <←ｷｰ押下>
				if(plane==ZX) {
					dx = m_MoveAdd;
				} else {
					dy = -m_MoveAdd;
				}
			} else if(m_DChar&0x2) {									// <↑ｷｰ押下>
				if(plane==XY) {
					dx = -m_MoveAdd;
				} else {
					dz = m_MoveAdd;
				}
			} else if(m_DChar&0x4) {									// <→ｷｰ押下>
				if(plane==ZX) {
					dx = -m_MoveAdd;
				} else {
					dy = m_MoveAdd;
				}
			} else if(m_DChar&0x8) {									// <↓ｷｰ押下>
				if(plane==XY) {
					dx = m_MoveAdd;
				} else {
					dz = -m_MoveAdd;
				}
			}
			StartMenu();												// ﾒﾆｭｰ開始処理
			ObjectMove(0, m_CopyNum, Spnt1, Sscr1, dx, dy, dz);			// 立体平行移動
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, dx, dy, dz);			// 平行移動終了処理
			ExitMenu(1, 1);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
		}
		DeleteBox();													// 矩形破棄
		Display(0);														// 再描画
		Sphase = 0;
	} else if(ms==1) {													// <<ﾏｳｽ左釦押下>>
		CalcScreenWorld(1, &pnt, point, plane);							// 座標点取得
		BaseCursor = pnt;												// 押下位置保存
		CountEdtObj();													// 選択立体数ｶｳﾝﾄ
		if(m_EdtCount>0&&m_Char=='M') {									// <立体移動(M)の場合>
			CalcScreenWorld(0, &Sbase, point, plane);					// 移動元点の座標取得
			Sphase = 3;
			m_CopyNum = 0, Si1 = 0;										// 複写数:0/位置モード:モード1
			for(i=0; i<DIM; i++) {
				Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;						// 倍率:1.0倍/基準点:中間
			}
			SetNumDisp(2);												// 数値表示初期処理
		} else if(m_EdtCount>0&&m_DChar!=0) {							// <立体移動(矢印)の場合>
			Sphase = 1;
		} else {														// <立体選択の場合>
			Sphase = 2;
		}
	} else if(ms==11&&Sphase==0) {										// <<Deleteｷｰ/DEL釦>>
		if(GetSelectNo()!=0) {											// <立体選択No設定済>
			ResetSelNo();												// 立体選択ﾘｾｯﾄ
		} else if(m_EdtCount>0) {										// <編集対象立体有>
			StartMenu();												// ﾒﾆｭｰ開始処理
			InitLevel();												// Undo/Redo初期化
			Delete();													// 立体の削除
			Display(0);
			CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
			ExitMenu(0, 0);												// ﾒﾆｭｰ終了
			EndJMenu1();												// ﾒﾆｭｰ終了処理
			m_SurfTension = (m_EdtCount==0) ? 0 : m_SurfTension;		// 編集対象立体無の場合
			MainWnd->SetGroupObj();										// ｸﾞﾙｰﾌﾟ立体設定
		}
		Display(0);
	}
}

/************************************************************/
void CJcad3GlbView::ObjChoose(UINT flg, const CPoint& point,
                              int plane, int ms)						// 選択立体の部分解除
/************************************************************/
{
	int	dsp, edt;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// 透視図上の場合
		EyeChange(flg, point, plane, ms);								// 視点変更
		return;
	}
	if(ms==0) {
		BaseObjPtr(2, &Sop1, &dsp, &edt);								// 選択立体ﾍﾞｰｽﾎﾟｲﾝﾀ取得
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// 選択No設定
		}
	} else if(ms==3) {
		SetSelNo(Sop1, 0);												// 選択No設定
		NextObjPtr(2, &Sop1, &dsp, &edt);								// 次選択立体ﾎﾟｲﾝﾀ取得
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// 選択No設定
		}
	} else if(ms==4) {
		OffEdtFlg(1, Sop1);												// 編集対象ﾌﾗｸﾞOFF
		SetSelNo(Sop1, 0);												// 選択No設定
		NextObjPtr(2, &Sop1, &dsp, &edt);								// 次選択立体ﾎﾟｲﾝﾀ取得
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// 選択No設定
		}
	} else if(ms==9) {
		Sop1 = NULL;
	}
	if(Sop1==NULL) {
		ExitMenu(0, 0);													// ﾒﾆｭｰ終了
		CountEdtObj();													// 編集対象立体ｶｳﾝﾄ
		ResetSelNo();													// 立体選択ﾘｾｯﾄ
	}
	Display(0);															// 対象立体表示
}
