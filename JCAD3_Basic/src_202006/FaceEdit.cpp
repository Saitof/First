/////////////////////////////////////////////////////////////////////////////
// 基準面編集 演算部
// FaceEdit.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "EditCtrlPoint1.h"
#include "CreatCurvePoint.h"
#include "NumFunc.h"
#include "Select.h"
#include "FaceEditType.h"

/******************************/
static void InitWorkArea(void)											//<<< 基準面編集初期処理
/******************************/
{
	Flist = NULL, Fcnt = 0;
}

/*******************************************************/
static void SetFaeData(double x1, double x2, double y1,
                       double y2, VECTYPE vc, int st)					//<<< 面情報ﾃﾞｰﾀ登録
/*******************************************************/
{
	FAETYPE* tf;

	tf = (FAETYPE *)malloc(sizeof(FAETYPE));							// 面情報領域確保
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->xmin = x1, tf->xmax = x2;
	tf->ymin = y1, tf->ymax = y2;
	tf->vec = vc,  tf->stfl = st;
	tf->next = Flist, Flist = tf, Fcnt++;
}

/************************************************/
static FAETYPE* MergeF(FAETYPE* la, FAETYPE* lb)						//<<< X最小値ｿｰﾄ
/************************************************/
{
	FAETYPE* lc, * ld, * rc;

	lc = (FAETYPE *)malloc(sizeof(FAETYPE));							// 作業領域確保
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->xmin<=lb->xmin) {										// X最小値の大小で比較
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// 作業領域解放
	return rc;
}

/************************************************/
static FAETYPE* F_Mergsrt(FAETYPE* mlist, int n)						//<<< X最小値のｿｰﾄ
/************************************************/
{
	FAETYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm=(FAETYPE *)MergeF(F_Mergsrt(la, n/2), F_Mergsrt(lb, n-n/2));	// 面ﾙｰﾌﾟ番号ｿｰﾄ
		rc = lm;
	}
	return rc;
}

/*************************************/
static void MakeFaceList(OBJTYPE* op)									//<<< 面情報ﾘｽﾄ作成
/*************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  pt;
	VECTYPE  vc;
	int      i, num, ffg, st, pn;
	double   minx, miny, maxx, maxy;

	num = GetFlpNum1(op);												// 面ﾙｰﾌﾟ数取得
	for(i=0, st=-1; i<num; i++) {
		GetFlpVtx1(op, i, &pt, &ffg);									// 面ﾙｰﾌﾟ頂点取得
		if(st==-1) {
			pn = 0, st = i;												// 面ﾙｰﾌﾟ開始処理
			minx=pt.p[0], maxx=pt.p[0], miny=pt.p[1], maxy=pt.p[1];		// XY最小最大初期値
		}
		pa[pn++] = pt;													// 頂点座標保存
		if(pt.p[0]<minx) {												// XY最小最大取得
			minx = pt.p[0];
		}
		if(pt.p[0]>maxx) {
			maxx = pt.p[0];
		}
		if(pt.p[1]<miny) {
			miny = pt.p[1];
		}
		if(pt.p[1]>maxy) {
			maxy = pt.p[1];
		}
		if(ffg) {														// <<面ﾙｰﾌﾟ終了>>
			if(minx<=XPos2&&maxx>=XPos1&&miny<=YPos2&&maxy>=YPos1) {	// <作業領域内の面>
				CalcNVec(pa, pn, &vc);									// 法線ﾍﾞｸﾄﾙ取得
				if(vc.p[2]>EPSILON) {									// <上向きの面>
					SetFaeData(minx, maxx, miny, maxy, vc, st);			// 面情報ﾃﾞｰﾀ登録
				}
			}
			st = -1;													// ﾙｰﾌﾟ開始ﾌﾗｸﾞ初期化
		}
	}
	if(Flist!=NULL) {
		Flist = F_Mergsrt(Flist, Fcnt);									// X最小値のｿｰﾄ
	}
	delete[] pa;
}

/************************************************************************/
static BOOL CheckInFace(PNTTYPE pt, FAETYPE* fp, OBJTYPE* op, double* z)//<<< 面内存在ﾁｪｯｸ
/************************************************************************/
{
	PNTTYPE p1, p2, sp, ep;
	int     ix, f1, f2, chx, chy, ucx, dcx, ucy, dcy, ret=FALSE;
	double  d1, d2, sx, sy, ex, ey;

	chx = 0, ucx = 0, dcx = 0, chy = 0, ucy = 0, dcy = 0;
	ix = fp->stfl;														// 面ﾙｰﾌﾟ開始番号
	while(TRUE) {
		GetFlpVtx1(op, ix, &p1, &f1);									// 面ﾙｰﾌﾟ頂点取得
		if(!f1) {
			GetFlpVtx1(op, ix+1, &p2, &f2);								// 次ﾙｰﾌﾟ頂点取得
		} else {
			GetFlpVtx1(op, fp->stfl, &p2, &f2);
		}
		if(p1.p[0]<p2.p[0]) {											// 小さい方を始点
			sp = p1, ep = p2;
		} else {
			sp = p2, ep = p1;
		}
		if(sp.p[0]<=pt.p[0]&&pt.p[0]<ep.p[0]) {							// <指定点Xが線分Xに含>
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			d1 = ey-sy,   d2 = ex-sx;
			d1 = d1/d2;													// 傾き
			d2 = sy-d1*sx;												// 定数
			d2 = d1*pt.p[0]+d2;											// Y値計算
			if(d2-pt.p[1]>EPSILON) {									// 線分上点が指定点より大
				ucx++;
			} else if(pt.p[1]-d2>EPSILON) {								// 線分上点が指定点より小
				dcx++;
			} else {													// 接する場合
				chx = 1; break;
			}
		}
		if(p1.p[1]<p2.p[1]) {											// 小さい方を始点
			sp = p1, ep = p2;
		} else {
			sp = p2, ep = p1;
		}
		if(sp.p[1]<=pt.p[1]&&pt.p[1]<ep.p[1]) {							// <指定点Yが線分Yに含>
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			d1 = ex-sx,   d2 = ey-sy;
			d1 = d1/d2;													// 傾き
			d2 = sx-d1*sy;												// 定数
			d2 = d1*pt.p[1]+d2;											// X値計算
			if(d2-pt.p[0]>EPSILON) {									// 線分上点が指定点より大
				ucy++;
			} else if(pt.p[0]-d2>EPSILON) {								// 線分上点が指定点より小
				dcy++;
			} else {													// 接する場合
				chy = 1; break;
			}
		}
		if((fabs(pt.p[0]-p1.p[0])<EPSILON&&
            fabs(pt.p[1]-p1.p[1])<EPSILON)||
           (fabs(pt.p[0]-p2.p[0])<EPSILON&&
            fabs(pt.p[1]-p2.p[1])<EPSILON)) {							// 始点か終点が一致する場合
			chx = 1; break;
		}
		if(f1) {														// 面ﾙｰﾌﾟ終了=>処理終了
			break;
		}
		ix++;
	}
	ucx = ucx%2, dcx = dcx%2;											// 2個1ｾｯﾄ
	ucy = ucy%2, dcy = dcy%2;
	if(((ucx&&dcx)&&(ucy&&ucy))||chx||chy) {							// <面の中に存在>
		GetFlpVtx1(op, fp->stfl, &p1, &f1);								// 面ﾙｰﾌﾟ頂点取得
		*z = (fp->vec.p[0]*p1.p[0]+fp->vec.p[1]*p1.p[1]+
              fp->vec.p[2]*p1.p[2]-fp->vec.p[0]*pt.p[0]-
              fp->vec.p[1]*pt.p[1])/fp->vec.p[2];						// 指定点(XY)位置面上のZ
		ret = TRUE;
	} else {
		*z = 0.0;
	}
	return ret;
}

/******************************/
static void FreeWorkArea(void)											//<<< 作業領域解放
/******************************/
{
	FAETYPE* wfp, * dfp;

	wfp = Flist;
	while(wfp) {														// 面情報領域解放
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
}

/****************************/
void ObjectSurfaceEdit(void)											//<<< 曲面指定移動
/****************************/
{
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxSCtA];
	OBJTYPE* op, * op2, * bop;
	CMDTYPE* cmp;
	PNTTYPE  pnt, pvc;
	VECTYPE  vec, rvc;
	BOXTYPE  box;
	FAETYPE* wfa;
	CString  str;
	int      i, f, cnt=0, ith, imv;
	double   inp=0.0, th, z, zmax, zmin=DBL_MAX;

	View->CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
	if(View->GetNumInp()!=0) {											// <数値入力あり>
		inp = View->GetBaseZPos();										// 基準面Z位置取得
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 基準面Z位置入力
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// 基準面Z位置設定
		} else {
			delete[] cpt;
			return;														// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙは中止
		}
	}
	SelBaseObjPtr(1, &bop);												// 基準曲面立体取得
	if(bop==NULL) {														// 基準曲面なしは中止
		delete[] cpt;
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	SelBaseObjPtr(2, &op);												// 編集対象立体取得
	while(op!=NULL) {
		if(cnt>=View->m_MaxSCtA) {										// <最大配列以上>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// ｴﾗｰﾒｯｾｰｼﾞ
			AfxMessageBox(str);
			cnt = -1; break;
		}
		if(op->part==2) {												// <ﾕﾆｯﾄ･子の場合>
			op2 = op->oset;												// ﾕﾆｯﾄ･親OPを使用
			if(op2==NULL) {												// ﾕﾆｯﾄ･親OPがNULLは自OP
				op2 = op;
			} else if(op2->edtflg==0) {									// ﾕﾆｯﾄ･親OPが非対象は自OP
				op2 = op;
			}
		} else {														// <ﾕﾆｯﾄ･子以外の場合>
			op2 = op;
		}
		GetObjBox(op2, &box);											// 立体BOXｻｲｽﾞ取得
		BoxCenter(box, &pnt);											// BOXｾﾝﾀｰ取得
		cpt[cnt++] = pnt;												// 中心点保存
		if(pnt.p[0]<XPos1) {											// 最小最大取得
			XPos1 = pnt.p[0];
		}
		if(pnt.p[0]>XPos2) {
			XPos2 = pnt.p[0];
		}
		if(pnt.p[1]<YPos1) {
			YPos1 = pnt.p[1];
		}
		if(pnt.p[1]>YPos2) {
			YPos2 = pnt.p[1];
		}
		if(box.bp[0].p[2]<zmin) {
			zmin = box.bp[0].p[2];
		}
		SelNextObjPtr(2, &op);											// 次編集対象立体取得
	}
	if(cnt<0) {															// 最大配列以上は中止
		delete[] cpt;
		return;
	}
	zmin -= inp;														// Z位置入力値を加算
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	InitWorkArea();														// 基準面編集初期処理
	MakeFaceList(bop);													// 立体面ﾘｽﾄ作成
	if(Flist!=NULL) {													// <面ﾘｽﾄ有の場合>
		cnt = 0;
		SelBaseObjPtr(2, &op);											// 編集対象立体取得
		while(op!=NULL) {
			pnt = cpt[cnt++], pnt.p[2] = zmin;							// BOXｾﾝﾀｰ取得
			wfa = Flist;												// 先頭面取得
			zmax = 1.0-DBL_MAX, f = 0;
			while(wfa!=NULL) {
				if(CheckInFace(pnt, wfa, bop, &z)) {					// <面内存在の場合>
					if(fabs(z-zmax)<0.01) {								// <Z最大に近似する面>
						for(i=0; i<DIM; i++) {
							vec.p[i] += wfa->vec.p[i];					// 法線ﾍﾞｸﾄﾙ加算
						}
					} else if(z>zmax) {									// <Z最大の面>
						zmax = z, vec = wfa->vec, f = 1;				// Z値/法線ﾍﾞｸﾄﾙ保存
					}
				}
				wfa = wfa->next;										// 次面取得
			}
			if(!f) {													// <面内存在なし>
				zmax = zmin;											// 移動なし
				vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;			// 回転なし
			} else {													// 面内存在は単位ﾍﾞｸﾄﾙ化
				VecN(&vec);
			}
			if(fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON) {		// 垂直方向回転角度算出
				th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
				th = atan2(th, vec.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			ith = (int)((180.0*th/PI)*1000.0);							// 垂直方向回転角度(整数)
			if(ith!=0) {												// <垂直方向回転あり>
				rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
				VecN(&rvc);												// 単位ﾍﾞｸﾄﾙ化
				for(i=0; i<DIM; i++) {
					pvc.p[i] = rvc.p[i];
				}
				CreatCmd(ROTATE, 1, 2, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				PutParaInt(cmp, 0, ith);								// 回転角度設定
				PutParaPnt(cmp, 0, pnt);								// 回転軸座標設定
				PutParaPnt(cmp, 1, pvc);								// 回転軸方向ﾍﾞｸﾄﾙ設定
				SetObjPtr(op);											// 対象立体設定
				SetCmdPtr(cmp);											// ｺﾏﾝﾄﾞ設定
				ExecCommand(FALSE);										// 回転ｺﾏﾝﾄﾞ実行
			}
			imv = (int)((zmax-zmin)*1000.0);							// Z方向移動量(整数)
			if(imv!=0) {												// <Z方向移動あり>
				CreatCmd(MOVE, 3, 0, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				PutParaInt(cmp, 0, 0);									// 移動量設定
				PutParaInt(cmp, 1, 0);
				PutParaInt(cmp, 2, imv);
				SetObjPtr(op);											// 対象立体設定
				SetCmdPtr(cmp);											// ｺﾏﾝﾄﾞ設定
				ExecCommand(FALSE);										// 移動ｺﾏﾝﾄﾞ実行
			}
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			SelNextObjPtr(2, &op);										// 次編集対象立体取得
		}
	}
	FreeWorkArea();														// 作業領域解放
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	delete[] cpt;
}

/**********************************************************************/
void ObjectAdjustCP(int on, int xn, PNTTYPE* px, int* ix, PNTTYPE* pa,
					PNTTYPE* pb, PNTTYPE* pc, PNTTYPE* pd,
					PNTTYPE* pw, int* ia, int* ib, int* ic)				//<<< 制御点立体合わせ
/**********************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	FAETYPE* wfa;
	PNTTYPE  zer;
	int      i, f, i1, i2, un, vn, ub, vb, an, rp, cnt=0;
	double   wd[10], wt, inp=0.0, z, zmax;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	View->CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
	if(View->GetNumInp()!=0) {											// <数値入力あり>
		inp = View->GetBaseZPos();										// 基準面Z位置取得
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 基準面Z位置入力
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// 基準面Z位置設定
		} else {
			return;														// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙは中止
		}
	}
	SelBaseObjPtr(1, &op);												// 基準曲面立体取得
	if(op==NULL) {														// 基準曲面なしは中止
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	for(i=0; i<xn; i++) {
		if(ix[i]>0) {													// <選択制御点の場合>
			if(px[i].p[0]<XPos1) {										// 最小最大取得
				XPos1 = px[i].p[0];
			}
			if(px[i].p[0]>XPos2) {
				XPos2 = px[i].p[0];
			}
			if(px[i].p[1]<YPos1) {
				YPos1 = px[i].p[1];
			}
			if(px[i].p[1]>YPos2) {
				YPos2 = px[i].p[1];
			}
		}
	}
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	InitWorkArea();														// 基準面編集初期処理
	MakeFaceList(op);													// 立体面ﾘｽﾄ作成
	if(Flist!=NULL) {													// <<<面ﾘｽﾄ有の場合>>>
		for(i=0; i<xn; i++) {
			if(ix[i]>0) {												// <<選択制御点の場合>>
				wfa = Flist;											// 先頭面取得
				zmax = 1.0-DBL_MAX, f = 0;
				while(wfa!=NULL) {
					if(CheckInFace(px[i], wfa, op, &z)) {				// <面内存在の場合>
						if(z>zmax) {
							zmax = z, f = 1;							// Z最大を検索
						}
					}
					wfa = wfa->next;									// 次面取得
				}
				if(f) {													// <面内存在あり>
					px[i].p[2] = zmax+inp;								// Z値は面上+補正値
				}
			}
		}
	}
	FreeWorkArea();														// 作業領域解放
	BaseObjPtr(2, &op, &i1, &i2);										// 編集対象立体取得
	while(op!=NULL) {													// <<編集立体有の場合>>
		if(GetSelNo(op)!=1) {											// 基準曲線以外登録
			SetSelNo(op, 2);
		}
		NextObjPtr(2, &op, &i1, &i2);									// 次編集立体へ
	}
	OffEdtAllObject();													// 編集立体全解除
	SelBaseObjPtr(2, &op);												// 編集対象立体取得
	while(op!=NULL) {													// <<編集立体有の場合>>
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		i1 = GetObjAtr(op);												// 立体属性取得
		if((i1==10||i1==11)&&cmp!=NULL) {								// <曲線の場合>
			vn = GetParaInt(cmp, 0);									// 制御点数取得
			vb = GetParaInt(cmp, 1);									// 境界ﾓｰﾄﾞ取得
			rp = GetParaInt(cmp, 2);									// 曲線分割数取得
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// 曲線重み係数取得
			for(i=0; i<vn; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// 制御点取得
				ia[i] = GetParaInt(cmp, 4+i);							// ﾗｲﾝﾓｰﾄﾞ取得
			}
			SetCurve(vn, vb, pa);										// 曲線制御点生成
			GetObjPtr(&op1);											// 制御点OP保存
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 曲面編集後ｺﾏﾝﾄﾞ
			while(cmp!=NULL) {
				RedoCommandCopy(op1, cmp);								// ｺﾏﾝﾄﾞ複写実行
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			for(i=0; i<vn; i++) {
				GetVtx(op1, i, &pb[i]);									// 立体編集後制御点取得
				if(ix[cnt+i]==4) {										// <選択済制御点>
					ib[i] = 1;											// 指示あり
					pc[i] = zer;										// Z移動あり
					pc[i].p[2] = px[cnt+i].p[2]-pb[i].p[2];
				} else {												// <未選択制御点>
					ib[i] = 0;											// 指示なし
					pc[i] = zer;										// 移動なし
				}
			}
			FreeObj(op1, 1);											// 編集後立体削除
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<vn; i++) {
					ic[i] = 1;											// 張力有は全範囲設定
				}
			}
			OnEdtFlg(1, op);											// 編集対象立体登録
			for(i=0; i<vn; i++) {
				pw[i*2] = pb[i];										// 制御点を曲線編集用に変更
			}
			MoveECurve(0, wd, pc, op, vn, vb, pa, pw, ib, pd, ic);		// 制御点移動
			ChgCurve(NULL, vn, vb, rp, wt, pd, ia);						// 曲線変更
			OffEdtFlg(1, op);											// 編集対象立体解除
			cnt += vn;													// 制御点数合計
		} else if((i1==20||i1==21)&&cmp!=NULL) {						// <通常曲面の場合>
			un = GetParaInt(cmp, 0);									// U制御点数取得
			vn = GetParaInt(cmp, 1);									// V制御点数取得
			ub = GetParaInt(cmp, 2);									// U境界ﾓｰﾄﾞ取得
			vb = GetParaInt(cmp, 3);									// V境界ﾓｰﾄﾞ取得
			rp = GetParaInt(cmp, 4);									// 曲線分割数取得
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// 曲線重み係数取得
			an = un*vn;													// 制御点数
			for(i=0; i<an; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// 制御点取得
				ia[i] = GetParaInt(cmp, 7+i);							// ﾗｲﾝﾓｰﾄﾞ取得
			}
			SetSurface(un, vn, ub, vb, pa);								// 曲面制御点生成
			GetObjPtr(&op1);											// 制御点OP保存
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 曲面編集後ｺﾏﾝﾄﾞ
			while(cmp!=NULL) {
				if(GetCmdNo(cmp)!=THICKNESS) {							// <厚みづけ以外の場合>
					RedoCommandCopy(op1, cmp);							// ｺﾏﾝﾄﾞ複写実行
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			for(i=0; i<an; i++) {
				GetVtx(op1, i, &pb[i]);									// 立体編集後制御点取得
				if(ix[cnt+i]==4) {										// <選択済制御点>
					ib[i] = 1;											// 指示あり
					pc[i] = zer;										// Z移動あり
					pc[i].p[2] = px[cnt+i].p[2]-pb[i].p[2];
				} else {												// <未選択制御点>
					ib[i] = 0;											// 指示なし
					pc[i] = zer;										// 移動なし
				}
			}
			FreeObj(op1, 1);											// 編集後立体削除
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<an; i++) {
					ic[i] = 1;											// 張力有は全範囲設定
				}
			}
			OnEdtFlg(1, op);											// 編集対象立体登録
			MoveCPoint(0, pc, op, un, vn, ub, vb, wt,
                       ia, ib, pa, pb, pd, ic);							// 制御点移動
			ChgSurface(NULL, un, vn, ub, vb, rp, wt, pd, ia);			// 曲面変更
			OffEdtFlg(1, op);											// 編集対象立体解除
			cnt += an;													// 制御点数合計
		}
		SelNextObjPtr(2, &op);											// 次編集立体へ
	}
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************/
void ObjectAdjust(void)													//<<< 曲面立体合わせ
/***********************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	int*     ia = new int[View->m_MaxSCtA];
	OBJTYPE* op, * bop;
	CMDTYPE* cmp;
	PNTTYPE  pt1, pt2;
	VECTYPE  vec, rvc;
	FAETYPE* wfa;
	CString  str;
	int      i, j, f, un, vn, ub, vb, rp, an;
	double   rot[3][3], wt, inp=0.0, th, z, zmax, zmin=DBL_MAX;

	View->CheckCtrlKey1();												// 制御ｷｰ確認(起動時)
	if(View->GetNumInp()!=0) {											// <数値入力あり>
		inp = View->GetBaseZPos();										// 基準面Z位置取得
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// 既存値表示
		if(dlg.DoModal()==IDOK) {										// 基準面Z位置入力
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// 基準面Z位置設定
		} else {
			delete[] pa;
			delete[] ia;
			return;														// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙは中止
		}
	}
	SelBaseObjPtr(1, &bop);												// 基準曲面立体取得
	if(bop==NULL) {														// 基準曲面なしは中止
		delete[] pa;
		delete[] ia;
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	SelBaseObjPtr(2, &op);												// 編集対象立体取得
	while(op!=NULL) {
		i = GetObjAtr(op);												// 立体属性取得
		BaseCmdPtr(1, op, &cmp);										// 有効先頭ｺﾏﾝﾄﾞ取得
		if((i==20||i==21)&&cmp!=NULL) {									// <通常曲面の場合>
			un = GetParaInt(cmp, 0);									// U制御点数取得
			vn = GetParaInt(cmp, 1);									// V制御点数取得
			an = un*vn;													// 制御点数
			for(i=0; i<an; i++) {
				GetParaPnt(cmp, i, &pt1);								// 制御点取得
				if(pt1.p[0]<XPos1) {									// 最小最大取得
					XPos1 = pt1.p[0];
				}
				if(pt1.p[0]>XPos2) {
					XPos2 = pt1.p[0];
				}
				if(pt1.p[1]<YPos1) {
					YPos1 = pt1.p[1];
				}
				if(pt1.p[1]>YPos2) {
					YPos2 = pt1.p[1];
				}
				if(pt1.p[2]<zmin) {
					zmin = pt1.p[2];
				}
			}
		} else {														// 通常曲面以外は中止
			delete[] pa;
			delete[] ia;
			return;
		}
		SelNextObjPtr(2, &op);											// 次編集対象立体取得
	}
	zmin -= inp;														// Z位置入力値を加算
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	InitWorkArea();														// 基準面編集初期処理
	MakeFaceList(bop);													// 立体面ﾘｽﾄ作成
	if(Flist!=NULL) {													// <面ﾘｽﾄ有の場合>
		OffEdtAllObject();												// 編集立体全解除
		SelBaseObjPtr(2, &op);											// 編集対象立体取得
		while(op!=NULL) {
			i = GetObjAtr(op);											// 立体属性取得
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			if((i==20||i==21)&&cmp!=NULL) {								// <通常曲面の場合>
				un = GetParaInt(cmp, 0);								// U制御点数取得
				vn = GetParaInt(cmp, 1);								// V制御点数取得
				ub = GetParaInt(cmp, 2);								// U境界ﾓｰﾄﾞ取得
				vb = GetParaInt(cmp, 3);								// V境界ﾓｰﾄﾞ取得
				rp = GetParaInt(cmp, 4);								// 曲線分割数取得
				wt = (double)GetParaInt(cmp, 5)/1000.0;					// 曲線重み係数取得
				an = un*vn;												// 制御点数
				for(i=0; i<an; i++) {
					GetParaPnt(cmp, i, &pt1);							// 制御点取得
					wfa = Flist;										// 先頭面取得
					zmax = 1.0-DBL_MAX, f = 0;
					while(wfa!=NULL) {
						if(CheckInFace(pt1, wfa, bop, &z)) {			// <面内存在の場合>
							if(fabs(z-zmax)<0.01) {						// <Z最大に近似する面>
								for(j=0; j<DIM; j++) {
									vec.p[j] += wfa->vec.p[j];			// 法線ﾍﾞｸﾄﾙ加算
								}
							} else if(z>zmax) {							// <Z最大の面>
								zmax = z, vec = wfa->vec, f = 1;		// Z値/法線ﾍﾞｸﾄﾙ保存
							}
						}
						wfa = wfa->next;								// 次面取得
					}
					if(!f) {											// <面内存在なし>
						zmax = zmin;									// 移動なし
						vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;	// 回転なし
					} else {											// 面内存在は単位ﾍﾞｸﾄﾙ化
						VecN(&vec);
					}
					pt2.p[0]=0.0, pt2.p[1]=0.0, pt2.p[2]=pt1.p[2]-zmin;	// 回転前基準点との変位量
					pt1.p[2] = zmax;									// 回転後基準点
					if(fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON) {// 垂直方向回転角度算出
						th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
						th = atan2(th, vec.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
					} else {
						th = 0.0;
					}
					rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
					VecN(&rvc);											// 単位ﾍﾞｸﾄﾙ化
					SetRotateMatrix(rvc, th, rot);						// 回転ﾏﾄﾘｯｸｽ作成
					VecRotateMove(&pt2, rot, pt1);						// 回転移動
					pa[i] = pt2;
					ia[i] = GetParaInt(cmp, 7+i);						// ﾗｲﾝﾓｰﾄﾞ取得
				}
				OnEdtFlg(1, op);										// 編集対象立体登録
				ChgSurface(NULL, un, vn, ub, vb, rp, wt, pa, ia);		// 曲面変更
				OffEdtFlg(1, op);										// 編集対象立体解除
			}
			SelNextObjPtr(2, &op);										// 次編集対象立体取得
		}
	}
	FreeWorkArea();														// 作業領域解放
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	delete[] pa;
	delete[] ia;
}
