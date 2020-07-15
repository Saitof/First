/////////////////////////////////////////////////////////////////////////////
// ユニット立体編集コマンド部
// ECmdUnit.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "RCommand.h"
#include "NumFunc.h"

/**********************************************************/
void InitArrange(int pln, int cn, double th, PNTTYPE ccp,
                 int* lno, int* mna, int* mxa,
                 PNTTYPE* ocp, VECTYPE* cvc, VECTYPE* rvc)				//<<< 石枠配置初期処理
/**********************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  cent, pnt, zer, pt1, pt2, dis;
	VECTYPE  vec, wvc, rtv;
	BOXTYPE  box;
	int      i, j, k, i1, i2, i3, wln, dsp, edt, st;
	double   ft, wk, rot[3][3];

	i1 = pln;															// 座標軸設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	GetAreaBox(2, &box);												// ｴﾘｱﾎﾞｯｸｽ取得
	BoxCenter(box, &cent);												// ﾎﾞｯｸｽｾﾝﾀｰ取得
	pnt.p[i1] = 0.0, pnt.p[i2] = 0.0, pnt.p[i3] = -1.0;					// 回転軸方向ﾍﾞｸﾄﾙ
	zer.p[i1] = 0.0, zer.p[i2] = 0.0, zer.p[i3] = 0.0;					// 仮回転中心点
	for(i=0; i<cn; i++) {
		st = 0;
		BaseObjPtr(2, &op, &dsp, &edt);									// 処理対象立体取得
		while(op!=NULL) {
			SetObjPtr(op);												// 複写対象立体設定
			wop = CopyObject();											// 立体複写
			if(st==0) {
				if(i==0) {
					BaseCmdPtr(0, wop, &cmp);							// 複写ｺﾏﾝﾄﾞ取得
					wln = GetCmdLvl(cmp);								// 現在ｺﾏﾝﾄﾞﾚﾍﾞﾙNo保存
				}
				mna[i] = GetObjNo(wop);									// 先頭･最終立体No保存
				mxa[i] = GetObjNo(wop);
				st = 1;
			} else {
				mxa[i] = GetObjNo(wop);									// 最終立体No保存
			}
			SetObjPtr(wop);												// 対象立体設定
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[複写回転用]
			PutParaInt(cmp, 0, (int)(th*i*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, ccp);									// 回転軸座標設定
			PutParaPnt(cmp, 1, pnt);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保[半径調整用]
			PutParaInt(cmp, 0, 0);										// 移動量設定
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdLvl(cmp, wln+1);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+1)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[水平回転用]
			PutParaInt(cmp, 0, 0);										// 回転角度設定
			PutParaPnt(cmp, 0, zer);									// 回転軸座標設定
			PutParaPnt(cmp, 1, pnt);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdLvl(cmp, wln+2);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+2)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[垂直回転用]
			PutParaInt(cmp, 0, 0);										// 回転角度設定
			PutParaPnt(cmp, 0, zer);									// 回転軸座標設定
			PutParaPnt(cmp, 1, pnt);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdLvl(cmp, wln+3);										// 仮ｺﾏﾝﾄﾞﾚﾍﾞﾙNo変更(+3)
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
			NextObjPtr(2, &op, &dsp, &edt);								// 次立体へ
		}
	}
	Delete();															// 元対象立体削除
	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		OnEdtFlg(1, op);												// 編集対象立体へ
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
	pt1 = ccp;
	pt1.p[i3] = 0.0;
	rtv.p[i1] = 0.0, rtv.p[i2] = 0.0, rtv.p[i3] = -1.0;					// 回転ﾍﾞｸﾄﾙ
	for(i=0; i<cn; i++) {
		ft = th*i*PI/180.0;												// 回転角度
		SetRotateMatrix(rtv, ft, rot);									// 回転ﾏﾄﾘｯｸｽ設定
		for(j=0; j<DIM; j++) {											// 移動量計算
			for(wk=0.0, k=0; k<DIM; k++) {
				wk += rot[j][k]*ccp.p[k];
			}
			dis.p[j] = ccp.p[j]-wk;
		}
		pt2 = cent;														// 複写元中心点
		VecRotateMove(&pt2, rot, dis);									// 頂点座標回転移動
		ocp[i] = pt2;													// 複写先中心点
		pt2.p[i3] = 0;
		VecV(&pt1, &pt2, &vec);											// 配列中心=>複写先中心ﾍﾞｸﾄﾙ
		VecN(&vec);														// 単位ﾍﾞｸﾄﾙ化
		cvc[i] = vec;
		wvc.p[i1] = -vec.p[i2], wvc.p[i2] = vec.p[i1], wvc.p[i3] = vec.p[i3];
		rvc[i] = wvc;													// 中心ﾍﾞｸﾄﾙの直交ﾍﾞｸﾄﾙ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	*lno = wln;
}

/*************************/
void EndArrange(int clvl)												//<<< 石枠配置終了処理
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th;

	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// 先頭ｺﾏﾝﾄﾞ取得
		while(cmp!=NULL) {
			del = 0;
			if(GetCmdLvl(cmp)==clvl+1) {								// <半径調整用ｺﾏﾝﾄﾞ>
				dx = GetParaInt(cmp, 0);								// 移動量取得
				dy = GetParaInt(cmp, 1);
				dz = GetParaInt(cmp, 2);
				del = (dx==0&&dy==0&&dz==0) ? 1 : del;
				SetCmdLvl(cmp, clvl);									// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo戻し
			} else if(GetCmdLvl(cmp)>clvl+1) {							// <水平/垂直回転用ｺﾏﾝﾄﾞ>
				th = GetParaInt(cmp, 0);								// 回転角度取得
				del = (th==0) ? 1 : del;
				SetCmdLvl(cmp, clvl);									// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo戻し
			}
			if(del) {
				dcmp = cmp;
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
			if(del) {
				FreeCmd(op, dcmp, 1);									// 無効ｺﾏﾝﾄﾞ削除
			}
		}
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/***********************************************************************/
static void UnitMove(int lvl, int sno, int eno, PNTTYPE mv, PNTTYPE cp)	//<<< 複写ﾕﾆｯﾄ移動
/***********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// 生成立体の検索
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<対象ﾕﾆｯﾄ>>
			BaseCmdPtr(0, op, &cmp);									// 先頭ｺﾏﾝﾄﾞ取得
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+1) {								// <半径調整用ｺﾏﾝﾄﾞ>
					PutParaInt(cmp, 0, (int)(mv.p[0]*1000.0));			// 移動量設定
					PutParaInt(cmp, 1, (int)(mv.p[1]*1000.0));
					PutParaInt(cmp, 2, (int)(mv.p[2]*1000.0));
				} else if(GetCmdLvl(cmp)>lvl+1) {						// <水平/垂直回転用ｺﾏﾝﾄﾞ>
					PutParaPnt(cmp, 0, cp);								// 回転軸座標設定
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
}

/********************************************************************/
void ArrangeMove(double mv, int lno, int cn, int* mna,
                 int* mxa, PNTTYPE* ocp, PNTTYPE* mcp, VECTYPE* cvc)	//<<< 石枠移動処理
/********************************************************************/
{
	PNTTYPE pt;
	int     i, j;

	for(i=0; i<cn; i++) {
		for(j=0; j<DIM; j++) {
			pt.p[j] = cvc[i].p[j]*mv;									// 今回移動量
			mcp[i].p[j] = ocp[i].p[j] + pt.p[j];						// 移動後中心点
		}
		UnitMove(lno, mna[i], mxa[i], pt, mcp[i]);						// 複写ﾕﾆｯﾄ移動
	}
}

/**************************************************/
static void UnitRotate1(int lvl, int sno, int eno,
                        double th, PNTTYPE cp)							//<<< 複写ﾕﾆｯﾄ回転1
/**************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// 生成立体検索
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<対象ﾕﾆｯﾄ>>
			BaseCmdPtr(0, op, &cmp);									// 先頭ｺﾏﾝﾄﾞ取得
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+2) {								// <水平回転用ｺﾏﾝﾄﾞ>
					PutParaInt(cmp, 0, (int)(th*1000.0));				// 回転角度設定
					PutParaPnt(cmp, 0, cp);								// 回転軸座標設定
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
}

/*****************************************************/
void ArrangeRotate1(double th, int lno, int cn,
                    int* mna, int* mxa, PNTTYPE* mcp)					//<<< 石枠回転処理1
/*****************************************************/
{
	int	i;

	for(i=0; i<cn; i++) {
		UnitRotate1(lno, mna[i], mxa[i], th, mcp[i]);					// 複写ﾕﾆｯﾄ回転1
	}
}

/**********************************************************/
static void UnitRotate2(int lvl, int sno, int eno,
                        double th, PNTTYPE cp, VECTYPE vc)				//<<< 複写ﾕﾆｯﾄ回転2
/**********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pvc;
	int      i;

	for(i=0; i<DIM; i++) {
		pvc.p[i] = vc.p[i];
	}
	LevelBaseObjPtr(0, &op);											// 生成立体の検索
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<対象ﾕﾆｯﾄ>>
			BaseCmdPtr(0, op, &cmp);									// 先頭ｺﾏﾝﾄﾞ取得
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+3) {								// <垂直回転用ｺﾏﾝﾄﾞ>
					PutParaInt(cmp, 0, (int)(th*1000.0));				// 回転角度設定
					PutParaPnt(cmp, 0, cp);								// 回転軸座標設定
					PutParaPnt(cmp, 1, pvc);							// 回転軸方向ﾍﾞｸﾄﾙ設定
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		LevelNextObjPtr(0, &op);										// 次生成立体検索
	}
}

/*********************************************************/
void ArrangeRotate2(double th, int lno, int cn, int* mia,
                    int* maa, PNTTYPE* mcp, VECTYPE* rvc)				//<<< 石枠回転処理2
/*********************************************************/
{
	int	i;

	for(i=0; i<cn; i++) {
		UnitRotate2(lno, mia[i], maa[i], th, mcp[i], rvc[i]);			// 複写ﾕﾆｯﾄ回転2
	}
}

/****************************************************************/
void InitBasePos(int flg, int ifg, int pln, int in, PNTTYPE* ip,
                 int* on, int* oia, int* oib, PNTTYPE* opa)				//<<< 基準移動初期処理
/****************************************************************/
{
	PNTTYPE* wc = new PNTTYPE[View->m_MaxSCtA];
	double*  ln = new double[View->m_MaxCPnt];
	OBJTYPE* op;
	OBJTYPE* wop;
	CMDTYPE* cmp;
	PNTTYPE  pv, zr, cp, sp, ep, pt;
	BOXTYPE  box;
	CString  str;
	int      i, j, k, f, i1, i2, i3, i4, w1, w2, mi2, ma2, cnt=0;
	double   a, b, t1, t2, sln, mi1, ma1, dd;

	i1 = pln;															// 座標軸設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	zr.p[0] = 0.0, zr.p[1] = 0.0, zr.p[2] = 0.0;
	pv = zr, pv.p[i3] = -1.0;											// 回転軸ﾍﾞｸﾄﾙ設定
	BaseObjPtr(2, &op, &w1, &w2);										// 処理対象立体取得
	while(op!=NULL) {
		if(cnt>=View->m_MaxSCtl) {										// <最大配列数以上>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// ｴﾗｰﾒｯｾｰｼﾞ
			AfxMessageBox(str);
			break;
		}
		if(op->part==2) {												// <ﾕﾆｯﾄ･子の場合>
			wop = op->oset;												// ﾕﾆｯﾄ･親とﾍﾟｱ
			if(wop==NULL) {
				wop = op;
			} else if(wop->edtflg==0) {
				wop = op;
			}
		} else {														// <ﾕﾆｯﾄ･子以外の場合>
			wop = op;
		}
		GetObjBox(wop, &box);											// 立体ﾎﾞｯｸｽ取得
		BoxCenter(box, &cp);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		wc[cnt] = cp;													// 立体中心点保存
		if(ifg) {														// <<初期処理>>
			oia[cnt] = GetObjNo(op);									// 立体No設定
			SetObjPtr(op);												// 立体ﾎﾟｲﾝﾀ設定
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保[移動用]
			PutParaInt(cmp, 0, 0);										// 移動量設定
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保[XY回転用]
			PutParaInt(cmp, 0, 0);										// 回転角度設定
			PutParaPnt(cmp, 0, zr);										// 回転軸座標設定
			PutParaPnt(cmp, 1, pv);										// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
		}
		cnt++;
		NextObjPtr(2, &op, &w1, &w2);									// 次立体へ
	}
	*on = cnt;															// 対象立体数
	for(ln[0]=0.0, sln=0.0, i=1; i<in; i++) {
		sln += PntD(&ip[i-1], &ip[i]);									// 合計線分距離
		ln[i] = sln;													// 累積線分距離
	}
	if(flg==0) {														// <基準線[開]の場合>
		sp = ip[0], ep = ip[in-1];										// 線分始点･終点
		i4 = (fabs(sp.p[i1]-ep.p[i1])>fabs(sp.p[i2]-ep.p[i2])) ? i1:i2;	// 最大差軸方向
		if(sp.p[i4]<ep.p[i4]) {											// 基準線最小最大取得
			mi1 = sp.p[i4], mi2 = 0, ma1 = ep.p[i4], ma2 = in-1;
		} else {
			mi1 = ep.p[i4], mi2 = in-1, ma1 = sp.p[i4], ma2 = 0;
		}
		if(fabs(sp.p[i1]-ep.p[i1])<EPSILON) {							// <i1軸平行線分>
			f = 1, a = sp.p[i1], b = 0.0;
		} else if(fabs(sp.p[i2]-ep.p[i2])<EPSILON) {					// <i2軸平行線分>
			f = 2, a = 0.0, b = sp.p[i2];
		} else {														// <i1,i2軸非平行線分>
			a = (ep.p[i2]-sp.p[i2])/(ep.p[i1]-sp.p[i1]);				// 線分傾き
			if(fabs(a)<EPSILON) {										// 傾きが殆どi2軸と平行
				f = 2, a = 0.0, b = sp.p[i2];
			} else {
				b = (ep.p[i1]-sp.p[i1])/(ep.p[i2]-sp.p[i2]);			// 線分逆傾き
				if(fabs(b)<EPSILON) {									// 傾きが殆どi1軸と平行
					f = 1, a = sp.p[i1], b = 0.0;
				} else {												// i1,i2と完全に非平行
					f = 0, b = sp.p[i2]-a*sp.p[i1];						// 線分定数
				}
			}
		}
		for(i=0; i<cnt; i++) {
			CalcLinePoint(pln, f, a, b, &wc[i], &pt);					// 線分交点算出
			pt.p[i3] = sp.p[i3];										// 平面垂直方向座標
			if(pt.p[i4]<=mi1) {											// <交点基準線最小以下>
				oib[i] = (int)(ln[mi2]/sln*100000.0);					// 線分比率設定
				opa[i] = ip[mi2];										// 線分交点設定
			} else if(pt.p[i4]>=ma1) {									// <交点基準線最大以上>
				oib[i] = (int)(ln[ma2]/sln*100000.0);
				opa[i] = ip[ma2];
			} else {													// <交点基準線内>
				dd = pt.p[i4];
				for(j=0; j<in-1; j++) {
					if((ip[j].p[i4]<=dd&&dd<ip[j+1].p[i4])||
						(ip[j+1].p[i4]<dd&&dd<=ip[j].p[i4])) {			// 該当線分内存在
						t1 = dd-ip[j].p[i4];
						t2 = ip[j+1].p[i4]-ip[j].p[i4];
						t1 = t1/t2;										// 線分比率
						if(t1<EPSILON) {								// 比率補正
							t1 = 0.0;
						} else if(fabs(t1-1.0)<EPSILON) {
							j++;
							t1 = 0.0;
						}
						t2 = PntD(&ip[j], &ip[j+1])*t1;					// 線分距離
						oib[i] = (int)((ln[j]+t2)/sln*100000.0);		// 線分比率設定
						opa[i] = pt;									// 線分交点設定
						break;
					}
				}
			}
		}
	} else {															// <基準円/基準線[閉]の場合>
		for(i=0; i<cnt; i++) {
			for(a=DBL_MAX, k=0, j=0; j<in-1; j++) {						// 楕円の最短垂線交点検索
				b = DistPointLine(&ip[j], &ip[j+1], &wc[i], &w1, &t1, &pt);
				if(b<a) {
					a = b, k = j, w2 = w1, t2 = t1, pv = pt;
				}
			}
			oib[i] = (int)((ln[k]+t2)/sln*100000.0);					// 線分比率設定
			opa[i] = pv;												// 線分交点設定
		}
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	delete[] wc;
	delete[] ln;
}

/*********************/
void EndBasePos(void)													//<<< 基準移動終了処理
/*********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th, dsp, edt;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		del = 0;
		if(GetCmdNo(cmp)==MOVE) {										// <移動ｺﾏﾝﾄﾞ>
			dx = GetParaInt(cmp, 0);									// 移動量取得
			dy = GetParaInt(cmp, 1);
			dz = GetParaInt(cmp, 2);
			del = (dx==0&&dy==0&&dz==0) ? 1 : del;
		} else if(GetCmdNo(cmp)==ROTATE) {								// <回転ｺﾏﾝﾄﾞ>
			th = GetParaInt(cmp, 0);									// 回転角度取得
			del = (th==0) ? 1 : del;
		}
		if(del) {
			dcmp = cmp;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		if(del) {
			GetCmdObj(dcmp, &op);										// 編集対象立体取得
			FreeCmd(op, dcmp, 1);										// 無効ｺﾏﾝﾄﾞ削除
		}
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// 処理対象立体取得
	while(op!=NULL) {
		RedoCommand(op);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &op, &dsp, &edt);									// 次立体へ
	}
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
}

/*****************************************************************/
static void BasePosMove(int ono, double dx, double dy, double dz)		//<<< 基準移動
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// 編集対象立体取得
		if(GetObjNo(op)==ono&&GetCmdNo(cmp)==MOVE) {					// <対象立体移動ｺﾏﾝﾄﾞ>
			PutParaInt(cmp, 0, (int)(dx*1000.0));						// 移動量設定
			PutParaInt(cmp, 1, (int)(dy*1000.0));
			PutParaInt(cmp, 2, (int)(dz*1000.0));
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/*********************************************************/
static void BasePosRotate(int ono, double th, PNTTYPE cp)				//<<< 基準回転
/*********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// 処理対象ｺﾏﾝﾄﾞ取得
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// 編集対象立体取得
		if(GetObjNo(op)==ono&&GetCmdNo(cmp)==ROTATE) {					// <対象立体回転ｺﾏﾝﾄﾞ>
			PutParaInt(cmp, 0, (int)(th*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, cp);										// 回転軸座標設定
			break;
		}
		LevelNextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
	}
}

/*******************************************************/
void EditBasePos(int flg, int pln, int ufg, int on,
                 PNTTYPE* op, int nn, PNTTYPE* np,
                 int no, int* ia, int* ib, PNTTYPE* pa)					//<<< 基準移動編集処理
/*******************************************************/
{
	PNTTYPE* wnp = new PNTTYPE[100];
	double*  oln = new double[View->m_MaxCPnt];
	double*  nln = new double[View->m_MaxCPnt];
	OBJTYPE* obj;
	PNTTYPE  opt, npt, mv, p0, p1, p2, p3, p4;
	VECTYPE  vec;
	int      i, j, i0, i1, i2, oa, na, os, ns;
	double   t1, t2, t3, t4, ol, nl, l1, l2, dd;

	p0.p[0] = 0.0, p0.p[1] = 0.0, p0.p[2] = 0.0;						// 作業領域初期化
	for(oln[0]=0.0, ol=0.0, i=1; i<on; i++) {							// <旧線分>
		l1 = PntD(&op[i-1], &op[i]);									// 線分距離
		ol += l1;														// 合計線分距離
		oln[i] = ol;													// 累積線分距離
	}
	if(flg==1) {														// <<基準円の場合>>
		i1 = (pln+1>2) ? 0 : pln+1;										// 座標軸設定
		i2 = (i1+1>2) ? 0 : i1+1;
		for(i=0; i<nn; i++) {
			wnp[i] = np[i];												// 新線分座標保存
			np[i].p[i2] = op[i].p[i2];									// i2軸は旧線分を使用
		}
		for(nln[0]=0.0, nl=0.0, i=1; i<nn; i++) {						// <新線分>
			l2 = PntD(&np[i-1], &np[i]);								// 線分距離
			nl += l2;													// 合計線分距離
			nln[i] = nl;												// 累積線分距離
		}
	} else {															// <<基準線[開]/基準線[閉]の場合>>
		for(nln[0]=0.0, nl=0.0, i=1; i<nn; i++) {						// <新線分>
			l2 = PntD(&np[i-1], &np[i]);								// 線分距離
			nl += l2;													// 合計線分距離
			nln[i] = nl;												// 累積線分距離
		}
	}
	for(i=0; i<no; i++) {
		opt = pa[i];													// 初期基準位置
		l1 = ib[i]/100000.0;											// 線分比率(旧)
		l2 = (ufg) ? l1*ol/nl : l1;										// 線分比率(新)
		if(l1<EPSILON) {												// 線分No
			oa = 0, os = 1;
		} else if(fabs(l1-1.0)<EPSILON) {
			oa = on-1, os = 1;
		} else {
			for(j=1; j<on; j++) {										// <旧線分>
				if(oln[j]/ol>l1) {
					break;
				}
			}
			oa = j-1;													// 該当位置線分No
			if(fabs(l1-oln[oa]/ol)<EPSILON)	{							// 始点近似
				os = 1;
			} else if(fabs(l1-oln[oa+1]/ol)<EPSILON) {					// 終点近似は次線分
				os = 1, oa++;
			} else {													// 線分中間
				os = 0;
			}
		}
		if(l2<EPSILON) {												// 線分No
			na = 0, ns = 1;
		} else if(fabs(l2-1.0)<EPSILON) {
			na = nn-1, ns = 1;
		} else {
			for(j=1; j<nn; j++) {										// <新線分>
				if(nln[j]/nl>l2) {
					break;
				}
			}
			na = j-1;													// 該当位置線分No
			if(fabs(l2-nln[na]/nl)<EPSILON) {							// 始点近似
				ns = 1;
			} else if(fabs(l2-nln[na+1]/nl)<EPSILON) {					// 終点近似は次線分
				ns = 1, na++;
			} else {													// 線分中間
				ns = 0;
			}
		}
		if(ns==1) {														// <線分端の場合>
			npt = np[na];												// 現在基準位置
			if(flg==1) {												// <<基準円の場合>>
				npt = wnp[na];											// 現在基準位置
			}
		} else {														// <線分間の場合>
			l2 = nl*l2-nln[na];											// 該当線分上距離
			VecV(&np[na], &np[na+1], &vec);								// 線分方向ﾍﾞｸﾄﾙ
			VecN(&vec);													// 単位ﾍﾞｸﾄﾙ化
			for(j=0; j<DIM; j++) {
				npt.p[j] = np[na].p[j]+l2*vec.p[j];						// 現在基準位置
			}
			if(flg==1) {												// <<基準円の場合>>
				VecV(&wnp[na], &wnp[na+1], &vec);						// 線分方向ﾍﾞｸﾄﾙ
				VecN(&vec);												// 単位ﾍﾞｸﾄﾙ化
				for(j=0; j<DIM; j++) {
					npt.p[j] = wnp[na].p[j]+l2*vec.p[j];				// 現在基準位置
				}
			}
		}
		for(j=0; j<DIM; j++) {
			mv.p[j] = npt.p[j]-opt.p[j];								// 移動量算出
		}
		BasePosMove(ia[i], mv.p[0], mv.p[1], mv.p[2]);					// 基準移動
		if(os==1) {														// <<<旧線分端の場合>>>
			if(oa>0&&oa<on-1) {											// <<旧中間線分の場合>>
				i0 = oa, i1 = oa+1, i2 = oa-1;
			} else {													// <<旧両端線分の場合>>
				if(flg==0) {											// <基準線[開]の場合>
					if(oa==0) {
						i0 = 0, i1 = 1, i2 = -1;
					} else {
						i0 = on-2, i1 = on-1, i2 = -1;
					}
				} else {												// <基準円/基準線[閉]の場合>
					i0 = 0, i1 = 1, i2 = on-2;
				}
			}
		} else {														// <<<旧線分間の場合>>>
			i0 = oa, i1 = oa+1, i2 = -1;
		}
		for(j=0; j<DIM; j++) {
			p1.p[j] = op[i1].p[j]-op[i0].p[j];							// 基準座標設定
		}
		if(i2!=-1) {
			for(j=0; j<DIM; j++) {
				p2.p[j] = op[i2].p[j]-op[i0].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				p2.p[j] = -p1.p[j];
			}
		}
		if((t3=CalcBaseAngle(p0, p1, pln))<0) {							// 基準角度取得
			t3 += 2.0*PI;
		}
		if((t4=CalcBaseAngle(p0, p2, pln))<0) {
			t4 += 2.0*PI;
		}
		t1 = (t3+t4)/2.0;
		if(t3>PI) {
			if(t1<t3-PI||t1>t3) {
				t1 = (t1-PI<0) ? t1-PI+(2.0*PI) : t1-PI;				// 線分左側に補正
			}
		} else {
			if(t1>t3&&t1<t3+PI) {
				t1 = (t1-PI<0) ? t1-PI+(2.0*PI) : t1-PI;
			}
		}
		if(ns==1) {														// <<<新線分端の場合>>>
			if(na>0&&na<nn-1) {											// <<新中間線分の場合>>
				i0 = na, i1 = na+1, i2 = na-1;
			} else {													// <<新両端線分の場合>>
				if(flg==0) {											// <基準線[開]の場合>
					if(na==0) {
						i0 = 0, i1 = 1, i2 = -1;
					} else {
						i0 = nn-2, i1 = nn-1, i2 = -1;
					}
				} else {												// <基準円/基準線[閉]の場合>
					i0 = 0, i1 = 1, i2 = nn-2;
				}
			}
		} else {														// <<<新線分間の場合>>>
			i0 = na, i1 = na+1, i2 = -1;
		}
		for(j=0; j<DIM; j++) {
			p3.p[j] = np[i1].p[j]-np[i0].p[j];							// 基準座標設定
		}
		if(i2!=-1) {
			for(j=0; j<DIM; j++) {
				p4.p[j] = np[i2].p[j]-np[i0].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				p4.p[j] = -p3.p[j];
			}
		}
		if((t3=CalcBaseAngle(p0, p3, pln))<0) {							// 基準角度取得
			t3 += 2.0*PI;
		}
		if((t4=CalcBaseAngle(p0, p4, pln))<0) {
			t4 += 2.0*PI;
		}
		t2 = (t3+t4)/2.0;
		if(t3>PI) {
			if(t2<t3-PI||t2>t3) {
				t2 = (t2-PI<0) ? t2-PI+(2.0*PI) : t2-PI;				// 線分左側に補正
			}
		} else {
			if(t2>t3&&t2<t3+PI) {
				t2 = (t2-PI<0) ? t2-PI+(2.0*PI) : t2-PI;
			}
		}
		dd = 180.0*(t1-t2)/PI;											// 変位角度
		BasePosRotate(ia[i], dd, npt);									// 基準回転
	}
	BaseObjPtr(2, &obj, &i1, &i2);										// 処理対象立体取得
	while(obj!=NULL) {
		RedoCommand(obj);												// ｺﾏﾝﾄﾞ再実行
		View->SetDispList(obj);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		NextObjPtr(2, &obj, &i1, &i2);									// 次立体へ
	}
	if(flg==1) {														// <<基準円の場合>>
		for(i=0; i<nn; i++) {
			np[i] = wnp[i];												// 新線分座標復元
		}
	}
	delete[] wnp;
	delete[] oln;
	delete[] nln;
}

/*************************************************************/
static void InitLinesEdit(int d1, int d2, int d3, PNTTYPE sp,
                          PNTTYPE ep, int on, PNTTYPE* cpt,
                          double* len, PNTTYPE* crs)					//<<< 曲線指定移動初期処理
/*************************************************************/
{
	int    i, f, md;
	double a, b;

	if(on<1) {															// 立体数0以下は中止
		return;
	}
	md = (fabs(sp.p[d1]-ep.p[d1])>fabs(sp.p[d2]-ep.p[d2])) ? d1 : d2;	// 最大差軸方向(md)
	if(fabs(sp.p[d1]-ep.p[d1])<EPSILON) {								// <d1軸平行線分>
		f = 1, a = sp.p[d1], b = 0.0;
	} else if(fabs(sp.p[d2]-ep.p[d2])<EPSILON) {						// <d2軸平行線分>
		f = 2, a = 0.0, b = sp.p[d2];
	} else {															// <d1,d2軸非平行線分>
		a = (ep.p[d2]-sp.p[d2])/(ep.p[d1]-sp.p[d1]);					// 線分傾き
		if(fabs(a)<EPSILON) {											// 傾きが殆どd2軸と平行
			f = 2, a = 0.0, b = sp.p[d2];
		} else {
			b = (ep.p[d1]-sp.p[d1])/(ep.p[d2]-sp.p[d2]);				// 線分逆傾き
			if(fabs(b)<EPSILON) {										// 傾きが殆どd1軸と平行
				f = 1, a = sp.p[d1], b = 0.0;
			} else {													// d1,d2と完全に非平行
				f = 0, b = sp.p[d2]-a*sp.p[d1];
			}
		}
	}
	for(i=0; i<on; i++) {
		CalcLinePoint(d1, f, a, b, &cpt[i], &crs[i]);					// 線分交点算出
		crs[i].p[d3] = sp.p[d3];										// 平面垂直方向座標
		len[i] = PntD(&sp, &crs[i]);									// 基準線始点との距離
	}
}

/********************************************************************/
static void ObjLinesEdit(OBJTYPE* op, int d1, int d2, int d3,
                         int uf, PNTTYPE sp, PNTTYPE ep, double oln,
                         int on, double* len, PNTTYPE* crs)				//<<< 曲線指定移動(1曲線毎)
/********************************************************************/
{
	PNTTYPE* cv = new PNTTYPE[View->m_MaxCPnt];
	double*  fta = new double[View->m_MaxCPnt];
	double*  vta = new double[View->m_MaxCPnt];
	double*  vln = new double[View->m_MaxCPnt];
	OBJTYPE* wop;
	OBJTYPE* cop;
	CMDTYPE* cmp;
	PNTTYPE  wp, ncrs, nsp, nep, stp, pvc;
	VECTYPE  svc, wvc, wv1, wv2, wv3, wv4;
	int      i, p, vn, st, ed, fg=0, ix=0, pm=0, ct=0;
	double   ln, nln, wln, fth, vth, w1, w2, th1, th2;

	vn = GetVtxNum(op);													// 立体頂点数取得
	for(i=0; i<vn; i++) {
		GetVtx(op, i, &cv[i]);											// 立体頂点取得
	}
	if(PntD(&wp, &cv[0])>PntD(&sp, &cv[vn-1])) {						// <曲線終点が基準線始点に近い>
		st = 0, ed = vn-1;												// 始点/終点番号
		while(st<ed) {													// <終点<始点まで繰返す>
			wp = cv[st], cv[st] = cv[ed], cv[ed] = wp;					// 始点側･終点側を入替え
			ed--, st++;													// 終点減算/始点加算
		}
	}
	for(vln[0]=0.0, nln=0.0, i=1; i<vn; i++) {
		nln += PntD(&cv[i-1], &cv[i]);									// 合計線分距離
		vln[i] = nln;													// 線分毎累積距離
	}
	CalcNVec(cv, vn, &wvc);												// 曲線法線ﾍﾞｸﾄﾙ取得
	if(fabs(wvc.p[2])<EPSILON) {										// <曲線が垂直に存在する場合>
		fg = 1;															// 曲線垂直ﾌﾗｸﾞON
		ix = (fabs(wvc.p[0])>fabs(wvc.p[1])) ? 1 : ix;					// 基準方向設定
	}
	VecV(&sp, &ep, &svc);												// 基準線ﾍﾞｸﾄﾙ取得
	VecN(&svc);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(svc.p[0])>EPSILON||fabs(svc.p[1])>EPSILON) {				// 基準線!=Z軸の場合は
		fth = atan2(svc.p[1], svc.p[0]);								// 基準線水平角度
		fth = (fth<-10||fth>10) ? 0.0 : fth;
		vth = sqrt(svc.p[0]*svc.p[0]+svc.p[1]*svc.p[1]);				// 基準線水平方向長
		vth = atan2(svc.p[2], vth);										// 基準線垂直角度
		vth = (vth<-10||vth>10) ? 0.0 : vth;
	} else {															// 基準線=Z軸の場合は
		fth = 0.0;														// 基準線水平角度
		vth = (svc.p[2]>0) ? PI/2.0 : -PI/2.0;							// 基準線垂直角度
		svc.p[0] = 0.0, svc.p[1] = 1.0, svc.p[2] = 0.0;					// 回転軸用に仮ﾍﾞｸﾄﾙ設定
	}
	VecV(&cv[0], &cv[1], &wvc);											// 曲線線分ﾍﾞｸﾄﾙ取得
	VecN(&wvc);															// 単位ﾍﾞｸﾄﾙ化
	if(fabs(wvc.p[0])>EPSILON||fabs(wvc.p[1])>EPSILON) {				// 曲線線分!=Z軸の場合は
		wv1 = wvc, wv1.p[2] = 0;										// 水平方向のみ抽出
		VecN(&wv1);														// 単位ﾍﾞｸﾄﾙ化
	} else {															// Z軸の場合は角度=0のﾍﾞｸﾄﾙ
		wv1.p[0] = 1.0, wv1.p[1] = 0.0, wv1.p[2] = 0.0;
	}
	w1 = sqrt(wvc.p[0]*wvc.p[0]+wvc.p[1]*wvc.p[1]);						// 曲線線分水平方向長
	wv2.p[0] = w1, wv2.p[1] = wvc.p[2], wv2.p[2] = 0.0;					// 垂直方向をXY平面に投影
	VecN(&wv2);															// 単位ﾍﾞｸﾄﾙ化
	if(fg) {															// <曲線が垂直の場合>
		if(wv1.p[ix]>EPSILON) {											// 水平基準方向:正
			pm = 1;
		} else if(wv1.p[ix]<-EPSILON) {									// 水平基準方向:負
			pm = -1;
		}
	}
	fta[1] = atan2(wv1.p[1], wv1.p[0]);									// 曲線線分水平角度
	fta[1] = (fta[1]<-10||fta[1]>10) ? 0.0 : fta[1];
	vta[1] = atan2(wv2.p[1], wv2.p[0]);									// 曲線線分垂直角度
	vta[1] = (vta[1]<-10||vta[1]>10) ? 0.0 : vta[1];
	for(i=2; i<vn; i++) {
		VecV(&cv[i-1], &cv[i], &wvc);									// 曲線線分ﾍﾞｸﾄﾙ取得
		VecN(&wvc);														// 単位ﾍﾞｸﾄﾙ化
		if(fabs(wvc.p[0])>EPSILON||fabs(wvc.p[1])>EPSILON) {			// 曲線線分!=Z軸の場合は
			wv3 = wvc, wv3.p[2] = 0;									// 水平方向のみ抽出
			VecN(&wv3);													// 単位ﾍﾞｸﾄﾙ化
		} else {														// Z軸の場合は角度=0のﾍﾞｸﾄﾙ
			wv3.p[0] = 1.0, wv3.p[1] = 0.0, wv3.p[2] = 0.0;
		}
		w1 = sqrt(wvc.p[0]*wvc.p[0]+wvc.p[1]*wvc.p[1]);					// 曲線線分水平方向長
		wv4.p[0] = w1, wv4.p[1] = wvc.p[2], wv4.p[2] = 0.0;				// 垂直方向をXY平面に投影
		VecN(&wv4);														// 単位ﾍﾞｸﾄﾙ化
		if(fg&&pm==0) {													// <曲線:垂直で基準:未の場合>
			if(wv3.p[ix]>EPSILON) {										// 水平基準方向:正
				pm = 1;
			} else if(wv3.p[ix]<-EPSILON) {								// 水平基準方向:負
				pm = -1;
			}
		}
		w1 = atan2(wv1.p[1], wv1.p[0]);									// 前線分の水平角度
		w1 = (w1<-10||w1>10) ? 0.0 : w1;
		w2 = atan2(wv3.p[1], wv3.p[0]);									// 今回線分の水平角度
		w2 = (w2<-10||w2>10) ? 0.0 : w2;
		w2 -= w1;														// 前線分との水平角度差
		w2 = (fabs(w2)>PI-EPSILON&&fabs(w2)<PI+EPSILON) ? 0.0 : w2;		// 180度反転は0度
		fta[i] = fta[i-1]+w2;											// 水平角度を加算
		w1 = atan2(wv2.p[1], wv2.p[0]);									// 前線分の垂直角度
		w1 = (w1<-10||w1>10) ? 0.0 : w1;
		w2 = atan2(wv4.p[1], wv4.p[0]);									// 今回線分の垂直角度
		w2 = (w2<-10||w2>10) ? 0.0 : w2;
		if(fg) {														// <<曲線が垂直の場合>>
			if(pm==1&&wv1.p[ix]<-EPSILON) {								// <基準:正でﾍﾞｸﾄﾙ:負>
				w1 = (w1>0.0) ? PI-w1 : -PI-w1;							// 角度変換
			} else if(pm==-1&&wv1.p[ix]>EPSILON) {						// <基準:負でﾍﾞｸﾄﾙ:正>
				w1 = (w1>0.0) ? PI-w1 : -PI-w1;							// 角度変換
			}
			if(pm==1&&wv3.p[ix]<-EPSILON) {								// <基準:正でﾍﾞｸﾄﾙ:負>
				w2 = (w2>0.0) ? PI-w2 : -PI-w2;							// 角度変換
			} else if(pm==-1&&wv3.p[ix]>EPSILON) {						// <基準:負でﾍﾞｸﾄﾙ:正>
				w2 = (w2>0.0) ? PI-w2 : -PI-w2;							// 角度変換
			}
		}
		w2 -= w1;														// 前線分との垂直角度差
		w2 = (fabs(w2)>PI-EPSILON&&fabs(w2)<PI+EPSILON) ? 0.0 : w2;		// 180度反転は0度
		vta[i] = vta[i-1]+w2;											// 垂直角度を加算
		wv1 = wv3, wv2 = wv4;											// 今回線分ﾍﾞｸﾄﾙ保存
	}
	SelBaseObjPtr(2, &wop);												// 編集対象立体取得
	while(wop!=NULL) {
		if(ct>=on) {													// 最大立体数で終了
			break;
		}
		if(uf) {														// <立体間距離変更あり>
			wln = len[ct]*nln/oln;
		} else {														// <立体間距離変更なし>
			wln = len[ct];
		}
		if(wln<0) {														// <基準線始点未満の場合>
			p = 1, nsp = cv[0], nep = cv[1], stp = nsp;					// 曲線対応の新線分
			ln = wln;													// 基準線始点をｵｰﾊﾞｰする長さ
		} else if(wln>vln[vn-1]) {										// <基準線終点以上の場合>
			p = vn-1, nsp = cv[vn-2], nep = cv[vn-1], stp = nep;		// 曲線対応の新線分
			ln = wln-vln[vn-1];											// 基準線終点をｵｰﾊﾞｰする長さ
		} else {														// <基準線の始点～終点の場合>
			for(p=1; p<vn-1; p++) {										// 該当線分検索
				if(wln<vln[p]) {
					break;
				}
			}
			nsp = cv[p-1], nep = cv[p], stp = nsp;						// 曲線対応の新線分
			ln = wln-vln[p-1];											// 線分始点をｵｰﾊﾞｰする長さ
		}
		VecV(&nsp, &nep, &wvc);											// 曲線線分ﾍﾞｸﾄﾙ取得
		VecN(&wvc);														// 単位ﾍﾞｸﾄﾙ化
		for(i=0; i<DIM; i++) {
			ncrs.p[i] = stp.p[i]+ln*wvc.p[i];							// 曲線対応の新交点位置
		}
		for(i=0; i<DIM; i++) {
			ncrs.p[i] -= crs[ct].p[i];									// 新交点への移動量
		}
		th1 = 180.0*(fth-fta[p])/PI;									// 水平回転角度
		th2 = 180.0*(vth-vta[p])/PI;									// 垂直回転角度
		SetObjPtr(wop);													// 複写対象立体設定
		cop = CopyObject();												// 立体複写
		if(fabs(th2)>EPSILON) {											// <垂直有効回転角度あり>
			wvc.p[0] = -svc.p[1], wvc.p[1] = svc.p[0], wvc.p[2] = 0.0;	// 基準線の直交ﾍﾞｸﾄﾙ
			VecN(&wvc);													// 単位ﾍﾞｸﾄﾙ化
			for(i=0; i<DIM; i++) {
				pvc.p[i] = wvc.p[i];									// 垂直方向回転軸ﾍﾞｸﾄﾙ
			}
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(th2*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, crs[ct]);								// 回転軸座標設定
			PutParaPnt(cmp, 1, pvc);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetObjPtr(cop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
		}
		if(fabs(th1)>EPSILON) {											// <水平有効回転角度あり>
			pvc.p[0] = 0.0, pvc.p[1] = 0.0, pvc.p[2] = -1.0;			// 水平方向回転軸ﾍﾞｸﾄﾙ
			CreatCmd(ROTATE, 1, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(th1*1000.0));						// 回転角度設定
			PutParaPnt(cmp, 0, crs[ct]);								// 回転軸座標設定
			PutParaPnt(cmp, 1, pvc);									// 回転軸方向ﾍﾞｸﾄﾙ設定
			SetObjPtr(cop);												// 対象立体設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 回転ｺﾏﾝﾄﾞ実行
		}
		if(fabs(ncrs.p[0])>EPSILON||
           fabs(ncrs.p[1])>EPSILON||
           fabs(ncrs.p[2])>EPSILON) {									// <有効移動量あり>
			CreatCmd(MOVE, 3, 0, &cmp);									// ｺﾏﾝﾄﾞ領域確保
			PutParaInt(cmp, 0, (int)(ncrs.p[0]*1000.0));				// 移動量設定
			PutParaInt(cmp, 1, (int)(ncrs.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(ncrs.p[2]*1000.0));
			SetObjPtr(cop);												// 立体ﾎﾟｲﾝﾀ設定
			SetCmdPtr(cmp);												// ｺﾏﾝﾄﾞ設定
			ExecCommand(FALSE);											// 移動ｺﾏﾝﾄﾞ実行
		}
		View->SetDispList(cop);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		SelNextObjPtr(2, &wop);											// 次立体へ
		ct++;
	}
	delete[] cv;
	delete[] fta;
	delete[] vta;
	delete[] vln;
}

/****************************************************/
void ObjectLinesEdit(int d1, int d2, int d3, int uf,
                     PNTTYPE sp, PNTTYPE ep)							//<<< 曲線指定移動処理
/****************************************************/
{
	PNTTYPE* cp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cr = new PNTTYPE[View->m_MaxSCtA];
	double*  ln = new double[View->m_MaxSCtA];
	OBJTYPE* op, * op2, * co[10];
	PNTTYPE  pt;
	BOXTYPE  box;
	CString  str;
	int      i, c1=0, c2=0;
	double   sln;

	SelBaseObjPtr(1, &op);												// 基準線曲線立体取得
	while(op!=NULL) {
		if(c1>=10) {													// <最大配列数以上>
			str.Format(IDS_ERR_PROCESS, 10);							// ｴﾗｰﾒｯｾｰｼﾞ
			AfxMessageBox(str);
			break;
		}
		co[c1++] = op;													// 基準線曲線立体OP保存
		SelNextObjPtr(1, &op);											// 次立体へ
	}
	SelBaseObjPtr(2, &op);												// 編集対象立体取得
	while(op!=NULL) {
		if(c2>=View->m_MaxSCtl) {										// <最大配列数以上>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// ｴﾗｰﾒｯｾｰｼﾞ
			AfxMessageBox(str);
			break;
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
		GetObjBox(op2, &box);											// 立体ﾎﾞｯｸｽ取得
		BoxCenter(box, &pt);											// ﾎﾞｯｸｽｾﾝﾀｰ取得
		cp[c2++] = pt;													// 立体中心点保存
		SelNextObjPtr(2, &op);											// 次立体へ
	}
	InitLinesEdit(d1, d2, d3, sp, ep, c2, cp, ln, cr);					// 曲線指定移動初期処理
	sln = PntD(&sp, &ep);												// 2点間距離(3D)
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	for(i=0; i<c1; i++) {
		ObjLinesEdit(co[i], d1, d2, d3, uf, sp, ep, sln, c2, ln, cr);	// 曲線指定移動(1曲線毎)
	}
	SelBaseObjPtr(2, &op);												// 編集対象立体取得
	while(op!=NULL) {
		op2 = op;														// 削除立体ﾎﾟｲﾝﾀ保存
		SelNextObjPtr(2, &op);											// 次立体へ
		LevelObj(1, op2);												// 同一ﾚﾍﾞﾙ削除立体接続
		TempObj(op2);													// 立体一時削除
	}
	LevelBaseObjPtr(0, &op);											// 生成立体取得
	while(op) {
		OnEdtFlg(1, op);												// edtflg:ON
		LevelNextObjPtr(0, &op);
	}
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	delete[] cp;
	delete[] cr;
	delete[] ln;
}
