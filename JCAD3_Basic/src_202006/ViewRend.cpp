/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewRend.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*********************************************/
BOOL CJcad3GlbView::GetJewelFVec(OBJTYPE* op)							//<<< �ʖ@���޸�َ擾
/*********************************************/
{
	VECTYPE vec;
	int     lnm, i, j, cnt, stn, vno, flg;

	if((lnm=GetFlpNum1(op))>m_MaxSPnt) {								// ��ٰ�ߐ�1�擾
		return FALSE;
	}
	PNTTYPE* pt = new PNTTYPE[m_MaxCPnt];
	VECTYPE* lv = new VECTYPE[m_MaxSPnt];
	for(i=0, cnt=0, stn=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ�ߔԍ��擾
		GetVtx(op, vno, &pt[cnt++]);									// ���_���W�擾
		if(flg==1) {													// <<<ٰ�ߍŏI�̏ꍇ>>>
			CalcNVec(pt, cnt, &vec);									// �@���޸�َ擾
			for(j=0; j<cnt; j++) {
				lv[stn+j] = vec;										// ��ٰ�ߒP�ʂɐݒ�
			}
			cnt = 0;													// ���_����������
			stn = i+1;													// ��ٰ�ߊJ�n�ԍ�
		}
	}
	if(lnm>0) {
		SetRenderList(op, lv);											// �ި���ڲ�ؽēo�^
	}
	delete[] pt;
	delete[] lv;
	return TRUE;
}

/***********************************************/
BOOL CJcad3GlbView::SetShadingData(OBJTYPE* op)							//<<< ����ިݸ��ް��ݒ�
/***********************************************/
{
	PNTTYPE wp;
	VECTYPE v1, v2, v3;
	int     vnm, lnm, i, j, k, l, cnt, an, vno, flg, stp;

	if((vnm=GetVtxNum(op))>m_MaxOPnt) {									// ���_���擾
		return FALSE;
	}
	if((lnm=GetFlpNum1(op))>4*m_MaxOPnt) {								// ��ٰ�ߐ�1�擾
		return FALSE;
	}
	PNTTYPE* pt = new PNTTYPE[m_MaxCPnt];
	VECTYPE* lv = new VECTYPE[4*m_MaxOPnt];
	VECTYPE* pv = new VECTYPE[m_MaxOPnt];
	VECTYPE* av = new VECTYPE[m_MaxOPnt];
	VECTYPE* wv = new VECTYPE[m_MaxCPnt];
	int*     fg = new int[m_MaxOPnt];
	int*     ln = new int[m_MaxOPnt];
	int*     nx = new int[m_MaxOPnt];
	int*     wf = new int[m_MaxCPnt];
	int*     pn = new int[m_MaxCPnt];
	int*     lp = new int[m_MaxCPnt];
	for(i=0; i<vnm; i++) {												// �׸ޏ�����
		fg[i] = -2;
	}
	for(i=0, cnt=0, an=0, stp=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ�ߔԍ��擾
		GetVtx(op, vno, &wp);											// ���_���W�擾
		pt[cnt] = wp, lp[cnt] = i;										// ���W,ٰ�ߔԍ�,���_�ԍ��ۑ�
		pn[cnt++] = vno;
		if(flg==1) {													// <ٰ�ߍŏI�̏ꍇ>
			CalcNVec(pt, cnt, &v1);										// �@���޸�َ擾
			for(j=0; j<cnt; j++) {
				if(fg[pn[j]]==-2) {										// <�����̏ꍇ>
					pv[pn[j]] = v1, fg[pn[j]] = -1;						// �Ώے��_�޸��,�׸ސݒ�
				} else {												// <�����ȊO�̏ꍇ>
					v2 = pv[pn[j]];										// �o�^���޸��
					VecN(&v2);											// �P���޸�ى�
					if(fabs(CalcVecAngle(&v1, &v2))<PI/5.0) {			// <36�x����(�Ȑ�)>
						for(k=0; k<DIM; k++) { 
							pv[pn[j]].p[k] += v1.p[k];					// �Ώے��_�޸�ى��Z
						}
					} else {											// <36�x�ȏ�(�ܐ�)>
						if(fg[pn[j]]==-1) {								// <�ŏ��̐ܐ�>
							if(an>=m_MaxOPnt) {							// �ް��ْ��~
								stp = 1; break;
							}
							fg[pn[j]] = an;								// �ܐ��z��ԍ��ݒ�
							av[an] = v1, ln[an] = lp[j], nx[an++] = -1;	// ���_�޸��,ٰ�ߔԍ��ݒ�
						} else {										// <2���ڈȍ~�̐ܐ�>
							if(an>=m_MaxOPnt) {							// �ް��ْ��~
								stp = 1; break;
							}
							k = fg[pn[j]];
							while(1) {
								if(nx[k]==-1) {							// �ŏI�ܐ��z��ԍ�����
									break;
								}
								k = nx[k];
							}
							nx[k] = an;									// �ܐ��z��ԍ��ݒ�
							av[an] = v1, ln[an] = lp[j], nx[an++] = -1;	// ���_�޸��,ٰ�ߔԍ��ݒ�
						}
					}
				}
			}
			if(stp) {													// �ް��ْ��~
				break;
			}
			cnt = 0;													// ���_����������
		}
	}
	if(stp) {															// <�ް��ق̏ꍇ>
		delete[] pt;
		delete[] lv;
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] ln;
		delete[] nx;
		delete[] wf;
		delete[] pn;
		delete[] lp;
		return FALSE;
	}
	for(i=0; i<vnm; i++) {
		VecN(&pv[i]);													// �P���޸�ى�
		if(fg[i]>=0) {													// <�ܐ��L�̏ꍇ>
			cnt = 0, j = fg[i];
			while(1) {
				wv[cnt] = av[j], wf[cnt++] = 0;							// �ܐ��޸�ٕ���
				if(nx[j]==-1) {
					break;
				}
				j = nx[j];
			}
			for(j=0; j<cnt; j++) {
				if(wf[j]==0) {											// <�������޸��>
					v1 = wv[j];
					for(k=j+1; k<cnt; k++) {
						v2 = v1;
						VecN(&v2);										// �P���޸�ى�
						v3 = wv[k];
						if(fabs(CalcVecAngle(&v2, &v3))<PI/5.0) {		// <36�x����(�Ȑ�)>
							for(l=0; l<DIM; l++) {
								v1.p[l] += v3.p[l];						// �Ώے��_�޸�ى��Z
							}
							wf[k] = 1;									// �����r��
						}
					}
					VecN(&v1);											// �P���޸�ى�
					wv[j] = v1, wf[j] = 2;								// ���e�͈͓��W�v��
					for(k=j+1; k<cnt; k++) {							// <�����r���̏ꍇ>
						if(wf[k]==1) {
							wv[k] = v1, wf[k] = 2;						// ���e�͈͓��W�v��
						}
					}
				}
			}
			cnt = 0;
			j = fg[i];
			while(1) {
				av[j] = wv[cnt++];										// �ܐ��޸�ٕ���
				if(nx[j]==-1) {
					break;
				}
				j = nx[j];
			}
		}
	}
	for(i=0; i<lnm; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ�ߔԍ��擾
		if(fg[vno]<0) {													// <�Sٰ�ߏW�v>
			lv[i] = pv[vno];											// ��ٰ�ߖ@���޸��
		} else {														// <��L�ȊO>
			v1 = pv[vno], j = fg[vno];
			while(1) {
				if(i==ln[j]) {											// <ٰ�ߔԍ���v>
					v1 = av[j]; break;
				}
				if(nx[j]==-1) {											// �޸�ْu��
					break;
				}
				j = nx[j];
			}
			lv[i] = v1;													// ��ٰ�ߖ@���޸��
		}
	}
	if(lnm>0) {
		SetRenderList(op, lv);											// �ި���ڲ�ؽēo�^
	}
	delete[] pt;
	delete[] lv;
	delete[] pv;
	delete[] av;
	delete[] wv;
	delete[] fg;
	delete[] ln;
	delete[] nx;
	delete[] wf;
	delete[] pn;
	delete[] lp;
	return TRUE;
}

/*****************************************/
BOOL CJcad3GlbView::ObjectRendering(void)                               //<<< �����ݸޏ���
/*****************************************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(1, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		if(GetListMode(op)!=3) {										// <<����ިݸ��ް����ݒ�̏ꍇ>>
			if(GetObjAtr(op)!=1) {										// <��ζ�ĈȊO>
				if(!SetShadingData(op)) {								// ����ިݸ��ް��ݒ�
					return FALSE;
				}
			} else {													// <��ζ��>
				if(!GetJewelFVec(op)) {									// �ʖ@���޸�َ擾
					return FALSE;
				}
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);									// �����̂�
	}
	return TRUE;
}

/**********************************************/
void CJcad3GlbView::ObjectShading(OBJTYPE* op)                          //<<< ����ިݸޏ���
/**********************************************/
{
	if(op!=NULL&&m_Shading) {											// <���̗L�ż���ިݸޕ\���L>
		if(GetListMode(op)!=3) {										// <<����ިݸ��ް����ݒ�̏ꍇ>>
			CWaitCursor wait;											// ���ĥ���ق�\��
			if(SetShadingData(op)) {									// ����ިݸ��ް��ݒ�
				SetSelNo(op, 3);										// ����ިݸޗ��̓o�^
				SetRenderMode(1);										// ����ިݸ�Ӱ��
			}
		} else {														// <<����ިݸ��ް��ݒ�ς̏ꍇ>>
			SetSelNo(op, 3);											// ����ިݸޗ��̓o�^
			SetRenderMode(1);											// ����ިݸ�Ӱ��
		}
	}
}

/**************************************************************/
void CJcad3GlbView::MenuAllShad(UINT flg, const CPoint& point,
                                int plane, int ms)                      //<<< ����ިݸ�
/**************************************************************/
{
	if(plane==PRS&&ms!=4) {												// �����}��ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==0) {
		m_RendPlane = (m_FChar&0x1) ? 1 : 0;							// [F5]:3�ʐ}�����ݸ�
		if(ObjectRendering()) {                                         // �����ݸޏ���
			SetRenderMode(3);
		}
		Display(0);                                                     // �ĕ`��
	} else if(ms==4||ms==9) {
		m_RendPlane = 0;
		SetRenderMode(0);
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/*************************************************************/
void CJcad3GlbView::MenuRender(UINT flg, const CPoint& point,
                               int plane, int ms)                       //<<< �����ݸ�
/*************************************************************/
{
	if(plane==PRS&&ms!=4) {												// �����}��ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==0) {
		m_RendPlane = (m_FChar&0x1) ? 1 : 0;							// [F5]:3�ʐ}�����ݸ�
		if(ObjectRendering()) {                                         // �����ݸޏ���
			SetRenderMode(2);
		}
		Display(0);                                                     // �ĕ`��
	} else if(ms==4||ms==9) {
		m_RendPlane = 0;
		SetRenderMode(0);
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}
