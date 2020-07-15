/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
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
void CJcad3GlbView::SetGrid(void)										//<<< ��د�ސݒ�
/*********************************/
{
	PNTTYPE pnttl, pntbr;
	SCRTYPE scrtl, scrbr;
	int     i, j, i1, j1;
	double  wi, wj;

	if(m_Fovy/m_GridSpace>ViewSize.cy/10) {
		return;
	}
	CalcScreenWorld(1, &pnttl, PlRect[XY].TopLeft(), XY);				// XY �ʏ��د��
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
	CalcScreenWorld(1, &pnttl, PlRect[XY].TopLeft(), XY);				// XY �ڐ���د��
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
	CalcScreenWorld(1, &pnttl, PlRect[YZ].TopLeft(), YZ);				// YZ �ʏ��د��
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
	CalcScreenWorld(1, &pnttl, PlRect[YZ].TopLeft(), YZ);				// YZ �ڐ���د��
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
	CalcScreenWorld(1, &pnttl, PlRect[ZX].TopLeft(), ZX);				// ZX �ʏ��د��
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
	CalcScreenWorld(1, &pnttl, PlRect[ZX].TopLeft(), ZX);				// ZX �ڐ���د��
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
void CJcad3GlbView::SetBase(void)										//<<< ��_/���ݒ�
/*********************************/
{
	double x1, y1, z1, x2, y2, z2, rc, gc, bc;

	MainWnd->GetColor(MainWnd->BaseColor, &rc, &gc, &bc);
	x1 = m_Bpoint.p[0]/1000.0;
	y1 = m_Bpoint.p[1]/1000.0;
	z1 = m_Bpoint.p[2]/1000.0;
	glNewList(DL_BASEPOINT, GL_COMPILE);
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		glPointSize(1.5f);												// �_���ނ��g��
		glBegin(GL_POINTS);												// ���_���W�o�^
			glVertex3d(x1, y1, z1);
		glEnd();
		glPointSize(1.0f);												// �_���ނ�W���ɖ߂�
	glEndList();
	x1 = m_Blspnt.p[0]/1000.0;
	y1 = m_Blspnt.p[1]/1000.0;
	z1 = m_Blspnt.p[2]/1000.0;
	x2 = m_Blepnt.p[0]/1000.0;
	y2 = m_Blepnt.p[1]/1000.0;
	z2 = m_Blepnt.p[2]/1000.0;
	glNewList(DL_BASELINE, GL_COMPILE);
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		glBegin(GL_LINES);												// �������W�o�^
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		glEnd();
		glPointSize(1.5f);												// �_���ނ��g��
		glBegin(GL_POINTS);												// ���_���W�o�^
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		glEnd();
		glPointSize(1.0f);												// �_���ނ�W���ɖ߂�
	glEndList();
}

/*************************************/
void CJcad3GlbView::SetXYZGuide(void)									//<<< XYZ�޲�ސݒ�
/*************************************/
{
	GLdouble modelMat[16];
	GLdouble projMat[16];
	GLint    vprt[4];
	PNTTYPE  pnt, ept;
	double   wx, wy, wz, ox, oy, oz, ln, rc, gc, bc;

	glPushMatrix();
		glCallList(DL_PARS);											// �����}�p�ϊ��Ăяo��
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);					// ���݂������ޭ��s��擾
		glGetDoublev(GL_PROJECTION_MATRIX, projMat);					// ���݂̎ˉe�s��擾
		GetViewport(PRS, &vprt[0], &vprt[1], &vprt[2], &vprt[3]);		// �ޭ��߰Ď擾
		wx = ViewSize.cx*19.0/20.0;										// �E�ォ��X��1/20�̋���
		wy = ViewSize.cy-ViewSize.cx/20.0;
		wz = 0.1;
		gluUnProject(wx, wy, wz, modelMat, projMat, vprt, &ox, &oy, &oz);// ��޼ު�č��W�ɕϊ�
	glPopMatrix();
	pnt.p[0] = ox, pnt.p[1] = oy, pnt.p[2] = oz;						// �ϊ��ʒu��ϊ�
	ScrToPnt(0, &m_Eye, &ept);
	ln = PntD(&ept, &pnt)/20.0;											// ���_���ϊ��ʒu����
	glNewList(DL_XYZGUIDE, GL_COMPILE);
		glBegin(GL_LINES);
			MainWnd->GetColor(MainWnd->XGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// �\���F�ݒ�
			glVertex3d(ox-ln, oy, oz);									// X���W���ē�
			glVertex3d(ox+ln, oy, oz);
			MainWnd->GetColor(MainWnd->YGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// �\���F�ݒ�
			glVertex3d(ox, oy-ln, oz);									// Y���W���ē�
			glVertex3d(ox, oy+ln, oz);
			MainWnd->GetColor(MainWnd->ZGuideColor, &rc, &gc, &bc);
			glColor3d(rc, gc, bc);										// �\���F�ݒ�
			glVertex3d(ox, oy, oz-ln);									// Z���W���ē�
			glVertex3d(ox, oy, oz+ln);
		glEnd();
	glEndList();
}

/*****************************************/
BOOL CJcad3GlbView::SetGLTPoint(FILE* fp)								//<<< GLT�߲�ēo�^
/*****************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	float  ps;
	double vt[3];

	ret = (fscanf_s(fp, "%f", &ps)!=1) ? FALSE : TRUE;					// �߲�Ļ��ގ擾
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// ���W�擾
	vt[Wi1] = atof(b1), vt[Wi2] = atof(b2), vt[Wi3] = 0.0;				// �����ϊ�
	if(ret) {
		glPointSize(ps);												// �_���ފg��
		glBegin(GL_POINTS);
			glVertex3d(vt[0], vt[1], vt[2]);							// ���_���W�o�^
		glEnd();
		glPointSize(1.0f);												// �_���ޕW��
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/****************************************/
BOOL CJcad3GlbView::SetGLTLine(FILE* fp)								//<<< GLTײݓo�^
/****************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3];

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// ���W1�擾
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// �����ϊ�
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// ���W2�擾
	v2[Wi1] = atof(b1), v2[Wi2] = atof(b2), v2[Wi3] = 0.0;				// �����ϊ�
	if(ret) {
		glBegin(GL_LINES);
			glVertex3d(v1[0], v1[1], v1[2]);							// LINE���_�o�^
			glVertex3d(v2[0], v2[1], v2[2]);
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/********************************************/
BOOL CJcad3GlbView::SetGLTPolyline(FILE* fp)							//<<< GLT���ײݓo�^
/********************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	int    i, vn;
	double vt[3];

	ret = (fscanf_s(fp, "%d", &vn)!=1) ? FALSE : TRUE;					// ���_���擾
	if(ret) {
		glBegin(GL_LINE_STRIP);
			for(i=0; i<vn; i++) {
				if(fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) {			// ���W�擾
					ret = FALSE; break;
				}
				vt[Wi1] = atof(b1), vt[Wi2] = atof(b2), vt[Wi3] = 0.0;	// �����ϊ�
				glVertex3d(vt[0], vt[1], vt[2]);						// POLYLINE���_�o�^
			}
		glEnd();
	}
	delete[] b1;
	delete[] b2;
	return ret;
}

/***************************************/
BOOL CJcad3GlbView::SetGLTBox(FILE* fp)									//<<< GLT�ޯ���o�^
/***************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3], v3[3], v4[3];

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// �ŏ����W�擾
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// �����ϊ�
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// �ő���W�擾
	v3[Wi1] = atof(b1), v3[Wi2] = atof(b2), v3[Wi3] = 0.0;				// �����ϊ�
	v2[Wi1] = v3[Wi1],  v2[Wi2] = v1[Wi2],  v2[Wi3] = 0.0;
	v4[Wi1] = v1[Wi1],  v4[Wi2] = v3[Wi2],  v4[Wi3] = 0.0;
	if(ret) {
		glBegin(GL_LINE_STRIP);
			glVertex3d(v1[0], v1[1], v1[2]);							// POLYLINE���_�o�^
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
BOOL CJcad3GlbView::SetGLTSquare(FILE* fp)								//<<< GLT���ړo�^
/******************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	BOOL   ret;
	double v1[3], v2[3], v3[3], v4[3], vs[3], ve[3], p1, p2, ps;

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// �ŏ����W�擾
	v1[Wi1] = atof(b1), v1[Wi2] = atof(b2), v1[Wi3] = 0.0;				// �����ϊ�
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// �ő���W�擾
	v3[Wi1] = atof(b1), v3[Wi2] = atof(b2), v3[Wi3] = 0.0;				// �����ϊ�
	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : ret;	// ���ڕ�1,2�擾
	p1 = atof(b1), p2 = atof(b2);										// �����ϊ�
	v2[Wi1] = v3[Wi1], v2[Wi2] = v1[Wi2], v2[Wi3] = 0.0;
	v4[Wi1] = v1[Wi1], v4[Wi2] = v3[Wi2], v4[Wi3] = 0.0;
	if(ret) {
		glBegin(GL_LINE_STRIP);
			glVertex3d(v1[0], v1[1], v1[2]);							// POLYLINE���_�o�^
			glVertex3d(v2[0], v2[1], v2[2]);
			glVertex3d(v3[0], v3[1], v3[2]);
			glVertex3d(v4[0], v4[1], v4[2]);
			glVertex3d(v1[0], v1[1], v1[2]);
		glEnd();
		vs[0] = v1[0], vs[1] = v1[1], vs[2] = v1[2];					// Wi1�����Ɉړ����Ȃ���
		ve[0] = v4[0], ve[1] = v4[1], ve[2] = v4[2];
		for(ps=v1[Wi1]+p1; ps<=v3[Wi1]; ps+=p1) {						// �ޯ�����ɖ���
			if(ps+0.001<v3[Wi1]) {										// �ő�ʒu��ײ݂��d�Ȃ�Ȃ�
				vs[Wi1] = ps, ve[Wi1] = ps;
				glBegin(GL_LINES);
					glVertex3d(vs[0], vs[1], vs[2]);					// LINE���_�o�^
					glVertex3d(ve[0], ve[1], ve[2]);
				glEnd();
			}
		}
		vs[0] = v1[0], vs[1] = v1[1], vs[2] = v1[2];					// Wi2�����Ɉړ����Ȃ���
		ve[0] = v2[0], ve[1] = v2[1], ve[2] = v2[2];
		for(ps=v1[Wi2]+p2; ps<=v3[Wi2]; ps+=p2) {						// �ޯ�����ɖ���
			if(ps+0.001<v3[Wi2]) {										// �ő�ʒu��ײ݂��d�Ȃ�Ȃ�
				vs[Wi2] = ps, ve[Wi2] = ps;
				glBegin(GL_LINES);
					glVertex3d(vs[0], vs[1], vs[2]);					// LINE���_�o�^
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
BOOL CJcad3GlbView::SetGLTArc(FILE* fp)									//<<< GLT�~�ʓo�^
/***************************************/
{
	char*  b1 = new char[100];
	char*  b2 = new char[100];
	char*  b3 = new char[100];
	BOOL   ret;
	int    i, dv;
	double cp[3], vt[3], rr, st, et, sth, eth, ath, th;

	ret = (fscanf_s(fp, "%s%s", b1, 100, b2, 100)!=2) ? FALSE : TRUE;	// ���S���W�擾
	cp[Wi1] = atof(b1), cp[Wi2] = atof(b2), cp[Wi3] = 0.0;				// �����ϊ�
	ret = (fscanf_s(fp, "%s%s%s%d", b1, 100, b2, 100, b3, 100, &dv)!=4)	// ���a,�J�n/�I���p�x,����\
          ? FALSE : ret;
	rr = atof(b1), st = atof(b2), et = atof(b3);						// �����ϊ�
	vt[Wi3] = 0.0;
	if(ret) {
		sth = PI*st/180.0;												// �J�n�p�x(׼ޱ�)
		eth = PI*et/180.0;												// �I���p�x(׼ޱ�)
		ath = (eth-sth)/dv;												// 1�����̊p�x(׼ޱ�)
		glBegin(GL_LINE_STRIP);
			for(i=0; i<dv; i++) {
				th = sth + ath*i;										// �����ʒu�̊p�x
				vt[Wi1] = cp[Wi1]+rr*cos(th);							// �����ʒu
				vt[Wi2] = cp[Wi2]+rr*sin(th);
				glVertex3d(vt[0], vt[1], vt[2]);						// POLYLINE���_�o�^
			}
			vt[Wi1] = cp[Wi1]+rr*cos(eth);								// �ŏI�ʒu
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
static CString GetGlbFile(int no, int plane)							//<<< GLB̧�ٖ��擾
/********************************************/
{
	CString name("");

	if(no==1) {
		if(plane==0)      { name = G1X_NAME; }							// XY���ʗp
		else if(plane==1) { name = G1Y_NAME; }							// YZ���ʗp
		else              { name = G1Z_NAME; }							// ZX���ʗp
	} else if(no==2) {
		if(plane==0)      { name = G2X_NAME; }							// XY���ʗp
		else if(plane==1) { name = G2Y_NAME; }							// YZ���ʗp
		else              { name = G2Z_NAME; }							// ZX���ʗp
	} else if(no==3) {
		if(plane==0)      { name = G3X_NAME; }							// XY���ʗp
		else if(plane==1) { name = G3Y_NAME; }							// YZ���ʗp
		else              { name = G3Z_NAME; }							// ZX���ʗp
	} else if(no==4) {
		if(plane==0)      { name = G4X_NAME; }							// XY���ʗp
		else if(plane==1) { name = G4Y_NAME; }							// YZ���ʗp
		else              { name = G4Z_NAME; }							// ZX���ʗp
	}
	return name;														// ̧�ٖ���Ԃ�
}

/****************************************************************/
BOOL CJcad3GlbView::ReadGLTTemplate(int mode, int no, int plane)		//<<< GLT����ڰē���
/****************************************************************/		// mode 0:GLB,1:����ڰ�
{
	char*   buf = new char[100];
	FILE*   fp;
	CString fname, tname;
	BOOL    ret;
	UINT    displst;
	int     i, r, g, b;
	double  dm, rc, gc, bc;

	Wi1 = plane;														// �\�����ʂ����W������
	Wi2 = (Wi1+1>2) ? 0 : Wi1+1;
	Wi3 = (Wi2+1>2) ? 0 : Wi2+1;
	if(m_GlbChange) {													// <���Ұ��X�V����>
		dm = (!m_RingKind) ? 15.0+(m_RingSize1-7.0)/3.0 : m_RingSize2;	// �ݸޒ��a�v�Z
	} else {															// <���Ұ��X�V�Ȃ�>
		dm = 0.0;
	}
	fname = (!mode) ? GetAppPath(0)+GetGlbFile(no, plane)				// GLB�̏ꍇ,GLB̧���߽�擾
                    : m_TempPath[no][plane];							// ����ڰĂ̏ꍇ,����ڰ�̧���߽�擾
	if(fopen_s(&fp, fname, "r")!=0) {									// GLŢ�ٵ����
		delete[] buf;
		return FALSE;
	}
	fclose(fp);															// GLŢ�ٸ۰��
	i = fname.ReverseFind('.');
	tname = fname.Left(i) + ".tmp";										// �g���q��ύX
	if(ChgGLTFile(fname, tname, m_GlbChange, (float)dm)) {				// GLŢ�ٕϊ�
		delete[] buf;
		return FALSE;
	}
	if(fopen_s(&fp, tname, "r")!=0) {									// ���̧�ٵ����
		delete[] buf;
		return FALSE;
	}
	ret = (fscanf_s(fp, "%s%d", buf, 100, &i)!=2) ? FALSE : TRUE;		// �擪����ގ擾
	ret = (memcmp(buf, "T00", 3)!=0) ? FALSE : ret;						// <�J�n�����(T00)>�ȊO�ʹװ
	if(!ret) {															// �װ�����̏ꍇ
		fclose(fp);														// ���̧�ٸ۰��
		delete[] buf;
		return FALSE;													// ���~
	}
	displst = glGenLists(1);											// �V�K�ި���ڲ�ؽĔԍ��擾
	if(!mode) {															// <GLB�̏ꍇ>
		m_GLB[plane] = displst;
	} else {															// <����ڰĂ̏ꍇ>
		m_TEMP[plane] = displst;
	}
	glNewList(displst, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		MainWnd->GetColor(MainWnd->TemplateColor, &rc, &gc, &bc);		// �����Ƃ�������ڰĕ\���F
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		while(ret) {
			if(fscanf_s(fp, "%s%d", buf, 100, &i)!=2) {					// ����ގ擾
				ret = FALSE;
			}
			if(memcmp(buf, "T01", 3)==0) {								// <�\���F�ݒ�����(T01)>
				ret = (fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3)
                      ? FALSE : ret;									// RGB�װ(����)�擾
				rc = r/255.0, gc = g/255.0, bc = b/255.0;
				glColor3d(rc, gc, bc);									// �\���F�ݒ�
			} else if(memcmp(buf, "T10", 3)==0) {						// <�߲�ĕ\�������(T10)>
				ret = SetGLTPoint(fp);									// GLT�߲�ēo�^
			} else if(memcmp(buf, "T11", 3)==0) {						// <ײݕ\�������(T11)>
				ret = SetGLTLine(fp);									// GLTײݓo�^
			} else if(memcmp(buf, "T12", 3)==0) {						// <���ײݕ\�������(T12)>
				ret = SetGLTPolyline(fp);								// GLT���ײݓo�^
			} else if(memcmp(buf, "T13", 3)==0) {						// <�ޯ���\�������(T13)>
				ret = SetGLTBox(fp);									// GLT�ޯ���o�^
			} else if(memcmp(buf, "T14", 3)==0) {						// <���ڕ\�������(T14)>
				ret = SetGLTSquare(fp);									// GLT���ړo�^
			} else if(memcmp(buf, "T15", 3)==0) {						// <�~�ʕ\�������(T15)>
				ret = SetGLTArc(fp);									// GLT�~�ʓo�^
			} else if(memcmp(buf, "T99", 3)==0) {						// <�I�������(T99)>
				break;
			} else {
				ret = FALSE;
			}
		}
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	fclose(fp);															// ���̧��CLOSE
	remove(tname);														// ���̧�ٍ폜
	delete[] buf;
	Wi0 = 0, Wi1 = 0, Wi2 = 0, Wi3 = 0;
	return ret;
}

/*********************************/
BOOL CJcad3GlbView::ReadGLB(void)										//<<< GLB����ڰē���
/*********************************/
{
	BOOL ret=TRUE;
	int  i;

	DeleteGLB();														// ��GLB����ڰĔj��
	if(m_GlbDisp==0) {													// GLB����ڰĕ\�����͏I��
		return TRUE;
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	for(i=0; i<3; i++) {												// <0:X-Y, 1:Y-Z, 2:Z-X>
		if(!(ret=ReadGLTTemplate(0, m_GlbDisp, i))) {					// GLT����ڰē���
			break;														// �װ�͒��~
		}
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	return ret;
}

/***********************************/
void CJcad3GlbView::DeleteGLB(void)										//<<< GLB����ڰĔj��
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
void CJcad3GlbView::SetTmpFile(int no, CString x, CString y, CString z)	//<<< ����ڰ�̧���߽�ݒ�
/***********************************************************************/
{
	m_TempPath[no][0] = x;
	m_TempPath[no][1] = y;
	m_TempPath[no][2] = z;
}

/******************************************************************/
void CJcad3GlbView::GetTmpFile(int no,
                               CString* x, CString* y, CString* z)		//<<< ����ڰ�̧���߽�擾
/******************************************************************/
{
	*x = m_TempPath[no][0];
	*y = m_TempPath[no][1];
	*z = m_TempPath[no][2];
}

/***************************************************/
static BOOL ReadParam(FILE* fp, int* id, char* cmd)						//<<< DXF�ް�����
/***************************************************/
{
	fscanf_s(fp, "%d", id);												// ID����
	fscanf_s(fp, "%s", cmd, 1000);										// ����ޓ���
	if(feof(fp)) {														// ̧�ُI����FALSE
		return FALSE;
	} else {															// ̧�ّ��s��TRUE
		return TRUE;
	}
}

/***************************************************************/
static BOOL ReadPoint(FILE* fp, int* id, char* cmd, double* pt)			//<<< ���_�ް�����
/***************************************************************/
{
	int   xf=0, yf=0, zf=0;
	float fd;

	pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
	while(TRUE) {
		if((*id)<10||(*id)>39) {										// ���_�ް��ȊO�͏I��
			break;
		}
		if(sscanf_s(cmd, "%g", &fd)!=1) {								// ���������ϊ�
			return FALSE;
		}
		if((*id)/10==1) {												// <X���W>
			if(xf==1) {
				break;
			} else {
				pt[0] = (double)fd, xf = 1;
			}
		} else if((*id)/10==2) {										// <Y���W>
			if(yf==1) {
				break;
			} else {
				pt[1] = (double)fd, yf = 1;
			}
		} else if((*id)/10==3) {										// <Z���W>
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
BOOL CJcad3GlbView::SetPoint(FILE* fp, int* id, char* cmd)				//<<< POINT�ް��o�^
/**********************************************************/
{
	double pt[3];
	BOOL   ret=TRUE;

	pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <���_�ް�>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// ���_�ް�����
			break;
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		}
	}
	glPointSize(2.5f);													// �_���ފg��
	glBegin(GL_POINTS);
		glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);							// ���_���W�o�^
	glEnd();
	glPointSize(1.0f);													// �_���ޕW��
	return ret;
}

/*********************************************************/
BOOL CJcad3GlbView::SetLine(FILE* fp, int* id, char* cmd)				//<<< LINE�ް��o�^
/*********************************************************/
{
	int    i, cnt=0;
	double pt[3], pa[2][3];
	BOOL   ret=TRUE;

	for(i=0; i<3; i++) {
		pa[0][i] = 0.0, pa[1][i] = 0.0;
	}
	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <���_�ް�>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// ���_�ް�����
			for(i=0; i<3; i++) {
				pa[cnt][i] = pt[i];
			}
			if(++cnt>1) {
				break;
			}
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		}
	}
	glBegin(GL_LINES);
		glVertex3d(pa[0][Wi1], pa[0][Wi2], pa[0][Wi3]);					// LINE���_�o�^
		glVertex3d(pa[1][Wi1], pa[1][Wi2], pa[1][Wi3]);
	glEnd();
	return ret;
}

/********************************************************/
BOOL CJcad3GlbView::SetArc(FILE* fp, int* id, char* cmd)				//<<< ARC�ް��o�^
/********************************************************/
{
	int    i, i1, i2, i3, ix;
	double cp[3], pt[3], th, th1, t;
	float  rr, t1, t2;
	BOOL   ret=TRUE;

	cp[0] = 0.0, cp[1] = 0.0, cp[2] = 0.0;
	rr = 0.0f, t1 = 0.0f, t2 = 0.0f;
	i1 = Wi0;															// �L�����W������
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <���S�_�ް�>
			ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;			// ���_�ް�����
		} else if((*id)==40) {											// <���a�ް�>
			ret = (sscanf_s(cmd, "%g", &rr)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
		} else if((*id)==50) {											// <�J�n�p�x�ް�>
			ret = (sscanf_s(cmd, "%g", &t1)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
		} else if((*id)==51) {											// <�I���p�x�ް�>
			ret = (sscanf_s(cmd, "%g", &t2)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			break;
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		}
	}
	if(fabs(t1)>360.0f) {												// �J�n�p�x�m�F
		ix = (int)t1, i = abs(ix)/360, t1 = t1 - (ix*i);
	}
	if(fabs(t2)>360.0f) {												// �I���p�x�m�F
		ix = (int)t2, i = abs(ix)/360, t2 = t2 - (ix*i);
	}
	th = ((th=(double)(t2-t1))<0.0) ? th + 360.0 : th;					// �J�n�`�I���p�x
	ix = ((int)(th/10.0)<3) ? 3 : (int)(th/10.0);						// �p�x����(��{10�x)
	th = th/ix;															// �����p�x(����)
	th1 = PI*t1/180.0;													// �J�n�p�x(׼ޱ�)
	th = PI*th/180.0;													// �����p�x(׼ޱ�)
	glBegin(GL_LINE_STRIP);
		for(i=0; i<=ix; i++) {
			t = th1+th*i;
			pt[i1] = cp[i1]+(double)rr*cos(t);
			pt[i2] = cp[i2]+(double)rr*sin(t);
			pt[i3] = cp[i3];
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// ARC���_�o�^
		}
	glEnd();
	return ret;
}

/***********************************************************/
BOOL CJcad3GlbView::SetCircle(FILE* fp, int* id, char* cmd)				//<<< CIRCLE�ް��o�^
/***********************************************************/
{
	int    i, i1, i2, i3;
	double cp[3], pt[3], t, th=PI/18.0;
	float  rr;
	BOOL   ret=TRUE;

	cp[0] = 0.0, cp[1] = 0.0, cp[2] = 0.0, rr = 0.0f;
	i1 = Wi0;															// �L�����W������
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <���S�_�ް�>
			ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;			// ���_�ް�����
		} else if((*id)==40) {											// <���a�ް�>
			ret = (sscanf_s(cmd, "%g", &rr)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			break;
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
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
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// CIRCLE���_�o�^
		}
		pt[i1] = cp[i1]+(double)rr*cos(0.0);
		pt[i2] = cp[i2]+(double)rr*sin(0.0);
		pt[i3] = cp[i3];
		glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);
	glEnd();
	return ret;
}

/************************************************************/
BOOL CJcad3GlbView::SetEllipse(FILE* fp, int* id, char* cmd)			//<<< ELLIPSE�ް��o�^
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
	i1 = Wi0;															// �L�����W������
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {
			if(ix==0) {													// <���S�_�ް�>
				ret = (!ReadPoint(fp, id, cmd, cp)) ? FALSE : ret;		// ���_�ް�����
				ix++;
			} else {													// <�����[�_�ް�>
				ret = (!ReadPoint(fp, id, cmd, bp)) ? FALSE : ret;		// ���_�ް�����
			}
		} else if((*id)==40) {											// <�Z���䗦�ް�>
			ret = (sscanf_s(cmd, "%g", &sc)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
		} else if((*id)==41) {											// <�J�n�p�x�ް�>
			ret = (sscanf_s(cmd, "%g", &t1)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
		} else if((*id)==42) {											// <�I���p�x�ް�>
			ret = (sscanf_s(cmd, "%g", &t2)!=1) ? FALSE : ret;			// ���������ϊ�
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			break;
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		}
	}
	r1 = sqrt(bp[i1]*bp[i1]+bp[i2]*bp[i2]);								// ������
	r2 = r1*(double)sc;															// �Z����
	rt = atan2(bp[i2], bp[i1]);											// ��]�p�x
	rt = (rt<-10||rt>10) ? 0.0 : rt;
	th = ((th=(double)(t2-t1))<0.0) ? th + PI*2.0 : th;					// �J�n�`�I���p�x
	ix = ((int)(18.0*th/PI)<3) ? 3 : (int)(18.0*th/PI);					// �p�x����(��{10�x)
	th = th/ix;															// �����p�x(����)
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
			glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);						// ELLIPSE���_�o�^
		}
	glEnd();
	return ret;
}

/*************************************************************/
BOOL CJcad3GlbView::SetPolyline(FILE* fp, int* id, char* cmd)			//<<< POLYLINE�ް��o�^
/*************************************************************/
{
	int    fg=0, st=0;
	double pt[3], ps[3];
	BOOL   ret=TRUE;

	if(!ReadParam(fp, id, cmd)) {										// DXF�ް�����
		return FALSE;
	}
	while(ret) {
		if((*id)>=10&&(*id)<=39) {										// <���_�ް�>
			ret = (!ReadPoint(fp, id, cmd, pt)) ? FALSE : ret;			// ���_�ް�����
			if((*id)==0) {
				break;
			}
		} else if((*id)==70) {											// <POLYLINE�׸�>
			ret = (sscanf_s(cmd, "%d", &fg)!=1) ? FALSE : ret;
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		} else {														// <���_�ް��ȊO>
			ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;				// DXF�ް�����
			if((*id)==0) {
				break;
			}
		}
	}
	glBegin(GL_LINE_STRIP);
		while(ret) {
			if(!strcmp(cmd, "SEQEND")) {								// POLYLINE�I��
				break;													// �����I��
			} else if(!strcmp(cmd, "VERTEX")) {							// <VERTEX�ް�>
				pt[0] = 0.0, pt[1] = 0.0, pt[2] = 0.0;
				ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;			// DXF�ް�����
				while(ret) {
					if((*id)>=10&&(*id)<=39) {							// <���_�ް�>
						ret = (!ReadPoint(fp, id, cmd, pt))
                              ? FALSE : ret;							// ���_�ް�����
						if((*id)==0) {
							break;
						}
					} else {											// <���_�ް��ȊO>
						ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;	// DXF�ް�����
						if((*id)==0) {
							break;
						}
					}
				}
				glVertex3d(pt[Wi1], pt[Wi2], pt[Wi3]);					// POLYLINE���_�o�^
				if(!st) {
					ps[0] = pt[0], ps[1] = pt[1], ps[2] = pt[2], st = 1;
				}
			} else {													// <��L�ȊO�ް�>
				ret = (!ReadParam(fp, id, cmd)) ? FALSE : ret;			// DXF�ް�����
			}
		}
		if(fg&&st) {
			glVertex3d(ps[Wi1], ps[Wi2], ps[Wi3]);						// �̏ꍇ�擪�ް��ǉ�
		}
	glEnd();
	return ret;
}

/******************************************************/
BOOL CJcad3GlbView::ReadDXFTemplate(int no, int plane)					//<<< DXF����ڰē���
/******************************************************/
{
	char*  cmd = new char[1000];
	FILE*  fp;
	BOOL   ret;
	int    id;
	double pt[3], r, g, b;

	Wi0 = -2;
	if(fopen_s(&fp, m_TempPath[no][plane], "r")==0) {    				// ̧��OPEN
		ret = (!ReadParam(fp, &id, cmd)) ? FALSE : TRUE;				// DXF�ް�����
		while(ret) {													// �L�����ʌ���
			Wi0 = (!strcmp(cmd, "ENTITIES")) ? -1 : Wi0;
			if(Wi0==-1&&id>=10&&id<=39) {								// <�L���Ȓ��_�ް�>
				if(!ReadPoint(fp, &id, cmd, pt)) {						// ���_�ް�����
					break;
				}
				if(fabs(pt[0])>EPSILON&&fabs(pt[1])>EPSILON) {			// XY���ʗL��
					Wi0 = 0; break;
				}
				if(fabs(pt[1])>EPSILON&&fabs(pt[2])>EPSILON) {			// YZ���ʗL��
					Wi0 = 1; break;
				}
				if(fabs(pt[2])>EPSILON&&fabs(pt[0])>EPSILON) {			// ZX���ʗL��
					Wi0 = 2; break;
				}
			} else {													// <�L���Ȓ��_�ް��ȊO>
				if(!ReadParam(fp, &id, cmd)) {							// DXF�ް�����
					break;
				}
			}
		}
		fclose(fp);														// ̧��CLOSE
	}
	ret = (Wi0<0) ? FALSE : ret;										// �L�����ʖ��ʹװ
	if(!ret) {															// �װ�͒��~
		delete[] cmd;
		return ret;
	}
	if(plane==0) {
		Wi1 = Wi0;														// �ް����ʂƕ\�����ʂ��
		Wi2 = (Wi1+1>2) ? 0 : Wi1+1;									// �ް��o�^�ʒu����
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
	if(fopen_s(&fp, m_TempPath[no][plane], "r")==0) {    				// ̧��OPEN
		while(ret) {													// ENTITIES�܂�
			if(!ReadParam(fp, &id, cmd)) {								// DXF�ް�����
				ret = FALSE; break;
			}
			if(!strcmp(cmd, "ENTITIES")) {
				break;
			}
		}
		if(ret) {														// ENTITIES�̏ꍇ
			m_TEMP[plane] = glGenLists(1);								// �V�K�ި���ڲ�ؽĔԍ��擾
			glNewList(m_TEMP[plane], GL_COMPILE);						// �ި���ڲ�ؽč쐬�J�n
				MainWnd->GetColor(MainWnd->TemplateColor,&r, &g, &b);
				glColor3d(r, g, b);										// �\���F�ݒ�
				ReadParam(fp, &id, cmd);								// DXF�ް�����
				while(ret) {											// ENDSEC�܂�
					if(!strcmp(cmd, "ENDSEC")) {						// ENTITIES�I��
						break;											// �����I��
					} else if(!strcmp(cmd, "POINT")) {					// <POINT�ް�>
						ret = (!SetPoint(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "LINE")) {					// <LINE�ް�>
						ret = (!SetLine(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "ARC")) {					// <ARC�ް�>
						ret = (!SetArc(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "CIRCLE")) {					// <CIRCLE�ް�>
						ret = (!SetCircle(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "ELLIPSE")) {				// <ELLIPSE�ް�>
						ret = (!SetEllipse(fp, &id, cmd)) ? FALSE : ret;
					} else if(!strcmp(cmd, "POLYLINE")) {				// <POLYLINE�ް�>
						ret = (!SetPolyline(fp, &id, cmd)) ? FALSE : ret;
					} else {											// <��L�ȊO�ް�>
						ret = (!ReadParam(fp, &id, cmd)) ? FALSE : ret;
					}
				}
			glEndList();												// �ި���ڲ�ؽč쐬�I��
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		ret = FALSE;
	}
	delete[] cmd;
	return ret;
}

/**********************************/
BOOL CJcad3GlbView::ReadTEMP(void)										//<<< ����ڰē���
/**********************************/
{
	CString ext;
	BOOL    ret=TRUE;
	int     i;

	DeleteTEMP();														// ������ڰĔj��
	if(m_DspTmpNo==0) {													// ����ڰĕ\�����͏I��
		return TRUE;
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	for(i=0; i<3; i++) {												// <<0:X-Y, 1:Y-Z, 2:Z-X>>
		if(m_TempPath[m_DspTmpNo-1][i]!=_T("")) {
			ext = m_TempPath[m_DspTmpNo-1][i].Right(3);					// �g���q�擾
			if(!ext.CompareNoCase("dxf")) {								// <�g���q:DXF�̏ꍇ>
				ret = ReadDXFTemplate(m_DspTmpNo-1, i);					// DXF����ڰē���
			} else if(!ext.CompareNoCase("glt")) {						// <�g���q:GLT�̏ꍇ>
				ret = ReadGLTTemplate(1, m_DspTmpNo-1, i);				// GLT����ڰē���
			} else {
				ret = FALSE;
			}
			if(!ret) {
				break;													// �װ�͒��~
			}
		}
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	return ret;
}

/************************************/
void CJcad3GlbView::DeleteTEMP(void)									//<<< ����ڰĔj��
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
                               int plane, CString file)					//<<< ø���̧�ِݒ�
/*******************************************************/
{
	BOOL ret;

	if(flg==1) {														// <̧�ِݒ�>
		m_TexFile[plane]   = file;
		m_TexMove1[plane]  = 0.0;
		m_TexMove2[plane]  = 0.0;
		m_TexRotate[plane] = 0;
		m_TexScale[plane]  = 1.0f;
		if(!kind) {														// <BMP�w��̏ꍇ>
			ret = SetTexImageBMP(plane, 0);								// ø����pBMP�摜�ް��ݒ�
		} else {														// <JPG�w��̏ꍇ>
			ret = SetTexImageJPG(plane);								// ø����pJPG�摜�ް��ݒ�
		}
		if(ret) {														// <�ް��ݒ�OK>
			m_TexFovy[plane] = m_Fovy;									// �o�^���̕\����
			m_TexDisp[plane] = 1;
			DrawTexPixels(plane);										// ø����߸�ٓo�^
		} else {														// <�ް��ݒ�NG>
			m_TexFile[plane] = _T("");									// �ް��ر
			m_TexDisp[plane] = 0;
			ClearTexImage(plane);										// ø����p�摜�ް�����
			DeleteTexPixels(plane);										// ø����߸�ٔj��
		}
	} else {															// <̧�ُ���>
		m_TexFile[plane]   = _T("");
		m_TexDisp[plane]   = 0;
		m_TexMove1[plane]  = 0.0;
		m_TexMove2[plane]  = 0.0;
		m_TexRotate[plane] = 0;
		m_TexScale[plane]  = 1.0f;
		ClearTexImage(plane);											// ø����p�摜�ް�����
		DeleteTexPixels(plane);											// ø����߸�ٔj��
	}
	Display(0);															// �ĕ`��
}

/*******************************************************************/
void CJcad3GlbView::GetTexInfo(int plane, CString* fl, double* mv1,
                               double* mv2, int* rt, float* sc)			//<<< ø����\����Ԏ擾
/*******************************************************************/
{
	*fl  = m_TexFile[plane];
	*mv1 = m_TexMove1[plane];
	*mv2 = m_TexMove2[plane];
	*rt  = m_TexRotate[plane];
	*sc  = m_TexScale[plane];
}

/***************************************************************/
void CJcad3GlbView::ChangeTexInfo(int flg, int plane, int mode)			//<<< ø����\����ԕύX
/***************************************************************/
{
	int wk;

	if(m_TexDisp[plane]) {
		if(mode==0) {													// <�ړ�1>
			m_TexMove1[plane] += (flg) ? 0.1 : -0.1;
			wk = (m_TexMove1[plane]>=0) ? (int)(m_TexMove1[plane]*10.0+0.5)
                                        : (int)(m_TexMove1[plane]*10.0-0.5);
			m_TexMove1[plane] = (double)wk/10.0;
			DrawTexPixels(plane);										// ø����߸�ٓo�^
		} else if(mode==1) {											// <�ړ�2>
			m_TexMove2[plane] += (flg) ? 0.1 : -0.1;
			wk = (m_TexMove2[plane]>=0) ? (int)(m_TexMove2[plane]*10.0+0.5)
                                        : (int)(m_TexMove2[plane]*10.0-0.5);
			m_TexMove2[plane] = (double)wk/10.0;
			DrawTexPixels(plane);										// ø����߸�ٓo�^
		} else if(mode==2) {											// <��]>
			if(flg) {													// �E���
				m_TexRotate[plane] =
                (m_TexRotate[plane]+1>3) ? 0 : m_TexRotate[plane]+1;
			} else {													// �����
				m_TexRotate[plane] =
                (m_TexRotate[plane]-1<0) ? 3 : m_TexRotate[plane]-1;
			}
			RotateTexImage(flg, plane);									// ø����p�摜�ް���]
			DrawTexPixels(plane);										// ø����߸�ٓo�^
		} else if(mode==3) {											// <���ޕύX>
			m_TexScale[plane] += (flg) ? 0.01f : -0.01f;
			wk = (m_TexScale[plane]>=0) ? (int)(m_TexScale[plane]*100.0+0.5)
                                        : (int)(m_TexScale[plane]*100.0-0.5);
			m_TexScale[plane] = (float)wk/100.0f;
			DrawTexPixels(plane);										// ø����߸�ٓo�^
		}
		Display(0);														// �ĕ`��
	}
}

/******************************************************/
BOOL CJcad3GlbView::SetTexImageBMP(int plane, int flg)					//<<< ø����pBMP�摜�ް��ݒ�
/******************************************************/
{
	WIN32_FIND_DATA wfd;
	CString path;
	FILE*   fp;
	int     i, j, w, h;
	BOOL    ret;

	if(flg==0) {														// <̧�ٖ��w��>
		path = m_TexFile[plane];										// �w��̧�ٖ�
	} else {															// <���̧�َw��>
		path = GetAppPath(0)+CNV_NAME;									// JPEĢ�ٕϊ����̧�ٖ�
	}
	if(FindFirstFile(path, &wfd)==INVALID_HANDLE_VALUE) {				// ̧�ٖ��̏ꍇ
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
		return FALSE;
	}
	if(fopen_s(&fp, path, "rb")!=0) {
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
		return FALSE;
	}
	ClearTexImage(plane);												// ø����p�摜�ް�����
	int* hd = new int[54];
	for(i=0; i<54; i++) {
		hd[i] = fgetc(fp);
	}
	w = hd[18]+hd[19]*256+hd[20]*256*256+hd[21]*256*256*256;			// �摜�̉��߸�ِ�
	m_TexWidth[plane] = w;
	h = hd[22]+hd[23]*256+hd[24]*256*256+hd[25]*256*256*256;			// �摜�̏c�߸�ِ�
	m_TexHeight[plane] = h;
	if(w%4||h%4) {														// <�߸�ِ���4�̔{���łȂ�>
		SetSplashMsg(IDS_ERR_PICFILE2);									// �摜̧�ٴװ
		ret = FALSE;
	} else {															// <�����ް�>
		m_TexImage[plane] = (GLubyte *)malloc(sizeof(GLubyte)*w*h*3);	// ø����p�摜�ް��̈�m��
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
BOOL CJcad3GlbView::SetTexImageJPG(int plane)							//<<< ø����pJPG�摜�ް��ݒ�
/*********************************************/
{
	WIN32_FIND_DATA wfd;
	HANDLE  hDIB;
	CString path;

	if(FindFirstFile(m_TexFile[plane], &wfd)==INVALID_HANDLE_VALUE) {	// ̧�ٖ��̏ꍇ
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
		return FALSE;
	}
	hDIB = IKJpegFileLoad(m_TexFile[plane], 0, NULL, NULL, NULL);		// JPEĢ�ٓǂݍ���
	if(hDIB==0) {
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
		return FALSE;
	}
	path = GetAppPath(0)+CNV_NAME;										// JPEĢ�ٕϊ����̧�ٖ�
	if(!IKBmpFileSave(path, hDIB, FALSE, 0, NULL, NULL, NULL)) {		// �ޯ�ϯ��̧�ُo��
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
		return FALSE;
	}
	if(!IKFreeMemory(hDIB)) {											// ��؉��
		SetSplashMsg(IDS_ERR_PICFILE);									// �摜̧�ٴװ
	}
	SetTexImageBMP(plane, 1);											// ø����pBMP�摜�ް��ݒ�
	remove(path);														// ���̧�ٍ폜
	return TRUE;
}

/********************************************/
void CJcad3GlbView::ClearTexImage(int plane)							//<<< ø����p�摜�ް�����
/********************************************/
{
	if(m_TexImage[plane]!=NULL) {										// <ø����p�摜�ް��ݒ�ς�>
		free(m_TexImage[plane]);										// �̈���
		m_TexImage[plane] = NULL;										// �ϐ�������
		m_TexWidth[plane] = 0, m_TexHeight[plane] = 0;
	}
}

/******************************************************/
void CJcad3GlbView::RotateTexImage(int flg, int plane)					//<<< ø����p�摜�ް���]
/******************************************************/
{
	GLubyte* buf;
	int      i, j, k, w, h, ct=0;

	w  = m_TexWidth[plane];												// ���݂̉摜���߸�ِ�
	h  = m_TexHeight[plane];											// ���݂̉摜�c�߸�ِ�
	buf = (GLubyte *)malloc(sizeof(GLubyte)*w*h*3);						// ø����p�摜�ް��̈�m��
	if(buf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	if(flg) {															// <�E����90�x��]>
		for(i=w-1; i>=0; i--) {
			for(j=0; j<h; j++) {
				for(k=0; k<3; k++) {
					buf[ct++] = m_TexImage[plane][(i*3)+(j*w*3)+k];
				}
			}
		}
	} else {															// <������90�x��]>
		for(i=0; i<w; i++) {
			for(j=h-1; j>=0; j--) {
				for(k=0; k<3; k++) {
					buf[ct++] = m_TexImage[plane][(i*3)+(j*w*3)+k];
				}
			}
		}
	}
	free(m_TexImage[plane]);											// ���̈���
	m_TexImage[plane] = buf;
	m_TexWidth[plane]  = h;												// ��]��̉摜���߸�ِ�
	m_TexHeight[plane] = w;												// ��]��̉摜�c�߸�ِ�
}

/*****************************************/
void CJcad3GlbView::ChangeTexPixels(void)								//<<< ø����߸�ٍX�V
/*****************************************/
{
	int i;

	for(i=0; i<3; i++) {
		if(m_TexDisp[i]) {
			DrawTexPixels(i);											// ø����߸�ٓo�^
		}
	}
}

/********************************************/
void CJcad3GlbView::DrawTexPixels(int plane)							//<<< ø����߸�ٓo�^
/********************************************/
{
	PNTTYPE cpt;
	int     xp, yp, zp, wk;
	double  wp, hp, wt, ht, wt2, ht2, px, py, pz, sx, sy, sz;
	float   sc;

	GetCentPnt(&cpt);													// ��ʾ����擾
	if(plane==XY) {														// <XY����>
		sc = (float)m_TexFovy[XY]/(float)m_Fovy;						// ���݂̕\���{��(�Γo�^��)
		wp = sc*m_TexScale[XY]*PlDstW[XY]/PlSizeW[XY];					// �{���������1�߸�ى�����(mm)
		hp = sc*m_TexScale[XY]*PlDstH[XY]/PlSizeH[XY];					// �{���������1�߸�ُc����(mm)
		wt = m_TexWidth[XY]*wp*0.5;										// ø����������̔���(mm)
		ht = m_TexHeight[XY]*hp*0.5;									// ø����c�����̔���(mm)
		px = ht - m_TexMove2[XY];										// ø��������̍��W
		py = -wt + m_TexMove1[XY];
		sx = cpt.p[0] + PlDstH[XY]/2.0;									// ��ʍ����̍��W
		sy = cpt.p[1] - PlDstW[XY]/2.0;
		wk = (sx<px) ? (int)((px-sx)/hp)+1 : 0;							// �͂ݏo��(�J�b�g����)X�����߸�ِ�
		xp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wk = (sy>py) ? (int)((sy-py)/wp)+1 : 0;							// �͂ݏo��(�J�b�g����)Y�����߸�ِ�
		yp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wt2 = yp*wp, ht2 = xp*hp;										// �J�b�g����X/Y�����̒���(mm)
		if(m_TEXTURE[XY]==0) {
			m_TEXTURE[XY] = glGenLists(1);								// �V�K�ި���ڲ�ؽĔԍ��擾
		}
		glNewList(m_TEXTURE[XY], GL_COMPILE);							// XYø����ި���ڲ�ؽ�
			glScissor(PlRect[XY].left+1, ViewSize.cy-PlRect[XY].bottom,
                      PlRect[XY].Width()-1, PlRect[XY].Height()-1);		// �`��̈�𐧌�
			glEnable(GL_SCISSOR_TEST);									// ��ް�ýĂ�L����
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(px-ht2, py+wt2, 0.0);
			glPixelZoom(m_TexScale[XY]*sc, m_TexScale[XY]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[XY]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, yp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, xp);
			glDrawPixels(m_TexWidth[XY]-yp, m_TexHeight[XY]-xp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[XY]);
			glDisable(GL_SCISSOR_TEST);									// ��ް�ýĂ𖳌���
		glEndList();
	} else if(plane==YZ) {												// <YZ����>
		sc = (float)m_TexFovy[YZ]/(float)m_Fovy;						// ���݂̕\���{��(�Γo�^��)
		wp = sc*m_TexScale[YZ]*PlDstW[YZ]/PlSizeW[YZ];					// �{���������1�߸�ى�����(mm)
		hp = sc*m_TexScale[YZ]*PlDstH[YZ]/PlSizeH[YZ];					// �{���������1�߸�ُc����(mm)
		wt = m_TexWidth[YZ]*wp*0.5;										// ø����������̔���(mm)
		ht = m_TexHeight[YZ]*hp*0.5;									// ø����c�����̔���(mm)
		py = -wt + m_TexMove1[YZ];										// ø��������̍��W
		pz = -ht + m_TexMove2[YZ];
		sy = cpt.p[1] - PlDstW[YZ]/2.0;									// ��ʍ����̍��W
		sz = cpt.p[2] - PlDstH[YZ]/2.0;
		wk = (sy>py) ? (int)((sy-py)/wp)+1 : 0;							// �͂ݏo��(�J�b�g����)Y�����߸�ِ�
		yp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wk = (sz>pz) ? (int)((sz-pz)/hp)+1 : 0;							// �͂ݏo��(�J�b�g����)Z�����߸�ِ�
		zp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wt2 = yp*wp, ht2 = zp*hp;										// �J�b�g����Y/Z�����̒���(mm)
		if(m_TEXTURE[YZ]==0) {
			m_TEXTURE[YZ] = glGenLists(1);								// �V�K�ި���ڲ�ؽĔԍ��擾
		}
		glNewList(m_TEXTURE[YZ], GL_COMPILE);							// XYø����ި���ڲ�ؽ�
			glScissor(PlRect[YZ].left+1, ViewSize.cy-PlRect[YZ].bottom,
                      PlRect[YZ].Width()-1, PlRect[YZ].Height()-1);		// �`��̈�𐧌�
			glEnable(GL_SCISSOR_TEST);									// ��ް�ýĂ�L����
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(0.0, py+wt2, pz+ht2);
			glPixelZoom(m_TexScale[YZ]*sc, m_TexScale[YZ]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[YZ]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, yp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, zp);
			glDrawPixels(m_TexWidth[YZ]-yp, m_TexHeight[YZ]-zp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[YZ]);
			glDisable(GL_SCISSOR_TEST);									// ��ް�ýĂ𖳌���
		glEndList();
	} else if(plane==ZX) {												// <ZX����>
		sc = (float)m_TexFovy[ZX]/(float)m_Fovy;						// ���݂̕\���{��(�Γo�^��)
		wp = sc*m_TexScale[ZX]*PlDstW[ZX]/PlSizeW[ZX];					// �{���������1�߸�ى�����(mm)
		hp = sc*m_TexScale[ZX]*PlDstH[ZX]/PlSizeH[ZX];					// �{���������1�߸�ُc����(mm)
		wt = m_TexWidth[ZX]*wp*0.5;										// ø����������̔���(mm)
		ht = m_TexHeight[ZX]*hp*0.5;									// ø����c�����̔���(mm)
		pz = -ht + m_TexMove2[ZX];										// ø��������̍��W
		px = wt - m_TexMove1[ZX];
		sz = cpt.p[2] - PlDstH[ZX]/2.0;									// ��ʍ����̍��W
		sx = cpt.p[0] + PlDstW[ZX]/2.0;
		wk = (sz>pz) ? (int)((sz-pz)/hp)+1 : 0;							// �͂ݏo��(�J�b�g����)Z�����߸�ِ�
		zp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wk = (sx<px) ? (int)((px-sx)/wp)+1 : 0;							// �͂ݏo��(�J�b�g����)X�����߸�ِ�
		xp = (wk%4) ? ((int)(wk/4)+1)*4 : wk;							// 4�̔{���ɒ���
		wt2 = xp*wp, ht2 = zp*hp;										// �J�b�g����Z/X�����̒���(mm)
		if(m_TEXTURE[ZX]==0) {
			m_TEXTURE[ZX] = glGenLists(1);								// �V�K�ި���ڲ�ؽĔԍ��擾
		}
		glNewList(m_TEXTURE[ZX], GL_COMPILE);							// XYø����ި���ڲ�ؽ�
			glScissor(PlRect[ZX].left+1, ViewSize.cy-PlRect[ZX].bottom,
                      PlRect[ZX].Width()-1, PlRect[ZX].Height()-1);		// �`��̈�𐧌�
			glEnable(GL_SCISSOR_TEST);									// ��ް�ýĂ�L����
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glRasterPos3d(px-wt2, 0.0, pz+ht2);
			glPixelZoom(m_TexScale[ZX]*sc, m_TexScale[ZX]*sc);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_TexWidth[ZX]);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, xp);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, zp);
			glDrawPixels(m_TexWidth[ZX]-xp, m_TexHeight[ZX]-zp, GL_RGB,
                         GL_UNSIGNED_BYTE, m_TexImage[ZX]);
			glDisable(GL_SCISSOR_TEST);									// ��ް�ýĂ𖳌���
		glEndList();
	}
}

/**********************************************/
void CJcad3GlbView::DeleteTexPixels(int plane)							//<<< ø����߸�ٔj��
/**********************************************/
{
	if(m_TEXTURE[plane]!=0) {
		glDeleteLists(m_TEXTURE[plane], 1);
		m_TEXTURE[plane] = 0;
	}
}

/**************************************************************/
void CJcad3GlbView::DrawBox(PNTTYPE sp, PNTTYPE ep, int plane)			//<<< ��`�\��
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
	MainWnd->GetColor(MainWnd->BoxColor, &r, &g, &b);					// �\���F�擾
	if(m_BOX==0) {
		m_BOX = glGenLists(1);											// �V�K�ި���ڲ�ؽĔԍ��擾
	}
	glNewList(m_BOX, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		glColor3d(r, g, b);												// �\���F�ݒ�
		glBegin(GL_LINE_STRIP);
			for(i=0; i<5; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// ���_�o�^
			}
		glEnd();
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	m_BoxPlane = plane;
}

/***********************************/
void CJcad3GlbView::DeleteBox(void)										//<<< ��`�j��
/***********************************/
{
	if(m_BOX!=0) {
		glDeleteLists(m_BOX, 1);
		m_BOX = 0;
	}
	m_BoxPlane = 0;
}

/*****************************************************************/
void CJcad3GlbView::DrawCircle(PNTTYPE cp, PNTTYPE ep, int plane)		//<<< �~�\��
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[73];
	int      i, i1, i2, i3;
	double   rr, th, r, g, b;

	i1 = plane;
	i2 = (i1+1>2) ? 0 : i1 + 1;
	i3 = (i2+1>2) ? 0 : i2 + 1;
	rr = PntD(&cp, &ep);												// 2�_�ԋ���(���a)
	th = PI/36.0;														// 5�x
	for(i=0; i<72; i++) {
		pa[i].p[i1] = cp.p[i1] + rr*cos(th*i);							// �~���W�ݒ�
		pa[i].p[i2] = cp.p[i2] + rr*sin(th*i);
		pa[i].p[i3] = cp.p[i3];
	}
	pa[i] = pa[0];														// �n�_����_�Ÿ۰��
	MainWnd->GetColor(MainWnd->CircleColor, &r, &g, &b);				// �\���F�擾
	if(m_CIRCLE==0) {
		m_CIRCLE = glGenLists(1);										// �V�K�ި���ڲ�ؽĔԍ��擾
	}
	glNewList(m_CIRCLE, GL_COMPILE);									// �ި���ڲ�ؽč쐬�J�n
		glColor3d(r, g, b);												// �\���F�ݒ�
		glBegin(GL_LINE_STRIP);
			for(i=0; i<73; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// ���_�o�^
			}
		glEnd();
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	m_CirclePlane = plane;
	delete[] pa;
}

/**************************************/
void CJcad3GlbView::DeleteCircle(void)									//<<< �~�j��
/**************************************/
{
	if(m_CIRCLE!=0) {
		glDeleteLists(m_CIRCLE, 1);
		m_CIRCLE = 0;
	}
	m_CirclePlane = 0;
}

/********************************************************/
void CJcad3GlbView::MeasureSCALE(PNTTYPE p1, PNTTYPE p2)				//<<< ��K�o�^
/********************************************************/
{
	int    i, i1, i2, i3, c1, c2;
	double pa[4][3], mn1, mn2, mx1, mx2, ln1, ln2, mm, rc, gc, bc;

	MainWnd->GetColor(MainWnd->MeasureColor, &rc, &gc, &bc);			// �\���F�擾(���F)
	i1 = m_ScalePlane;													// ���W���̐ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	if(p1.p[i1]<p2.p[i1]) {												// i1���̍ŏ�/�ő�
		mn1 = p1.p[i1], mx1 = p2.p[i1];
	} else {
		mn1 = p2.p[i1], mx1 = p1.p[i1];
	}
	if(p1.p[i2]<p2.p[i2]) {												// i2���̍ŏ�/�ő�
		mn2 = p1.p[i2], mx2 = p2.p[i2];
	} else {
		mn2 = p2.p[i2], mx2 = p1.p[i2];
	}
	ln1 = mx1 - mn1;													// i1�������̒���
	ln2 = mx2 - mn2;													// i2�������̒���
	c1 = (int)(ln1-0.001);												// i1�������̖ڐ��萔
	c2 = (int)(ln2-0.001);												// i2�������̖ڐ��萔
	if(m_SCALE==0) {
		m_SCALE = glGenLists(1);										// �V�K�ި���ڲ�ؽĔԍ��擾
	}
	glNewList(m_SCALE, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		pa[0][i1] = mn1, pa[0][i2] = mn2, pa[0][i3] = 0.0;				// <�l���_�̐ݒ�>
		pa[1][i1] = mx1, pa[1][i2] = mn2, pa[1][i3] = 0.0;
		pa[2][i1] = mx1, pa[2][i2] = mx2, pa[2][i3] = 0.0;
		pa[3][i1] = mn1, pa[3][i2] = mx2, pa[3][i3] = 0.0;
		glBegin(GL_LINE_STRIP);
			for(i=0; i<4; i++) {
				glVertex3d(pa[i][0], pa[i][1], pa[i][2]);				// ���_�o�^
			}
			glVertex3d(pa[0][0], pa[0][1], pa[0][2]);
		glEnd();
		glBegin(GL_LINES);
			for(i=0; i<c1; i++) {										// <i1�������̖ڐ���>
				mm = ((i+1)%10==0) ? 1.0 : (((i+1)%5==0)?0.75:0.5);		// �ڐ��蕝�̐ݒ�
				pa[0][i1] = mn1+(i+1), pa[0][i2] = mn2;
				pa[1][i1] = mn1+(i+1), pa[1][i2] = mn2+mm;
				pa[2][i1] = mn1+(i+1), pa[2][i2] = mx2;
				pa[3][i1] = mn1+(i+1), pa[3][i2] = mx2-mm;
				glVertex3d(pa[0][0], pa[0][1], pa[0][2]);				// ���_�o�^
				glVertex3d(pa[1][0], pa[1][1], pa[1][2]);
				glVertex3d(pa[2][0], pa[2][1], pa[2][2]);
				glVertex3d(pa[3][0], pa[3][1], pa[3][2]);
			}
			for(i=0; i<c2; i++) {										// <i2�������̖ڐ���>
				mm = ((i+1)%10==0) ? 1.0 : (((i+1)%5==0)?0.75:0.5);		// �ڐ��蕝�̐ݒ�
				pa[0][i1] = mn1,    pa[0][i2] = mn2+(i+1);
				pa[1][i1] = mn1+mm, pa[1][i2] = mn2+(i+1);
				pa[2][i1] = mx1,    pa[2][i2] = mn2+(i+1);
				pa[3][i1] = mx1-mm, pa[3][i2] = mn2+(i+1);
				glVertex3d(pa[0][0], pa[0][1], pa[0][2]);				// ���_�o�^
				glVertex3d(pa[1][0], pa[1][1], pa[1][2]);
				glVertex3d(pa[2][0], pa[2][1], pa[2][2]);
				glVertex3d(pa[3][0], pa[3][1], pa[3][2]);
			}
		glEnd();
	glEndList();														// �ި���ڲ�ؽč쐬�I��
}

/*************************************/
void CJcad3GlbView::DeleteSCALE(void)									//<<< ��K�j��
/*************************************/
{
	if(m_SCALE!=0) {
		glDeleteLists(m_SCALE, 1);
		m_SCALE = 0;
	}
	m_ScalePlane = 0;
}

/*******************************************************/
void CJcad3GlbView::MeasurePROT(PNTTYPE cp, PNTTYPE ep)					//<<< ���x��o�^
/*******************************************************/
{
	PNTTYPE* pa = new PNTTYPE[72];
	PNTTYPE  pb;
	VECTYPE  vec;
	int      i, i1, i2, i3;
	double   len, l1, l2, ath, dth, rc, gc, bc;

	MainWnd->GetColor(MainWnd->MeasureColor, &rc, &gc, &bc);			// �\���F�擾(���F)
	i1 = m_ProtPlane;													// ���W���̐ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	l1 = ep.p[i1]-cp.p[i1];
	l2 = ep.p[i2]-cp.p[i2];
	len = sqrt(l1*l1+l2*l2);											// ���x�피�a
	dth = (2.0*PI)/72.0;												// 32������]�p�x
	for(ath=0.0, i=0; i<72; ath+=dth, i++) {
		pa[i].p[i1] = cp.p[i1]+len*cos(ath);							// ���_���W�ݒ�
		pa[i].p[i2] = cp.p[i2]+len*sin(ath);
		pa[i].p[i3] = 0.0;
	}
	if(m_PROT==0) {
		m_PROT = glGenLists(1);											// �V�K�ި���ڲ�ؽĔԍ��擾
	}
	glNewList(m_PROT, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		glColor3d(rc, gc, bc);											// �\���F�ݒ�
		glBegin(GL_LINE_STRIP);
			for(i=0; i<72; i++) {
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// ���_�o�^
			}
			glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);
		glEnd();
		glBegin(GL_LINES);
			for(i=0; i<72; i++) {										// <�ڐ���̐ݒ�>
				len = (i%18==0) ? 1.0 : ((i%2==0)?0.75:0.5);			// �ڐ��蕝�̐ݒ�
				VecV(&pa[i], &cp, &vec);								// ���S�_�����޸��
				VecN(&vec);												// �P���޸�ى�
				pb = pa[i];												// �ڐ��ʒu�ݒ�
				pb.p[i1] += vec.p[i1]*len;
				pb.p[i2] += vec.p[i2]*len;
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// ���_�o�^
				glVertex3d(pb.p[0], pb.p[1], pb.p[2]);
			}
		glEnd();
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	delete[] pa;
}

/************************************/
void CJcad3GlbView::DeletePROT(void)									//<<< ���x��j��
/************************************/
{
	if(m_PROT!=0) {
		glDeleteLists(m_PROT, 1);
		m_PROT = 0;
	}
	m_ProtPlane = 0;
}

/******************************************/
BOOL CJcad3GlbView::CheckMeasure(int menu)								//<<< ��K/���x��\���m�F
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
                                PNTTYPE* pa, PNTTYPE* pb, int plane)	//<<< ����o�^
/********************************************************************/
{
	int    i, j;
	double r, g, b;

	if(pn<2) {
		DeleteBaseData();				                                // ����j��
		return;
	}
	m_DAraFlg = 1;														// �\���͈͐ݒ��׸�:ON
	for(i=0; i<DIM; i++) {
		m_DAra.bp[0].p[i] = DBL_MAX;									// �\���̈揉����
		m_DAra.bp[1].p[i] = 1.0-DBL_MAX;
	}
	MainWnd->GetColor(MainWnd->MeasureColor, &r, &g, &b);				// �\���F�擾(���F)
	if(m_BASE==0) {
		m_BASE = glGenLists(1);											// �V�K�ި���ڲ�ؽĔԍ��擾
	}
	glNewList(m_BASE, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		glColor3d(r, g, b);												// �\���F�ݒ�
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
				glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);			// ���_�o�^
			}
			if(bc) {													// ���E����:�̏ꍇ
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);			// �n�_�o�^
			}
		glEnd();
		if(cn>1) {														// <2�{�ڂ��L���ȏꍇ>
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
					glVertex3d(pb[i].p[0], pb[i].p[1], pb[i].p[2]);		// ���_�o�^
				}
				if(bc) {												// ���E����:�̏ꍇ
					glVertex3d(pb[0].p[0], pb[0].p[1], pb[0].p[2]);		// �n�_�o�^
				}
			glEnd();
		}
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	if(plane>=0) {														// ����\�����ʐݒ�
		m_BasePlane = plane;
	}
}

/****************************************/
void CJcad3GlbView::DeleteBaseData(void)								//<<< ����j��
/****************************************/
{
	if(m_BASE!=0) {
		glDeleteLists(m_BASE, 1);
		m_BASE = 0;
	}
	m_BasePlane = 0;
	m_DAraFlg = 0;														// �\���͈͐ݒ��׸�:OFF
}

/**************************************************/
void CJcad3GlbView::StartStageDisplay(BOXTYPE box)						//<<< ڲ��ڰ�ݸ޽ð�ޏ�������
/**************************************************/
{
	PNTTYPE cpt;

	SscrA = m_Cent3D;
	Wi0   = m_Fovy;
	m_Fovy = (int)((BoxCenter(box, &cpt)*1000.0)/0.8+0.5);				// �ޯ������&�ő啝�擾
	ScrToPnt(1, &m_Cent3D, &cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	CentPars();															// �����}�Q�Ɠ_�ύX
}

/*****************************************/
void CJcad3GlbView::EndStageDisplay(void)								//<<< ڲ��ڰ�ݸ޽ð�ޏI������
/*****************************************/
{
	m_Cent3D = SscrA;
	m_Fovy   = Wi0;
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	CentPars();															// �����}�Q�Ɠ_�ύX
}

/********************************************************/
void CJcad3GlbView::SetStage(int* flg, double size[][3])				//<<< ڲ��ڰ�ݸ޽ð�ސݒ�
/********************************************************/
{
	double rc, gc, bc;

	MainWnd->GetColor(MainWnd->BoolColor, &rc, &gc, &bc);				// �\���F�擾(�D�F)
	if(flg[0]||flg[1]||flg[2]||flg[3]) {
		if(m_STAGE==0) {
			m_STAGE = glGenLists(1);									// �V�K�ި���ڲ�ؽĔԍ��擾
		}
		glNewList(m_STAGE, GL_COMPILE);									// �ި���ڲ�ؽč쐬�J�n
			glColor3d(rc, gc, bc);										// �\���F�ݒ�
			if(flg[0]) {												// <���>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// ���_�o�^
					glVertex3d(size[2][0], size[2][1], size[2][2]);
					glVertex3d(size[3][0], size[3][1], size[3][2]);
					glVertex3d(size[1][0], size[1][1], size[1][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
			if(flg[1]) {												// <�w��>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// ���_�o�^
					glVertex3d(size[1][0], size[1][1], size[1][2]);
					glVertex3d(size[5][0], size[5][1], size[5][2]);
					glVertex3d(size[4][0], size[4][1], size[4][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
			if(flg[2]) {												// <�E����>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[1][0], size[1][1], size[1][2]);		// ���_�o�^
					glVertex3d(size[3][0], size[3][1], size[3][2]);
					glVertex3d(size[7][0], size[7][1], size[7][2]);
					glVertex3d(size[5][0], size[5][1], size[5][2]);
					glVertex3d(size[1][0], size[1][1], size[1][2]);
				glEnd();
			}
			if(flg[3]) {												// <������>
				glBegin(GL_LINE_STRIP);
					glVertex3d(size[0][0], size[0][1], size[0][2]);		// ���_�o�^
					glVertex3d(size[4][0], size[4][1], size[4][2]);
					glVertex3d(size[6][0], size[6][1], size[6][2]);
					glVertex3d(size[2][0], size[2][1], size[2][2]);
					glVertex3d(size[0][0], size[0][1], size[0][2]);
				glEnd();
			}
		glEndList();													// �ި���ڲ�ؽč쐬�I��
	} else {
		DeleteStage();													// ���ð�ޔj��
	}
}

/*************************************/
void CJcad3GlbView::DeleteStage(void)									//<<< ڲ��ڰ�ݸ޽ð�ލ폜
/*************************************/
{
	if(m_STAGE!=0) {
		glDeleteLists(m_STAGE, 1);
		m_STAGE = 0;
	}
}

/****************************************************************/
void CJcad3GlbView::SetLightPos(int no, LIGHT* lgt, double* oct)		//<<< ڲ��ڰ�ݸތ����ʒu�ݒ�
/****************************************************************/
{
	PNTTYPE p1, p2;
	int     i, j;

	for(i=0, j=0; i<8; i++) {
		p1.p[0] = lgt[i].x1+oct[0];										// ���̒��S���W�����Z
		p1.p[1] = lgt[i].y1+oct[1];
		p1.p[2] = lgt[i].z1+oct[2];
		p2.p[0] = lgt[i].x2+oct[0];
		p2.p[1] = lgt[i].y2+oct[1];
		p2.p[2] = lgt[i].z2+oct[2];
		if(i==no) {
			Spa[0] = p1, Spa[1] = p2, Sia[0] = 0, Sia[1] = 1;			// �ׯ�����ݒ�
		} else {
			Spb[j] = p1, Spb[j+1] = p2, Sib[j] = 3, Sib[j+1] = 3;		// �ׯ��2���ݒ�
			j += 2;
		}
		Spc[i] = p1, Spd[i] = p2, Sic[i] = (i==no) ? 0 : 3;				// ײݏ��ݒ�
	}
	Tracker1.Setup(Spa, Sia, NULL, 2, 5, 0);							// �ׯ��1�ݒ�
	Tracker2.Setup(Spb, Sib, NULL, 14, 5, 0);							// �ׯ��2�ݒ�
	Line.Setup(Spc, Spd, Sic, 8, 5);									// ײݕ\��
}

/***************************************/
void CJcad3GlbView::ResetLightPos(void)									//<<< ڲ��ڰ�ݸތ����ʒu����
/***************************************/
{
	Tracker1.Reset(1);													// �ׯ��1����
	Tracker2.Reset(1);													// �ׯ��2����
	Line.Reset();														// ײ݉���
}
