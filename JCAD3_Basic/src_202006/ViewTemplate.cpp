/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewTemplate.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataConv.h"
#include "NumFunc.h"
#include "GLTF32_Api.h"

/*********************************/
void CJcad3GlbView::SetGrid(void)										//<<< ｸﾞﾘｯﾄﾞ設定
/*********************************/
{
	PNTTYPE pnttl, pntbr;
	SCRTYPE scrtl, scrbr;
	int     i, j, i1, j1;
	double  wi, wj;

	if(m_Fovy/m_GridSpace>ViewSize.cy/10) {
		return;
	}
	CalcScreenWorld(1, &pnttl, PlRect[XY].TopLeft(), XY);				// XY 通常ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[XY].BottomRight(), XY);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[0] = (scrtl.p[0]/m_GridSpace)*m_GridSpace;
	scrtl.p[1] = (scrtl.p[1]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDXY, GL_COMPILE);
		glPointSize(1.2f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[0]; i<scrbr.p[0]; i+=m_GridSpace) {
				for(j=scrtl.p[1]; j<scrbr.p[1]; j+=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(i1||j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(wi, wj, 0.0);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
	CalcScreenWorld(1, &pnttl, PlRect[XY].TopLeft(), XY);				// XY 目盛ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[XY].BottomRight(), XY);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[0] = (scrtl.p[0]/m_GridSpace)*m_GridSpace;
	scrtl.p[1] = (scrtl.p[1]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDXY2, GL_COMPILE);
		glPointSize(1.5f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[0]; i<scrbr.p[0]; i+=m_GridSpace) {
				for(j=scrtl.p[1]; j<scrbr.p[1]; j+=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(!i1&&!j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(wi, wj, 0.0);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
	CalcScreenWorld(1, &pnttl, PlRect[YZ].TopLeft(), YZ);				// YZ 通常ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[YZ].BottomRight(), YZ);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[1] = (scrtl.p[1]/m_GridSpace)*m_GridSpace;
	scrtl.p[2] = (scrtl.p[2]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDYZ, GL_COMPILE);
		glPointSize(1.2f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[2]; i>scrbr.p[2]; i-=m_GridSpace) {
				for(j=scrtl.p[1]; j<scrbr.p[1]; j+=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(i1||j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(0.0, wj, wi);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
	CalcScreenWorld(1, &pnttl, PlRect[YZ].TopLeft(), YZ);				// YZ 目盛ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[YZ].BottomRight(), YZ);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[1] = (scrtl.p[1]/m_GridSpace)*m_GridSpace;
	scrtl.p[2] = (scrtl.p[2]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDYZ2, GL_COMPILE);
		glPointSize(1.5f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[2]; i>scrbr.p[2]; i-=m_GridSpace) {
				for(j=scrtl.p[1]; j<scrbr.p[1]; j+=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(!i1&&!j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(0.0, wj, wi);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
	CalcScreenWorld(1, &pnttl, PlRect[ZX].TopLeft(), ZX);				// ZX 通常ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[ZX].BottomRight(), ZX);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[2] = (scrtl.p[2]/m_GridSpace)*m_GridSpace;
	scrtl.p[0] = (scrtl.p[0]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDZX, GL_COMPILE);
		glPointSize(1.2f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[2]; i>scrbr.p[2]; i-=m_GridSpace) {
				for(j=scrtl.p[0]; j>scrbr.p[0]; j-=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(i1||j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(wj, 0.0, wi);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
	CalcScreenWorld(1, &pnttl, PlRect[ZX].TopLeft(), ZX);				// ZX 目盛ｸﾞﾘｯﾄﾞ
	CalcScreenWorld(1, &pntbr, PlRect[ZX].BottomRight(), ZX);
	ScrToPnt(1, &scrtl, &pnttl);
	ScrToPnt(1, &scrbr, &pntbr);
	scrtl.p[2] = (scrtl.p[2]/m_GridSpace)*m_GridSpace;
	scrtl.p[0] = (scrtl.p[0]/m_GridSpace)*m_GridSpace;
	glNewList(DL_GRIDZX2, GL_COMPILE);
		glPointSize(1.5f);
		glBegin(GL_POINTS);
			for(i=scrtl.p[2]; i>scrbr.p[2]; i-=m_GridSpace) {
				for(j=scrtl.p[0]; j>scrbr.p[0]; j-=m_GridSpace) {
					i1 = i/m_GridSpace, i1 = i1%m_GridIntvl;
					j1 = j/m_GridSpace, j1 = j1%m_GridIntvl;
					if(!i1&&!j1) {
						wi = i/1000.0, wj = j/1000.0;
						glVertex3d(wj, 0.0, wi);
					}
				}
			}
		glEnd();
		glPointSize(1.0f);
	glEndList();
}

/*********************************/
void CJcad3GlbView::SetBase(void)										//<<< 基準点/線設定
/*********************************/
{
	double x1, y1, z1, x2, y2, z2, rc, gc, bc;

	MainWnd->GetColor(MainWnd->BaseColor, &rc, &gc, &bc);
	x1 = m_Bpoint.p[0]/1000.0;
	y1 = m_Bpoint.p[1]/1000.0;
	z1 = m_Bpoint.p[2]/1000.0;
	glNewList(DL_BASEPOINT, GL_COMPILE);
		glColor3d(rc, gc, bc);											// 表示色設定
		glPointSize(1.5f);												// 点ｻｲｽﾞを拡大
		glBegin(GL_POINTS);												// 頂点座標登録
			glVertex3d(x1, y1, z1);
		glEnd();
		glPointSize(1.0f);												// 点ｻｲｽﾞを標準に戻す
	glEndList();
	x1 = m_Blspnt.p[0]/1000.0;
	y1 = m_Blspnt.p[1]/1000.0;
	z1 = m_Blspnt.p[2]/1000.0;
	x2 = m_Blepnt.p[0]/1000.0;
	y2 = m_Blepnt.p[1]/1000.0;
	z2 = m_Blepnt.p[2]/1000.0;
	glNewList(DL_BASELINE, GL_COMPILE);
		glColor3d(rc, gc, bc);											// 表示色設定
		glBegin(GL_LINES);												// 線分座標登録
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		glEnd();
		glPointSize(1.5f);												// 点ｻｲｽﾞを拡大
		glBegin(GL_POINTS);												// 頂点座標登録
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		glEnd();
		glPointSize(1.0f);												// 点ｻｲｽﾞを標準に戻す
	glEndList();
}

/*************************************/
void CJcad3GlbView::SetXYZGuide(void)									//<<< XYZｶﾞｲﾄﾞ設定
/*************************************/
{
	GLdouble modelMat[16];
	GLdouble projMat[16];
	GLint    vprt[4];
	PNTTYPE  pnt, ept;
	double   wx, wy, wz, ox, oy, oz, ln, rc, gc, bc;

	glPushMatrix();
		glCallList(DL_PARS);											// 透視図用変換呼び出し
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);					// 現在のﾓﾃﾞﾙﾋﾞｭｰ行列取得
		glGetDoublev(GL_PROJECTION_MATRIX, projMat);					// 現在の射影行列取得
		GetViewport(PRS, &vprt[0], &vprt[1], &vprt[2], &vprt[3]);		// ﾋﾞｭｰﾎﾟｰﾄ取得
		wx = ViewSize.cx*19.0/20.0;										// 右上からXの1/20の距離
		wy = ViewSize.cy-ViewSize.cx/20.0;
		wz = 0.1;
		gluUnProject(wx, wy, wz, modelMat, projMat, vprt, &ox, &oy, &oz);// ｵﾌﾞｼﾞｪｸﾄ座標に変換
	glPopMatrix();
	pnt.p[0] = ox, pnt.p[1] = oy, pnt.p[2] = oz;						// 変換位置を変換
	ScrToPnt(0, &m_Eye, &ept);
	ln = PntD(&ept, &pnt)/20.0;											// 視点→変換位置距離
	glNewList(DL_XYZGUIDE, GL_COMPILE);
		glBegin(GL_LINES);
			MainWnd->GetColor(MainWnd->XGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// 表示色設定
			glVertex3d(ox-ln, oy, oz);									// X座標軸案内
			glVertex3d(ox+ln, oy, oz);
			MainWnd->GetColor(MainWnd->YGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// 表示色設定
			glVertex3d(ox, oy-ln, oz);									// Y座標軸案内
			glVertex3d(ox, oy+ln, oz);
			MainWnd->GetColor(MainWnd->ZGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// 表示色設定
			glVertex3d(ox, oy, oz-ln);									// Z座標軸案内
			glVertex3d(ox, oy, oz+ln);
		glEnd();
	glEndList();
}

/*****************************************/
BOOL CJcad3GlbView::SetGLTPoint(FILE* fp)								//<<< GLTﾎﾟｲﾝﾄ登録
/*****************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	float  ps;
	double vt[3];

	ret = (fscanf_s(fp, "%f", &ps)!=1) ? FALSE : TRUE;					// ﾎﾟｲﾝﾄｻｲｽﾞ取得
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// 座標取得
	vt[Wi1] = atof(b1), vt[Wi2] = atof(b2), vt[Wi3] = 0.0;				// 実数変換
	if(ret) {
		glPointSize(ps);												// 点ｻｲｽﾞ拡大
		glBegin(GL_POINTS);
			glVertex3d(vt[0], vt[1], vt[2]);							// 頂点座標登録
		glEnd();
		glPointSize(1.0f);												// 点ｻｲｽﾞ標準
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/****************************************/
BOOL CJcad3GlbView::SetGLTLine(FILE* fp)								//<<< GLTﾗｲﾝ登録
/****************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3];

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// 座標1取得
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// 実数変換
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// 座標2取得
	v2[Wi1] = atof(b1), v2[Wi2] = atof(b2), v2[Wi3] = 0.0;				// 実数変換
	if(ret) {
		glBegin(GL_LINES);
			glVertex3d(v1[0], v1[1], v1[2]);							// LINE頂点登録
			glVertex3d(v2[0], v2[1], v2[2]);
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/********************************************/
BOOL CJcad3GlbView::SetGLTPolyline(FILE* fp)							//<<< GLTﾎﾟﾘﾗｲﾝ登録
/********************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	int    i, vn;
	double vt[3];

	ret = (fscanf_s(fp, "%d", &vn)!=1) ? FALSE : TRUE;					// 頂点数取得
	if(ret) {
		glBegin(GL_LINE_STRIP);
			for(i=0; i<vn; i++) {
				if(fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) {			// 座標取得
					ret = FALSE; break;
				}
				vt[Wi1] = atof(b1), vt[Wi2] = atof(b2), vt[Wi3] = 0.0;	// 実数変換
				glVertex3d(vt[0], vt[1], vt[2]);						// POLYLINE頂点登録
			}
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/***************************************/
BOOL CJcad3GlbView::SetGLTBox(FILE* fp)									//<<< GLTﾎﾞｯｸｽ登録
/***************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3], v3[3], v4[3];

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// 最小座標取得
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// 実数変換
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// 最大座標取得
	v3[Wi1] = atof(b1), v3[Wi2] = atof(b2), v3[Wi3] = 0.0;				// 実数変換
	v2[Wi1] = v3[Wi1],  v2[Wi2] = v1[Wi2],  v2[Wi3] = 0.0;
	v4[Wi1] = v1[Wi1],  v4[Wi2] = v3[Wi2],  v4[Wi3] = 0.0;
	if(ret) {
		glBegin(GL_LINE_STRIP);
			glVertex3d(v1[0], v1[1], v1[2]);							// POLYLINE頂点登録
			glVertex3d(v2[0], v2[1], v2[2]);
			glVertex3d(v3[0], v3[1], v3[2]);
			glVertex3d(v4[0], v4[1], v4[2]);
			glVertex3d(v1[0], v1[1], v1[2]);
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/******************************************/
BOOL CJcad3GlbView::SetGLTSquare(FILE* fp)								//<<< GLT桝目登録
/******************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3], v3[3], v4[3], vs[3], ve[3], p1, p2, ps;

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// 最小座標取得
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// 実数変換
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// 最大座標取得
	v3[Wi1] = atof(b1), v3[Wi2] = atof(b2), v3[Wi3] = 0.0;				// 実数変換
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// 桝目幅1,2取得
	p1 = atof(b1), p2 = atof(b2);										// 実数変換
	v2[Wi1] = v3[Wi1], v2[Wi2] = v1[Wi2], v2[Wi3] = 0.0;
	v4[Wi1] = v1[Wi1], v4[Wi2] = v3[Wi2], v4[Wi3] = 0.0;
	if(ret) {
		glBegin(GL_LINE_STRIP);
			glVertex3d(v1[0], v1[1], v1[2]);							// POLYLINE頂点登録
			glVertex3d(v2[0], v2[1], v2[2]);
			glVertex3d(v3[0], v3[1], v3[2]);
			glVertex3d(v4[0], v4[1], v4[2]);
			glVertex3d(v1[0], v1[1], v1[2]);
		glEnd();
		vs[0] = v1[0], vs[1] = v1[1], vs[2] = v1[2];					// Wi1方向に移動しながら
		ve[0] = v4[0], ve[1] = v4[1], ve[2] = v4[2];
		for(ps=v1[Wi1]+p1; ps<=v3[Wi1]; ps+=p1) {						// ﾎﾞｯｸｽ内に桝目
			if(ps+0.001<v3[Wi1]) {										// 最大位置のﾗｲﾝが重ならない
				vs[Wi1] = ps, ve[Wi1] = ps;
				glBegin(GL_LINES);
					glVertex3d(vs[0], vs[1], vs[2]);					// LINE頂点登録
					glVertex3d(ve[0], ve[1], ve[2]);
				glEnd();
			}
		}
		vs[0] = v1[0], vs[1] = v1[1], vs[2] = v1[2];					// Wi2方向に移動しながら
		ve[0] = v2[0], ve[1] = v2[1], ve[2] = v2[2];
		for(ps=v1[Wi2]+p2; ps<=v3[Wi2]; ps+=p2) {						// ﾎﾞｯｸｽ内に桝目
			if(ps+0.001<v3[Wi2]) {										// 最大位置のﾗｲﾝが重ならない
				vs[Wi2] = ps, ve[Wi2] = ps;
				glBegin(GL_LINES);
					glVertex3d(vs[0], vs[1], vs[2]);					// LINE頂点登録
					glVertex3d(ve[0], ve[1], ve[2]);
				glEnd();
			}
		}
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/***************************************/
BOOL CJcad3GlbView::SetGLTArc(FILE* fp)									//<<< GLT円弧登録
/***************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	char*  b3 = new char[100];
	BOOL   ret;
	int    i, dv;
	double cp[3], vt[3], rr, st, et, sth, eth, ath, th;

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// 中心座標取得
	cp[Wi1] = atof(b1), cp[Wi2] = atof(b2), cp[Wi3] = 0.0;				// 実数変換
	ret = (fscanf_s(fp, "%s%s%s%d", b1, 100, b2, 100, b3, 100, &dv)!=4)	// 半径,開始/終了角度,分解能
          ? FALSE : ret;
	rr = atof(b1), st = atof(b2), et = atof(b3);						// 実数変換
	vt[Wi3] = 0.0;
	if(ret) {
		sth = PI*st/180.0;												// 開始角度(ﾗｼﾞｱﾝ)
		eth = PI*et/180.0;												// 終了角度(ﾗｼﾞｱﾝ)
		ath = (eth-sth)/dv;												// 1分解の角度(ﾗｼﾞｱﾝ)
		glBegin(GL_LINE_STRIP);
			for(i=0; i<dv; i++) {
				th = sth + ath*i;										// 分解位置の角度
				vt[Wi1] = cp[Wi1]+rr*cos(th);							// 分解位置
				vt[Wi2] = cp[Wi2]+rr*sin(th);
				glVertex3d(vt[0], vt[1], vt[2]);						// POLYLINE頂点登録
			}
			vt[Wi1] = cp[Wi1]+rr*cos(eth);								// 最終位置
			vt[Wi2] = cp[Wi2]+rr*sin(eth);
			glVertex3d(vt[0], vt[1], vt[2]);
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	delete[] b3;
	return ret;
}

/********************************************/
static CString GetGlbFile(int no, int plane)							//<<< GLBﾌｧｲﾙ名取得
/********************************************/
{
	CString name("");

	if(no==1) {
		if(plane==0)      { name = G1X_NAME; }							// XY平面用
		else if(plane==1) { name = G1Y_NAME; }							// YZ平面用
		else              { name = G1Z_NAME; }							// ZX平面用
	} else if(no==2) {
		if(plane==0)      { name = G2X_NAME; }							// XY平面用
		else if(plane==1) { name = G2Y_NAME; }							// YZ平面用
		else              { name = G2Z_NAME; }							// ZX平面用
	} else if(no==3) {
		if(plane==0)      { name = G3X_NAME; }							// XY平面用
		else if(plane==1) { name = G3Y_NAME; }							// YZ平面用
		else              { name = G3Z_NAME; }							// ZX平面用
	} else if(no==4) {
		if(plane==0)      { name = G4X_NAME; }							// XY平面用
		else if(plane==1) { name = G4Y_NAME; }							// YZ平面用
		else              { name = G4Z_NAME; }							// ZX平面用
	}
	return name;														// ﾌｧｲﾙ名を返す
}

/****************************************************************/
BOOL CJcad3GlbView::ReadGLTTemplate(int mode, int no, int plane)		//<<< GLTﾃﾝﾌﾟﾚｰﾄ入力
/****************************************************************/		// mode 0:GLB,1:ﾃﾝﾌﾟﾚｰﾄ
{
	char*   buf = new char[100];
	FILE*   fp;
	CString fname, tname;
	BOOL    ret;
	UINT    displst;
	int     i, r, g, b;
	double  dm, rc, gc, bc;

	Wi1 = plane;														// 表示平面より座標軸決定
	Wi2 = (Wi1+1>2) ? 0 : Wi1+1;
	Wi3 = (Wi2+1>2) ? 0 : Wi2+1;
	if(m_GlbChange) {													// <ﾊﾟﾗﾒｰﾀ更新あり>
		dm = (!m_RingKind) ? 15.0+(m_RingSize1-7.0)/3.0 : m_RingSize2;	// ﾘﾝｸﾞ直径計算
	} else {															// <ﾊﾟﾗﾒｰﾀ更新なし>
		dm = 0.0;
	}
	fname = (!mode) ? GetAppPath(0)+GetGlbFile(no, plane)				// GLBの場合,GLBﾌｧｲﾙﾊﾟｽ取得
                    : m_TempPath[no][plane];							// ﾃﾝﾌﾟﾚｰﾄの場合,ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙﾊﾟｽ取得
	if(fopen_s(&fp, fname, "r")!=0) {									// GLTﾌｧｲﾙｵｰﾌﾟﾝ
		delete[] buf;
		return FALSE;
	}
	fclose(fp);															// GLTﾌｧｲﾙｸﾛｰｽﾞ
	i = fname.ReverseFind('.');
	tname = fname.Left(i) + ".tmp";										// 拡張子を変更
	if(ChgGLTFile(fname, tname, m_GlbChange, (float)dm)) {				// GLTﾌｧｲﾙ変換
		delete[] buf;
		return FALSE;
	}
	if(fopen_s(&fp, tname, "r")!=0) {									// 作業ﾌｧｲﾙｵｰﾌﾟﾝ
		delete[] buf;
		return FALSE;
	}
	ret = (fscanf_s(fp, "%s%d", buf, 100, &i)!=2) ? FALSE : TRUE;		// 先頭ｺﾏﾝﾄﾞ取得
	ret = (memcmp(buf, "T00", 3)!=0) ? FALSE : ret;						// <開始ｺﾏﾝﾄﾞ(T00)>以外はｴﾗｰ
	if(!ret) {															// ｴﾗｰ発生の場合
		fclose(fp);														// 作業ﾌｧｲﾙｸﾛｰｽﾞ
		delete[] buf;
		return FALSE;													// 中止
	}
	displst = glGenLists(1);											// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	if(!mode) {															// <GLBの場合>
		m_GLB[plane] = displst;
	} else {															// <ﾃﾝﾌﾟﾚｰﾄの場合>
		m_TEMP[plane] = displst;
	}
	glNewList(displst, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		MainWnd->GetColor(MainWnd->TemplateColor, &rc, &gc, &bc);		// 初期としてﾃﾝﾌﾟﾚｰﾄ表示色
		glColor3d(rc, gc, bc);											// 表示色設定
		while(ret) {
			if(fscanf_s(fp, "%s%d", buf, 100, &i)!=2) {					// ｺﾏﾝﾄﾞ取得
				ret = FALSE;
			}
			if(memcmp(buf, "T01", 3)==0) {								// <表示色設定ｺﾏﾝﾄﾞ(T01)>
				ret = (fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3)
                      ? FALSE : ret;									// RGBｶﾗｰ(整数)取得
				rc = r/255.0, gc = g/255.0, bc = b/255.0;
				glColor3d(rc, gc, bc);									// 表示色設定
			} else if(memcmp(buf, "T10", 3)==0) {						// <ﾎﾟｲﾝﾄ表示ｺﾏﾝﾄﾞ(T10)>
				ret = SetGLTPoint(fp);									// GLTﾎﾟｲﾝﾄ登録
			} else if(memcmp(buf, "T11", 3)==0) {						// <ﾗｲﾝ表示ｺﾏﾝﾄﾞ(T11)>
				ret = SetGLTLine(fp);									// GLTﾗｲﾝ登録
			} else if(memcmp(buf, "T12", 3)==0) {						// <ﾎﾟﾘﾗｲﾝ表示ｺﾏﾝﾄﾞ(T12)>
				ret = SetGLTPolyline(fp);								// GLTﾎﾟﾘﾗｲﾝ登録
			} else if(memcmp(buf, "T13", 3)==0) {						// <ﾎﾞｯｸｽ表示ｺﾏﾝﾄﾞ(T13)>
				ret = SetGLTBox(fp);									// GLTﾎﾞｯｸｽ登録
			} else if(memcmp(buf, "T14", 3)==0) {						// <桝目表示ｺﾏﾝﾄﾞ(T14)>
				ret = SetGLTSquare(fp);									// GLT桝目登録
			} else if(memcmp(buf, "T15", 3)==0) {						// <円弧表示ｺﾏﾝﾄﾞ(T15)>
				ret = SetGLTArc(fp);									// GLT円弧登録
			} else if(memcmp(buf, "T99", 3)==0) {						// <終了ｺﾏﾝﾄﾞ(T99)>
				break;
			} else {
				ret = FALSE;
			}
		}
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	remove(tname);														// 作業ﾌｧｲﾙ削除
	delete[] buf;
	Wi0 = 0, Wi1 = 0, Wi2 = 0, Wi3 = 0;
	return ret;
}

/*********************************/
BOOL CJcad3GlbView::ReadGLB(void)										//<<< GLBﾃﾝﾌﾟﾚｰﾄ入力
/*********************************/
{
	BOOL ret=TRUE;
	int  i;

	DeleteGLB();														// 旧GLBﾃﾝﾌﾟﾚｰﾄ破棄
	if(m_GlbDisp==0) {													// GLBﾃﾝﾌﾟﾚｰﾄ表示無は終了
		return TRUE;
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	for(i=0; i<3; i++) {												// <0:X-Y, 1:Y-Z, 2:Z-X>
		if(!(ret=ReadGLTTemplate(0, m_GlbDisp, i))) {					// GLTﾃﾝﾌﾟﾚｰﾄ入力
			break;														// ｴﾗｰは中止
		}
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	return ret;
}

/***********************************/
void CJcad3GlbView::DeleteGLB(void)										//<<< GLBﾃﾝﾌﾟﾚｰﾄ破棄
/***********************************/
{
	int i;

	for(i=0; i<3; i++) {
		if(m_GLB[i]!=0) {
			glDeleteLists(m_GLB[i], 1);
			m_GLB[i] = 0;
		}
	}
}

/***********************************************************************/
void CJcad3GlbView::SetTmpFile(int no, CString x, CString y, CString z)	//<<< ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙﾊﾟｽ設定
/***********************************************************************/
{
	m_TempPath[no][0] = x;
	m_TempPath[no][1] = y;
	m_TempPath[no][2] = z;
}

/******************************************************************/
void CJcad3GlbView::GetTmpFile(int no,
                               CString* x, CString* y, CString* z)		//<<< ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙﾊﾟｽ取得
/******************************************************************/
{
	*x = m_TempPath[no][0];
	*y = m_TempPath[no][1];
	*z = m_TempPath[no][2];
}

/***************************************************/
static BOOL ReadParam(FILE* fp, int* id, char* cmd)						//<<< DXFﾃﾞｰﾀ入力
/***************************************************/
{
	fscanf_s(fp, "%d", id);												// ID入力
	fscanf_s(fp, "%s", cmd, 1000);										// ｺﾏﾝﾄﾞ入力
	if(feof(fp)) {														// ﾌｧｲﾙ終了はFALSE
		return FALSE;
	} else {															// ﾌｧｲﾙ続行はTRUE
		return TRUE;
	}
}

/***************************************************************/
static BOOL ReadPoint(FILE* fp, int* id, char* cmd, double* pt)			//<<< 頂点ﾃﾞｰﾀ入力
/***************************************************************/
{
	int   xf=0, yf=0, zf=0;
	float fd;

	pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
	while(TRUE) {
		if((*id)<10||(*id)>39) {										// 頂点ﾃﾞｰﾀ以外は終了
			break;
		}
		if(sscanf_s(cmd, "%g", &fd)!=1) {								// 浮動小数変換
			return FALSE;
		}
		if((*id)/10==1) {												// <X座標>
			if(xf==1) {
				break;
			} else {
				pt[0] = (double)fd, xf = 1;
			}
		} else if((*id)/10==2) {										// <Y座標>
			if(yf==1) {
				break;
			} else {
				pt[1] = (double)fd, yf = 1;
			}
		} else if((*id)/10==3) {										// <Z座標>
			if(zf==1) {
				break;
			} else {
				pt[2] = (double)fd, zf = 1;
			}
		}
		if(!ReadParam(fp, id, cmd)) {
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************/
BOOL CJcad3GlbView::SetPoint(FILE* fp, int* id, char* cmd)				//<<< POINTﾃﾞｰﾀ登録
/**********************************************************/
{
	double pt[3];
	BOOL   ret=TRUE;

	pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <頂点ﾃﾞｰﾀ>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// 頂点ﾃﾞｰﾀ入力
			break;
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	glPointSize(2.5f);													// 点ｻｲｽﾞ拡大
	glBegin(GL_POINTS);
		glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);							// 頂点座標登録
	glEnd();
	glPointSize(1.0f);													// 点ｻｲｽﾞ標準
	return ret;
}

/*********************************************************/
BOOL CJcad3GlbView::SetLine(FILE* fp, int* id, char* cmd)				//<<< LINEﾃﾞｰﾀ登録
/*********************************************************/
{
	int    i, cnt=0;
	double pt[3], pa[2][3];
	BOOL   ret=TRUE;

	for(i=0; i<3; i++) {
		pa[0][i] = 0.0, pa[1][i] = 0.0;
	}
	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <頂点ﾃﾞｰﾀ>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// 頂点ﾃﾞｰﾀ入力
			for(i=0; i<3; i++) {
				pa[cnt][i] = pt[i];
			}
			if(++cnt>1) {
				break;
			}
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	glBegin(GL_LINES);
		glVertex3d(pa[0][Wi1], pa[0][Wi2], pa[0][Wi3]);					// LINE頂点登録
		glVertex3d(pa[1][Wi1], pa[1][Wi2], pa[1][Wi3]);
	glEnd();
	return ret;
}

/********************************************************/
BOOL CJcad3GlbView::SetArc(FILE* fp, int* id, char* cmd)				//<<< ARCﾃﾞｰﾀ登録
/********************************************************/
{
	int    i, i1, i2, i3, ix;
	double cp[3], pt[3], th, th1, t;
	float  rr, t1, t2;
	BOOL   ret=TRUE;

	cp[0] = 0.0, cp[1] = 0.0, cp[2] = 0.0;
	rr = 0.0f, t1 = 0.0f, t2 = 0.0f;
	i1 = Wi0;															// 有効座標軸決定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <中心点ﾃﾞｰﾀ>
			ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;			// 頂点ﾃﾞｰﾀ入力
		} else if((*id)==40) {											// <半径ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &rr)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
		} else if((*id)==50) {											// <開始角度ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &t1)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
		} else if((*id)==51) {											// <終了角度ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &t2)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			break;
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	if(fabs(t1)>360.0f) {												// 開始角度確認
		ix = (int)t1, i = abs(ix)/360, t1 = t1 - (ix*i);
	}
	if(fabs(t2)>360.0f) {												// 終了角度確認
		ix = (int)t2, i = abs(ix)/360, t2 = t2 - (ix*i);
	}
	th = ((th=(double)(t2-t1))<0.0) ? th + 360.0 : th;					// 開始～終了角度
	ix = ((int)(th/10.0)<3) ? 3 : (int)(th/10.0);						// 角度分解数(基本10度)
	th = th/ix;															// 分解角度(増分)
	th1 = PI*t1/180.0;													// 開始角度(ﾗｼﾞｱﾝ)
	th = PI*th/180.0;													// 分解角度(ﾗｼﾞｱﾝ)
	glBegin(GL_LINE_STRIP);
		for(i=0; i<=ix; i++) {
			t = th1+th*i;
			pt[i1] = cp[i1]+(double)rr*cos(t);
			pt[i2] = cp[i2]+(double)rr*sin(t);
			pt[i3] = cp[i3];
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// ARC頂点登録
		}
	glEnd();
	return ret;
}

/***********************************************************/
BOOL CJcad3GlbView::SetCircle(FILE* fp, int* id, char* cmd)				//<<< CIRCLEﾃﾞｰﾀ登録
/***********************************************************/
{
	int    i, i1, i2, i3;
	double cp[3], pt[3], t, th=PI/18.0;
	float  rr;
	BOOL   ret=TRUE;

	cp[0] = 0.0, cp[1] = 0.0, cp[2] = 0.0, rr = 0.0f;
	i1 = Wi0;															// 有効座標軸決定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <中心点ﾃﾞｰﾀ>
			ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;			// 頂点ﾃﾞｰﾀ入力
		} else if((*id)==40) {											// <半径ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &rr)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			break;
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	glBegin(GL_LINE_STRIP);
		for(i=0; i<36; i++) {
			t = th*i;
			pt[i1] = cp[i1]+(double)rr*cos(t);
			pt[i2] = cp[i2]+(double)rr*sin(t);
			pt[i3] = cp[i3];
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// CIRCLE頂点登録
		}
		pt[i1] = cp[i1]+(double)rr*cos(0.0);
		pt[i2] = cp[i2]+(double)rr*sin(0.0);
		pt[i3] = cp[i3];
		glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);
	glEnd();
	return ret;
}

/************************************************************/
BOOL CJcad3GlbView::SetEllipse(FILE* fp, int* id, char* cmd)			//<<< ELLIPSEﾃﾞｰﾀ登録
/************************************************************/
{
	PNTTYPE pnt;
	VECTYPE vec;
	int     i, i1, i2, i3, ix=0;
	double  cp[3], bp[3], pt[3], rot[3][3], r1, r2, rt, th, t;
	float   sc, t1, t2;
	BOOL    ret=TRUE;

	cp[0] = 0.0, cp[1] = 0.0, cp[2] = 0.0;
	bp[0] = 0.0, bp[1] = 0.0, bp[2] = 0.0;
	t1 = 0.0f, t2 = 0.0f;
	i1 = Wi0;															// 有効座標軸決定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {
			if(ix==0) {													// <中心点ﾃﾞｰﾀ>
				ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;		// 頂点ﾃﾞｰﾀ入力
				ix++;
			} else {													// <長軸端点ﾃﾞｰﾀ>
				ret = (!ReadPoint(fp, id, cmd, bp)) ? FALSE : ret;		// 頂点ﾃﾞｰﾀ入力
			}
		} else if((*id)==40) {											// <短軸比率ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &sc)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
		} else if((*id)==41) {											// <開始角度ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &t1)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
		} else if((*id)==42) {											// <終了角度ﾃﾞｰﾀ>
			ret = (sscanf_s(cmd, "%g", &t2)!=1) ? FALSE : ret;			// 浮動小数変換
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			break;
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	r1 = sqrt(bp[i1]*bp[i1]+bp[i2]*bp[i2]);								// 長軸長
	r2 = r1*(double)sc;															// 短軸長
	rt = atan2(bp[i2], bp[i1]);											// 回転角度
	rt = (rt<-10||rt>10) ? 0.0 : rt;
	th = ((th=(double)(t2-t1))<0.0) ? th + PI*2.0 : th;					// 開始～終了角度
	ix = ((int)(18.0*th/PI)<3) ? 3 : (int)(18.0*th/PI);					// 角度分解数(基本10度)
	th = th/ix;															// 分解角度(増分)
	vec.p[i1] = 0.0, vec.p[i2] = 0.0, vec.p[i3] = 1.0;
	SetRotateMatrix(vec, rt, rot);
	glBegin(GL_LINE_STRIP);
		for(i=0; i<=ix; i++) {
			t = (double)t1+th*i;
			pnt.p[i1] = r1*cos(t), pnt.p[i2] = r2*sin(t), pnt.p[i3] = 0.0;
			VecRotate(&pnt, rot);
			pt[i1] = cp[i1]+pnt.p[i1];
			pt[i2] = cp[i2]+pnt.p[i2];
			pt[i3] = cp[i3];
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// ELLIPSE頂点登録
		}
	glEnd();
	return ret;
}

/*************************************************************/
BOOL CJcad3GlbView::SetPolyline(FILE* fp, int* id, char* cmd)			//<<< POLYLINEﾃﾞｰﾀ登録
/*************************************************************/
{
	int    fg=0, st=0;
	double pt[3], ps[3];
	BOOL   ret=TRUE;

	if(!ReadParam(fp, id, cmd)) {										// DXFﾃﾞｰﾀ入力
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <頂点ﾃﾞｰﾀ>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// 頂点ﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		} else if((*id)==70) {											// <POLYLINEﾌﾗｸﾞ>
			ret = (sscanf_s(cmd, "%d", &fg)!=1) ? FALSE : ret;
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		} else {														// <頂点ﾃﾞｰﾀ以外>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXFﾃﾞｰﾀ入力
			if((*id)==0) {
				break;
			}
		}
	}
	glBegin(GL_LINE_STRIP);
		while(ret) {
			if(!strcmp(cmd, "SEQEND")) {								// POLYLINE終了
				break;													// 処理終了
			} else if(!strcmp(cmd, "VERTEX")) {							// <VERTEXﾃﾞｰﾀ>
				pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
				ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;			// DXFﾃﾞｰﾀ入力
				while(ret) {
					if((*id)>=10&&(*id)<=39) {							// <頂点ﾃﾞｰﾀ>
						ret = (!ReadPoint(fp, id, cmd, pt))
                              ? FALSE : ret;							// 頂点ﾃﾞｰﾀ入力
						if((*id)==0) {
							break;
						}
					} else {											// <頂点ﾃﾞｰﾀ以外>
						ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;	// DXFﾃﾞｰﾀ入力
						if((*id)==0) {
							break;
						}
					}
				}
				glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);					// POLYLINE頂点登録
				if(!st) {
					ps[0] = pt[0], ps[1] = pt[1], ps[2] = pt[2], st = 1;
				}
			} else {													// <上記以外ﾃﾞｰﾀ>
				ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;			// DXFﾃﾞｰﾀ入力
			}
		}
		if(fg&&st) {
			glVertex3d(ps[Wi1], ps[Wi2], ps[Wi3]);						// 閉の場合先頭ﾃﾞｰﾀ追加
		}
	glEnd();
	return ret;
}

/******************************************************/
BOOL CJcad3GlbView::ReadDXFTemplate(int no, int plane)					//<<< DXFﾃﾝﾌﾟﾚｰﾄ入力
/******************************************************/
{
	char*  cmd = new char[1000];
	FILE*  fp;
	BOOL   ret;
	int    id;
	double pt[3], r, g, b;

	Wi0 = -2;
	if(fopen_s(&fp, m_TempPath[no][plane], "r")==0) {    				// ﾌｧｲﾙOPEN
		ret = (!ReadParam(fp, &id, cmd)) ? FALSE : TRUE;				// DXFﾃﾞｰﾀ入力
		while(ret) {													// 有効平面検索
			Wi0 = (!strcmp(cmd, "ENTITIES")) ? -1 : Wi0;
			if(Wi0==-1&&id>=10&&id<=39) {								// <有効な頂点ﾃﾞｰﾀ>
				if(!ReadPoint(fp, &id, cmd, pt)) {						// 頂点ﾃﾞｰﾀ入力
					break;
				}
				if(fabs(pt[0])>EPSILON&&fabs(pt[1])>EPSILON) {			// XY平面有効
					Wi0 = 0; break;
				}
				if(fabs(pt[1])>EPSILON&&fabs(pt[2])>EPSILON) {			// YZ平面有効
					Wi0 = 1; break;
				}
				if(fabs(pt[2])>EPSILON&&fabs(pt[0])>EPSILON) {			// ZX平面有効
					Wi0 = 2; break;
				}
			} else {													// <有効な頂点ﾃﾞｰﾀ以外>
				if(!ReadParam(fp, &id, cmd)) {							// DXFﾃﾞｰﾀ入力
					break;
				}
			}
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	}
	ret = (Wi0<0) ? FALSE : ret;										// 有効平面無はｴﾗｰ
	if(!ret) {															// ｴﾗｰは中止
		delete[] cmd;
		return ret;
	}
	if(plane==0) {
		Wi1 = Wi0;														// ﾃﾞｰﾀ平面と表示平面より
		Wi2 = (Wi1+1>2) ? 0 : Wi1+1;									// ﾃﾞｰﾀ登録位置決定
		Wi3 = (Wi2+1>2) ? 0 : Wi2+1;
	} else if(plane==1) {
		Wi1 = (Wi0+2>2) ? 0 : Wi0+2;
		Wi2 = (Wi1+1>2) ? 0 : Wi1+1;
		Wi3 = (Wi2+1>2) ? 0 : Wi2+1;
	} else {
		Wi1 = (Wi0+1>2) ? 0 : Wi0+1;
		Wi2 = (Wi1+1>2) ? 0 : Wi1+1;
		Wi3 = (Wi2+1>2) ? 0 : Wi2+1;
	}
	if(fopen_s(&fp, m_TempPath[no][plane], "r")==0) {    				// ﾌｧｲﾙOPEN
		while(ret) {													// ENTITIESまで
			if(!ReadParam(fp, &id, cmd)) {								// DXFﾃﾞｰﾀ入力
				ret = FALSE; break;
			}
			if(!strcmp(cmd, "ENTITIES")) {
				break;
			}
		}
		if(ret) {														// ENTITIESの場合
			m_TEMP[plane] = glGenLists(1);								// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
			glNewList(m_TEMP[plane], GL_COMPILE);						// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
				MainWnd->GetColor(MainWnd->TemplateColor,&r, &g, &b);
				glColor3d(r, g, b);										// 表示色設定
				ReadParam(fp, &id, cmd);								// DXFﾃﾞｰﾀ入力
				while(ret) {											// ENDSECまで
					if(!strcmp(cmd, "ENDSEC")) {						// ENTITIES終了
						break;											// 処理終了
					} else if(!strcmp(cmd, "POINT")) {					// <POINTﾃﾞｰﾀ>
						ret = (!SetPoint(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "LINE")) {					// <LINEﾃﾞｰﾀ>
						ret = (!SetLine(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "ARC")) {					// <ARCﾃﾞｰﾀ>
						ret = (!SetArc(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "CIRCLE")) {					// <CIRCLEﾃﾞｰﾀ>
						ret = (!SetCircle(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "ELLIPSE")) {				// <ELLIPSEﾃﾞｰﾀ>
						ret = (!SetEllipse(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "POLYLINE")) {				// <POLYLINEﾃﾞｰﾀ>
						ret = (!SetPolyline(fp, &id, cmd)) ? FALSE : ret;
					} else {											// <上記以外ﾃﾞｰﾀ>
						ret = (!ReadParam(fp, &id, cmd)) ? FALSE : ret;
					}
				}
			glEndList();												// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		ret = FALSE;
	}
	delete[] cmd;
	return ret;
}

/**********************************/
BOOL CJcad3GlbView::ReadTEMP(void)										//<<< ﾃﾝﾌﾟﾚｰﾄ入力
/**********************************/
{
	CString ext;
	BOOL    ret=TRUE;
	int     i;

	DeleteTEMP();														// 旧ﾃﾝﾌﾟﾚｰﾄ破棄
	if(m_DspTmpNo==0) {													// ﾃﾝﾌﾟﾚｰﾄ表示無は終了
		return TRUE;
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	for(i=0; i<3; i++) {												// <<0:X-Y, 1:Y-Z, 2:Z-X>>
		if(m_TempPath[m_DspTmpNo-1][i]!=_T("")) {
			ext = m_TempPath[m_DspTmpNo-1][i].Right(3);					// 拡張子取得
			if(!ext.CompareNoCase("dxf")) {								// <拡張子:DXFの場合>
				ret = ReadDXFTemplate(m_DspTmpNo-1, i);					// DXFﾃﾝﾌﾟﾚｰﾄ入力
			} else if(!ext.CompareNoCase("glt")) {						// <拡張子:GLTの場合>
				ret = ReadGLTTemplate(1, m_DspTmpNo-1, i);				// GLTﾃﾝﾌﾟﾚｰﾄ入力
			} else {
				ret = FALSE;
			}
			if(!ret) {
				break;													// ｴﾗｰは中止
			}
		}
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	return ret;
}

/************************************/
void CJcad3GlbView::DeleteTEMP(void)									//<<< ﾃﾝﾌﾟﾚｰﾄ破棄
/************************************/
{
	int i;

	for(i=0; i<3; i++) {
		if(m_TEMP[i]!=0) {
			glDeleteLists(m_TEMP[i], 1);
			m_TEMP[i] = 0;
		}
	}
}

/*******************************************************/
void CJcad3GlbView::SetTexFile(int flg, int kind,
                               int plane, CString file)					//<<< ﾃｸｽﾁｬﾌｧｲﾙ設定
/*******************************************************/
{
	BOOL ret;

	if(flg==1) {														// <ﾌｧｲﾙ設定>
		m_TexFile[plane]   = file;
		m_TexMove1[plane]  = 0.0;
		m_TexMove2[plane]  = 0.0;
		m_TexRotate[plane] = 0;
		m_TexScale[plane]  = 1.0f;
		if(!kind) {														// <BMP指定の場合>
			ret = SetTexImageBMP(plane, 0);								// ﾃｸｽﾁｬ用BMP画像ﾃﾞｰﾀ設定
		} else {														// <JPG指定の場合>
			ret = SetTexImageJPG(plane);								// ﾃｸｽﾁｬ用JPG画像ﾃﾞｰﾀ設定
		}
		if(ret) {														// <ﾃﾞｰﾀ設定OK>
			m_TexFovy[plane] = m_Fovy;									// 登録時の表示幅
			m_TexDisp[plane] = 1;
			DrawTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		} else {														// <ﾃﾞｰﾀ設定NG>
			m_TexFile[plane] = _T("");									// ﾃﾞｰﾀｸﾘｱ
			m_TexDisp[plane] = 0;
			ClearTexImage(plane);										// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ消去
			DeleteTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ破棄
		}
	} else {															// <ﾌｧｲﾙ消去>
		m_TexFile[plane]   = _T("");
		m_TexDisp[plane]   = 0;
		m_TexMove1[plane]  = 0.0;
		m_TexMove2[plane]  = 0.0;
		m_TexRotate[plane] = 0;
		m_TexScale[plane]  = 1.0f;
		ClearTexImage(plane);											// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ消去
		DeleteTexPixels(plane);											// ﾃｸｽﾁｬﾋﾟｸｾﾙ破棄
	}
	Display(0);															// 再描画
}

/*******************************************************************/
void CJcad3GlbView::GetTexInfo(int plane, CString* fl, double* mv1,
                               double* mv2, int* rt, float* sc)			//<<< ﾃｸｽﾁｬ表示状態取得
/*******************************************************************/
{
	*fl  = m_TexFile[plane];
	*mv1 = m_TexMove1[plane];
	*mv2 = m_TexMove2[plane];
	*rt  = m_TexRotate[plane];
	*sc  = m_TexScale[plane];
}

/***************************************************************/
void CJcad3GlbView::ChangeTexInfo(int flg, int plane, int mode)			//<<< ﾃｸｽﾁｬ表示状態変更
/***************************************************************/
{
	int wk;

	if(m_TexDisp[plane]) {
		if(mode==0) {													// <移動1>
			m_TexMove1[plane] += (flg) ? 0.1 : -0.1;
			wk = (m_TexMove1[plane]>=0) ? (int)(m_TexMove1[plane]*10.0+0.5)
                                        : (int)(m_TexMove1[plane]*10.0-0.5);
			m_TexMove1[plane] = (double)wk/10.0;
			DrawTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		} else if(mode==1) {											// <移動2>
			m_TexMove2[plane] += (flg) ? 0.1 : -0.1;
			wk = (m_TexMove2[plane]>=0) ? (int)(m_TexMove2[plane]*10.0+0.5)
                                        : (int)(m_TexMove2[plane]*10.0-0.5);
			m_TexMove2[plane] = (double)wk/10.0;
			DrawTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		} else if(mode==2) {											// <回転>
			if(flg) {													// 右回り
				m_TexRotate[plane] =
                (m_TexRotate[plane]+1>3) ? 0 : m_TexRotate[plane]+1;
			} else {													// 左回り
				m_TexRotate[plane] =
                (m_TexRotate[plane]-1<0) ? 3 : m_TexRotate[plane]-1;
			}
			RotateTexImage(flg, plane);									// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
			DrawTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		} else if(mode==3) {											// <ｻｲｽﾞ変更>
			m_TexScale[plane] += (flg) ? 0.01f : -0.01f;
			wk = (m_TexScale[plane]>=0) ? (int)(m_TexScale[plane]*100.0+0.5)
                                        : (int)(m_TexScale[plane]*100.0-0.5);
			m_TexScale[plane] = (float)wk/100.0f;
			DrawTexPixels(plane);										// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		}
		Display(0);														// 再描画
	}
}

/******************************************************/
BOOL CJcad3GlbView::SetTexImageBMP(int plane, int flg)					//<<< ﾃｸｽﾁｬ用BMP画像ﾃﾞｰﾀ設定
/******************************************************/
{
	WIN32_FIND_DATA wfd;
	CString path;
	FILE*   fp;
	int     i, j, w, h;
	BOOL    ret;

	if(flg==0) {														// <ﾌｧｲﾙ名指定>
		path = m_TexFile[plane];										// 指定ﾌｧｲﾙ名
	} else {															// <作業ﾌｧｲﾙ指定>
		path = GetAppPath(0)+CNV_NAME;									// JPEGﾌｧｲﾙ変換作業ﾌｧｲﾙ名
	}
	if(FindFirstFile(path, &wfd)==INVALID_HANDLE_VALUE) {				// ﾌｧｲﾙ無の場合
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		return FALSE;
	}
	if(fopen_s(&fp, path, "rb")!=0) {
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		return FALSE;
	}
	ClearTexImage(plane);												// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ消去
	int* hd = new int[54];
	for(i=0; i<54; i++) {
		hd[i] = fgetc(fp);
	}
	w = hd[18]+hd[19]*256+hd[20]*256*256+hd[21]*256*256*256;			// 画像の横ﾋﾟｸｾﾙ数
	m_TexWidth[plane] = w;
	h = hd[22]+hd[23]*256+hd[24]*256*256+hd[25]*256*256*256;			// 画像の縦ﾋﾟｸｾﾙ数
	m_TexHeight[plane] = h;
	if(w%4||h%4) {														// <ﾋﾟｸｾﾙ数が4の倍数でない>
		SetSplashMsg(IDS_ERR_PICFILE2);									// 画像ﾌｧｲﾙｴﾗｰ
		ret = FALSE;
	} else {															// <正常ﾃﾞｰﾀ>
		m_TexImage[plane] = (GLubyte *)malloc(sizeof(GLubyte)*w*h*3);	// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ領域確保
		if(m_TexImage[plane]==NULL) {
			MemErr(IDS_MEMERR5);
		}
		for(i=0; i<h; i++) {
			for(j=0; j<w; j++) {
				m_TexImage[plane][(i*w*3)+(j*3)+2] = (UCHAR)fgetc(fp);
				m_TexImage[plane][(i*w*3)+(j*3)+1] = (UCHAR)fgetc(fp);
				m_TexImage[plane][(i*w*3)+(j*3)+0] = (UCHAR)fgetc(fp);
			}
		}
		ret = TRUE;
	}
	fclose(fp);
	delete[] hd;
	return ret;
}

/*********************************************/
BOOL CJcad3GlbView::SetTexImageJPG(int plane)							//<<< ﾃｸｽﾁｬ用JPG画像ﾃﾞｰﾀ設定
/*********************************************/
{
	WIN32_FIND_DATA wfd;
	HANDLE  hDIB;
	CString path;

	if(FindFirstFile(m_TexFile[plane], &wfd)==INVALID_HANDLE_VALUE) {	// ﾌｧｲﾙ無の場合
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		return FALSE;
	}
	hDIB = IKJpegFileLoad(m_TexFile[plane], 0, NULL, NULL, NULL);		// JPEGﾌｧｲﾙ読み込み
	if(hDIB==0) {
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		return FALSE;
	}
	path = GetAppPath(0)+CNV_NAME;										// JPEGﾌｧｲﾙ変換作業ﾌｧｲﾙ名
	if(!IKBmpFileSave(path, hDIB, FALSE, 0, NULL, NULL, NULL)) {		// ﾋﾞｯﾄﾏｯﾌﾟﾌｧｲﾙ出力
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
		return FALSE;
	}
	if(!IKFreeMemory(hDIB)) {											// ﾒﾓﾘ解放
		SetSplashMsg(IDS_ERR_PICFILE);									// 画像ﾌｧｲﾙｴﾗｰ
	}
	SetTexImageBMP(plane, 1);											// ﾃｸｽﾁｬ用BMP画像ﾃﾞｰﾀ設定
	remove(path);														// 作業ﾌｧｲﾙ削除
	return TRUE;
}

/********************************************/
void CJcad3GlbView::ClearTexImage(int plane)							//<<< ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ消去
/********************************************/
{
	if(m_TexImage[plane]!=NULL) {										// <ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ設定済み>
		free(m_TexImage[plane]);										// 領域解放
		m_TexImage[plane] = NULL;										// 変数初期化
		m_TexWidth[plane] = 0, m_TexHeight[plane] = 0;
	}
}

/******************************************************/
void CJcad3GlbView::RotateTexImage(int flg, int plane)					//<<< ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ回転
/******************************************************/
{
	GLubyte* buf;
	int      i, j, k, w, h, ct=0;

	w  = m_TexWidth[plane];												// 現在の画像横ﾋﾟｸｾﾙ数
	h  = m_TexHeight[plane];											// 現在の画像縦ﾋﾟｸｾﾙ数
	buf = (GLubyte *)malloc(sizeof(GLubyte)*w*h*3);						// ﾃｸｽﾁｬ用画像ﾃﾞｰﾀ領域確保
	if(buf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	if(flg) {															// <右側へ90度回転>
		for(i=w-1; i>=0; i--) {
			for(j=0; j<h; j++) {
				for(k=0; k<3; k++) {
					buf[ct++] = m_TexImage[plane][(i*3)+(j*w*3)+k];
				}
			}
		}
	} else {															// <左側へ90度回転>
		for(i=0; i<w; i++) {
			for(j=h-1; j>=0; j--) {
				for(k=0; k<3; k++) {
					buf[ct++] = m_TexImage[plane][(i*3)+(j*w*3)+k];
				}
			}
		}
	}
	free(m_TexImage[plane]);											// 旧領域解放
	m_TexImage[plane] = buf;
	m_TexWidth[plane]  = h;												// 回転後の画像横ﾋﾟｸｾﾙ数
	m_TexHeight[plane] = w;												// 回転後の画像縦ﾋﾟｸｾﾙ数
}

/*****************************************/
void CJcad3GlbView::ChangeTexPixels(void)								//<<< ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
/*****************************************/
{
	int i;

	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			DrawTexPixels(i);											// ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
		}
	}
}

/********************************************/
void CJcad3GlbView::DrawTexPixels(int plane)							//<<< ﾃｸｽﾁｬﾋﾟｸｾﾙ登録
/********************************************/
{
	PNTTYPE cpt;
	int     xp, yp, zp, wk;
	double  wp, hp, wt, ht, wt2, ht2, px, py, pz, sx, sy, sz;
	float   sc;

	GetCentPnt(&cpt);													// 画面ｾﾝﾀｰ取得
	if(plane==XY) {														// <XY平面>
		sc = (float)m_TexFovy[XY]/(float)m_Fovy;						// 現在の表示倍率(対登録時)
		wp = sc*m_TexScale[XY]*PlDstW[XY]/PlSizeW[XY];					// 倍率調整後の1ﾋﾟｸｾﾙ横長さ(mm)
		hp = sc*m_TexScale[XY]*PlDstH[XY]/PlSizeH[XY];					// 倍率調整後の1ﾋﾟｸｾﾙ縦長さ(mm)
		wt = m_TexWidth[XY]*wp*0.5;										// ﾃｸｽﾁｬ横長さの半分(mm)
		ht = m_TexHeight[XY]*hp*0.5;									// ﾃｸｽﾁｬ縦長さの半分(mm)
		px = ht - m_TexMove2[XY];										// ﾃｸｽﾁｬ左下の座標
		py = -wt + m_TexMove1[XY];
		sx = cpt.p[0] + PlDstH[XY]/2.0;									// 画面左下の座標
		sy = cpt.p[1] - PlDstW[XY]/2.0;
		wk = (sx<px) ? (int)((px-sx)/hp)+1 : 0;							// はみ出す(カットする)X方向ﾋﾟｸｾﾙ数
		xp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wk = (sy>py) ? (int)((sy-py)/wp)+1 : 0;							// はみ出す(カットする)Y方向ﾋﾟｸｾﾙ数
		yp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wt2 = yp*wp, ht2 = xp*hp;										// カットしたX/Y方向の長さ(mm)
		if(m_TEXTURE[XY]==0) {
			m_TEXTURE[XY] = glGenLists(1);								// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		}
		glNewList(m_TEXTURE[XY], GL_COMPILE);							// XYﾃｸｽﾁｬﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
			glScissor(PlRect[XY].left+1, ViewSize.cy-PlRect[XY].bottom,
                      PlRect[XY].Width()-1, PlRect[XY].Height()-1);		// 描画領域を制限
			glEnable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを有効化
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(px-ht2, py+wt2, 0.0);
			glPixelZoom(m_TexScale[XY]*sc, m_TexScale[XY]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[XY]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, yp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, xp);
			glDrawPixels(m_TexWidth[XY]-yp, m_TexHeight[XY]-xp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[XY]);
			glDisable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを無効化
		glEndList();
	} else if(plane==YZ) {												// <YZ平面>
		sc = (float)m_TexFovy[YZ]/(float)m_Fovy;						// 現在の表示倍率(対登録時)
		wp = sc*m_TexScale[YZ]*PlDstW[YZ]/PlSizeW[YZ];					// 倍率調整後の1ﾋﾟｸｾﾙ横長さ(mm)
		hp = sc*m_TexScale[YZ]*PlDstH[YZ]/PlSizeH[YZ];					// 倍率調整後の1ﾋﾟｸｾﾙ縦長さ(mm)
		wt = m_TexWidth[YZ]*wp*0.5;										// ﾃｸｽﾁｬ横長さの半分(mm)
		ht = m_TexHeight[YZ]*hp*0.5;									// ﾃｸｽﾁｬ縦長さの半分(mm)
		py = -wt + m_TexMove1[YZ];										// ﾃｸｽﾁｬ左下の座標
		pz = -ht + m_TexMove2[YZ];
		sy = cpt.p[1] - PlDstW[YZ]/2.0;									// 画面左下の座標
		sz = cpt.p[2] - PlDstH[YZ]/2.0;
		wk = (sy>py) ? (int)((sy-py)/wp)+1 : 0;							// はみ出す(カットする)Y方向ﾋﾟｸｾﾙ数
		yp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wk = (sz>pz) ? (int)((sz-pz)/hp)+1 : 0;							// はみ出す(カットする)Z方向ﾋﾟｸｾﾙ数
		zp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wt2 = yp*wp, ht2 = zp*hp;										// カットしたY/Z方向の長さ(mm)
		if(m_TEXTURE[YZ]==0) {
			m_TEXTURE[YZ] = glGenLists(1);								// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		}
		glNewList(m_TEXTURE[YZ], GL_COMPILE);							// XYﾃｸｽﾁｬﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
			glScissor(PlRect[YZ].left+1, ViewSize.cy-PlRect[YZ].bottom,
                      PlRect[YZ].Width()-1, PlRect[YZ].Height()-1);		// 描画領域を制限
			glEnable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを有効化
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(0.0, py+wt2, pz+ht2);
			glPixelZoom(m_TexScale[YZ]*sc, m_TexScale[YZ]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[YZ]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, yp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, zp);
			glDrawPixels(m_TexWidth[YZ]-yp, m_TexHeight[YZ]-zp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[YZ]);
			glDisable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを無効化
		glEndList();
	} else if(plane==ZX) {												// <ZX平面>
		sc = (float)m_TexFovy[ZX]/(float)m_Fovy;						// 現在の表示倍率(対登録時)
		wp = sc*m_TexScale[ZX]*PlDstW[ZX]/PlSizeW[ZX];					// 倍率調整後の1ﾋﾟｸｾﾙ横長さ(mm)
		hp = sc*m_TexScale[ZX]*PlDstH[ZX]/PlSizeH[ZX];					// 倍率調整後の1ﾋﾟｸｾﾙ縦長さ(mm)
		wt = m_TexWidth[ZX]*wp*0.5;										// ﾃｸｽﾁｬ横長さの半分(mm)
		ht = m_TexHeight[ZX]*hp*0.5;									// ﾃｸｽﾁｬ縦長さの半分(mm)
		pz = -ht + m_TexMove2[ZX];										// ﾃｸｽﾁｬ左下の座標
		px = wt - m_TexMove1[ZX];
		sz = cpt.p[2] - PlDstH[ZX]/2.0;									// 画面左下の座標
		sx = cpt.p[0] + PlDstW[ZX]/2.0;
		wk = (sz>pz) ? (int)((sz-pz)/hp)+1 : 0;							// はみ出す(カットする)Z方向ﾋﾟｸｾﾙ数
		zp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wk = (sx<px) ? (int)((px-sx)/wp)+1 : 0;							// はみ出す(カットする)X方向ﾋﾟｸｾﾙ数
		xp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4の倍数に調整
		wt2 = xp*wp, ht2 = zp*hp;										// カットしたZ/X方向の長さ(mm)
		if(m_TEXTURE[ZX]==0) {
			m_TEXTURE[ZX] = glGenLists(1);								// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		}
		glNewList(m_TEXTURE[ZX], GL_COMPILE);							// XYﾃｸｽﾁｬﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ
			glScissor(PlRect[ZX].left+1, ViewSize.cy-PlRect[ZX].bottom,
                      PlRect[ZX].Width()-1, PlRect[ZX].Height()-1);		// 描画領域を制限
			glEnable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを有効化
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(px-wt2, 0.0, pz+ht2);
			glPixelZoom(m_TexScale[ZX]*sc, m_TexScale[ZX]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[ZX]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, xp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, zp);
			glDrawPixels(m_TexWidth[ZX]-xp, m_TexHeight[ZX]-zp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[ZX]);
			glDisable(GL_SCISSOR_TEST);									// ｼｻﾞｰ･ﾃｽﾄを無効化
		glEndList();
	}
}

/**********************************************/
void CJcad3GlbView::DeleteTexPixels(int plane)							//<<< ﾃｸｽﾁｬﾋﾟｸｾﾙ破棄
/**********************************************/
{
	if(m_TEXTURE[plane]!=0) {
		glDeleteLists(m_TEXTURE[plane], 1);
		m_TEXTURE[plane] = 0;
	}
}

/**************************************************************/
void CJcad3GlbView::DrawBox(PNTTYPE sp, PNTTYPE ep, int plane)			//<<< 矩形表示
/**************************************************************/
{
	PNTTYPE pa[5];
	int     i, i1, i2, i3;
	double  r, g, b;

	i1 = plane;
	i2 = (i1+1>2) ? 0 : i1 + 1;
	i3 = (i2+1>2) ? 0 : i2 + 1;
	pa[0] = sp;
	pa[1].p[i1]=sp.p[i1], pa[1].p[i2]=ep.p[i2], pa[1].p[i3]=sp.p[i3];
	pa[2] = ep;
	pa[3].p[i1]=ep.p[i1], pa[3].p[i2]=sp.p[i2], pa[3].p[i3]=sp.p[i3];
	pa[4] = sp;
	MainWnd->GetColor(MainWnd->BoxColor, &r, &g, &b);					// 表示色取得
	if(m_BOX==0) {
		m_BOX = glGenLists(1);											// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	}
	glNewList(m_BOX, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glColor3d(r, g, b);												// 表示色設定
		glBegin(GL_LINE_STRIP);
			for(i=0; i<5; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// 頂点登録
			}
		glEnd();
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	m_BoxPlane = plane;
}

/***********************************/
void CJcad3GlbView::DeleteBox(void)										//<<< 矩形破棄
/***********************************/
{
	if(m_BOX!=0) {
		glDeleteLists(m_BOX, 1);
		m_BOX = 0;
	}
	m_BoxPlane = 0;
}

/*****************************************************************/
void CJcad3GlbView::DrawCircle(PNTTYPE cp, PNTTYPE ep, int plane)		//<<< 円表示
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[73];
	int      i, i1, i2, i3;
	double   rr, th, r, g, b;

	i1 = plane;
	i2 = (i1+1>2) ? 0 : i1 + 1;
	i3 = (i2+1>2) ? 0 : i2 + 1;
	rr = PntD(&cp, &ep);												// 2点間距離(半径)
	th = PI/36.0;														// 5度
	for(i=0; i<72; i++) {
		pa[i].p[i1] = cp.p[i1] + rr*cos(th*i);							// 円座標設定
		pa[i].p[i2] = cp.p[i2] + rr*sin(th*i);
		pa[i].p[i3] = cp.p[i3];
	}
	pa[i] = pa[0];														// 始点同一点でｸﾛｰｽﾞ
	MainWnd->GetColor(MainWnd->CircleColor, &r, &g, &b);				// 表示色取得
	if(m_CIRCLE==0) {
		m_CIRCLE = glGenLists(1);										// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	}
	glNewList(m_CIRCLE, GL_COMPILE);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glColor3d(r, g, b);												// 表示色設定
		glBegin(GL_LINE_STRIP);
			for(i=0; i<73; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// 頂点登録
			}
		glEnd();
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	m_CirclePlane = plane;
	delete[] pa;
}

/**************************************/
void CJcad3GlbView::DeleteCircle(void)									//<<< 円破棄
/**************************************/
{
	if(m_CIRCLE!=0) {
		glDeleteLists(m_CIRCLE, 1);
		m_CIRCLE = 0;
	}
	m_CirclePlane = 0;
}

/********************************************************/
void CJcad3GlbView::MeasureSCALE(PNTTYPE p1, PNTTYPE p2)				//<<< 定規登録
/********************************************************/
{
	int    i, i1, i2, i3, c1, c2;
	double pa[4][3], mn1, mn2, mx1, mx2, ln1, ln2, mm, rc, gc, bc;

	MainWnd->GetColor(MainWnd->MeasureColor, &rc, &gc, &bc);			// 表示色取得(黄色)
	i1 = m_ScalePlane;													// 座標軸の設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(p1.p[i1]<p2.p[i1]) {												// i1軸の最小/最大
		mn1 = p1.p[i1], mx1 = p2.p[i1];
	} else {
		mn1 = p2.p[i1], mx1 = p1.p[i1];
	}
	if(p1.p[i2]<p2.p[i2]) {												// i2軸の最小/最大
		mn2 = p1.p[i2], mx2 = p2.p[i2];
	} else {
		mn2 = p2.p[i2], mx2 = p1.p[i2];
	}
	ln1 = mx1 - mn1;													// i1軸方向の長さ
	ln2 = mx2 - mn2;													// i2軸方向の長さ
	c1 = (int)(ln1-0.001);												// i1軸方向の目盛り数
	c2 = (int)(ln2-0.001);												// i2軸方向の目盛り数
	if(m_SCALE==0) {
		m_SCALE = glGenLists(1);										// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	}
	glNewList(m_SCALE, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glColor3d(rc, gc, bc);											// 表示色設定
		pa[0][i1] = mn1, pa[0][i2] = mn2, pa[0][i3] = 0.0;				// <四隅点の設定>
		pa[1][i1] = mx1, pa[1][i2] = mn2, pa[1][i3] = 0.0;
		pa[2][i1] = mx1, pa[2][i2] = mx2, pa[2][i3] = 0.0;
		pa[3][i1] = mn1, pa[3][i2] = mx2, pa[3][i3] = 0.0;
		glBegin(GL_LINE_STRIP);
			for(i=0; i<4; i++) {
				glVertex3d(pa[i][0], pa[i][1], pa[i][2]);				// 頂点登録
			}
			glVertex3d(pa[0][0], pa[0][1], pa[0][2]);
		glEnd();
		glBegin(GL_LINES);
			for(i=0; i<c1; i++) {										// <i1軸方向の目盛り>
				mm = ((i+1)%10==0) ? 1.0 : (((i+1)%5==0)?0.75:0.5);		// 目盛り幅の設定
				pa[0][i1] = mn1+(i+1), pa[0][i2] = mn2;
				pa[1][i1] = mn1+(i+1), pa[1][i2] = mn2+mm;
				pa[2][i1] = mn1+(i+1), pa[2][i2] = mx2;
				pa[3][i1] = mn1+(i+1), pa[3][i2] = mx2-mm;
				glVertex3d(pa[0][0], pa[0][1], pa[0][2]);				// 頂点登録
				glVertex3d(pa[1][0], pa[1][1], pa[1][2]);
				glVertex3d(pa[2][0], pa[2][1], pa[2][2]);
				glVertex3d(pa[3][0], pa[3][1], pa[3][2]);
			}
			for(i=0; i<c2; i++) {										// <i2軸方向の目盛り>
				mm = ((i+1)%10==0) ? 1.0 : (((i+1)%5==0)?0.75:0.5);		// 目盛り幅の設定
				pa[0][i1] = mn1,    pa[0][i2] = mn2+(i+1);
				pa[1][i1] = mn1+mm, pa[1][i2] = mn2+(i+1);
				pa[2][i1] = mx1,    pa[2][i2] = mn2+(i+1);
				pa[3][i1] = mx1-mm, pa[3][i2] = mn2+(i+1);
				glVertex3d(pa[0][0], pa[0][1], pa[0][2]);				// 頂点登録
				glVertex3d(pa[1][0], pa[1][1], pa[1][2]);
				glVertex3d(pa[2][0], pa[2][1], pa[2][2]);
				glVertex3d(pa[3][0], pa[3][1], pa[3][2]);
			}
		glEnd();
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
}

/*************************************/
void CJcad3GlbView::DeleteSCALE(void)									//<<< 定規破棄
/*************************************/
{
	if(m_SCALE!=0) {
		glDeleteLists(m_SCALE, 1);
		m_SCALE = 0;
	}
	m_ScalePlane = 0;
}

/*******************************************************/
void CJcad3GlbView::MeasurePROT(PNTTYPE cp, PNTTYPE ep)					//<<< 分度器登録
/*******************************************************/
{
	PNTTYPE* pa = new PNTTYPE[72];
	PNTTYPE  pb;
	VECTYPE  vec;
	int      i, i1, i2, i3;
	double   len, l1, l2, ath, dth, rc, gc, bc;

	MainWnd->GetColor(MainWnd->MeasureColor, &rc, &gc, &bc);			// 表示色取得(黄色)
	i1 = m_ProtPlane;													// 座標軸の設定
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	l1 = ep.p[i1]-cp.p[i1];
	l2 = ep.p[i2]-cp.p[i2];
	len = sqrt(l1*l1+l2*l2);											// 分度器半径
	dth = (2.0*PI)/72.0;												// 32分割回転角度
	for(ath=0.0, i=0; i<72; ath+=dth, i++) {
		pa[i].p[i1] = cp.p[i1]+len*cos(ath);							// 頂点座標設定
		pa[i].p[i2] = cp.p[i2]+len*sin(ath);
		pa[i].p[i3] = 0.0;
	}
	if(m_PROT==0) {
		m_PROT = glGenLists(1);											// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	}
	glNewList(m_PROT, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glColor3d(rc, gc, bc);											// 表示色設定
		glBegin(GL_LINE_STRIP);
			for(i=0; i<72; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// 頂点登録
			}
			glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);
		glEnd();
		glBegin(GL_LINES);
			for(i=0; i<72; i++) {										// <目盛りの設定>
				len = (i%18==0) ? 1.0 : ((i%2==0)?0.75:0.5);			// 目盛り幅の設定
				VecV(&pa[i], &cp, &vec);								// 中心点方向ﾍﾞｸﾄﾙ
				VecN(&vec);												// 単位ﾍﾞｸﾄﾙ化
				pb = pa[i];												// 目盛位置設定
				pb.p[i1] += vec.p[i1]*len;
				pb.p[i2] += vec.p[i2]*len;
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// 頂点登録
				glVertex3d(pb.p[0], pb.p[1], pb.p[2]);
			}
		glEnd();
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	delete[] pa;
}

/************************************/
void CJcad3GlbView::DeletePROT(void)									//<<< 分度器破棄
/************************************/
{
	if(m_PROT!=0) {
		glDeleteLists(m_PROT, 1);
		m_PROT = 0;
	}
	m_ProtPlane = 0;
}

/******************************************/
BOOL CJcad3GlbView::CheckMeasure(int menu)								//<<< 定規/分度器表示確認
/******************************************/
{
	BOOL ret;

	if(menu==M_MEASURE_SCALE) {
		ret = (m_SCALE>0) ? TRUE : FALSE;
	} else {
		ret = (m_PROT>0) ? TRUE : FALSE;
	}
	return ret;
}

/********************************************************************/
void CJcad3GlbView::SetBaseData(int cn, int pn, int bc,
                                PNTTYPE* pa, PNTTYPE* pb, int plane)	//<<< 基準線登録
/********************************************************************/
{
	int    i, j;
	double r, g, b;

	if(pn<2) {
		DeleteBaseData();				                                // 基準線破棄
		return;
	}
	m_DAraFlg = 1;														// 表示範囲設定ﾌﾗｸﾞ:ON
	for(i=0; i<DIM; i++) {
		m_DAra.bp[0].p[i] = DBL_MAX;									// 表示領域初期化
		m_DAra.bp[1].p[i] = 1.0-DBL_MAX;
	}
	MainWnd->GetColor(MainWnd->MeasureColor, &r, &g, &b);				// 表示色取得(黄色)
	if(m_BASE==0) {
		m_BASE = glGenLists(1);											// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
	}
	glNewList(m_BASE, GL_COMPILE);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
		glColor3d(r, g, b);												// 表示色設定
		glBegin(GL_LINE_STRIP);
			for(i=0; i<pn; i++) {
				for(j=0; j<DIM; j++) {
					if(m_DAra.bp[0].p[j]>pa[i].p[j]) {
						m_DAra.bp[0].p[j] = pa[i].p[j];
					}
					if(m_DAra.bp[1].p[j]<pa[i].p[j]) {
						m_DAra.bp[1].p[j] = pa[i].p[j];
					}
				}
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// 頂点登録
			}
			if(bc) {													// 境界条件:閉の場合
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);			// 始点登録
			}
		glEnd();
		if(cn>1) {														// <2本目が有効な場合>
			glBegin(GL_LINE_STRIP);
				for(i=0; i<pn; i++) {
					for(j=0; j<DIM; j++) {
						if(m_DAra.bp[0].p[j]>pb[i].p[j]) {
							m_DAra.bp[0].p[j] = pb[i].p[j];
						}
						if(m_DAra.bp[1].p[j]<pb[i].p[j]) {
							m_DAra.bp[1].p[j] = pb[i].p[j];
						}
					}
					glVertex3d(pb[i].p[0], pb[i].p[1], pb[i].p[2]);		// 頂点登録
				}
				if(bc) {												// 境界条件:閉の場合
					glVertex3d(pb[0].p[0], pb[0].p[1], pb[0].p[2]);		// 始点登録
				}
			glEnd();
		}
	glEndList();														// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	if(plane>=0) {														// 基準線表示平面設定
		m_BasePlane = plane;
	}
}

/****************************************/
void CJcad3GlbView::DeleteBaseData(void)								//<<< 基準線破棄
/****************************************/
{
	if(m_BASE!=0) {
		glDeleteLists(m_BASE, 1);
		m_BASE = 0;
	}
	m_BasePlane = 0;
	m_DAraFlg = 0;														// 表示範囲設定ﾌﾗｸﾞ:OFF
}

/**************************************************/
void CJcad3GlbView::StartStageDisplay(BOXTYPE box)						//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ初期処理
/**************************************************/
{
	PNTTYPE cpt;

	SscrA = m_Cent3D;
	Wi0   = m_Fovy;
	m_Fovy = (int)((BoxCenter(box, &cpt)*1000.0)/0.8+0.5);				// ﾎﾞｯｸｽｾﾝﾀｰ&最大幅取得
	ScrToPnt(1, &m_Cent3D, &cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	CentPars();															// 透視図参照点変更
}

/*****************************************/
void CJcad3GlbView::EndStageDisplay(void)								//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ終了処理
/*****************************************/
{
	m_Cent3D = SscrA;
	m_Fovy   = Wi0;
	SetSeishaei(m_Cent3D, m_Fovy);										// 正射影行列再構築
	ChangeTexPixels();													// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
	CentPars();															// 透視図参照点変更
}

/********************************************************/
void CJcad3GlbView::SetStage(int* flg, double size[][3])				//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ設定
/********************************************************/
{
	double rc, gc, bc;

	MainWnd->GetColor(MainWnd->BoolColor, &rc, &gc, &bc);				// 表示色取得(灰色)
	if(flg[0]||flg[1]||flg[2]||flg[3]) {
		if(m_STAGE==0) {
			m_STAGE = glGenLists(1);									// 新規ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ番号取得
		}
		glNewList(m_STAGE, GL_COMPILE);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成開始
			glColor3d(rc, gc, bc);										// 表示色設定
			if(flg[0]) {												// <底面>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// 頂点登録
					glVertex3d(size[2][0], size[2][1], size[2][2]);
					glVertex3d(size[3][0], size[3][1], size[3][2]);
					glVertex3d(size[1][0], size[1][1], size[1][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
			if(flg[1]) {												// <背面>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// 頂点登録
					glVertex3d(size[1][0], size[1][1], size[1][2]);
					glVertex3d(size[5][0], size[5][1], size[5][2]);
					glVertex3d(size[4][0], size[4][1], size[4][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
			if(flg[2]) {												// <右側面>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[1][0], size[1][1], size[1][2]);		// 頂点登録
					glVertex3d(size[3][0], size[3][1], size[3][2]);
					glVertex3d(size[7][0], size[7][1], size[7][2]);
					glVertex3d(size[5][0], size[5][1], size[5][2]);
					glVertex3d(size[1][0], size[1][1], size[1][2]);
				glEnd();
			}
			if(flg[3]) {												// <左側面>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// 頂点登録
					glVertex3d(size[4][0], size[4][1], size[4][2]);
					glVertex3d(size[6][0], size[6][1], size[6][2]);
					glVertex3d(size[2][0], size[2][1], size[2][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
		glEndList();													// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ作成終了
	} else {
		DeleteStage();													// 旧ｽﾃｰｼﾞ破棄
	}
}

/*************************************/
void CJcad3GlbView::DeleteStage(void)									//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ削除
/*************************************/
{
	if(m_STAGE!=0) {
		glDeleteLists(m_STAGE, 1);
		m_STAGE = 0;
	}
}

/****************************************************************/
void CJcad3GlbView::SetLightPos(int no, LIGHT* lgt, double* oct)		//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
/****************************************************************/
{
	PNTTYPE p1, p2;
	int     i, j;

	for(i=0, j=0; i<8; i++) {
		p1.p[0] = lgt[i].x1+oct[0];										// 立体中心座標を加算
		p1.p[1] = lgt[i].y1+oct[1];
		p1.p[2] = lgt[i].z1+oct[2];
		p2.p[0] = lgt[i].x2+oct[0];
		p2.p[1] = lgt[i].y2+oct[1];
		p2.p[2] = lgt[i].z2+oct[2];
		if(i==no) {
			Spa[0] = p1, Spa[1] = p2, Sia[0] = 0, Sia[1] = 1;			// ﾄﾗｯｶｰ情報設定
		} else {
			Spb[j] = p1, Spb[j+1] = p2, Sib[j] = 3, Sib[j+1] = 3;		// ﾄﾗｯｶｰ2情報設定
			j += 2;
		}
		Spc[i] = p1, Spd[i] = p2, Sic[i] = (i==no) ? 0 : 3;				// ﾗｲﾝ情報設定
	}
	Tracker1.Setup(Spa, Sia, NULL, 2, 5, 0);							// ﾄﾗｯｶｰ1設定
	Tracker2.Setup(Spb, Sib, NULL, 14, 5, 0);							// ﾄﾗｯｶｰ2設定
	Line.Setup(Spc, Spd, Sic, 8, 5);									// ﾗｲﾝ表示
}

/***************************************/
void CJcad3GlbView::ResetLightPos(void)									//<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置解除
/***************************************/
{
	Tracker1.Reset(1);													// ﾄﾗｯｶｰ1解除
	Tracker2.Reset(1);													// ﾄﾗｯｶｰ2解除
	Line.Reset();														// ﾗｲﾝ解除
}
