/////////////////////////////////////////////////////////////////////////////
// 曲面編集制御点操作部
// EditCtrlPoint1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "BackCommand.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/****************************************************************/
static void MoveCtrlPoint(OBJTYPE* op, PNTTYPE opt, PNTTYPE npt,
                          int mx, int flg, PNTTYPE* pc)					//<<< 制御点移動
/****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	CMDTYPE* cmp;
	PNTTYPE  wpt, spt, p1, p2, p3;
	int      i, i1, i2, i3, i4, i5, i6, i7, wf, ct, cno;
	double   d1, d2, d3, d4, d5, d6, d7, d8, d9;

	if(flg==1) {														// <移動対象の場合>
		while(mx>0) {
			wpt = opt;													// 立体編集前制御点
			BaseCmdPtr(1, op, &cmp);									// 有効先頭ｺﾏﾝﾄﾞ取得
			NextCmdPtr(&cmp);											// 曲線編集後ｺﾏﾝﾄﾞ
			ct = 0;														// ｺﾏﾝﾄﾞｶｳﾝﾀ初期化
			while(cmp!=NULL) {
				if(ct>mx-1) {											// ｺﾏﾝﾄﾞ個数以上中止
					break;
				}
				wf = (ct==mx-1) ? 1 : 0;								// 指定個数目更新有
				cno = GetCmdNo(cmp);									// ｺﾏﾝﾄﾞNo取得
				if(cno==MOVE) {											// <平行移動の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					BackMove(wf, &wpt, &npt, d1, d2, d3);				// 逆平行移動実行
				} else if(cno==ROTATE) {								// <回転移動の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// 回転角度取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// 回転軸ﾍﾞｸﾄﾙ取得
					BackRotate(wf, &wpt, &npt, p1, p2, d1);				// 逆回転移動実行
				} else if(cno==MIRROR) {								// <鏡像移動の場合>
					i1 = GetParaInt(cmp, 0);							// 対象平面取得
					GetParaPnt(cmp, 0, &p1);							// 鏡像軸の1点取得
					GetParaPnt(cmp, 1, &p2);							// 鏡像軸の1点取得
					BackMirror(wf, &wpt, &npt, p1, p2, i1);				// 逆鏡像移動実行
				} else if(cno==SCALE) {									// <ｻｲｽﾞ変更の場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向倍率取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向倍率取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// ｻｲｽﾞ変更中心点取得
					BackScale(wf, &wpt, &npt, p1, d1, d2, d3);			// 逆ｻｲｽﾞ変更実行
				} else if(cno==RESIZE1) {								// <直線指定ﾘｻｲｽﾞ1の場合>
					i1 = GetParaInt(cmp, 0);							// 指定平面取得
					i2 = GetParaInt(cmp, 1);							// 正負ﾌﾗｸﾞ取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 倍率1取得
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// 倍率2取得
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// 倍率3取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					GetParaPnt(cmp, 1, &p2);							// 基準点2取得
					GetParaPnt(cmp, 2, &p3);							// 基準点3取得
					BackResize1(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                p1, p2, p3);							// 逆直線指定ﾘｻｲｽﾞ1実行
				} else if(cno==RESIZE2) {								// <曲線指定ﾘｻｲｽﾞ1の場合>
					i1 = GetParaInt(cmp, 0);							// 正負ﾌﾗｸﾞ取得
					i2 = GetParaInt(cmp, 1);							// 両側ﾌﾗｸﾞ取得
					i3 = GetParaInt(cmp, 2);							// 長さ方向取得
					i4 = GetParaInt(cmp, 3);							// ｻｲｽﾞ指定方向取得
					d1 = (double)GetParaInt(cmp, 4)/1000.0;				// 基準長さ取得
					i5 = GetParaInt(cmp, 5);							// 制御点数取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					for(i=0; i<i5; i++) {
						ia[i] = GetParaInt(cmp, i+6);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i+1, &pa[i]);					// 曲線制御点取得
					}
					BackResize2(wf, &wpt, &npt, i1, i2, i3, i4, d1,
                                p1, i5, pa, ia);						// 逆曲線指定ﾘｻｲｽﾞ1実行
				} else if(cno==BEND) {									// <円筒曲げの場合>
					i1 = GetParaInt(cmp, 0);							// 曲げ水平方向取得
					i2 = GetParaInt(cmp, 1);							// 曲げ垂直方向取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 曲げ角度取得
					GetParaPnt(cmp, 0, &p1);							// 基準線点取得
					GetParaPnt(cmp, 1, &p2);							// 基準線点取得
					BackBend(wf, &wpt, &npt, i1, i2, d1, p1, p2);		// 逆円筒曲げ実行
				} else if(cno==TWIST) {									// <ひねりの場合>
					i1 = GetParaInt(cmp, 0);							// ひねり軸方向取得
					d1 = (double)GetParaInt(cmp, 1)/1000.0;				// ひねり角度取得
					d2 = (double)GetParaInt(cmp, 2)/1000.0;				// ひねり軸幅取得
					GetParaPnt(cmp, 0, &p1);							// ひねり基準点取得
					BackTwist(wf, &wpt, &npt, i1, d1, d2, p1);			// 逆ひねり実行
				} else if(cno==SHEAR) {									// <せん断の場合>
					i1 = GetParaInt(cmp, 0);							// せん断垂直方向取得
					i2 = GetParaInt(cmp, 1);							// せん断水平方向取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// せん断傾き取得
					GetParaPnt(cmp, 0, &p1);							// 基準線点取得
					BackShear(wf, &wpt, &npt, i1, i2, d1, p1);			// 逆せん断実行
				} else if(cno==RING) {									// <ﾘﾝｸﾞの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向倍率取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向倍率取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向倍率取得
					i1 = GetParaInt(cmp, 3);							// 曲げ水平方向取得
					i2 = GetParaInt(cmp, 4);							// 曲げ垂直方向取得
					d4 = (double)GetParaInt(cmp, 5)/1000.0;				// 曲げ角度取得
					GetParaPnt(cmp, 0, &p1);							// ｻｲｽﾞ変更中心点取得
					GetParaPnt(cmp, 1, &p2);							// 基準線点取得
					GetParaPnt(cmp, 2, &p3);							// 基準線点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新無)
						BackBend(0, &wpt, &npt, i1, i2, d4, p2, p3);	// 逆円筒曲げ実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新無)
						BackBend(1, &wpt, &npt, i1, i2, d4, p2, p3);	// 逆円筒曲げ実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p1, d1, d2, d3);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==EARTH) {									// <ｱｰｽの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X軸回転角度取得
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y軸回転角度取得
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z軸回転角度取得
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X方向倍率取得
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y方向倍率取得
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// ｻｲｽﾞ変更中心点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==SPACE) {									// <ｽﾍﾟｰｽの場合>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X方向移動量取得
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y方向移動量取得
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z方向移動量取得
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X軸回転角度取得
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y軸回転角度取得
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z軸回転角度取得
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X方向倍率取得
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y方向倍率取得
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z方向倍率取得
					GetParaPnt(cmp, 0, &p1);							// 回転中心点取得
					GetParaPnt(cmp, 1, &p2);							// ｻｲｽﾞ変更中心点取得
					if(wf==0) {											// <更新対象外>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新無)
					} else {											// <更新対象>
						spt = wpt;										// 制御点保存
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// 逆平行移動実行(更新有)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新無)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X軸用回転ﾍﾞｸﾄﾙ
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// 逆回転移動実行(更新無)
						wpt = spt;										// 制御点復元
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// 逆ｻｲｽﾞ変更実行(更新有)
					}
				} else if(cno==RESIZE3) {								// <直線指定ﾘｻｲｽﾞ2の場合>
					i1 = GetParaInt(cmp, 0);							// 指定平面取得
					i2 = GetParaInt(cmp, 1);							// 正負ﾌﾗｸﾞ取得
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// 倍率1取得
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// 倍率2取得
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// 倍率3取得
					i3 = GetParaInt(cmp, 5);							// 垂直限定ﾌﾗｸﾞ取得
					d4 = (double)GetParaInt(cmp, 6)/1000.0;				// 垂直範囲1取得
					d5 = (double)GetParaInt(cmp, 7)/1000.0;				// 垂直範囲2取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					GetParaPnt(cmp, 1, &p2);							// 基準点2取得
					GetParaPnt(cmp, 2, &p3);							// 基準点3取得
					BackResize3(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                i3, d4, d5, p1, p2, p3);				// 逆直線指定ﾘｻｲｽﾞ2実行
				} else if(cno==RESIZE4) {								// <曲線指定ﾘｻｲｽﾞ2の場合>
					i1 = GetParaInt(cmp, 0);							// 正負ﾌﾗｸﾞ取得
					i2 = GetParaInt(cmp, 1);							// 両側ﾌﾗｸﾞ取得
					i3 = GetParaInt(cmp, 2);							// 長さ方向取得
					i4 = GetParaInt(cmp, 3);							// ｻｲｽﾞ指定方向取得
					i5 = GetParaInt(cmp, 4);							// 垂直方向取得
					d1 = (double)GetParaInt(cmp, 5)/1000.0;				// 基準長さ取得
					i6 = GetParaInt(cmp, 6);							// 垂直限定ﾌﾗｸﾞ取得
					d2 = (double)GetParaInt(cmp, 7)/1000.0;				// 垂直範囲1取得
					d3 = (double)GetParaInt(cmp, 8)/1000.0;				// 垂直範囲2取得
					i7 = GetParaInt(cmp, 9);							// 制御点数取得
					GetParaPnt(cmp, 0, &p1);							// 基準点1取得
					for(i=0; i<i7; i++) {
						ia[i] = GetParaInt(cmp, i+10);					// 曲線ﾓｰﾄﾞ取得
						GetParaPnt(cmp, i+1, &pa[i]);					// 曲線制御点取得
					}
					BackResize4(wf, &wpt, &npt, i1, i2, i3, i4, i5,
                                d1, i6, d2, d3, p1, i7, pa, ia);		// 逆曲線指定ﾘｻｲｽﾞ2実行
				}
				ct++;													// ｺﾏﾝﾄﾞｶｳﾝﾀUP
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
			}
			mx--;														// ｺﾏﾝﾄﾞ個数減算
		}
		*pc = npt;
	} else {															// <移動対象外の場合>
		*pc = opt;
	}
	delete[] pa;
	delete[] ia;
}

/***************************************************************/
void MoveCPoint(int md, PNTTYPE* ma, OBJTYPE* op, int un, int vn,
                int ub, int vb, double wt, int* ia, int* ib,
                PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)			//<<< 制御点移動
/***************************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxSCtA];
	int*     flg = new int[View->m_MaxSCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  zro, opt, npt;
	int      i, j, k, l, ten, pn, mx=0, chk=1, p1, p2, ps, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	pn = un*vn;															// 総制御点数
	ten = View->GetSurfaceTension();									// 張力設定番号
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	zro.p[0] = 0.0, zro.p[1] = 0.0, zro.p[2] = 0.0;						// 作業用座標原点
	for(i=0; i<pn; i++) {
		if(ib[i]) {														// 移動量/ﾌﾗｸﾞ配列複写
			mov[i] = ma[i], flg[i] = 1;
		} else {
			mov[i] = zro,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<張力設定有の場合>>
		for(i=0; i<vn; i++) {
			for(j=0, sct=0; j<un-1; j++) {								// <U方向条件確認>
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// 指定点境界ｶｳﾝﾄ
					sct++;
				}
			}
			if(sct>2) {													// 処理条件NG
				chk = 0; break;
			}
		}
		if(chk) {
			for(i=0; i<un; i++) {
				for(j=0, sct=0; j<vn-1; j++) {							// <V方向条件確認>
					if(flg[i+un*j]!=flg[i+un*(j+1)]) {					// 指定点境界ｶｳﾝﾄ
						sct++;
					}
				}
				if(sct>2) {												// 処理条件NG
					chk = 0; break;
				}
			}
		}
	} else {															// 張力設定無NG
		chk = 0;
	}
	if(chk) {															// <<処理条件OK>>
		for(i=0; i<vn; i++) {											// <U方向移動量更新>
			for(j=0, sct=0, p1=-1, p2=-1; j<un-1; j++) {
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// <前後ﾌﾗｸﾞ異>
					sct++;												// 指定点境界ｶｳﾝﾄ
					if(flg[un*i+j]) {									// 境界1位置
						p1 = j+1;
					} else {											// 境界2位置
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <指定点連続>
				for(j=0; j<un; j++) {									// 制御点間距離測定
					len[j] = (j==un-1) ? PntD(&pb[un*(i+1)-1], &pb[un*i])
                                       : PntD(&pb[un*i+j], &pb[un*i+j+1]);
					wta[j] = ta[un*i+j];								// 張力限定情報抽出
				}
				if(p1==-1) {											// 指定点前後位置&点数設定
					if(ub) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = un-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = un-1;
					}
				} else if(p2==-1) {
					if(ub) {
						sct = (un-p1)/2, ect = (un-p1)/2;
						sno = un-1, eno = p1-1;
					} else {
						sct = 0, ect = un-p1;
						sno = un-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(ub) {
						sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = un-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <指定点前制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k-1<0) ? un-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				lst = (lst-1<0) ? un-1 : lst-1;							// 有効点前線分も含む
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln ;						// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k-1<0) ? un-1 : k-1;
				}
				ps = (sno+1>un-1) ? 0 : sno+1;							// 基準移動量番地
				ps = un*i+ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// 指定制御点からの距離
					l = un*i+k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
					k = (k-1<0) ? un-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <指定点後制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k+1>un-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>un-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k+1>un-1) ? 0 : k+1;
				}
				ps = un*i+eno;											// 基準移動量番地
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// 指定制御点からの距離
					k = (k+1>un-1) ? 0 : k+1;
					l = un*i+k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
				}
			}
		}
		for(i=0; i<un; i++) {											// <V方向移動量更新>
			for(j=0, sct=0, p1=-1, p2=-1; j<vn-1; j++) {
				if(flg[i+un*j]!=flg[i+un*(j+1)]) {						// <前後ﾌﾗｸﾞ異>
					sct++;												// 指定点境界ｶｳﾝﾄ
					if(flg[i+un*j]) {									// 境界1位置
						p1 = j+1;
					} else {											// 境界2位置
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <指定点連続>
				for(j=0; j<vn; j++) {									// 制御点間距離測定
					len[j] = (j==vn-1) ? PntD(&pb[i+un*(vn-1)], &pb[i])
                                       : PntD(&pb[i+un*j], &pb[i+un*(j+1)]);
					wta[j] = ta[i+un*j];								// 張力限定情報抽出
				}
				if(p1==-1) {											// 指定点前後位置&点数設定
					if(vb) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = vn-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = vn-1;
					}
				} else if(p2==-1) {
					if(vb) {
						sct = (vn-p1)/2, ect = (vn-p1)/2;
						sno = vn-1, eno = p1-1;
					} else {
						sct = 0, ect = vn-p1;
						sno = vn-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(vb) {
						sct = (p2+1+vn-p1)/2, ect = (p2+1+vn-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = vn-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <指定点前制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k-1<0) ? vn-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				lst = (lst-1<0) ? vn-1 : lst-1;							// 有効点前線分も含む
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k-1<0) ? vn-1 : k-1;
				}
				ps = (sno+1>vn-1) ? 0 : sno+1;							// 基準移動量番地
				ps = i+un*ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// 指定制御点からの距離
					l = i+un*k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
					k = (k-1<0) ? vn-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <指定点後制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>vn-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ps = i+un*eno;											// 基準移動量番地
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// 指定制御点からの距離
					k = (k+1>vn-1) ? 0 : k+1;
					l = i+un*k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i].p[j];									// 立体編集&移動後座標
		}
	}
	CalcSurfacePoint(md, 0, 0, un, vn, ub, vb, wt, ia, mov, cpt);		// 通過点=>制御点変換
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// 通過点は全て移動済み
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// 移動前/移動後 制御点
		MoveCtrlPoint(op, opt, npt, mx, flg[i], &pc[i]);				// 制御点移動
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] wta;
	delete[] len;
}

/*********************************************************************/
static void TwistPoint(PNTTYPE cpt, VECTYPE vec, double th,
                       int ix, int mx, OBJTYPE* op, int un,
                       int vn, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc)	//<<< 制御点ひねり
/*********************************************************************/
{
	PNTTYPE opt, npt;
	int     i, j;
	double  rot[3][3];

	if(fabs(th)>EPSILON&&												// <<角度有の場合>>
       (fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON||fabs(vec.p[2])>EPSILON)) {
		SetRotateMatrix(vec, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(i=0; i<vn; i++) {
			opt = pa[ix+i*un];											// 立体編集前制御点
			for(j=0; j<DIM; j++) {
				npt.p[j] = pb[ix+i*un].p[j]-cpt.p[j];					// 立体編集&移動後制御点
			}
			VecRotateMove(&npt, rot, cpt);								// 回転移動
			MoveCtrlPoint(op, opt, npt, mx, 1, &pc[ix+i*un]);			// 制御点移動
		}
	} else {															// <<角度無の場合>>
		for(i=0; i<vn; i++) {
			pc[ix+i*un] = pa[ix+i*un];									// 制御点複写
		}
	}
}

/***********************************************************************/
void TwistCLine(PNTTYPE* cpt, VECTYPE* vec, double th, int bn,
                int* sa, int sn, OBJTYPE* op, int un, int vn, int ub,
                int sf, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)	//<<< 制御点ひねり
/***********************************************************************/
{
	int*     wta = new int[View->m_MaxCCtA];
	double*  uln = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, k, ct, ix, sx, ex, ten, mx=0, lst, ok;
	double   sln, wln, ft, wt;

	ten = View->GetSurfaceTension();									// 張力設定番号取得
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	ct = un*vn;
	for(i=0; i<ct; i++) {
		pc[i] = pa[i];													// 制御点仮複写
	}
	for(i=0; i<un; i++) {
		uln[i] = (i==un-1) ? PntD(&cpt[un-1], &cpt[0])
                           : PntD(&cpt[i], &cpt[i+1]);					// 中心点間距離測定
		for(ok=0, j=0; j<vn; j++) {										// 張力有効確認
			if(ta[i+un*j]) {											// 1点でも有効でOK
				ok = 1; break;
			}
		}
		wta[i] = ok;
	}
	for(i=0, j=0, wt=0.0; i<DIM; i++) {									// 基準断面の
		if(wt<fabs(vec[bn].p[i])) {										// ﾍﾞｸﾄﾙ最大方向検索
			wt = fabs(vec[bn].p[i]), j = i;
		}
	}
	if(vec[bn].p[j]>0.0) {												// 基準断面のﾍﾞｸﾄﾙ
		for(i=0; i<un; i++) {											// 最大方向が正の場合
			vec[i].p[0]*=-1.0, vec[i].p[1]*=-1.0, vec[i].p[2]*=-1.0;	// 法線ﾍﾞｸﾄﾙ補正
		}
	}
	for(i=0, sx=-1; i<sn; i++) {
		if(sa[i]) {
			ix = (sf%2) ? i+1 : i;
			sx = (sx==-1) ? ix : sx;									// 開始断面保存
			ex = ix;													// 終了断面保存
			ft = th*PI/180.0;
			TwistPoint(cpt[ix], vec[ix], ft, ix, mx, op, un, vn,
                       pa, pb, pc);										// 指定断面制御点ひねり
		}
	}
	if(ub) {															// <U:閉の場合>
		for(i=0, ct=0; i<sn; i++) {
			if(sa[i]) {													// 先頭の指示済み
				ix = i;
				for(j=0, k=0; j<sn; j++) {
					ix = (ix+1>sn-1) ? 0 : ix+1;
					if(sa[ix]) {										// 最後の指示済み
						break;
					} else {											// 中間の未指示をｶｳﾝﾄ
						k++;
					}
				}
				if(k>ct) {												// 最長の未指示領域
					ct = k, sx = ix, ex = i;
				}
			}
		}
		sx = (sf%2) ? sx+1 : sx;										// 開始断面保存
		ex = (sf%2) ? ex+1 : ex;										// 終了断面保存
		ct = ct/2;														// 有効断面数(指定断面前/後分)
	}
	if(ten>0) {															// <<<張力設定有の場合>>>
		ct = (!ub) ? sx : ct;											// <<指定断面前ひねり>>
		for(i=0, j=sx, lst=-1; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			lst = (wta[j]) ? j : lst;									// 最終張力有効点検索
		}
		ok = (lst==-1) ? 0 : 1;											// 張力有効点有無
		lst = (lst-1<0) ? un-1 : lst-1;									// 有効点前線分も含む
		for(i=0, sln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			sln = (ok) ? sln+uln[j] : sln;								// 指定面前中心距離合計
			ok = (j==lst) ? 0 : ok;										// 最終張力有効点以降は無視
		}
		for(i=0, wln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			wln += uln[j];												// 指定断面からの中心距離
			if(wta[j]) {												// <張力有効断面>
				wt = ft;												// 指定断面ひねり角度
				GetTensionFltValue(ten, sln, wln, &wt);					// 張力設定値変換
				TwistPoint(cpt[j], vec[j], wt, j, mx, op, un, vn,
                           pa, pb, pc);									// 制御点ひねり
			}
		}
		ct = (!ub) ? un-ex-1 : ct;										// <<指定断面後ひねり>>
		for(i=0, j=ex, lst=-1; i<ct; i++) {
			lst = (wta[j]) ? j : lst;									// 最終張力有効点検索
			j = (j+1>un-1) ? 0 : j+1;
		}
		ok = (lst==-1) ? 0 : 1;											// 張力有効点有無
		for(i=0, sln=0.0, j=ex; i<ct; i++) {
			k = (j+1>un-1) ? 0 : j+1;
			sln = (ok||wta[k]) ? sln+uln[j] : sln;						// 指定面後中心距離合計
			ok = (j==lst) ? 0 : ok;										// 最終張力有効点以降は無視
			j = (j+1>un-1) ? 0 : j+1;
		}
		for(i=0, wln=0.0, j=ex; i<ct; i++) {
			wln += uln[j];												// 指定断面からの中心距離
			j = (j+1>un-1) ? 0 : j+1;
			if(wta[j]) {												// <張力有効断面>
				wt = ft;												// 指定断面ひねり角度
				GetTensionFltValue(ten, sln, wln, &wt);					// 張力設定値変換
				TwistPoint(cpt[j], vec[j], wt, j, mx, op, un, vn,
                           pa, pb, pc);									// 制御点ひねり
			}
		}
	}
	delete[] wta;
	delete[] uln;
}

/*****************************************************************/
static void ScalePoint(VECTYPE vec, int vd, int fg, PNTTYPE bpt,
                       int bn, int dn, double sc, int ix, int mx,
                       OBJTYPE* op, int un, int vn, PNTTYPE* pa,
                       PNTTYPE* pb, PNTTYPE* pc)						//<<< 制御点拡大/縮小
/*****************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  cpt, opt, npt;
	VECTYPE  rvc;
	int      i, i1, i2, i3, j1, j2, chk=0;
	double   rot[3][3], fln, rth, mn1, mn2, mx1, mx2, dd;

	if(fabs(sc-1.0)>EPSILON&&											// <<倍率有の場合>>
       (fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON||fabs(vec.p[2])>EPSILON)) {
		for(i=0, i1=0; i<vn; i++) {
			pt[i1++] = pb[ix+i*un];										// 対象V断面抽出
		}
		i1 = (vd+1>2) ? 0 : vd+1;										// 座標軸設定
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		if(vd==0) {
			j1 = 1, j2 = 2;
		} else if(vd==1) {
			j1 = 0, j2 = 2;
		} else {
			j1 = 1, j2 = 0;
		}
		fln = sqrt(vec.p[i1]*vec.p[i1]+vec.p[i2]*vec.p[i2]);			// 基準面方向長さ
		if(fln>EPSILON) {												// <基準面にない場合>
			rvc.p[i1] = -vec.p[i2], rvc.p[i2] = vec.p[i1], rvc.p[i3] = 0.0;
			VecN(&rvc);
			rth = atan2(fln, vec.p[i3]);								// 回転角度算出
			rth = (rth<-10||rth>10) ? 0.0 : rth;
			rth = -rth;
			rth = (fg) ? rth-PI : rth;									// 基準面との向き違い
			SetRotateMatrix(rvc, rth, rot);								// 回転ﾏﾄﾘｯｸｽ作成
			for(i=0; i<vn; i++) {
				VecRotate(&pt[i], rot);									// 基準面に回転
			}
			chk = 1;													// 実行ﾌﾗｸﾞON
		} else if((fg&&vec.p[vd]>0.0)||((!fg)&&vec.p[vd]<0.0)) {		// <基準点と反転の場合>
			if(fabs(bpt.p[0])>EPSILON||
               fabs(bpt.p[1])>EPSILON||
               fabs(bpt.p[2])>EPSILON) {
				rvc.p[i1] = -bpt.p[i2];									// 回転軸設定
				rvc.p[i2] = bpt.p[i1];
				rvc.p[i3] = 0.0;
				VecN(&rvc);
				rth = -PI;												// 回転角度反転
				SetRotateMatrix(rvc, rth, rot);							// 回転ﾏﾄﾘｯｸｽ作成
				for(i=0; i<vn; i++) {									// 基準面に回転
					VecRotate(&pt[i], rot);
				}
				chk = 1;												// 実行ﾌﾗｸﾞON
			}
		}
		mn1 = pt[0].p[j1], mx1 = pt[0].p[j1];
		mn2 = pt[0].p[j2], mx2 = pt[0].p[j2];
		for(i=1; i<vn; i++) {
			mn1 = (mn1>pt[i].p[j1]) ? pt[i].p[j1] : mn1;				// 平面最小最大検索
			mx1 = (mx1<pt[i].p[j1]) ? pt[i].p[j1] : mx1;
			mn2 = (mn2>pt[i].p[j2]) ? pt[i].p[j2] : mn2;
			mx2 = (mx2<pt[i].p[j2]) ? pt[i].p[j2] : mx2;
		}
		if(vd==1) {														// 最小<->最大入替
			dd = mn1, mn1 = mx1, mx1 = dd;
		} else if(vd==2) {
			dd = mn2, mn2 = mx2, mx2 = dd;
		}
		if(bn==0||bn==3||bn==6) {										// 基準点設定
			cpt.p[j1] = mn1;
		} else if(bn==1||bn==4||bn==7) {
			cpt.p[j1] = (mn1+mx1)/2.0;
		} else {
			cpt.p[j1] = mx1;
		}
		if(bn==6||bn==7||bn==8) {
			cpt.p[j2] = mn2;
		} else if(bn==3||bn==4||bn==5) {
			cpt.p[j2] = (mn2+mx2)/2.0;
		} else {
			cpt.p[j2] = mx2;
		}
		for(i=0; i<vn; i++) {
			if(dn>=1) {
				pt[i].p[j1] -= cpt.p[j1];
				pt[i].p[j1] = pt[i].p[j1]*sc+cpt.p[j1];					// 基準点移動後,拡大/縮小
			}
			if(dn<=1) {
				pt[i].p[j2] -= cpt.p[j2];
				pt[i].p[j2] = pt[i].p[j2]*sc+cpt.p[j2];
			}
		}
		if(chk) {														// <回転実行の場合>
			SetRotateMatrix(rvc, -rth, rot);							// 回転ﾏﾄﾘｯｸｽ作成
			for(i=0; i<vn; i++) {
				VecRotate(&pt[i], rot);									// 元位置に逆回転
			}
		}
		for(i=0; i<vn; i++) {
			opt = pa[ix+i*un];											// 立体編集前制御点
			npt = pt[i];												// 立体&曲面編集後制御点
			MoveCtrlPoint(op, opt, npt, mx, 1, &pc[ix+i*un]);			// 制御点移動
		}
	} else {															// <<倍率無の場合>>
		for(i=0; i<vn; i++) {
			pc[ix+i*un] = pa[ix+i*un];									// 制御点複写
		}
	}
	delete[] pt;
}

/***************************************************************/
void ScaleCLine(PNTTYPE* cpt, VECTYPE* vec, int bn, int dn,
                double sc, int bs, int* sa, int sn,
                OBJTYPE* op, int un, int vn, int ub, int sf,
                PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)			//<<< 制御点拡大/縮小
/***************************************************************/
{
	int*     wta = new int[View->m_MaxCCtA];
	double*  uln = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  bpt;
	int      i, j, k, l, vd, fg, ct, sx, ex, ten, mx=0, lst, ok;
	double   sln, wln, wsc;

	ten = View->GetSurfaceTension();									// 張力設定番号取得
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	ct = un*vn;
	for(i=0; i<ct; i++) {
		pc[i] = pa[i];													// 制御点仮複写
	}
	for(i=0; i<un; i++) {
		uln[i] = (i==un-1) ? PntD(&cpt[un-1], &cpt[0])
                           : PntD(&cpt[i], &cpt[i+1]);					// 中心点間距離測定
		for(ok=0, j=0; j<vn; j++) {										// 張力有効確認
			if(ta[i+un*j]) {											// 1点でも有効でOK
				ok = 1; break;
			}
		}
		wta[i] = ok;
	}
	for(i=0, vd=0, wln=0.0; i<DIM; i++) {
		if(wln<fabs(vec[bs].p[i])) {									// 基準断面ﾍﾞｸﾄﾙ最大方向検索
			wln = fabs(vec[bs].p[i]), vd = i;
		}
	}
	fg = (vec[bs].p[vd]<0) ? 1 : 0;										// 基準断面最大方向負ﾌﾗｸﾞ
	for(i=0, sx=-1; i<sn; i++) {
		if(sa[i]) {
			j = (sf%2) ? i+1 : i;
			sx = (sx==-1) ? j : sx;										// 開始断面保存
			ex = j;														// 終了断面保存
			for(k=0; k<DIM; k++) {
				bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];					// 基準断面-対象断面 中心点誤差
			}
			ScalePoint(vec[j], vd, fg, bpt, bn, dn, sc, j, mx,
                       op, un, vn, pa, pb, pc);							// 指定断面制御点拡大縮小
		}
	}
	if(ub) {															// <U:閉の場合>
		for(i=0, ct=0; i<sn; i++) {
			if(sa[i]) {													// 先頭の指示済み
				j = i;
				for(k=0, l=0; k<sn; k++) {
					j = (j+1>sn-1) ? 0 : j+1;
					if(sa[j]) {											// 最後の指示済み
						break;
					} else {											// 中間の未指示をｶｳﾝﾄ
						l++;
					}
				}
				if(l>ct) {												// 最長の未指示領域
					ct = l, sx = j, ex = i;
				}
			}
		}
		sx = (sf%2) ? sx+1 : sx;										// 開始断面保存
		ex = (sf%2) ? ex+1 : ex;										// 終了断面保存
		ct = ct/2;														// 有効断面数(指定断面前/後分)
	}
	if(ten>0) {															// <<<張力設定有の場合>>>
		ct = (!ub) ? sx : ct;											// <<指定断面前拡大縮小>>
		for(i=0, j=sx, lst=-1; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			lst = (wta[j]) ? j : lst;									// 最終張力有効点検索
		}
		ok = (lst==-1) ? 0 : 1;											// 張力有効点有無
		lst = (lst-1<0) ? un-1 : lst-1;									// 有効点前線分も含む
		for(i=0, sln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			sln = (ok) ? sln+uln[j] : sln;								// 指定面前中心距離合計
			ok = (j==lst) ? 0 : ok;										// 最終張力有効点以降は無視
		}
		for(i=0, wln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			wln += uln[j];												// 指定断面からの中心距離
			if(wta[j]) {												// <張力有効断面>
				wsc = sc - 1.0;											// 指定断面倍率
				GetTensionFltValue(ten, sln, wln, &wsc);				// 張力設定値変換
				wsc = wsc + 1.0;
				for(k=0; k<DIM; k++) {									// 基準断面と対象断面の
					bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];				// 中心点の誤差
				}
				ScalePoint(vec[j], vd, fg, bpt, bn, dn, wsc, j,
                           mx, op, un, vn, pa, pb, pc);					// 制御点拡大/縮小
			}
		}
		ct = (!ub) ? un-ex-1 : ct;										// <<指定断面後拡大縮小>>
		for(i=0, j=ex, lst=-1; i<ct; i++) {
			lst = (wta[j]) ? j : lst;									// 最終張力有効点検索
			j = (j+1>un-1) ? 0 : j+1;
		}
		ok = (lst==-1) ? 0 : 1;											// 張力有効点有無
		for(i=0, sln=0.0, j=ex; i<ct; i++) {
			k = (j+1>un-1) ? 0 : j+1;
			sln = (ok||wta[k]) ? sln+uln[j] : sln;						// 指定面後中心距離合計
			ok = (j==lst) ? 0 : ok;										// 最終張力有効点以降は無視
			j = (j+1>un-1) ? 0 : j+1;
		}
		for(i=0, wln=0.0, j=ex; i<ct; i++) {
			wln += uln[j];												// 指定断面からの中心距離
			j = (j+1>un-1) ? 0 : j+1;
			if(wta[j]) {												// <張力有効断面>
				wsc = sc - 1.0;											// 指定断面倍率
				GetTensionFltValue(ten, sln, wln, &wsc);				// 張力設定値変換
				wsc = wsc + 1.0;
				for(k=0; k<DIM; k++) {									// 基準断面と対象断面の
					bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];				// 中心点の誤差
				}
				ScalePoint(vec[j], vd, fg, bpt, bn, dn, wsc, j,
                           mx, op, un, vn, pa, pb, pc);					// 制御点拡大/縮小
			}
		}
	}
	delete[] wta;
	delete[] uln;
}

/***********************************************************/
void MoveSCurve(int md, double *db, PNTTYPE* ma,
                OBJTYPE* op, int pn, int bc, PNTTYPE* pa,
                PNTTYPE* pb, int* ia, PNTTYPE* pc, int* ta)				//<<< 断面制御点移動
/***********************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxCCtA];
	int*     flg = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	PNTTYPE  opt, npt;
	int      i, j, k, ten, p1, p2, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// 張力設定番号取得
	opt.p[0] = 0.0, opt.p[1] = 0.0, opt.p[2] = 0.0;						// 座標値:0配列作成
	for(i=0; i<pn; i++) {
		if(ia[i]) {														// 移動量/ﾌﾗｸﾞ配列複写
			mov[i] = ma[i], flg[i] = 1;
		} else {
			mov[i] = opt,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<張力設定有の場合>>
		for(i=0, j=0, p1=-1, p2=-1; i<pn-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <前後ﾌﾗｸﾞ異>
				j++;													// 指定点境界ｶｳﾝﾄ
				if(flg[i]) {											// 境界1位置
					p1 = i+1;
				} else {												// 境界2位置
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <指定点連続>
			for(i=0; i<pn; i++) {
				len[i] = (i==pn-1) ? PntD(&pb[i], &pb[0])
                                   : PntD(&pb[i], &pb[i+1]);			// 制御点間距離測定
			}
			if(p1==-1) {												// 指定点前後位置&点数設定
				if(bc) {
					sct = (p2+1)/2, ect = (p2+1)/2;
					sno = p2, eno = pn-1;
				} else {
					sct = p2+1, ect = 0;
					sno = p2, eno = pn-1;
				}
			} else if(p2==-1) {
				if(bc) {
					sct = (pn-p1)/2, ect = (pn-p1)/2;
					sno = pn-1, eno = p1-1;
				} else {
					sct = 0, ect = pn-p1;
					sno = pn-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(bc) {
					sct = (p2+1+pn-p1)/2, ect = (p2+1+pn-p1)/2;
					sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = pn-p1;
					sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
				sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <指定点前制御点移動>
				lst = (ta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j-1<0) ? pn-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			lst = (lst-1<0) ? pn-1 : lst-1;								// 有効点前線分も含む
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// 指定制御点からの距離
				if(ta[j]) {												// <張力有効点>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// 張力設定値変換
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// 指定ﾌﾗｸﾞON
					}
				}
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <指定点後制御点移動>
				lst = (ta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j+1>pn-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>pn-1) ? 0 : j+1;
				sln = (ok||ta[k]) ? sln+len[j] : sln;					// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j+1>pn-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// 指定制御点からの距離
				j = (j+1>pn-1) ? 0 : j+1;
				if(ta[j]) {												// <張力有効点>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// 張力設定値変換
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// 指定ﾌﾗｸﾞON
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i].p[j];									// 立体編集&移動後座標
		}
	}
	CalcCurvePoint(md, pn, mov, cpt, db);								// 通過点=>制御点変換
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// 通過点は全て移動済み
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// 移動前/移動後 制御点
		MoveCtrlPoint(op, opt, npt, 0, flg[i], &pc[i]);					// 制御点移動
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] len;
}

/****************************************************************/
void MoveSPoint(OBJTYPE* op, int un, int vn, int ub, int vb,
                PNTTYPE* mov, PNTTYPE* pa, PNTTYPE* pb, int* ta)		//<<< 断面制御点移動
/****************************************************************/
{
	int*     flg = new int[View->m_MaxSCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  opt, npt;
	int      i, j, k, l, ten, pn, mx=0, chk=1, p1, p2, ps, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	pn = un*vn;															// 総制御点数
	ten = View->GetSurfaceTension();									// 張力設定番号
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	for(i=0; i<pn; i++) {
		for(j=0, k=0; j<DIM; j++) {
			if(fabs(mov[i].p[j])>EPSILON) {								// 処理ﾌﾗｸﾞ設定
				k = 1; break;
			}
		}
		flg[i] = k;
	}
	if(ten>0) {															// <<張力設定有の場合>>
		for(i=0; i<vn; i++) {
			for(j=0, sct=0; j<un-1; j++) {								// <U方向条件確認>
				sct = (flg[un*i+j]!=flg[un*i+j+1]) ? sct+1 : sct;		// 指定点境界ｶｳﾝﾄ
			}
			if(sct>2) {													// 処理条件NG
				chk = 0; break;
			}
		}
		if(chk) {
			for(i=0; i<un; i++) {
				for(j=0, sct=0; j<vn-1; j++) {							// <V方向条件確認>
					sct = (flg[i+un*j]!=flg[i+un*(j+1)]) ? sct+1 : sct;	// 指定点境界ｶｳﾝﾄ
				}
				if(sct>2) {												// 処理条件NG
					chk = 0; break;
				}
			}
		}
	} else {															// 張力設定無NG
		chk = 0;
	}
	if(chk) {															// <<処理条件OK>>
		for(i=0; i<vn; i++) {											// <U方向移動量更新>
			for(j=0, sct=0, p1=-1, p2=-1; j<un-1; j++) {
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// <前後ﾌﾗｸﾞ異>
					sct++;												// 指定点境界ｶｳﾝﾄ
					if(flg[un*i+j]) {									// 境界1位置
						p1 = j+1;
					} else {											// 境界2位置
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <指定点連続>
				for(j=0; j<un; j++) {									// 制御点間距離測定
					len[j] = (j==un-1) ? PntD(&pb[un*(i+1)-1], &pb[un*i])
                                       : PntD(&pb[un*i+j], &pb[un*i+j+1]);
					wta[j] = ta[un*i+j];								// 張力限定情報抽出
				}
				if(p1==-1) {											// 指定点前後位置&点数設定
					if(ub) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = un-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = un-1;
					}
				} else if(p2==-1) {
					if(ub) {
						sct = (un-p1)/2, ect = (un-p1)/2;
						sno = un-1, eno = p1-1;
					} else {
						sct = 0, ect = un-p1;
						sno = un-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(ub) {
						sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = un-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <指定点前制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k-1<0) ? un-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				lst = (lst-1<0) ? un-1 : lst-1;							// 有効点前線分も含む
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k-1<0) ? un-1 : k-1;
				}
				ps = (sno+1>un-1) ? 0 : sno+1;							// 基準移動量番地
				ps = un*i+ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// 指定制御点からの距離
					l = un*i+k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
					k = (k-1<0) ? un-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <指定点後制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k+1>un-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>un-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k+1>un-1) ? 0 : k+1;
				}
				ps = un*i+eno;											// 基準移動量番地
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// 指定制御点からの距離
					k = (k+1>un-1) ? 0 : k+1;
					l = un*i+k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
				}
			}
		}
		for(i=0; i<un; i++) {											// <V方向移動量更新>
			for(j=0, sct=0, p1=-1, p2=-1; j<vn-1; j++) {
				if(flg[i+un*j]!=flg[i+un*(j+1)]) {						// <前後ﾌﾗｸﾞ異>
					sct++;												// 指定点境界ｶｳﾝﾄ
					if(flg[i+un*j]) {									// 境界1位置
						p1 = j+1;
					} else {											// 境界2位置
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <指定点連続>
				for(j=0; j<vn; j++) {									// 制御点間距離測定
					len[j] = (j==vn-1) ? PntD(&pb[i+un*(vn-1)], &pb[i])
                                       : PntD(&pb[i+un*j], &pb[i+un*(j+1)]);
					wta[j] = ta[i+un*j];								// 張力限定情報抽出
				}
				if(p1==-1) {											// 指定点前後位置&点数設定
					if(vb) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = vn-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = vn-1;
					}
				} else if(p2==-1) {
					if(vb) {
						sct = (vn-p1)/2, ect = (vn-p1)/2;
						sno = vn-1, eno = p1-1;
					} else {
						sct = 0, ect = vn-p1;
						sno = vn-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(vb) {
						sct = (p2+1+vn-p1)/2, ect = (p2+1+vn-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = vn-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <指定点前制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k-1<0) ? vn-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				lst = (lst-1<0) ? vn-1 : lst-1;							// 有効点前線分も含む
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k-1<0) ? vn-1 : k-1;
				}
				ps = (sno+1>vn-1) ? 0 : sno+1;							// 基準移動量番地
				ps = i+un*ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// 指定制御点からの距離
					l = i+un*k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
					k = (k-1<0) ? vn-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <指定点後制御点移動>
					lst = (wta[k]) ? k : lst;							// 最終張力有効点検索
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// 張力有効点有無
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>vn-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// 制御点間距離集計
					ok = (k==lst) ? 0 : ok;								// 最終張力有効点以降は無視
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ps = i+un*eno;											// 基準移動量番地
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// 指定制御点からの距離
					k = (k+1>vn-1) ? 0 : k+1;
					l = i+un*k;											// 移動量配列番地
					if(wta[k]) {										// <張力有効点>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// 張力設定値変換
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// 指定ﾌﾗｸﾞON
						}
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i];													// 立体編集前制御点
		for(j=0; j<DIM; j++) {
			npt.p[j] = pb[i].p[j]+mov[i].p[j];							// 立体編集&移動後制御点
		}
		MoveCtrlPoint(op, opt, npt, mx, flg[i], &pa[i]);				// 制御点移動
	}
	delete[] flg;
	delete[] wta;
	delete[] len;
}

/**************************************************************/
static void CalcBaseLine(PNTTYPE* pp, int un, int vn, int ub,
                         int vflg, PNTTYPE* cp1, PNTTYPE* cp2)			//<<< ﾍﾞｰｽ線移動計算
/**************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3];
	VECTYPE  rvc, ovc, nvc, vc1, vc2;
	int      i, j, k;
	double   th, rot[3][3];

	for(i=0; i<un; i++) {
		for(j=0; j<vn; j++) {
			pt[j] = pp[i+j*un];											// V断面ﾃﾞｰﾀ抽出
		}
		if(i==0) {														// <<開始断面の場合>>
			if(vflg%2) {												// <開始断面同一>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[2].p[k]-cp2[0].p[k];					// 新旧ﾍﾞｸﾄﾙ取得
					nvc.p[k] = cp1[2].p[k]-cp1[0].p[k];
				}
			} else {													// <開始断面不同一>
				if(!ub) {												// <U開の場合>
					for(k=0; k<DIM; k++) {
						ovc.p[k] = cp2[1].p[k]-cp2[0].p[k];				// 新旧ﾍﾞｸﾄﾙ取得
						nvc.p[k] = cp1[1].p[k]-cp1[0].p[k];
					}
				} else {												// <U閉の場合>
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp2[0].p[k]-cp2[un-1].p[k];			// 前後面ﾍﾞｸﾄﾙ取得
						vc2.p[k] = cp2[1].p[k]-cp2[0].p[k];
					}
					VecN(&vc1);											// 単位ﾍﾞｸﾄﾙ化
					VecN(&vc2);
					for(k=0; k<DIM; k++) { 
						ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// 旧ﾍﾞｸﾄﾙ取得
					}
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp1[0].p[k]-cp1[un-1].p[k];			// 前後面ﾍﾞｸﾄﾙ取得
						vc2.p[k] = cp1[1].p[k]-cp1[0].p[k];
					}
					VecN(&vc1);											// 単位ﾍﾞｸﾄﾙ化
					VecN(&vc2);
					for(k=0; k<DIM; k++) {
						nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// 新ﾍﾞｸﾄﾙ取得
					}
				}
			}
		} else if(i==un-1) {											// <<終了断面の場合>>
			if(vflg/2) {												// <終了断面同一>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[un-3].p[k]-cp2[un-1].p[k];			// 新旧ﾍﾞｸﾄﾙ取得
					nvc.p[k] = cp1[un-3].p[k]-cp1[un-1].p[k];
				}
			} else {													// <終了断面不同一>
				if(!ub) {												// <U開の場合>
					for(k=0; k<DIM; k++) {
						ovc.p[k] = cp2[un-2].p[k]-cp2[un-1].p[k];		// 新旧ﾍﾞｸﾄﾙ取得
						nvc.p[k] = cp1[un-2].p[k]-cp1[un-1].p[k];
					}
				} else {												// <U閉の場合>
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp2[un-1].p[k]-cp2[un-2].p[k];		// 前後面ﾍﾞｸﾄﾙ取得
						vc2.p[k] = cp2[0].p[k]-cp2[un-1].p[k];
					}
					VecN(&vc1);											// 単位ﾍﾞｸﾄﾙ化
					VecN(&vc2);
					for(k=0; k<DIM; k++) { 
						ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// 旧ﾍﾞｸﾄﾙ取得
					}
					for(k=0; k<DIM; k++) {								// 前後面ﾍﾞｸﾄﾙ取得
						vc1.p[k] = cp1[un-1].p[k]-cp1[un-2].p[k];
						vc2.p[k] = cp1[0].p[k]-cp1[un-1].p[k];
					}
					VecN(&vc1);											// 単位ﾍﾞｸﾄﾙ化
					VecN(&vc2);
					for(k=0; k<DIM; k++) {
						nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// 新ﾍﾞｸﾄﾙ取得
					}
				}
			}
		} else {														// <<中間断面の場合>>
			if(vflg%2&&i==1) {											// <開始断面同一>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[2].p[k]-cp2[1].p[k];					// 新旧ﾍﾞｸﾄﾙ取得
					nvc.p[k] = cp1[2].p[k]-cp1[1].p[k];
				}
			} else if(vflg/2&&i==un-2) {								// <終了断面同一>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[un-3].p[k]-cp2[un-2].p[k];			// 新旧ﾍﾞｸﾄﾙ取得
					nvc.p[k] = cp1[un-3].p[k]-cp1[un-2].p[k];
				}
			} else {													// <以外の場合>
				for(k=0; k<DIM; k++) {
					vc1.p[k] = cp2[i].p[k]-cp2[i-1].p[k];				// 前後面ﾍﾞｸﾄﾙ取得
					vc2.p[k] = cp2[i+1].p[k]-cp2[i].p[k];
				}
				VecN(&vc1);												// 単位ﾍﾞｸﾄﾙ化
				VecN(&vc2);
				for(k=0; k<DIM; k++) {
					ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;					// 旧ﾍﾞｸﾄﾙ取得
				}
				for(k=0; k<DIM; k++) {
					vc1.p[k] = cp1[i].p[k]-cp1[i-1].p[k];				// 前後面ﾍﾞｸﾄﾙ取得
					vc2.p[k] = cp1[i+1].p[k]-cp1[i].p[k];
				}
				VecN(&vc1);												// 単位ﾍﾞｸﾄﾙ化
				VecN(&vc2);
				for(k=0; k<DIM; k++) {
					nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;					// 新ﾍﾞｸﾄﾙ取得
				}
			}
		}
		VecN(&ovc);														// 単位ﾍﾞｸﾄﾙ化
		VecN(&nvc);
		for(j=0; j<vn; j++) {
			for(k=0; k<DIM; k++) {
				pt[j].p[k] -= cp2[i].p[k];								// 断面原点移動
			}
		}
		for(k=0; k<DIM; k++) {
			wp[0].p[k] = 0.0;											// 原点とﾍﾞｸﾄﾙの面
			wp[1].p[k] = ovc.p[k]*1000.0;
			wp[2].p[k] = nvc.p[k]*1000.0;
		}
		CalcNVec(wp, 3, &rvc);											// 回転軸取得
		if(fabs(ovc.p[0]-nvc.p[0])>EPSILON||
           fabs(ovc.p[1]-nvc.p[1])>EPSILON||
           fabs(ovc.p[2]-nvc.p[2])>EPSILON) {							// 回転角取得
			th = acos(ovc.p[0]*nvc.p[0]+ovc.p[1]*nvc.p[1]+ovc.p[2]*nvc.p[2]);
			th = (th<-10||th>10) ? 0.0 : th;
		} else {
			th = 0.0;
		}
		SetRotateMatrix(rvc, th, rot);									// 回転ﾏﾄﾘｯｸｽ作成
		for(j=0; j<vn; j++) {
			VecRotateMove(&pt[j], rot, cp1[i]);							// 回転移動
			pp[i+j*un] = pt[j];											// 断面ﾃﾞｰﾀ格納
		}
	}
	delete[] pt;
}

/****************************************************************/
void MoveBPoint(int md, PNTTYPE* cpt, PNTTYPE* ma, OBJTYPE* op,
                int un, int vn, int ub, int vb, double wt,
                int vflg, int wn, int* ia, int* ib, PNTTYPE* pa,
                PNTTYPE* pb, PNTTYPE* pc, int* ta)						//<<< ﾍﾞｰｽ線移動
/****************************************************************/
{
	PNTTYPE* wma = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* wcp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* wpb = new PNTTYPE[View->m_MaxSCtA];
	int*     flg = new int[View->m_MaxCCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  zro;
	int      i, j, k, ten, p1, p2, st, sct, ect, sno, eno, mx=0, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// 張力設定番号取得
	BaseCmdPtr(1, op, &cmp);											// 有効先頭ｺﾏﾝﾄﾞ取得
	NextCmdPtr(&cmp);													// 曲線編集後ｺﾏﾝﾄﾞ
	while(cmp!=NULL) {
		mx++;															// ｺﾏﾝﾄﾞ個数ｶｳﾝﾄ
		NextCmdPtr(&cmp);												// 次ｺﾏﾝﾄﾞへ
	}
	zro.p[0] = 0.0, zro.p[1] = 0.0, zro.p[2] = 0.0;						// 座標値:0配列作成
	for(i=0; i<un; i++) {												// 作業領域初期化
		wcp[i] = zro, flg[i] = 0;
	}
	st = (vflg%2) ? 1 : 0;												// V始点同一は+1
	for(i=0; i<wn; i++) {
		wma[i+st] = ma[i];												// 移動量をV断面位置に設定
	}
	wma[0] = (vflg%2) ? wma[1] : wma[0];
	wma[un-1] = (vflg/2) ? wma[un-2] : wma[un-1];
	for(i=0; i<wn; i++) {
		if(ib[i]) {														// 対象ﾌﾗｸﾞON/移動量設定
			flg[i+st] = 1, wcp[i+st] = ma[i];
		}
	}
	if(vflg%2) {
		flg[0] = flg[1], wcp[0] = wcp[1];
	}
	if(vflg/2) {
		flg[un-1] = flg[un-2], wcp[un-1] = wcp[un-2];
	}
	if(ten>0) {															// <<張力設定有の場合>>
		for(i=0, j=0, p1=-1, p2=-1; i<un-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <前後ﾌﾗｸﾞ異>
				j++;													// 指定点境界ｶｳﾝﾄ
				if(flg[i]) {											// 境界1位置
					p1 = i+1;
				} else {												// 境界2位置
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <指定点連続>
			for(i=0; i<un; i++) {
				len[i] = (i==un-1) ? PntD(&cpt[i], &cpt[0])
                                   : PntD(&cpt[i], &cpt[i+1]);			// 制御点間距離測定
				for(ok=0, j=0; j<vn; j++) {								// 張力有効確認
					if(ta[i+un*j]) {									// 1点でも有効でOK
						ok = 1; break;
					}
				}
				wta[i] = ok;
			}
			if(p1==-1) {												// 指定点前後位置&点数設定
				if(ub) {
					sct = (p2+1)/2, ect = (p2+1)/2;
					sno = p2, eno = un-1;
				} else {
					sct = p2+1, ect = 0;
					sno = p2, eno = un-1;
				}
			} else if(p2==-1) {
				if(ub) {
					sct = (un-p1)/2, ect = (un-p1)/2;
					sno = un-1, eno = p1-1;
				} else {
					sct = 0, ect = un-p1;
					sno = un-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(ub) {
					sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
					sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = un-p1;
					sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
				sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <指定点前制御点移動>
				lst = (wta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j-1<0) ? un-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			lst = (lst-1<0) ? un-1 : lst-1;								// 有効点前線分も含む
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j-1<0) ? un-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// 指定制御点からの距離
				if(wta[j]) {											// <張力有効点>
					wcp[j] = wma[j];
					GetTensionPntValue(ten, sln, wln, &wcp[j]);			// 張力設定値変換
				}
				j = (j-1<0) ? un-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <指定点後制御点移動>
				lst = (wta[j]) ? j : lst;								// 最終張力有効点検索
				j = (j+1>un-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// 張力有効点有無
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>un-1) ? 0 : j+1;
				sln = (ok||wta[k]) ? sln+len[j] : sln;					// 制御点間距離集計
				ok = (j==lst) ? 0 : ok;									// 最終張力有効点以降は無視
				j = (j+1>un-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// 指定制御点からの距離
				j = (j+1>un-1) ? 0 : j+1;
				if(wta[j]) {											// <張力有効点>
					wcp[j] = wma[j];
					GetTensionPntValue(ten, sln, wln, &wcp[j]);			// 張力設定値変換
				}
			}
		}
	}
	for(i=0; i<un; i++) {
		for(j=0; j<DIM; j++) {
			wcp[i].p[j] += cpt[i].p[j];									// 移動後位置算出
		}
	}
	sct = un*vn;
	for(i=0; i<sct; i++) {
		wpb[i] = pb[i];													// 現在制御点複写
	}
	CalcBaseLine(wpb, un, vn, ub, vflg, wcp, cpt);						// ﾍﾞｰｽ線移動計算
	CalcSurfacePoint(md, 0, 0, un, vn, ub, vb, wt, ia, wpb, wma);		// 通過点=>制御点変換
	for(i=0; i<sct; i++) {
		MoveCtrlPoint(op, pa[i], wma[i], mx, 1, &pc[i]);				// 制御点移動
	}
	delete[] wma;
	delete[] wcp;
	delete[] wpb;
	delete[] flg;
	delete[] wta;
	delete[] len;
}
