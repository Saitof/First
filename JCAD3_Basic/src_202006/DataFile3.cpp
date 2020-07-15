/////////////////////////////////////////////////////////////////////////////
// ̧�ٓ��o�͕�3
// DataFile3.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "Command.h"
#include "CCommand.h"
#include "RCommand.h"
#include "NumFunc.h"
#include "CFAC32_Api.h"
#include "SLCA32_Api.h"

#define	MAXFVTX	1000													// �ő�ʒ��_��
#define	MAXPART	10000													// �ő��߰�

typedef	struct {
	int    vno;															// 3������̒��_�ԍ�
	double x;															// 2�������X���W
	double y;															//    �V    Y���W
} VERTEX;

CProgressDlg* m_pProgressF3=NULL;										// ��۸�ڽ�ݼ޹���
int	FacNum;																// �ʐ�
int	ObjNum;																// ���̐�
int PolyFlg;															// ��غ���׸�(0:̧�َ��,1:��غ�ݍ쐬)

/***********************************************************************/
static void ReadCommonFileParam(int* ver, int* out, int* typ, int* fac)	//<<< ����̧�����Ұ��ۑ�̧�ٓǍ�
/***********************************************************************/
{
	FILE*    fp;
	CString  path;
	int      er=0;

	path = GetAppPath(0)+CFAC_NAME;										// CFAC32�p�ް��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		if(fscanf_s(fp, "%d%d%d%d", ver, out, typ, fac)!=4) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		er = 1;
	}
	if(er) {
		*ver = 0, *out = 0, *typ = 0, *fac = 0;
	}
}

/********************************************************************/
static void WriteCommonFileParam(int ver, int out, int typ, int fac)	//<<< ����̧�����Ұ��ۑ�̧�ُ���
/********************************************************************/
{
	FILE*    fp;
	CString  path;
	int      er=0;

	path = GetAppPath(0)+CFAC_NAME;										// CFAC32�p�ް��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		if(fprintf(fp, "%d %d %d %d\n", ver, out, typ, fac)<0) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*********************************************************************/
static void PutTriangle(FILE* fp, OBJTYPE* op, VECTYPE v, VERTEX* pa)	//<<< �O�p�`�߯��o��
/*********************************************************************/
{
	PNTTYPE pt;
	int     i, flg;

	if(!PolyFlg) {														// <̧�ُo�͂̏ꍇ>
		fprintf(fp, "%d %f %f %f\n", 2, v.p[0], v.p[1], v.p[2]);		// �@���޸�ُo��
	}
	for(i=0; i<3; i++) {
		GetVtx(op, pa[i].vno, &pt);										// ���_���W�擾
		flg = (i==2) ? 1 : 0;											// ��ٰ�ߒ���/�I��
		fprintf(fp, "%d %f %f %f\n", flg, pt.p[0], pt.p[1], pt.p[2]);	// ���_���W�o��
	}
	FacNum++;															// �ʐ����Z
}

/**************************************************************/
static double TriangleArea(int p1, int p2, int p3, VERTEX* pa)			//<<< �O�p�`�����t���ʐ�
/**************************************************************/
{
	return ((pa[p1].x-pa[p3].x)*(pa[p2].y-pa[p3].y)
            -(pa[p2].x-pa[p3].x)*(pa[p1].y-pa[p3].y));
}

/*****************************************************************/
static BOOL CheckArea(int p1, int p2, int p3, int pp, VERTEX* pa)		//<<< �O�p�`�����m�F
/*****************************************************************/
{
	if(TriangleArea(pp, p1, p2, pa)<0.0) {								// �O�p�`�ʐ�
		return FALSE;
	}
	if(TriangleArea(pp, p2, p3, pa)<0.0) {								// �O�p�`�ʐ�
		return FALSE;
	}
	if(TriangleArea(pp, p3, p1, pa)<0.0) {								// �O�p�`�ʐ�
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
static int CheckInsideTriangle(int vm, int vf, int vb,VERTEX* pa,int pn)//<<< �O�p�߯�������
/************************************************************************/
{
	int pp, lp=-1;

    pp = (vf+1==pn) ? 0 : vf+1;
	while(pp!=vb) {
		if(CheckArea(vm, vf, vb, pp, pa)) {								// �O�p�`�����m�F
			if(lp==-1) {
				lp = pp;
			} else {
				if(pa[pp].y<pa[lp].y) {
					lp = pp;
				}
			}
		}
        pp = (pp+1==pn) ? 0 : pp+1;
	}
	return lp;
}

/**************************************************************/
static void MakeDivPolygon(int sp, int ep, VERTEX* pa, int pn,
                           VERTEX* pa1, int* pn1)						//<<< �V���p�`����
/**************************************************************/
{
	int ps, pd=0;

	ps = sp;
	while(ps!=ep) {
		pa1[pd++] = pa[ps++];
		if(ps==pn) {
			ps = 0;
		}
	}
	pa1[pd++] = pa[ps++], *pn1 = pd;
}

/******************************************/
static int FindYminVtx(VERTEX* pa, int pn)								//<<< �ŏ�Y���W�l�擾
/******************************************/
{
	int    i, lp=0;
	double miny;

	miny = pa[0].y;
	for(i=1; i<pn; i++) {
		if(pa[i].y<miny) {
			miny = pa[i].y, lp = i;
		}
	}
	return lp;
}

/***************************************************************/
static void DivisionPolygon(FILE* fp, OBJTYPE* op, VECTYPE vec, 
                            VERTEX* pa, int pn)							//<<< ���p�`���O�p�`�ɕ���
/***************************************************************/
{
	VERTEX* pa1 = new VERTEX[MAXFVTX];
	int     vm, vf, vb, vs, i, lc, pn1;
	double  fa;

	if(pn==3) {
		PutTriangle(fp, op, vec, pa);									// �O�p�`�߯��o��
	} else {
		lc = 0;
		do {
			vm = FindYminVtx(pa, pn);									// �ŏ�Y���W�l�擾
			if((vf=vm+1)>=pn) {
				vf = 0;
			}
			if((vb=vm-1)<0) {
				vb = pn-1;
			}
			fa = TriangleArea(vb, vm, vf, pa);							// �O�p�`�ʐ�
			if(fa<EPSILON) {
				pa[pn] = pa[0];
				for(i=0; i<pn; ++i) {
					pa[i] = pa[i+1];
				}
			}
			if(lc++>pn) {
				break;
			}
		} while(fa<EPSILON);
		vs = CheckInsideTriangle(vm, vf, vb, pa, pn);					// �O�p�߯�������
		if(vs<0) {														// <���_���̏ꍇ>
			MakeDivPolygon(vb, vf, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, op, vec, pa1, pn1);						// ���p�`����
			MakeDivPolygon(vf, vb, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, op, vec, pa1, pn1);						// ���p�`����
		} else {														// <���_�L�̏ꍇ>
			MakeDivPolygon(vm, vs, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, op, vec, pa1, pn1);						// ���p�`����
			MakeDivPolygon(vs, vm, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, op, vec, pa1, pn1);						// ���p�`����
		}
	}
	delete[] pa1;
}

/*******************************************************/
static void MakePolygon(PNTTYPE* pnt, int* vno, int pn,
                        VERTEX* pa, VECTYPE* vec)						//<<< �������ް��쐬
/*******************************************************/
{
	VECTYPE rvc;
	double  rot[3][3], th;
	int     i;

	CalcNVec(pnt, pn, vec);												// �@���޸�َ擾
	th = sqrt(vec->p[0]*vec->p[0]+vec->p[1]*vec->p[1]);					// �p�x,��]���擾
	if(th<EPSILON) {
		rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (vec->p[2]<0.0) ? PI : 0.0;
	} else {
		rvc.p[0] = -vec->p[1], rvc.p[1] = vec->p[0], rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (fabs(vec->p[2])>EPSILON) ? atan2(th, vec->p[2]) : PI/2.0;
		th = (th<-10||th>10) ? 0.0 : th;
	}
	SetRotateMatrix(rvc, -th, rot);										// ��]��د���쐬
	for(i=0; i<pn; i++) {
		VecRotate(&pnt[i], rot);										// �ʂ���](XY���ʂƕ��s)
		pa[i].vno = vno[i], pa[i].x = pnt[i].p[0], pa[i].y = pnt[i].p[1];
	}
}

/*********************************************/
static BOOL CheckPolygon(VERTEX* pa, int* pn)							//<<< ���p�`���_�m�F
/*********************************************/
{
	int i, j, pnum;

	pnum = *pn;
	pa[pnum] = pa[0];
	for(i=0; i<pnum; i++) {
		if(fabs(pa[i].x-pa[i+1].x)<EPSILON&&
           fabs(pa[i].y-pa[i+1].y)<EPSILON) {
			if(i!=(pnum-1)) {
				for(j=i+1; j<pnum; ++j) {
					pa[j] = pa[j+1];
				}
				pnum--;
			} else {
				pnum--; break;
			}
		}
	}
	if(pnum<3) {
		return FALSE;
	}
	*pn = pnum;
	return TRUE;
}

/************************************************/
int WriteCommonFile(int mode, const char* fname)						//<<< ����̧�ُo��
/************************************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	char*    tname = new char[_MAX_PATH];
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op;
	VECTYPE  vec;
	CString  pname;
	int      ret=0, ver, out, typ, fac, i, i1, i2, lp, pn, sln=0, cln=0, spr=0;
	double   dpr;

	PolyFlg = 0;														// ��غ���׸�(0:̧�َ��)
	ReadCommonFileParam(&ver, &out, &typ, &fac);						// ����̧�����Ұ��ۑ�̧�ٓǍ�
	if(mode==0) {														// <DXF̧�ق̏ꍇ>
		ret = InpDXFParam(&ver, &out, &fac);							// DXF���Ұ�����
	} else {															// <STĻ�ق̏ꍇ>
		ret = InpSTLParam(&typ, &fac);									// STL���Ұ�����
	}
	if(!ret) {															// <�޲�۸ސ���I��>
		WriteCommonFileParam(ver, out, typ, fac);						// ����̧�����Ұ��ۑ�̧�ُ���
	} else {															// <�޲�۸ގ���I��>
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] tname;
		return 0;
	}
	FacNum = 0, ObjNum = 0;												// �ʐ�����̐��ر
	i = strlen(fname)-3;												// ̧�ٖ���(�g���q�Ȃ�)
	memset(tname, 0x0, _MAX_PATH);
	memcpy(tname, fname, i);											// ̧�ٖ�����(�g���q�Ȃ�)
	memcpy(&tname[i], "tmp", 3);										// �g���q(tmp)
	if(fopen_s(&fp, tname, "w")!=0) {									// ���̧��OPEN
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] tname;
		return 1;
	}
	m_pProgressF3 = new CProgressDlg(3);
	m_pProgressF3->Create();											// �޲�۸ޕ\��
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	if(fac>0) {															// <�Ȗʍו�������>
		OnCurveReptSW(fac);												// �Ȗʍו���SW�I��
		BaseObjPtr(1, &op, &i1, &i2);									// �\�������߲���擾
		while(op!=NULL) {
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			NextObjPtr(1, &op, &i1, &i2);								// ���ҏW���̂�
		}
		View->Display(0);												// �ĕ`��
	}
	BaseObjPtr(1, &op, &i1, &i2);										// �\���Ώۗ��̎O�p�`����
	while(op) {
		sln += GetFlpNum1(op);											// ��ٰ�ߐ����Z
		NextObjPtr(1, &op, &i1, &i2);									// �����̂�
	}
	fprintf(fp, "%d %08d %08d\n", 9, FacNum, ObjNum);					// �ʐ�����̐��o��(��)
	BaseObjPtr(1, &op, &i1, &i2);										// �\���Ώۗ��̎O�p�`����
	while(op) {
		lp = GetFlpNum1(op);											// ��ٰ�ߐ��擾
		for(i=0, pn=0; i<lp; i++) {										// �S�ʂ��O�p�`����
			GetFlp1(op, i, &vno[pn], &i1);								// ��ٰ�ߔԍ��擾
			GetVtx(op, vno[pn], &pnt[pn]);								// ���_���W�擾
			pn++;
			if(i1) {													// <��ٰ�ߏI��>
				MakePolygon(pnt, vno, pn, pa, &vec);					// �������ް��쐬
				if(CheckPolygon(pa, &pn)) {								// ���p�`���_�m�F
					DivisionPolygon(fp, op, vec, pa, pn);				// ���p�`����
				}
				pn = 0;
			}
			cln++;
			dpr = 100.0*((double)cln/(double)sln);						// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF3->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
		}
		if(ret) {														// �����I�����͒��~
			break;
		}
		if(mode==0) {													// <DXF̧�ق̏ꍇ>
			fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);			// ���̏I���o��
			ObjNum++;													// ���̐����Z
		}
		NextObjPtr(1, &op, &i1, &i2);									// �����̂�
	}
	if(!ret&&mode==1) {													// <STĻ�ق̏ꍇ>
		fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);				// ���̏I���o��
		ObjNum++;														// ���̐����Z
	}
	fclose(fp);															// ���̧��CLOSE
	if(!ret) {															// <���폈����>
		if(fopen_s(&fp, tname, "r+")==0) {								// ���̧��OPEN
			fprintf(fp, "%d %08d %08d\n", 9, FacNum, ObjNum);			// �ʐ�����̐��o��(�{)
			fclose(fp);													// ���̧��CLOSE
		}
	}
	m_pProgressF3->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressF3;
	pname = GetAppPath(0)+CFAC_NAME;									// �ް��ۑ�̧��PATH�擾
	if(!ret) {															// <���폈����>
		if(mode==0) {													// <DXF̧�ق̏ꍇ>
			ret = ChgWDXFFile(fname, tname, ver, out);					// DXF̧�ٕϊ�(Write)
		} else {														// <STĻ�ق̏ꍇ>
			ret = ChgWSTLFile(fname, tname, typ);						// STĻ�ٕϊ�(Write)
		}
	}
	if(fac>0) {															// <�Ȗʍו�������>
		OffCurveReptSW();												// �Ȗʍו���SW�I�t
		BaseObjPtr(1, &op, &i1, &i2);									// �\�������߲���擾
		while(op!=NULL) {
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			NextObjPtr(1, &op, &i1, &i2);								// ���ҏW���̂�
		}
		View->Display(0);												// �ĕ`��
	}
	remove(tname);														// ���̧�ٍ폜
	if(!ret) {
		OffCmdFlg();													// ����ޖ��ۑ�OFF
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	delete[] tname;
	return ret;
}

/***********************************************/
int ReadCommonFile(int mode, const char* fname)							//<<< ����̧�ٓ���
/***********************************************/
{
	int*     ln = new int[MAXPART];
	int*     fn = new int[MAXPART];
	char*    tname = new char[_MAX_PATH];
	char*    b1 = new char[100];
	char*    b2 = new char[100];
	char*    b3 = new char[100];
	MSG      msg;
	OBJTYPE* op, * sop;
	OIFTYPE* oif;
	PNTTYPE  pa;
	FILE*    fp;
	int      ret, i, j, on, flg, pn, tvn, psw=0, cln=0, spr=0;
	double   dpr;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	i = strlen(fname)-3;												// ̧�ٖ���(�g���q�Ȃ�)
	memset(tname, 0x0, _MAX_PATH);
	memcpy(tname, fname, i);											// ̧�ٖ�����(�g���q�Ȃ�)
	memcpy(&tname[i], "tmp", 3);										// �g���q(tmp)
	if(mode==0) {														// <DXF̧�ق̏ꍇ>
		ret = ChgRDXFFile(fname, tname);								// DXF̧�ٕϊ�(Read)
	} else {															// <STĻ�ق̏ꍇ>
		ret = ChgRSTLFile(fname, tname);								// STĻ�ٕϊ�(Read)
	}
	if(!ret) {															// <���폈����>
		View->Display(0);												// �ĕ`��
		if(fopen_s(&fp, tname, "r")!=0) {								// ���̧��OPEN
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		psw = 1;														// ��۸�ڽ�\����
		m_pProgressF3 = new CProgressDlg(4);
		m_pProgressF3->Create();										// �޲�۸ޕ\��
		on = 0, pn = 0, ln[0] = 0, fn[0] = 0, sop = NULL;				// �߰�񏉊���
		FacNum = 0, ObjNum = 0;
		fscanf_s(fp, "%d%d%d", &flg, &FacNum, &ObjNum);					// �ʐ�����̐��擾
		while(TRUE) {													// EOF�܂�
			fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100);
			if(feof(fp)) {
				break;
			}
			if(flg<0) {													// <���̏I��>
				on++;													// �߰¶���UP
				pn = 0, ln[on] = 0, fn[on] = 0;							// �߰�񏉊���
			} else {													// <��ٰ��>
				pn++;													// ���_�����Z
				if(flg==1) {											// <��ٰ�ߏI��>
					ln[on] += pn, pn = 0;
					fn[on]++, cln++;									// ��ٰ�ߐ���ʐ����
					dpr = 5.0*((double)cln/(double)FacNum);				// �����߰���
					if(spr<(int)dpr) {									// <�߰���UP>
						spr = (int)dpr;
						m_pProgressF3->SetProgress(spr);				// �i���\��
					}
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// �����I��
						ret = -1; break;
					}
				}
			}
		}
		fclose(fp);														// ���̧��CLOSE
		if(!ret&&ln[0]==0) {											// �ް���
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		if(fopen_s(&fp, tname, "r")!=0) {								// ���̧��OPEN
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		if(on==0) {														// �ް��L�͗��̶�����1�ȏ�
			on = 1;
		}
		fscanf_s(fp, "%d%d%d", &flg, &FacNum, &ObjNum);					// �ʐ�����̐��擾
		OnCmdFlgStp();													// ���ۑ��׸ލX�V���~ON
		for(i=0, cln=0; i<on; i++) {									// �߰��ɍs��
			InitCVtx();													// ���_��ď�������
			for(j=0; j<fn[i]; j++) {									// �ʐ����s��
				do {	                                                // ���_���W����
					if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
						ret = 1; break;
					}
					pa.p[0] = atof(b1);									// �����ϊ�
					pa.p[1] = atof(b2);
					pa.p[2] = atof(b3);
					SetCVtx(pa);										// ��Ē��_�ݒ�
				} while(flg==0);
				if(ret) {												// ��ƒ��~
					break;
				}
				SetCFace();												// ��ٰ�ߏI���ݒ�
				cln++;
				dpr = 95.0*((double)cln/(double)FacNum)+5.0;			// �����߰���
				if(spr<(int)dpr) {										// <�߰���UP>
					spr = (int)dpr;
					m_pProgressF3->SetProgress(spr);					// �i���\��
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// �����I��
						ret = -1; break;
					}
				}
			}			                                                // ���̏I������
			if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
				ret = 1;
			}
			if(ret) {													// ��ƒ��~
				break;
			}
			if((tvn=SortCVtx())>0) {									// ���_���̿��&����
				CreatObj(tvn, ln[i], 0, &op);							// ���̗̈�m��
				LevelObj(0, op);										// �������ِ������̐ڑ�
				SetCVtxObj(op);											// ���_���ݒ�
				PutObjAtr(op, 0);										// ����No�ݒ�(DIRECT)
				PutObjPrt(op, 0, 1);									// �߰�No�ݒ�(�ʏ�)
				PutObjCol(op, 0);										// �װNo�ݒ�(�ް���)
				SetObjBox(op);											// �ޯ�����ސݒ�
				MakeDirectCommand(op);									// ���ڐ�������ސ���
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
				if(sop==NULL) {
					sop = op;
				}
			}
		}
		if(!ret&&sop!=NULL) {
			GetObjOif(sop, &oif);										// ���̍\������߲���擾
			View->SelectOifItem(oif);									// ���̍\������ёI��
		}
		OffCmdFlgStp();													// ���ۑ��׸ލX�V���~OFF
		fclose(fp);														// ���̧��CLOSE
	}
	remove(tname);														// ���̧�ٍ폜
	View->SetDataUse();													// �ް��g�p���ݒ�
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	if(psw) {															// <��۸�ڽ�\����>
		m_pProgressF3->DestroyWindow();									// �޲�۸ޔp��
		delete m_pProgressF3;
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] ln;
	delete[] fn;
	delete[] tname;
	delete[] b1;
	delete[] b2;
	delete[] b3;
	return ret;
}

/*****************************/
int MakePolygonData(int mode)											//<<< ��غ���ް��쐬
/*****************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	int*     ln = new int[MAXPART];
	int*     fn = new int[MAXPART];
	char*    b1 = new char[100];
	char*    b2 = new char[100];
	char*    b3 = new char[100];
	CString  path;
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op, * sop;
	OIFTYPE* oif;
	PNTTYPE  pt;
	VECTYPE  vec;
	int      ret=0, i, j, i1, i2, lp, pn, on, flg, tvn, sln, cln, spr=0;
	double   dpr;

	PolyFlg = 1;														// ��غ���׸�(1:��غ�ݍ쐬)
	FacNum = 0, ObjNum = 0;												// �ʐ�����̐��ر
	path = GetAppPath(0)+TMP_NAME;										// �ް�̧��PATH�擾
	if(fopen_s(&fp, path, "w")!=0) {									// ���̧��OPEN
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		delete[] ln;
		delete[] fn;
		delete[] b1;
		delete[] b2;
		delete[] b3;
		return 1;
	}
	m_pProgressF3 = new CProgressDlg(5);
	m_pProgressF3->Create();											// �޲�۸ޕ\��
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	sln = 0, cln = 0;
	if(!mode) {
		SelBaseObjPtr(1, &op);											// A�����̎擾
	} else {
		BaseObjPtr(2, &op, &i1, &i2);									// B�����̎擾
	}
	while(op) {
		sln += GetFlpNum1(op);											// ��ٰ�ߐ����Z
		if(!mode) {
			SelNextObjPtr(1, &op);										// ��A�����̂�
		} else {
			NextObjPtr(2, &op, &i1, &i2);								// ��B�����̂�
		}
	}
	if(!mode) {
		SelBaseObjPtr(1, &op);											// A�����̎擾
	} else {
		BaseObjPtr(2, &op, &i1, &i2);									// B�����̎擾
	}
	while(op) {
		lp = GetFlpNum1(op);											// ��ٰ�ߐ��擾
		for(i=0, pn=0; i<lp; i++) {										// �S�ʂ��O�p�`����
			GetFlp1(op, i, &vno[pn], &i1);								// ��ٰ�ߔԍ��擾
			GetVtx(op, vno[pn], &pnt[pn]);								// ���_���W�擾
			pn++;
			if(i1) {													// <��ٰ�ߏI��>
				MakePolygon(pnt, vno, pn, pa, &vec);					// �������ް��쐬
				if(CheckPolygon(pa, &pn)) {								// ���p�`���_�m�F
					DivisionPolygon(fp, op, vec, pa, pn);				// ���p�`����
				}
				pn = 0;
			}
			cln++;
			dpr = 50.0*((double)cln/(double)sln);						// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF3->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
		}
		if(ret) {														// �����I�����͒��~
			break;
		}
		fprintf(fp, "%d %f %f %f\n", -1, 0.0, 0.0, 0.0);				// ���̏I���o��
		ObjNum++;														// ���̐����Z
		if(!mode) {
			SelNextObjPtr(1, &op);										// ��A�����̂�
		} else {
			NextObjPtr(2, &op, &i1, &i2);								// ��B�����̂�
		}
	}
	fclose(fp);															// ���̧��CLOSE
	if(!ret) {															// <���폈����>
		if(fopen_s(&fp, path, "r")!=0) {								// ���̧��OPEN
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		if(!mode) {
			SelBaseObjPtr(1, &op);										// A�����̎擾
			while(op!=NULL) {
				sop = op;												// �폜�����߲���ۑ�
				SelNextObjPtr(1, &op);									// �����̂�
				LevelObj(1, sop);										// �������ٍ폜���̐ڑ�
				TempObj(sop);											// ���̈ꎞ�폜
			}
		} else {
			Delete();													// B�����̂̍폜
		}
		cln = 0, on = 0, pn = 0, ln[0] = 0, fn[0] = 0, sop = NULL;		// �߰�񏉊���
		while(TRUE) {													// EOF�܂�
			fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100);
			if(feof(fp)) {
				break;
			}
			if(flg<0) {													// <���̏I��>
				on++;													// �߰¶���UP
				pn = 0, ln[on] = 0, fn[on] = 0;							// �߰�񏉊���
			} else {													// <��ٰ��>
				pn++;													// ���_�����Z
				if(flg==1) {											// <��ٰ�ߏI��>
					ln[on] += pn, pn = 0;
					fn[on]++, cln++;									// ��ٰ�ߐ���ʐ����
					dpr = 5.0*((double)cln/(double)FacNum)+50.0;		// �����߰���
					if(spr<(int)dpr) {									// <�߰���UP>
						spr = (int)dpr;
						m_pProgressF3->SetProgress(spr);				// �i���\��
					}
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// �����I��
						ret = -1; break;
					}
				}
			}
		}
		fclose(fp);														// ���̧��CLOSE
		if(!ret&&ln[0]==0) {											// �ް���
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		if(fopen_s(&fp, path, "r")!=0) {								// ���̧��OPEN
			ret = 1;
		}
	}
	if(!ret) {															// <���폈����>
		if(on==0) {														// �ް��L�͗��̶�����1�ȏ�
			on = 1;
		}
		for(i=0, cln=0; i<on; i++) {									// �߰��ɍs��
			InitCVtx();													// ���_��ď�������
			for(j=0; j<fn[i]; j++) {									// �ʐ����s��
				do {	                                                // ���_���W����
					if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
						ret = 1; break;
					}
					pt.p[0] = atof(b1);									// �����ϊ�
					pt.p[1] = atof(b2);
					pt.p[2] = atof(b3);
					SetCVtx(pt);										// ��Ē��_�ݒ�
				} while(flg==0);
				if(ret) {												// ��ƒ��~
					break;
				}
				SetCFace();												// ��ٰ�ߏI���ݒ�
				cln++;
				dpr = 45.0*((double)cln/(double)FacNum)+55.0;			// �����߰���
				if(spr<(int)dpr) {										// <�߰���UP>
					spr = (int)dpr;
					m_pProgressF3->SetProgress(spr);					// �i���\��
				}
				if(PeekMessage(&msg, m_pProgressF3->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressF3->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// �����I��
						ret = -1; break;
					}
				}
			}			                                                // ���̏I������
			if(fscanf_s(fp, "%d%s%s%s", &flg, b1, 100, b2, 100, b3, 100)!=4) {
				ret = 1;
			}
			if(ret) {													// ��ƒ��~
				break;
			}
			if((tvn=SortCVtx())>0) {									// ���_���̿��&����
				CreatObj(tvn, ln[i], 0, &op);							// ���̗̈�m��
				LevelObj(0, op);										// �������ِ������̐ڑ�
				SetCVtxObj(op);											// ���_���ݒ�
				PutObjAtr(op, 0);										// ����No�ݒ�(DIRECT)
				PutObjPrt(op, 0, 1);									// �߰�No�ݒ�(�ʏ�)
				PutObjCol(op, 0);										// �װNo�ݒ�(�ް���)
				SetObjBox(op);											// �ޯ�����ސݒ�
				MakeDirectCommand(op);									// ���ڐ�������ސ���
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
				if(!mode) {
					SetSelNo(op, 1);									// A���}�`���Z���̓o�^
				} else {
					OnEdtFlg(1, op);									// B���}�`���Z���̓o�^
				}
				if(sop==NULL) {
					sop = op;
				}
			}
		}
		if(!ret&&sop!=NULL) {
			GetObjOif(sop, &oif);										// ���̍\������߲���擾
			View->SelectOifItem(oif);									// ���̍\������ёI��
		}
		fclose(fp);														// ���̧��CLOSE
	}
	EndJMenu1();														// �ƭ��I������
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->SetDataUse();													// �ް��g�p���ݒ�
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	m_pProgressF3->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressF3;
	remove(path);														// ���̧�ٍ폜
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	delete[] ln;
	delete[] fn;
	delete[] b1;
	delete[] b2;
	delete[] b3;
	return ret;
}

/********************************/
void ExportSLCFile(CString file)										//<<< ����߰�SLC
/********************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      ret, i1, i2;
	CString  pname;

	BaseObjPtr(1, &op, &i1, &i2);										// �\�������߲���擾
	if(op==NULL) {														// <���̖��͒��~>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// �ް��ް��擪�߲���擾
	pname = GetAppPath(0)+SLCA_NAME;									// �ް��ۑ�̧��PATH�擾
	ret = MakeSLCFile(op, vtx, flp, file, pname);						// SLÇ�ٍ쐬
	if(ret==1) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ̧�ُo�ʹװ
	} else if(ret==2) {
		MemErr(IDS_MEMERR5);											// ��شװ
	} else if(ret<0) {
		AfxMessageBox(IDS_STOP_PROCESS);								// ���~
	}
}
