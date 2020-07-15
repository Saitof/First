/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/06/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "RCommand.h"
#include "Select.h"
#include "CreatCtrlPoint1.h"
#include "CreatCtrlPoint2.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*************************************************************/
void CJcad3GlbView::MouseSweep(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< �Ȗʐ���(�|����)
/*************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, endf=0;
	double   d[3], dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(Sphase>0&&Si3>0) {
		i = (m_FChar&0x20) ? 1 : 0;										// [F10]:�f��90�x��]
		i = (m_FChar&0x40) ? 2 : i;										// [F11]:�f��180�x��]
		if(i>=0) {
			ChangeRotate2D(i, Si6, Si1, Spa);							// 2D�f�ʉ�]�X�V
		}
		m_FChar = m_FChar&~(0x20);										// [F10]OFF
		m_FChar = m_FChar&~(0x40);										// [F11]OFF
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2&&HitTestPlane(point)==Si5&&Si3>0) {				// <��������OK>
			CalcScreenWorld(0, &pnt, point, Si5);						// ���W�_�擾
			if(CheckNextPoint(Spnt1, pnt)) {							// ���O�_�Ƃ̋���OK
				if(flg&m_LimKey) {
					LimitPoint(Si5, &pnt, &Spnt1);						// ϳ����͐������W
				}
				Sib[Si3] = 1, Spb[Si3] = pnt;							// Ӱ��/����_�ݒ�(��)
				CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3+1,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// �|���̐���_�v�Z
				RedoSetSurface(Si3+1, Si1, 0, Si2, Spc);				// �Ȗʐ���_����(��)
				if(Si3>0) {
					Tracker1.Setup(Spc, Sic, NULL, (Si3+1)*Si1, 5, 0);	// �ׯ��1�ݒ�
				} else {
					Tracker1.Reset(0);									// �ׯ��1����
				}
				for(i=0; i<DIM; i++) {
					d[i] = pnt.p[i]-Spnt1.p[i];							// ���O�_�Ƃ̍�
				}
				dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);				// ���O�_�Ƃ̋���
				m_pNumDisp->SetParam(d[0], d[1], d[2], dd);				// ϳ��ړ��ʕ\��
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(Sphase==1) {													// <<�����1�_��>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			Si5 = plane;
			Sib[Si3] = 1;												// �n�_�ܐ��_Ӱ��
			Spb[Si3++] = pnt;											// ����_�ݒ�(�{)
			Spnt1 = pnt;												// ���W�_�ۑ�
			InitLevel();												// Undo/Redo������
			InitSweep(Sbase, Si6, Si4, Si1, Spa);						// �|����������
			SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3, Sib, Sda, Sdb);	// �ʉߓ_=>����_�ϊ���د��
			CalcSweepPoint(0, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                           Spa, Sia, Spb, Sib, Spc, Sic, Sdb);			// �|���̐���_�v�Z
			GetObjPtr(&Sop1);											// �Ȗ�OP�ۑ�
			SetSurface(Si3, Si1, 0, Si2, Spc);							// �Ȗʐ���_����(����)
			GetObjPtr(&Sop2);											// ����_OP�ۑ�
			SetSelNo(Sop2, 2);											// ���F�\��OP�ݒ�
			Sphase = 2;
			SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                            Sib, Sda, Sdb);								// �ʉߓ_=>����_�ϊ���د��
			Display(0);													// �ĕ`��
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				CInputBtnDlg5 dlg(2);
				dlg.SetParam2(m_SweepFlg, Si7, Si6, Si2,
                              m_CurveWeight, Si1, &Si3, Spa, Spb, Spc,
                              Sia, Sib, Sic, Sda, Sdb, m_SweepAdd);		// ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_SweepAdd);								// ���Ұ��擾
				endf = 1;												// �ƭ��I��
			}
		} else if(plane==Si5) {											// <<2�_�ڈȍ~�œ��ꕽ��>>
			CalcScreenWorld(0, &pnt, point, Si5);						// ���W�_�擾
			if(Si3==0) {												// <1�_�ڂ܂Ŏ������ꂽ�ꍇ>
				Sib[Si3] = 1;											// �n�_�ܐ��_Ӱ��
				Spb[Si3++] = pnt;										// ����_�ݒ�(�{)
				Spnt1 = pnt;											// ���W�_�ۑ�
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                Sib, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
				CalcSweepPoint(0, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// �|���̐���_�v�Z
				GetObjPtr(&Sop1);										// �Ȗ�OP�ۑ�
				SetSurface(Si3, Si1, 0, Si2, Spc);						// �Ȗʐ���_����(����)
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				SetSelNo(Sop2, 2);										// ���F�\��OP�ݒ�
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                Sib, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
				Display(0);												// �ĕ`��
			} else {													// <2�_�ڈȍ~>
				if(CheckNextPoint(Spnt1, pnt)) {						// ���O�_�Ƃ̋���OK
					if(Si3>0&&(flg&m_LimKey)) {
						LimitPoint(Si5, &pnt, &Spnt1);					// ϳ����͐������W
					}
					if(Si1*(2*Si6+Si3+1)<=m_MaxSCtl&&
                       2*Si6+Si3+1<=m_MaxCCtl) {						// <����_�������>
						Sib[Si3] = (m_CurInp) ? 1 : 0;					// Shift�ܐ��_Ӱ��
						Spb[Si3++] = pnt;								// ����_�ݒ�(�{)
						Spnt1 = pnt;									// ���W�_�ۑ�
					} else {											// <����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                        Sib, Sda, Sdb);					// �ʉߓ_=>����_�ϊ���د��
					}
					CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                                   Spa, Sia, Spb, Sib, Spc, Sic, Sdb);	// �|���̐���_�v�Z
					RedoSetSurface(Si3, Si1, 0, Si2, Spc);				// �Ȗʐ���_����(�{)
					if(Si3>1) {
						Tracker1.Setup(Spc, Sic, NULL, Si3*Si1, 5, 0);	// �ׯ��1�ݒ�
					} else {
						Tracker1.Reset(0);								// �ׯ��1����
					}
					SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                    Sib, Sda, Sdb);						// �ʉߓ_=>����_�ϊ���د��
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==11) {													// <<<DEL(���)>>>
		if(Sphase==2&&Si3>0) {											// <����_��o�^��>
			Si3--;														// ����_�����Z
			if(Si3>0) {
				Spnt1 = Spb[Si3-1];										// ���W�_�ۑ�
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                Sib, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
				CalcSweepPoint(1, m_SweepFlg, Si7, 0, Si1, Si2, Si3,
                               Spa, Sia, Spb, Sib, Spc, Sic, Sdb);		// �|���̐���_�v�Z
				RedoSetSurface(Si3, Si1, 0, Si2, Spc);					// �Ȗʐ���_����(�{)
				SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3+1,
                                Sib, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
			} else {
				Spnt1.p[0]=DBL_MAX, Spnt1.p[1]=DBL_MAX, Spnt1.p[2]=DBL_MAX;
				if(Sop1!=NULL) {
					FreeObj(Sop1, 1);									// ���̍폜
				}
				if(Sop2!=NULL) {
					FreeObj(Sop2, 1);									// ���̍폜
				}
			}
			if(Si3>1) {
				Tracker1.Setup(Spc, Sic, NULL, Si3*Si1, 5, 0);			// �ׯ��1�ݒ�
			} else {
				Tracker1.Reset(0);										// �ׯ��1����
			}
			Display(0);													// �ĕ`��
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si6 = 0;
			m_SolidFlg = m_AtrEnv;										// �����Ȗʑ���
			Si8 = m_SecEnv;												// �����[�ʌ`��
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			if(m_FChar&0x1) {											// [F5]:�����������]
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:�[�ʌ`�󔽓]
				Si8 = (Si8) ? 0 : 1;
			}
			m_SweepFlg = (m_FChar&0x4) ? 1 : 0;							// [F7]:�|���Ȗ�
			Si7 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
			Si9 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�����޲�۸ޖ�
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==1&&cmp!=NULL) {									// <�Ȑ��̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				Display(0);												// �ĕ`��
				GetObjBox(Sop1, &box);									// ����BOX���ގ擾
				Si1 = GetParaInt(cmp, 0);								// ����_���擾
				Si2 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// �Ȑ�����_�擾
				}
				SetCurve(Si1, Si2, Spa);								// �Ȑ�����_����
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// ���̕ҏW�㐧��_�擾
				}
				FreeObj(Sop2, 1);										// �ҏW�㗧�̍폜
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si4)) {			// ������������
					if(InputBasePoint1(&m_SecBase2)) {					// �f�ʐ����ʒu�擾
						SetBasePoint(Si4, m_SecBase2, box, &Sbase);		// �f�ʐ������W�ݒ�
						StartMenu();									// �ƭ��J�n����
						if(!m_SweepFlg) {								// <�ʏ�Ȗ�>
							i1 = GetVtxNum(Sop1);						// �Ȗʒ��_���擾
							for(i=0; i<i1; i++) {
								GetVtx(Sop1, i, &Spb[i]);				// �Ȗʒ��_���W�擾
							}
							Si6 = GetCurveCenter(Si4, Si2, i1,
                                                 Spb, &pnt);			// 2D�}�`���S�擾
							Si6 = ((!m_SolidFlg)||Si8) ? 0 : Si6;		// OFF�͒f�ʖ�
							if(Si6) {
								Spa[Si1] = pnt;							// 2D���S�ǉ�
							}
						} else {										// <�|���Ȗ�>
							for(i=0; i<Si1; i++) {
								Sia[i] /= 2;							// �Ȑ�Ӱ�ޕϊ�(0/1)
							}
						}
						OffUndoRedo();									// Undo/Redo�s��
						SetNumDisp(2);									// ���l�\����������
						Sphase = 1;
						if(m_SweepFlg&&Si7) {							// �|���Ȗ�&�ʉߓ_Ӱ��
							AfxMessageBox(IDS_ERR_SWEEP);				// �װү����
							Si7 = 0;									// �|���Ȗʂ͐���_Ӱ��
						}
					} else {											// <��ݾ�>
						ExitMenu(0, 1);									// �ƭ��I��
					}
				} else {												// <���ݕ���:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// �װү����
					ExitMenu(0, 1);										// �ƭ��I��
				}
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
			Sop1 = NULL, Sop2 = NULL;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		endf = 1;														// �ƭ��I��
	}
	if(endf) {															// <�ƭ��I���̏ꍇ>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// ���̍폜
		}
		if(Sphase==2) {
			if(Si3>1) {													// <<<����ȋȐ�>>>
				Sib[0] = 1, Sib[Si3-1] = 1;								// �n�_/�I�_�ܐ��_Ӱ��
				if(Si3==2) {											// <�n�_/�I�_�݂̂̏ꍇ>
					i1 = InsertSweepPoint(Si9, Si1, Si6, &m_SweepDivi,
                                          &Si3, Spb, Sib);				// �|���̒��Ԑ���_�}��
				} else {
					i1 = TRUE;
				}
				if(i1) {												// <�޲�۸޷�ݾقȂ�>
					Delete();											// 2D�}�`�폜
					SetCurveMatrix1(Si7, m_CurveWeight, 0, Si3,
                                    Sib, Sda, Sdb);						// �ʉߓ_=>����_�ϊ���د��
					CalcSweepPoint(1, m_SweepFlg, Si7, Si6,
                                   Si1, Si2, Si3, Spa, Sia,
                                   Spb, Sib, Spc, Sic, Sdb);			// �|���̐���_�v�Z
					i = GetObjAtr(Sop1);								// ���̑����擾
					GetObjOif(Sop1, &oif);								// ���̍\������߲���擾
					if(i==20) {
						SetOifName(0,oif,OBJ_NAME03);					// �̍\����(�ʏ�|����)
					} else if(i==21) {
						SetOifName(0,oif,OBJ_NAME04);					//         (�ʏ�|����[�ʑ�])
					} else if(i==22) {
						SetOifName(0,oif,OBJ_NAME05);					//         (�|���|����)
					} else {
						SetOifName(0,oif,OBJ_NAME06);					//         (�|���|����[�ʑ�])
					}
					OnEdtFlg(1, Sop1);									// �Ȗ�edtflg:ON
					CountEdtObj();										// �ҏW�Ώۗ��̶���
				} else {												// <�޲�۸޷�ݾق���>
					FreeObj(Sop1, 1);									// �Ȑ��폜
					BackLevel();										// ���ٖ�
				}
			} else {													// <<<�L������_������>>>
				AfxMessageBox(IDS_FEW_CTRLP);							// �װү����
				FreeObj(Sop1, 1);										// �Ȑ��폜
				BackLevel();											// ���ٖ�
			}
			Tracker1.Reset(1);											// �ׯ��1����
		}
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
    }
}

/***************************************************************/
void CJcad3GlbView::MouseRevolve(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< �Ȗʐ���(��]��)
/***************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt, ps[3], pe[3];
	BOXTYPE  box;
	int      i, j, i1, i2, i3, cl[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if((Sphase==2||Sphase==3)&&(flg&MK_LBUTTON)) {					// ϳ����t�������̂ݗL��
			CalcScreenWorld(0, &Spnt2, point, Si6);						// ��]��(ײ�)�I�_
			if(CheckNextPoint(Spnt1, Spnt2)) {							// 1�_�ڂƂ̋���OK
				if(flg&m_LimKey) {
					LimitPoint(Si6, &Spnt2, &Spnt1);					// ϳ����͐������W
				}
				ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 0;
				Line.Setup(ps, pe, cl, 1, Si6);							// ��]��(ײ�)�ݒ�
				if(Sphase==2) {											// <1���>
					InitLevel();										// Undo/Redo������
					CalcRevolvePoint(0, m_SweepFlg, Si10, Si9, Si6, 
                                     Si1, Si2, Si3, Si4, Sd1, Spnt1,
                                     Spnt2, Spnt3, Spa, Sia);			// ��]�̐���_�v�Z
					GetObjPtr(&Sop1);									// �Ȗ�OP�ۑ�
					Sphase = 3;
				} else {												// <2��ڈȍ~>
					CalcRevolvePoint(1, m_SweepFlg, Si10, Si9, Si6,
                                     Si1, Si2, Si3, Si4, Sd1, Spnt1,
                                     Spnt2, Spnt3, Spa, Sia);			// ��]�̐���_�v�Z
				}
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1&&HitTestPlane(point)==Si6) {						// <��������OK>
			i1 = -1;
			if(Si8>0) {
				CalcScreenWorld(1, &Spnt1, point, Si6);					// ���W�_�擾
				i1 = Tracker1.HitTracker(1, Si6, Spnt1, Spb, Si8);		// �ׯ��1����
			}
			if(i1==-1) {												// <�ׯ����/��I��>
				CalcScreenWorld(0, &Spnt1, point, Si6);					// ��]��(ײ�)�n�_
				if(Si8>0) {
					Tracker1.Reset(0);									// �ׯ��1����
				}
				Sphase = 2;
			} else {													// <�ׯ���I��>
				InitLevel();											// Undo/Redo������
				SetSRevolvePoint(Si10, Si6, i1, Si1, Si3, Sd1,
                                 Spa, Sia, Spnt1);						// ��]�̿د�ސ���_�쐬
				GetObjPtr(&Sop1);										// �Ȗ�OP�ۑ�
				Sphase = 4;
			}
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si4 = 0;
			Si8 = 0, Si9 = 0, Si10 = 0;
			m_SolidFlg = m_AtrEnv;										// �����Ȗʑ���
			Si11 = m_SecEnv;											// �����[�ʌ`��
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			if(m_FChar&0x1) {											// [F5]:�����������]
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:�[�ʌ`�󔽓]
				Si11 = (Si11) ? 0 : 1;
			}
			m_SweepFlg = (m_FChar&0x4) ? 1 : 0;							// [F7]:�|���Ȗ�
			Si12 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�����޲�۸ޖ�
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==1&&cmp!=NULL) {									// <�Ȑ��̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				Display(0);												// �ĕ`��
				GetObjBox(Sop1, &box);									// ����BOX���ގ擾
				Si1 = GetParaInt(cmp, 0);								// ����_���擾
				Si2 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// �Ȑ�����_�擾
				}
				SetCurve(Si1, Si2, Spa);								// �Ȑ�����_����
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ����ނ̕��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// ���̕ҏW�㐧��_�擾
				}
				FreeObj(Sop2, 1);										// �ҏW�㗧�̍폜
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si6)) {			// ������������
					if(GetRevolveInfo(m_NumInp, Si12, &m_RevolveTh,
                                      &m_RevolveDivi)) {				// ��]�̏��擾
						Sd1 = (m_NumInp) ? m_RevolveTh : 360.0;
						Si3 = (!Si12) ? m_RevolveDivi : 8;
					} else {
						Sd1 = 0.0;
					}
				} else {												// <���ݕ���:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// �װү����
					Sd1 = 0.0;
				}
				if(Sd1>0.0) {											// <�L�������p�x>
					if(fabs(Sd1-360.0)<EPSILON) {						// 360�x�͋��E:��
						Si4 = 1;
					}
					if(!m_SweepFlg) {									// <<�ʏ�Ȑ�>>
						if(fabs(Sd1-360.0)<EPSILON) {					// <360�x>
							InitRevolve(Si2, Si1, Spa, Sia,
                                        &Si8, Spb, Sib);				// ��]��������
							if((!Si2)&&m_SolidFlg&&(!Si11)) {			// ON�ŊJ�͒f�ʗL
								Si9 = 1;
							}
						} else {										// <360�x����>
							i1 = GetVtxNum(Sop1);						// �Ȗʒ��_���擾
							for(i=0; i<i1; i++) {
								GetVtx(Sop1, i, &Spb[i]);				// �Ȗʒ��_���W�擾
							}
							Si10 = GetCurveCenter(Si6, Si2, i1,
                                                  Spb, &pnt);			// 2D�}�`���S�擾
							Si10 = ((!m_SolidFlg)||Si11) ? 0 : Si10;	// OFF�͒f�ʖ�
							if(Si10) {
								Spa[Si1] = pnt;							// 2D���S�ǉ�
							}
						}
					} else {											// <<�|���Ȑ�>>
						for(i=0; i<DIM; i++) {							// 2D���_���S�_�擾
							Spnt3.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
						}
						for(i=0; i<Si1; i++) {
							for(j=0; j<DIM; j++) {
								Spa[i].p[j] -= Spnt3.p[j];				// 2D�𒆐S�_�ֈړ�
							}
						}
						for(i=0; i<Si1; i++) {
							Sia[i] /= 2;								// �Ȑ�Ӱ�ޕϊ�(0/1)
						}
					}
					if((2*(Si9+Si10)+Si1)*Si3>m_MaxSCtl) {				// <����_�������>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						ExitMenu(0, 1);									// �ƭ��I��
					} else {											// <����_����ȉ�>
						OffUndoRedo();									// Undo/Redo�s��
						StartMenu();									// �ƭ��J�n����
						if(Si8>0) {
							Tracker1.Setup(Spb, Sib, NULL, Si8, Si6, 7);// �ׯ��1�ݒ�
							Display(0);									// �ĕ`��
						}
						Sphase = 1;
					}
				} else {
					ExitMenu(0, 1);										// �ƭ��I��
				}
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
			Sop1 = NULL, Sop2 = NULL;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if((ms==3&&Sphase>=3)||ms==4||ms==9) {						// <<<�����/�E����/�I��>>>
		if(ms==3&&Sphase==3) {											// <2��ڈȍ~>
			CalcRevolvePoint(2, m_SweepFlg, Si10, Si9, Si6,
                             Si1, Si2, Si3, Si4, Sd1, Spnt1,
                             Spnt2, Spnt3, Spa, Sia);					// ��]�̐���_�v�Z
		}
		if(Sphase>=3) {
			i1 = GetObjAtr(Sop1);										// ���̑����擾
			GetObjOif(Sop1, &oif);										// ���̍\������߲���擾
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME07);							// �̍\����(�ʏ��]��)
			} else if(i1==21) {
				SetOifName(0, oif, OBJ_NAME08);							//         (�ʏ��]��[�ʑ�])
			} else if(i1==22) {
				SetOifName(0, oif, OBJ_NAME09);							//         (�|����]��)
			} else {
				SetOifName(0, oif, OBJ_NAME10);							//         (�|����]��[�ʑ�])
			}
			Delete();													// 2D�}�`�폜
			OnEdtFlg(1, Sop1);											// �Ȗ�edtflg:ON
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		}
		Line.Reset();													// ײ݉���
		Tracker1.Reset(1);												// �ׯ��1����
		OnUndoRedo();													// Undo/Redo��
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
	}
}

/***************************************************************/
void CJcad3GlbView::MousePyramid(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< �Ȗʐ���(��)
/***************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, i1, i2, i3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2&&(flg&MK_LBUTTON)) {								// ϳ����t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// �����_���W�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Spnt2);						// ϳ����͐������W
			}
			i1 = plane;													// �w�蕽�ʗL�����W
			i2 = (i1+1>2) ? 0 : i1+1;
			Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];			// �L�����W�X�V
			Pyramid(0, Si5, Si1, Spa, Sia, Spnt1, Si3, Spb, Sib);		// ������_����(��)
			RedoCrtSurface((Si3+2), Si1, 0, Si2, Spb, Sib);				// �Ȗʐ���(��)
			RedoSetSurface((Si3+2), Si1, 0, Si2, Spb);					// �Ȗʐ���_����(��)
			Tracker1.Setup(&Spnt1, &Si4, NULL, 1, 5, 0);				// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			Sphase=(Tracker1.HitTracker(1, plane, pnt, &Spnt1, 1)!=-1)
                                                         ? 2 : Sphase;	// �ׯ��1����
		} 
	} else if(ms==3) {													// <<<ϳ����t���>>>
		Sphase = (Sphase==2) ? 1 : Sphase;
		Spnt2 = Spnt1;													// �����_���W�ۑ�
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Si1 = 0, Si2 = 0, Si3 = 0, Si4 = 0, Si6 = 0;
			m_SolidFlg = m_AtrEnv;										// �����Ȗʑ���
			Si7 = m_SecEnv;												// �����[�ʌ`��
			if(m_FChar&0x1) {											// [F5]:�����������]
				m_SolidFlg = (m_SolidFlg) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:�[�ʌ`�󔽓]
				Si7 = (Si7) ? 0 : 1;
			}
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==1&&cmp!=NULL) {									// <�Ȑ��̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				Display(0);												// �ĕ`��
				Si1 = GetParaInt(cmp, 0);								// ����_���擾
				Si2 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// �Ȑ�����_�擾
				}
				SetCurve(Si1, Si2, Spa);								// �Ȑ�����_����
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				for(i=0; i<Si1; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// ���̕ҏW�㐧��_�擾
				}
				FreeObj(Sop2, 1);										// �ҏW�㗧�̂̍폜
				if(InitCreat(1, Si2, Si1, Spa, Sia, &Si5)) {			// ������������
					CheckCtrlKey1();									// ���䷰�m�F(�N����)
					if(m_NumInp!=0) {									// <���l���͂̏ꍇ>
						CInputDlg dlg(22);
						dlg.SetValue(m_PyramidDivi);					// �����l�\��
						if(dlg.DoModal()==IDOK) {						// �޲�۸ޕ\��
							dlg.GetValue(&m_PyramidDivi);				// ���������擾
							Si3 = m_PyramidDivi;
						} else {
							Si3 = 0;
						}
					} else {
						Si3 = 1;										// �w�薳�͐�������:1
					}
					if(Si3>0) {
						OffUndoRedo();									// Undo/Redo�s��
						StartMenu();									// �ƭ��J�n����
						InitLevel();									// Undo/Redo������
						InitPyramid(Si5, Si1, Spa, &Spnt1);				// ����������
						i1 = GetVtxNum(Sop1);							// �Ȗʒ��_���擾
						for(i=0; i<i1; i++) {
							GetVtx(Sop1, i, &Spb[i]);					// �Ȗʒ��_���W�擾
						}
						Si6 = GetCurveCenter(Si5, Si2, i1, Spb, &pnt);	// 2D�}�`���S�擾
						Si6 = ((!m_SolidFlg)||Si7) ? 0 : Si6;			// OFF�͒f�ʖ�
						if(Si6) {
							Spa[Si1] = pnt;								// 2D���S�ǉ�
						}
						Pyramid(0, Si5, Si1, Spa, Sia, Spnt1,
                                Si3, Spb, Sib);							// ������_����(����)
						Spnt2 = Spnt1;									// �����_���W�ۑ�
						CrtSurface(Si3+2, Si1, 0, Si2, m_CurveRepeat,
                                   m_CurveWeight, m_SolidFlg, Spb, Sib);// �Ȗʐ���(����)
						GetObjPtr(&Sop1);								// �Ȗ�OP�ۑ�
						SetSurface(Si3+2, Si1, 0, Si2, Spb);			// �Ȗʐ���_����(����)
						GetObjPtr(&Sop2);								// ����_OP�ۑ�
						SetSelNo(Sop2, 2);								// ���F�\��OP�ݒ�
						Tracker1.Setup(&Spnt1, &Si4, NULL, 1, 5, 0);	// �ׯ��1�ݒ�
						Display(0);										// �ĕ`��
						Sphase = 1;
					} else {											// ���~�̏ꍇ
						ExitMenu(0, 0);									// �ƭ��I��
						Sop1 = NULL, Sop2 = NULL;
					}
				} else {												// <���ݕ���:NG>
					AfxMessageBox(IDS_ERR_PLANE);						// �װү����
					ExitMenu(0, 0);										// �ƭ��I��
					Sop1 = NULL, Sop2 = NULL;
				}
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
				Sop1 = NULL, Sop2 = NULL;
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==1) {
			i1 = Si6+1;
			Pyramid(i1, Si5, Si1, Spa, Sia, Spnt1, Si3, Spb, Sib);		// ������_����(�{)
			RedoCrtSurface(Si3+Si6+2, Si1, 0, Si2, Spb, Sib);			// �Ȗʐ���(�{)
			i1 = GetObjAtr(Sop1);										// ���̑����擾
			GetObjOif(Sop1, &oif);										// ���̍\������߲���擾
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME11);							// �̍\����(�ʏ퐍��)
			} else {
				SetOifName(0, oif, OBJ_NAME12);							//         (�ʏ퐍��[�ʑ�])
			}
			FreeObj(Sop2, 1);											// ���̍폜
			Delete();													// 2D�}�`�폜
			OnEdtFlg(1, Sop1);											// �Ȗ�edtflg:ON
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			Tracker1.Reset(1);											// �ׯ��1����
			ResetSelNo();												// ���̑I��Noؾ��
			OnUndoRedo();												// Undo/Redo��
			Display(0);													// �ĕ`��
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
			SetDataUse();												// �ް��g�p���ݒ�
		}
	}
}

/******************************************************************/
void CJcad3GlbView::MouseSetSection(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< �Ȗʐ���(�Ȑ��f�ʊ��t)
/******************************************************************/
{
	OBJTYPE* op, * op1, * op2;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	VECTYPE  vc1, vc2;
	BOXTYPE  box;
	int      i, i1, i2, i3, err=0;
	double   d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(!Si4) {														// ���̑I��
			ObjSelect(flg, point, plane, ms);
		} else if(Sphase==1&&(Si5==0||Si13==plane)) {					// <����_�ړ��̏ꍇ>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			if(plane==0) {
				pnt.p[2] = Spx[Si12].p[2];
			} else if(plane==1) {
				pnt.p[0] = Spx[Si12].p[0];
			} else if(plane==2) {
				pnt.p[1] = Spx[Si12].p[1];
			}
			Spx[Si12] = pnt;											// �ʉߓ_�ʒu�ύX
			if(Si12<Si7) {												// <�|����1���̕ύX>
				CalcCurvePoint(1, Si7, Spx, Spa, Sdb);					// �ʉߓ_=>����_�ϊ�
				for(i1=0; i1<Si7; i1++) {
					Spy[i1] = Spa[i1], Siy[i1] = Sia[i1];				// ����_��񕡎�
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spy, Siy);									// �Ȑ�����
			} else {													// <�|����2���̕ύX>
				CalcCurvePoint(1, Si7, &Spx[Si7], Spb, Sdb);			// �ʉߓ_=>����_�ϊ�
				for(i1=0; i1<Si7; i1++) {
					Spz[i1] = Spb[i1], Siy[i1] = Sib[i1];				// ����_��񕡎�
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spz, Siy);									// �Ȑ�����
			}
			Si3 = 0, Si9 = 0, Si10 = 0;
			for(i=0; i<Si7; i++) {
				Siz[i] = 0;												// ���t�׸ޏ�����
			}
			for(i=0; i<Si11; i++) {
				Six[i] = 3;												// �ׯ���F������
			}
			SetBaseData(Si6, i1, Si8, Spy, Spz, 3);						// ������o�^
			Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);					// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		if((Si12=Tracker1.HitTracker(1, plane, pnt, Spx, Si11))!=-1) {	// �ׯ��1����
			Si4 = 1, i1 = 0;											// �ׯ���د�����
			BaseObjPtr(2, &op, &i2, &i3);								// �ҏW�Ώۗ��̎擾
			while(op!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				op1 = op;												// OP�ۑ�&����
				i1++;
				NextObjPtr(2, &op, &i2, &i3);							// �����̎擾
			}
			if(i1==1) {													// <�f�ʗ��̐�:1�̏ꍇ>
				BaseCmdPtr(1, op1, &cmp);								// �L���擪����ގ擾
				i2 = GetObjAtr(op1);									// ���̑����擾
				if(i2/10==1&&cmp!=NULL) {
					if(Si3==0) {										// <1��ڂ̒f�ʊ��t�̏ꍇ>
						Si9 = GetParaInt(cmp, 0);						// ����_���擾
						Si10 = GetParaInt(cmp, 1);						// ���EӰ�ގ擾
						err = (Si7*Si9>m_MaxSCtl) ? 2 : err;			// ����_�����ް
						err = (Si5&&Si10==1) ? 1 : err;					// �د�ނŋ��E:�ʹװ
					} else {											// <2�x�ڈȍ~�̒f�ʊ��t�̏ꍇ>
						err = (GetParaInt(cmp, 0)!=Si9) ? 1 : err;		// ����_������
						err = (GetParaInt(cmp, 1)!=Si10) ? 1 : err;		// ���EӰ������
					}
					if(!err) {											// <����ȋȖʂ̏ꍇ>
						for(i=0; i<Si9; i++) {
							Siw[i] = GetParaInt(cmp, 4+i);				// �Ȑ�Ӱ�ގ擾
							GetParaPnt(cmp, i, &Spw[i]);				// �Ȑ�����_�擾
						}
						SetCurve(Si9, Si10, Spw);						// �f�ʋȐ�����_����
						GetObjPtr(&op);									// �f�ʐ���_OP�ۑ�
						BaseCmdPtr(1, op1, &cmp);						// �f�ʗL���擪����ގ擾
						NextCmdPtr(&cmp);								// �f�ʋȐ��ҏW������
						while(cmp!=NULL) {
							RedoCommandCopy(op, cmp);					// �f�ʺ���ޕ��ʎ��s
							NextCmdPtr(&cmp);							// �f�ʎ�����ނ�
						}
						for(i=0; i<Si9; i++) {
							GetVtx(op,i,&Spw[i]);						// �f�ʗ��̕ҏW�㐧��_�擾
						}
						FreeObj(op, 1);									// �f�ʕҏW�㗧�̍폜
					}
				} else {
					err = 1;											// ����ȋȐ��ȊO�ʹװ
				}
			}
			if(i1==1&&err==0) {											// <<�����1�{�̋Ȑ��̏ꍇ>>
				GetObjBox(op1, &box);									// ����BOX���ގ擾
				err = (fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON&&
                       fabs(box.bp[0].p[1]-box.bp[1].p[1])>EPSILON&&
                       fabs(box.bp[0].p[2]-box.bp[1].p[2])>EPSILON)
                      ? 3 : err;										// 3�ʐ}��ɑ��݂Ȃ��ʹװ
			}
			if(i1==0) {													// <<�f�ʑI���Ȃ��̏ꍇ>>
				Sbase = Spx[Si12], Sphase = 1;							// ����_�ړ�������
			} else if(i1==1&&err==0) {									// <<�����1�{�̋Ȑ��̏ꍇ>>
				if(Si6==1&&Si5!=1) {									// �|����1�{�̏ꍇ
					i2 = InputBasePoint1(&m_SecBase2);					// �f�ʐ����ʒu�ݒ�1
				} else if(Si6==2) {										// �|����2�{�̏ꍇ
					i2 = InputBasePoint2(&m_SecBase3a,
                                         &m_SecBase3b, &m_SecBase3c);	// �f�ʐ����ʒu�ݒ�2
				} else {
					i2 = 1;
				}
				i1 = (!i2) ? 0 : i1;									// ��ݾق͏������~
			}
			if(i1==1&&err==0) {											// <<�����1�{�̋Ȑ��̏ꍇ>>
				SetSelNo(op1, 9);										// �f�ʋȐ��o�^
				Si12 = (Si12>=Si7) ? Si12 - Si7 : Si12;					// 2�{�ڈʒu��1�{�ڈʒu�ɕϊ�
				SetCurveSection(Si6, Si5, Si12, m_SecBase2, m_SecBase3a,
                                m_SecBase3b, box, Si9, Si10,
                                Spw, Siw, Sps, Sis);					// �Ȑ��f�ʊ��t
				Si3 = 1, Siz[Si12] = m_SecBase3c+1;						// �f�ʊ��t�ς�
				Six[Si12] = 5;											// �ׯ���F�ύX
				Six[Si7+Si12] = (Si6>1) ? 5 : Six[Si7+Si12];
				Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);				// �ׯ��1�ݒ�
				Display(0);												// �ĕ`��
			} else if(i1>1||err==1) {									// <<2�{�ȏォ�A�װ����>>
				AfxMessageBox(IDS_ERR_SEL9);							// �װү����
			} else if(err==2) {											// <<����_������ް>>
				AfxMessageBox(IDS_MANY_CTRLP);							// �װү����
			} else if(err==3) {											// <<3�ʐ}��ɑ��݂Ȃ�>>
				AfxMessageBox(IDS_ERR_PLANE);							// �װү����
			}
		} else {
			Si4 = 0;													// �ׯ���د��Ȃ�
			ObjSelect(flg, point, plane, ms);							// ���̑I��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(!Si4) {
			ObjSelect(flg, point, plane, ms);							// ���̑I��
		} else if(Sphase==1) {
			Sphase = 0;
		}
		m_EdtCount += Si6;												// �ҏW�Ώۗ��̐����Z(��)
	} else if(ms==11) {													// <<<DEL>>>
		Si3  = 0, Si9  = 0, Si10 = 0;
		for(i=0; i<Si7; i++) {
			Siz[i] = 0;													// ���t�׸ޏ�����
		}
		for(i=0; i<Si11; i++) {
			Six[i] = 3;													// �ׯ���F������
		}
		SelBaseObjPtr(9, &op);											// �f�ʓo�^���̎擾
		while(op!=NULL) {
			SetSelNo(op, 0);											// �f�ʓo�^���̉���
			SelNextObjPtr(9, &op);										// ���o�^���̂�
		}
		Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);						// �ׯ��1�ݒ�
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		ResetSelNo();													// ���̑I��ؾ��
		Si2 = 0, Si3 = 0, Si4 = 0, Si5 = 0, Si6 = 0;
		Si7 = 0, Si8 = 0, Si9 = 0, Si10 = 0, Si11 = 0;
		m_SolidFlg = m_AtrEnv;											// �����Ȗʑ���
		Si1 = m_SecEnv;													// �����[�ʌ`��
		if(m_FChar&0x1) {												// [F5]:�����������]
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {												// [F6]:�[�ʌ`�󔽓]
			Si1 = (Si1) ? 0 : 1;
		}
		Si14 = (m_FChar&0x8) ? 0 : 1;									// [F8]:�폜�Ȃ�
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {												// <<�ҏW���̗L�̏ꍇ>>
			if(Si6>1) {													// �|����3�{�ȏ�ʹװ
				err = 1; break;
			}
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			i3 = GetObjAtr(op);											// ���̑����擾
			if(i3/10==1&&cmp!=NULL) {
				if(Si6==0) {											// <1�{�ڂ̋Ȑ��̏ꍇ>
					op1 = op;											// 1�{�ڑ|�����Ȑ�����
					Si7 = GetParaInt(cmp, 0);							// ����_���擾
					Si8 = GetParaInt(cmp, 1);							// ���EӰ�ގ擾
					for(i=0; i<Si7; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i, &Spa[i]);					// �Ȑ�����_�擾
					}
				} else {												// <2�{�ڂ̋Ȑ��̏ꍇ>
					op2 = op;											// 2�{�ڑ|�����Ȑ�����
					if(GetParaInt(cmp, 0)!=Si7) {						// ����_������
						err = 1; break;
					}
					if(GetParaInt(cmp, 1)!=Si8) {						// ���EӰ������
						err = 1; break;
					}
					for(i=0; i<Si7; i++) {
						Sib[i] = GetParaInt(cmp, 4+i);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i, &Spb[i]);					// �Ȑ�����_�擾
					}
				}
			} else {													// �Ȑ��ȊO�ʹװ
				err = 1; break;
			}
			NextObjPtr(2, &op, &i1, &i2);								// �����̎擾
			Si6++;
		}
		if(!err&&Si6==1&&Si8==1) {										// <<�|������1�{�ŕ̏ꍇ>>
			CRevolveDlg dlg;
			if(dlg.DoModal()==IDOK) {									// ��]�̎�ސݒ�
				dlg.GetValue(&Si5);
			}
			if(Si5==1) {												// <�د�ނ̏ꍇ>
				GetObjBox(op1, &box);									// ����BOX���ގ擾
				err = (fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON&&
                       fabs(box.bp[0].p[1]-box.bp[1].p[1])>EPSILON&&
                       fabs(box.bp[0].p[2]-box.bp[1].p[2])>EPSILON)
                      ? 2 : err;										// 3�ʐ}��ɑ��݂Ȃ��ʹװ
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {		// �د�ޑ|�����w�蕽��
					Si13 = 1;
				} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
					Si13 = 2;
				} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
					Si13 = 0;
				}
			}
		}
		if(!err) {														// <<����ȋȐ��̏ꍇ>>
			SetCurve(Si7, Si8, Spa);									// 1�{�ڋȐ�����_����
			GetObjPtr(&op);												// 1�{�ڐ���_OP�ۑ�
			BaseCmdPtr(1, op1, &cmp);									// 1�{�ڗL���擪����ގ擾
			NextCmdPtr(&cmp);											// 1�{�ڋȐ��ҏW������
			while(cmp!=NULL) {
				RedoCommandCopy(op, cmp);								// 1�{�ں���ޕ��ʎ��s
				NextCmdPtr(&cmp);										// 1�{�ڎ�����ނ�
			}
			for(i=0; i<Si7; i++) {
				GetVtx(op, i, &Spa[i]);									// 1�{�ڗ��̕ҏW�㐧��_�擾
			}
			FreeObj(op, 1);												// 1�{�ڕҏW�㗧�̍폜
			if(Si6>1) {													// <2�{�ڂ�����ꍇ>
				SetCurve(Si7, Si8, Spb);								// 2�{�ڋȐ�����_����
				GetObjPtr(&op);											// 2�{�ڐ���_OP�ۑ�
				BaseCmdPtr(1, op2, &cmp);								// 2�{�ڗL���擪����ގ擾
				NextCmdPtr(&cmp);										// 2�{�ڋȐ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(op, cmp);							// 2�{�ں���ޕ��ʎ��s
					NextCmdPtr(&cmp);									// 2�{�ڎ�����ނ�
				}
				for(i=0; i<Si7; i++) {
					GetVtx(op, i, &Spb[i]);								// 2�{�ڗ��̕ҏW�㐧��_�擾
				}
				FreeObj(op, 1);											// 2�{�ڕҏW�㗧�̍폜
			}
		}
		if(!err&&Si5==1) {												// <����ȿد�ނ̏ꍇ>
			CalcNVec(Spa, Si7, &vc1);									// �د�ދȐ��@���޸�َ擾
			i = (Si13-1<0) ? 2 : Si13-1;								// �@���޸�ٕ���
			if(vc1.p[i]>0) {
				ChangeLoop(Si7, Spa, Sia);								// ��ٰ�ߔ��]
			}
		}
		if(!err&&Si6>1) {												// <<����ȋȐ���2�{�̏ꍇ>>
			i1 = 0;
			if(!Si8) {													// <���E:�J�̏ꍇ>
				i1 = (PntD(&Spa[0], &Spb[0])>PntD(&Spa[0], &Spb[Si7-1]))// 1�{�ڂ̎n�_�ɑ΂�
					 ? 1 : i1;											// 2�{�ڂ̎n�_���I�_���߂�
			} else {													// <���E:�̏ꍇ>
				CalcNVec(Spa, Si7, &vc1);								// 1�{�ڋȐ��@���޸�َ擾
				CalcNVec(Spb, Si7, &vc2);								// 2�{�ڋȐ��@���޸�َ擾
				for(i=0, i2=0, d1=0.0; i<DIM; i++) {
					if((d2=fabs(vc1.p[i]))>d1) {						// 1�{�ږ@���޸�ق̍ő����
						d1 = d2, i2 = i;
					}
				}
				i1 = ((vc1.p[i2]>0&&vc2.p[i2]<0)||
                      (vc1.p[i2]<0&&vc2.p[i2]>0)) ? 1 : i1;				// �@���޸�ٕ������قȂ�
			}
			if(i1) {
				ChangeLoop(Si7, Spb, Sib);								// �������t��2�{�ږ�ٰ�ߔ��]
			}
			for(i=0; i<Si7; i++) {
				if(Sia[i]!=Sib[i]) {									// �Ȑ�Ӱ������
					err = 1; break;
				}
			}
		}
		if(!err) {														// <<����ȋȐ��̏ꍇ>>
			Si2 = 1;													// �����J�n�ς�
			OffUndoRedo();												// Undo/Redo�s��
			SetSelNo(op1, 1);											// 1�{�ڑ|�������̓o�^
			OffEdtFlg(1, op1);											// 1�{�ڑ|�������̔�I��
			OffDspFlg(1, op1);											// 1�{�ڑ|�������̔�\��
			for(i1=0; i1<Si7; i1++) {
				Spy[i1] = Spa[i1], Siy[i1] = Sia[i1];					// ����_��񕡎�
			}
			MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                       Spy, Siy);										// �Ȑ�����
			SetCurveMatrix3(1, m_CurveWeight, Si8, Si7, Sia, Sda, Sdb);	// ����_=>�ʉߓ_�ϊ���د��
			CalcCurvePoint(1, Si7, Spa, Spx, Sda);						// ����_=>�ʉߓ_�ϊ�
			for(i=0; i<Si7; i++) {
				Six[i] = 3, Siz[i] = 0;									// �ׯ���F&���t�׸ޏ�����
			}
			Si11 = Si7;													// �ׯ�����ݒ�
			if(Si6>1) {													// <2�{�ڂ�����ꍇ>
				SetSelNo(op2, 1);										// 2�{�ڑ|�������̓o�^
				OffEdtFlg(1, op2);										// 2�{�ڑ|�������̔�I��
				OffDspFlg(1, op2);										// 2�{�ڑ|�������̔�\��
				for(i1=0; i1<Si7; i1++) {
					Spz[i1] = Spb[i1], Siy[i1] = Sib[i1];				// ����_��񕡎�
				}
				MakeSCurve(&i1, Si8, m_CurveRepeat, m_CurveWeight,
                           Spz, Siy);									// �Ȑ�����
				CalcCurvePoint(1, Si7, Spb, &Spx[Si7], Sda);			// ����_=>�ʉߓ_�ϊ�
				for(i=0; i<Si7; i++) {
					Six[i+Si7] = 3;										// �ׯ���F������
				}
				Si11 += Si7;											// �ׯ�������Z
			}
			SetBaseData(Si6, i1, Si8, Spy, Spz, 3);						// ������o�^
			Tracker1.Setup(Spx, Six, NULL, Si11, 5, 7);					// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
			m_CursorNo = 1;												// ����No�ݒ�
		} else {														// <<�ُ�ȗ��̂̏ꍇ>>
			if(err==1) {
				AfxMessageBox(IDS_ERR_SEL9);							// �װү����
			} else {
				AfxMessageBox(IDS_ERR_PLANE);
			}
			ExitMenu(0, 0);												// �ƭ��I��
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Si2==1) {													// <�����J�n�ς݂̏ꍇ>
			OffEdtAllObject();											// �ҏW���̑S����
			if(!Si3) {													// <�f�ʖ����t�̏ꍇ>
				if(Si5) {												// ��]��(�د��)��V:�J
					Si9  = 5, Si10 = 0;
				} else {												// �ȊO��V:��
					Si9  = 8, Si10 = 1;
				}
				SetCircleSection(Si6, Si5, m_Fovy/12000.0, Si7, Si9,
                                 Spa, Spb, Sps, Sis);					// �~�f�ʊ��t
			} else {													// <�f�ʊ��t�ς̏ꍇ>
				CalcCurveSection(Si6, Si5, Si7, Si8, Si9, Si10,
                                 Spa, Spb, Siz, Sps, Sis);				// �Ȑ����t�f�ʌv�Z
			}
			CrtSectionSweep(Si6, Si5, Si7, Si8, Si9, Si10, Spa,
                            Spb, Sia, Sps, Sis, Spc, Sic);				// ���t�f�ʑ|���̐���
			if(!Si8&&Si10&&m_SolidFlg&&!Si1) {							// <U:�J,V:��,�د��:ON,1�[��:OFF>
				AddCenterPoint(&Si7, Si9, Si10, Spc, Sic);				// �[�ʒ��S�_�ǉ�
			}
			StartMenu();												// �ƭ��J�n����
			InitLevel();												// Undo/Redo������
			SelBaseObjPtr(1, &op);										// �|�������̎擾
			while(op!=NULL) {
				op1 = op;												// �폜�����߲���ۑ�
				SelNextObjPtr(1, &op);									// ���|�������̂�
				LevelObj(1, op1);										// �������ٍ폜���̐ڑ�
				TempObj(op1);											// ���̈ꎞ�폜
			}
			if(Si14) {													// <�f�ʓo�^���̍폜����>
				SelBaseObjPtr(9, &op);									// �f�ʓo�^���̎擾
				while(op!=NULL) {
					op1 = op;											// �폜�����߲���ۑ�
					SelNextObjPtr(9, &op);								// ���f�ʓo�^���̂�
					LevelObj(1, op1);									// �������ٍ폜���̐ڑ�
					TempObj(op1);										// ���̈ꎞ�폜
				}
			}
			CrtSurface(Si7, Si9, Si8, Si10, m_CurveRepeat,
                       m_CurveWeight, m_SolidFlg, Spc, Sic);			// �ʏ�Ȑ�����
			GetObjPtr(&op);												// �ʏ�Ȗ�OP�擾
			i1 = GetObjAtr(op);											// ���̑����擾
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			if(i1==20) {
				SetOifName(0, oif, OBJ_NAME03);							// �̍\����(�ʏ�|����)
			} else {
				SetOifName(0, oif, OBJ_NAME04);							//         (�ʏ�|����[�ʑ�])
			}
			OnEdtFlg(1, op);											// �ʏ�Ȗ�edtflg:ON
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			DeleteBaseData();											// ������폜
			Tracker1.Reset(1);											// �ׯ��1����
			ResetSelNo();												// ���̑I��Noؾ��
			OnUndoRedo();												// Undo/Redo��
			Display(0);													// �ĕ`��
			ExitMenu(0, 0);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		}
	}
}

/**************************************/
void CJcad3GlbView::MenuSetCurve(void)									//<<< �Ȗʐ���(�Ȑ��g����)
/**************************************/
{
	OBJTYPE* op, * dop;
	BOXTYPE  box;
	int      i, i1, i2, cnt1=0, cnt2=0, err=0, gno[32];

	if(GetSelectNo()==0) {												// <<2D�Ȑ�OP��>>
		m_SolidFlg = m_AtrEnv;                                          // �����Ȗʑ���
		Si1 = m_SecEnv;												    // �����[�ʌ`��
		if(m_FChar&0x1) {											    // [F5]:�����������]
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {											    // [F6]:�[�ʌ`�󔽓]
			Si1 = (Si1) ? 0 : 1;
		}
		m_SweepFlg = (m_FChar&0x4) ? 1 : 0;								// [F7]:�|���Ȗ�
		Si3 = (m_FChar&0x8) ? 0 : 1;									// [F8]:�폜�Ȃ�
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			GetObjBox(op, &box);										// ����BOX���ގ擾
			SetSelNo(op, 1);											// �f�ʋȐ����̓o�^
			err = (GetObjAtr(op)/10!=1) ? 1 : err;						// �Ȑ��ȊO�ʹװ
			if(cnt1==0) {
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {		// �Ȑ����ݕ�������
					i = 0, Si2 = 1;
				} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
					i = 1, Si2 = 2;
				} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
					i = 2, Si2 = 0;
				} else {
					err = 1;
				}
			} else {
				err = (fabs(box.bp[0].p[i]-box.bp[1].p[i])>EPSILON)		// �Ȑ����ݕ�������
                      ? 1 : err;
			}
			if(err) {													// �װ�͒��~
				break;
			}
			cnt1++;														// ����UP
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(!err&&cnt1>0&&cnt1<101) {									// <�L���ȏꍇ>
			OffEdtAllObject();											// �ҏW���̑S����
		} else {														// <�װ�̏ꍇ>
			ResetSelNo();												// ���̑I��ؾ��
			AfxMessageBox(IDS_ERR_SEL8);								// �װү����
		}
	} else {															// <<2D�Ȑ�OP�L>>
		m_SolidFlg = m_AtrEnv;											// �����Ȗʑ���
		Si1 = m_SecEnv;													// �����[�ʌ`��
		if(m_FChar&0x1) {												// [F5]:�����������]
			m_SolidFlg = (m_SolidFlg) ? 0 : 1;
		}
		if(m_FChar&0x2) {												// [F6]:�[�ʌ`�󔽓]
			Si1 = (Si1) ? 0 : 1;
		}
		m_SweepFlg = (m_FChar&0x4) ? 1 : m_SweepFlg;					// [F7]:�|���Ȗ�
		Si3 = (m_FChar&0x8) ? 0 : Si3;									// [F8]:�폜�Ȃ�
		BaseObjPtr(1, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10!=1) ? 1 : err;					// �Ȑ��ȊO�ʹװ
				cnt1++;													// �f�ʋȐ�����UP
			} else if(i2==1) {
				SetSelNo(op, 2);										// �|���Ȑ����̓o�^
				err = (GetObjAtr(op)/10!=1) ? 1 : err;					// �Ȑ��ȊO�ʹװ
				cnt2++;													// �|��������UP
			}
			if(err) {													// �װ�͒��~
				break;
			}
			NextObjPtr(1, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(cnt1>0&&cnt2>0&&err==0) {
			OffEdtAllObject();											// �ҏW���̑S����
			i = 0;
			if(InputBasePoint1(&m_SecBase2)) {							// �f�ʐ����ʒu�擾
				while(TRUE) {
					StartMenu();										// �ƭ��J�n����
					InitLevel();										// Undo/Redo������
					CreatSetCurve(i, m_SolidFlg, Si1, m_SweepFlg,
                                  Si2, m_SecBase2);						// �Ȑ��g����
					Display(0);											// �ĕ`��
					if(AfxMessageBox(IDS_ROTATE_2D,MB_YESNO)==IDYES){	// 2D�f�ʉ�]�m�F?
						EndJMenu1();									// �ƭ��I������
						Undo(gno);										// ��ײ��Undo
						i++;											// 90�x��]
					} else {											// �I��
						break;
					}
				}
				BaseObjPtr(1, &op, &i1, &i2);							// �����Ώۗ��̎擾
				while(op!=NULL) {
					dop = op;											// �폜�����߲���ۑ�
					i = GetSelNo(op);									// �I��No�擾
					NextObjPtr(1, &op, &i1, &i2);						// �����̂�
					if((Si3==1&&i==1)||i==2) {							// �f�ʋȐ�/�|���Ȑ�
						LevelObj(1, dop);								// �������ٍ폜���̐ڑ�
						TempObj(dop);									// ���̈ꎞ�폜
					}
				}
				EndJMenu1();											// �ƭ��I������
			}
		} else {
			AfxMessageBox(IDS_ERR_SEL8);								// �Ȑ��ȊO,�װү����
		}
		ResetSelNo();													// ���̑I��ؾ��
	}
	CountEdtObj();														// �ҏW�Ώۗ��̶���
	Display(0);															// �ĕ`��
}

/*************************************/
void CJcad3GlbView::MenuSection(void)									//<<< �Ȗʐ���(�f�ʎw��)
/*************************************/
{
	OBJTYPE** sop = new OBJTYPE*[1000];
	OBJTYPE*  op, * wop;
	OIFTYPE*  oif;
	CMDTYPE*  cmp;
	VECTYPE   vc1, vc2;
	int       i, j, k, i1, i2, i3, i4, er=0, oct=0, pct=0;

	m_SolidFlg = m_AtrEnv;												// �����Ȗʑ���
	Si1 = m_SecEnv;														// �����[�ʌ`��
	if(m_FChar&0x1) {													// [F5]:�����������]
		m_SolidFlg = (m_SolidFlg) ? 0 : 1;
	}
	if(m_FChar&0x2) {													// [F6]:�[�ʌ`�󔽓]
		Si1 = (Si1) ? 0 : 1;
	}
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		i3 = GetObjAtr(op);												// ���̑����擾
		if(i3/10==1&&cmp!=NULL) {										// <�Ȑ��̏ꍇ>
			if(oct>=1000) {												// �ő�Ȑ����ȏ㒆�~
				er = 1; break;
			}
			i1 = GetParaInt(cmp, 0);									// ����_���擾
			i2 = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			if(oct==0) {												// �����
				Si2 = i1, Si3 = i2;										// ����_��/���EӰ�ޕۑ�
			} else  {
				if(Si2!=i1||Si3!=i2) {									// ����_��,���EӰ�ނ�
					er = 1; break;										// �قȂ�ꍇ�͒��~
				}
			}
			for(i=0; i<Si2; i++) {
				Sia[pct+i] = GetParaInt(cmp, 4+i);						// �Ȑ�Ӱ�ގ擾
				GetParaPnt(cmp, i, &Sps[i]);							// �Ȑ�����_�擾
			}
			SetCurve(Si2, Si3, Sps);									// ��Ɨ��̐���
			GetObjPtr(&wop);											// ��Ɨ���OP�擾
			NextCmdPtr(&cmp);											// �Ȑ�������
			while(cmp!=NULL) {
				RedoCommandCopy(wop, cmp);								// ����ޕ��ʎ��s
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<Si2; i++) {
				GetVtx(wop, i, &Spa[pct+i]);							// ���̕ҏW�㐧��_�擾
			}
			FreeObj(wop, 1);											// ��Ɨ��̍폜
			CalcCent(&Spa[pct], Si2, &Spb[oct]);						// �Ȑ����S�_�擾
			Sib[oct] = pct;												// ����_�J�n�ʒu
			Sic[oct] = Si2;												// ����_��
			sop[oct] = op;												// �����߲���ۑ�
			pct += Si2;													// �ݐϐ���_��
			oct++;														// �o�^�Ȑ���
		} else {														// <�ȊO�̏ꍇ�͒��~>
			er = 1; break;
		}
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	if(oct>1&&er==0) {													// <<������ް�>>
		er = (ConctSEPoint(oct, Spb, Spb, Ssa)) ? 2 : er;				// �n�_�I�_����_�A��
		er = (GetConctObj(oct, Ssa, Ssb)) ? 2 : er;						// �A�����̏��擾
	}
	if(oct>1&&er==0) {													// <<������ް�>>
		for(i=0; i<DIM; i++) {
			vc1.p[i] = Spb[Ssb[1].p[0]].p[i]-Spb[Ssb[0].p[0]].p[i];		// �J�n�f�ʂƎ��f�ʂ̕���
		}
		j = Sib[Ssb[0].p[0]];											// �J�n�f�ʂ̐���_�J�n�ʒu
		CalcNVec(&Spa[j], Si2, &vc2);									// �J�n�f�ʖ@���擾
		if(fabs(vc2.p[0])>0.5) {										// �J�n�f�ʖ@��/���ʕ����擾
			i1 = 0, i2 = 1;
		} else if(fabs(vc2.p[1])>0.5) {
			i1 = 1, i2 = 2;
		} else {
			i1 = 2, i2 = 0;
		}
		i1 = ((vc1.p[i1]>0&&vc2.p[i1]>0)||(vc1.p[i1]<0&&vc2.p[i1]<0))	// �f�ʕ����Ɩ@��������
             ? 1 : 0;													// �����ꍇ�͖ʂ�ϓ]
		i4 = GetVtxNum(sop[Ssb[0].p[0]]);								// �J�n�f�ʋȖʒ��_���擾
		for(i=0; i<i4; i++) {
			GetVtx(sop[Ssb[0].p[0]], i, &Sps[i]);						// �J�n�f�ʋȖʒ��_���W�擾
		}
		i2 = GetCurveCenter(i2, Si3, i4, Sps, &Spb[Ssb[0].p[0]]);		// �J�n�f��2D�}�`���S�擾
		j = Sib[Ssb[oct-1].p[0]];										// �I���f�ʂ̐���_�J�n�ʒu
		CalcNVec(&Spa[j], Si2, &vc2);									// �I���f�ʖ@���擾
		i3 = (fabs(vc2.p[0])>0.5) ? 1 : ((fabs(vc2.p[1])>0.5) ? 2 : 0);	// �I���f�ʕ��ʕ����擾
		i4 = GetVtxNum(sop[Ssb[oct-1].p[0]]);							// �I���f�ʋȖʒ��_���擾
		for(i=0; i<i4; i++) {
			GetVtx(sop[Ssb[oct-1].p[0]], i, &Sps[i]);					// �I���f�ʋȖʒ��_���W�擾
		}
		i3 = GetCurveCenter(i3, Si3, i4, Sps, &Spb[Ssb[oct-1].p[0]]);	// �I���f��2D�}�`���S�擾
		if((!m_SolidFlg)||Si1) {										// OFF�͊J�n�I���f�ʖ�
			i2 = 0, i3 = 0;
		}
		for(i=0; i<oct; i++) {											// �e�f�ʖ�(���]�͊J�n�Ɠ�)
			j = Sib[i];													// �e�f�ʂ̐���_�J�n�ʒu
			InitCreat2(i1, Si3, Si2, &Spa[j], &Sia[j]);					// ������������2
		}
		i1 = oct;														// �w��f�ʐ�
		i1 = (i2) ? i1+1 : i1;											// �J�n/�I���f�ʂ����S�L��+1�Â�
		i1 = (i3) ? i1+1 : i1;
		if(Si2*i1>m_MaxSCtl) {											// <����_�������>
			AfxMessageBox(IDS_MANY_CTRLP);								// �װү����
		} else {														// <����_����ȉ�>
			for(i=0, pct=0; i<Si2; i++) {								// V����
				if(i2) {												// <�J�n�f�ʂ̒��S�L>
					Sps[pct] = Spb[Ssb[0].p[0]];						// �J�n�f�ʒ��S�_
					k = Sib[Ssb[0].p[0]];								// �J�n�f�ʂ̐���_�J�n�ʒu
					Sis[pct++] = Sia[k+i]+1;							// Ӱ�ސݒ�
				}
				for(j=0; j<oct; j++) {									// U����
					k = Sib[Ssb[j].p[0]];								// �e�f�ʂ̐���_�J�n�ʒu
					Sps[pct] = Spa[k+i];								// ���������ɐ���_�ݒ�
					Sis[pct++] = Sia[k+i];								// ����������Ӱ�ސݒ�
					if(j==0||j==oct-1) {								// U���[Ӱ�ނ�1
						Sis[pct-1]++;
					}
				}
				if(i3) {												// <�I���f�ʂ̒��S�L>
					Sps[pct] = Spb[Ssb[oct-1].p[0]];					// �I���f�ʒ��S�_
					k = Sib[Ssb[oct-1].p[0]];							// �I���f�ʂ̐���_�J�n�ʒu
					Sis[pct++] = Sia[k+i]+1;							// Ӱ�ސݒ�
				}
			}
			StartMenu();												// �ƭ��J�n����
			InitLevel();												// Undo/Redo������
			CrtSurface(i1, Si2, 0, Si3, m_CurveRepeat,
                       m_CurveWeight, m_SolidFlg, Sps, Sis);			// �Ȗʐ���
			GetObjPtr(&op);												// �Ȗʗ���OP�擾
			i = GetObjAtr(op);											// ���̑����擾
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			if(i==20) {
				SetOifName(0, oif, OBJ_NAME03);							// �̍\����(�ʏ�|����)
			} else if(i==21) {
				SetOifName(0, oif, OBJ_NAME04);							//         (�ʏ�|����[�ʑ�])
			} else if(i==22) {
				SetOifName(0, oif, OBJ_NAME05);							//         (�|���|����)
			} else {
				SetOifName(0, oif, OBJ_NAME06);							//         (�|���|����[�ʑ�])
			}
			Delete();													// 2D�}�`�폜
			OnEdtFlg(1, op);											// �Ȗ�edtflg:ON
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			Display(0);													// �ĕ`��
			SetDataUse();												// �ް��g�p���ݒ�
			EndJMenu1();												// �ƭ��I������
		}
	} else if(er==2) {
		AfxMessageBox(IDS_ERR_CONECT);									// �װү����
	} else {
		AfxMessageBox(IDS_ERR_SEL9);									// �װү����
	}
	delete[] sop;
}

/************************************/
void CJcad3GlbView::MenuSpiral(void)									//<<< �Ȗʐ���(����)
/************************************/
{
	OBJTYPE* op, * op2;
	CMDTYPE* cmp;
	BOXTYPE  box;
	int      i, i1, i2, i3, cnt=0, err=0;

	m_SolidFlg = m_AtrEnv;												// �����Ȗʑ���
	Si1 = m_SecEnv;														// �����[�ʌ`��
	if(m_FChar&0x1) {													// [F5]:�����������]
		m_SolidFlg = (m_SolidFlg) ? 0 : 1;
	}
	if(m_FChar&0x2) {													// [F6]:�[�ʌ`�󔽓]
		Si1 = (Si1) ? 0 : 1;
	}
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		GetObjBox(op, &box);											// ����BOX���ގ擾
		err = (GetObjAtr(op)/10!=1) ? 1 : err;							// �Ȑ��ȊO�ʹװ
		if(cnt==0) {
			if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {			// �Ȑ����ݕ�������
				Si2 = 1;
			} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
				Si2 = 2;
			} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
				Si2 = 0;
			} else {
				err = 2;
			}
		} else {
			err = 1;
		}
		if(err) {														// �װ�͒��~
			break;
		}
		cnt++;															// ����UP
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	if(err==0) {														// <�L���ȏꍇ>
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		if(op!=NULL) {													// <�ҏW���̗L�̏ꍇ>
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			i3 = GetObjAtr(op);											// ���̑����擾
		}
		if(i3/10==1&&cmp!=NULL) {										// <�Ȑ��̏ꍇ>
			Si3 = GetParaInt(cmp, 0);									// ����_���擾
			Si4 = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			for(i=0; i<Si3; i++) {
				Sia[i] = GetParaInt(cmp, 4+i);							// ײ�Ӱ�ގ擾
				GetParaPnt(cmp, i, &Spa[i]);							// �Ȑ�����_�擾
			}
			i1 = GetVtxNum(op);											// �Ȗʒ��_���擾
			for(i=0; i<i1; i++) {
				GetVtx(op, i, &Spb[i]);									// �Ȗʒ��_���W�擾
			}
			Si5 = GetCurveCenter(Si2, Si4, i1, Spb, &Spnt1);			// 2D�}�`���S�擾
			Si5 = ((!m_SolidFlg)||Si1) ? 0 : Si5;						// OFF�͒f�ʖ�
			SetCurve(Si3, Si4, Spa);									// �Ȑ�����_����
			GetObjPtr(&op2);											// ����_OP�ۑ�
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �Ȑ��ҏW������
			while(cmp!=NULL) {
				RedoCommandCopy(op2, cmp);								// ����ޕ��ʎ��s
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<Si3; i++) {
				GetVtx(op2, i, &Spa[i]);								// ���̕ҏW�㐧��_�擾
			}
			FreeObj(op2, 1);											// �ҏW�㗧�̍폜
		} else {
			err = 1;													// �Ȑ��װ
		}
	}
	if(err==1) {
		AfxMessageBox(IDS_ERR_SEL8);									// �װү����
	} else if(err==2) {
		AfxMessageBox(IDS_ERR_PLANE);
	} else {															// <�L���ȏꍇ>
		CSpiralDlg dlg;
		dlg.SetValue(m_SPDiameter, m_SPHeight,
                     m_SPCtrlNum, m_SPSprlNum);							// �����l�\��
		while(TRUE) {
			if(dlg.DoModal()==IDOK) {									// �������ݒ��޲�۸ޕ\��
				dlg.GetValue(&m_SPDiameter, &m_SPHeight,
                             &m_SPCtrlNum, &m_SPSprlNum);				// �������擾
				Sd1 = m_SPDiameter, Sd2 = m_SPHeight;
				Si8 = m_SPCtrlNum, Si9 = m_SPSprlNum;
			} else {
				err = 3; break;											// �������~
			}
			if(2*Si5+Si8*Si9+1>m_MaxCCtl||
               Si3*(2*Si5+Si8*Si9+1)>m_MaxSCtl) {						// ����_�����ް��
				AfxMessageBox(IDS_MANY_CTRLP);							// �װү���ތ����ײ
			} else {
				break;													// ����͏I��
			}
		}
	}
	if(err==0) {														// <�L���ȏꍇ>
		GetCentPnt(&Spnt2);												// ��ʂ̒��S�擾
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		Delete();														// 2D�}�`�폜
		InitCreat(1, Si4, Si3, Spa, Sia, &i1);							// ������������
		CreatSpiralObject(Si2, Si3, Si4, Si5, Sd1, Sd2, Si8, Si9,
                          Spnt1, Spnt2, Spa, Sia);						// �����Ȗʐ���
		EndJMenu1();													// �ƭ��I������
		CountEdtObj();													// �ҏW�Ώۗ��̶���
		Display(0);														// �ĕ`��
	}
}

/******************************************/
void CJcad3GlbView::MenuResetSurface(void)								//<<<  �Ȗʐ���(�č쐬)
/******************************************/
{
	char*    name = new char[24];
	OBJTYPE* op, * cop, * dop;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	int      cpf, cmf, un, vn, ub, vb, mx, wt, fg, ct, i, j, i1, i2, i3, mir, err=0;
	double   dwt;

	cpf = (m_FChar&0x1) ? 1 : 0;										// [F5]:�������`��
	cpf = (m_FChar&0x2) ? 2 : cpf;										// [F6]:�����ʒu
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		i3 = GetObjAtr(op);												// ���̑����擾
		err = (cmp==NULL) ? 1 : err;									// ����ޖ����ʹװ
		err = (i3/10!=1&&i3!=20&&i3!=21) ? 1 : err;						// �Ȑ���ʏ�ȖʈȊO�ʹװ
		if(err) {														// �װ�͒��~
			break;
		}
		SetSelNo(op, 1);												// �č쐬���̓o�^
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	if(!err) {															// <<����ȏꍇ>>
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		SelBaseObjPtr(1, &op);											// �č쐬���̐擪�߲���擾
		while(op!=NULL) {
			cmf = (cpf==1) ? 0 : 1;										// ����ތ���(0:�擪,1:�L���擪)
			BaseCmdPtr(cmf, op, &cmp);									// �L���擪����ގ擾
			i3 = GetObjAtr(op);											// ���̑����擾
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			GetOifName(oif, name);										// ���̍\�����擾
			if(i3==10||i3==11) {										// <�Ȑ��̏ꍇ>
				un = GetParaInt(cmp, 0);								// ����_���擾
				ub = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				mx = GetParaInt(cmp, 2);								// �Ȑ����J�Ԑ�
				wt = GetParaInt(cmp, 3);								// �Ȑ��d�݌W��
				dwt = wt/1000.0;
				for(i=0, ct=0; i<un; i++) {
					GetParaPnt(cmp, i, &Spa[ct]);						// ����_�擾
					Sia[ct++] = GetParaInt(cmp, i+4);					// ײ�Ӱ�ގ擾
				}
				if(cpf==0) {											// <���݈ʒu�ōĐ���>
					SetCurve(un, ub, Spa);								// �Ȑ�����_����
					GetObjPtr(&dop);									// ����_OP�ۑ�
					NextCmdPtr(&cmp);									// �Ȑ��ҏW������
					while(cmp!=NULL) {
						RedoCommandCopy(dop, cmp);						// ����ޕ��ʎ��s
						NextCmdPtr(&cmp);								// ������ނ�
					}
					for(i=0; i<un; i++) {
						GetVtx(dop, i, &Spa[i]);						// ���̕ҏW�㐧��_�擾
					}
					FreeObj(dop, 1);									// ���̍폜
				}
				CrtCurve(un, ub, mx, dwt, Spa, Sia);					// �Ȑ�����
				GetObjPtr(&cop);										// �������̎擾
			} else {													// <�ʏ�Ȗʂ̏ꍇ>
				un = GetParaInt(cmp, 0);								// U����_���擾
				vn = GetParaInt(cmp, 1);								// V����_���擾
				ub = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
				vb = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
				mx = GetParaInt(cmp, 4);								// �Ȑ����J�Ԑ�
				wt = GetParaInt(cmp, 5);								// �Ȑ��d�݌W��
				dwt = wt/1000.0;
				fg = GetParaInt(cmp, 6);								// �د��Ӱ��
				for(i=0, ct=0; i<vn; i++) {
					for(j=0; j<un; j++) {
						GetParaPnt(cmp, un*i+j, &Spa[ct]);				// ����_�擾
						Sia[ct++] = GetParaInt(cmp, un*i+j+7);			// Ӱ�ގ擾
					}
				}
				if(cpf==0) {											// <���݈ʒu�ōĐ���>
					SetSurface(un, vn, ub, vb, Spa);					// �Ȗʐ���_����
					GetObjPtr(&dop);									// ����_OP�ۑ�
					NextCmdPtr(&cmp);									// �ȖʕҏW������
					mir = 0;
					while(cmp!=NULL) {
						if(GetCmdNo(cmp)==MIRROR) {						// ��������ސ�����
							mir++;
						}
						if(GetCmdNo(cmp)!=THICKNESS) {					// ���ݕt���ȊO��
							RedoCommandCopy(dop, cmp);					// ����ޕ��ʎ��s
						}
						NextCmdPtr(&cmp);								// ������ނ�
					}
					j = un*vn;
					for(i=0; i<j; i++) {
						GetVtx(dop, i, &Spa[i]);						// ���̕ҏW�㐧��_�擾
					}
					FreeObj(dop, 1);									// ���̍폜
					if(mir%2==1) {
						ReverseSurf(un, vn, Spa, Sia);					// �Ȗʐ���_���]
					}
				}
				CrtSurface(un, vn, ub, vb, mx, dwt, fg, Spa, Sia);		// �Ȗʐ���
				GetObjPtr(&cop);										// �������̎擾
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)==THICKNESS) {						// ���ݕt����
						RedoCommandCopy(cop, cmp);						// ����ޕ��ʎ��s
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
			}
			GetObjOif(cop, &oif);										// ���̍\������߲���擾
			SetOifName(0, oif, name);									// ���̍\�����ݒ�
			OnEdtFlg(1, cop);											// edtflg:ON
			dop = op;													// �폜�����߲���ۑ�
			SelNextObjPtr(1, &op);										// ���č쐬����
			LevelObj(1, dop);											// �������ٍ폜���̐ڑ�
			TempObj(dop);												// ���̈ꎞ�폜
		}
		ExitMenu(0, 0);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	} else {
		AfxMessageBox(IDS_ERR_SEL5);									// �װү����
	}
	ResetSelNo();														// ���̑I��ؾ��
	CountEdtObj();														// �ҏW�Ώۗ��̶���
	SetDataUse();														// �ް��g�p���ݒ�
	Display(0);															// �ĕ`��
	delete[] name;
}

/*************************************/
void CJcad3GlbView::MenuConnect(void)									//<<<  �Ȗʐ���(�A��)
/*************************************/
{
	OBJTYPE** sop = new OBJTYPE*[1000];
	OBJTYPE*  op;
	OIFTYPE*  oif;
	CMDTYPE*  cmp;
	int       i, j, i1, i2, i3, i4, clf, cn, ct1=0, ct2=0, err=0;

	clf = (m_FChar&0x1) ? 1 : 0;										// [F5]:�A���׸�
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		i3 = GetObjAtr(op);												// ���̑����擾
		if(i3/10==1&&cmp!=NULL) {										// <�Ȑ��̏ꍇ>
			sop[ct1] = op;												// �����߲���ۑ�
			Sia[ct1] = GetParaInt(cmp, 0);								// ����_���擾
			Siw[ct1] = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
			if(Siw[ct1]==1) {											// ���EӰ��:�ʹװ
				err = 1; break;
			}
			if(++ct1>1000) {											// �Ȑ�����
				err = 1; break;
			}
		} else if((i3==20||i3==21)&&cmp!=NULL) {						// <�ʏ�Ȗʂ̏ꍇ>
			sop[ct2] = op;												// �����߲���ۑ�
			Sia[ct2] = GetParaInt(cmp, 0);								// U����_���擾
			Sib[ct2] = GetParaInt(cmp, 1);								// V����_���擾
			Siw[ct2] = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
			Six[ct2] = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
			Sic[ct2] = i3;												// ���̑����ۑ�
			if(Siw[ct2]==1&&Six[ct2]==1) {								// ���EӰ��:�ʹװ
				err = 1; break;
			}
			if(++ct2>1000) {											// �Ȗʶ���
				err = 1; break;
			}
		} else {														// �Ȑ���ȖʈȊO�ʹװ
			err = 1; break;
		}
		if(ct1>0&&ct2>0) {												// �Ȑ���Ȗʍ��݂ʹװ
			err = 1; break;
		}
		while(cmp!=NULL) {
			i = GetCmdNo(cmp);											// �����No�擾
			if(i!=CRTCURVE&&i!=CRTSURF&&i!=CRTSWEEP&&
               i!=CHGCURVE&&i!=CHGSURF&&i!=CHGSWEEP) {					// �Ȑ���Ȗʐ����ȊO�װ
				err = 2; break;
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		if(err) {
			break;
		}
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	err = (!err&&ct1+ct2<1) ? 1 : err;									// �ް����ʹװ
	if(!err) {
		if(ct2>0) {														// <����Ȗʂȏꍇ>
			err = CheckSurface(ct2, Sia, Sib, Siw, Six, Sic, &i2);		// �ȖʘA������
			i1 = 1;
		} else {
			i1 = 0;
		}
	}
	if(!err) {															// <����ȏꍇ>
		cn = (ct1>ct2) ? ct1 : ct2;										// �A�����̐�
		GetSEPoint(i1, i2, cn, sop, Sia, Sib, Siw, Six,
                   Spa, Spb, Ssa, &i3, &i4);							// �n�_�I�_����_�擾
		if(cn>1) {
			err = (ConctSEPoint(cn, Spa, Spb, Ssb)) ? 4 : err;			// �n�_�I�_����_�A��
		}
	}
	if(!err&&cn>1) {													// <����ȕ������̂̏ꍇ>
		for(i=0, j=0; i<cn; i++) {
			if(Ssb[i].p[0]<0) {
				j++;													// �n�_�����A��������
			} else if(Ssa[i].p[0]) {
				i3--;													// �n�_�������_�����Z
			}
			if(Ssb[i].p[1]<0) {
				j++;													// �I�_�����A��������
			} else if(Ssa[i].p[1]) {
				i3--;													// �I�_�������_�����Z
			}
		}
		err = (i3>m_MaxCCtl||i3*i4>m_MaxSCtl) ? 3 : err;				// ����_��������ް
		err = (j!=2) ? 4 : err;											// ���A������2�ȊO�ʹװ
	}
	if(!err) {															// <����ȏꍇ>
		if(cn>1) {
			err = (GetConctObj(cn, Ssb, Ssc)) ? 4 : err;				// �A�����̏��擾
		} else {
			Ssc[0].p[0] = 0, Ssc[0].p[1] = 0;
		}
	}
	if(!err) {															// <<����ȏꍇ>>
		clf = (cn==1) ? 1 : clf;										// 1���͕̂K��,���E:��
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		if(i1==0) {
			ConctCurve(clf, cn, sop, Ssc);								// �Ȑ��A��
		} else {
			ConctSurface(clf, i2, cn, sop, Ssc, Ssa);					// �ʏ�ȖʘA��
		}
		GetObjPtr(&op);													// �������̎擾
		GetObjOif(op, &oif);											// ���̍\������߲���擾
		if(i1==0) {
			if(clf) {
				SetOifName(0, oif, OBJ_NAME01);							// �̍\����(�Ȑ�[��])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//         (�Ȑ�[�J])
			}
		} else {
			SetOifName(0, oif, OBJ_NAME03);								//         (�ʏ�|����)
		}
		Delete();														// �I�𗧑̍폜
		OnEdtFlg(1, op);												// edtflg:ON
		ExitMenu(0, 0);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
		Display(0);														// �ĕ`��
	} else if(err==2) {													// ���̕ҏW����ޗL�̏ꍇ
		AfxMessageBox(IDS_ERR_EDIT);
	} else if(err==3) {													// ����_������������ꍇ
		AfxMessageBox(IDS_MANY_CTRLP);
	} else if(err==4) {													// �A���s�\�ȏꍇ
		AfxMessageBox(IDS_ERR_CONECT);
	} else {															// �Ȑ���ȖʈȊO�������ُ�
		AfxMessageBox(IDS_ERR_SEL5);
	}
	delete[] sop;
}

/***************************************/
void CJcad3GlbView::MenuThickness(void)									//<<< �Ȗʐ���(���݂Â�)
/***************************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	int      i1, i2, i3=0;
	double   d1=0.0;

	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	if(op!=NULL) {
		i3 = GetObjAtr(op);												// ���̑����擾
	}
	if(i3==21||i3==23) {												// <�Ȗ�(�ʑ�)�̏ꍇ>
		OffEdtOtherObject(op);											// �w��O�ҏW�Ώۉ���
		Display(0);														// �ĕ`��
		CInputDlg dlg(5);
		dlg.SetValue(m_ThickValue);										// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �����޲�۸ޕ\��
			dlg.GetValue(&m_ThickValue);								// ���ݕ�����
			d1 = m_ThickValue;
		}
		if(fabs(d1)>EPSILON) {
			StartMenu();												// �ƭ��J�n����
			InitLevel();												// Undo/Redo������
			Thickness(op, d1);											// �Ȗʌ��݂Â�
			i2 = GetObjAtr(op);											// ���̑����擾
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			if(i2==20) {
				SetOifName(1, oif, OBJ_NAME03);							// �̍\����(�ʏ�|����)
			} else {
				SetOifName(1, oif, OBJ_NAME05);							//         (�|���|����)
			}
			Display(0);													// �ĕ`��
			EndJMenu1();												// �ƭ��I������
		}
	} else {
		AfxMessageBox(IDS_ERR_SEL6);									// �װү����
	}
}
