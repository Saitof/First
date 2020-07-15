/////////////////////////////////////////////////////////////////////////////
// ���̐��� �Ȑ������R�}���h��
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
static void SCurve(int* pn, int bc, int mx, double wt, PNTTYPE* pa)		//<<< �A���Ȑ�����
/*******************************************************************/
{
	PNTTYPE* rp = new PNTTYPE[View->m_MaxCPnt];
	int      rn, i, j, k;

	if(*pn<2) {															// ����_��2�������~
		delete[] rp;
		return;
	}
	rn = *pn;
	if(rn>2) {															// <����_��3�ȏ�>
		for(i=0; i<mx; i++) {											// �w��񐔕��J��
			for(k=0; k<DIM; k++) {										// ���ԓ_�擪�Ȑ��v�Z
				rp[1].p[k] = (pa[0].p[k]+pa[1].p[k])/2.0;
			}
			for(j=0; j<rn-2; j++) {
				for(k=0; k<DIM; k++) {									// �擪�ȊO�Ȑ��v�Z
					rp[2*j+3].p[k] = (pa[j+1].p[k]+pa[j+2].p[k])/2.0;
					rp[2*j+2].p[k] = (rp[2*j+1].p[k]+rp[2*j+3].p[k]+
                                      wt*pa[j+1].p[k])/(wt+2.0);
				}
			}
			if(bc) {													// <���E���̏ꍇ>
				for(k=0; k<DIM; k++) {									// �n�_�Ȑ��v�Z
					rp[0].p[k] = (rp[1].p[k]+rp[2*(rn-1)-1].p[k]+
                                  wt*pa[0].p[k])/(wt+2.0);
				}
				rp[2*(rn-1)] = rp[0];									// �I�_�͎n�_�Ɠ���
			} else {													// <���E���J�̏ꍇ>
				rp[0] = pa[0], rp[2*(rn-1)] = pa[rn-1];					// �n�_�I�_�ύX��
			}
			rn = 2*rn-1;												// �V����_�u��
			for(j=0; j<rn; j++) {
				pa[j] = rp[j];
			}
		}
	} else {															// <����_��2�ȉ�>
		for(i=0; i<mx; i++) {											// �w��񐔕��J��
			for(j=0; j<rn-1; j++) {
				rp[2*j] = pa[j];
				for(k=0; k<DIM; k++) {									// ���ԓ_�v�Z
					rp[2*j+1].p[k] = (pa[j].p[k]+pa[j+1].p[k])/2.0;
				}
			}
			rp[2*(rn-1)] = pa[rn-1];
			rn = 2*rn-1;												// �V����_�u��
			for(j=0; j<rn; j++) {
				pa[j] = rp[j];
			}
		}
	}
	*pn = rn;
	delete[] rp;
}

/************************************************************************/
int MakeSCurve(int* pn, int bc, int mx, double wt, PNTTYPE* pa, int* ea)//<<< S�Ȑ�����
/************************************************************************/
{
	PNTTYPE* wpa = new PNTTYPE[View->m_MaxCPnt];
	int*     wea = new int[View->m_MaxCPnt];
	PNTTYPE  spa;
	int      sea, cn, en, wn, sn, nn, i, j, cnt=0;

	if(*pn<2) {
		delete[] wpa;
		delete[] wea;
		return cnt;														// ����_��2�����͒��~
	}
	cn = *pn;
	if(bc) {															// <���E���̏ꍇ>
		for(i=0, en=0; i<cn; i++) {
			if(ea[i]==1) {												// �ܐ��_���m�F
				en++;
			}
		}
		if(en>0) {														// <�ܐ��_�L�̏ꍇ>
			while(1) {													// �擪���ܐ��_�܂ŌJ��
				if(ea[0]==1) {
					break;
				}
				spa = pa[0], sea = ea[0];								// �擪�ް��ۑ�
				for(i=0; i<cn-1; i++) {
					pa[i] = pa[i+1], ea[i] = ea[i+1];					// 1�O�ɼ��
				}
				pa[cn-1] = spa, ea[cn-1] = sea;							// �Ō�ɋ��擪�ݒ�
				cnt++;													// �ް���ĉ񐔶���
			}
		}
		pa[cn] = pa[0], ea[cn++] = ea[0];								// �Ō�ɐ擪�ǉ�
	} else {															// <���E���J�̏ꍇ>
		en = 2;															// �ܐ��_��2�ȏ�
		ea[0] = 1, ea[cn-1] = 1;										// �擪&�Ō�=�ܐ��_
	}
	if(en==0) {															// <�ܐ��_���̏ꍇ>
		for(i=0; i<cn; i++) {
			wpa[i] = pa[i];												// ��Ɨ̈搧��_�ۑ�
		}
		SCurve(&cn, bc, mx, wt, wpa);									// �A���Ȑ�����
		for(i=0; i<cn; i++) {
			wea[i] = 0;													// �S�ċȐ��_Ӱ��
		}
	} else {															// <�ܐ��_�L�̏ꍇ>
		wn = 0, sn = 0;
		while(1) {
			for(i=sn, nn=0; i<cn; i++) {								// �擪���珇���s��
				wpa[wn+(nn++)] = pa[i];									// 2�_�̐ܐ��_�Ԃ�
				if(i!=sn&&ea[i]==1) {									// ����_���o
					sn = i; break;
				}
			}
			if(i==cn) {													// �Ō�ŏI��
				break;
			}
			SCurve(&nn, 0, mx, wt, &wpa[wn]);							// �A���Ȑ�����
			for(i=wn+1; i<wn+nn-1; i++) {								// ���ԓ_�͋Ȑ��_Ӱ��
				wea[i] = 0;
			}
			wn = wn+nn-1;												// �ܐ��_�͑O��ŋ��ʎg�p
			wea[wn] = 1;												// �Ō�_�͐ܐ��_Ӱ��
		}
		cn = wn+1;
	}
	for(i=0; i<cn; i++) {
		pa[i] = wpa[i];													// ��Ɨ̈��萳�K�ݒ�
	}
	for(i=1; i<cn-1; i++) {
		if(wea[i]!=1) {
			for(j=0; j<DIM; j++) {										// �Ȑ��_����
				pa[i].p[j] = (wpa[i-1].p[j]+wpa[i+1].p[j]+
                              (wt+2.0)*wpa[i].p[j])/(wt+4.0);
			}
		}
	}
	if(bc&&en==0) {														// <���E:�őS�Ȑ��̏ꍇ>
		for(j=0; j<DIM; j++) {											// 2�_��&�ŏI-1�_�ڂŎ���
			pa[0].p[j] = (wpa[cn-2].p[j]+wpa[1].p[j]+
                          (wt+2.0)*wpa[0].p[j])/(wt+4.0);
		}
		pa[cn-1] = pa[0];												// �n�_�ƏI�_�͓���
	}
	*pn = cn;
	delete[] wpa;
	delete[] wea;
	return cnt;															// �ް���ĉ񐔂�Ԃ�
}

/************************/
OBJTYPE* MakeCurve(void)												//<<< �Ȑ����������
/************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc, mx, wt;
	double   dwt;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	pn = GetParaInt(cmp, 0);											// ����_���擾
	bc = GetParaInt(cmp, 1);											// ���EӰ�ގ擾
	mx = GetParaInt(cmp, 2);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 3);											// �Ȑ��d�݌W��
	for(i=0; i<pn; i++) {
		ea[i] = GetParaInt(cmp, 4+i);									// ײ�Ӱ�ގ擾
		GetParaPnt(cmp, i, &pa[i]);										// ����_�擾
	}
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	MakeSCurve(&pn, bc, mx, dwt, pa, ea);								// S�Ȑ�����
	CreatObj(pn, pn, 0, &op);											// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()) {												// <<<����ގ��s��~�Ȃ�>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// ���_���W�ݒ�
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// ��ٰ�ߏ��ݒ�
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// ����No�ݒ�(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ea;
	return op;
}

/**************************/
OBJTYPE* ChangeCurve(void)												//<<< �Ȑ��ҏW�����
/**************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc, mx, wt;
	double   dwt;

	GetObjPtr(&op);														// �����Ώۗ��̎擾
	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	pn = GetParaInt(cmp, 0);											// ����_���擾
	bc = GetParaInt(cmp, 1);											// ���EӰ�ގ擾
	mx = GetParaInt(cmp, 2);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 3);											// �Ȑ��d�݌W��
	for(i=0; i<pn; i++) {
		ea[i] = GetParaInt(cmp, 4+i);									// ײ�Ӱ�ގ擾
		GetParaPnt(cmp, i, &pa[i]);										// ����_�擾
	}
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	MakeSCurve(&pn, bc, mx, dwt, pa, ea);								// S�Ȑ�����
	AppendCmd(op, cmp);													// ����&����ސڑ�
	ChangeObj(op, pn, pn, 0);											// ���̗̈�ύX
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <<<����ގ��s��~�Ȃ�>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// ���_���W�ݒ�
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// ��ٰ�ߏ��ݒ�
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// ����No�ݒ�(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ea;
	return op;
}

/***********************/
OBJTYPE* SetCurve(void)													//<<< �Ȑ�����_���������
/***********************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA*2];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, bc;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	pn = GetParaInt(cmp, 0);											// ����_���擾
	bc = GetParaInt(cmp, 1);											// ���EӰ�ގ擾
	for(i=0; i<pn; i++) {
		GetParaPnt(cmp, i, &pa[i]);										// ����_�擾
	}
	if(bc) {															// ���E:�͏I�_���n�_
		pa[pn++] = pa[0];
	}
	CreatObj(pn, pn, 0, &op);											// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()) {												// <<<����ގ��s��~�Ȃ�>>>
		for(i=0; i<pn; i++) {
			PutVtx(op, i, pa[i]);										// ���_���W�ݒ�
		}
		for(i=0; i<pn-1; i++) {
			PutFlp1(op, i, i, 0);										// ��ٰ�ߏ��ݒ�
		}
		if(pn>0) {
			PutFlp1(op, pn-1, pn-1, 1);
		}
	}
	if(bc) {															// ����No�ݒ�(CURVE)
		PutObjAtr(op, 10);
	} else {
		PutObjAtr(op, 11);
	}
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
		OnEdtFlg(1, op);												// edtflg:ON
	}
	delete[] pa;
	return op;
}
