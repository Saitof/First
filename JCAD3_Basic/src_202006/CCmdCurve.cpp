/////////////////////////////////////////////////////////////////////////////
// 立体生成 曲線生成コマンド部
// CCmdCurve.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"

/*******************************************************************/
static void SCurve(int* pn, int bc, int mx, double wt, PNTTYPE* pa)		//<<< 連続曲線生成
/*******************************************************************/
{
	PNTTYPE* rp = new PNTTYPE[View->m_MaxCPnt];
	int      rn, i, j, k;

	if(*pn<2) {															// 制御点数2未満中止
		delete[] rp;
		return;
	}
	rn = *pn;
	if(rn>2) {															// <制御点数3以上>
		for(i=0; i<mx; i++) {											// 指定回数分繰返
			for(k=0; k<DIM; k++) {										// 中間点先頭曲線計算
				rp[1].p[k] = (pa[0].p[k]+pa[1].p[k])/2.0;
			}
			for(j=0; j<rn-2; j++) {
				for(k=0; k<DIM; k++) {									// 先頭以外曲線計算
					rp[2*j+3].p[k] = (pa[j+1].p[k]+pa[j+2].p[k])/2.0;
					rp[2*j+2].p[k] = (rp[2*j+1].p[k]+rp[2*j+3].p[k]+
                                      wt*pa[j+1].p[k])/(wt+2.0);
				}
			}
			if(bc) {													// <境界が閉の場合>
				for(k=0; k<DIM; k++) {									// 始点曲線計算
					rp[0].p[k] = (rp[1].p[k]+rp[2*(rn-1)-1].p[k]+
                                  wt*pa[0].p[k])/(wt+2.0);
				}
				rp[2*(rn-1)] = rp[0];									// 終点は始点と同一
			} else {													// <境界が開の場合>
				rp[0] = pa[0], rp[2*(rn-1)] = pa[rn-1];					// 始点終点変更無
			}
			rn = 2*rn-1;												// 新制御点置換
			for(j=0; j<rn; j++) {
				pa[j] = rp[j];
			}
		}
	} else {															// <制御点数2以下>
		for(i=0; i<mx; i++) {											// 指定回数分繰返
			for(j=0; j<rn-1; j++) {
				rp[2*j] = pa[j];
				for(k=0; k<DIM; k++) {									// 中間点計算
					rp[2*j+1].p[k] = (pa[j].p[k]+pa[j+1].p[k])/2.0;
				}
			}
			rp[2*(rn-1)] = pa[rn-1];
			rn = 2*rn-1;												// 新制御点置換
			for(j=0; j<rn; j++) {
				pa[j] = rp[j];
			}
		}
	}
	*pn = rn;
	delete[] rp;
}

/************************************************************************/
int MakeSCurve(int* pn, int bc, int mx, double wt, PNTTYPE* pa, int* ea)//<<< S曲線生成
/************************************************************************/
{
	PNTTYPE* wpa = new PNTTYPE[View->m_MaxCPnt];
	int*     wea = new int[View->m_MaxCPnt];
	PNTTYPE  spa;
	int      sea, cn, en, wn, sn, nn, i, j, cnt=0;

	if(*pn<2) {
		delete[] wpa;
		delete[] wea;
		return cnt;														// 制御点数2未満は中止
	}
	cn = *pn;
	if(bc) {															// <境界が閉の場合>
		for(i=0, en=0; i<cn; i++) {
			if(ea[i]==1) {												// 折線点数確認
				en++;
			}
		}
		if(en>0) {														// <折線点有の場合>
			while(1) {													// 先頭が折線点まで繰返
				if(ea[0]==1) {
					break;
				}
				spa = pa[0], sea = ea[0];								// 先頭ﾃﾞｰﾀ保存
				for(i=0; i<cn-1; i++) {
					pa[i] = pa[i+1], ea[i] = ea[i+1];					// 1つ前にｼﾌﾄ
				}
				pa[cn-1] = spa, ea[cn-1] = sea;							// 最後に旧先頭設定
				cnt++;													// ﾃﾞｰﾀｼﾌﾄ回数ｶｳﾝﾄ
			}
		}
		pa[cn] = pa[0], ea[cn++] = ea[0];								// 最後に先頭追加
	} else {															// <境界が開の場合>
		en = 2;															// 折線点は2以上
		ea[0] = 1, ea[cn-1] = 1;										// 先頭&最後=折線点
	}
	if(en==0) {															// <折線点無の場合>
		for(i=0; i<cn; i++) {
			wpa[i] = pa[i];												// 作業領域制御点保存
		}
		SCurve(&cn, bc, mx, wt, wpa);									// 連続曲線生成
		for(i=0; i<cn; i++) {
			wea[i] = 0;													// 全て曲線点ﾓｰﾄﾞ
		}
	} else {															// <折線点有の場合>
		wn = 0, sn = 0;
		while(1) {
			for(i=sn, nn=0; i<cn; i++) {								// 先頭から順次行う
				wpa[wn+(nn++)] = pa[i];									// 2点の折線点間の
				if(i!=sn&&ea[i]==1) {									// 制御点抽出
					sn = i; break;
				}
			}
			if(i==cn) {													// 最後で終了
				break;
			}
			SCurve(&nn, 0, mx, wt, &wpa[wn]);							// 連続曲線生成
			for(i=wn+1; i<wn+nn-1; i++) {								// 中間点は曲線点ﾓｰﾄﾞ
				wea[i] = 0;
			}
			wn = wn+nn-1;												// 折線点は前後で共通使用
			wea[wn] = 1;												// 最後点は折線点ﾓｰﾄﾞ
		}
		cn = wn+1;
	}
	for(i=0; i<cn; i++) {
		pa[i] = wpa[i];													// 作業領域より正規設定
	}
	for(i=1; i<cn-1; i++) {
		if(wea[i]!=1) {
			for(j=0; j<DIM; j++) {										// 曲線点収束
				pa[i].p[j] = (wpa[i-1].p[j]+wpa[i+1].p[j]+
                              (wt+2.0)*wpa[i].p[j])/(wt+4.0);
			}
		}
	}
	if(bc&&en==0) {														// <境界:閉で全曲線の場合>
		for(j=0; j<DIM; j++) {											// 2点目&最終-1点目で収束
			pa[0].p[j] = (wpa[cn-2].p[j]+wpa[1].p[j]+
                          (wt+2.0)*wpa[0].p[j])/(wt+4.0);
		}
		pa[cn-1] = pa[0];												// 始点と終点は同一
	}
	*pn = cn;
	delete[] wpa;
	delete[] wea;
	return cnt;															// ﾃﾞｰﾀｼﾌﾄ回数を返す
}

/************************/
OBJTYPE* MakeCurve(void)												//<<< 曲線生成ｺﾏﾝﾄﾞ
/************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc, mx, wt;
	double   dwt;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	pn = GetParaInt(cmp, 0);											// 制御点数取得
	bc = GetParaInt(cmp, 1);											// 境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 2);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 3);											// 曲線重み係数
	for(i=0; i<pn; i++) {
		ea[i] = GetParaInt(cmp, 4+i);									// ﾗｲﾝﾓｰﾄﾞ取得
		GetParaPnt(cmp, i, &pa[i]);										// 制御点取得
	}
	dwt = wt/1000.0;													// 重み係数実数変換
	MakeSCurve(&pn, bc, mx, dwt, pa, ea);								// S曲線生成
	CreatObj(pn, pn, 0, &op);											// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()) {												// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// 頂点座標設定
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// 面ﾙｰﾌﾟ情報設定
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// 属性No設定(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ea;
	return op;
}

/**************************/
OBJTYPE* ChangeCurve(void)												//<<< 曲線編集ｺﾏﾝﾄﾞ
/**************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc, mx, wt;
	double   dwt;

	GetObjPtr(&op);														// 処理対象立体取得
	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	pn = GetParaInt(cmp, 0);											// 制御点数取得
	bc = GetParaInt(cmp, 1);											// 境界ﾓｰﾄﾞ取得
	mx = GetParaInt(cmp, 2);											// 曲線化繰返数
	if(GetCurveReptSW()) {												// <曲面細分化オン>
		mx = GetCurveReptNum();											// 曲面分割数取得
	}
	wt = GetParaInt(cmp, 3);											// 曲線重み係数
	for(i=0; i<pn; i++) {
		ea[i] = GetParaInt(cmp, 4+i);									// ﾗｲﾝﾓｰﾄﾞ取得
		GetParaPnt(cmp, i, &pa[i]);										// 制御点取得
	}
	dwt = wt/1000.0;													// 重み係数実数変換
	MakeSCurve(&pn, bc, mx, dwt, pa, ea);								// S曲線生成
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	ChangeObj(op, pn, pn, 0);											// 立体領域変更
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	if(!GetCmdExeStp()) {												// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// 頂点座標設定
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// 面ﾙｰﾌﾟ情報設定
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// 属性No設定(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <ｺﾏﾝﾄﾞ停止無&RedoCommand以外>
		SetObjBox(op);													// ﾎﾞｯｸｽｻｲｽﾞ設定
	}
	delete[] pa;
	delete[] ea;
	return op;
}

/***********************/
OBJTYPE* SetCurve(void)													//<<< 曲線制御点生成ｺﾏﾝﾄﾞ
/***********************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA*2];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc;

	GetCmdPtr(&cmp);													// 処理対象ｺﾏﾝﾄﾞ取得
	pn = GetParaInt(cmp, 0);											// 制御点数取得
	bc = GetParaInt(cmp, 1);											// 境界ﾓｰﾄﾞ取得
	for(i=0; i<pn; i++) {
		GetParaPnt(cmp, i, &pa[i]);										// 制御点取得
	}
	if(bc) {															// 境界:閉は終点を始点
		pa[pn++] = pa[0];
	}
	CreatObj(pn, pn, 0, &op);											// 立体領域確保
	AppendCmd(op, cmp);													// 立体&ｺﾏﾝﾄﾞ接続
	LevelCmd(cmp);														// 同一ﾚﾍﾞﾙｺﾏﾝﾄﾞ接続
	LevelObj(0, op);													// 同一ﾚﾍﾞﾙ生成立体接続
	if(!GetCmdExeStp()) {												// <<<ｺﾏﾝﾄﾞ実行停止なし>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// 頂点座標設定
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// 面ﾙｰﾌﾟ情報設定
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// 属性No設定(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	PutObjPrt(op, 0, 0);												// ﾊﾟｰﾂNo設定(通常)
	PutObjCol(op, 0);													// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
	if(!GetCmdExeStp()) {												// <ｺﾏﾝﾄﾞ実行停止無>
		if(!GetResetObjMode()) {										// <RedoCommand以外>
			SetObjBox(op);												// ﾎﾞｯｸｽｻｲｽﾞ設定
		}
		OnEdtFlg(1, op);												// edtflg:ON
	}
	delete[] pa;
	return op;
}
