/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewSelect.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataFile1.h"
#include "ECommand.h"
#include "RCommand.h"
#include "Select.h"
#include "InputDlg.h"
#include "EditObject.h"
#include "NumFunc.h"

/*******************************************************************/
void CJcad3GlbView::MousePasteObject(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< �߰��
/*******************************************************************/
{
	OBJTYPE* op;
	CPCTYPE* cmp;
	PNTTYPE  pnt, crs, mov, vpt;
	VECTYPE  vec;
	BOXTYPE  box;
	int      i, i1, i2;
	double   th;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==5) {															// <<<�p��������>>>
		if(Si1==1) {													// <<��ׯ�ޒ��̏ꍇ>>
			if(flg=='Z') {												// <10�x��]>
				Sd2 += 10.0;
			} else if(flg=='X') {										// <5�x��]>
				Sd2 += 5.0;
			} else if(flg=='C') {										// <0.5�x��]>
				Sd2 += 0.5;
			} else if(flg=='A') {										// <-10�x��]>
				Sd2 -= 10.0;
			} else if(flg=='S') {										// <-5�x��]>
				Sd2 -= 5.0;
			} else if(flg=='D') {										// <-0.5�x��]>
				Sd2 -= 0.5;
			}
			EditPasteObject2(Si2, Sd2);									// �����߰�Z����]
			Display(0);													// �ĕ`��
		}
	} else if(ms==2) {													// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ����t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			if(!GetObjCrossPnt(plane, pnt, &crs, &vec, &th)) {			// <���̌�_�Ȃ�>
				for(i=0; i<DIM; i++) {
					mov.p[i] = pnt.p[i]-Spnt1.p[i];						// ���̈ړ��ʎZ�o
				}
				EditPasteObject1(Si2, mov, 0.0, pnt, Spnt3);			// �����߰�Ĕz�u(�ړ����])
			} else {													// <���̌�_����>
				for(i=0; i<DIM; i++) {
					mov.p[i] = crs.p[i]-Spnt2.p[i];						// ���̈ړ��ʎZ�o
					vpt.p[i] = vec.p[i];								// �@���޸�ٕϊ�
				}
				EditPasteObject1(Si2, mov, th, crs, vpt);				// �����߰�Ĕz�u(�ړ����])
			}
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(0, &Sbase, point, plane);						// �ړ����_�̍��W�擾
		OffEdtAllObject();												// �ҏW���̑S����
		InitPasteObject(&Si2);											// �����߰�ď�������
		GetAreaBox(2, &box);											// BOX���ގ擾
		BoxCenter(box, &Spnt1);											// BOX�����擾
		Spnt2 = Spnt1;
		if(plane==0) {
			Spnt2.p[2] = box.bp[0].p[2]-Sd1;							// �w�蕽�ʏ��
		} else if(plane==1) {											// BOX������ʓ_
			Spnt2.p[0] = box.bp[0].p[0]-Sd1;
		} else {
			Spnt2.p[1] = box.bp[0].p[1]-Sd1;
		}
		Spnt3.p[0] = 0.0, Spnt3.p[1] = 0.0, Spnt3.p[2] = -1.0;			// ����]���޸��
		if(!GetObjCrossPnt(plane, Sbase, &crs, &vec, &th)) {			// <���̌�_�Ȃ�>
			for(i=0; i<DIM; i++) {
				mov.p[i] = Sbase.p[i] - Spnt1.p[i];						// ���̈ړ��ʎZ�o
			}
			EditPasteObject1(Si2, mov, 0.0, Sbase, Spnt3);				// �����߰�Ĕz�u(�ړ����])
		} else {														// <���̌�_����>
			for(i=0; i<DIM; i++) {
				mov.p[i] = crs.p[i]-Spnt2.p[i];							// ���̈ړ��ʎZ�o
				vpt.p[i] = vec.p[i];									// �@���޸�ٕϊ�
			}
			EditPasteObject1(Si2, mov, th, crs, vpt);					// �����߰�Ĕz�u(�ړ����])
		}
		Display(0);														// �ĕ`��
		Si1 = 1, Sd2 = 0.0;
	} else if(ms==3) {													// <<<ϳ����t���>>>
		EndPasteObject(Si2);											// �����߰�ďI������
		Si1 = 0, Sd2 = 0.0;
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			Si1 = 0, Sd1 = 0.0, Sd2 = 0.0, i = 0;
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			FreeCopyCmd();												// ��߰����ޗ̈���
			ReadCopyFile(m_Char);										// ��߰�ް�̧�ٓ���
			BaseCopyCmdPtr(&cmp);										// �擪��߰������߲���擾
			if(cmp==NULL) {												// <��߰����ނȂ�>
				CountEdtObj();											// �ҏW�Ώۗ��̶���
				ExitMenu(0, 1);											// �ƭ��I��
			} else {													// <��߰����ނ���>
				BaseObjPtr(2, &op, &i1, &i2);							// �ҏW�Ώۗ��̎擾
				while(op!=NULL) {
					if(m_FChar&0x1) {									// [F5]:����ިݸޕ\��
						m_Shading = 2;									// ���ިݸޕ\�����
						ObjectShading(op);								// ����ިݸ�ؽēo�^
					} else {											// �ʏ�\��
						SetSelNo(op, 1);								// �I��No�ݒ�
					}
					OffEdtFlg(1, op);									// �ҏW���̉���
					i++;												// ���̶���
					NextObjPtr(2, &op, &i1, &i2);						// �����̎擾
				}
				if(i>0&&m_NumInp) {										// <�I�𗧑̗L�Ő��l����>
				    CInputDlg dlg(21);
					dlg.SetValue(m_PastePos);							// �����l�\��
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						dlg.GetValue(&m_PastePos);						// �߰�Ċ�ʒu������
						Sd1 = m_PastePos;
					} else {											// �޲�۸޷�ݾ�
						m_Shading = 0;									// ���ިݸޕ\���Ȃ�
						SetRenderMode(0);								// ܲ԰�ڰ�Ӱ��
						ResetSelNo();									// ���̑I��Noؾ��
						CountEdtObj();									// �ҏW�Ώۗ��̶���
						ExitMenu(0, 1);									// �ƭ��I��
					}
				}
			}
		}
		Display(0);														// �ĕ`��
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(ms==4&&(m_CChar&0x1||m_CChar&0x2||m_Char!=0)) {				// <�E�t�د��ŷ�������>
			ReadCopyFile(m_Char);										// ��߰�ް�̧�ٓ���
		} else {														// <��L�ȊO>
			m_Shading = 0;												// ���ިݸޕ\���Ȃ�
			SetRenderMode(0);											// ܲ԰�ڰ�Ӱ��
			ResetSelNo();												// ���̑I��Noؾ��
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			Display(0);													// �ĕ`��
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu2();												// �ƭ��I������
		}
	}
}

/************************************************************/
void CJcad3GlbView::ObjSelect(UINT flg, const CPoint& point,
                              int plane, int ms)						// ���̂̑I��
/************************************************************/
{
	BOXTYPE	area;
	PNTTYPE	cpnt, pnt;
	int     i, ckey;
	double  d[3], dd, dx, dy, dz;

	if(plane!=XY&&plane!=YZ&&plane!=ZX) {
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// <<ϳ����t�����Ȃ�����ׯ��>>
		if(Sphase==2) {													// <���̑I���̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==3||Sphase==4) {								// <���̈ړ��̏ꍇ>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			for(i=0; i<DIM; i++) {
				d[i] = pnt.p[i]-Sbase.p[i];								// ���̈ړ��ʎZ�o
			}
			if(flg&m_LimKey) {
				LimitValue(plane, d);									// ϳ����͐����l
			}
			if(Sphase==3) {												// <�ړ���������>
				StartMenu();											// �ƭ��J�n����
			}
			ObjectMove(Sphase-3, m_CopyNum, Spnt1, Sscr1,
                       d[0], d[1], d[2]);								// ���̕��s�ړ�
			Sphase = (Sphase==3) ? 4 : Sphase;
			dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
			m_pNumDisp->SetParam(d[0], d[1], d[2], dd);					// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Sd1 = d[0], Sd2 = d[1], Sd3 = d[2];							// �ړ��ʕۑ�
		}
	} else if(ms==3) {													// <<ϳ����t���>>
		if(Sphase==2) {													// <���̑I���̏ꍇ>
			if((flg&m_ObjKey)&&(flg&m_NObKey)) {						// �ǉ�=�������͗�������
				ckey = 1;
			} else if(flg&m_ObjKey) {									// �ǉ�����
				ckey = 2;
			} else if(flg&m_NObKey) {									// ��������
				ckey = 3;
			} else {													// ������
				ckey = 0;
			}
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &cpnt, point, plane);				// ���W�_�擾
				area.bp[0] = BaseCursor, area.bp[1] = cpnt;
				EdtAreaObject(plane, area, ckey);
			} else {
				EdtPointObject(plane, BaseCursor, ckey);
			}
			CountEdtObj();
			m_SurfTension = (m_EdtCount==0) ? 0 : m_SurfTension;		// �ҏW�Ώۗ��̖��̏ꍇ
			CheckEdtObjPrm();											// �ҏW�Ώۗ��̐������Ұ�����
		} else if(Sphase==4) {											// <���̈ړ�(M)�̏ꍇ>
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, Sd1, Sd2, Sd3);		// ���s�ړ��I������
			ExitMenu(1, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(Sphase==1) {											// <���̈ړ�(���)�̏ꍇ>
			m_CopyNum = 0, Si1 = 0;										// ���ʐ�:0/�ʒu���[�h:���[�h1
			for(i=0; i<DIM; i++) {
				Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;						// �{��:1.0�{/��_:����
			}
			dx = 0.0, dy = 0.0, dz = 0.0;
			if(m_DChar&0x1) {											// <��������>
				if(plane==ZX) {
					dx = m_MoveAdd;
				} else {
					dy = -m_MoveAdd;
				}
			} else if(m_DChar&0x2) {									// <��������>
				if(plane==XY) {
					dx = -m_MoveAdd;
				} else {
					dz = m_MoveAdd;
				}
			} else if(m_DChar&0x4) {									// <��������>
				if(plane==ZX) {
					dx = -m_MoveAdd;
				} else {
					dy = m_MoveAdd;
				}
			} else if(m_DChar&0x8) {									// <��������>
				if(plane==XY) {
					dx = m_MoveAdd;
				} else {
					dz = -m_MoveAdd;
				}
			}
			StartMenu();												// �ƭ��J�n����
			ObjectMove(0, m_CopyNum, Spnt1, Sscr1, dx, dy, dz);			// ���̕��s�ړ�
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, dx, dy, dz);			// ���s�ړ��I������
			ExitMenu(1, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==1) {													// <<ϳ����t����>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CountEdtObj();													// �I�𗧑̐�����
		if(m_EdtCount>0&&m_Char=='M') {									// <���̈ړ�(M)�̏ꍇ>
			CalcScreenWorld(0, &Sbase, point, plane);					// �ړ����_�̍��W�擾
			Sphase = 3;
			m_CopyNum = 0, Si1 = 0;										// ���ʐ�:0/�ʒu���[�h:���[�h1
			for(i=0; i<DIM; i++) {
				Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;						// �{��:1.0�{/��_:����
			}
			SetNumDisp(2);												// ���l�\����������
		} else if(m_EdtCount>0&&m_DChar!=0) {							// <���̈ړ�(���)�̏ꍇ>
			Sphase = 1;
		} else {														// <���̑I���̏ꍇ>
			Sphase = 2;
		}
	} else if(ms==11&&Sphase==0) {										// <<Delete��/DEL�t>>
		if(GetSelectNo()!=0) {											// <���̑I��No�ݒ��>
			ResetSelNo();												// ���̑I��ؾ��
		} else if(m_EdtCount>0) {										// <�ҏW�Ώۗ��̗L>
			StartMenu();												// �ƭ��J�n����
			InitLevel();												// Undo/Redo������
			Delete();													// ���̂̍폜
			Display(0);
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			ExitMenu(0, 0);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
			m_SurfTension = (m_EdtCount==0) ? 0 : m_SurfTension;		// �ҏW�Ώۗ��̖��̏ꍇ
			MainWnd->SetGroupObj();										// ��ٰ�ߗ��̐ݒ�
		}
		Display(0);
	}
}

/************************************************************/
void CJcad3GlbView::ObjChoose(UINT flg, const CPoint& point,
                              int plane, int ms)						// �I�𗧑̂̕�������
/************************************************************/
{
	int	dsp, edt;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}��̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==0) {
		BaseObjPtr(2, &Sop1, &dsp, &edt);								// �I�𗧑��ް��߲���擾
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// �I��No�ݒ�
		}
	} else if(ms==3) {
		SetSelNo(Sop1, 0);												// �I��No�ݒ�
		NextObjPtr(2, &Sop1, &dsp, &edt);								// ���I�𗧑��߲���擾
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// �I��No�ݒ�
		}
	} else if(ms==4) {
		OffEdtFlg(1, Sop1);												// �ҏW�Ώ��׸�OFF
		SetSelNo(Sop1, 0);												// �I��No�ݒ�
		NextObjPtr(2, &Sop1, &dsp, &edt);								// ���I�𗧑��߲���擾
		if(Sop1!=NULL) {
			SetSelNo(Sop1, 2);											// �I��No�ݒ�
		}
	} else if(ms==9) {
		Sop1 = NULL;
	}
	if(Sop1==NULL) {
		ExitMenu(0, 0);													// �ƭ��I��
		CountEdtObj();													// �ҏW�Ώۗ��̶���
		ResetSelNo();													// ���̑I��ؾ��
	}
	Display(0);															// �Ώۗ��̕\��
}
