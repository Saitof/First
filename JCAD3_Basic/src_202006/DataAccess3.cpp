/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암
// DataAccess3.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "Command.h"
#include "NumFunc.h"
#include "Texture.h"

/*********************************************************/
/*****             ڲ��ڰ�ݸފ��ϐ�����            *****/
/*********************************************************/
/********************************/
double ChangeFltToDbl(float num)										//<<< float => double
/********************************/
{
	double w1;
	int    w2;

	w1 = (num>=0.0) ? num*10000.0+0.000999 : num*10000.0-0.000999;
	w2 = (int)w1;
	return (double)w2/10000.0;
}

/******************************/
void GetObjBottom(double* btm)											//<<< �\���������ю擾
/******************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j;

	BaseObjPtr(1, &op, &i, &j);											// �\�����̎擾
	if(!op) {															// <�\�����̖�>
		btm[0] = 0.0, btm[1] = 0.0, btm[2] = 0.0;						// ���т͌��_
	} else {															// <�\�����̗L>
		GetAreaBox(1, &box);											// �S����BOX�擾
		BoxCenter(box, &pnt);											// BOX�����擾
		btm[0] = pnt.p[0], btm[1] = pnt.p[1], btm[2] = box.bp[0].p[2];	// ���шʒu(Z)�ݒ�
	}
}

/***********************************************************/
void SetRayTraceEnv(COLORREF bk, int max, int ant, int edg,
                    int stf, double* sts)								//<<< ڲ��ڰ����ݒ�
/***********************************************************/
{
	int i;

	BkRGB = bk;															// �w�i�F�ݒ�
	Max_Lvl = (short)max;												// �ő�T������
	Anti = (short)ant;													// �����ر�ݸ�
	Edge = (short)edg;													// �Ő��׸�
	StgFlg = (short)stf;												// �ð�ޗL���׸�
	for(i=0; i<4; i++) {
		StgSize[i] = sts[i];											// �ð�޻��ސݒ�
	}
}

/***************************************************************/
void GetRayTraceEnv(COLORREF* bk, int* max, int* ant, int* edg,
                    int* stf, double* sts)								//<<< ڲ��ڰ����擾
/***************************************************************/
{
	int i;

	*bk = BkRGB;														// �w�i�F�擾
	*max = (int)Max_Lvl;												// �ő�T������
	*ant = (int)Anti;													// �����ر�ݸ�
	*edg = (int)Edge;													// �Ő��׸�
	*stf = (int)StgFlg;													// �ð�ޗL���׸�
	for(i=0; i<4; i++) {
		sts[i] = StgSize[i];											// �ð�޻��ގ擾
	}
}

/******************************/
void InitRayTraceEnvFile(void)											//<<< ����ڲ��ڰ���̧�ٓǍ�
/******************************/
{
	FILE*   fp;
	CString path;
	int     i1, i2, i3, i4, er=0;
	float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+TEN_NAME;										// ڲ��ڰ�������̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// �w�i�F�ݒ�
				er = 1; break;
			}
			BkRGB = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d%d", &i1, &i2, &i3, &i4)!=4) {
				er = 1; break;
			}
			Max_Lvl = (short)i1;										// �ő�T������
			Anti = (short)i2;											// �����ر�ݸ�
			Edge = (short)i3;											// �Ő��׸�
			StgFlg = (short)i4;											// �ð�ޗL���׸�
			if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {		// �ð�޻��ސݒ�
				er = 1; break;
			}
			StgSize[0] = ChangeFltToDbl(f1);
			StgSize[1] = ChangeFltToDbl(f2);
			StgSize[2] = ChangeFltToDbl(f3);
			StgSize[3] = ChangeFltToDbl(f4);
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/****************************/
void GetObjCent(double* oct)											//<<< �\�����̾����擾
/****************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j;

	BaseObjPtr(1, &op, &i, &j);											// �\�����̎擾
	if(!op) {															// <�\�����̖�>
		oct[0] = 0.0, oct[1] = 0.0, oct[2] = 0.0;						// �����͌��_
	} else {															// <�\�����̗L>
		GetAreaBox(1, &box);											// �S����BOX�擾
		BoxCenter(box, &pnt);											// BOX�����擾
		oct[0] = pnt.p[0], oct[1] = pnt.p[1], oct[2] = pnt.p[2];
	}
}

/****************************************/
void SetRayTraceLight(int no, LIGHT lgt)								//<<< ڲ��ڰ������ݒ�
/****************************************/
{
	Lgt_Tbl[no] = lgt;													// �������ݒ�
}

/*****************************************/
void GetRayTraceLight(int no, LIGHT* lgt)								//<<< ڲ��ڰ������擾
/*****************************************/
{
	*lgt = Lgt_Tbl[no];													// �������擾
}

/********************************/
void InitRayTraceLightFile(void)										//<<< ����ڲ��ڰ�����̧�ٓǍ�
/********************************/
{
	FILE*   fp;
	LIGHT   lgt;
	CString path;
	int     i, i1, i2, i3, er=0;
	float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+LGT_NAME;										// ڲ��ڰ���������̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<LGMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {
				er = 1; break;
			}
			lgt.flg = i1;												// �L���׸�
			lgt.type = i2;												// ��������
			lgt.k = i3;													// ������������
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// ����RGB
				er = 1; break;
			}
			lgt.lc = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// �������W�ʒu
				er = 1; break;
			}
			lgt.x1 = ChangeFltToDbl(f1);
			lgt.y1 = ChangeFltToDbl(f2);
			lgt.z1 = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// ���������ʒu
				er = 1; break;
			}
			lgt.x2 = ChangeFltToDbl(f1);
			lgt.y2 = ChangeFltToDbl(f2);
			lgt.z2 = ChangeFltToDbl(f3);
			Lgt_Tbl[i] = lgt;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/******************************************/
void SetRayTraceColor(int no, ATTRIBT atr)								//<<< ڲ��ڰ��\�ʑ����ݒ�
/******************************************/
{
	Atr_Tbl[no] = atr;													// �\�ʑ����ݒ�
}

/*******************************************/
void GetRayTraceColor(int no, ATTRIBT* atr)								//<<< ڲ��ڰ��\�ʑ����擾
/*******************************************/
{
	*atr = Atr_Tbl[no];													// �\�ʑ����擾
}

/********************************/
void InitRayTraceColorFile(void)										//<<< ����ڲ��ڰ��װ̧�ٓǍ�
/********************************/
{
	FILE*   fp;
	ATTRIBT atr;
	CString path;
	int     i, i1, i2, i3, er=0;
	float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+TCL_NAME;										// ڲ��ڰ������װ̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<ATMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// ����
				er = 1; break;
			}
			atr.ka = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// �g�U���ˌ�
				er = 1; break;
			}
			atr.kd = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// ���ʔ��ˌW��
				er = 1; break;
			}
			atr.ks = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {				// ���ߒ����W��
				er = 1; break;
			}
			atr.ct = RGB(i1, i2, i3);
			if(fscanf_s(fp, "%d%f", &i1, &f1)!=2) {
				er = 1; break;
			}
			atr.trace = i1;												// �f�荞�ݗL��
			atr.n = ChangeFltToDbl(f1);									// �������ܗ�
			if(fscanf_s(fp, "%d%d%f", &i1, &i2, &f1)!=3) {
				er = 1; break;
			}
			atr.model = i1;												// ����(0:Phong, 1:Cook-T)
			atr.m1 = i2;												// ʲײĂ̍L����1(Phong)
			atr.m2 = ChangeFltToDbl(f1);								// ʲײĂ̍L����2(Cook-T)
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {				// �������ܗ�
				er = 1; break;
			}
			atr.nr = ChangeFltToDbl(f1);
			atr.ng = ChangeFltToDbl(f2);
			atr.nb = ChangeFltToDbl(f3);
			Atr_Tbl[i] = atr;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/***************************/
void ReadRayTraceFile(void)												//<<< ڲ��ڰ��ۑ�̧�ٓǍ�
/***************************/
{
	FILE*    fp;
	CString  path;
	LIGHT    lgt;
	ATTRIBT  atr;
	COLORREF bk;
	int      i, r, g, b, max, ant, edg, stf, er=0;
	float    f1, f2, f3, f4;
	double   sts[4];

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+RTS_NAME;										// ڲ��ڰ��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			bk = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d%d", &max, &ant, &edg, &stf)!=4) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {
				er = 1; break;
			}
			sts[0] = ChangeFltToDbl(f1);
			sts[1] = ChangeFltToDbl(f2);
			sts[2] = ChangeFltToDbl(f3);
			sts[3] = ChangeFltToDbl(f4);
			SetRayTraceEnv(bk, max, ant, edg, stf, sts);
			break;
		}
		for(i=0; i<LGMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &lgt.flg, &lgt.type, &lgt.k)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			lgt.lc = RGB(r, g, b);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			lgt.x1 = ChangeFltToDbl(f1);
			lgt.y1 = ChangeFltToDbl(f2);
			lgt.z1 = ChangeFltToDbl(f3);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			lgt.x2 = ChangeFltToDbl(f1);
			lgt.y2 = ChangeFltToDbl(f2);
			lgt.z2 = ChangeFltToDbl(f3);
			SetRayTraceLight(i, lgt);
		}
		for(i=0; i<ATMAX; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ka = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.kd = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ks = RGB(r, g, b);
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			atr.ct = RGB(r, g, b);
			if(fscanf_s(fp, "%d%f", &atr.trace, &f1)!=2) {
				er = 1; break;
			}
			atr.n = ChangeFltToDbl(f1);
			if(fscanf_s(fp, "%d%d%f", &atr.model, &atr.m1, &f1)!=3) {
				er = 1; break;
			}
			atr.m2 = ChangeFltToDbl(f1);
			if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
				er = 1; break;
			}
			atr.nr = ChangeFltToDbl(f1);
			atr.ng = ChangeFltToDbl(f2);
			atr.nb = ChangeFltToDbl(f3);
			SetRayTraceColor(i, atr);
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*****************************/
void WriteRayTraceFile(void)											//<<< ڲ��ڰ��ۑ�̧�ُ���
/****************************/
{
	FILE*    fp;
	CString  path;
	LIGHT    lgt;
	ATTRIBT  atr;
	COLORREF bk;
	int      i, r, g, b, max, ant, edg, stf, er=0;
	float    f1, f2, f3, f4;
	double   sts[4];

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+RTS_NAME;										// ڲ��ڰ��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		while (TRUE) {
			GetRayTraceEnv(&bk, &max, &ant, &edg, &stf, sts);
			r = GetRValue(bk), g = GetGValue(bk), b = GetBValue(bk);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d %d\n", max, ant, edg, stf)<0) {
				er = 1; break;
			}
			f1 = (float)sts[0], f2 = (float)sts[1];
			f3 = (float)sts[2], f4 = (float)sts[3];
			if(fprintf(fp, "%f %f %f %f\n", f1, f2, f3, f4)<0) {
				er = 1; break;
			}
			break;
		}
		for(i=0; i<LGMAX; i++) {
			GetRayTraceLight(i, &lgt);
			if(fprintf(fp, "%d %d %d\n", lgt.flg, lgt.type, lgt.k)<0) {
				er = 1; break;
			}
			r = GetRValue(lgt.lc), g = GetGValue(lgt.lc), b = GetBValue(lgt.lc);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			f1 = (float)lgt.x1, f2 = (float)lgt.y1, f3 = (float)lgt.z1;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
			f1 = (float)lgt.x2, f2 = (float)lgt.y2, f3 = (float)lgt.z2;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
		}
		for(i=0; i<ATMAX; i++) {
			GetRayTraceColor(i, &atr);
			r = GetRValue(atr.ka), g = GetGValue(atr.ka), b = GetBValue(atr.ka);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.kd), g = GetGValue(atr.kd), b = GetBValue(atr.kd);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.ks), g = GetGValue(atr.ks), b = GetBValue(atr.ks);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			r = GetRValue(atr.ct), g = GetGValue(atr.ct), b = GetBValue(atr.ct);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
			f1 = (float)atr.n;
			if(fprintf(fp, "%d %f\n", atr.trace, f1)<0) {
				er = 1; break;
			}
			f1 = (float)atr.m2;
			if(fprintf(fp, "%d %d %f\n", atr.model, atr.m1, f1)<0) {
				er = 1; break;
			}
			f1 = (float)atr.nr, f2 = (float)atr.ng, f3 = (float)atr.nb;
			if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
				er = 1; break;
			}
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/******************************************************/
/*****             POV-Ray���ϐ�����            *****/
/******************************************************/
/*************************************************************/
void SetPovData1(CString ifile, CString ipath, CString opath,
                 int smt, double agl,
                 int trs, double tsx, double tsy, double tsz,
                 int rot, double rox, double roy, double roz,
                 int scl, double scx, double scy, double scz,
                 int ani, int aniz, int aniy)							//<<< POV-Ray�ް��ݒ�1
/*************************************************************/
{
	PovInFile     = ifile;												// POV-Ray��`̧�ٖ�(���߽)
	PovInPath     = ipath;												// POV-Ray��`̧���߽
	PovOutPath    = opath;												// POV-Ray�o��̫����߽
	PovSmooth     = (BOOL)smt;											// �X���[�W���O�L��
	PovAngle      = agl;												// �אږ@���x�N�g���p�x��
	PovTranslate  = (BOOL)trs;											// Translate�L��
	PovTranslateX = tsx;												// Translate��(X)
	PovTranslateY = tsy;												// Translate��(Y)
	PovTranslateZ = tsz;												// Translate��(Z)
	PovRotate     = (BOOL)rot;											// Rotate�L��
	PovRotateX    = rox;												// Rotate��(X)
	PovRotateY    = roy;												// Rotate��(Y)
	PovRotateZ    = roz;												// Rotate��(Z)
	PovScale      = (BOOL)scl;											// Scale�L��
	PovScaleX     = scx;												// Scale��(X)
	PovScaleY     = scy;												// Scale��(Y)
	PovScaleZ     = scz;												// Scale��(Z)
	PovAnima      = (BOOL)ani;											// ��Ұ��ݗL��
	PovAnimaZ     = aniz;												// ��Ұ��ݕ�����(Z)
	PovAnimaY     = aniy;												// ��Ұ��ݕ�����(Y)
}

/*****************************************************************/
void GetPovData1(CString* ifile, CString* ipath, CString* opath,
                 int* smt, double* agl,
                 int* trs, double* tsx, double* tsy, double* tsz,
                 int* rot, double* rox, double* roy, double* roz,
                 int* scl, double* scx, double* scy, double* scz,
                 int* ani, int* aniz, int* aniy)						//<<< POV-Ray�ް��擾1
/*****************************************************************/
{
	*ifile = PovInFile;													// POV-Ray��`̧�ٖ�(���߽)
	*ipath = PovInPath;													// POV-Ray��`̧���߽
	*opath = PovOutPath;												// POV-Ray�o��̫����߽
	*smt = (int)PovSmooth;												// �X���[�W���O�L��
	*agl = PovAngle;													// �אږ@���x�N�g���p�x��
	*trs = (int)PovTranslate;											// Translate�L��
	*tsx = PovTranslateX;												// Translate��(X)
	*tsy = PovTranslateY;												// Translate��(Y)
	*tsz = PovTranslateZ;												// Translate��(Z)
	*rot = (int)PovRotate;												// Rotate�L��
	*rox = PovRotateX;													// Rotate��(X)
	*roy = PovRotateY;													// Rotate��(Y)
	*roz = PovRotateZ;													// Rotate��(Z)
	*scl = (int)PovScale;												// Scale�L��
	*scx = PovScaleX;													// Scale��(X)
	*scy = PovScaleY;													// Scale��(Y)
	*scz = PovScaleZ;													// Scale��(Z)
	*ani = (int)PovAnima;												// ��Ұ��ݗL��
	*aniz = PovAnimaZ;													// ��Ұ��ݕ�����(Z)
	*aniy = PovAnimaY;													// ��Ұ��ݕ�����(Y)
}

/************************************************************/
void SetPovData2(CString file1, CString file2, CString path,
                 int targ, int colc, int refl, int refr)				//<<< POV-Ray�ް��ݒ�2
/************************************************************/
{
	PovDefFile1   = file1;												// հ�ޒ�`̧�ٖ�1(���߽)
	PovDefFile2   = file2;												// հ�ޒ�`̧�ٖ�2(���߽)
	PovDefPath    = path;												// հ�ޒ�`̧���߽
	PovTarget     = targ;												// photons target �L/��
	PovCollect    = colc;												// photons collect on/off
	PovReflection = refl;												// photons reflection on/off
	PovRefraction = refr;												// photons refraction on/off
}

/***************************************************************/
void GetPovData2(CString* file1, CString* file2, CString* path,
                 int* targ, int* colc, int* refl, int* refr)			//<<< POV-Ray�ް��擾2
/***************************************************************/
{
	*file1 = PovDefFile1;												// հ�ޒ�`̧�ٖ�1(���߽)
	*file2 = PovDefFile2;												// հ�ޒ�`̧�ٖ�2(���߽)
	*path  = PovDefPath;												// հ�ޒ�`̧���߽
	*targ  = PovTarget;													// photons target �L/��
	*colc  = PovCollect;												// photons collect on/off
	*refl  = PovReflection;												// photons reflection on/off
	*refr  = PovRefraction;												// photons refraction on/off
	PovDefFile1W = PovDefFile1;											// հ�ޒ�`̧�ٖ�WK1(���߽)
	PovDefFile2W = PovDefFile2;											// հ�ޒ�`̧�ٖ�WK2(���߽)
}

/*************************************************/
void SetPovData3(int* shp, int* tex1, int* tex2,
                 int* tex3, int* tex4, int* tex5)						//<<< POV-Ray�ް��ݒ�3
/*************************************************/
{
	int i;

	for(i=0; i<POVCMAX; i++) {
		PovShape[i] = shp[i];											// ��޼ު�Č`��
		PovTex1[i] = tex1[i];											// ø���No
		PovTex2[i] = tex2[i];
		PovTex3[i] = tex3[i];
		PovTex4[i] = tex4[i];
		PovTex5[i] = tex5[i];
	}
}

/*************************************************/
void GetPovData3(int* shp, int* tex1, int* tex2,
                 int* tex3, int* tex4, int* tex5)						//<<< POV-Ray�ް��擾3
/*************************************************/
{
	int i;

	for(i=0; i<POVCMAX; i++) {
		shp[i] = PovShape[i];											// ��޼ު�Č`��
		tex1[i] = PovTex1[i];											// ø���No
		tex2[i] = PovTex2[i];
		tex3[i] = PovTex3[i];
		tex4[i] = PovTex4[i];
		tex5[i] = PovTex5[i];
	}
}

/*******************************/
void SetWDefFile1(CString file)											//<<< POV-Rayհ�ޒ�`WĶ�ِݒ�1
/*******************************/
{
	PovDefFile1W = file;												// հ�ޒ�`̧�ٖ�WK1(���߽)
}

/*******************************/
void SetWDefFile2(CString file)											//<<< POV-Rayհ�ޒ�`WĶ�ِݒ�2
/*******************************/
{
	PovDefFile2W = file;												// հ�ޒ�`̧�ٖ�WK2(���߽)
}

/************************************************/
void GetWDefFile(CString* file1, CString* file2)						//<<< POV-Rayհ�ޒ�`WĶ�َ擾
/************************************************/
{
	*file1 = PovDefFile1W;												// հ�ޒ�`̧�ٖ�WK1(���߽)
	*file2 = PovDefFile2W;												// հ�ޒ�`̧�ٖ�WK2(���߽)
}

/**********************************************************/
void ReadPovDefFile(CString path, int* cnt, CString* data)				//<<< POV-Rayհ�ޒ�`̧�ٓǍ�
/**********************************************************/
{
	char* cdmy = new char[256];
	FILE* fp;

	*cnt = 0;
	if(fopen_s(&fp, path, "r")==0) {									// ̧��Open
		while(TRUE) {													// EOF�܂ŌJ��
			fscanf_s(fp, "%s", cdmy, 256);
			if(feof(fp)) {
				break;
			}
			if(!strcmp(cdmy, "#declare")) {								// declare�錾��
				fscanf_s(fp, "%s", cdmy, 256);
				if(feof(fp)) {
					break;
				}
				data[*cnt] = CString(cdmy);								// �錾����z��ɕۑ�
				(*cnt)++;
			}
		}
		fclose(fp);														// ̧��Close
	}
	delete[] cdmy;
}

/*************************/
void ReadPovRayFile(void)												//<<< POV-Ray�ۑ�̧�ٓǍ�
/*************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+POV_NAME;										// POV-Ray�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%f", &PovSmooth, &fd1)!=2) {
				er = 1; break;
			}
			PovAngle = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d%d", &PovTranslate,
                                      &PovRotate,
                                      &PovScale)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovTranslateX = ChangeFltToDbl(fd1);
			PovTranslateY = ChangeFltToDbl(fd2);
			PovTranslateZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovRotateX = ChangeFltToDbl(fd1);
			PovRotateY = ChangeFltToDbl(fd2);
			PovRotateZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			PovScaleX = ChangeFltToDbl(fd1);
			PovScaleY = ChangeFltToDbl(fd2);
			PovScaleZ = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &PovAnima,
                                      &PovAnimaZ,
                                      &PovAnimaY)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &PovTarget,
                                    &PovCollect)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &PovReflection,
                                    &PovRefraction)!=2) {
				er = 1; break;
			}
			for(i=0; i<POVCMAX; i++) {
				if(fscanf_s(fp, "%d%d%d", &PovShape[i],
                                          &PovTex1[i],
                                          &PovTex2[i])!=3) {
					er = 1; break;
				}
				if(fscanf_s(fp, "%d%d%d", &PovTex3[i],
                                          &PovTex4[i],
                                          &PovTex5[i])!=3) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovInFile.Format("%s", lpath);
				if(PovInFile=="") {
					PovInFile = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovInPath.Format("%s", lpath);
				if(PovInPath=="") {
					PovInPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovOutPath.Format("%s", lpath);
				if(PovOutPath=="") {
					PovOutPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovDefFile1.Format("%s", lpath);
				if(PovDefFile1=="") {
					PovDefFile1 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovDefFile2.Format("%s", lpath);
				if(PovDefFile2=="") {
					PovDefFile2 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				PovDefPath.Format("%s", lpath);
				if(PovDefPath=="") {
					PovDefPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		er = 1;
	}
	if(er) {															// �װ�������͏����l
		PovSmooth     = 0,   PovAngle      = 45.0;
		PovTranslate  = 0,   PovRotate     = 0,   PovScale      = 0;
		PovTranslateX = 0.0, PovTranslateY = 0.0, PovTranslateZ = 0.0;
		PovRotateX    = 0.0, PovRotateY    = 0.0, PovRotateZ    = 0.0;
		PovScaleX     = 0.0, PovScaleY     = 0.0, PovScaleZ     = 0.0;
		PovAnima      = 0,   PovAnimaZ     = 15,  PovAnimaY     = 6;
		PovTarget     = 1,   PovCollect    = 0;
		PovReflection = 1,   PovRefraction = 1;
		PovShape[0]   = 0,   PovTex1[0]    = 0,   PovTex2[0]    = 0;	// �ް���
		PovShape[1]   = 0,   PovTex1[1]    = 0,   PovTex2[1]    = 2;	// ���ް
		PovShape[2]   = 1,   PovTex1[2]    = 1,   PovTex2[2]    = 0;	// �ؽ��
		PovShape[3]   = 1,   PovTex1[3]    = 1,   PovTex2[3]    = 2;	// ��ް
		PovShape[4]   = 1,   PovTex1[4]    = 1,   PovTex2[4]    = 1;	// ������
		PovShape[5]   = 1,   PovTex1[5]    = 1,   PovTex2[5]    = 3;	// �̧��
		PovShape[6]   = 1,   PovTex1[6]    = 1,   PovTex2[6]    = 4;	// հ�ް1
		PovShape[7]   = 1,   PovTex1[7]    = 1,   PovTex2[7]    = 5;	// հ�ް2
		PovShape[8]   = 1,   PovTex1[8]    = 1,   PovTex2[8]    = 6;	// հ�ް3
		PovShape[9]   = 1,   PovTex1[9]    = 1,   PovTex2[9]    = 7;	// հ�ް4
		PovShape[10]  = 1,   PovTex1[10]   = 1,   PovTex2[10]   = 8;	// հ�ް5
		PovShape[11]  = 1,   PovTex1[11]   = 1,   PovTex2[11]   = 9;	// հ�ް6
		PovShape[12]  = 1,   PovTex1[12]   = 1,   PovTex2[12]   = 10;	// հ�ް7
		PovShape[13]  = 1,   PovTex1[13]   = 1,   PovTex2[13]   = 11;	// հ�ް8
		PovShape[14]  = 1,   PovTex1[14]   = 1,   PovTex2[14]   = 12;	// հ�ް9
		PovShape[15]  = 1,   PovTex1[15]   = 1,   PovTex2[15]   = 13;	// հ�ް10
		PovShape[16]  = 1,   PovTex1[16]   = 1,   PovTex2[16]   = 14;	// հ�ް11
		PovShape[17]  = 1,   PovTex1[17]   = 1,   PovTex2[17]   = 15;	// հ�ް12
		for(i=0; i<POVCMAX; i++) {
			PovTex3[i] = 0, PovTex4[i] = 0, PovTex5[i] = 0;
		}
		PovInFile   = GetAppPath(0)+POV_DEF1;
		PovInPath   = GetAppPath(0);
		PovOutPath  = GetAppPath(0);
		PovDefFile1 = GetAppPath(0)+POV_DEF2;
		PovDefFile2 = GetAppPath(0)+POV_DEF3;
		PovDefPath  = GetAppPath(0);
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
	delete[] lpath;
}

/**************************/
void WritePovRayFile(void)												//<<< POV-Ray�ۑ�̧�ُ���
/**************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+POV_NAME;										// POV-Ray�ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		while (TRUE) {
			fd1 = (float)PovAngle;
			if(fprintf(fp, "%d %f\n", PovSmooth, fd1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", PovTranslate,
                                         PovRotate,
                                         PovScale)<0) {
				er = 1; break;
			}
			fd1 = (float)PovTranslateX;
			fd2 = (float)PovTranslateY;
			fd3 = (float)PovTranslateZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)PovRotateX;
			fd2 = (float)PovRotateY;
			fd3 = (float)PovRotateZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)PovScaleX;
			fd2 = (float)PovScaleY;
			fd3 = (float)PovScaleZ;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", PovAnima,
                                         PovAnimaZ,
                                         PovAnimaY)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", PovTarget,
                                      PovCollect)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", PovReflection,
                                      PovRefraction)<0) {
				er = 1; break;
			}
			for(i=0; i<POVCMAX; i++) {
				if(fprintf(fp, "%d %d %d\n", PovShape[i],
                                             PovTex1[i],
                                             PovTex2[i])<0) {
					er = 1; break;
				}
				if(fprintf(fp, "%d %d %d\n", PovTex3[i],
                                             PovTex4[i],
                                             PovTex5[i])<0) {
					er = 1; break;
				}
			}
			if(er) {
				break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovInFile, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovInPath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovOutPath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefFile1, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefFile2, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(PovDefPath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
}

/**********************************************************/
static char GetTexFlg1(CString def1, CString def2, int no)				//<<< ø����׸�1�擾
/**********************************************************/
{
	int i, cnt=0, pos;

	if(def1!="") {														// <հ�ޒ�`1����>
		if(no==cnt) {
			return '1';
		}
		cnt++;
	}
	if(def2!="") {														// <հ�ޒ�`2����>
		if(no==cnt) {
			return '2';
		}
		cnt++;
	}
	for(i=0; i<MATERIAL_CNT; i++) {										// <ø����ގ�>
		if(no==cnt+i) {
			pos = T_Material[i].Find("|");								// ��ؕ���(|)
			return T_Material[i].GetAt(pos+1);
		}
	}
	return '0';
}

/***************************************************************/
static void GetTexture2(CString* name1, CString* name2, int no,
                        char flg1, CString* tex2, char* flg2)			//<<< ø���2�擾
/***************************************************************/
{
	int pos;

	if(flg1=='A') {														// <��>
		pos = T_Gold[no].Find("|");										// ��ؕ���(|)
		*tex2 = T_Gold[no].Left(pos);
		*flg2 = T_Gold[no].GetAt(pos+1);
	} else if(flg1=='B') {												// <��>
		pos = T_Silver[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Silver[no].Left(pos);
		*flg2 = T_Silver[no].GetAt(pos+1);
	} else if(flg1=='C') {												// <��>
		pos = T_Copper[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Copper[no].Left(pos);
		*flg2 = T_Copper[no].GetAt(pos+1);
	} else if(flg1=='D') {												// <�^�J>
		pos = T_Brass[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Brass[no].Left(pos);
		*flg2 = T_Brass[no].GetAt(pos+1);
	} else if(flg1=='E') {												// <���>
		pos = T_Chrome[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Chrome[no].Left(pos);
		*flg2 = T_Chrome[no].GetAt(pos+1);
	} else if(flg1=='F') {												// <����>
		pos = T_Metal[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Metal[no].Left(pos);
		*flg2 = T_Metal[no].GetAt(pos+1);
	} else if(flg1=='G') {												// <��׽1>
		pos = T_Glass1[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Glass1[no].Left(pos);
		*flg2 = T_Glass1[no].GetAt(pos+1);
	} else if(flg1=='J') {												// <��>
		pos = T_Wood[no].Find("|");										// ��ؕ���(|)
		*tex2 = T_Wood[no].Left(pos);
		*flg2 = T_Wood[no].GetAt(pos+1);
	} else if(flg1=='K') {												// <��>
		pos = T_Stone[no].Find("|");									// ��ؕ���(|)
		*tex2 = T_Stone[no].Left(pos);
		*flg2 = T_Stone[no].GetAt(pos+1);
	} else if(flg1=='L') {												// <��>
		pos = T_Sky[no].Find("|");										// ��ؕ���(|)
		*tex2 = T_Sky[no].Left(pos);
		*flg2 = T_Sky[no].GetAt(pos+1);
	} else if(flg1=='1') {												// <հ�ޒ�`1>
		*tex2 = name1[no];
		*flg2 = 'D';
	} else {															// <հ�ޒ�`2>
		*tex2 = name2[no];
		*flg2 = 'E';
	}
}

/**********************************/
static CString GetTexture3(int no)										//<<< ø���3�擾
/**********************************/
{
	return G_Out_Prpt[no];
}

/*********************************************/
static CString GetTexture4(int no, char flg1)							//<<< ø���4�擾
/*********************************************/
{
	if(flg1=='I') {														// <��׽3>
		return G_In_Prpt2[no];
	} else {															// <��׽2>
		return G_In_Prpt1[no];
	}
}

/*********************************************/
static CString GetTexture5(int no, char flg1)							//<<< ø���5�擾
/*********************************************/
{
	if(flg1=='I') {														// <��׽3>
		return G_Color2[no];
	} else {															// <��׽2>
		return G_Color1[no];
	}
}

/******************************************************************/
void GetColorTex(int* it1, int* it2, int* it3, int* it4, int* it5,
                 char* ft1, char* ft2, CString* st2,
                 CString* st3, CString* st4, CString* st5)				// �װ��ø����擾
/******************************************************************/
{
	CString	def1, def2, path, dname1[128], dname2[128];
	int     i, targ, colc, refl, refr, dcnt1=0, dcnt2=0;

	GetPovData2(&def1, &def2, &path, &targ, &colc, &refl, &refr);		// POV-Ray�ް��擾2
	if(def1!="") {
		ReadPovDefFile(def1, &dcnt1, dname1);							// POV-Rayհ�ޒ�`̧�ٓǍ�
		if(dcnt1==0) {
			def1 = _T("");
		}
	}
	if(def2!="") {
		ReadPovDefFile(def2, &dcnt2, dname2);							// POV-Rayհ�ޒ�`̧�ٓǍ�
		if(dcnt2==0) {
			def2 = _T("");
		}
	}
	for(i=0; i<POVCMAX; i++) {											// <�װ����>
		ft1[i] = GetTexFlg1(def1, def2, it1[i]);						// ø����׸�1�擾
		GetTexture2(dname1, dname2, it2[i], ft1[i], &st2[i], &ft2[i]);	// ø���2�擾
		st3[i] = GetTexture3(it3[i]);									// ø���3�擾
		st4[i] = GetTexture4(it4[i], ft1[i]);							// ø���4�擾
		st5[i] = GetTexture5(it5[i], ft1[i]);							// ø���5�擾
	}
}

/********************/
void InitCNVtx(void)													//<<< ���_��ď�������
/********************/
{
	NVp = NULL;
	Now_no = 0;
}

/*************************************/
void SetCNVtx(PNTTYPE pt, VECTYPE vc)									//<<< ��Ē��_�ݒ�
/*************************************/
{
	NVTYPE* tv;

	tv = (NVTYPE *)malloc(sizeof(NVTYPE));								// ��ė̈�m��
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->next = NVp;
	tv->no   = Now_no;
	tv->pt   = pt;
	tv->vc   = vc;
	tv->fg1  = 0;
	tv->fg2  = 0;
	NVp      = tv;
	Now_no++;
}

/***********************************************/
static NVTYPE* MergeNVx(NVTYPE* la, NVTYPE* lb)							//<<< ���_���W:X���
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[0]<=lb->pt.p[0]) {									// ���_���W:X�召�Ŕ�r
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtx(NVTYPE* mlist, int n)						//<<< ���_���W:X���
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVx(NV_Mergsrtx(la, n/2),
                                NV_Mergsrtx(lb, n-n/2));				// ���_���W:X���
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVy(NVTYPE* la, NVTYPE* lb)							//<<< ���_���W:Y���
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[1]<=lb->pt.p[1]) {									// ���_���W:Y�召�Ŕ�r
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrty(NVTYPE* mlist, int n)						//<<< ���_���W:Y���
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVy(NV_Mergsrty(la, n/2),
                                NV_Mergsrty(lb, n-n/2));				// ���_���W:Y���
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVz(NVTYPE* la, NVTYPE* lb)							//<<< ���_���W:Z���
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[2]<=lb->pt.p[2]) {									// ���_���W:Z�召�Ŕ�r
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtz(NVTYPE* mlist, int n)						//<<< ���_���W:Z���
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVz(NV_Mergsrtz(la, n/2),
                                NV_Mergsrtz(lb, n-n/2));				// ���_���W:Z���
		rc = lm;
	}
	return rc;
}

/***********************************************/
static NVTYPE* MergeNVn(NVTYPE* la, NVTYPE* lb)							//<<< ��ٰ�ߔԍ����
/***********************************************/
{
	NVTYPE* lc, * ld, * rc;

	lc = (NVTYPE *)malloc(sizeof(NVTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// ��ٰ�ߔԍ��召�Ŕ�r
			lc->next = la;
			lc = la;
			la = la->next;
		} else {
			lc->next = lb;
			lc = lb;
			lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	if(la==NULL) {
		lc->next = lb;
	} else {
		lc->next = la;
	}
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/************************************************/
static NVTYPE* NV_Mergsrtn(NVTYPE* mlist, int n)						//<<< ��ٰ�ߔԍ����
/************************************************/
{
	NVTYPE* la, * lb, * lm, * rc;
	int     i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (NVTYPE *)MergeNVn(NV_Mergsrtn(la, n/2),
                                NV_Mergsrtn(lb, n-n/2));				// ��ٰ�ߔԍ����
		rc = lm;
	}
	return rc;
}

/*****************************************************/
static void CorrectionNVec1(NVTYPE* svp, NVTYPE* evp)					//<<< �@���޸�ٕ␳1
/*****************************************************/					//   (����@���޸�ٌ���)
{
	NVTYPE* wvp, * cvp;
	VECTYPE vec;

	wvp = svp;
	while(wvp!=NULL) {													// <���꒸�_(svp�`evp)>
		if(wvp==evp) {													// �ŏI���_�������ΏۊO
			break;
		}
		if(wvp->fg1==0) {												// <�������̂ݑΏ�>
			wvp->fg1 = 1;												// �P�Ɩ�/��\��
			vec = wvp->vc;
			cvp = wvp->next;											// �����_�ȍ~������
			while(cvp!=NULL) {
				if(cvp->fg1==0) {										// <<���������ް�>>
					if(fabs(CalcVecAngle(&wvp->vc, &cvp->vc))<PI/360) {	// �@���޸�يp�x��0.5�ȓ���
						cvp->fg1 = 2;									// ���ꕽ�ʂƂ��A�t����
					}
				}
				if(cvp==evp) {											// �ŏI���_�ŏI��
					break;
				}
				cvp = cvp->next;
			}
		}
		wvp = wvp->next;												// �����_��
	}
}

/*****************************************************/
static void CorrectionNVec2(NVTYPE* svp, NVTYPE* evp,
                            int smooth, double angle)					//<<< �@���޸�ٕ␳2
/*****************************************************/					//   (�@���޸�ٕ��ω�)
{
	NVTYPE* wvp, * cvp;
	VECTYPE vec;
	int     i;

	wvp = svp;
	while(wvp!=NULL) {													// <���꒸�_(svp�`evp)>
		if(wvp==evp) {													// �ŏI���_�������ΏۊO
			break;
		}
		if(wvp->fg2==0) {												// <�������̂ݑΏ�>
			wvp->fg2 = 1;												// �␳��
			vec = wvp->vc;
			cvp = wvp->next;											// �����_�ȍ~������
			while(cvp!=NULL) {
				if(cvp->fg2==0) {										// <<���������ް�>>
					if(!smooth||fabs(CalcVecAngle(&wvp->vc, &cvp->vc))
                                                      <PI*angle/180) {	// <�Ѱ��ݸޗL/�w��p�x����>
						if(cvp->fg1<2) {								// �P�Ɩ�/��\�ʂ̏ꍇ
							for(i=0; i<DIM; i++) {
								vec.p[i] += cvp->vc.p[i];				// �Ώے��_�޸�ى��Z
							}
						}
						cvp->fg2 = 1;									// �␳��
					}
				}
				if(cvp==evp) {											// �ŏI���_�ŏI��
					break;
				}
				cvp = cvp->next;
			}
			VecN(&vec);													// �P���޸�ى�
			cvp = wvp;
			while(cvp!=NULL) {
				if(cvp->fg2==1) {										// <<���������ް�>>
					cvp->fg2 = 2;										// ������
					cvp->vc = vec;										// �␳�ϖ@���޸��
				}
				if(cvp==evp) {											// �ŏI���_�ŏI��
					break;
				}
				cvp = cvp->next;
			}
		}
		wvp = wvp->next;												// �����_��
	}
}

/****************************************/
BOOL SortCNVtx(int smooth, double angle)								//<<< ���_����&�ݒ�
/****************************************/
{
	NVTYPE* wvp, * svp=NULL, * cvp;
	PNTTYPE pt;

	if(Now_no==0) {
		return FALSE;
	}
	NVp = NV_Mergsrtz(NVp, Now_no);										// ���_���W:Z���
	NVp = NV_Mergsrty(NVp, Now_no);										// ���_���W:Y���
	NVp = NV_Mergsrtx(NVp, Now_no);										// ���_���W:X���
	pt = NVp->pt;
	pt.p[0] = pt.p[0] - 1.0f;											// �����l�ݒ�
	wvp = NVp;
	while(wvp!=NULL) {													// �ݒ�ςƈقȂ�ꍇ
		if(pt.p[0]!=wvp->pt.p[0]||
           pt.p[1]!=wvp->pt.p[1]||
           pt.p[2]!=wvp->pt.p[2]) {										// <���_���W���قȂ�>
			if(svp!=NULL) {
				CorrectionNVec1(svp, cvp);								// �@���޸�ٕ␳1
				CorrectionNVec2(svp, cvp, smooth, angle);				// �@���޸�ٕ␳2
			}
			svp = wvp;													// �擪�߲���ۑ�
			pt = wvp->pt;
		}
		cvp = wvp;														// ���O���߲���ۑ�
		wvp = wvp->next;
	}
	if(svp!=NULL) {
		CorrectionNVec1(svp, cvp);										// �@���޸�ٕ␳1
		CorrectionNVec2(svp, cvp, smooth, angle);						// �@���޸�ٕ␳2
	}
	NVp = NV_Mergsrtn(NVp, Now_no);										// ��ٰ�ߔԍ����
	return TRUE;
}

/*******************/
void EndCNVtx(void)														//<<< ���_��ďI������
/*******************/
{
	NVTYPE* wvp, * cvp;

	wvp = NVp;
	while(wvp) {														// ��ė̈���
		cvp = wvp->next;
		free(wvp);
		wvp  = cvp;
	}
}

/*********************************************************/
/*****            ���̥����ލ�Ɨp�ϐ�����           *****/
/*********************************************************/
/************************/
void CreateObjWork(void)												//<<< ���̍���ް��̈�m��
/************************/
{
	OWKTYPE* optr;

	Rf_owk = (OWKTYPE *)malloc(sizeof(OWKTYPE));						// �V�K�̈�m��
	if(Rf_owk==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Rf_owk->next = NULL;												// ���ް����߲��
	Rf_owk->op = NULL;													// �����߲��
	Rf_owk->cwk = NULL;													// ����ލ���ް��߲��
	if(Base_owk==NULL) {
		Base_owk = Rf_owk;
	} else {
		optr = Base_owk;
		while(optr->next!=NULL) {
			optr = optr->next;											// �ŏI���̌���
		}
		optr->next = Rf_owk;											// �ŏI����ؽĐڑ�
	}
}

/******************************************/
void CreateCmdWork(int clvl, CMDTYPE* cmp)								//<<< ����ލ���ް��̈�m��
/******************************************/
{
	CWKTYPE* cwkp, * cptr;

	cwkp = (CWKTYPE *)malloc(sizeof(CWKTYPE));							// �V�K�̈�m��
	if(cwkp==NULL) {
		MemErr(IDS_MEMERR5);
	}
	cwkp->next = NULL;													// ���ް����߲��
	cwkp->owk = Rf_owk;													// ���̍���ް��߲��
	cwkp->cmp = cmp;													// ������߲��
	cwkp->clvl = clvl;													// ���������No
	cwkp->flg = 0;														// �����׸�(0:��)
	if(Rf_owk->cwk==NULL) {
		Rf_owk->cwk = cwkp;
	} else {
		cptr = Rf_owk->cwk;
		while(cptr->next!=NULL) {
			cptr = cptr->next;											// �ŏI����ތ���
		}
		cptr->next = cwkp;												// �ŏI�����ؽĐڑ�
	}
}

/***********************************************************************/
void SearchCmdWork(int lvl, CWKTYPE** cwk, CMDTYPE** cmp, OBJTYPE** op)	//<<< ����ލ���ް�����
/***********************************************************************/
{
	OWKTYPE* optr;
	CWKTYPE* cptr;

	*cwk = NULL, *cmp = NULL, *op = NULL;								// �߲��������
	optr = Base_owk;													// �J�n���̍���߲��
	while(optr!=NULL) {
		cptr = optr->cwk;												// ���̕ʊJ�n����ލ���߲��
		while(cptr!=NULL) {
			if(cptr->flg==0&&cptr->clvl==lvl) {							// <�������̓�����������No>
				cptr->flg = 1;											// �����ς�
				*cwk = cptr;											// �Ώۺ���ލ���߲��
				*cmp = cptr->cmp;										// �Ώۺ�����߲��
				*op = (cptr->owk)->op;									// �Ώۗ����߲��
				return;
			}
			cptr = cptr->next;											// ������ލ���߲��
		}
		optr = optr->next;												// �����̍���߲��
	}
}

/********************************************/
void SetOwkObjPtr(CWKTYPE* cwk, OBJTYPE* op)							//<<< ���̍���ް������߲���ݒ�
/********************************************/
{
	(cwk->owk)->op = op;												// �����߲���ݒ�
}

/*******************************/
void ChangeCwkFlg(CWKTYPE* cwk)											//<<< ����ލ���ް��׸ލX�V
/*******************************/
{
	cwk->flg = 2;														// ����ގ��s�ς�
}

/**************************/
void LastObjWorkProc(void)												//<<< ���̍���ް��ŏI����
/**************************/
{
	OWKTYPE* optr;
	OIFTYPE* oif;
	int      oct=0;

	optr = Base_owk;													// �J�n���̍���߲��
	while(optr!=NULL) {
		if(optr->op!=NULL) {											// <�����߲���o�^�ς�>
			RedoCommand(optr->op);										// ����ލĎ��s
			GetObjOif(optr->op, &oif);									// ���̍\������߲���擾
			SetOifNo(oif, oct++);										// ���̍\������No�ݒ�
			View->SetDispList(optr->op);								// �ި���ڲ�ؽēo�^
		}
		optr = optr->next;												// �����̍���߲��
	}
}

/****************************/
void FreeObjCmdWork(int flg)											//<<< ����/����ލ���ް����
/****************************/
{
	OWKTYPE* optr, * dop;
	CWKTYPE* cptr, * dcp;

	optr = Base_owk;													// �J�n���̍���߲��
	while(optr!=NULL) {
		cptr = optr->cwk;												// ���̕ʊJ�n����ލ���߲��
		while(cptr!=NULL) {
			if(flg==1) {												// <����ލ폜�w���L>
				FreeCmd(NULL, cptr->cmp, 0);							// ����ލ폜
			}
			dcp = cptr, cptr = cptr->next;								// ������ލ���߲��
			free(dcp);													// ����ލ�Ɨ̈���
		}
		dop = optr, optr = optr->next;									// �����̍���߲��
		free(dop);														// ���̍�Ɨ̈���
	}
	Base_owk = NULL, Rf_owk = NULL;										// ������
}

/*********************************************************/
/*****            �Ȗʍו�����Ɨp�ϐ�����           *****/
/*********************************************************/
/****************************/
void OnCurveReptSW(int mode)											//<<< �Ȗʍו���SW�I��
/****************************/
{
	Curve_Rept_SW = 1;													// �Ȗʍו�����Ƃ���
	if(mode==1) {
		Curve_Rept_Num = 3;												// �Ȑ�������:3
	} else if(mode==2) {
		Curve_Rept_Num = 4;												// �Ȑ�������:4
	} else if(mode==3) {
		Curve_Rept_Num = 5;												// �Ȑ�������:5
	} else {
		Curve_Rept_Num = 2;												// �Ȑ�������:2
	}
}

/*************************/
void OffCurveReptSW(void)												//<<< �Ȗʍו���SW�I�t
/*************************/
{
	Curve_Rept_SW = 0;													// �Ȗʍו�����ƂȂ�
	Curve_Rept_Num = 3;		//202006 2->3					�@			// �Ȑ�������:4
}

/*************************/
BOOL GetCurveReptSW(void)												//<<< �Ȗʍו���SW�擾
/*************************/
{
	return (BOOL)Curve_Rept_SW;											// �Ȗʍו���SW
}

/*************************/
int GetCurveReptNum(void)												//<<< �Ȗʕ������擾
/*************************/
{
	return (int)Curve_Rept_Num;											// �Ȑ�������
}
