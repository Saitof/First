/////////////////////////////////////////////////////////////////////////////
// POV-Ray�f�[�^�쐬��
// CreatPovData.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <direct.h>
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

#define	MAXFVTX	1000													// �ő�ʒ��_��

typedef	struct {
	int    vno;															// 3������̒��_�ԍ�
	double x;															// 2�������X���W
	double y;															//    �V    Y���W
} VERTEX;

CProgressDlg* m_pProgressP=NULL;										// ��۸�ڽ�ݼ޹���

/*********************************************************************/
static void PutTriangle(FILE* fp, int shape, OBJTYPE* op, VERTEX* pa)	//<<< �O�p�`�߯��o��
/*********************************************************************/
{
	PNTTYPE pt[3];
	VECTYPE vec;
	CString sbuf;
	int     i;

	for(i=0; i<3; i++) {
		GetVtx(op, pa[i].vno, &pt[i]);									// ���_���W�擾
	}
	if(shape==0) {														// <Smooth Triangle>
		CalcNVec(pt, 3, &vec);											// �@���޸�َ擾
		SetCNVtx(pt[0], vec);											// ��Ē��_�ݒ�
		SetCNVtx(pt[1], vec);											// ��Ē��_�ݒ�
		SetCNVtx(pt[2], vec);											// ��Ē��_�ݒ�
	} else {															// <Triangle>
		sbuf.LoadString(IDS_STR121);
		fprintf(fp, "%s\n", (LPCTSTR)sbuf);  //##
		sbuf.Format(IDS_STR116, pt[0].p[0], pt[0].p[1], pt[0].p[2],
								pt[1].p[0], pt[1].p[1], pt[1].p[2],
								pt[2].p[0], pt[2].p[1], pt[2].p[2]);
		fprintf(fp, "%s\n\t}\n", (LPCTSTR)sbuf);  //##
	}
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

/************************************************************/
static void DivisionPolygon(FILE* fp, int shape,
                            OBJTYPE* op, VERTEX* pa, int pn)			//<<< ���p�`���O�p�`�ɕ���
/************************************************************/
{
	VERTEX* pa1 = new VERTEX[MAXFVTX];
	int     vm, vf, vb, vs, i, lc, pn1;
	double  fa;

	if(pn==3) {
		PutTriangle(fp, shape, op, pa);									// �O�p�`�߯��o��
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
			DivisionPolygon(fp, shape, op, pa1, pn1);					// ���p�`����
			MakeDivPolygon(vf, vb, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, shape, op, pa1, pn1);					// ���p�`����
		} else {														// <���_�L�̏ꍇ>
			MakeDivPolygon(vm, vs, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, shape, op, pa1, pn1);					// ���p�`����
			MakeDivPolygon(vs, vm, pa, pn, pa1, &pn1);					// �V���p�`����
			DivisionPolygon(fp, shape, op, pa1, pn1);					// ���p�`����
		}
	}
	delete[] pa1;
}

/*******************************************************************/
static void MakePolygon(PNTTYPE* pnt, int* vno, int pn, VERTEX* pa)		//<<< �������ް��쐬
/*******************************************************************/
{
	VECTYPE vec, rvc;
	double  rot[3][3], th;
	int     i;

	CalcNVec(pnt, pn, &vec);											// �@���޸�َ擾
	th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);						// �p�x,��]���擾
	if(th<EPSILON) {
		rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (vec.p[2]<0.0) ? PI : 0.0;
	} else {
		rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
		VecN(&rvc);
		th = (fabs(vec.p[2])>EPSILON) ? atan2(th, vec.p[2]) : PI/2.0;
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

/**************************************************************/
int OutputPovObj(int cno, int ono, CString path, CString name,
				 int shape, int smoth, double angle,
				 double sper, double per)								//<<< POV-Ray��޼ު�ďo��
/**************************************************************/
{
	PNTTYPE* pnt = new PNTTYPE[MAXFVTX];
	VERTEX*  pa = new VERTEX[MAXFVTX];
	int*     vno = new int[MAXFVTX];
	CString  ofile, sbuf;
	MSG      msg;
	FILE*    fp;
	OBJTYPE* op;
	NVTYPE*  wvp, * wv1, * wv2, * wv3;
	int      ret=0, i, j, lp, pn, sln=0, cln=0, spr=0;
	double   dpr;

	sbuf.Format(IDS_STR118, name, ono);									// �ݸٰ��̧�ٖ�
	ofile = path  + "\\" + sbuf;
	if(fopen_s(&fp, ofile, "w")!=0) {									// �o��̧��Open
		delete[] pnt;
		delete[] pa;
		delete[] vno;
		return 1;
	}
	sbuf.Format(IDS_STR112, ono);
	fprintf(fp, "%s\n", (LPCTSTR)sbuf);											// DECLARE�錾�o��
	InitCNVtx();														// ���_��ď�������
	ColorBaseObjPtr(cno, &op);											// �װ�ʗ����ް��߲���擾
	while(op) {
		sln += GetFlpNum1(op);											// ��ٰ�ߐ����Z
		ColorNextObjPtr(cno, &op);										// �װ�ʎ������߲���ړ�
	}
	ColorBaseObjPtr(cno, &op);											// �װ�ʗ����ް��߲���擾
	while(op) {
		lp = GetFlpNum1(op);											// ��ٰ�ߐ��擾
		for(i=0, pn=0; i<lp; i++) {										// �S�ʂ��O�p�`����
			GetFlp1(op, i, &vno[pn], &j);								// ��ٰ�ߔԍ��擾
			GetVtx(op, vno[pn], &pnt[pn]);								// ���_���W�擾
			pn++;
			if(j) {														// <��ٰ�ߏI��>
				MakePolygon(pnt, vno, pn, pa);							// �������ް��쐬
				if(CheckPolygon(pa, &pn)) {								// ���p�`���_�m�F
					DivisionPolygon(fp, shape, op, pa, pn);				// ���p�`����
				}
				pn = 0;
			}
			cln++;
			if(shape==0) {												// <Smooth Triangle>
				dpr = (per/2.0)*((double)cln/(double)sln)+sper;			// �����߰���
			} else {													// <Triangle>
				dpr = per*((double)cln/(double)sln)+sper;				// �����߰���
			}
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressP->SetProgress(spr);							// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressP->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressP->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
		}
		if(ret) {														// �����I�����͒��~
			break;
		}
		ColorNextObjPtr(cno, &op);										// �װ�ʎ������߲���ړ�
	}
	if(shape==0) {														// <Smooth Triangle>
		if(SortCNVtx(smoth, angle)) {									// ���_����&�ݒ�
			cln = 0;
			wvp = NVp;
			while(wvp) {
				wv1 = wvp;												// 1�_��
				if((wvp=wvp->next)==NULL) {
					break;
				}
				wv2 = wvp;												// 2�_��
				if((wvp=wvp->next)==NULL) {
					break;
				}
				wv3 = wvp;												// 3�_��
				wvp = wvp->next;										// ���ʂ�
				sbuf.LoadString(IDS_STR122);
				fprintf(fp, "%s\n", (LPCTSTR)sbuf);
				sbuf.Format(IDS_STR117, wv1->pt.p[0], wv1->pt.p[1], wv1->pt.p[2],
                                        wv1->vc.p[0], wv1->vc.p[1], wv1->vc.p[2],
                                        wv2->pt.p[0], wv2->pt.p[1], wv2->pt.p[2],
                                        wv2->vc.p[0], wv2->vc.p[1], wv2->vc.p[2],
                                        wv3->pt.p[0], wv3->pt.p[1], wv3->pt.p[2],
                                        wv3->vc.p[0], wv3->vc.p[1], wv3->vc.p[2]);
				fprintf(fp, "%s\n\t}\n", (LPCTSTR)sbuf);
				cln += 3;
				dpr = (per/2.0)*((double)cln/(double)Now_no)+sper+(per/2.0);
				if(spr<(int)dpr) {										// <�߰���UP>
					spr = (int)dpr;
					m_pProgressP->SetProgress(spr);						// �i���\��
				}
				if(PeekMessage(&msg, m_pProgressP->m_Button.m_hWnd,
                               0, 0, PM_NOREMOVE)) {
					GetMessage(&msg, m_pProgressP->m_Button.m_hWnd, 0, 0);
					if(msg.message==WM_LBUTTONUP) {						// �����I��
						ret = -1; break;
					}
				}
			}
		}
		EndCNVtx();														// ���_��Đݒ�I��
	}
	fprintf(fp, "}\n");
	fclose(fp);															// ���̧��CLOSE
	delete[] pnt;
	delete[] pa;
	delete[] vno;
	return ret;
}

/*******************************/
int CreatePovData(CString name)											//<<< POV-Ray�ް��쐬
/*******************************/
{
	WIN32_FIND_DATA	wfd;
	OBJTYPE* op;
	CString  ifile, opath, ofile, def1, def2, dname, sbuf;
	CString  stex2[POVCMAX], stex3[POVCMAX], stex4[POVCMAX], stex5[POVCMAX];
	int      i, j, cnt, ret, smot, tran, rota, scal;
	int      anim, aniz, aniy, targ, colc, refl, refr;
	int      shape[POVCMAX], itex1[POVCMAX], itex2[POVCMAX];
	int      itex3[POVCMAX], itex4[POVCMAX], itex5[POVCMAX];
	double   angl, trnx, trny, trnz, rotx, roty, rotz, scax, scay, scaz; 
	double   sper, per;
	char     buff[256], buff2[256], ftex1[POVCMAX], ftex2[POVCMAX];

	// �p�����[�^�擾
	GetPovData1(&ifile, &sbuf, &opath, &smot, &angl,
                &tran, &trnx, &trny, &trnz,
                &rota, &rotx, &roty, &rotz,
                &scal, &scax, &scay, &scaz,
                &anim, &aniz, &aniy);									// POV-Ray�ް��擾1
	GetPovData2(&def1, &def2, &sbuf, &targ, &colc, &refl, &refr);		// POV-Ray�ް��擾2
	GetPovData3(shape, itex1, itex2, itex3, itex4, itex5);				// POV-Ray�ް��擾3
	GetColorTex(itex1, itex2, itex3, itex4, itex5,
                ftex1, ftex2, stex2, stex3, stex4, stex5);				// �װ��ø����擾
	opath = opath + name;												// �o��̫���(���߽)
	ofile = opath + "\\" + name + ".pov";								// �o��̧��(���߽)
	// �o�̓t�H���_�`�F�b�N
	if(FindFirstFile(opath, &wfd)==INVALID_HANDLE_VALUE) {				// <̫��ޖ��̏ꍇ>
		if(_mkdir(opath)!=0) {											// ̫��ލ쐬
			AfxMessageBox(IDS_ERR_MKDIR);								// �̫��ލ쐬NG�
		}
	}
	// �t�@�C���`�F�b�N
	std::fstream istrm(ifile, std::ios::in);							// ��`̧��Open
	if(!istrm.rdbuf()->is_open()) {										// ��`̧��Open����
		AfxMessageBox(IDS_ERR_DEFFILE);									// ���`̧�قɈُ킠��
		return 0;
	}
	istrm.getline(buff, sizeof(buff));									// 1�s�ړ���
	sbuf.LoadString(IDS_STR96);										// ���K��1�s���ް�
	i = sbuf.GetLength();
	memset(buff2, 0x0, sizeof(buff2));
	memcpy(buff2, sbuf.GetBuffer(i), i);
	if(strncmp(buff, buff2, i)) {										// <��v���Ȃ��ꍇ>
		AfxMessageBox(IDS_ERR_DEFFILE);									// ���`̧�قɈُ킠��
		return 0;
	}
	std::fstream ostrm(ofile, std::ios::out);							// �o��̧��Open
	if(!ostrm.rdbuf()->is_open()) {										// �o��̧��Open����
		AfxMessageBox(IDS_ERR_MKFILE);									// �̧�ٍ쐬NG�
		return 0;
	}
	// POV-Ray�t�@�C��(.pov)�ҏW
	m_pProgressP = new CProgressDlg(6);
	m_pProgressP->Create();												// �޲�۸ޕ\��
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	strcpy_s(buff, "");
	while(istrm.getline(buff, sizeof(buff))) {
		if(!strncmp(buff, "/*", 2)) {									// �ׯ��+���ؽ��s�͖���
			continue;
		}
		if(!strncmp(buff, "[[[A]]]", 7)) {								// <�ݸٰ�ސ錾�ʒu>
			if(def1!="") {												// հ�ޒ�`̧�ٖ�1
				i = def1.ReverseFind('\\');
				j = def1.ReverseFind('.');
				dname = def1.Mid(i+1, j-i-1);							// հ�ޒ�`̧�ٖ�1(�g���q�Ȃ�)
				sbuf.Format(IDS_STR111, dname);							// �ݸٰ�ސ錾
				ostrm<<sbuf;
			}
			if(def2!="") {												// հ�ޒ�`̧�ٖ�2
				i = def2.ReverseFind('\\');
				j = def2.ReverseFind('.');
				dname = def2.Mid(i+1, j-i-1);							// հ�ޒ�`̧�ٖ�2(�g���q�Ȃ�)
				sbuf.Format(IDS_STR111, dname);							// �ݸٰ�ސ錾
				ostrm<<sbuf;
			}
			for(cnt=0, i=0; i<POVCMAX; i++) {							// �S�װ�J��Ԃ�
				ColorBaseObjPtr(i, &op);								// �װ�ʗ����ް��߲���擾
				if(op!=NULL) {											// <�װ�ʂ̗L�����̂�����ꍇ>
					sbuf.Format(IDS_STR110, name, cnt);					// �ݸٰ�ސ錾
					ostrm<<sbuf;
					cnt++;
				}
			}
			if(anim) {													// <��Ұ��݂���>
				sbuf.LoadString(IDS_STR109);							// ��s
				ostrm<<sbuf;
				sbuf.LoadString(IDS_STR123);							// ���čs(����)
				ostrm<<sbuf;
				sbuf.Format(IDS_STR124, aniz*(aniy+1), aniz*(aniy+1)-1);// ���čs(��Ұ��ݺϐ�)
				ostrm<<sbuf;
			}
		} else if(!strncmp(buff, "[[[B]]]", 7)) {						// <��޼ު�ď��ʒu>
			for(cnt=0, i=0; i<POVCMAX; i++) {							// �S�װ�J��Ԃ�
				ColorBaseObjPtr(i, &op);								// �װ�ʗ����ް��߲���擾
				if(op!=NULL) {											// <�װ�ʂ̗L�����̂�����ꍇ>
					sbuf.Format(IDS_STR119, cnt);						// ��޼ު�ď��
					ostrm<<sbuf;
					if(ftex1[i]=='H') {									// <ø����ގ��敪:��׽2>
						sbuf.Format(IDS_STR105, stex3[i]);
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR106, stex4[i], stex5[i]);
						ostrm<<"  "<<sbuf;
					} else if(ftex1[i]=='I') {							// <ø����ގ��敪:��׽3>
						sbuf.Format(IDS_STR107, stex5[i], stex3[i]);
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR108, stex4[i]);
						ostrm<<"  "<<sbuf;
					} else {											// <ø����ގ��敪:���̑�>
						if(ftex2[i]=='A') {
							sbuf.Format(IDS_STR101, stex2[i]);
						} else if(ftex2[i]=='B') {
							sbuf.Format(IDS_STR102, stex2[i]);
						} else if(ftex2[i]=='C') {
							sbuf.Format(IDS_STR103, stex2[i]);
						} else {
							sbuf.Format(IDS_STR104, stex2[i]);
						}
						ostrm<<"  "<<sbuf;
						if(ftex2[i]=='E') {								// <ø���̫�ϯċ敪:հ�ޒ�`2>
							sbuf.LoadString(IDS_STR120);				// photons
							ostrm<<"  "<<sbuf;
							if(targ) {									// target�L
								sbuf.LoadString(IDS_STR97);
								ostrm<<"    "<<sbuf;
							}
							if(colc) {									// collect on
								sbuf.Format(IDS_STR98, "on");
							} else {									// collect off
								sbuf.Format(IDS_STR98, "off");
							}
							ostrm<<"    "<<sbuf;
							if(refl) {									// reflection on
								sbuf.Format(IDS_STR99, "on");
							} else {									// reflection off
								sbuf.Format(IDS_STR99, "off");
							}
							ostrm<<"    "<<sbuf;
							if(refr) {									// refraection on
								sbuf.Format(IDS_STR100, "on");
							} else {									// refraction off
								sbuf.Format(IDS_STR100, "off");
							}
							ostrm<<"    "<<sbuf;
							sbuf.LoadString(IDS_STR109);
							ostrm<<"  }"<<sbuf;
						}
					}
					if(tran) {											// <Translate����>
						sbuf.Format(IDS_STR113, trnx, trny, trnz);
						ostrm<<"  "<<sbuf;
					}
					if(rota) {											// <Rotate����>
						sbuf.Format(IDS_STR114, rotx, roty, rotz);
						ostrm<<"  "<<sbuf;
					}
					if(scal) {											// <Scale����>
						sbuf.Format(IDS_STR115, scax, scay, scaz);
						ostrm<<"  "<<sbuf;
					}
					if(anim) {											// <��Ұ��݂���>
						sbuf.Format(IDS_STR125, 360*(aniy+1));
						ostrm<<"  "<<sbuf;
						sbuf.Format(IDS_STR126, aniy+1, 180.0/aniy);
						ostrm<<"  "<<sbuf;
					}
					sbuf.LoadString(IDS_STR109);
					ostrm<<"}"<<sbuf;
					cnt++;
				}
			}
		} else {														// <POV-Ray�L�q>
			sbuf.LoadString(IDS_STR109);
			ostrm<<buff<<sbuf;											// ���̂܂ܺ�߰
		}
	}
	// ���[�U��`�t�@�C��(.inc)�R�s�[
	if(def1!="") {
		i = def1.ReverseFind('\\');
		dname = def1.Mid(i+1);											// հ�ޒ�`̧�ٖ�1
		sbuf = opath  + "\\" + dname;
		CopyFile(def1, sbuf, FALSE);									// ̧�ٺ�߰
	}
	if(def2!="") {
		i = def2.ReverseFind('\\');
		dname = def2.Mid(i+1);											// հ�ޒ�`̧�ٖ�2
		sbuf = opath  + "\\" + dname;
		CopyFile(def2, sbuf, FALSE);									// ̧�ٺ�߰
	}
	// �I�u�W�F�N�g�f�[�^�ϊ�
	per = 100.0/(double)cnt;											// 1���̖��߰���
	for(cnt=0, i=0; i<POVCMAX; i++) {									// �S�װ�J��Ԃ�
		ColorBaseObjPtr(i, &op);										// �װ�ʗ����ް��߲���擾
		if(op!=NULL) {													// <�װ�ʂ̗L�����̂�����ꍇ>
			sper = per*cnt;
			ret = OutputPovObj(i, cnt, opath, name,
                               shape[i], smot, angl, sper, per);		// POV-Ray��޼ު�ďo��
			if(ret!=0) {												// �ُ�͏������~
				break;
			}
			cnt++;
		}
	}
	m_pProgressP->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressP;
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	return 0;
}
