/////////////////////////////////////////////////////////////////////////////
// 曲面編集操作部
// EditSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "CreatCurvePoint.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/********************************************************************/
void GetSurfCmd1(CMDTYPE* cmp, int* un, int* vn, int* ub, int* vb,
                 int* rp, double* wt, int* an, PNTTYPE* pa, int* ia)	//<<< 曲面ｺﾏﾝﾄﾞ取得1
/********************************************************************/
{
	int i;

	*un = GetParaInt(cmp, 0);											// U制御点数取得
	*vn = GetParaInt(cmp, 1);											// V制御点数取得
	*ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	*vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	*rp = GetParaInt(cmp, 4);											// 曲線分割数取得
	*wt = (double)GetParaInt(cmp, 5)/1000.0;							// 曲線重み係数取得
	*an = (*un)*(*vn);													// 総制御点数
	for(i=0; i<(*an); i++) {
		GetParaPnt(cmp, i, &pa[i]);										// 制御点取得
		ia[i] = GetParaInt(cmp, 7+i);									// ﾗｲﾝﾓｰﾄﾞ取得
	}
}

/*********************************************************************/
void GetSurfCmd2(CMDTYPE* cmp, int atr, int* md, int* un, int* vn,
                 int* ub, int* vb, int* rp, double* wt, int* an,
                 PNTTYPE* pa, int* ia, int* pl, PNTTYPE* pb, int* ib)	//<<< 曲面ｺﾏﾝﾄﾞ取得2
/*********************************************************************/
{
	int i;

	*md = (atr==20||atr==21) ? 1 : 0;									// 曲面/掃引曲面
	*un = GetParaInt(cmp, 0);											// U制御点数取得
	*vn = GetParaInt(cmp, 1);											// V制御点数取得
	*ub = GetParaInt(cmp, 2);											// U境界ﾓｰﾄﾞ取得
	*vb = GetParaInt(cmp, 3);											// V境界ﾓｰﾄﾞ取得
	*rp = GetParaInt(cmp, 4);											// 曲線分割数取得
	*wt = (double)GetParaInt(cmp, 5)/1000.0;							// 曲線重み係数取得
	*an = (*un)*(*vn);													// 総制御点数
	if(*md) {															// <通常曲面>
		for(i=0; i<(*an); i++) {
			GetParaPnt(cmp, i, &pa[i]);									// 制御点取得
			ia[i] = GetParaInt(cmp, 7+i);								// ﾗｲﾝﾓｰﾄﾞ取得
		}
	} else {															// <掃引曲面>
		*pl = GetParaInt(cmp, 7);										// 掃引平面取得
		for(i=0; i<(*un)+(*vn); i++) {
			GetParaPnt(cmp, i, &pb[i]);									// 制御点取得
			ib[i] = GetParaInt(cmp, 8+i);								// ﾗｲﾝﾓｰﾄﾞ取得
		}
	} 
}

/****************************************************************/
void ExecSurface(int md, OBJTYPE* op1, int un, int vn, int ub,
                 int vb, int rp, double wt, int an, PNTTYPE* pa,
                 int* ia, PNTTYPE* pb, OBJTYPE** op2)					//<<< 通常曲面実行
/****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	ChgSurface(NULL, un, vn, ub, vb, rp, wt, pa, ia);					// 曲面変更(初回)
	CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);			// 制御点=>通過点変換
	SetSurface(un, vn, ub, vb, pb);										// 曲面制御点生成(初回)
	GetObjPtr(&op);														// 制御点OP保存
	SetSelNo(op, 2);													// 黄色表示OP設定
	BaseCmdPtr(1, op1, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲面編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)!=THICKNESS) {									// <厚みづけ以外の場合>
			RedoCommandCopy(op, cmp);									// ｺﾏﾝﾄﾞ複写実行
		}
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// 立体編集後制御点取得
	}
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
	*op2 = op;
}

/********************************************************************/
void ExecSweepSurf(OBJTYPE* op1, int un, int vn, int ub, int vb,
                   int rp, double wt, int an, int pl, PNTTYPE* ps,
                   int* is, PNTTYPE* pa, PNTTYPE* pb, OBJTYPE** op2)	//<<< 掃引曲面実行
/********************************************************************/
{
	int*     wk = new int[View->m_MaxSCtA];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	ChgSweepSurf(NULL, un, vn, ub, vb, rp, wt, ps, is);					// 掃引曲面変更(初回)
	SetSweepCtrl(pl, un, vn, ub, ps, is, pa, wk);						// 掃引曲面制御点変換
	SetSurface(un, vn, ub, vb, pa);										// 掃引曲面制御点生成(初回)
	GetObjPtr(&op);														// 制御点OP保存
	SetSelNo(op, 2);													// 黄色表示OP設定
	BaseCmdPtr(1, op1, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲面編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)!=THICKNESS) {									// <厚みづけ以外の場合>
			RedoCommandCopy(op, cmp);									// ｺﾏﾝﾄﾞ複写実行
		}
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// 立体編集後制御点取得
	}
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
	*op2 = op;
	delete[] wk;
}

/***************************/
void AttachObj(OBJTYPE* op)												//<<< ｺﾏﾝﾄﾞ再接続/再実行
/***************************/
{
	AttachCmd(op);														// ｺﾏﾝﾄﾞ再接続
	RedoCommand(op);													// ｺﾏﾝﾄﾞ再実行
	View->SetDispList(op);												// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ登録
}

/*************************************************************/
void PickPointMove(int md, int pn, int* pk, int un, int vn,
                   int ub, int vb, double wt, int an, int f1,
                   int f2, int sf, OBJTYPE* op, PNTTYPE* pa,
                   PNTTYPE* pb, int* ia, int* ib, int* ic)				//<<< 制御点移動 点選択
/*************************************************************/
{
	int i;

	for(i=0; i<pn; i++) {
		PickCPoint(pb[pk[i]], pk[i], an, pb, ub, vb, un, vn,
                   f1, f2, sf, ia, ib);									// 対象制御点選択
	}
	SetLineNo1(un, vn, f1, f2, ib);										// ﾗｲﾝ番号設定1
	RedoChgSurface(un, vn, ub, vb, pa, ia);								// 曲面生成
	CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);			// 制御点=>通過点変換
	RedoSetSurface(un, vn, ub, vb, pb);									// 曲面制御点生成
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// 立体編集後制御点取得
		ic[i] = (!ib[i]) ? ia[i] : 4;
	}
}

/********************************************************************/
void PickLineMove(int md, int pn, int* pk, int sw, int un, int vn,
                  int ub, int vb, double wt, int an, int f1, int f2,
                  int sf, int pl, int* tn, OBJTYPE* op, PNTTYPE* pa,
                  PNTTYPE* pb, PNTTYPE* ps, PNTTYPE* pw,
                  int* ia, int* ib, int* ic, int* is, int* iw)			//<<< 制御点列移動 列選択
/********************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, no;

	for(i=0; i<pn; i++) {
		no = pk[i];														// 選択番号
		iw[no] = (iw[no]) ? 0 : 1;										// 選択状態反転
		if(sw) {														// <通常曲面>
			PickCLine(no, ub, vb, un, vn, f1, f2, sf, pb, ia, ib);		// 対象制御点列選択
		} else {														// <掃引曲面>
			if(sf) {
				if(no<vn) {
					is[un+no] = (is[un+no]) ? 0 : 1;					// 反転
				} else {
					is[no-vn] = (is[no-vn]) ? 0 : 1;
				}
				if(!ub) {
					is[0] = 1, is[un-1] = 1;
				}
				if(!vb) {
					is[un] = 1, is[un+vn-1] = 1;
				}
			}
		}
	}
	SetLineNo2(vn, *tn, f1, f2, iw);									// ﾗｲﾝ番号設定2
	if(sw) {															// <通常曲面>
		RedoChgSurface(un, vn, ub, vb, pa, ia);							// 曲面変更
		CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);		// 制御点=>通過点変換
	} else {															// <掃引曲面>
		RedoChgSweepSurf(un, vn, ub, vb, ps, is);						// 掃引曲面変更
		SetSweepCtrl(pl, un, vn, ub, ps, is, pb, wk);					// 掃引曲面制御点変換
	}
	RedoSetSurface(un, vn, ub, vb, pb);									// 曲面制御点生成
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// 立体編集後制御点取得
	}
	TrackerCLine(md, sw, un, vn, pb, f1, f2, tn, pw, ic);				// 点列ﾄﾗｯｶｰ取得
	for(i=0; i<*tn; i++) {
		ic[i] = (iw[i]) ? 4 : ic[i];									// 指定点色変更
	}
	delete[] wk;
}

/************************************************************/
BOOL CheckLineCopy(int md, int pn, int* pk, int un, int vn,
				   int ub, int vb, int vf, int* cn, int* bn)			//<<< 制御点列複写ﾁｪｯｸ
/************************************************************/
{
	int i, mx, ct1=0, ct2=0, err=0;

	if(md) {															// <<通常曲面の場合>>
		mx = 2*un+vn;													// 最大挿入位置No
		mx = (!ub) ? mx-1 : mx;											// Uが開は減算
		mx = (!vb) ? mx-1 : mx;											// Vが開は減算
		for(i=0; i<pn; i++) {
			if(pk[i]<mx) {												// 挿入指示数ｶｳﾝﾄ
				ct1++;
			} else {													// 複写指示数ｶｳﾝﾄ
				ct2++;
			}
			err = (vf&&(pk[i]==0||pk[i]==2*un-2)) ? 1 : err;			// V収束で両端点はｴﾗｰ
		}
		if(err==0&&ct1==0&&ct2==1) {									// <複写指示:1の場合>
			*cn = pk[0]-mx;
			CSetBaseDlg4 dlg;
			dlg.SetValue(*bn);											// 既存値表示
			if(dlg.DoModal()==IDOK) {									// 入力ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(bn);										// 基準点取得
			} else {
				*cn = -1;
			}
		} else if(ct2>1) {
			*cn = -1;
		}
		if(err==1||ct2>0) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

/**********************************************************************/
void PickLineInsert(int md, int pn, int* pk, int* un, int* vn, int ub,
                    int vb, int* an, int uf, int sf, int pl, int* cn,
                    int bn, OBJTYPE* op, PNTTYPE cpt, PNTTYPE* pa,
                    PNTTYPE* pb, PNTTYPE* ps, int* ia, int* is)			//<<< 制御点列挿入 列選択
/**********************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, j, no, i1, i2, i3;

	for(i=pn-1; i>=0; i--) {
		if(md) {														// <<通常曲面>>
			no = pk[i], i1 = 2*(*un), i2 = *un, i3 = *vn;
			i1 = (!ub) ? i1-1 : i1;										// Uが開は減算
			if(no<i1) {													// <U方向指定の場合>
				i2 = (no%2==1) ? i2+1 : i2;
			} else {													// <V方向指定の場合>
				i3++;
			}
		} else {														// <<掃引曲面>>
			no = pk[i], i1 = *un, i2 = *un, i3 = *vn;
			i1 = (!ub) ? i1-1 : i1;										// Uが開は減算
			if(no<i1) {													// <U方向指定の場合>
				i2++;
			} else {													// <V方向指定の場合>
				i3++;
			}
		}
		if(i2>View->m_MaxCCtl||i3>View->m_MaxCCtl||
           i2*i3>View->m_MaxSCtl) {										// <<制御点上限より大>>
			AfxMessageBox(IDS_MANY_CTRLP);								// ｴﾗｰﾒｯｾｰｼﾞ
		} else {														// <<制御点上限以下>>
			if(md) {													// <通常曲面>
				InsertCLine(no, sf, ub, un, vn, uf,cn,bn,cpt,pa,ia);	// 対象制御点列挿入
				RedoChgSurface(*un, *vn, ub, vb, pa, ia);				// 曲面変更
			} else {													// <掃引曲面>
				InsertSweep(no, sf, ub, un, vn, ps, is);				// 掃引制御点列挿入
				RedoChgSweepSurf(*un, *vn, ub, vb, ps, is);				// 掃引曲面変更
				SetSweepCtrl(pl, *un, *vn, ub, ps, is, pa, wk);			// 掃引曲面制御点変換
			}
			*an = (*un)*(*vn);											// 総制御点数
			RedoSetSurface(*un, *vn, ub, vb, pa);						// 曲面制御点生成
			for(j=0; j<*an; j++) {
				GetVtx(op, j, &pb[j]);									// 立体編集後制御点取得
			}
		}
	}
	delete[] wk;
}

/********************************************************************/
BOOL PickLineDivide(int md, int pn, int* pk, int un, int vn, int ub,
                    int vb, int rp, double wt, int pl, OBJTYPE* op,
                    PNTTYPE* pa, PNTTYPE* ps, int* ia, int* is)			//<<< 制御点列分割
/********************************************************************/
{
	OBJTYPE* cop;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE* wp1 = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* wp2 = new PNTTYPE[View->m_MaxSCtA];
	int*     wi1 = new int[View->m_MaxSCtA];
	int*     wi2 = new int[View->m_MaxSCtA];
	char*    name = new char[24];
	int      i, j, u1, u2, v1, v2, uv, no, ct, sm;
	BOOL     ret=TRUE;

	no = pk[0];
	if(md) {															// <<通常曲面>>
		u1 = (!ub) ? 2*un-1 : 2*un;										// Uが開は減算
		if(no<u1) {														// <U方向指定の場合>
			if(no%2==1) {
				no = (no+1)/2, uv = 0;
			} else {
				ret = FALSE;
			}
		} else {														// <V方向指定の場合>
			no = no-u1+1, uv = 1;
		}
	} else {															// <<掃引曲面>>
		u1 = (!ub) ? un-1 : un;											// Uが開は減算
		if(no<u1) {														// <U方向指定の場合>
			no = no+1, uv = 0;
		} else {														// <V方向指定の場合>
			no = no-u1+1, uv = 1;
		}
	}
	if(ret) {															// <<<有効な指定の場合>>>
		u1 = 0, v1 = 0, u2 = 0, v2 = 0;
		if(md) {														// <<通常曲面>>
			if(!uv) {													// <U方向指定の場合>
				if(ub) {												// <U閉の場合>
					u1 = un, v1 = vn, ub = 0, ct = 0;
					for(j=0; j<vn; j++) {
						for(i=no; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// 切断位置以降を複写
							if(i==no) {
								wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;	// 切断端は折線点
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
						for(i=0; i<no; i++) {
							wp1[ct] = pa[un*j+i];						// 連続して切断位置より前を複写
							if(i==no-1) {
								wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;	// 切断端は折線点
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
				} else {												// <U開の場合>
					if(no<2&&un-no<2) {									// <切断位置前後が制御点数不足>
						AfxMessageBox(IDS_FEW_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					} else if(no>=2&&un-no<2) {							// <切断位置前が有効>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=0; i<no; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置より前を複写
								if(i==no-1) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(no<2&&un-no>=2) {							// <切断位置後が有効>
						u1 = un-no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=no; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置以降を複写
								if(i==no) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else {											// <切断位置前後が有効>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=0; i<no; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置より前を複写
								if(i==no-1) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						u2 = un-no, v2 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=no; i<un; i++) {
								wp2[ct] = pa[un*j+i];					// 切断位置以降を複写
								if(i==no) {
									wi2[ct++] = (int)(ia[un*j+i]/2)*2+1;// 切断端は折線点
								} else {
									wi2[ct++] = ia[un*j+i];
								}
							}
						}
					}
				}
			} else {													// <V方向指定の場合>
				if(vb) {												// <V閉の場合>
					u1 = un, v1 = vn, vb = 0, ct = 0;
					for(j=no; j<vn; j++) {
						for(i=0; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// 切断位置以降を複写
							if(j==no) {
								wi1[ct++] = ia[un*j+i]%2+2;				// 切断端は折線点
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
					for(j=0; j<no; j++) {
						for(i=0; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// 連続して切断位置より前を複写
							if(j==no-1) {
								wi1[ct++] = ia[un*j+i]%2+2;				// 切断端は折線点
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
				} else {												// <V開の場合>
					if(no<3&&vn-no<3) {									// <切断位置前後が制御点数不足>
						AfxMessageBox(IDS_FEW_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					} else if(no>=3&&vn-no<3) {							// <切断位置前が有効>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(j=0; j<no; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置より前を複写
								if(j==no-1) {
									wi1[ct++] = ia[un*j+i]%2+2;			// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(no<3&&vn-no>=3) {							// <切断位置後が有効>
						u1 = un, v1 = vn-no, vb = 0, ct = 0;
						for(j=no; j<vn; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置以降を複写
								if(j==no) {
									wi1[ct++] = ia[un*j+i]%2+2;			// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else {											// <切断位置前後が有効>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(j=0; j<no; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// 切断位置より前を複写
								if(j==no-1) {
									wi1[ct++] = ia[un*j+i]%2+2;			// 切断端は折線点
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						u2 = un, v2 = vn-no, vb = 0, ct = 0;
						for(j=no; j<vn; j++) {
							for(i=0; i<un; i++) {
								wp2[ct] = pa[un*j+i];					// 切断位置以降を複写
								if(j==no) {
									wi2[ct++] = ia[un*j+i]%2+2;			// 切断端は折線点
								} else {
									wi2[ct++] = ia[un*j+i];
								}
							}
						}
					}
				}
			}
		} else {														// <<掃引曲面>>
			if(!uv) {													// <U方向指定の場合>
				if(ub) {												// <U閉の場合>
					u1 = un, v1 = vn, ub = 0, ct = 0;
					for(i=no; i<un; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// 切断位置以降を複写
					}
					for(i=0; i<no; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// 連続して切断位置より前を複写
					}
					wi1[0] = 1, wi1[ct-1] = 1;							// 切断の両端は折線点
					for(i=0; i<vn; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// V側制御点列
					}
				} else {												// <U開の場合>
					if(no<2&&un-no<2) {									// <切断位置前後が制御点数不足>
						AfxMessageBox(IDS_FEW_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					} else if(no>=2&&un-no<2) {							// <切断位置前が有効>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// 切断位置より前を複写
						}
						wi1[ct-1] = 1;									// 切断端は折線点
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V側制御点列
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(no<2&&un-no>=2) {							// <切断位置後が有効>
						u1 = un-no, v1 = vn, ub = 0, ct = 0;
						for(i=no; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// 切断位置以降を複写
						}
						wi1[0] = 1;										// 切断端は折線点
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V側制御点列
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else {											// <切断位置前後が有効>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// 切断位置より前を複写
						}
						wi1[ct-1] = 1;									// 切断端は折線点
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V側制御点列
						}
						u2 = un-no, v2 = vn, ub = 0, ct = 0;
						for(i=no; i<un; i++) {
							wp2[ct] = ps[i], wi2[ct++] = is[i];			// 切断位置以降を複写
						}
						wi1[0] = 1;										// 切断端は折線点
						for(i=0; i<vn; i++) {
							wp2[ct] = ps[i+un], wi2[ct++] = is[i+un];	// V側制御点列
						}
					}
				}
			} else {													// <V方向指定の場合>
				if(vb) {												// <V閉の場合>
					u1 = un, v1 = vn, vb = 0, ct = 0;
					for(i=0; i<un; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// U側制御点列
					}
					for(i=no; i<vn; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// 切断位置以降を複写
					}
					for(i=0; i<no; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// 連続して切断位置より前を複写
					}
					wi1[un] = 1, wi1[ct-1] = 1;							// 切断の両端は折線点
				} else {												// <V開の場合>
					if(no<3&&vn-no<3) {									// <切断位置前後が制御点数不足>
						AfxMessageBox(IDS_FEW_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					} else if(no>=3&&vn-no<3) {							// <切断位置前が有効>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U側制御点列
						}
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// 切断位置より前を複写
						}
						wi1[ct-1] = 1;									// 切断端は折線点
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else if(no<3&&vn-no>=3) {							// <切断位置後が有効>
						u1 = un, v1 = vn-no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U側制御点列
						}
						for(i=no; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// 切断位置以降を複写
						}
						wi1[un] = 1;									// 切断端は折線点
						AfxMessageBox(IDS_ERR_CTRLNUM);					// ｴﾗｰﾒｯｾｰｼﾞ
					} else {											// <切断位置前後が有効>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U側制御点列
						}
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// 切断位置より前を複写
						}
						wi1[ct-1] = 1;									// 切断端は折線点
						u2 = un, v2 = vn-no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp2[ct] = ps[i], wi2[ct++] = is[i];			// U側制御点列
						}
						for(i=no; i<vn; i++) {
							wp2[ct] = ps[i+un], wi2[ct++] = is[i+un];	// 切断位置以降を複写
						}
						wi1[un] = 1;									// 切断端は折線点
					}
				}
			}
		}
	}
	if(ret) {															// <<<有効な指定の場合>>>
		if(md) {														// <<通常曲面>>
			if(u1>0&&v1>0) {											// <Redo:OK>
				RedoChgSurface(u1, v1, ub, vb, wp1, wi1);				// 曲面変更
			}
			if(u2>0&&v2>0) {											// <Create:OK>
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				sm = GetParaInt(cmp, 6);								// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
				CrtSurface(u2, v2, ub, vb, rp, wt, sm, wp2, wi2);		// 曲面生成
				GetObjPtr(&cop);										// 制御点OP保存
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(cop, cmp);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				View->SetDispList(cop);									// ﾃﾞｨｽﾌﾟﾚｲ・ﾘｽﾄ登録
				OnEdtFlg(1, cop);										// edtflg:ON
				GetObjOif(op, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
				GetOifName(oif, name);									// 立体構成名取得
				GetObjOif(cop, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
				SetOifName(0, oif, name);								// 立体構成名設定
			}
		} else {														// <<掃引曲面>>
			if(u1>0&&v1>0) {											// <Redo:OK>
				RedoChgSweepSurf(u1, v1, ub, vb, wp1, wi1);				// 掃引曲面変更
			}
			if(u2>0&&v2>0) {											// <Create:OK>
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				sm = GetParaInt(cmp, 6);								// ｿﾘｯﾄﾞﾓｰﾄﾞ取得
				CrtSweepSurf(u2, v2, ub, vb, rp, wt, sm, pl, wp2, wi2);	// 掃引曲面生成
				GetObjPtr(&cop);										// 制御点OP保存
				BaseCmdPtr(1, op, &cmp);								// 有効先頭ｺﾏﾝﾄﾞ取得
				NextCmdPtr(&cmp);										// 曲線編集後ｺﾏﾝﾄﾞ
				while(cmp!=NULL) {
					RedoCommandCopy(cop, cmp);							// ｺﾏﾝﾄﾞ複写実行
					NextCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				}
				View->SetDispList(cop);									// ﾃﾞｨｽﾌﾟﾚｲ・ﾘｽﾄ登録
				OnEdtFlg(1, cop);										// edtflg:ON
				GetObjOif(op, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
				GetOifName(oif, name);									// 立体構成名取得
				GetObjOif(cop, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
				SetOifName(0, oif, name);								// 立体構成名設定
			}
		}
	}
	delete[] wp1;
	delete[] wp2;
	delete[] wi1;
	delete[] wi2;
	delete[] name;
	return ret;
}

/*********************************************************/
void PickSurfLine(int pn, int* pk, int fg, int tn,
                  OBJTYPE* op, int* ib, int* iw, int* bn)				//<<< 制御点列選択
/*********************************************************/
{
	int i, j;

	for(i=0; i<pn; i++) {
		if(ib[pk[i]]) {													// 選択済は解除
			ib[pk[i]] = 0, iw[pk[i]] = 2;
		} else {														// 未選択は選択
			ib[pk[i]] = 1, iw[pk[i]] = 4;
		}
	}
	SetLineNo3(tn, fg, ib, op);											// ﾗｲﾝ番号設定3
	for(i=0, j=-1; i<tn; i++) {
		if(ib[i]) {														// 最小Noの指示済み
			j = i; break;
		}
	}
	if(*bn==-1&&j!=-1) {												// 基準点No設定
		*bn = (fg%2) ? j+1 : j;
	}
}

/***************************************************************/
static void GetSPoint(int fg, int ix, int un, int vn, int vflg,
                      PNTTYPE scp, PNTTYPE* pa, int* ia,
                      PNTTYPE* pb, int* ib, int* ta, int* tt,
                      PNTTYPE* cpt, VECTYPE* vec, int* plane)			//<<< 断面制御点情報取得
/***************************************************************/
{
	VECTYPE nvc, rvc;
	int     i, j, i1, i2, i3, vd;
	double  rot[3][3], rth, dd;

	if(vflg%2==1) {														// <V始点同一の場合>
		ix++;
	}
	for(i=0; i<vn; i++) {												// <<V方向ﾄﾗｯｶｰ>>
		pb[i] = pa[ix+i*un];											// 制御点座標抽出
		ib[i] = ia[ix+i*un]/2;											// 曲線ﾓｰﾄﾞ抽出
		tt[i] = ta[ix+i*un];											// 張力限定情報抽出
	}
	CalcCent(pb, vn, cpt);												// V断面中心取得
	CalcNVec(pb, vn, &nvc);												// V断面法線ﾍﾞｸﾄﾙ取得
	*vec = nvc;
	for(i=0, vd=0, dd=0.0; i<DIM; i++) {
		if(dd<fabs(nvc.p[i])) {											// 基準断面ﾍﾞｸﾄﾙ最大方向検索
			dd = fabs(nvc.p[i]), vd = i;
		}
	}
	*plane = (vd+1>2) ? 0 : vd+1;										// 断面存在平面
	i1 = (vd+1>2) ? 0 : vd+1;											// 座標軸設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pb[i].p[j] -= cpt->p[j];									// 断面原点移動
		}
	}
	rth = sqrt(nvc.p[i1]*nvc.p[i1]+nvc.p[i2]*nvc.p[i2]);				// 基準面方向長さ
	if(rth>EPSILON) {													// <基準面にない場合>
		rvc.p[i1] = -nvc.p[i2], rvc.p[i2] = nvc.p[i1], rvc.p[i3] = 0.0;
		VecN(&rvc);
		rth = atan2(rth, nvc.p[i3]);									// 回転角度算出
		rth = (rth<-10||rth>10) ? 0.0 : rth;
		rth = -rth;
		rth = (rth<-PI/2) ? rth-PI : rth;
		SetRotateMatrix(rvc, rth, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {											// 基準面に回転
			VecRotate(&pb[i], rot);
		}
	}
	if(fg) {															// <立体消去>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] += scp.p[j];									// 断面編集位置移動
			}
		}
	} else {															// <立体非消去>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] += cpt->p[j];								// 断面編集位置移動
			}
		}
	}
}

/************************************************************************/
void PickSPointMove1(int no, int md, double* da, double* db, int m1,
                     int m2, int un, int vn, int vb, int rp, double wt,
                     int fg, int pl, int* vl, PNTTYPE* pb, PNTTYPE* ps,
                     PNTTYPE* pz, PNTTYPE* pw, PNTTYPE* p1, VECTYPE* p2,
                     PNTTYPE* p3, int* ia, int* is, int* iz, int* iw,
                     int* iy, int* t1, int* t2, OBJTYPE* op1,
                     OBJTYPE* op2, OBJTYPE** op3, OBJTYPE** op4)		//<<< 断面制御点移動 列選択
/************************************************************************/
{
	int i, j;

	View->GetCentPnt(p3);												// 画面中心取得
	if(m2) {															// <他立体消去：有>
		OffEdtFlg(1, op1);												// 曲面立体立体消去
		OffDspFlg(1, op1);
		OffEdtFlg(1, op2);												// 曲面制御点立体消去
		OffDspFlg(1, op2);
	} else {															// <他立体消去：無>
		OffEdtFlg(1, op1);												// 曲面立体立体対象解除
		OffEdtFlg(1, op2);												// 曲面制御点立体消去
		OffDspFlg(1, op2);
	}
	if(m1) {															// <通常曲面>
		GetSPoint(m2, no, un, vn, fg, *p3, pb, ia, pw, iz,
                  t1, t2, p1, p2, vl);									// 断面制御点情報取得
	} else {															// <掃引曲面>
		for(i=0; i<vn; i++) {
			pw[i] = ps[un+i], iz[i] = is[un+i];							// U断面情報抽出
			for(j=0; j<DIM; j++) {
				pw[i].p[j] += p3->p[j];
			}
		}
		if(pl>-1) {														// 旧掃引曲面の場合
			*vl = (pl-1<0) ? 2 : pl-1;									// U断面平面
		} else {														// 新掃引曲面はYZ平面固定
			*vl = 1;
		}
	}
	SetCurveMatrix3(md, wt, vb, vn, iz, da, db);						// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(md, vn, pw, pz, db);									// 通過点=>制御点変換
	CrtCurve(vn, vb, rp, wt, pz, iz);									// 断面曲線生成
	GetObjPtr(op3);														// 曲線OP保存
	OnEdtFlg(1, *op3);													// 曲線立体編集対象設定
	SetCurve(vn, vb, pw);												// 制御点立体生成
	GetObjPtr(op4);														// 制御点OP保存
	SetSelNo(*op4, 2);													// 黄色表示OP設定
	for(i=0; i<vn; i++) {												// ﾄﾗｯｶｰ色設定/指示ﾌﾗｸﾞ初期化
		iw[i] = iz[i]*2, iy[i] = 0;
	}
}

/*********************************************************************/
void PickSPointMove2(int pn, int* pk, int md, double* da, double* db,
                     int vn, int vb, double wt, int sf, PNTTYPE* pz,
                     PNTTYPE* pw, int* iz, int* iw, int* iy)			//<<< 断面制御点移動 点選択
/*********************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	int      i;

	for(i=0; i<pn; i++) {
		iy[pk[i]] = (iy[pk[i]]) ? 0 : 1;								// 選択状態反転
		if(sf) {
			iz[pk[i]] = (iz[pk[i]]) ? 0 : 1;							// 折線/曲線反転
		}
	}
	CalcCurvePoint(md, vn, pz, pt, da);									// 制御点=>通過点変換
	SetCurveMatrix3(md, wt, vb, vn, iz, da, db);						// 制御点<=>通過点変換ﾏﾄﾘｯｸｽ
	CalcCurvePoint(md, vn, pt, pz, db);									// 通過点=>制御点変換
	RedoCrtCurve(vn, vb, pz, iz);										// 断面曲線生成
	CalcCurvePoint(md, vn, pz, pw, da);									// 制御点=>通過点変換
	RedoSetCurve(vn, vb, pw);											// 制御点立体生成
	for(i=0; i<vn; i++) {
		iw[i] = (iy[i]) ? 4 : iz[i]*2;									// ﾄﾗｯｶｰ色設定
	}
	delete[] pt;
}

/************************************************************************/
void ChangeSPointMove(int m1, int m2, int m3, int m4, int no, int un,
                      int vn, int ub, int vb, double wt, int an, int fg,
                      int pl, PNTTYPE p1, VECTYPE p2, PNTTYPE p3,
                      PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                      PNTTYPE* ps, PNTTYPE* pz, int* ia, int* is,
                      int* iz, int* ta, CMDTYPE* cmp, OBJTYPE* op1,
                      OBJTYPE* op2, OBJTYPE* op3, OBJTYPE* op4)			//<<< 断面制御点移動 断面変更
/************************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, j;

	if(m4) {
		AttachCmd(op1);													// ｺﾏﾝﾄﾞ再接続
	}
	if(op3!=NULL) {
		FreeObj(op3, 1);												// 立体削除
	}
	if(op4!=NULL) {
		FreeObj(op4, 1);												// 立体削除
	}
	if(m4) {
		DetachCmd(op1, cmp);											// ｺﾏﾝﾄﾞ再度切離
	}
	if(m3) {															// <他立体消去：有>
		OnDspFlg(1, op1);												// 曲面立体編集対象設定
		OnEdtFlg(1, op1);
		OnDspFlg(1, op2);												// 曲線制御点立体表示
	} else {															// <他立体消去：無>
		OnEdtFlg(1, op1);												// 曲面立体対象設定
		OnDspFlg(1, op2);												// 曲線制御点立体表示
	}
	SetSelNo(op2, 2);													// 黄色表示OP設定
	if(m1) {															// <通常曲面>
		PutSPoint(m3, no, un, vn, fg, p3, pz, iz, p1, p2, pb, pc, ia);	// 断面制御点情報更新
		MoveSPoint(op1, un, vn, ub, vb, pc, pa, pb, ta);				// 断面制御点移動
		RedoChgSurface(un, vn, ub, vb, pa, ia);							// 曲面編集
		CalcSurfacePoint(m2, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);		// 制御点=>通過点変換
		RedoSetSurface(un, vn, ub, vb, pb);								// 曲面制御点生成
		for(i=0; i<an; i++) {
			GetVtx(op2, i, &pb[i]);										// 立体編集後制御点取得
		}
	} else {															// <掃引曲面>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pz[i].p[j] -= p3.p[j];
			}
			ps[un+i] = pz[i], is[un+i] = iz[i];							// U断面情報設定
		}
		RedoChgSweepSurf(un, vn, ub, vb, ps, is);						// 掃引曲面変更
		SetSweepCtrl(pl, un, vn, ub, ps, is, pa, wk);					// 掃引曲面制御点変換
		RedoSetSurface(un, vn, ub, vb, pa);								// 曲面制御点生成
		for(i=0; i<an; i++) {
			GetVtx(op2, i, &pb[i]);										// 立体編集後制御点取得
		}
	}
	delete[] wk;
}

/**********************************************************/
void PickBPointMove(int pn, int* pk, int fg,
                    int tn, OBJTYPE* op, int* ib, int* iw)				//<<< ﾍﾞｰｽ線移動 列選択
/**********************************************************/
{
	int i;

	for(i=0; i<pn; i++) {
		if(ib[pk[i]]) {													// 選択済は解除
			ib[pk[i]] = 0, iw[pk[i]] = 2;
		} else {														// 未選択は選択
			ib[pk[i]] = 1, iw[pk[i]] = 4;
		}
	}
	SetLineNo3(tn, fg, ib, op);											// ﾗｲﾝ番号設定3
}

/*********************************/
BOOL InitCScale(int* bn, int* dn)										//<<< 拡大/縮小初期処理
/*********************************/
{
	CSetBaseDlg1 dlg;
	dlg.SetValue(*bn, *dn);												// 既存値表示
	if(dlg.DoModal()==IDOK) {											// 入力ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(bn, dn);											// 基準点,方向取得
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************************************/
void EraseObjSPointMove(int* on, int* dsp, OBJTYPE* top)				//<<< 断面制御点移動
/********************************************************/				//    立体非表示
{
	OBJTYPE* op;
	int      i1, i2;

	BaseObjPtr(0, &op, &i1, &i2);										// 全立体取得
	while(op!=NULL) {
		dsp[(*on)++] = i1;												// 表示ﾓｰﾄﾞ保存
		if(*on>View->m_MaxSCtl) {
			*on = 0; break;
		}
		NextObjPtr(0, &op, &i1, &i2);									// 次立体へ
	}
	if(*on>0) {															// <有効件数以内>
		OffDspAllObject();												// 全立体非表示
		OnDspFlg(1, top);												// 対象立体dspflg:ON
		OnEdtFlg(1, top);												// 対象立体edtflg:ON
	}
}

/******************************************************/
void DispObjSPointMove(int on, int* dsp, OBJTYPE* top)					//<<< 断面制御点移動
/******************************************************/				//    立体表示
{
	OBJTYPE* op;
	int      i1, i2, ct=0;

	if(on>0) {															// <対象外立体消去有>
		BaseObjPtr(0, &op, &i1, &i2);									// 全立体取得
		while(op!=NULL) {
			if(dsp[ct++]) {												// 表示ﾓｰﾄﾞ復元
				OnDspFlg(1, op);
			}
			NextObjPtr(0, &op, &i1, &i2);								// 次立体へ
		}
		OnEdtFlg(1, top);												// 対象立体edtflg:ON
	}
}

/*****************************************************************/
static void AdjustMoveValue(int d1, int d2, int cn,
                            PNTTYPE* ca, PNTTYPE pt, PNTTYPE* mv)		//<<< 曲線合わせ移動量取得
/*****************************************************************/
{
	int    i;
	double l1, l2, l3, p1, p2;

	for(i=0, l1=DBL_MAX; i<cn-1; i++) {
		if(((ca[i].p[d2]<=pt.p[d2]&&ca[i+1].p[d2]>=pt.p[d2])||
			(ca[i].p[d2]>=pt.p[d2]&&ca[i+1].p[d2]<=pt.p[d2]))&&
			fabs(ca[i].p[d2]-ca[i+1].p[d2])>EPSILON) {					// <該当線分内>
			l2 = fabs(ca[i].p[d2]-ca[i+1].p[d2]);						// 線分d2方向長
			l3 = fabs(ca[i].p[d2]-pt.p[d2]);							// 交点位置d2方向長
			p2 = ca[i].p[d1]+(ca[i+1].p[d1]-ca[i].p[d1])*l3/l2;			// d1交点位置
			l2 = fabs(pt.p[d1]-p2);										// ptと交点位置の長さ
			if(l2<l1) {													// 最も近い交点位置
				p1 = p2, l1 = l2;
			}
		}
	}
	mv->p[0] = 0.0, mv->p[1] = 0.0, mv->p[2] = 0.0;						// 移動量設定
	mv->p[d1] = p1-pt.p[d1];
}

/**********************************************************************/
void AdjustCPoint(int d1, int d2, int cn, PNTTYPE* ca, int on,
                  int tn, int* ta, PNTTYPE* pa, PNTTYPE* pb,
                  PNTTYPE* pc, PNTTYPE* pd, int* ia, int* ib, int* ic)	//<<< 制御点曲線合わせ
/**********************************************************************/
{
	PNTTYPE* wp = new PNTTYPE[View->m_MaxCCtA*2];
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  zer;
	int      i, i1, i2, un, vn, ub, vb, an, rp, cnt=0;
	double   wd[10], min1, max1, wt;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	for(i=0, min1=DBL_MAX, max1=1.0-DBL_MAX; i<cn; i++) {
		min1 = (min1>ca[i].p[d2]) ? ca[i].p[d2] : min1;					// 基準曲線最小値
		max1 = (max1<ca[i].p[d2]) ? ca[i].p[d2] : max1;					// 基準曲線最大値
	}
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
			}
			FreeObj(op1, 1);											// 編集後立体削除
			for(i=0; i<vn; i++) {
				ib[i] = (ta[cnt+i]==4) ? 1 : 0;							// 指示ﾌﾗｸﾞ設定
			}
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<vn; i++) {
					ic[i] = 1;											// 張力有は全範囲設定
				}
			}
			for(i=0; i<vn; i++) {
				if(ib[i]&&pb[i].p[d2]>=min1&&pb[i].p[d2]<=max1) {		// <指示済で基準曲線範囲内>
					AdjustMoveValue(d1, d2, cn, ca, pb[i], &pc[i]);		// 曲線合わせ移動量取得
				} else {												// 以外は移動量なし
					pc[i] = zer;
				}
			}
			OnEdtFlg(1, op);											// 編集対象立体登録
			for(i=0; i<vn; i++) {
				wp[i*2] = pb[i];										// 制御点を曲線編集用に変更
			}
			MoveECurve(0, wd, pc, op, vn, vb, pa, wp, ib, pd, ic);		// 制御点移動
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
			}
			FreeObj(op1, 1);											// 編集後立体削除
			for(i=0; i<an; i++) {
				ib[i] = (ta[cnt+i]==4) ? 1 : 0;							// 指示ﾌﾗｸﾞ設定
			}
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<an; i++) {
					ic[i] = 1;											// 張力有は全範囲設定
				}
			}
			for(i=0; i<an; i++) {
				if(ib[i]&&pb[i].p[d2]>=min1&&pb[i].p[d2]<=max1) {		// <指示済で基準曲線範囲内>
					AdjustMoveValue(d1, d2, cn, ca, pb[i], &pc[i]);		// 曲線合わせ移動量取得
				} else {												// 以外は移動量なし
					pc[i] = zer;
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
	delete[] wp;
}

/************************************************************/
BOOL GetSurfaceCurve(int un, int vn, int ub, int vb, int uf,
                     int vf, int mx, int no, OBJTYPE* op,
                     int* pn, int* bc, PNTTYPE* pa)						//<<< 曲面曲線取得
/************************************************************/
{
	int  i, wuv, wua, wva, flg, lno, uu, vv, ms, div=1;
	BOOL ret=TRUE;

	wva = (uf%2==1) ? 1 : 0;											// U/V境界と補正数取得
	wua = (vf%2==1) ? 1 : 0;
	wuv = (uf%2==1) ? vn-1 : vn;
	wuv = (uf>=2) ? wuv-1 : wuv;
	if(no<wuv) {														// <Vﾗｲﾝ>
		flg = 1;
		lno = no+wva;
	} else {															// <Uﾗｲﾝ>
		flg = 0;
		lno = no-wuv+wua;
	}
	uu = un, vv = vn;													// U/V制御点数
	if(ub) {															// U閉は制御点数UP
		uu++;
	}
	if(vb) {															// V閉は制御点数UP
		vv++;
	}
	if(uu>1&&vv>1) {
		for(i=0; i<mx; i++) {
			div = div*2;												// 制御点から頂点への倍数
		}
		uu = (uu-1)*div+1;												// 頂点数
		vv = (vv-1)*div+1;
		if(uf==1||uf==2||uf==3) {										// <<U端同一点収束>>
			if(ub) {													// U閉は始点/終点同一
				uu--;
			}
			if(uf==1) {													// <<先頭Uﾙｰﾌﾟ同一>>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*(div*lno-1)+i+1, &pa[i]);		// 指定頂点取得
						}
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// 指定頂点取得
						for(i=1; i<vv; i++) {
							GetVtx(op, lno*div+uu*(i-1)+1, &pa[i]);		// 指定頂点取得
						}
					}
				}
			} else if(uf==2) {											// <<最終Uﾙｰﾌﾟ同一>>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*div*lno+i, &pa[i]);			// 指定頂点取得
						}
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv-1; i++) {
							GetVtx(op, lno*div+uu*i, &pa[i]);			// 指定頂点取得
						}
						GetVtx(op, uu*(vv-1), &pa[i]);					// 指定頂点取得
					}
				}
			} else {													// <<先頭/最終同一>>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*(div*lno-1)+i+1, &pa[i]);		// 指定頂点取得
						}
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// 指定頂点取得
						for(i=1; i<vv-1; i++) {
							GetVtx(op, lno*div+uu*(i-1)+1, &pa[i]);		// 指定頂点取得
						}
						GetVtx(op, uu*(vv-2)+1, &pa[i]);				// 指定頂点取得
					}
				}
			}
		} else if(vf==1||vf==2||vf==3) {								// <<V端同一点収束>>
			if(vb) {													// V閉は始点/終点同一
				vv--;
			}
			if(vf==1) {													// <先頭Vﾙｰﾌﾟ同一>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// 指定頂点取得
						for(i=1; i<uu; i++) {
							GetVtx(op, (uu-1)*div*lno+i, &pa[i]);		// 指定頂点取得
						}
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							GetVtx(op, lno*div+(uu-1)*i, &pa[i]);		// 指定頂点取得
						}
					}
				}
			} else if(vf==2) {											// <最終Vﾙｰﾌﾟ同一>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						ms = (div*lno<2) ? 0 : div*lno-1;				// 補正(-)数設定
						for(i=0; i<uu-1; i++) {
							GetVtx(op, uu*div*lno+i-ms, &pa[i]);		// 指定頂点取得
						}
						GetVtx(op, uu-1, &pa[i]);						// 指定頂点取得
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							ms = (i<2) ? 0 : i-1;
							GetVtx(op, lno*div+uu*i-ms, &pa[i]);		// 指定頂点取得
						}
					}
				}
			} else {													// <先頭/最終同一>
				if(flg) {												// <U列指定>
					*pn = uu;											// 曲線頂点数
					*bc = ub;											// 曲線境界条件
					if(uu>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						if(div*lno==0) {								// 補正(-)数設定
							ms = 0;
						} else if(div*lno==1) {
							ms = 1;
						} else {
							ms = 2*div*lno-1;
						}
						GetVtx(op, 0, &pa[0]);							// 指定頂点取得
						for(i=1; i<uu-1; i++) {
							GetVtx(op, uu*div*lno+i-ms, &pa[i]);		// 指定頂点取得
						}
						GetVtx(op, uu-1, &pa[i]);						// 指定頂点取得
					}
				} else {												// <V列指定>
					*pn = vv;											// 曲線頂点数
					*bc = vb;											// 曲線境界条件
					if(vv>View->m_MaxCPnt) {							// <曲線頂点上限以上>
						AfxMessageBox(IDS_MANY_CTRLP);					// ｴﾗｰﾒｯｾｰｼﾞ
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							if(i==0) {
								ms = 0;
							} else if(i==1) {
								ms = 1;
							} else {
								ms = 2*i-1;
							}
							GetVtx(op, lno*div+uu*i-ms, &pa[i]);		// 指定頂点取得
						}
					}
				}
			}
		} else {														// <<同一点収束無>>
			if(ub) {													// U閉は始点/終点同一
				uu--;
			}
			if(vb) {													// V閉は始点/終点同一
				vv--;
			}
			if(flg) {													// <U列指定>
				*pn = uu;												// 曲線頂点数
				*bc = ub;												// 曲線境界条件
				if(uu>View->m_MaxCPnt) {								// <曲線頂点上限以上>
					AfxMessageBox(IDS_MANY_CTRLP);						// ｴﾗｰﾒｯｾｰｼﾞ
					ret = FALSE;
				}
				if(ret) {
					for(i=0; i<uu; i++) {
						GetVtx(op, div*uu*lno+i, &pa[i]);				// 指定頂点取得
					}
				}
			} else {													// <V列指定>
				*pn = vv;												// 曲線頂点数
				*bc = vb;												// 曲線境界条件
				if(vv>View->m_MaxCPnt) {								// <曲線頂点上限以上>
					AfxMessageBox(IDS_MANY_CTRLP);						// ｴﾗｰﾒｯｾｰｼﾞ
					ret = FALSE;
				}
				if(ret) {
					for(i=0; i<vv; i++) {
						GetVtx(op, lno*div+uu*i, &pa[i]);				// 指定頂点取得
					}
				}
			}
		}
	}
	return ret;
}
