/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암
// DataAccess2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"

/**********************************************************/
/*****                  ���̍\�����p                *****/
/**********************************************************/
/***********************************************/
void CreatOif(int flg, OBJTYPE* op, char* name,
              OIFTYPE* bfo, OIFTYPE** oif)								//<<< ���̍\�����̈�m��
/***********************************************/
{
	OIFTYPE* optr;

	if(Gc_oif==NULL) {													// <GC���̏ꍇ>
		*oif = (OIFTYPE *)malloc(sizeof(OIFTYPE));						// �V�K�̈�m��
		if(*oif==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC�L�̏ꍇ>
		*oif = Gc_oif, Gc_oif = Gc_oif->next;							// GC�擪�g�p
	}
	(*oif)->op = op;
	memcpy((*oif)->name, name, 24);
	(*oif)->level = 0;
	(*oif)->ono = -1;
	(*oif)->next = NULL;
	if(flg==0) {														// <<�����ڑ��̏ꍇ>>
		if(Base_oif==NULL) {											// <�ް����̏ꍇ>
			Base_oif = (*oif);											// �擪�֐ڑ�
		} else {														// <�ް��L�̏ꍇ>
			optr = Base_oif;
			while(optr->next!=NULL) {
				optr = optr->next;										// �ŏI�\������
			}
			optr->next = (*oif);										// �ŏI�\���֐ڑ�
		}
	} else if(flg==1) {													// <<�擪�ڑ��̏ꍇ>>
		if(Base_oif==NULL) {											// <�ް����̏ꍇ>
			Base_oif = (*oif);											// �擪�֐ڑ�
		} else {														// <�ް��L�̏ꍇ>
			(*oif)->next = Base_oif, Base_oif = (*oif);					// ���擪�����\����
		}
	} else {															// <<���Ԑڑ��̏ꍇ>>
		if(bfo==NULL) {													// <�ڑ��߲�����̏ꍇ>
			if(Base_oif==NULL) {										// �����ڑ�
				Base_oif = (*oif);
			} else {
				optr = Base_oif;
				while(optr->next!=NULL) {
					optr = optr->next;
				}
				optr->next = (*oif);
			}
		} else {														// <�ڑ��߲���L�̏ꍇ>
			(*oif)->next = bfo->next, bfo->next = (*oif);				// �w��\���̎��\����ڑ�
		}
	}
}

/*********************************/
static void TempOif(OIFTYPE* oif)										//<<< ���̍\�����ꎞ�폜
/*********************************/
{
	OIFTYPE* optr;

	if(oif!=NULL) {
		View->DelOifItem(oif);											// ���̍\������э폜
	}
	if(Base_oif==oif) {													// <�擪�\���̏ꍇ>
		Base_oif = oif->next;											// ���\��Base�ڑ�
	} else {															// <�擪�\���ȊO�̏ꍇ>
		if((optr=Base_oif)==NULL) {
			return;
		}
		while(optr->next!=NULL&&optr->next!=oif) {
			optr = optr->next;											// �O�\������
		}
		if(optr->next==NULL) {
			return;														// �w��\�������~
		}
		optr->next = oif->next;											// �O�\�����\���ڑ�
	}
	oif->next = NULL;
	if(Temp_oif==NULL) {
		Temp_oif = oif;
	} else {
		optr = Temp_oif;
		while(optr->next!=NULL) {
			optr = optr->next;											// �ŏI�\������
		}
		optr->next = oif;												// �ŏI�\��ؽĐڑ�
	}
}

/***********************************/
void FreeOif(int flg, OIFTYPE* oif)										//<<< ���̍\�����̈���
/***********************************/
{
	OIFTYPE* optr;
	int      chk=-1;

	if(flg==0&&oif!=NULL) {
		View->DelOifItem(oif);											// ���̍\������э폜
	}
	if(Temp_oif==oif) {													// �\�����݊m�F
		chk = 0;
	} else if(Base_oif==oif) {
		chk = 1;
	} else {
		if((optr=Temp_oif)!=NULL) {										// Temp�m�F
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;
			}
			chk = (optr->next==oif) ? 0 : chk;
		}
		if(chk==-1) {
			if((optr=Base_oif)!=NULL) {									// Base�m�F
				while(optr->next!=NULL&&optr->next!=oif) {
					optr = optr->next;
				}
				chk = (optr->next==oif) ? 1 : chk;
			}
		}
	}
	if(chk==-1) {
		return;															// �\�������~
	}
	if(!chk) {															// <Temp�L�̏ꍇ>
		if(Temp_oif==oif) {												// <�擪�\���̏ꍇ>
			Temp_oif = oif->next;										// ���\��Temp�ڑ�
		} else {														// <�擪�\���ȊO�̏ꍇ>
			optr = Temp_oif;
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;										// �O�\������
			}
			optr->next = oif->next;										// �O�\�����\���ڑ�
		}
	} else {															// <Base�L�̏ꍇ>
		if(Base_oif==oif) {												// <�擪�\���̏ꍇ>
			Base_oif = oif->next;										// ���\��Base�ڑ�
		} else {														// <�擪�\���ȊO�̏ꍇ>
			optr = Base_oif;
			while(optr->next!=NULL&&optr->next!=oif) {
				optr = optr->next;										// �O�\������
			}
			optr->next = oif->next;										// �O�\�����\���ڑ�
		}
	}
	oif->next = Gc_oif, Gc_oif = oif;									// Gc�擪�ڑ�
}

/*********************************/
static void UndoOif(OIFTYPE* oif)										//<<< ���̍\����񕜌�
/*********************************/
{
	OIFTYPE *optr;

	if(Temp_oif==oif) {													// <�擪�\���̏ꍇ>
		Temp_oif = oif->next;											// ���\��Temp�ڑ�
	} else {															// <�擪�\���ȊO�̏ꍇ>
		if((optr=Temp_oif)==NULL) {
			return;
		}
		while(optr->next!=NULL&&optr->next!=oif) {
			optr = optr->next;											// �O�\������
		}
		if(optr->next==NULL) {
			return;														// �w��\�������~
		}
		optr->next = oif->next;											// �O�\�����\���ڑ�
	}
	oif->level = 0, oif->ono = -1, oif->next = NULL;
	if(Base_oif==NULL) {
		Base_oif = oif;
	} else {
		optr = Base_oif;
		while(optr->next!=NULL) {
			optr = optr->next;											// �ŏI�\������
		}
		optr->next = oif;												// �ŏI�\��ؽĐڑ�
	}
	if(oif!=NULL) {
		View->SetOifItem(oif);											// ���̍\������ѐݒ�
	}
}

/*****************************************/
void SetOifObj(OIFTYPE* oif, OBJTYPE* op)								//<<< �\����񗧑��߲���ݒ�
/*****************************************/
{
	oif->op = op;
	if(op!=NULL) {
		op->oift = oif;
	}
}

/******************************************/
void GetOifObj(OIFTYPE* oif, OBJTYPE** op)								//<<< �\����񗧑��߲���擾
/******************************************/
{
	*op = oif->op;
}

/******************************************/
void GetObjOif(OBJTYPE* op, OIFTYPE** oif)								//<<< ���̍\������߲���擾
/******************************************/
{
	*oif = op->oift;
}

/*************************************************/
void BaseOifPtr(int flg, int mode, OIFTYPE** oif)						//<<< ���̏���ް��߲���擾
/*************************************************/						// op==NULL mode 0:NG, 1:OK
{
	OIFTYPE* optr;

	optr = Base_oif, *oif = NULL;
	if(flg==0) {														// <�S���̎w��̏ꍇ>
		*oif = Base_oif;
	} else if(flg==1) {													// <dsp:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->dspflg==1) {								// dsp:ON����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	} else if(flg==2) {													// <edt:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1) {								// edt:ON����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	} else {															// <edt:ON/selno=1�̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1||optr->op->selno==1) {			// edt:ON/selno=1����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	}
}

/*************************************************/
void NextOifPtr(int flg, int mode, OIFTYPE** oif)						//<<< �����̏���߲���ړ�
/*************************************************/						// op==NULL mode 0:NG, 1:OK
{
	OIFTYPE* optr;

	if(*oif==NULL) {
		return;
	}
	optr = (*oif)->next, *oif = NULL;
	if(flg==0) {														// <�S���̎w��̏ꍇ>
		*oif = optr;
	} else if(flg==1) {													// <dsp:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->dspflg==1) {								// dsp:ON����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	} else if(flg==2) {													// <edt:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1) {								// edt:ON����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	} else {															// <edt:ON/selno=1�̏ꍇ>
		while(optr!=NULL) {
			if(optr->op!=NULL) {
				if(optr->op->edtflg==1||optr->op->selno==1) {			// edt:ON/selno=1����
					*oif = optr; break;
				}
			} else {
				if(mode) {
					*oif = optr; break;									// op==NULL��OK
				}
			}
			optr = optr->next;
		}
	}
}

/**************************************************/
void SetOifName(int flg, OIFTYPE* oif, char* name)						//<<< ���̍\�����̐ݒ�
/**************************************************/
{
	memcpy(oif->name, name, 24);
	if(flg==0) {
		if(oif!=NULL) {
			View->SetOifItem(oif);										// ���̍\������ѐݒ�
		}
	} else if(flg==1) {
		if(oif!=NULL) {
			View->UpdateOifItem(oif);									// ���̍\������эX�V
		}
	}
}

/*****************************************/
void GetOifName(OIFTYPE* oif, char* name)								//<<< ���̍\�����̎擾
/*****************************************/
{
	memcpy(name, oif->name, ONM_LENG);
}

/***************************************/
void SetOifLevel(OIFTYPE* oif, int lvl)									//<<< ���̍\�����ِݒ�
/***************************************/
{
	oif->level = (short)lvl;
}

/*****************************/
int GetOifLevel(OIFTYPE* oif)											//<<< ���̍\�����َ擾
/*****************************/
{
	return (int)oif->level;
}

/********************/
void InitOifNo(void)													//<<< ���̍\������No������
/********************/
{
	OIFTYPE* oif;

	oif = Base_oif;
	while(oif!=NULL) {
		oif->ono = -1;
		oif = oif->next;
	}
}

/***********************************/
void SetOifNo(OIFTYPE* oif, int no)										//<<< ���̍\������No�ݒ�
/***********************************/
{
	oif->ono = (short)no;
}

/**************************/
int GetOifNo(OIFTYPE* oif)												//<<< ���̍\������No�擾
/**************************/
{
	return (int)oif->ono;
}

/**********************/
void SortOifData(void)													//<<< ���̍\���ް����
/**********************/
{
	OIFTYPE* oif, * lst, * osv, * owk;
	int      i, max=0;

	oif = Base_oif, lst = NULL, osv = NULL;
	while(oif!=NULL) {
		if(oif->ono>=0) {												// <����o���ް�>
			if(max<oif->ono) {
				max = oif->ono;											// �ő嗧��No�擾
			}
			owk = oif;
			oif = oif->next;											// ���ް���
			if(Base_oif==owk) {											// �����ް���ؗ�
				Base_oif = owk->next;
			} else {
				lst->next = owk->next;
			}
			owk->next = osv;											// �����ް��̂ݕʐڑ�
			osv = owk;
		} else {														// <����o�͈ȊO>
			lst = oif;													// ���O�ް���ۑ�
			oif = oif->next;											// ���ް���
		}
	}
	for(i=0; i<=max; i++) {												// <<����No���ɌJ��Ԃ�>>
		oif = osv, owk = NULL;
		while(oif!=NULL) {
			if(oif->ono==i) {											// <�Y������No�Ɉ�v>
				if(osv==oif) {											// �����ް���ؗ�
					osv = oif->next;
				} else {
					owk->next = oif->next;
				}
				oif->next = NULL;
				break;
			}
			owk = oif;													// ���O�ް���ۑ�
			oif = oif->next;											// ���ް���
		}
		if(oif!=NULL) {
			if(Base_oif==NULL) {										// �o�^���ް��̖�����
				Base_oif = oif;											// ����No���ɐڑ�
			} else {
				lst->next = oif;
			}
			lst = oif;
		}
	}
}

/**********************************************************/
/*****                    ���̐����p                  *****/
/**********************************************************/
/************************************************************/
void GetDataBasePtr(OBJTYPE** bop, PNTTYPE** vtx, int** flp)			//<<< �ް��ް��擪�߲���擾
/************************************************************/
{
	*bop = Base_op;														// �����ް��̐擪�߲��
	*vtx = Pnt_ary;														// ���_�ް��̐擪�߲��
	*flp = Int_ary;														// ��ٰ���ް��̐擪�߲��
}

/****************************/
void OnResetObj(OBJTYPE* op)											//<<< ؾ�ė��̓o�^
/****************************/
{
	Reset_op = op;
}

/**********************/
void OffResetObj(void)													//<<< ؾ�ė��̉���
/**********************/
{
	Reset_op = NULL;
}

/**************************/
BOOL GetResetObjMode(void)												//<<< ؾ�ė���Ӱ�ގ擾
/**************************/
{
	if(Reset_op!=NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************************************/
void ChangeObj(OBJTYPE* op, int vno, int lno1, int lno2)				//<<< �����ް��̈�ύX
/********************************************************/
{
	int ovn, ova, oln, ola, src, chn, i;

	if(op->vnum==vno&&op->lnum1==lno1&&op->lnum2==lno2) {
		return;
	}
	ovn = op->vnum, ova = Pnt_adr;										// ���̈搔�ۑ�
	oln = op->lnum1+op->lnum2, ola = Int_adr;
	op->vnum = vno, Pnt_adr += vno-ovn;									// ���_�ް��̈�m��
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR3);
	}
	op->lnum1 = lno1, op->lnum2 = lno2, Int_adr += lno1+lno2-oln;		// ��ٰ���ް��̈�m��
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR4);
	}
	src = op->vsa+ovn;													// �̈���(���_�ް�)
	if(ova<Pnt_adr) {
		chn = Pnt_adr-ova;
		if(chn!=0) {
			for(i=ova-1; i>=src; i--) {
				Pnt_ary[i+chn] = Pnt_ary[i];
			}
		}
	} else {
		chn = ova-Pnt_adr;
		if(chn!=0) {
			for(i=src; i<ova; i++) {
				Pnt_ary[i-chn] = Pnt_ary[i];
			}
		}
	}
	src = op->lsa+oln;													// �̈���(��ٰ��)
	if(ola<Int_adr) {
		chn = Int_adr-ola;
		if(chn!=0) {
			for(i=ola-1; i>=src; i--) {
				Int_ary[i+chn] = Int_ary[i];
			}
		}
	} else {
		chn = ola-Int_adr;
		if(chn!=0) {
			for(i=src; i<ola; i++) {
				Int_ary[i-chn] = Int_ary[i];
			}
		}
	}
	ChangeAreaAdr(op->lsa, lno1+lno2-oln, op->vsa, vno-ovn);			// �z����ڽ�ύX
}

/********************************************************/
void CreatObj(int vno, int lno1, int lno2, OBJTYPE** op)				//<<< �����ް��̈�m��
/********************************************************/
{
	OBJTYPE* optr;
	OIFTYPE* oif;

	if(Reset_op!=NULL) {												// <�̈�m�ۍς�>
		ChangeObj(Reset_op, vno, lno1, lno2);							// �����ް��̈�ύX
		*op = Reset_op;													// �߲�����->���~
		return;
	}  
	if(Gc_op==NULL) {													// <GC���̏ꍇ>
		*op = (OBJTYPE *)malloc(sizeof(OBJTYPE));						// �V�K�̈�m��
		if(*op==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC�L�̏ꍇ>
		*op = Gc_op, Gc_op = Gc_op->onxt;								// GC�擪�g�p
	}
	(*op)->vnum = vno;													// ���_�ް��̈�m��
	(*op)->vsa = Pnt_adr, Pnt_adr += vno;
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR3);
	}
	(*op)->lnum1 = lno1;												// ��ٰ���ް��̈�m��
	(*op)->lnum2 = lno2;
	(*op)->lsa = Int_adr, Int_adr += lno1+lno2;
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR4);
	}
	(*op)->attr = 0;													// ���̏�񏉊���
	(*op)->part = 0;
	(*op)->color = 0;
	(*op)->dspflg = 1;
	(*op)->edtflg = 0;
	(*op)->lmode = 0;
	(*op)->list = 0;
	(*op)->group = 0;
	(*op)->selno = 0;
	(*op)->ono = ++Obj_No;												// ����No
	(*op)->bcmp = NULL;													// �擪������߲��
	(*op)->ccmp = NULL;													// �L���擪������߲��
	(*op)->rscmp = NULL;												// �ݸސ擪������߲��
	(*op)->rccmp = NULL;												// �ݸތ��ݺ�����߲��
	(*op)->ecmp = NULL;													// ���������߲��
	(*op)->onxt = NULL;													// �������߲��
	(*op)->cnxt = NULL;													// ���ꐶ�����ٓ����߲��
	(*op)->dnxt = NULL;													// ����폜���ٓ����߲��
	(*op)->oset = NULL;													// ��ė����߲��
	(*op)->oift = NULL;													// ���̏���߲��
	if(Base_op==NULL) {
		Base_op = (*op);
	} else {
		optr = Base_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// �ŏI���̌���
		}
		optr->onxt = (*op);												// �ŏI����ؽĐڑ�
	}
	CreatOif(0, (*op), OBJ_NAME00, NULL, &oif);							// ���̍\�����̈�m��
	(*op)->oift = oif;
}

/*************************/
void TempObj(OBJTYPE* op)												//<<< �����ް��ꎞ�폜
/*************************/
{
	OBJTYPE* optr;

	if(Base_op==op) {													// <�擪���̂̏ꍇ>
		Base_op = op->onxt;												// ������Base�ڑ�
	} else {															// <�擪���̈ȊO�̏ꍇ>
		if((optr=Base_op)==NULL) {
			return;
		}
		while(optr->onxt!=NULL&&optr->onxt!=op) {
			optr = optr->onxt;											// �O���̌���
		}
		if(optr->onxt==NULL) {
			return;														// �w�藧�̖����~
		}
		optr->onxt = op->onxt;											// �O���̎����̐ڑ�
	}
	op->edtflg = 0;														// edtflg:OFF
	op->dspflg = 1;														// dspflg:ON
	op->selno = 0;														// �I��No������
	op->onxt = NULL;
	if(Temp_op==NULL) {
		Temp_op = op;
	} else {
		optr = Temp_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// �ŏI���̌���
		}
		optr->onxt = op;												// �ŏI����ؽĐڑ�
	}
	TempOif(op->oift);													// ���̍\�����ꎞ�폜
}

/**********************************/
void FreeObj(OBJTYPE* op, int flg)										//<<< �����ް��̈���
/**********************************/
{
	OBJTYPE* optr;
	CMDTYPE* cptr;
	int      src, i, chk=-1;

	if(Temp_op==op) {													// ���̑��݊m�F
		chk = 0;
	} else if(Base_op==op) {
		chk = 1;
	} else {
		if((optr=Temp_op)!=NULL) {										// Temp�m�F
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;
			}
			chk = (optr->onxt==op) ? 0 : chk;
		}
		if(chk==-1) {
			if((optr=Base_op)!=NULL) {									// Base�m�F
				while(optr->onxt!=NULL&&optr->onxt!=op) {
					optr = optr->onxt;
				}
				chk = (optr->onxt==op) ? 1 : chk;
			}
		}
	}
	if(chk==-1) {
		return;															// ���̖����~
	}
	View->DelDispList(op);												// �ި���ڲ�ؽč폜
	while(TRUE) {
		BaseCmdPtr(0, op, &cptr);										// �擪����ގ擾
		if(cptr==NULL) {
			break;
		}
		FreeCmd(op, cptr, flg);											// ����ޗ̈���
	}
	if(!chk) {															// <Temp�L�̏ꍇ>
		if(Temp_op==op) {												// <�擪���̂̏ꍇ>
			Temp_op = op->onxt;											// ������Temp�ڑ�
		} else {														// <�擪���̈ȊO�̏ꍇ>
			optr = Temp_op;
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;										// �O���̌���
			}
			optr->onxt = op->onxt;										// �O���̎����̐ڑ�
		}
	} else {															// <Base�L�̏ꍇ>
		if(Base_op==op) {												// <�擪���̂̏ꍇ>
			Base_op = op->onxt;											// ������Base�ڑ�
		} else {														// <�擪���̈ȊO�̏ꍇ>
			optr = Base_op;
			while(optr->onxt!=NULL&&optr->onxt!=op) {
				optr = optr->onxt;										// �O���̌���
			}
			optr->onxt = op->onxt;										// �O���̎����̐ڑ�
		}
	}
	op->edtflg = 0;														// edtflg:OFF
	op->dspflg = 1;														// dspflg:ON
	op->selno = 0;														// �I��No������
	op->onxt = Gc_op, Gc_op = op;										// Gc�擪�ڑ�
	if(flg) {															// <Undo/Redo����>
		if(Undo_ary[Undo_adr].cop==op) {								// <���ِ擪�̏ꍇ>
			Undo_ary[Undo_adr].cop = op->cnxt;							// �����̐擪�ڑ�
		} else {														// <���ِ擪�łȂ��ꍇ>
			if((optr=Undo_ary[Undo_adr].cop)!=NULL) {					// �擪��藧�̌���
				while(optr->cnxt!=NULL&&optr->cnxt!=op) {
					optr = optr->cnxt;
				}
				if(optr->cnxt==op) {
					optr->cnxt = op->cnxt;								// �w��O��ڑ�
				}
			}
		}
	}
	if(op->vnum!=0) {													// �̈���(���_�ް�)
		src = op->vsa+op->vnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-op->vnum] = Pnt_ary[i];
		}
		Pnt_adr -= op->vnum;
	}
	if(op->lnum1+op->lnum2!=0) {										// �̈���(��ٰ��)
		src = op->lsa+(op->lnum1+op->lnum2);
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-(op->lnum1+op->lnum2)] = Int_ary[i];
		}
		Int_adr -= (op->lnum1+op->lnum2);
	}
	ChangeAreaAdr(op->lsa, -(op->lnum1+op->lnum2), op->vsa, -op->vnum);	// �z����ڽ�ύX
	FreeOif(0, op->oift);												// ���̍\�����̈���
}

/*************************/
void UndoObj(OBJTYPE* op)												//<<< �����ް�����
/*************************/
{
	OBJTYPE* optr;

	if(Temp_op==op) {													// <�擪���̂̏ꍇ>
		Temp_op = op->onxt;												// ������Temp�ڑ�
	} else {															// <�擪���̈ȊO�̏ꍇ>
		if((optr=Temp_op)==NULL) {
			return;
		}
		while(optr->onxt!=NULL&&optr->onxt!=op) {
			optr = optr->onxt;											// �O���̌���
		}
		if(optr->onxt==NULL) {
			return;														// �w�藧�̖����~
		}
		optr->onxt = op->onxt;											// �O���̎����̐ڑ�
	}
	op->dspflg = 1;														// dspflg:ON
	op->edtflg = 0;														// edtflg:OFF
	op->onxt = NULL;
	if(Base_op==NULL) {
		Base_op = op, op->oset = NULL;
	} else {
		optr = Base_op;
		while(optr->onxt!=NULL) {
			optr = optr->onxt;											// �ŏI���̌���
		}
		optr->onxt = op;												// �ŏI����ؽĐڑ�
		if(op->oset!=optr&&op->oset!=optr->oset) {
			op->oset = NULL;
		}
	}
	UndoOif(op->oift);													// ���̍\����񕜌�
}

/***********************************/
void LevelObj(int flg, OBJTYPE* op)										//<<< �������ٗ��̐ڑ�
/***********************************/
{
	OBJTYPE* optr;
	int      wadr;

	if(Undo_stp==1) {
		return;															// Undo�ۑ���~�����~
	}
	if(Reset_op!=NULL) {
		return;															// �ڑ��ϒ��~
	}
	if(!flg) {															// <flg=0 �������̐ڑ�>
		op->cnxt = NULL;
		if(Undo_ary[Undo_adr].cop==NULL) {
			Undo_ary[Undo_adr].cop = op;
		} else {
			optr = Undo_ary[Undo_adr].cop;
			while(optr->cnxt!=NULL) {
				optr = optr->cnxt;										// �ŏI���̌���
			}
			optr->cnxt = op;											// �ŏI����ؽĐڑ�
		}
	} else {															// <flg=1 �폜���̐ڑ�>
		op->dnxt = NULL;
		if(Undo_ary[Undo_adr].dop==NULL) {
			Undo_ary[Undo_adr].dop = op;
		} else {
			optr = Undo_ary[Undo_adr].dop;
			while(optr->dnxt!=NULL) {
				optr = optr->dnxt;										// �ŏI���̌���
			}
			optr->dnxt = op;											// �ŏI����ؽĐڑ�
		}
	}
	Undo_ary[Undo_adr].urflg = 1;										// �����ƭ�Undo��
	Undo_ary[Undo_adr].cmflg = 0;										// �O���ƭ�Redo��
	if((wadr=Undo_adr+1)>Undo_max-1) {
		wadr = 0;
	}
	Undo_ary[wadr].cmflg = 1;											// �����ƭ�Redo�s��
}

/**********************************************************/
void BaseObjPtr(int flg, OBJTYPE** op, int* dsp, int* edt)				//<<< �����ް��߲���擾
/**********************************************************/
{
	OBJTYPE* optr;

	optr = Base_op, *op = NULL;
	if(flg==0) {														// <�S���̎w��̏ꍇ>
		*op = Base_op;
	} else if(flg==1) {													// <dsp:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->dspflg==1) {										// dsp:ON����
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	} else {															// <edt:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->edtflg==1) {										// edt:ON����
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	}
	if(*op!=NULL) {
		*dsp = (int)((*op)->dspflg);
		*edt = (int)((*op)->edtflg);
	}
}

/**************************************/
BOOL CheckObjPtr(int flg, OBJTYPE* op)									//<<< �w�藧�̑��݊m�F
/**************************************/
{
	OBJTYPE* optr;

	optr = Base_op;														// Base��藧�̌���
	while(optr!=NULL) {
		if(optr==op) {													// ���̈�v
			if(flg==0) {												// <���������̂̏ꍇ>
				return TRUE;
			} else if(flg==1) {											// <dsp:ON���̂̏ꍇ>
				if(op->dspflg==1) {
					return TRUE;
				} else {
					return FALSE;
				}
			} else {													// <edt:ON���̂̏ꍇ>
				if(op->edtflg==1) {
					return TRUE;
				} else {
					return FALSE;
				}
			}
		}
		optr = optr->onxt;
	}
	return FALSE;
}

/**********************************************************/
void NextObjPtr(int flg, OBJTYPE** op, int* dsp, int* edt)				//<<< �������߲���ړ�
/**********************************************************/
{
	OBJTYPE* optr;

	if(*op==NULL) {
		return;
	}
	optr = (*op)->onxt, *op = NULL;
	if(flg==0) {														// <�S���̎w��̏ꍇ>
		*op = optr;
	} else if(flg==1) {													// <dsp:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->dspflg==1) {										// dsp:ON����
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	} else {															// <edt:ON���̎w��̏ꍇ>
		while(optr!=NULL) {
			if(optr->edtflg==1) {										// edt:ON����
				*op = optr; break;
			}
			optr = optr->onxt;
		}
	}
	if(*op!=NULL) {
		*dsp = (int)((*op)->dspflg);
		*edt = (int)((*op)->edtflg);
	}
}

/****************************************/
static unsigned int GetGroupFlg(int gno)								//<<< ��ٰ���׸ގ擾
/****************************************/
{
	unsigned int gflg=0;

	if(gno==0) {
		gflg = 0x1;
	} else if(gno==1) {
		gflg = 0x2;
	} else if(gno==2) {
		gflg = 0x4;
	} else if(gno==3) {
		gflg = 0x8;
	} else if(gno==4) {
		gflg = 0x10;
	} else if(gno==5) {
		gflg = 0x20;
	} else if(gno==6) {
		gflg = 0x40;
	} else if(gno==7) {
		gflg = 0x80;
	} else if(gno==8) {
		gflg = 0x100;
	} else if(gno==9) {
		gflg = 0x200;
	} else if(gno==10) {
		gflg = 0x400;
	} else if(gno==11) {
		gflg = 0x800;
	} else if(gno==12) {
		gflg = 0x1000;
	} else if(gno==13) {
		gflg = 0x2000;
	} else if(gno==14) {
		gflg = 0x4000;
	} else if(gno==15) {
		gflg = 0x8000;
	} else if(gno==16) {
		gflg = 0x10000;
	} else if(gno==17) {
		gflg = 0x20000;
	} else if(gno==18) {
		gflg = 0x40000;
	} else if(gno==19) {
		gflg = 0x80000;
	} else if(gno==20) {
		gflg = 0x100000;
	} else if(gno==21) {
		gflg = 0x200000;
	} else if(gno==22) {
		gflg = 0x400000;
	} else if(gno==23) {
		gflg = 0x800000;
	} else if(gno==24) {
		gflg = 0x1000000;
	} else if(gno==25) {
		gflg = 0x2000000;
	} else if(gno==26) {
		gflg = 0x4000000;
	} else if(gno==27) {
		gflg = 0x8000000;
	} else if(gno==28) {
		gflg = 0x10000000;
	} else if(gno==29) {
		gflg = 0x20000000;
	} else if(gno==30) {
		gflg = 0x40000000;
	} else if(gno==31) {
		gflg = 0x80000000;
	}
	return gflg;
}

/*******************************************/
void GroupBaseObjPtr(int gno, OBJTYPE** op)								//<<< �����ް��߲���擾
/*******************************************/							//    (group)
{
	OBJTYPE* optr;
	unsigned int gflg, grp;

	gflg = GetGroupFlg(gno);											// ��ٰ���׸ގ擾
	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		grp = optr->group;
		if(grp&gflg) {													// �w���ٰ�ߗ��̌���
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void GroupNextObjPtr(int gno, OBJTYPE** op)								//<<< �������߲���ړ�
/*******************************************/							//    (group)
{
	OBJTYPE* optr;
	unsigned int gflg, grp;

	if(*op==NULL) {
		return;
	}
	gflg = GetGroupFlg(gno);											// ��ٰ���׸ގ擾
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		grp = optr->group;
		if(grp&gflg) {													// �w���ٰ�ߗ��̌���
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void LevelBaseObjPtr(int flg, OBJTYPE** op)								//<<< �������ِ擪���̎擾
/*******************************************/
{
	*op = (!flg) ? Undo_ary[Undo_adr].cop : Undo_ary[Undo_adr].dop;		// �������̐ڑ�/�폜���̐ڑ�
}

/*******************************************/
void LevelNextObjPtr(int flg, OBJTYPE** op)								//<<< �������َ����̈ړ�
/*******************************************/
{
	if(!flg) {															// <flg=0 �������̐ڑ�>
		if(*op!=NULL) {
			*op = (*op)->cnxt;
		}
	} else {															// <flg=1 �폜���̐ڑ�>
		if(*op!=NULL) {
			*op = (*op)->dnxt;
		}
	}
}

/*******************************************/
void ColorBaseObjPtr(int col, OBJTYPE** op)								//<<< �װ�ʗ����ް��߲���擾
/*******************************************/
{
	OBJTYPE* optr;

	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		if(optr->dspflg==1&&optr->color==(BYTE)col) {					// dsp:ON�Ŷװ��v����
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*******************************************/
void ColorNextObjPtr(int col, OBJTYPE** op)								//<<< �װ�ʎ������߲���ړ�
/*******************************************/
{
	OBJTYPE* optr;

	if(*op==NULL) {
		return;
	}
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		if(optr->dspflg==1&&optr->color==(BYTE)col) {					// dsp:ON�Ŷװ��v����
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/*****************************************/
void SelBaseObjPtr(int flg, OBJTYPE** op)								//<<< �����ް��߲���擾
/*****************************************/								//    (�I�𗧑�)
{
	OBJTYPE* optr;
	int      sno1, sno2;

	if(flg==99) {														//<�߰�Ď���1or3>
		sno1 = 1,   sno2 = 3;
	} else {															//<�ȊO�͎w��l>
		sno1 = flg, sno2 = flg;
	}
	optr = Base_op, *op = NULL;
	while(optr!=NULL) {
		if(sno1==optr->selno||sno2==optr->selno) {						// �w��I�𗧑̌���
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/****************************************/
void SelNextObjPtr(int flg, OBJTYPE** op)								//<<< �������߲���ړ�
/****************************************/								//    (�I�𗧑�)
{
	OBJTYPE* optr;
	int      sno1, sno2;

	if(*op==NULL) {
		return;
	}
	if(flg==99) {														//<�߰�Ď���1or3>
		sno1 = 1,   sno2 = 3;
	} else {															//<�ȊO�͎w��l>
		sno1 = flg, sno2 = flg;
	}
	optr = (*op)->onxt, *op = NULL;
	while(optr!=NULL) {
		if(sno1==optr->selno||sno2==optr->selno) {						// �w��I�𗧑̌���
			*op = optr; break;
		}
		optr = optr->onxt;
	}
}

/***************************/
void SetObjPtr(OBJTYPE* op)												//<<< �����Ώۗ��̐ݒ�
/***************************/
{
	Rf_op = op;
}

/****************************/
void GetObjPtr(OBJTYPE** op)											//<<< �����Ώۗ��̎擾
/****************************/
{
	*op = Rf_op;
}

/***********************************/
void OnDspFlg(int flg, OBJTYPE* op)										//<<< �\��(�L��)�׸�ON
/***********************************/
{
	op->dspflg = 1;
	if(flg) {
		View->UpdateDspFlg(1, op);										// ���̍\�����\����ԍX�V
	}
}

/************************************/
void OffDspFlg(int flg, OBJTYPE* op)									//<<< �\��(�L��)�׸�OFF
/************************************/
{
	op->dspflg = 0;
	if(flg) {
		View->UpdateDspFlg(0, op);										// ���̍\�����\����ԍX�V
	}
}

/***********************************/
void OnEdtFlg(int flg, OBJTYPE* op)										//<<< �ҏW�Ώ��׸�ON
/***********************************/
{
	op->edtflg = 1;
	if(flg) {
		View->UpdateEdtFlg(1, op);										// ���̍\�����I����ԍX�V
	}
}

/************************************/
void OffEdtFlg(int flg, OBJTYPE* op)									//<<< �ҏW�Ώ��׸�OFF
/************************************/
{
	op->edtflg = 0;
	if(flg) {
		View->UpdateEdtFlg(0, op);										// ���̍\�����I����ԍX�V
	}
}

/**********************************/
void SetObjNo(OBJTYPE* op, int no)										//<<< ����No�ݒ�
/**********************************/
{
	op->ono = no;
}

/*************************/
int GetObjNo(OBJTYPE* op)												//<<< ����No�擾
/*************************/
{
	return op->ono;
}

/****************************/
void ChangeTestFlg(int mode)											//<<< ýėp�׸ޕύX
/****************************/
{
	OBJTYPE* op;

	op = Base_op;
	if(mode==0) {														// <ýĊJ�n����>
		while(op!=NULL) {
			op->dspflg = (op->dspflg==1) ? 2 : op->dspflg;
			op->edtflg = (op->edtflg==1) ? 2 : op->edtflg;
			op = op->onxt;
		}
	} else {															// <ýďI������>
		while(op!=NULL) {
			op->dspflg = (op->dspflg==2) ? 1 : op->dspflg;
			op->edtflg = (op->edtflg==2) ? 1 : op->edtflg;
			op = op->onxt;
		}
	}
}

/************************************/
void PutObjAtr(OBJTYPE* op, int atr)									//<<< ����No�ݒ�
/************************************/
{
	if(Reset_op==NULL||op->attr!=1) {
		op->attr = (BYTE)atr;
	}
}

/**************************/
int GetObjAtr(OBJTYPE* op)												//<<< ����No�擾
/**************************/
{
	return (int)op->attr;
}

/*********************************************/
void PutObjPrt(OBJTYPE* op, int prt, int upd)							//<<< �߰�No�ݒ�
/*********************************************/
{
	if(Reset_op==NULL) {
		op->part = (BYTE)prt;
		if(prt==1) {													// �Ưĥ�eop�ۑ�
			op->oset = NULL;
			Oset_op = op;
		} else if(prt==2) {												// �Ưĥ�q�͐eop�ݒ�
			op->oset = Oset_op;
			op->part = (Oset_op==NULL) ? 0 : 2;							// �e�s�����Ưĉ���
		} else {
			op->oset = NULL;
			if(upd==1) {
				Oset_op = NULL;											// �ƯĈȊO�͐eopؾ��
			}
		}
	}
}

/**************************/
int GetObjPrt(OBJTYPE* op)												//<<< �߰�No�擾
/**************************/
{
	return (int)op->part;
}

/************************************/
void PutObjCol(OBJTYPE* op, int col)									//<<< �װNo�ݒ�
/************************************/
{
	if(Reset_op==NULL) {
		op->color = (BYTE)col;
	}
}

/**************************/
int GetObjCol(OBJTYPE* op)												//<<< �װNo�擾
/**************************/
{
	return (int)op->color;
}

/**********************************************/
void PutListNo(OBJTYPE* op, unsigned int list)							//<<< �ި���ڲ�ؽ�No�ݒ�
/**********************************************/
{
	op->list = list;
}

/***********************************/
unsigned int GetListNo(OBJTYPE* op)										//<<< �ި���ڲ�ؽ�No�擾
/***********************************/
{
	return op->list;
}

/***************************************/
void PutListMode(OBJTYPE* op, int mode)									//<<< ؽ�Ӱ�ސݒ�
/***************************************/
{
	op->lmode = (BYTE)mode;
}

/****************************/
int GetListMode(OBJTYPE* op)											//<<< ؽ�Ӱ�ގ擾
/****************************/
{
	return (int)op->lmode;
}

/************************************/
void PutGroup(OBJTYPE* op, int* gno)									//<<< ���̸�ٰ�ߏ��ݒ�
/************************************/
{
	unsigned int gflg;

	gflg = (gno[0]) ? 0x1 : 0;
	gflg = (gno[1]) ? gflg|0x2 : gflg;
	gflg = (gno[2]) ? gflg|0x4 : gflg;
	gflg = (gno[3]) ? gflg|0x8 : gflg;
	gflg = (gno[4]) ? gflg|0x10 : gflg;
	gflg = (gno[5]) ? gflg|0x20 : gflg;
	gflg = (gno[6]) ? gflg|0x40 : gflg;
	gflg = (gno[7]) ? gflg|0x80 : gflg;
	gflg = (gno[8]) ? gflg|0x100 : gflg;
	gflg = (gno[9]) ? gflg|0x200 : gflg;
	gflg = (gno[10]) ? gflg|0x400 : gflg;
	gflg = (gno[11]) ? gflg|0x800 : gflg;
	gflg = (gno[12]) ? gflg|0x1000 : gflg;
	gflg = (gno[13]) ? gflg|0x2000 : gflg;
	gflg = (gno[14]) ? gflg|0x4000 : gflg;
	gflg = (gno[15]) ? gflg|0x8000 : gflg;
	gflg = (gno[16]) ? gflg|0x10000 : gflg;
	gflg = (gno[17]) ? gflg|0x20000 : gflg;
	gflg = (gno[18]) ? gflg|0x40000 : gflg;
	gflg = (gno[19]) ? gflg|0x80000 : gflg;
	gflg = (gno[20]) ? gflg|0x100000 : gflg;
	gflg = (gno[21]) ? gflg|0x200000 : gflg;
	gflg = (gno[22]) ? gflg|0x400000 : gflg;
	gflg = (gno[23]) ? gflg|0x800000 : gflg;
	gflg = (gno[24]) ? gflg|0x1000000 : gflg;
	gflg = (gno[25]) ? gflg|0x2000000 : gflg;
	gflg = (gno[26]) ? gflg|0x4000000 : gflg;
	gflg = (gno[27]) ? gflg|0x8000000 : gflg;
	gflg = (gno[28]) ? gflg|0x10000000 : gflg;
	gflg = (gno[29]) ? gflg|0x20000000 : gflg;
	gflg = (gno[30]) ? gflg|0x40000000 : gflg;
	gflg = (gno[31]) ? gflg|0x80000000 : gflg;
	if(Reset_op==NULL) {
		op->group = gflg;
	}
}

/************************************/
void GetGroup(OBJTYPE* op, int* gno)									//<<< ���̸�ٰ�ߏ��擾
/************************************/
{
	unsigned int gflg;

	gflg = op->group;
	gno[0] = (gflg&0x1) ? 1 : 0;
	gno[1] = (gflg&0x2) ? 1 : 0;
	gno[2] = (gflg&0x4) ? 1 : 0;
	gno[3] = (gflg&0x8) ? 1 : 0;
	gno[4] = (gflg&0x10) ? 1 : 0;
	gno[5] = (gflg&0x20) ? 1 : 0;
	gno[6] = (gflg&0x40) ? 1 : 0;
	gno[7] = (gflg&0x80) ? 1 : 0;
	gno[8] = (gflg&0x100) ? 1 : 0;
	gno[9] = (gflg&0x200) ? 1 : 0;
	gno[10] = (gflg&0x400) ? 1 : 0;
	gno[11] = (gflg&0x800) ? 1 : 0;
	gno[12] = (gflg&0x1000) ? 1 : 0;
	gno[13] = (gflg&0x2000) ? 1 : 0;
	gno[14] = (gflg&0x4000) ? 1 : 0;
	gno[15] = (gflg&0x8000) ? 1 : 0;
	gno[16] = (gflg&0x10000) ? 1 : 0;
	gno[17] = (gflg&0x20000) ? 1 : 0;
	gno[18] = (gflg&0x40000) ? 1 : 0;
	gno[19] = (gflg&0x80000) ? 1 : 0;
	gno[20] = (gflg&0x100000) ? 1 : 0;
	gno[21] = (gflg&0x200000) ? 1 : 0;
	gno[22] = (gflg&0x400000) ? 1 : 0;
	gno[23] = (gflg&0x800000) ? 1 : 0;
	gno[24] = (gflg&0x1000000) ? 1 : 0;
	gno[25] = (gflg&0x2000000) ? 1 : 0;
	gno[26] = (gflg&0x4000000) ? 1 : 0;
	gno[27] = (gflg&0x8000000) ? 1 : 0;
	gno[28] = (gflg&0x10000000) ? 1 : 0;
	gno[29] = (gflg&0x20000000) ? 1 : 0;
	gno[30] = (gflg&0x40000000) ? 1 : 0;
	gno[31] = (gflg&0x80000000) ? 1 : 0;
}

/*********************************************/
void SetGroupNo(OBJTYPE* op, int no, int flg)							//<<< ��ٰ�ߔԍ��ݒ�
/*********************************************/
{
	unsigned int gon, goff, gno;

	if(no==0) {
		gon = 0x1, goff = ~(0x1);
	} else if(no==1) {
		gon = 0x2, goff = ~(0x2);
	} else if(no==2) {
		gon = 0x4, goff = ~(0x4);
	} else if(no==3) {
		gon = 0x8, goff = ~(0x8);
	} else if(no==4) {
		gon = 0x10, goff = ~(0x10);
	} else if(no==5) {
		gon = 0x20, goff = ~(0x20);
	} else if(no==6) {
		gon = 0x40, goff = ~(0x40);
	} else if(no==7) {
		gon = 0x80, goff = ~(0x80);
	} else if(no==8) {
		gon = 0x100, goff = ~(0x100);
	} else if(no==9) {
		gon = 0x200, goff = ~(0x200);
	} else if(no==10) {
		gon = 0x400, goff = ~(0x400);
	} else if(no==11) {
		gon = 0x800, goff = ~(0x800);
	} else if(no==12) {
		gon = 0x1000, goff = ~(0x1000);
	} else if(no==13) {
		gon = 0x2000, goff = ~(0x2000);
	} else if(no==14) {
		gon = 0x4000, goff = ~(0x4000);
	} else if(no==15) {
		gon = 0x8000, goff = ~(0x8000);
	} else if(no==16) {
		gon = 0x10000, goff = ~(0x10000);
	} else if(no==17) {
		gon = 0x20000, goff = ~(0x20000);
	} else if(no==18) {
		gon = 0x40000, goff = ~(0x40000);
	} else if(no==19) {
		gon = 0x80000, goff = ~(0x80000);
	} else if(no==20) {
		gon = 0x100000, goff = ~(0x100000);
	} else if(no==21) {
		gon = 0x200000, goff = ~(0x200000);
	} else if(no==22) {
		gon = 0x400000, goff = ~(0x400000);
	} else if(no==23) {
		gon = 0x800000, goff = ~(0x800000);
	} else if(no==24) {
		gon = 0x1000000, goff = ~(0x1000000);
	} else if(no==25) {
		gon = 0x2000000, goff = ~(0x2000000);
	} else if(no==26) {
		gon = 0x4000000, goff = ~(0x4000000);
	} else if(no==27) {
		gon = 0x8000000, goff = ~(0x8000000);
	} else if(no==28) {
		gon = 0x10000000, goff = ~(0x10000000);
	} else if(no==29) {
		gon = 0x20000000, goff = ~(0x20000000);
	} else if(no==30) {
		gon = 0x40000000, goff = ~(0x40000000);
	} else if(no==31) {
		gon = 0x80000000, goff = ~(0x80000000);
	}
	gno = op->group;
	gno = (!flg) ? gno&goff : gno|gon;
	op->group = gno;
}

/***********************************/
void SetSelNo(OBJTYPE* op, int flg)										//<<< �I��No�ݒ�
/***********************************/
{
	op->selno = (BYTE)flg;
	SelectNo = flg;
}

/*************************/
int GetSelNo(OBJTYPE* op)												//<<< �I��No�擾
/*************************/
{
	return (int)op->selno;
}

/*********************/
int GetSelectNo(void)													//<<< ���̑I��No�擾
/*********************/
{
	return SelectNo;
}

/*********************/
void ResetSelNo(void)													//<<< �I��Noؾ��
/*********************/
{
	OBJTYPE* op;

	op = Base_op;
	while(op!=NULL) {
		op->selno = 0;													// �I��Noؾ��
		op = op->onxt;
	}
	SelectNo = 0;
}

/**************************/
int GetVtxNum(OBJTYPE* op)												//<<< ���̒��_���擾
/**************************/
{
	return op->vnum;
}

/*********************************************/
void PutVtx(OBJTYPE* op, int no, PNTTYPE pnt)							//<<< �w�蒸�_�ݒ�
/*********************************************/
{
	Pnt_ary[op->vsa+no] = pnt;
}

/**********************************************/
void GetVtx(OBJTYPE* op, int no, PNTTYPE* pnt)							//<<< �w�蒸�_�擾
/**********************************************/
{
	*pnt = Pnt_ary[op->vsa+no];
}

/***************************/
int GetFlpNum1(OBJTYPE* op)												//<<< ���̖�ٰ�ߐ��擾1
/***************************/
{
	return op->lnum1;
}

/***************************************************/
void PutFlp1(OBJTYPE* op, int no, int vno, int flg)						//<<< �w���ٰ�ߔԍ��ݒ�1
/***************************************************/
{
	Int_ary[op->lsa+no] = (!flg) ? vno+1 : -(vno+1);
}

/*****************************************************/
void GetFlp1(OBJTYPE* op, int no, int* vno, int* flg)					//<<< �w���ٰ�ߔԍ��擾1
/*****************************************************/
{
	if(Int_ary[op->lsa+no]>0) {
		*flg = 0, *vno = Int_ary[op->lsa+no]-1;
	} else {
		*flg = 1, *vno = -(Int_ary[op->lsa+no]+1);
	}
}

/************************************************************/
void GetFlpVtx1(OBJTYPE* op, int no, PNTTYPE* pnt, int* flg)			//<<< �w���ٰ�ߍ��W�擾1
/************************************************************/
{
	if(Int_ary[op->lsa+no]>0) {
		*flg = 0, *pnt = Pnt_ary[op->vsa+Int_ary[op->lsa+no]-1];
	} else {
		*flg = 1, *pnt = Pnt_ary[op->vsa-(Int_ary[op->lsa+no]+1)];
	}
}

/***************************/
int GetFlpNum2(OBJTYPE* op)												//<<< ���̖�ٰ�ߐ��擾2
/***************************/
{
	return op->lnum2;
}

/***************************************************/
void PutFlp2(OBJTYPE* op, int no, int vno, int flg)						//<<< �w���ٰ�ߔԍ��ݒ�2
/***************************************************/
{
	Int_ary[(op->lsa+op->lnum1)+no] = (!flg) ? vno+1 : -(vno+1);
}

/*****************************************************/
void GetFlp2(OBJTYPE* op, int no, int* vno, int* flg)					//<<< �w���ٰ�ߔԍ��擾2
/*****************************************************/
{
	if(Int_ary[(op->lsa+op->lnum1)+no]>0) {
		*flg = 0, *vno = Int_ary[(op->lsa+op->lnum1)+no]-1;
	} else {
		*flg = 1, *vno = -(Int_ary[(op->lsa+op->lnum1)+no]+1);
	}
}

/************************************************************/
void GetFlpVtx2(OBJTYPE* op, int no, PNTTYPE* pnt, int* flg)			//<<< �w���ٰ�ߍ��W�擾2
/************************************************************/
{
	if(Int_ary[(op->lsa+op->lnum1)+no]>0) {
		*flg = 0, *pnt = Pnt_ary[op->vsa+Int_ary[(op->lsa+op->lnum1)+no]-1];
	} else {
		*flg = 1, *pnt = Pnt_ary[op->vsa-(Int_ary[(op->lsa+op->lnum1)+no]+1)];
	}
}

/***************************/
void SetObjBox(OBJTYPE* op)												//<<< �����ޯ���ݒ�
/***************************/
{
	PNTTYPE mip, map, pnt;
	int     i, j, num;

	num = GetVtxNum(op);
	GetVtx(op, 0, &pnt);
	mip = pnt, map = pnt;
	for(i=1; i<num; i++) {
		GetVtx(op, i, &pnt);
		for(j=0; j<DIM; j++) {
			mip.p[j] = (mip.p[j]>pnt.p[j]) ? pnt.p[j] : mip.p[j];
			map.p[j] = (map.p[j]<pnt.p[j]) ? pnt.p[j] : map.p[j];
		}
	}
	op->box.bp[0] = mip, op->box.bp[1] = map;
}

/*****************************************/
void GetObjBox(OBJTYPE* op, BOXTYPE* box)								//<<< �����ޯ���擾
/*****************************************/
{
	*box = op->box;
}

/********************/
void SetScrBox(void)													//<<< ��ذ��ޯ���ݒ�
/********************/
{
	OBJTYPE* op;
	PNTTYPE  mip, map;
	BOXTYPE  box;
	int      i, dsp, edt;

	for(i=0; i<DIM; i++) {
		Scr_Box.bp[0].p[i] = 0.0;
		Scr_Box.bp[1].p[i] = 0.0;
	}
	BaseObjPtr(0, &op, &dsp, &edt);
	if(op==NULL) {
		return;
	}
	GetObjBox(op, &box);
	mip = box.bp[0], map = box.bp[1];
	NextObjPtr(0, &op, &dsp, &edt);
	while(op!=NULL) {
		GetObjBox(op, &box);
		for(i=0; i<DIM; i++) {
			mip.p[i] = (mip.p[i]>box.bp[0].p[i]) ? box.bp[0].p[i] : mip.p[i];
			map.p[i] = (map.p[i]<box.bp[1].p[i]) ? box.bp[1].p[i] : map.p[i];
		}
		NextObjPtr(0, &op, &dsp, &edt);
	}
	Scr_Box.bp[0] = mip, Scr_Box.bp[1] = map;
}

/****************************/
void GetScrBox(BOXTYPE* box)											//<<< ��ذ��ޯ���擾
/****************************/
{
	*box = Scr_Box;
}

/**************************************/
void GetAreaBox(int flg, BOXTYPE* box)									//<<< �ر�ޯ���擾
/**************************************/
{
	OBJTYPE* op;
	PNTTYPE  mip, map;
	BOXTYPE  wbx;
	int      i, dsp, edt;

	BaseObjPtr(flg, &op, &dsp, &edt);
	if(op==NULL) {
		return;
	}
	GetObjBox(op, &wbx);
	mip = wbx.bp[0], map = wbx.bp[1];
	NextObjPtr(flg, &op, &dsp, &edt);
	while(op!=NULL) {
		GetObjBox(op, &wbx);
		for(i=0; i<DIM; i++) {
			mip.p[i] = (mip.p[i]>wbx.bp[0].p[i]) ? wbx.bp[0].p[i] : mip.p[i];
			map.p[i] = (map.p[i]<wbx.bp[1].p[i]) ? wbx.bp[1].p[i] : map.p[i];
		}
		NextObjPtr(flg, &op, &dsp, &edt);
	}
	box->bp[0] = mip, box->bp[1] = map;
}

/******************************************/
void GetUnitBox(OBJTYPE* op, BOXTYPE* box)								//<<< �Ưė����ޯ���擾
/******************************************/
{
	OBJTYPE* optr, * oya;
	PNTTYPE  mip, map;
	BOXTYPE  wbx;
	int      i, dsp, edt;

	if(op==NULL) {														// ���̖��͏I��
		return;
	}
	if(op->edtflg!=1) {													// �ҏW�ΏۊO�͏I��
		return;
	}
	GetObjBox(op, box);													// �����ޯ���擾
	if(op->part!=0) {													// <<�ƯėL>>
		wbx = *box;
		if(op->part==1) {												// <�e�̏ꍇ>
			oya = op;													// �����߲�����e�߲��
		} else {														// <�q�̏ꍇ>
			if(op->oset==NULL) {
				oya = op;
			} else {
				oya = op->oset;
			}
		}
		if(oya->edtflg==1) {
			GetObjBox(oya, &wbx);										// �e�����ޯ���擾
		}
		mip = wbx.bp[0], map = wbx.bp[1];
		BaseObjPtr(2, &optr, &dsp, &edt);
		while(optr!=NULL) {
			if(optr->oset==oya) {										// <�e���̂��o�^����Ă���q����>
				GetObjBox(optr, &wbx);
				for(i=0; i<DIM; i++) {
					mip.p[i] = (mip.p[i]>wbx.bp[0].p[i]) ? wbx.bp[0].p[i] : mip.p[i];
					map.p[i] = (map.p[i]<wbx.bp[1].p[i]) ? wbx.bp[1].p[i] : map.p[i];
				}
			}
			NextObjPtr(2, &optr, &dsp, &edt);
		}
		box->bp[0] = mip, box->bp[1] = map;
	}
}

/********************************************************************/
void ChangeObjVtx(int md, OBJTYPE* op, double x, double y, double z)	//<<< ���̒��_�ύX
/********************************************************************/
{
	PNTTYPE pnt;
	int     i, num;

	num = GetVtxNum(op);												// ���̒��_���擾
	for(i=0; i<num; i++) {
		GetVtx(op, i, &pnt);											// ���_���W�擾
		if(md==0) {
			pnt.p[0] = pnt.p[0] + x;									// ���_�ް��ύX
			pnt.p[1] = pnt.p[1] + y;
			pnt.p[2] = pnt.p[2] + z;
		} else {
			pnt.p[0] = pnt.p[0] - x;									// ���_�ް�����
			pnt.p[1] = pnt.p[1] - y;
			pnt.p[2] = pnt.p[2] - z;
		}
		PutVtx(op, i, pnt);												// ���_���W�ݒ�
	}
	SetObjBox(op);														// �����ޯ�����ސݒ�
}

/*******************/
void InitCVtx(void)														//<<< ���_��ď�������
/*******************/
{
	Vp = NULL;
	Now_no = 0;
}

/************************/
void SetCVtx(PNTTYPE pt)												//<<< ��Ē��_�ݒ�
/************************/
{
	VTYPE* tv;

	tv = (VTYPE *)malloc(sizeof(VTYPE));								// ��ė̈�m��
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->next = Vp;
	tv->no = Now_no;
	tv->pt = pt;
	tv->vt = -1;
	tv->flg = FALSE;
	Vp = tv;
	Now_no++;
}

/*******************/
void SetCFace(void)														//<<< ��ٰ�ߏI���ݒ�
/*******************/
{
	Vp->flg = TRUE;
}

/*******************************************/
static VTYPE* MergeVx(VTYPE* la, VTYPE* lb)								//<<< ���_���W:X���
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[0]<=lb->pt.p[0]) {									// ���_���W:X�召��r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtx(VTYPE* mlist, int n)							//<<< ���_���W:X���
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// ���_���W:X���
		lm = (VTYPE *)MergeVx(V_Mergsrtx(la, n/2), V_Mergsrtx(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVy(VTYPE* la, VTYPE* lb)								//<<< ���_���W:Y���
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[1]<=lb->pt.p[1]) {									// ���_���W:Y�召��r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrty(VTYPE* mlist, int n)							//<<< ���_���W:Y���
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// ���_���W:Y���
		lm = (VTYPE *)MergeVy(V_Mergsrty(la, n/2), V_Mergsrty(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVz(VTYPE* la, VTYPE* lb)								//<<< ���_���W:Z���
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[2]<=lb->pt.p[2]) {									// ���_���W:Z�召��r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtz(VTYPE* mlist, int n)							//<<< ���_���W:Z���
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// ���_���W:Z���
		lm = (VTYPE *)MergeVz(V_Mergsrtz(la, n/2), V_Mergsrtz(lb,n-n/2));
		rc = lm;
	}
	return rc;
}

/*******************************************/
static VTYPE* MergeVn(VTYPE* la, VTYPE* lb)								//<<< ��ٰ�ߔԍ����
/*******************************************/
{
	VTYPE* lc, * ld, * rc;

	lc = (VTYPE *)malloc(sizeof(VTYPE));								// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// ��ٰ�ߔԍ��召��r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*********************************************/
static VTYPE* V_Mergsrtn(VTYPE* mlist, int n)							//<<< ��ٰ�ߔԍ��̿��
/*********************************************/
{
	VTYPE* la, * lb, * lm, * rc;
	int    i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;												// ��ٰ�ߔԍ����
		lm = (VTYPE *)MergeVn(V_Mergsrtn(la, n/2), V_Mergsrtn(lb, n-n/2));
		rc = lm;
	}
	return rc;
}

/******************/
int SortCVtx(void)														//<<< ���_���̿��&����
/******************/
{
	VTYPE*  wvp;
	PNTTYPE pt;
	int     cnt=0;

	if(Now_no==0) {
		return 0;
	}
	Vp = V_Mergsrtz(Vp, Now_no);										// ���_���W:Z�̿��
	Vp = V_Mergsrty(Vp, Now_no);										// ���_���W:Y�̿��
	Vp = V_Mergsrtx(Vp, Now_no);										// ���_���W:X�̿��
	pt = Vp->pt;														// �����l�ݒ�
	pt.p[0] = pt.p[0]-1.0;
	wvp = Vp;
	while(wvp!=NULL) {													// �ݒ�ςƈقȂ�ꍇ
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {
			wvp->vt = cnt;												// ���_�ԍ��ۑ�
			cnt++;
			pt = wvp->pt;
		} else {
			wvp->vt = cnt-1;											// ���͒��_�ԍ��ۑ�
		}
		wvp = wvp->next;
	}
	return cnt;
}

/****************************/
void SetCVtxObj(OBJTYPE* op)											//<<< ���_���ݒ�
/****************************/
{
	VTYPE*  wvp, * cvp;
	PNTTYPE pt;
	int     lct=0;

	pt = Vp->pt;														// �����l�ݒ�
	pt.p[0] = pt.p[0]-1.0;
	wvp = Vp;
	while(wvp!=NULL) {													// �ݒ�ςƈقȂ�ꍇ
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {
			Pnt_ary[op->vsa+wvp->vt] = wvp->pt;							// ���_���W�ݒ�
			pt = wvp->pt;
		}
		wvp = wvp->next;
	}
	Vp = V_Mergsrtn(Vp, Now_no);										// ��ٰ�ߔԍ����
	wvp = Vp;
	while(wvp!=NULL) {
		Int_ary[op->lsa+lct]=(!wvp->flg) ? wvp->vt+1 : -(wvp->vt+1);	// ��ٰ��1�ݒ�
		lct++;
		wvp = wvp->next;
	}
	wvp = Vp;
	while(wvp) {														// ��ė̈���
		cvp = wvp->next;
		free(wvp);
		wvp = cvp;
	}
}
