/////////////////////////////////////////////////////////////////////////////
// トラッカークラス(CTracker)定義
// Tracker.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "NumFunc.h"

/////////////////////////////////////////////////////////////////////////////
// CTracker

/************************/
CTracker::CTracker(void)												//<<< ｸﾗｽの構築
/************************/
{
	SetFlg = FALSE;
	PntSize = 5.0f;
	Lsno = 0, Mode = 0, Disp = 0;
	XLimit1 = 1.0-DBL_MAX, XLimit2 = DBL_MAX;
	YLimit1 = 1.0-DBL_MAX, YLimit2 = DBL_MAX;
	ZLimit1 = 1.0-DBL_MAX, ZLimit2 = DBL_MAX;
}

/***************************************************/
void CTracker::Setup(PNTTYPE* pa, int* ia, int* op,
                     int pn, int mod, int dsp)							//<<< ﾄﾗｯｶｰ設定
/***************************************************/
{
	PNTTYPE p1, p2;
	int     i, j, pl, st=0;
	double  fv, sz, rr, th, d1, d2, rc[6], gc[6], bc[6];

	if(pn<1) {
		return;
	}
	MainWnd->GetColor(MainWnd->TrackerColor0, &rc[0], &gc[0], &bc[0]);
	MainWnd->GetColor(MainWnd->TrackerColor1, &rc[1], &gc[1], &bc[1]);
	MainWnd->GetColor(MainWnd->TrackerColor2, &rc[2], &gc[2], &bc[2]);
	MainWnd->GetColor(MainWnd->TrackerColor3, &rc[3], &gc[3], &bc[3]);
	MainWnd->GetColor(MainWnd->TrackerColor4, &rc[4], &gc[4], &bc[4]);
	MainWnd->GetColor(MainWnd->TrackerColor5, &rc[5], &gc[5], &bc[5]);
	Lsno = glGenLists(1);												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	glNewList(Lsno, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		if(dsp==1||dsp==2) {
			glColor3d(rc[ia[0]], gc[ia[0]], bc[ia[0]]);					// 表示色設定
			fv = View->GetDispHeight()/40.0;							// 平面表示高(mm単位)の1/40
			glBegin(GL_LINES);											// 十字表示
				glVertex3d(pa[0].p[0]-fv, pa[0].p[1], pa[0].p[2]);		// 座標設定
				glVertex3d(pa[0].p[0]+fv, pa[0].p[1], pa[0].p[2]);
				glVertex3d(pa[0].p[0], pa[0].p[1]-fv, pa[0].p[2]);		// 座標設定
				glVertex3d(pa[0].p[0], pa[0].p[1]+fv, pa[0].p[2]);
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]-fv);		// 座標設定
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]+fv);
			glEnd();
			glPointSize(3.0f);											// ﾄﾗｯｶｰ点ｻｲｽﾞ設定
			glBegin(GL_POINTS);
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);			// 座標設定
			glEnd();
			st = 1;
		}
		if((dsp!=1&&dsp!=2)||pn>1) {
			if(dsp==0) {												// ﾄﾗｯｶｰ点ｻｲｽﾞ設定
				PntSize = 5.0f;
				glPointSize(PntSize);
			} else if(dsp==1) {
				PntSize = 7.0f;
				glPointSize(PntSize);
			} else if(dsp==2||dsp==3) {
				PntSize = 3.0f;
				glPointSize(PntSize);
			} else {
				PntSize = (float)dsp;
				glPointSize(PntSize);
			}
			if(op==NULL) {												// <<表示ｵﾌﾟｼｮﾝなし>>
				glBegin(GL_POINTS);
				for(i=st; i<pn; i++) {
					if(XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
                       YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
                       ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2) {		// <限定範囲内>
						glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);		// 表示色設定
						glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);	// 座標設定
					}
				}
				glEnd();
			} else {													// <<表示ｵﾌﾟｼｮﾝあり>>
				pl = View->GetPlSize();									// 平面高(ﾋﾟｸｾﾙ単位)取得
				fv = View->GetDispHeight();								// 平面表示高(mm単位)取得
				sz = (PntSize/2.0)*fv/pl;								// 点ｻｲｽﾞ3D変換
				for(i=st; i<pn; i++) {
					glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);			// 表示色設定
					if(op[i]==1) {										// <表示ｵﾌﾟｼｮﾝ:1>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==2) {								// <表示ｵﾌﾟｼｮﾝ:2>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==3) {								// <表示ｵﾌﾟｼｮﾝ:3>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==4) {								// <表示ｵﾌﾟｼｮﾝ:4>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==5) {								// <表示ｵﾌﾟｼｮﾝ:5>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==6) {								// <表示ｵﾌﾟｼｮﾝ:6>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==7) {								// <表示ｵﾌﾟｼｮﾝ:7>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
							glEnd();
						}
					} else if(op[i]==8) {								// <表示ｵﾌﾟｼｮﾝ:8>
						glColor3d(rc[5], gc[5], bc[5]);					// 表示色設定
						glLineWidth(4.0f);								// 線幅設定
						if(mod==0||mod>3) {								// XY平面用
							p1 = pa[0], p2 = pa[i];
							p1.p[2] = 0.0, p2.p[2] = 0.0;
							rr = PntD(&p1, &p2);						// 2点間距離
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// 分解位置の角度
									d1 = pa[0].p[0]+rr*cos(th);			// 分解位置
									d2 = pa[0].p[1]+rr*sin(th);
									glVertex3d(d1, d2, pa[i].p[2]);		// POLYLINE頂点登録
								}
							glEnd();
						}
						if(mod==1||mod>3) {								// YZ平面用
							p1 = pa[0], p2 = pa[i];
							p1.p[0] = 0.0, p2.p[0] = 0.0;
							rr = PntD(&p1, &p2);						// 2点間距離
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// 分解位置の角度
									d1 = pa[0].p[1]+rr*cos(th);			// 分解位置
									d2 = pa[0].p[2]+rr*sin(th);
									glVertex3d(pa[i].p[0], d1, d2);		// POLYLINE頂点登録
								}
							glEnd();
						}
						if(mod==2||mod>3) {								// ZX平面用
							p1 = pa[0], p2 = pa[i];
							p1.p[1] = 0.0, p2.p[1] = 0.0;
							rr = PntD(&p1, &p2);						// 2点間距離
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// 分解位置の角度
									d1 = pa[0].p[2]+rr*cos(th);			// 分解位置
									d2 = pa[0].p[0]+rr*sin(th);
									glVertex3d(d2, pa[i].p[1], d1);		// POLYLINE頂点登録
								}
							glEnd();
						}
						glLineWidth(1.0f);								// 線幅設定
					} else if(op[i]==9) {								// <表示ｵﾌﾟｼｮﾝ:9>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==10) {								// <表示ｵﾌﾟｼｮﾝ:10>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
							glEnd();
						}
					} else if(op[i]==11) {								// <表示ｵﾌﾟｼｮﾝ:11>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
							glEnd();
						}
					} else if(op[i]==12) {								// <表示ｵﾌﾟｼｮﾝ:12>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ平面用
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX平面用
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					}
				}
				for(i=st; i<pn; i++) {
					glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);			// 表示色設定
					glBegin(GL_POINTS);
						glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);	// 座標設定
					glEnd();
				}
			}
		}
		glPointSize(1.0f);												// ﾄﾗｯｶｰ点ｻｲｽﾞ標準
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	Mode = mod, Disp = dsp;
	SetFlg = TRUE;														// ﾄﾗｯｶｰ設定ﾌﾗｸﾞON
}

/*****************************/
void CTracker::Reset(int flg)											//<<< ﾄﾗｯｶｰ･ﾘｾｯﾄ
/*****************************/
{
	SetFlg = FALSE;														// ﾄﾗｯｶｰ設定ﾌﾗｸﾞOFF
	if(Lsno!=0) {
		glDeleteLists(Lsno, 1);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ削除
	}
	PntSize = 5.0f;
	Lsno = 0, Mode = 0, Disp = 0;
	if(flg) {															// <最終ﾘｾｯﾄ>
		XLimit1 = 1.0-DBL_MAX, XLimit2 = DBL_MAX;						// 範囲限定もﾘｾｯﾄ
		YLimit1 = 1.0-DBL_MAX, YLimit2 = DBL_MAX;
		ZLimit1 = 1.0-DBL_MAX, ZLimit2 = DBL_MAX;
	}
}

/********************************************************************/
void CTracker::Limit(double x, double y, double z,
                     int x1, int x2, int y1, int y2, int z1, int z2)	//<<< ﾄﾗｯｶｰ範囲限定
/********************************************************************/
{
	XLimit1 = (x1) ? 1.0-DBL_MAX : x-0.0001;							// X 最小側限定
	XLimit2 = (x2) ? DBL_MAX : x+0.0001;								// X 最大側限定
	YLimit1 = (y1) ? 1.0-DBL_MAX : y-0.0001;							// Y 最小側限定
	YLimit2 = (y2) ? DBL_MAX : y+0.0001;								// Y 最大側限定
	ZLimit1 = (z1) ? 1.0-DBL_MAX : z-0.0001;							// Z 最小側限定
	ZLimit2 = (z2) ? DBL_MAX : z+0.0001;								// Z 最大側限定
}

/******************************/
void CTracker::Draw(int plane)											//<<< ﾄﾗｯｶｰ描画
/******************************/
{
	if(Mode==plane||Mode==4||(Mode==5&&plane!=PRS)) {
		glCallList(Lsno);												// ﾄﾗｯｶｰﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ呼出し
	}
}

/*****************************************************************/
int CTracker::HitTracker(int flg, int plane,
                         const PNTTYPE& pnt, PNTTYPE* pa, int pn)		//<<< ﾄﾗｯｶｰ選択(3面図用)
/*****************************************************************/
{
	int    plsize, pln2, i, st, ed;
	double fovy, psize;

	plsize = View->GetPlSize();											// 平面高(ﾋﾟｸｾﾙ単位)取得
	fovy = View->GetDispHeight();										// 平面表示高(mm単位)取得
	psize = fovy*PntSize/plsize;										// 点ｻｲｽﾞ3D変換
	pln2 = (plane+1)%3;
	st = (Disp==1) ? 1 : 0;												// 開始位置設定
	ed = (Disp==2) ? 0 : pn;											// 終了位置設定
	for(i=st; i<ed; i++) {
		if(flg||
           (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
            YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
            ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {					// <限定範囲内>
			if(pnt.p[plane]-psize<=pa[i].p[plane]&&
               pa[i].p[plane]<=pnt.p[plane]+psize&&
               pnt.p[pln2]-psize<=pa[i].p[pln2]&&
               pa[i].p[pln2]<=pnt.p[pln2]+psize) {
				return i;												// 頂点要素番号RETURN
			}
		}
	}
	return -1;															// 該当頂点無
}

/*****************************************************************/
int CTracker::HitTrackerS(int flg, int plane, const PNTTYPE& pnt,
                          PNTTYPE* pa, int pn, int* ha)					//<<< ﾄﾗｯｶｰ(複数)選択(3面図用)
/*****************************************************************/
{
	int    plsize, pln2, i, j, k, st, ed, cnt;
	double fovy, psize;

	plsize = View->GetPlSize();											// 平面高(ﾋﾟｸｾﾙ単位)取得
	fovy = View->GetDispHeight();										// 平面表示高(mm単位)取得
	psize = fovy*PntSize/plsize;										// 点ｻｲｽﾞ3D変換
	pln2 = (plane+1)%3;
	st = (Disp==1) ? 1 : 0;												// 開始位置設定
	ed = (Disp==2) ? 0 : pn;											// 終了位置設定
	for(i=st, cnt=0; i<ed; i++) {
		if(flg||
           (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
            YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
            ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {					// <限定範囲内>
			if(pnt.p[plane]-psize<=pa[i].p[plane]&&
               pa[i].p[plane]<=pnt.p[plane]+psize&&
               pnt.p[pln2]-psize<=pa[i].p[pln2]&&
               pa[i].p[pln2]<=pnt.p[pln2]+psize) {
				ha[cnt++] = i;											// 頂点要素番号
			}
		}
	}
	for(i=0; i<cnt; i++) {
		for(j=i+1; j<cnt; j++) {										// 同一頂点分ｶｯﾄ
			if(fabs(pa[ha[i]].p[0]-pa[ha[j]].p[0])<EPSILON&&
               fabs(pa[ha[i]].p[1]-pa[ha[j]].p[1])<EPSILON&&
               fabs(pa[ha[i]].p[2]-pa[ha[j]].p[2])<EPSILON) {
				for(k=j; k<cnt-1; k++) {
					ha[k] = ha[k+1];
				}
				cnt--, j--;
			}
		}
	}
	return cnt;															// 該当頂点数
}

/********************************************************/
int CTracker::HitTracker3D(int flg, const CPoint& point,
                           PNTTYPE* pa, int pn)							//<<< ﾄﾗｯｶｰ選択(透視図用)
/********************************************************/
{
	double*  depth = new double[pn];
	int*     hnum = new int[pn];
	GLdouble modelMat[16], projMat[16], x, y;
	GLint    viewport[4];
	double   min_depth, psize=PntSize/2.0+0.5;
	int      i, st, ed, cnt, hpnum=-1;

	glPushMatrix();
		glCallList(DL_PARS);											// 透視図用変換呼出
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);					// 現在ﾓﾃﾞﾙﾋﾞｭｰ行列取得
		glGetDoublev(GL_PROJECTION_MATRIX, projMat);					// 現在射影行列取得
		View->GetViewport(PRS, &viewport[0], &viewport[1],
                          &viewport[2], &viewport[3]);
		st = (Disp==1) ? 1 : 0;											// 開始位置設定
		ed = (Disp==2) ? 0 : pn;										// 終了位置設定
		for(i=st, cnt=0; i<ed; i++) {									// 配列点座標->ｳｨﾝﾄﾞｳ座標
			if(flg||
               (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
                YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
                ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {				// <限定範囲内>
				gluProject(pa[i].p[0], pa[i].p[1], pa[i].p[2], modelMat,
                           projMat, viewport, &x, &y, &depth[i]);
				y = View->ViewSize.cy - y;
				if(point.x<=x+psize&&x-psize<=point.x&&
                   point.y<=y+psize&&y-psize<=point.y) {
					hnum[cnt++] = i;									// 該当頂点保存
				}
			}
		}
		if(cnt>0) {
			hpnum = hnum[0];											// 初期値取得
			min_depth = depth[hpnum];									// 奥行取得
			for(i=1; i<cnt; i++) {
				if(min_depth>=depth[hnum[i]]) {							// 手前頂点の場合
					hpnum = hnum[i];									// 該当頂点番号取得
					min_depth = depth[hpnum];							// 奥行保存
				}
			}
		}
	glPopMatrix();
	delete[] depth;
	delete[] hnum;
	return hpnum;														// 該当頂点番号
}
