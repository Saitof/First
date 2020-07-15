/////////////////////////////////////////////////////////////////////////////
// �g���b�J�[�N���X(CTracker)��`
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
CTracker::CTracker(void)												//<<< �׽�̍\�z
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
                     int pn, int mod, int dsp)							//<<< �ׯ���ݒ�
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
	Lsno = glGenLists(1);												// �ި���ڲ�ؽĔԍ��擾
	glNewList(Lsno, GL_COMPILE);										// �ި���ڲ�ؽč쐬�J�n
		if(dsp==1||dsp==2) {
			glColor3d(rc[ia[0]], gc[ia[0]], bc[ia[0]]);					// �\���F�ݒ�
			fv = View->GetDispHeight()/40.0;							// ���ʕ\����(mm�P��)��1/40
			glBegin(GL_LINES);											// �\���\��
				glVertex3d(pa[0].p[0]-fv, pa[0].p[1], pa[0].p[2]);		// ���W�ݒ�
				glVertex3d(pa[0].p[0]+fv, pa[0].p[1], pa[0].p[2]);
				glVertex3d(pa[0].p[0], pa[0].p[1]-fv, pa[0].p[2]);		// ���W�ݒ�
				glVertex3d(pa[0].p[0], pa[0].p[1]+fv, pa[0].p[2]);
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]-fv);		// ���W�ݒ�
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]+fv);
			glEnd();
			glPointSize(3.0f);											// �ׯ���_���ސݒ�
			glBegin(GL_POINTS);
				glVertex3d(pa[0].p[0], pa[0].p[1], pa[0].p[2]);			// ���W�ݒ�
			glEnd();
			st = 1;
		}
		if((dsp!=1&&dsp!=2)||pn>1) {
			if(dsp==0) {												// �ׯ���_���ސݒ�
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
			if(op==NULL) {												// <<�\����߼�݂Ȃ�>>
				glBegin(GL_POINTS);
				for(i=st; i<pn; i++) {
					if(XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
                       YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
                       ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2) {		// <����͈͓�>
						glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);		// �\���F�ݒ�
						glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);	// ���W�ݒ�
					}
				}
				glEnd();
			} else {													// <<�\����߼�݂���>>
				pl = View->GetPlSize();									// ���ʍ�(�߸�ْP��)�擾
				fv = View->GetDispHeight();								// ���ʕ\����(mm�P��)�擾
				sz = (PntSize/2.0)*fv/pl;								// �_����3D�ϊ�
				for(i=st; i<pn; i++) {
					glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);			// �\���F�ݒ�
					if(op[i]==1) {										// <�\����߼��:1>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==2) {								// <�\����߼��:2>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==3) {								// <�\����߼��:3>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==4) {								// <�\����߼��:4>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==5) {								// <�\����߼��:5>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==6) {								// <�\����߼��:6>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==7) {								// <�\����߼��:7>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
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
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
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
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
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
					} else if(op[i]==8) {								// <�\����߼��:8>
						glColor3d(rc[5], gc[5], bc[5]);					// �\���F�ݒ�
						glLineWidth(4.0f);								// �����ݒ�
						if(mod==0||mod>3) {								// XY���ʗp
							p1 = pa[0], p2 = pa[i];
							p1.p[2] = 0.0, p2.p[2] = 0.0;
							rr = PntD(&p1, &p2);						// 2�_�ԋ���
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// �����ʒu�̊p�x
									d1 = pa[0].p[0]+rr*cos(th);			// �����ʒu
									d2 = pa[0].p[1]+rr*sin(th);
									glVertex3d(d1, d2, pa[i].p[2]);		// POLYLINE���_�o�^
								}
							glEnd();
						}
						if(mod==1||mod>3) {								// YZ���ʗp
							p1 = pa[0], p2 = pa[i];
							p1.p[0] = 0.0, p2.p[0] = 0.0;
							rr = PntD(&p1, &p2);						// 2�_�ԋ���
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// �����ʒu�̊p�x
									d1 = pa[0].p[1]+rr*cos(th);			// �����ʒu
									d2 = pa[0].p[2]+rr*sin(th);
									glVertex3d(pa[i].p[0], d1, d2);		// POLYLINE���_�o�^
								}
							glEnd();
						}
						if(mod==2||mod>3) {								// ZX���ʗp
							p1 = pa[0], p2 = pa[i];
							p1.p[1] = 0.0, p2.p[1] = 0.0;
							rr = PntD(&p1, &p2);						// 2�_�ԋ���
							glBegin(GL_LINE_LOOP);
								for(j=0; j<36; j++) {
									th = j*PI/18.0;						// �����ʒu�̊p�x
									d1 = pa[0].p[2]+rr*cos(th);			// �����ʒu
									d2 = pa[0].p[0]+rr*sin(th);
									glVertex3d(d2, pa[i].p[1], d1);		// POLYLINE���_�o�^
								}
							glEnd();
						}
						glLineWidth(1.0f);								// �����ݒ�
					} else if(op[i]==9) {								// <�\����߼��:9>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
							glEnd();
						}
					} else if(op[i]==10) {								// <�\����߼��:10>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
							glEnd();
						}
					} else if(op[i]==11) {								// <�\����߼��:11>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]-sz*5);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1], pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
							glEnd();
						}
					} else if(op[i]==12) {								// <�\����߼��:12>
						if(mod==0||mod>3) {
							glBegin(GL_LINE_LOOP);						// XY���ʗp
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]-sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz*5, pa[i].p[1], pa[i].p[2]);
								glVertex3d(pa[i].p[0]-sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]+sz, pa[i].p[2]);
								glVertex3d(pa[i].p[0]+sz, pa[i].p[1]-sz, pa[i].p[2]);
							glEnd();
						}
						if(mod==1||mod>3) {
							glBegin(GL_LINE_LOOP);						// YZ���ʗp
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]-sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz*5, pa[i].p[2]);
								glVertex3d(pa[i].p[0], pa[i].p[1]-sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]+sz*5);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]+sz);
								glVertex3d(pa[i].p[0], pa[i].p[1]+sz, pa[i].p[2]-sz);
							glEnd();
						}
						if(mod==2||mod>3) {
							glBegin(GL_LINE_LOOP);						// ZX���ʗp
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
					glColor3d(rc[ia[i]], gc[ia[i]], bc[ia[i]]);			// �\���F�ݒ�
					glBegin(GL_POINTS);
						glVertex3d(pa[i].p[0], pa[i].p[1], pa[i].p[2]);	// ���W�ݒ�
					glEnd();
				}
			}
		}
		glPointSize(1.0f);												// �ׯ���_���ޕW��
	glEndList();														// �ި���ڲ�ؽč쐬�I��
	Mode = mod, Disp = dsp;
	SetFlg = TRUE;														// �ׯ���ݒ��׸�ON
}

/*****************************/
void CTracker::Reset(int flg)											//<<< �ׯ���ؾ��
/*****************************/
{
	SetFlg = FALSE;														// �ׯ���ݒ��׸�OFF
	if(Lsno!=0) {
		glDeleteLists(Lsno, 1);											// �ި���ڲ�ؽč폜
	}
	PntSize = 5.0f;
	Lsno = 0, Mode = 0, Disp = 0;
	if(flg) {															// <�ŏIؾ��>
		XLimit1 = 1.0-DBL_MAX, XLimit2 = DBL_MAX;						// �͈͌����ؾ��
		YLimit1 = 1.0-DBL_MAX, YLimit2 = DBL_MAX;
		ZLimit1 = 1.0-DBL_MAX, ZLimit2 = DBL_MAX;
	}
}

/********************************************************************/
void CTracker::Limit(double x, double y, double z,
                     int x1, int x2, int y1, int y2, int z1, int z2)	//<<< �ׯ���͈͌���
/********************************************************************/
{
	XLimit1 = (x1) ? 1.0-DBL_MAX : x-0.0001;							// X �ŏ�������
	XLimit2 = (x2) ? DBL_MAX : x+0.0001;								// X �ő呤����
	YLimit1 = (y1) ? 1.0-DBL_MAX : y-0.0001;							// Y �ŏ�������
	YLimit2 = (y2) ? DBL_MAX : y+0.0001;								// Y �ő呤����
	ZLimit1 = (z1) ? 1.0-DBL_MAX : z-0.0001;							// Z �ŏ�������
	ZLimit2 = (z2) ? DBL_MAX : z+0.0001;								// Z �ő呤����
}

/******************************/
void CTracker::Draw(int plane)											//<<< �ׯ���`��
/******************************/
{
	if(Mode==plane||Mode==4||(Mode==5&&plane!=PRS)) {
		glCallList(Lsno);												// �ׯ���ި���ڲ�ؽČďo��
	}
}

/*****************************************************************/
int CTracker::HitTracker(int flg, int plane,
                         const PNTTYPE& pnt, PNTTYPE* pa, int pn)		//<<< �ׯ���I��(3�ʐ}�p)
/*****************************************************************/
{
	int    plsize, pln2, i, st, ed;
	double fovy, psize;

	plsize = View->GetPlSize();											// ���ʍ�(�߸�ْP��)�擾
	fovy = View->GetDispHeight();										// ���ʕ\����(mm�P��)�擾
	psize = fovy*PntSize/plsize;										// �_����3D�ϊ�
	pln2 = (plane+1)%3;
	st = (Disp==1) ? 1 : 0;												// �J�n�ʒu�ݒ�
	ed = (Disp==2) ? 0 : pn;											// �I���ʒu�ݒ�
	for(i=st; i<ed; i++) {
		if(flg||
           (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
            YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
            ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {					// <����͈͓�>
			if(pnt.p[plane]-psize<=pa[i].p[plane]&&
               pa[i].p[plane]<=pnt.p[plane]+psize&&
               pnt.p[pln2]-psize<=pa[i].p[pln2]&&
               pa[i].p[pln2]<=pnt.p[pln2]+psize) {
				return i;												// ���_�v�f�ԍ�RETURN
			}
		}
	}
	return -1;															// �Y�����_��
}

/*****************************************************************/
int CTracker::HitTrackerS(int flg, int plane, const PNTTYPE& pnt,
                          PNTTYPE* pa, int pn, int* ha)					//<<< �ׯ��(����)�I��(3�ʐ}�p)
/*****************************************************************/
{
	int    plsize, pln2, i, j, k, st, ed, cnt;
	double fovy, psize;

	plsize = View->GetPlSize();											// ���ʍ�(�߸�ْP��)�擾
	fovy = View->GetDispHeight();										// ���ʕ\����(mm�P��)�擾
	psize = fovy*PntSize/plsize;										// �_����3D�ϊ�
	pln2 = (plane+1)%3;
	st = (Disp==1) ? 1 : 0;												// �J�n�ʒu�ݒ�
	ed = (Disp==2) ? 0 : pn;											// �I���ʒu�ݒ�
	for(i=st, cnt=0; i<ed; i++) {
		if(flg||
           (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
            YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
            ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {					// <����͈͓�>
			if(pnt.p[plane]-psize<=pa[i].p[plane]&&
               pa[i].p[plane]<=pnt.p[plane]+psize&&
               pnt.p[pln2]-psize<=pa[i].p[pln2]&&
               pa[i].p[pln2]<=pnt.p[pln2]+psize) {
				ha[cnt++] = i;											// ���_�v�f�ԍ�
			}
		}
	}
	for(i=0; i<cnt; i++) {
		for(j=i+1; j<cnt; j++) {										// ���꒸�_�����
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
	return cnt;															// �Y�����_��
}

/********************************************************/
int CTracker::HitTracker3D(int flg, const CPoint& point,
                           PNTTYPE* pa, int pn)							//<<< �ׯ���I��(�����}�p)
/********************************************************/
{
	double*  depth = new double[pn];
	int*     hnum = new int[pn];
	GLdouble modelMat[16], projMat[16], x, y;
	GLint    viewport[4];
	double   min_depth, psize=PntSize/2.0+0.5;
	int      i, st, ed, cnt, hpnum=-1;

	glPushMatrix();
		glCallList(DL_PARS);											// �����}�p�ϊ��ďo
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);					// ���������ޭ��s��擾
		glGetDoublev(GL_PROJECTION_MATRIX, projMat);					// ���ݎˉe�s��擾
		View->GetViewport(PRS, &viewport[0], &viewport[1],
                          &viewport[2], &viewport[3]);
		st = (Disp==1) ? 1 : 0;											// �J�n�ʒu�ݒ�
		ed = (Disp==2) ? 0 : pn;										// �I���ʒu�ݒ�
		for(i=st, cnt=0; i<ed; i++) {									// �z��_���W->����޳���W
			if(flg||
               (XLimit1<pa[i].p[0]&&pa[i].p[0]<XLimit2&&
                YLimit1<pa[i].p[1]&&pa[i].p[1]<YLimit2&&
                ZLimit1<pa[i].p[2]&&pa[i].p[2]<ZLimit2)) {				// <����͈͓�>
				gluProject(pa[i].p[0], pa[i].p[1], pa[i].p[2], modelMat,
                           projMat, viewport, &x, &y, &depth[i]);
				y = View->ViewSize.cy - y;
				if(point.x<=x+psize&&x-psize<=point.x&&
                   point.y<=y+psize&&y-psize<=point.y) {
					hnum[cnt++] = i;									// �Y�����_�ۑ�
				}
			}
		}
		if(cnt>0) {
			hpnum = hnum[0];											// �����l�擾
			min_depth = depth[hpnum];									// ���s�擾
			for(i=1; i<cnt; i++) {
				if(min_depth>=depth[hnum[i]]) {							// ��O���_�̏ꍇ
					hpnum = hnum[i];									// �Y�����_�ԍ��擾
					min_depth = depth[hpnum];							// ���s�ۑ�
				}
			}
		}
	glPopMatrix();
	delete[] depth;
	delete[] hnum;
	return hpnum;														// �Y�����_�ԍ�
}
