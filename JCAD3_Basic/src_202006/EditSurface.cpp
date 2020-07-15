/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW���암
// EditSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "CreatCurvePoint.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/********************************************************************/
void GetSurfCmd1(CMDTYPE* cmp, int* un, int* vn, int* ub, int* vb,
                 int* rp, double* wt, int* an, PNTTYPE* pa, int* ia)	//<<< �Ȗʺ���ގ擾1
/********************************************************************/
{
	int i;

	*un = GetParaInt(cmp, 0);											// U����_���擾
	*vn = GetParaInt(cmp, 1);											// V����_���擾
	*ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	*vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	*rp = GetParaInt(cmp, 4);											// �Ȑ��������擾
	*wt = (double)GetParaInt(cmp, 5)/1000.0;							// �Ȑ��d�݌W���擾
	*an = (*un)*(*vn);													// ������_��
	for(i=0; i<(*an); i++) {
		GetParaPnt(cmp, i, &pa[i]);										// ����_�擾
		ia[i] = GetParaInt(cmp, 7+i);									// ײ�Ӱ�ގ擾
	}
}

/*********************************************************************/
void GetSurfCmd2(CMDTYPE* cmp, int atr, int* md, int* un, int* vn,
                 int* ub, int* vb, int* rp, double* wt, int* an,
                 PNTTYPE* pa, int* ia, int* pl, PNTTYPE* pb, int* ib)	//<<< �Ȗʺ���ގ擾2
/*********************************************************************/
{
	int i;

	*md = (atr==20||atr==21) ? 1 : 0;									// �Ȗ�/�|���Ȗ�
	*un = GetParaInt(cmp, 0);											// U����_���擾
	*vn = GetParaInt(cmp, 1);											// V����_���擾
	*ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	*vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	*rp = GetParaInt(cmp, 4);											// �Ȑ��������擾
	*wt = (double)GetParaInt(cmp, 5)/1000.0;							// �Ȑ��d�݌W���擾
	*an = (*un)*(*vn);													// ������_��
	if(*md) {															// <�ʏ�Ȗ�>
		for(i=0; i<(*an); i++) {
			GetParaPnt(cmp, i, &pa[i]);									// ����_�擾
			ia[i] = GetParaInt(cmp, 7+i);								// ײ�Ӱ�ގ擾
		}
	} else {															// <�|���Ȗ�>
		*pl = GetParaInt(cmp, 7);										// �|�����ʎ擾
		for(i=0; i<(*un)+(*vn); i++) {
			GetParaPnt(cmp, i, &pb[i]);									// ����_�擾
			ib[i] = GetParaInt(cmp, 8+i);								// ײ�Ӱ�ގ擾
		}
	} 
}

/****************************************************************/
void ExecSurface(int md, OBJTYPE* op1, int un, int vn, int ub,
                 int vb, int rp, double wt, int an, PNTTYPE* pa,
                 int* ia, PNTTYPE* pb, OBJTYPE** op2)					//<<< �ʏ�Ȗʎ��s
/****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	ChgSurface(NULL, un, vn, ub, vb, rp, wt, pa, ia);					// �ȖʕύX(����)
	CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);			// ����_=>�ʉߓ_�ϊ�
	SetSurface(un, vn, ub, vb, pb);										// �Ȗʐ���_����(����)
	GetObjPtr(&op);														// ����_OP�ۑ�
	SetSelNo(op, 2);													// ���F�\��OP�ݒ�
	BaseCmdPtr(1, op1, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �ȖʕҏW������
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)!=THICKNESS) {									// <���݂Â��ȊO�̏ꍇ>
			RedoCommandCopy(op, cmp);									// ����ޕ��ʎ��s
		}
		NextCmdPtr(&cmp);												// ������ނ�
	}
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// ���̕ҏW�㐧��_�擾
	}
	View->SetDispList(op);												// �ި���ڲؽēo�^
	*op2 = op;
}

/********************************************************************/
void ExecSweepSurf(OBJTYPE* op1, int un, int vn, int ub, int vb,
                   int rp, double wt, int an, int pl, PNTTYPE* ps,
                   int* is, PNTTYPE* pa, PNTTYPE* pb, OBJTYPE** op2)	//<<< �|���Ȗʎ��s
/********************************************************************/
{
	int*     wk = new int[View->m_MaxSCtA];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	ChgSweepSurf(NULL, un, vn, ub, vb, rp, wt, ps, is);					// �|���ȖʕύX(����)
	SetSweepCtrl(pl, un, vn, ub, ps, is, pa, wk);						// �|���Ȗʐ���_�ϊ�
	SetSurface(un, vn, ub, vb, pa);										// �|���Ȗʐ���_����(����)
	GetObjPtr(&op);														// ����_OP�ۑ�
	SetSelNo(op, 2);													// ���F�\��OP�ݒ�
	BaseCmdPtr(1, op1, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �ȖʕҏW������
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)!=THICKNESS) {									// <���݂Â��ȊO�̏ꍇ>
			RedoCommandCopy(op, cmp);									// ����ޕ��ʎ��s
		}
		NextCmdPtr(&cmp);												// ������ނ�
	}
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// ���̕ҏW�㐧��_�擾
	}
	View->SetDispList(op);												// �ި���ڲؽēo�^
	*op2 = op;
	delete[] wk;
}

/***************************/
void AttachObj(OBJTYPE* op)												//<<< ����ލĐڑ�/�Ď��s
/***************************/
{
	AttachCmd(op);														// ����ލĐڑ�
	RedoCommand(op);													// ����ލĎ��s
	View->SetDispList(op);												// �ި���ڲؽēo�^
}

/*************************************************************/
void PickPointMove(int md, int pn, int* pk, int un, int vn,
                   int ub, int vb, double wt, int an, int f1,
                   int f2, int sf, OBJTYPE* op, PNTTYPE* pa,
                   PNTTYPE* pb, int* ia, int* ib, int* ic)				//<<< ����_�ړ� �_�I��
/*************************************************************/
{
	int i;

	for(i=0; i<pn; i++) {
		PickCPoint(pb[pk[i]], pk[i], an, pb, ub, vb, un, vn,
                   f1, f2, sf, ia, ib);									// �Ώې���_�I��
	}
	SetLineNo1(un, vn, f1, f2, ib);										// ײݔԍ��ݒ�1
	RedoChgSurface(un, vn, ub, vb, pa, ia);								// �Ȗʐ���
	CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);			// ����_=>�ʉߓ_�ϊ�
	RedoSetSurface(un, vn, ub, vb, pb);									// �Ȗʐ���_����
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// ���̕ҏW�㐧��_�擾
		ic[i] = (!ib[i]) ? ia[i] : 4;
	}
}

/********************************************************************/
void PickLineMove(int md, int pn, int* pk, int sw, int un, int vn,
                  int ub, int vb, double wt, int an, int f1, int f2,
                  int sf, int pl, int* tn, OBJTYPE* op, PNTTYPE* pa,
                  PNTTYPE* pb, PNTTYPE* ps, PNTTYPE* pw,
                  int* ia, int* ib, int* ic, int* is, int* iw)			//<<< ����_��ړ� ��I��
/********************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, no;

	for(i=0; i<pn; i++) {
		no = pk[i];														// �I��ԍ�
		iw[no] = (iw[no]) ? 0 : 1;										// �I����Ԕ��]
		if(sw) {														// <�ʏ�Ȗ�>
			PickCLine(no, ub, vb, un, vn, f1, f2, sf, pb, ia, ib);		// �Ώې���_��I��
		} else {														// <�|���Ȗ�>
			if(sf) {
				if(no<vn) {
					is[un+no] = (is[un+no]) ? 0 : 1;					// ���]
				} else {
					is[no-vn] = (is[no-vn]) ? 0 : 1;
				}
				if(!ub) {
					is[0] = 1, is[un-1] = 1;
				}
				if(!vb) {
					is[un] = 1, is[un+vn-1] = 1;
				}
			}
		}
	}
	SetLineNo2(vn, *tn, f1, f2, iw);									// ײݔԍ��ݒ�2
	if(sw) {															// <�ʏ�Ȗ�>
		RedoChgSurface(un, vn, ub, vb, pa, ia);							// �ȖʕύX
		CalcSurfacePoint(md, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);		// ����_=>�ʉߓ_�ϊ�
	} else {															// <�|���Ȗ�>
		RedoChgSweepSurf(un, vn, ub, vb, ps, is);						// �|���ȖʕύX
		SetSweepCtrl(pl, un, vn, ub, ps, is, pb, wk);					// �|���Ȗʐ���_�ϊ�
	}
	RedoSetSurface(un, vn, ub, vb, pb);									// �Ȗʐ���_����
	for(i=0; i<an; i++) {
		GetVtx(op, i, &pb[i]);											// ���̕ҏW�㐧��_�擾
	}
	TrackerCLine(md, sw, un, vn, pb, f1, f2, tn, pw, ic);				// �_���ׯ���擾
	for(i=0; i<*tn; i++) {
		ic[i] = (iw[i]) ? 4 : ic[i];									// �w��_�F�ύX
	}
	delete[] wk;
}

/************************************************************/
BOOL CheckLineCopy(int md, int pn, int* pk, int un, int vn,
				   int ub, int vb, int vf, int* cn, int* bn)			//<<< ����_�񕡎�����
/************************************************************/
{
	int i, mx, ct1=0, ct2=0, err=0;

	if(md) {															// <<�ʏ�Ȗʂ̏ꍇ>>
		mx = 2*un+vn;													// �ő�}���ʒuNo
		mx = (!ub) ? mx-1 : mx;											// U���J�͌��Z
		mx = (!vb) ? mx-1 : mx;											// V���J�͌��Z
		for(i=0; i<pn; i++) {
			if(pk[i]<mx) {												// �}���w��������
				ct1++;
			} else {													// ���ʎw��������
				ct2++;
			}
			err = (vf&&(pk[i]==0||pk[i]==2*un-2)) ? 1 : err;			// V�����ŗ��[�_�ʹװ
		}
		if(err==0&&ct1==0&&ct2==1) {									// <���ʎw��:1�̏ꍇ>
			*cn = pk[0]-mx;
			CSetBaseDlg4 dlg;
			dlg.SetValue(*bn);											// �����l�\��
			if(dlg.DoModal()==IDOK) {									// �����޲�۸ޕ\��
				dlg.GetValue(bn);										// ��_�擾
			} else {
				*cn = -1;
			}
		} else if(ct2>1) {
			*cn = -1;
		}
		if(err==1||ct2>0) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

/**********************************************************************/
void PickLineInsert(int md, int pn, int* pk, int* un, int* vn, int ub,
                    int vb, int* an, int uf, int sf, int pl, int* cn,
                    int bn, OBJTYPE* op, PNTTYPE cpt, PNTTYPE* pa,
                    PNTTYPE* pb, PNTTYPE* ps, int* ia, int* is)			//<<< ����_��}�� ��I��
/**********************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, j, no, i1, i2, i3;

	for(i=pn-1; i>=0; i--) {
		if(md) {														// <<�ʏ�Ȗ�>>
			no = pk[i], i1 = 2*(*un), i2 = *un, i3 = *vn;
			i1 = (!ub) ? i1-1 : i1;										// U���J�͌��Z
			if(no<i1) {													// <U�����w��̏ꍇ>
				i2 = (no%2==1) ? i2+1 : i2;
			} else {													// <V�����w��̏ꍇ>
				i3++;
			}
		} else {														// <<�|���Ȗ�>>
			no = pk[i], i1 = *un, i2 = *un, i3 = *vn;
			i1 = (!ub) ? i1-1 : i1;										// U���J�͌��Z
			if(no<i1) {													// <U�����w��̏ꍇ>
				i2++;
			} else {													// <V�����w��̏ꍇ>
				i3++;
			}
		}
		if(i2>View->m_MaxCCtl||i3>View->m_MaxCCtl||
           i2*i3>View->m_MaxSCtl) {										// <<����_�������>>
			AfxMessageBox(IDS_MANY_CTRLP);								// �װү����
		} else {														// <<����_����ȉ�>>
			if(md) {													// <�ʏ�Ȗ�>
				InsertCLine(no, sf, ub, un, vn, uf,cn,bn,cpt,pa,ia);	// �Ώې���_��}��
				RedoChgSurface(*un, *vn, ub, vb, pa, ia);				// �ȖʕύX
			} else {													// <�|���Ȗ�>
				InsertSweep(no, sf, ub, un, vn, ps, is);				// �|������_��}��
				RedoChgSweepSurf(*un, *vn, ub, vb, ps, is);				// �|���ȖʕύX
				SetSweepCtrl(pl, *un, *vn, ub, ps, is, pa, wk);			// �|���Ȗʐ���_�ϊ�
			}
			*an = (*un)*(*vn);											// ������_��
			RedoSetSurface(*un, *vn, ub, vb, pa);						// �Ȗʐ���_����
			for(j=0; j<*an; j++) {
				GetVtx(op, j, &pb[j]);									// ���̕ҏW�㐧��_�擾
			}
		}
	}
	delete[] wk;
}

/********************************************************************/
BOOL PickLineDivide(int md, int pn, int* pk, int un, int vn, int ub,
                    int vb, int rp, double wt, int pl, OBJTYPE* op,
                    PNTTYPE* pa, PNTTYPE* ps, int* ia, int* is)			//<<< ����_�񕪊�
/********************************************************************/
{
	OBJTYPE* cop;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE* wp1 = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* wp2 = new PNTTYPE[View->m_MaxSCtA];
	int*     wi1 = new int[View->m_MaxSCtA];
	int*     wi2 = new int[View->m_MaxSCtA];
	char*    name = new char[24];
	int      i, j, u1, u2, v1, v2, uv, no, ct, sm;
	BOOL     ret=TRUE;

	no = pk[0];
	if(md) {															// <<�ʏ�Ȗ�>>
		u1 = (!ub) ? 2*un-1 : 2*un;										// U���J�͌��Z
		if(no<u1) {														// <U�����w��̏ꍇ>
			if(no%2==1) {
				no = (no+1)/2, uv = 0;
			} else {
				ret = FALSE;
			}
		} else {														// <V�����w��̏ꍇ>
			no = no-u1+1, uv = 1;
		}
	} else {															// <<�|���Ȗ�>>
		u1 = (!ub) ? un-1 : un;											// U���J�͌��Z
		if(no<u1) {														// <U�����w��̏ꍇ>
			no = no+1, uv = 0;
		} else {														// <V�����w��̏ꍇ>
			no = no-u1+1, uv = 1;
		}
	}
	if(ret) {															// <<<�L���Ȏw��̏ꍇ>>>
		u1 = 0, v1 = 0, u2 = 0, v2 = 0;
		if(md) {														// <<�ʏ�Ȗ�>>
			if(!uv) {													// <U�����w��̏ꍇ>
				if(ub) {												// <U�̏ꍇ>
					u1 = un, v1 = vn, ub = 0, ct = 0;
					for(j=0; j<vn; j++) {
						for(i=no; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// �ؒf�ʒu�ȍ~�𕡎�
							if(i==no) {
								wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;	// �ؒf�[�͐ܐ��_
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
						for(i=0; i<no; i++) {
							wp1[ct] = pa[un*j+i];						// �A�����Đؒf�ʒu���O�𕡎�
							if(i==no-1) {
								wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;	// �ؒf�[�͐ܐ��_
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
				} else {												// <U�J�̏ꍇ>
					if(no<2&&un-no<2) {									// <�ؒf�ʒu�O�オ����_���s��>
						AfxMessageBox(IDS_FEW_CTRLP);					// �װү����
						ret = FALSE;
					} else if(no>=2&&un-no<2) {							// <�ؒf�ʒu�O���L��>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=0; i<no; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu���O�𕡎�
								if(i==no-1) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else if(no<2&&un-no>=2) {							// <�ؒf�ʒu�オ�L��>
						u1 = un-no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=no; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu�ȍ~�𕡎�
								if(i==no) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else {											// <�ؒf�ʒu�O�オ�L��>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=0; i<no; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu���O�𕡎�
								if(i==no-1) {
									wi1[ct++] = (int)(ia[un*j+i]/2)*2+1;// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						u2 = un-no, v2 = vn, ub = 0, ct = 0;
						for(j=0; j<vn; j++) {
							for(i=no; i<un; i++) {
								wp2[ct] = pa[un*j+i];					// �ؒf�ʒu�ȍ~�𕡎�
								if(i==no) {
									wi2[ct++] = (int)(ia[un*j+i]/2)*2+1;// �ؒf�[�͐ܐ��_
								} else {
									wi2[ct++] = ia[un*j+i];
								}
							}
						}
					}
				}
			} else {													// <V�����w��̏ꍇ>
				if(vb) {												// <V�̏ꍇ>
					u1 = un, v1 = vn, vb = 0, ct = 0;
					for(j=no; j<vn; j++) {
						for(i=0; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// �ؒf�ʒu�ȍ~�𕡎�
							if(j==no) {
								wi1[ct++] = ia[un*j+i]%2+2;				// �ؒf�[�͐ܐ��_
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
					for(j=0; j<no; j++) {
						for(i=0; i<un; i++) {
							wp1[ct] = pa[un*j+i];						// �A�����Đؒf�ʒu���O�𕡎�
							if(j==no-1) {
								wi1[ct++] = ia[un*j+i]%2+2;				// �ؒf�[�͐ܐ��_
							} else {
								wi1[ct++] = ia[un*j+i];
							}
						}
					}
				} else {												// <V�J�̏ꍇ>
					if(no<3&&vn-no<3) {									// <�ؒf�ʒu�O�オ����_���s��>
						AfxMessageBox(IDS_FEW_CTRLP);					// �װү����
						ret = FALSE;
					} else if(no>=3&&vn-no<3) {							// <�ؒf�ʒu�O���L��>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(j=0; j<no; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu���O�𕡎�
								if(j==no-1) {
									wi1[ct++] = ia[un*j+i]%2+2;			// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else if(no<3&&vn-no>=3) {							// <�ؒf�ʒu�オ�L��>
						u1 = un, v1 = vn-no, vb = 0, ct = 0;
						for(j=no; j<vn; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu�ȍ~�𕡎�
								if(j==no) {
									wi1[ct++] = ia[un*j+i]%2+2;			// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else {											// <�ؒf�ʒu�O�オ�L��>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(j=0; j<no; j++) {
							for(i=0; i<un; i++) {
								wp1[ct] = pa[un*j+i];					// �ؒf�ʒu���O�𕡎�
								if(j==no-1) {
									wi1[ct++] = ia[un*j+i]%2+2;			// �ؒf�[�͐ܐ��_
								} else {
									wi1[ct++] = ia[un*j+i];
								}
							}
						}
						u2 = un, v2 = vn-no, vb = 0, ct = 0;
						for(j=no; j<vn; j++) {
							for(i=0; i<un; i++) {
								wp2[ct] = pa[un*j+i];					// �ؒf�ʒu�ȍ~�𕡎�
								if(j==no) {
									wi2[ct++] = ia[un*j+i]%2+2;			// �ؒf�[�͐ܐ��_
								} else {
									wi2[ct++] = ia[un*j+i];
								}
							}
						}
					}
				}
			}
		} else {														// <<�|���Ȗ�>>
			if(!uv) {													// <U�����w��̏ꍇ>
				if(ub) {												// <U�̏ꍇ>
					u1 = un, v1 = vn, ub = 0, ct = 0;
					for(i=no; i<un; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// �ؒf�ʒu�ȍ~�𕡎�
					}
					for(i=0; i<no; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// �A�����Đؒf�ʒu���O�𕡎�
					}
					wi1[0] = 1, wi1[ct-1] = 1;							// �ؒf�̗��[�͐ܐ��_
					for(i=0; i<vn; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// V������_��
					}
				} else {												// <U�J�̏ꍇ>
					if(no<2&&un-no<2) {									// <�ؒf�ʒu�O�オ����_���s��>
						AfxMessageBox(IDS_FEW_CTRLP);					// �װү����
						ret = FALSE;
					} else if(no>=2&&un-no<2) {							// <�ؒf�ʒu�O���L��>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// �ؒf�ʒu���O�𕡎�
						}
						wi1[ct-1] = 1;									// �ؒf�[�͐ܐ��_
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V������_��
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else if(no<2&&un-no>=2) {							// <�ؒf�ʒu�オ�L��>
						u1 = un-no, v1 = vn, ub = 0, ct = 0;
						for(i=no; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// �ؒf�ʒu�ȍ~�𕡎�
						}
						wi1[0] = 1;										// �ؒf�[�͐ܐ��_
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V������_��
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else {											// <�ؒf�ʒu�O�オ�L��>
						u1 = no, v1 = vn, ub = 0, ct = 0;
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// �ؒf�ʒu���O�𕡎�
						}
						wi1[ct-1] = 1;									// �ؒf�[�͐ܐ��_
						for(i=0; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// V������_��
						}
						u2 = un-no, v2 = vn, ub = 0, ct = 0;
						for(i=no; i<un; i++) {
							wp2[ct] = ps[i], wi2[ct++] = is[i];			// �ؒf�ʒu�ȍ~�𕡎�
						}
						wi1[0] = 1;										// �ؒf�[�͐ܐ��_
						for(i=0; i<vn; i++) {
							wp2[ct] = ps[i+un], wi2[ct++] = is[i+un];	// V������_��
						}
					}
				}
			} else {													// <V�����w��̏ꍇ>
				if(vb) {												// <V�̏ꍇ>
					u1 = un, v1 = vn, vb = 0, ct = 0;
					for(i=0; i<un; i++) {
						wp1[ct] = ps[i], wi1[ct++] = is[i];				// U������_��
					}
					for(i=no; i<vn; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// �ؒf�ʒu�ȍ~�𕡎�
					}
					for(i=0; i<no; i++) {
						wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];		// �A�����Đؒf�ʒu���O�𕡎�
					}
					wi1[un] = 1, wi1[ct-1] = 1;							// �ؒf�̗��[�͐ܐ��_
				} else {												// <V�J�̏ꍇ>
					if(no<3&&vn-no<3) {									// <�ؒf�ʒu�O�オ����_���s��>
						AfxMessageBox(IDS_FEW_CTRLP);					// �װү����
						ret = FALSE;
					} else if(no>=3&&vn-no<3) {							// <�ؒf�ʒu�O���L��>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U������_��
						}
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// �ؒf�ʒu���O�𕡎�
						}
						wi1[ct-1] = 1;									// �ؒf�[�͐ܐ��_
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else if(no<3&&vn-no>=3) {							// <�ؒf�ʒu�オ�L��>
						u1 = un, v1 = vn-no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U������_��
						}
						for(i=no; i<vn; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// �ؒf�ʒu�ȍ~�𕡎�
						}
						wi1[un] = 1;									// �ؒf�[�͐ܐ��_
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else {											// <�ؒf�ʒu�O�オ�L��>
						u1 = un, v1 = no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp1[ct] = ps[i], wi1[ct++] = is[i];			// U������_��
						}
						for(i=0; i<no; i++) {
							wp1[ct] = ps[i+un], wi1[ct++] = is[i+un];	// �ؒf�ʒu���O�𕡎�
						}
						wi1[ct-1] = 1;									// �ؒf�[�͐ܐ��_
						u2 = un, v2 = vn-no, vb = 0, ct = 0;
						for(i=0; i<un; i++) {
							wp2[ct] = ps[i], wi2[ct++] = is[i];			// U������_��
						}
						for(i=no; i<vn; i++) {
							wp2[ct] = ps[i+un], wi2[ct++] = is[i+un];	// �ؒf�ʒu�ȍ~�𕡎�
						}
						wi1[un] = 1;									// �ؒf�[�͐ܐ��_
					}
				}
			}
		}
	}
	if(ret) {															// <<<�L���Ȏw��̏ꍇ>>>
		if(md) {														// <<�ʏ�Ȗ�>>
			if(u1>0&&v1>0) {											// <Redo:OK>
				RedoChgSurface(u1, v1, ub, vb, wp1, wi1);				// �ȖʕύX
			}
			if(u2>0&&v2>0) {											// <Create:OK>
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				sm = GetParaInt(cmp, 6);								// �د��Ӱ�ގ擾
				CrtSurface(u2, v2, ub, vb, rp, wt, sm, wp2, wi2);		// �Ȗʐ���
				GetObjPtr(&cop);										// ����_OP�ۑ�
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(cop, cmp);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				View->SetDispList(cop);									// �ި���ڲ�Eؽēo�^
				OnEdtFlg(1, cop);										// edtflg:ON
				GetObjOif(op, &oif);									// ���̍\������߲���擾
				GetOifName(oif, name);									// ���̍\�����擾
				GetObjOif(cop, &oif);									// ���̍\������߲���擾
				SetOifName(0, oif, name);								// ���̍\�����ݒ�
			}
		} else {														// <<�|���Ȗ�>>
			if(u1>0&&v1>0) {											// <Redo:OK>
				RedoChgSweepSurf(u1, v1, ub, vb, wp1, wi1);				// �|���ȖʕύX
			}
			if(u2>0&&v2>0) {											// <Create:OK>
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				sm = GetParaInt(cmp, 6);								// �د��Ӱ�ގ擾
				CrtSweepSurf(u2, v2, ub, vb, rp, wt, sm, pl, wp2, wi2);	// �|���Ȗʐ���
				GetObjPtr(&cop);										// ����_OP�ۑ�
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(cop, cmp);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				View->SetDispList(cop);									// �ި���ڲ�Eؽēo�^
				OnEdtFlg(1, cop);										// edtflg:ON
				GetObjOif(op, &oif);									// ���̍\������߲���擾
				GetOifName(oif, name);									// ���̍\�����擾
				GetObjOif(cop, &oif);									// ���̍\������߲���擾
				SetOifName(0, oif, name);								// ���̍\�����ݒ�
			}
		}
	}
	delete[] wp1;
	delete[] wp2;
	delete[] wi1;
	delete[] wi2;
	delete[] name;
	return ret;
}

/*********************************************************/
void PickSurfLine(int pn, int* pk, int fg, int tn,
                  OBJTYPE* op, int* ib, int* iw, int* bn)				//<<< ����_��I��
/*********************************************************/
{
	int i, j;

	for(i=0; i<pn; i++) {
		if(ib[pk[i]]) {													// �I���ς͉���
			ib[pk[i]] = 0, iw[pk[i]] = 2;
		} else {														// ���I���͑I��
			ib[pk[i]] = 1, iw[pk[i]] = 4;
		}
	}
	SetLineNo3(tn, fg, ib, op);											// ײݔԍ��ݒ�3
	for(i=0, j=-1; i<tn; i++) {
		if(ib[i]) {														// �ŏ�No�̎w���ς�
			j = i; break;
		}
	}
	if(*bn==-1&&j!=-1) {												// ��_No�ݒ�
		*bn = (fg%2) ? j+1 : j;
	}
}

/***************************************************************/
static void GetSPoint(int fg, int ix, int un, int vn, int vflg,
                      PNTTYPE scp, PNTTYPE* pa, int* ia,
                      PNTTYPE* pb, int* ib, int* ta, int* tt,
                      PNTTYPE* cpt, VECTYPE* vec, int* plane)			//<<< �f�ʐ���_���擾
/***************************************************************/
{
	VECTYPE nvc, rvc;
	int     i, j, i1, i2, i3, vd;
	double  rot[3][3], rth, dd;

	if(vflg%2==1) {														// <V�n�_����̏ꍇ>
		ix++;
	}
	for(i=0; i<vn; i++) {												// <<V�����ׯ��>>
		pb[i] = pa[ix+i*un];											// ����_���W���o
		ib[i] = ia[ix+i*un]/2;											// �Ȑ�Ӱ�ޒ��o
		tt[i] = ta[ix+i*un];											// ���͌����񒊏o
	}
	CalcCent(pb, vn, cpt);												// V�f�ʒ��S�擾
	CalcNVec(pb, vn, &nvc);												// V�f�ʖ@���޸�َ擾
	*vec = nvc;
	for(i=0, vd=0, dd=0.0; i<DIM; i++) {
		if(dd<fabs(nvc.p[i])) {											// ��f���޸�ٍő��������
			dd = fabs(nvc.p[i]), vd = i;
		}
	}
	*plane = (vd+1>2) ? 0 : vd+1;										// �f�ʑ��ݕ���
	i1 = (vd+1>2) ? 0 : vd+1;											// ���W���ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pb[i].p[j] -= cpt->p[j];									// �f�ʌ��_�ړ�
		}
	}
	rth = sqrt(nvc.p[i1]*nvc.p[i1]+nvc.p[i2]*nvc.p[i2]);				// ��ʕ�������
	if(rth>EPSILON) {													// <��ʂɂȂ��ꍇ>
		rvc.p[i1] = -nvc.p[i2], rvc.p[i2] = nvc.p[i1], rvc.p[i3] = 0.0;
		VecN(&rvc);
		rth = atan2(rth, nvc.p[i3]);									// ��]�p�x�Z�o
		rth = (rth<-10||rth>10) ? 0.0 : rth;
		rth = -rth;
		rth = (rth<-PI/2) ? rth-PI : rth;
		SetRotateMatrix(rvc, rth, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {											// ��ʂɉ�]
			VecRotate(&pb[i], rot);
		}
	}
	if(fg) {															// <���̏���>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] += scp.p[j];									// �f�ʕҏW�ʒu�ړ�
			}
		}
	} else {															// <���̔����>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] += cpt->p[j];								// �f�ʕҏW�ʒu�ړ�
			}
		}
	}
}

/************************************************************************/
void PickSPointMove1(int no, int md, double* da, double* db, int m1,
                     int m2, int un, int vn, int vb, int rp, double wt,
                     int fg, int pl, int* vl, PNTTYPE* pb, PNTTYPE* ps,
                     PNTTYPE* pz, PNTTYPE* pw, PNTTYPE* p1, VECTYPE* p2,
                     PNTTYPE* p3, int* ia, int* is, int* iz, int* iw,
                     int* iy, int* t1, int* t2, OBJTYPE* op1,
                     OBJTYPE* op2, OBJTYPE** op3, OBJTYPE** op4)		//<<< �f�ʐ���_�ړ� ��I��
/************************************************************************/
{
	int i, j;

	View->GetCentPnt(p3);												// ��ʒ��S�擾
	if(m2) {															// <�����̏����F�L>
		OffEdtFlg(1, op1);												// �Ȗʗ��̗��̏���
		OffDspFlg(1, op1);
		OffEdtFlg(1, op2);												// �Ȗʐ���_���̏���
		OffDspFlg(1, op2);
	} else {															// <�����̏����F��>
		OffEdtFlg(1, op1);												// �Ȗʗ��̗��̑Ώۉ���
		OffEdtFlg(1, op2);												// �Ȗʐ���_���̏���
		OffDspFlg(1, op2);
	}
	if(m1) {															// <�ʏ�Ȗ�>
		GetSPoint(m2, no, un, vn, fg, *p3, pb, ia, pw, iz,
                  t1, t2, p1, p2, vl);									// �f�ʐ���_���擾
	} else {															// <�|���Ȗ�>
		for(i=0; i<vn; i++) {
			pw[i] = ps[un+i], iz[i] = is[un+i];							// U�f�ʏ�񒊏o
			for(j=0; j<DIM; j++) {
				pw[i].p[j] += p3->p[j];
			}
		}
		if(pl>-1) {														// ���|���Ȗʂ̏ꍇ
			*vl = (pl-1<0) ? 2 : pl-1;									// U�f�ʕ���
		} else {														// �V�|���Ȗʂ�YZ���ʌŒ�
			*vl = 1;
		}
	}
	SetCurveMatrix3(md, wt, vb, vn, iz, da, db);						// ����_<=>�ʉߓ_�ϊ���د��
	CalcCurvePoint(md, vn, pw, pz, db);									// �ʉߓ_=>����_�ϊ�
	CrtCurve(vn, vb, rp, wt, pz, iz);									// �f�ʋȐ�����
	GetObjPtr(op3);														// �Ȑ�OP�ۑ�
	OnEdtFlg(1, *op3);													// �Ȑ����̕ҏW�Ώېݒ�
	SetCurve(vn, vb, pw);												// ����_���̐���
	GetObjPtr(op4);														// ����_OP�ۑ�
	SetSelNo(*op4, 2);													// ���F�\��OP�ݒ�
	for(i=0; i<vn; i++) {												// �ׯ���F�ݒ�/�w���׸ޏ�����
		iw[i] = iz[i]*2, iy[i] = 0;
	}
}

/*********************************************************************/
void PickSPointMove2(int pn, int* pk, int md, double* da, double* db,
                     int vn, int vb, double wt, int sf, PNTTYPE* pz,
                     PNTTYPE* pw, int* iz, int* iw, int* iy)			//<<< �f�ʐ���_�ړ� �_�I��
/*********************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	int      i;

	for(i=0; i<pn; i++) {
		iy[pk[i]] = (iy[pk[i]]) ? 0 : 1;								// �I����Ԕ��]
		if(sf) {
			iz[pk[i]] = (iz[pk[i]]) ? 0 : 1;							// �ܐ�/�Ȑ����]
		}
	}
	CalcCurvePoint(md, vn, pz, pt, da);									// ����_=>�ʉߓ_�ϊ�
	SetCurveMatrix3(md, wt, vb, vn, iz, da, db);						// ����_<=>�ʉߓ_�ϊ���د��
	CalcCurvePoint(md, vn, pt, pz, db);									// �ʉߓ_=>����_�ϊ�
	RedoCrtCurve(vn, vb, pz, iz);										// �f�ʋȐ�����
	CalcCurvePoint(md, vn, pz, pw, da);									// ����_=>�ʉߓ_�ϊ�
	RedoSetCurve(vn, vb, pw);											// ����_���̐���
	for(i=0; i<vn; i++) {
		iw[i] = (iy[i]) ? 4 : iz[i]*2;									// �ׯ���F�ݒ�
	}
	delete[] pt;
}

/************************************************************************/
void ChangeSPointMove(int m1, int m2, int m3, int m4, int no, int un,
                      int vn, int ub, int vb, double wt, int an, int fg,
                      int pl, PNTTYPE p1, VECTYPE p2, PNTTYPE p3,
                      PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                      PNTTYPE* ps, PNTTYPE* pz, int* ia, int* is,
                      int* iz, int* ta, CMDTYPE* cmp, OBJTYPE* op1,
                      OBJTYPE* op2, OBJTYPE* op3, OBJTYPE* op4)			//<<< �f�ʐ���_�ړ� �f�ʕύX
/************************************************************************/
{
	int* wk = new int[View->m_MaxSCtA];
	int  i, j;

	if(m4) {
		AttachCmd(op1);													// ����ލĐڑ�
	}
	if(op3!=NULL) {
		FreeObj(op3, 1);												// ���̍폜
	}
	if(op4!=NULL) {
		FreeObj(op4, 1);												// ���̍폜
	}
	if(m4) {
		DetachCmd(op1, cmp);											// ����ލēx�ؗ�
	}
	if(m3) {															// <�����̏����F�L>
		OnDspFlg(1, op1);												// �Ȗʗ��̕ҏW�Ώېݒ�
		OnEdtFlg(1, op1);
		OnDspFlg(1, op2);												// �Ȑ�����_���̕\��
	} else {															// <�����̏����F��>
		OnEdtFlg(1, op1);												// �Ȗʗ��̑Ώېݒ�
		OnDspFlg(1, op2);												// �Ȑ�����_���̕\��
	}
	SetSelNo(op2, 2);													// ���F�\��OP�ݒ�
	if(m1) {															// <�ʏ�Ȗ�>
		PutSPoint(m3, no, un, vn, fg, p3, pz, iz, p1, p2, pb, pc, ia);	// �f�ʐ���_���X�V
		MoveSPoint(op1, un, vn, ub, vb, pc, pa, pb, ta);				// �f�ʐ���_�ړ�
		RedoChgSurface(un, vn, ub, vb, pa, ia);							// �ȖʕҏW
		CalcSurfacePoint(m2, 1, 0, un, vn, ub, vb, wt, ia, pa, pb);		// ����_=>�ʉߓ_�ϊ�
		RedoSetSurface(un, vn, ub, vb, pb);								// �Ȗʐ���_����
		for(i=0; i<an; i++) {
			GetVtx(op2, i, &pb[i]);										// ���̕ҏW�㐧��_�擾
		}
	} else {															// <�|���Ȗ�>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pz[i].p[j] -= p3.p[j];
			}
			ps[un+i] = pz[i], is[un+i] = iz[i];							// U�f�ʏ��ݒ�
		}
		RedoChgSweepSurf(un, vn, ub, vb, ps, is);						// �|���ȖʕύX
		SetSweepCtrl(pl, un, vn, ub, ps, is, pa, wk);					// �|���Ȗʐ���_�ϊ�
		RedoSetSurface(un, vn, ub, vb, pa);								// �Ȗʐ���_����
		for(i=0; i<an; i++) {
			GetVtx(op2, i, &pb[i]);										// ���̕ҏW�㐧��_�擾
		}
	}
	delete[] wk;
}

/**********************************************************/
void PickBPointMove(int pn, int* pk, int fg,
                    int tn, OBJTYPE* op, int* ib, int* iw)				//<<< �ް����ړ� ��I��
/**********************************************************/
{
	int i;

	for(i=0; i<pn; i++) {
		if(ib[pk[i]]) {													// �I���ς͉���
			ib[pk[i]] = 0, iw[pk[i]] = 2;
		} else {														// ���I���͑I��
			ib[pk[i]] = 1, iw[pk[i]] = 4;
		}
	}
	SetLineNo3(tn, fg, ib, op);											// ײݔԍ��ݒ�3
}

/*********************************/
BOOL InitCScale(int* bn, int* dn)										//<<< �g��/�k����������
/*********************************/
{
	CSetBaseDlg1 dlg;
	dlg.SetValue(*bn, *dn);												// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(bn, dn);											// ��_,�����擾
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************************************/
void EraseObjSPointMove(int* on, int* dsp, OBJTYPE* top)				//<<< �f�ʐ���_�ړ�
/********************************************************/				//    ���̔�\��
{
	OBJTYPE* op;
	int      i1, i2;

	BaseObjPtr(0, &op, &i1, &i2);										// �S���̎擾
	while(op!=NULL) {
		dsp[(*on)++] = i1;												// �\��Ӱ�ޕۑ�
		if(*on>View->m_MaxSCtl) {
			*on = 0; break;
		}
		NextObjPtr(0, &op, &i1, &i2);									// �����̂�
	}
	if(*on>0) {															// <�L�������ȓ�>
		OffDspAllObject();												// �S���̔�\��
		OnDspFlg(1, top);												// �Ώۗ���dspflg:ON
		OnEdtFlg(1, top);												// �Ώۗ���edtflg:ON
	}
}

/******************************************************/
void DispObjSPointMove(int on, int* dsp, OBJTYPE* top)					//<<< �f�ʐ���_�ړ�
/******************************************************/				//    ���̕\��
{
	OBJTYPE* op;
	int      i1, i2, ct=0;

	if(on>0) {															// <�ΏۊO���̏����L>
		BaseObjPtr(0, &op, &i1, &i2);									// �S���̎擾
		while(op!=NULL) {
			if(dsp[ct++]) {												// �\��Ӱ�ޕ���
				OnDspFlg(1, op);
			}
			NextObjPtr(0, &op, &i1, &i2);								// �����̂�
		}
		OnEdtFlg(1, top);												// �Ώۗ���edtflg:ON
	}
}

/*****************************************************************/
static void AdjustMoveValue(int d1, int d2, int cn,
                            PNTTYPE* ca, PNTTYPE pt, PNTTYPE* mv)		//<<< �Ȑ����킹�ړ��ʎ擾
/*****************************************************************/
{
	int    i;
	double l1, l2, l3, p1, p2;

	for(i=0, l1=DBL_MAX; i<cn-1; i++) {
		if(((ca[i].p[d2]<=pt.p[d2]&&ca[i+1].p[d2]>=pt.p[d2])||
			(ca[i].p[d2]>=pt.p[d2]&&ca[i+1].p[d2]<=pt.p[d2]))&&
			fabs(ca[i].p[d2]-ca[i+1].p[d2])>EPSILON) {					// <�Y��������>
			l2 = fabs(ca[i].p[d2]-ca[i+1].p[d2]);						// ����d2������
			l3 = fabs(ca[i].p[d2]-pt.p[d2]);							// ��_�ʒud2������
			p2 = ca[i].p[d1]+(ca[i+1].p[d1]-ca[i].p[d1])*l3/l2;			// d1��_�ʒu
			l2 = fabs(pt.p[d1]-p2);										// pt�ƌ�_�ʒu�̒���
			if(l2<l1) {													// �ł��߂���_�ʒu
				p1 = p2, l1 = l2;
			}
		}
	}
	mv->p[0] = 0.0, mv->p[1] = 0.0, mv->p[2] = 0.0;						// �ړ��ʐݒ�
	mv->p[d1] = p1-pt.p[d1];
}

/**********************************************************************/
void AdjustCPoint(int d1, int d2, int cn, PNTTYPE* ca, int on,
                  int tn, int* ta, PNTTYPE* pa, PNTTYPE* pb,
                  PNTTYPE* pc, PNTTYPE* pd, int* ia, int* ib, int* ic)	//<<< ����_�Ȑ����킹
/**********************************************************************/
{
	PNTTYPE* wp = new PNTTYPE[View->m_MaxCCtA*2];
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  zer;
	int      i, i1, i2, un, vn, ub, vb, an, rp, cnt=0;
	double   wd[10], min1, max1, wt;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	for(i=0, min1=DBL_MAX, max1=1.0-DBL_MAX; i<cn; i++) {
		min1 = (min1>ca[i].p[d2]) ? ca[i].p[d2] : min1;					// ��Ȑ��ŏ��l
		max1 = (max1<ca[i].p[d2]) ? ca[i].p[d2] : max1;					// ��Ȑ��ő�l
	}
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {													// <<�ҏW���̗L�̏ꍇ>>
		if(GetSelNo(op)!=1) {											// ��Ȑ��ȊO�o�^
			SetSelNo(op, 2);
		}
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	OffEdtAllObject();													// �ҏW���̑S����
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {													// <<�ҏW���̗L�̏ꍇ>>
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		i1 = GetObjAtr(op);												// ���̑����擾
		if((i1==10||i1==11)&&cmp!=NULL) {								// <�Ȑ��̏ꍇ>
			vn = GetParaInt(cmp, 0);									// ����_���擾
			vb = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			rp = GetParaInt(cmp, 2);									// �Ȑ��������擾
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// �Ȑ��d�݌W���擾
			for(i=0; i<vn; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// ����_�擾
				ia[i] = GetParaInt(cmp, 4+i);							// ײ�Ӱ�ގ擾
			}
			SetCurve(vn, vb, pa);										// �Ȑ�����_����
			GetObjPtr(&op1);											// ����_OP�ۑ�
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �ȖʕҏW������
			while(cmp!=NULL) {
				RedoCommandCopy(op1, cmp);								// ����ޕ��ʎ��s
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<vn; i++) {
				GetVtx(op1, i, &pb[i]);									// ���̕ҏW�㐧��_�擾
			}
			FreeObj(op1, 1);											// �ҏW�㗧�̍폜
			for(i=0; i<vn; i++) {
				ib[i] = (ta[cnt+i]==4) ? 1 : 0;							// �w���׸ސݒ�
			}
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<vn; i++) {
					ic[i] = 1;											// ���͗L�͑S�͈͐ݒ�
				}
			}
			for(i=0; i<vn; i++) {
				if(ib[i]&&pb[i].p[d2]>=min1&&pb[i].p[d2]<=max1) {		// <�w���ςŊ�Ȑ��͈͓�>
					AdjustMoveValue(d1, d2, cn, ca, pb[i], &pc[i]);		// �Ȑ����킹�ړ��ʎ擾
				} else {												// �ȊO�͈ړ��ʂȂ�
					pc[i] = zer;
				}
			}
			OnEdtFlg(1, op);											// �ҏW�Ώۗ��̓o�^
			for(i=0; i<vn; i++) {
				wp[i*2] = pb[i];										// ����_���Ȑ��ҏW�p�ɕύX
			}
			MoveECurve(0, wd, pc, op, vn, vb, pa, wp, ib, pd, ic);		// ����_�ړ�
			ChgCurve(NULL, vn, vb, rp, wt, pd, ia);						// �Ȑ��ύX
			OffEdtFlg(1, op);											// �ҏW�Ώۗ��̉���
			cnt += vn;													// ����_�����v
		} else if((i1==20||i1==21)&&cmp!=NULL) {						// <�ʏ�Ȗʂ̏ꍇ>
			un = GetParaInt(cmp, 0);									// U����_���擾
			vn = GetParaInt(cmp, 1);									// V����_���擾
			ub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			vb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			rp = GetParaInt(cmp, 4);									// �Ȑ��������擾
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// �Ȑ��d�݌W���擾
			an = un*vn;													// ����_��
			for(i=0; i<an; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// ����_�擾
				ia[i] = GetParaInt(cmp, 7+i);							// ײ�Ӱ�ގ擾
			}
			SetSurface(un, vn, ub, vb, pa);								// �Ȗʐ���_����
			GetObjPtr(&op1);											// ����_OP�ۑ�
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �ȖʕҏW������
			while(cmp!=NULL) {
				if(GetCmdNo(cmp)!=THICKNESS) {							// <���݂Â��ȊO�̏ꍇ>
					RedoCommandCopy(op1, cmp);							// ����ޕ��ʎ��s
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<an; i++) {
				GetVtx(op1, i, &pb[i]);									// ���̕ҏW�㐧��_�擾
			}
			FreeObj(op1, 1);											// �ҏW�㗧�̍폜
			for(i=0; i<an; i++) {
				ib[i] = (ta[cnt+i]==4) ? 1 : 0;							// �w���׸ސݒ�
			}
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<an; i++) {
					ic[i] = 1;											// ���͗L�͑S�͈͐ݒ�
				}
			}
			for(i=0; i<an; i++) {
				if(ib[i]&&pb[i].p[d2]>=min1&&pb[i].p[d2]<=max1) {		// <�w���ςŊ�Ȑ��͈͓�>
					AdjustMoveValue(d1, d2, cn, ca, pb[i], &pc[i]);		// �Ȑ����킹�ړ��ʎ擾
				} else {												// �ȊO�͈ړ��ʂȂ�
					pc[i] = zer;
				}
			}
			OnEdtFlg(1, op);											// �ҏW�Ώۗ��̓o�^
			MoveCPoint(0, pc, op, un, vn, ub, vb, wt,
                       ia, ib, pa, pb, pd, ic);							// ����_�ړ�
			ChgSurface(NULL, un, vn, ub, vb, rp, wt, pd, ia);			// �ȖʕύX
			OffEdtFlg(1, op);											// �ҏW�Ώۗ��̉���
			cnt += an;													// ����_�����v
		}
		SelNextObjPtr(2, &op);											// ���ҏW���̂�
	}
	delete[] wp;
}

/************************************************************/
BOOL GetSurfaceCurve(int un, int vn, int ub, int vb, int uf,
                     int vf, int mx, int no, OBJTYPE* op,
                     int* pn, int* bc, PNTTYPE* pa)						//<<< �ȖʋȐ��擾
/************************************************************/
{
	int  i, wuv, wua, wva, flg, lno, uu, vv, ms, div=1;
	BOOL ret=TRUE;

	wva = (uf%2==1) ? 1 : 0;											// U/V���E�ƕ␳���擾
	wua = (vf%2==1) ? 1 : 0;
	wuv = (uf%2==1) ? vn-1 : vn;
	wuv = (uf>=2) ? wuv-1 : wuv;
	if(no<wuv) {														// <Vײ�>
		flg = 1;
		lno = no+wva;
	} else {															// <Uײ�>
		flg = 0;
		lno = no-wuv+wua;
	}
	uu = un, vv = vn;													// U/V����_��
	if(ub) {															// U�͐���_��UP
		uu++;
	}
	if(vb) {															// V�͐���_��UP
		vv++;
	}
	if(uu>1&&vv>1) {
		for(i=0; i<mx; i++) {
			div = div*2;												// ����_���璸�_�ւ̔{��
		}
		uu = (uu-1)*div+1;												// ���_��
		vv = (vv-1)*div+1;
		if(uf==1||uf==2||uf==3) {										// <<U�[����_����>>
			if(ub) {													// U�͎n�_/�I�_����
				uu--;
			}
			if(uf==1) {													// <<�擪Uٰ�ߓ���>>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*(div*lno-1)+i+1, &pa[i]);		// �w�蒸�_�擾
						}
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// �w�蒸�_�擾
						for(i=1; i<vv; i++) {
							GetVtx(op, lno*div+uu*(i-1)+1, &pa[i]);		// �w�蒸�_�擾
						}
					}
				}
			} else if(uf==2) {											// <<�ŏIUٰ�ߓ���>>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*div*lno+i, &pa[i]);			// �w�蒸�_�擾
						}
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv-1; i++) {
							GetVtx(op, lno*div+uu*i, &pa[i]);			// �w�蒸�_�擾
						}
						GetVtx(op, uu*(vv-1), &pa[i]);					// �w�蒸�_�擾
					}
				}
			} else {													// <<�擪/�ŏI����>>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<uu; i++) {
							GetVtx(op, uu*(div*lno-1)+i+1, &pa[i]);		// �w�蒸�_�擾
						}
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// �w�蒸�_�擾
						for(i=1; i<vv-1; i++) {
							GetVtx(op, lno*div+uu*(i-1)+1, &pa[i]);		// �w�蒸�_�擾
						}
						GetVtx(op, uu*(vv-2)+1, &pa[i]);				// �w�蒸�_�擾
					}
				}
			}
		} else if(vf==1||vf==2||vf==3) {								// <<V�[����_����>>
			if(vb) {													// V�͎n�_/�I�_����
				vv--;
			}
			if(vf==1) {													// <�擪Vٰ�ߓ���>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						GetVtx(op, 0, &pa[0]);							// �w�蒸�_�擾
						for(i=1; i<uu; i++) {
							GetVtx(op, (uu-1)*div*lno+i, &pa[i]);		// �w�蒸�_�擾
						}
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							GetVtx(op, lno*div+(uu-1)*i, &pa[i]);		// �w�蒸�_�擾
						}
					}
				}
			} else if(vf==2) {											// <�ŏIVٰ�ߓ���>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						ms = (div*lno<2) ? 0 : div*lno-1;				// �␳(-)���ݒ�
						for(i=0; i<uu-1; i++) {
							GetVtx(op, uu*div*lno+i-ms, &pa[i]);		// �w�蒸�_�擾
						}
						GetVtx(op, uu-1, &pa[i]);						// �w�蒸�_�擾
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							ms = (i<2) ? 0 : i-1;
							GetVtx(op, lno*div+uu*i-ms, &pa[i]);		// �w�蒸�_�擾
						}
					}
				}
			} else {													// <�擪/�ŏI����>
				if(flg) {												// <U��w��>
					*pn = uu;											// �Ȑ����_��
					*bc = ub;											// �Ȑ����E����
					if(uu>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						if(div*lno==0) {								// �␳(-)���ݒ�
							ms = 0;
						} else if(div*lno==1) {
							ms = 1;
						} else {
							ms = 2*div*lno-1;
						}
						GetVtx(op, 0, &pa[0]);							// �w�蒸�_�擾
						for(i=1; i<uu-1; i++) {
							GetVtx(op, uu*div*lno+i-ms, &pa[i]);		// �w�蒸�_�擾
						}
						GetVtx(op, uu-1, &pa[i]);						// �w�蒸�_�擾
					}
				} else {												// <V��w��>
					*pn = vv;											// �Ȑ����_��
					*bc = vb;											// �Ȑ����E����
					if(vv>View->m_MaxCPnt) {							// <�Ȑ����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ret = FALSE;
					}
					if(ret) {
						for(i=0; i<vv; i++) {
							if(i==0) {
								ms = 0;
							} else if(i==1) {
								ms = 1;
							} else {
								ms = 2*i-1;
							}
							GetVtx(op, lno*div+uu*i-ms, &pa[i]);		// �w�蒸�_�擾
						}
					}
				}
			}
		} else {														// <<����_������>>
			if(ub) {													// U�͎n�_/�I�_����
				uu--;
			}
			if(vb) {													// V�͎n�_/�I�_����
				vv--;
			}
			if(flg) {													// <U��w��>
				*pn = uu;												// �Ȑ����_��
				*bc = ub;												// �Ȑ����E����
				if(uu>View->m_MaxCPnt) {								// <�Ȑ����_����ȏ�>
					AfxMessageBox(IDS_MANY_CTRLP);						// �װү����
					ret = FALSE;
				}
				if(ret) {
					for(i=0; i<uu; i++) {
						GetVtx(op, div*uu*lno+i, &pa[i]);				// �w�蒸�_�擾
					}
				}
			} else {													// <V��w��>
				*pn = vv;												// �Ȑ����_��
				*bc = vb;												// �Ȑ����E����
				if(vv>View->m_MaxCPnt) {								// <�Ȑ����_����ȏ�>
					AfxMessageBox(IDS_MANY_CTRLP);						// �װү����
					ret = FALSE;
				}
				if(ret) {
					for(i=0; i<vv; i++) {
						GetVtx(op, lno*div+uu*i, &pa[i]);				// �w�蒸�_�擾
					}
				}
			}
		}
	}
	return ret;
}
