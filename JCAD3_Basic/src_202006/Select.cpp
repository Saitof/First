/////////////////////////////////////////////////////////////////////////////
// ���̑I��
// Select.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*************************/
void OnDspAllObject(void)												//<<< ���̕\��(�S����)
/*************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(0, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OnDspFlg(1, op);												// dspflg:ON
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
}

/**************************/
void OffDspAllObject(void)												//<<< ���̔�\��(�S����)
/**************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(0, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OffDspFlg(1, op);												// dspflg:OFF
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
}

/******************************/
void OnDspGroupObject(int gno)											//<<< ���̕\��(��ٰ��)
/******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(0, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		GetGroup(op, grp);												// ��ٰ�ߏ��擾
		if(grp[gno]) {
			OnDspFlg(1, op);											// dspflg:ON
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
}

/*******************************/
void OffDspGroupObject(int gno)											//<<< ���̔�\��(��ٰ��)
/*******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(0, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		GetGroup(op, grp);												// ��ٰ�ߏ��擾
		if(grp[gno]) {
			OffDspFlg(1, op);											// dspflg:OFF
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
}

/*****************************************************/
void EdtPointObject(int plane, PNTTYPE pnt, int ckey)					//<<< �ҏW���̑I��(�߲��)
/*****************************************************/
{
	OBJTYPE* op, * sop=NULL;
	BOXTYPE  box;
	int      dsp, edt, prt, oed, chk=0;

	BaseObjPtr(1, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		if(ckey==0) {
			OffEdtFlg(1, op);											// edtflg:OFF
			edt = 0;
		}
		prt = GetObjPrt(op);											// �߰�No�擾
		GetObjBox(op, &box);											// �ޯ�����ގ擾
		if(BoxCheck(plane, box, pnt)) {									// <<�I�𗧑�>>
			if(ckey==1) {												// <������(�ǉ�=����)>
				if(!edt) {
					OnEdtFlg(1, op);									// edtflg:ON
					oed = 1;
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
					oed = 0;
				}
			} else if(ckey==2||ckey==0) {								// <������(�ǉ�)/��������>
				OnEdtFlg(1, op);										// edtflg:ON
				oed = 1;
			} else if(ckey==3) {										// <������(����)>
				OffEdtFlg(1, op);										// edtflg:OFF
				oed = 0;
			}
			if(prt==1) {												// �Ưĥ�e
				chk = 1, sop = op;
			}
		}
		if(prt==2) {													// <<�Ưĥ�q>>
			if(op->oset!=sop) {
				chk = 0;
			}
			if(chk) {
				if(oed) {
					OnEdtFlg(1, op);									// edtflg:ON
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
				}
			}
		} else {														// <<�Ưĥ�q�ȊO>>
			if(op!=sop) {
				chk = 0;
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/*****************************************************/
void EdtAreaObject(int plane, BOXTYPE area, int ckey)					//<<< �ҏW���̑I��(�ر)
/*****************************************************/
{
	OBJTYPE* op, * sop=NULL;
	PNTTYPE  pnt;
	BOXTYPE  box, abox;
	int      i, dsp, edt, prt, oed, chk=0;

	for(i=0; i<DIM; i++) {												// �ޯ�����ވʒu�␳
		if(area.bp[0].p[i]<area.bp[1].p[i]) {
			abox.bp[0].p[i] = area.bp[0].p[i];
			abox.bp[1].p[i] = area.bp[1].p[i];
		} else {
			abox.bp[0].p[i] = area.bp[1].p[i];
			abox.bp[1].p[i] = area.bp[0].p[i];
		}
	}
	BaseObjPtr(1, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		if(!ckey) {
			OffEdtFlg(1, op);											// edtflg:OFF
			edt = 0;
		}
		prt = GetObjPrt(op);											// �߰�No�擾
		GetObjBox(op, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &pnt);											// �ޯ�������擾
		if(BoxCheck(plane, abox, pnt)) {								// <<�I�𗧑�>>
			if(ckey==1) {												// <������(�ǉ�=����)>
				if(!edt) {
					OnEdtFlg(1, op);									// edtflg:ON
					oed = 1;
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
					oed = 0;
				}
			} else if(ckey==2||ckey==0) {								// <������(�ǉ�)/��������>
				OnEdtFlg(1, op);										// edtflg:ON
				oed = 1;
			} else if(ckey==3) {										// <������(����)>
				OffEdtFlg(1, op);										// edtflg:OFF
				oed = 0;
			}
			if(prt==1) {												// �Ưĥ�e
				chk = 1, sop = op;
			}
		}
		if(prt==2) {													// <<�Ưĥ�q>>
			if(op->oset!=sop) {
				chk = 0;
			}
			if(chk) {
				if(oed) {
					OnEdtFlg(1, op);									// edtflg:ON
				} else {
					OffEdtFlg(1, op);									// edtflg:OFF
				}
			}
		} else {														// <<�Ưĥ�q�ȊO>>
			if(op!=sop) {
				chk = 0;
			}
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/******************************/
void OnEdtGroupObject(int gno)											//<<< �ҏW���̑I��(��ٰ��)
/******************************/
{
	OBJTYPE* op;
	int      dsp, edt, grp[32];

	BaseObjPtr(1, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		GetGroup(op, grp);												// ��ٰ�ߏ��擾
		if(grp[gno]) {
			OnEdtFlg(1, op);											// edtflg:ON
		}
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/*************************/
void OnEdtAllObject(void)												//<<< �ҏW���̑S�I��(�\������)
/*************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(1, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OnEdtFlg(1, op);												// edtflg:ON
		NextObjPtr(1, &op, &dsp, &edt);
	}
}

/**************************/
void OffEdtAllObject(void)												//<<< �ҏW���̑S����
/**************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		OffEdtFlg(1, op);												// edtflg:OFF
		NextObjPtr(2, &op, &dsp, &edt);
	}
}

/************************************/
void OffEdtOtherObject(OBJTYPE* sop)									//<<< �w��O�ҏW�Ώۉ���
/************************************/
{
	OBJTYPE* op;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		if(op!=sop) {
			OffEdtFlg(1, op);											// edtflg:OFF
		}
		NextObjPtr(2, &op, &dsp, &edt);
	}
}
