/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewCurve.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "CreatCurvePoint.h"
#include "EditSurface.h"
#include "EditCtrlTracker.h"
#include "Symmetry.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*************************************************************/
void CJcad3GlbView::MouseCurve(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< �Ȑ�����
/*************************************************************/
{
	OIFTYPE* oif;
	PNTTYPE  pnt;
	int      i, j, f1, f2, endf=0;
	double   th, dd, d[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2&&HitTestPlane(point)==Si2&&Si1>0) {				// <��������OK>
			CalcScreenWorld(0, &pnt, point, Si2);						// ���W�_�擾
			if(CheckNextPoint(Spnt1, pnt)) {							// ���O�_�Ƃ̋���OK
				if(m_CurveAFlg&&Si1>1) {								// <�p�x�w��L��>
					th = Sd5-(PI*m_CurveAngle/180.0);					// �␳��p�x
					LimitAngle(Si2, &pnt, &Spnt1, th);					// ϳ��p�x�w�萧��
				} else {												// <�p�x�w�薳��>
					if(flg&m_LimKey) {
						LimitPoint(Si2, &pnt, &Spnt1);					// ϳ����͐������W
					}
				}
				Sia[Si1] = 0;											// Ӱ�ސݒ�(��)
				Spb[Si1] = pnt;											// ����_�ݒ�(��)
				CalcCurvePoint(Si4, Si1+1, Spb, Spa, Sdb);				// �ʉߓ_=>����_�ϊ�
				RedoCrtCurve(Si1+1, 0, Spa, Sia);						// �Ȑ�����(��)
				RedoSetCurve(Si1+1, 0, Spb);							// �Ȑ�����_����(��)
				for(i=0; i<DIM; i++) {
					d[i] = pnt.p[i]-Spnt1.p[i];							// ���O�_�Ƃ̍�
				}
				dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);				// ���O�_�Ƃ̋���
				m_pNumDisp->SetParam(d[0], d[1], d[2], dd);				// ϳ��ړ��ʕ\��
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// �ׯ��1�ݒ�
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			Sphase = 1;
		} else if(Sphase==1&&(Si2==-1||Si2==plane)) {					// <<�����1�_��>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			i = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                  Si12, Si15, Si16);					// �Ώ̗̈�����
			if(Si5!=3||i==1) {											// ��]�Ώ̈ȊO������OK
				InitLevel();											// Undo/Redo������
				Si2 = (Si2==-1) ? plane : Si2;							// �w�蕽��
				Sia[Si1] = (m_CurInp) ? 1 : 0;							// Shift�ܐ��_Ӱ��
				Spa[Si1] = pnt;											// ����_�ݒ�(�{)
				Spb[Si1++] = pnt;
				Spnt1 = pnt;											// ���W�_�ۑ�
				CrtCurve(Si1, 0, m_CurveRepeat, m_CurveWeight,
                         Spa, Sia);										// �Ȑ�����(�{)
				GetObjPtr(&Sop1);										// �Ȑ�����OP�ۑ�
				SetCurve(Si1, 0, Spb);									// �Ȑ�����_����(�{)
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				SetSelNo(Sop2, 2);										// ���F�\��OP�ݒ�
				for(i=0; i<Si1; i++) {
					Sib[i] = Sia[i]*2;
				}
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// �ׯ��1�ݒ�
				Sphase = 2;
				Sd3 = (m_CurveRFlg) ? m_CurveRadius : 0.0;				// �I�_R�w��ۑ�
				ClearCurveData();										// �Ȗʐ����ް��ر
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Display(0);											// �ĕ`��
					CInputBtnDlg5 dlg(1);
					dlg.SetParam1(Si4, Si5, Si13, Sbase, Si12, Si15,
                                  Si16, Si3, m_CurveWeight, &Si1, Spa,
                                  Spb, Sia, Sib, Sda, Sdb, m_CurveAdd);	// ���Ұ��ݒ�1
					dlg.DoModal();										// �����޲�۸ޕ\��
					dlg.GetParam(&m_CurveAdd);							// ���Ұ��擾
					endf = 2;											// �ƭ��I��
				}
			}
		} else if(Sphase==2&&Si2==plane) {								// <<2�_�ڈȍ~�œ��ꕽ��>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(Si1==0) {												// <1�_�ڂ܂Ŏ������ꂽ�ꍇ>
				i = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                      Si12, Si15, Si16);				// �Ώ̗̈�����
				if(i==1) {												// ��]�Ώ̈ȊO������OK
					Sia[Si1] = (m_CurInp) ? 1 : 0;						// Shift�ܐ��_Ӱ��
					Spa[Si1] = pnt;										// ����_�ݒ�(�{)
					Spb[Si1++] = pnt;
					Spnt1 = pnt;										// ���W�_�ۑ�
					CrtCurve(Si1, 0, m_CurveRepeat, m_CurveWeight,
                             Spa, Sia);									// �Ȑ�����(�{)
					GetObjPtr(&Sop1);									// �Ȑ�����OP�ۑ�
					SetCurve(Si1, 0, Spb);								// �Ȑ�����_����(�{)
					GetObjPtr(&Sop2);									// ����_OP�ۑ�
					SetSelNo(Sop2, 2);									// ���F�\��OP�ݒ�
					for(i=0; i<Si1; i++) {
						Sib[i] = Sia[i]*2;
					}
					Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);		// �ׯ��1�ݒ�
					Sd3 = (m_CurveRFlg) ? m_CurveRadius : 0.0;			// �I�_R�w��ۑ�
					ClearCurveData();									// �Ȗʐ����ް��ر
					SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                    Sia, Sda, Sdb);						// �ʉߓ_=>����_�ϊ���د��
				}
			} else {													// <2�_�ڈȍ~>
				i = CheckNextPoint(Spnt1, pnt);							// ���O�_�Ƃ̋�������
				j = CheckSymmetryArea(&Si13, pnt, Si5, Sbase,
                                      Si12, Si15, Si16);				// �Ώ̗̈�����
				if(i&&j) {												// ����OK
					if(m_CurveAFlg&&Si1>1) {							// <�p�x�w��L��>
						th = Sd5-(PI*m_CurveAngle/180.0);				// �␳��p�x
						LimitAngle(Si2, &pnt, &Spnt1, th);				// ϳ��p�x�w�萧��
					} else if(Si1>0) {									// <�p�x�w�薳��>
						if(flg&m_LimKey) {								// ϳ����͐������W
							LimitPoint(Si2, &pnt, &Spnt1);
						}
					}
					if((Si1+1)*Si12<=m_MaxCCtl) {						// <����_�������>
						Sia[Si1] = (m_CurInp) ? 1 : 0;					// Shift�ܐ��_Ӱ��
						Spb[Si1++] = pnt;								// ����_�ݒ�(�{)
						Spnt1 = pnt;									// ���W�_�ۑ�
						dd = (m_CurveRFlg) ? m_CurveRadius : 0.0;
						if(dd>1.0&&Si1>2) {								// <R�w��L��>
							CalcRPoint(0, Si2, Si12, dd, &Si1, Spb,Sia);// R���_�ʒu�Z�o
						}
					} else {
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1,
                                        Sia, Sda, Sdb);					// �ʉߓ_=>����_�ϊ���د��
					}
					CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);			// �ʉߓ_=>����_�ϊ�
					RedoCrtCurve(Si1, 0, Spa, Sia);						// �Ȑ�����(�{)
					RedoSetCurve(Si1, 0, Spb);							// �Ȑ�����_����(�{)
					for(i=0; i<Si1; i++) {
						Sib[i] = Sia[i]*2;
					}
					Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);		// �ׯ��1�ݒ�
					if(Si1>1) {
						Sd5=CalcBaseAngle(Spb[Si1-2], Spb[Si1-1], Si2);	// ��p�x�擾
					}
				}
				if(Si1==2) {
					Sd4 = (m_CurveRFlg) ? m_CurveRadius : 0.0;			// �n�_R�w��ۑ�
				}
				ClearCurveData();										// �Ȗʐ����ް��ر
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(Si5==4) {												// <����Ӱ��:����>
				if(Si8*Si9+1<=m_MaxCCtl) {								// ������_��
					InitLevel();										// Undo/Redo������
					Si1 = Si8*Si9+1;
					CalcSpiralPoint(pnt, plane, Sd1, Sd2, Si8,
                                    Si9, Spb, Sia);						// ��������_�v�Z
					SetCurveMatrix1(Si4, m_CurveWeight, Si3, Si1,
                                    Sia, Sda, Sdb);						// �ʉߓ_=>����_�ϊ���د��
					CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);			// �ʉߓ_=>����_�ϊ�
					CrtCurve(Si1, Si3, m_CurveRepeat, m_CurveWeight,
                             Spa, Sia);									// �Ȑ�����(�{)
					GetObjPtr(&Sop1);									// �Ȑ�����OP�ۑ�
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// �װү����
				}
				endf = 2;												// �ƭ��I��
			} else {													// <����Ӱ��:�����ȊO>
				Si2 = plane;											// �w�蕽��
				ScrToPnt(0, &m_Cent3D, &Sbase);							// 3�ʐ}���S�_
				Si14 = (flg&MK_CONTROL) ? 1 : 0;						// �������w��
				SetSymmetryLine(0, Si5, Sbase, Si2, Si14, Si12,
                                &Si15, &Si16);							// �Ώ̎��ݒ�
			}
		}
	} else if(ms==11) {													// <<<DEL(���)>>>
		if(Sphase==2&&Si1>0) {											// <����_��o�^�ς�>
			Si1--;														// ����_�����Z
			if(Si1>0) {
				Spnt1 = Spb[Si1-1];										// ���W�_�ۑ�
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1,
                                Sia, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
				CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);				// �ʉߓ_=>����_�ϊ�
				RedoCrtCurve(Si1, 0, Spa, Sia);							// �Ȑ�����(�{)
				RedoSetCurve(Si1, 0, Spb);								// �Ȑ�����_����(�{)
				for(i=0; i<Si1; i++) {
					Sib[i] = Sia[i]*2;
				}
				Tracker1.Setup(Spb, Sib, NULL, Si1, Si2, 0);			// �ׯ��1�ݒ�
				Sd5 = CalcBaseAngle(Spb[Si1-2], Spb[Si1-1], Si2);		// ��p�x�擾
				SetCurveMatrix1(Si4, m_CurveWeight, 0, Si1+1,
                                Sia, Sda, Sdb);							// �ʉߓ_=>����_�ϊ���د��
			} else {
				Spnt1.p[0]=DBL_MAX, Spnt1.p[1]=DBL_MAX, Spnt1.p[2]=DBL_MAX;
				Si13 = 0;												// �Ώ̗̈斢����
				if(Sop1!=NULL) {
					FreeObj(Sop1, 1);									// ���̍폜
				}
				if(Sop2!=NULL) {
					FreeObj(Sop2, 1);									// ���̍폜
				}
				Tracker1.Reset(0);										// �ׯ��1����
			}
			Display(0);													// �ĕ`��
		}
		ClearCurveData();												// �Ȗʐ����ް��ر
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			i = (m_FChar&0x1) ? 1 : 0;									// [F5]:���͐����L
			Si4 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
			Si5 = 0;													// ����Ӱ��:�ʏ�
			Si5 = (m_FChar&0x2) ? 3 : Si5;								// [F6]:����Ӱ��:��]�Ώ�
			Si5 = (m_FChar&0x8) ? 2 : Si5;								// [F8]:����Ӱ��:2���Ώ�
			Si5 = (m_FChar&0x4) ? 1 : Si5;								// [F7]:����Ӱ��:1���Ώ�
			Si5 = (m_FChar&0x20) ? 4 : Si5;								// [F10]:����Ӱ��:����
			if(Si5==4) {												// <����Ӱ��:����>
				i = 0, Si4 = 1, Si3 = 0, m_NumInp = 0;					// ������/�ʉߓ_/���E�J/���l��
				CSpiralDlg dlg;
				dlg.SetValue(m_SPDiameter, m_SPHeight,
                             m_SPCtrlNum, m_SPSprlNum);					// �����l�\��
				if(dlg.DoModal()==IDOK) {								// �������ݒ��޲�۸ޕ\��
					dlg.GetValue(&m_SPDiameter, &m_SPHeight,
                                 &m_SPCtrlNum, &m_SPSprlNum);			// �������擾
					Sd1 = m_SPDiameter, Sd2 = m_SPHeight;
					Si8 = m_SPCtrlNum,  Si9 = m_SPSprlNum;
				} else {
					ExitMenu(0, 1);                                     // �ƭ��I��
				}
			} else {													// <����Ӱ��:�����ȊO>
				if(m_Char=='O') {										// O������:�J
					Si3 = 0;
				} else if(m_Char=='C') {								// C������:��
					Si3 = 1;
				} else {
					CCurveDlg dlg;
					if(dlg.DoModal()==IDOK) {							// �J/�ݒ��޲�۸ޕ\��
						dlg.GetValue(&Si3);
						Si3 = (Si3==0) ? 1 : 0;
					} else {
						ExitMenu(0, 1);									// �ƭ��I��
					}
				}
			}
			if(Si5==3) {												// <����Ӱ��:��]�Ώ�>
				Si12 = (InputRevolveDivi(&m_SymRDivi)) ? m_SymRDivi : 0;// ��]�Ώ̕������擾
				if(Si12<0) {
					ExitMenu(0, 1);										// �ƭ��I��
				}
			} else if(Si5==1) {
				Si12 = 2;												// �ر������
			} else if(Si5==2) {
				Si12 = 4;
			} else { Si12 = 1;
			}
			if(m_ActMenu!=NONE) {										// <���~�łȂ��ꍇ>
				i = (m_NumInp!=0) ? 0 : i;								// ���l����:�L�͐�����
				Sop1 = NULL, Sop2 = NULL;
				Si1 = 0, Si2 = -1, Si13 = 0;
				Sd3 = 0.0, Sd4 = 0.0;
				OffEdtAllObject();										// �S����edtflg:OFF
				OffUndoRedo();											// Undo/Redo�s��
				Display(0);												// �ĕ`��
				Sphase = (Si5==0) ? 1 : Sphase;							// �ʏ�Ӱ�ނ͐���_�w���
				m_CursorNo = 1;											// ����No�ݒ�
				if(i) {
					ShowCurveCtrl();									// �Ȑ������޲�۸ޕ\��
				} else {
					ClearCurveData();									// �Ȗʐ����ް��ر
				}
				StartMenu();											// �ƭ��J�n����
				SetNumDisp(2);											// ���l�\����������
			}
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		endf = (ms==9) ? 1 : 2;											// �ƭ��I��
	}
	if(endf>0) {														// <�ƭ��I���̏ꍇ>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// ���̍폜
		}
		if(Si1==2&&Si3==0&&Si5==0) {									// 2�_&�J&�ʏ�Ӱ�ނ̏ꍇ
			InsertCurvePoint(&m_InstPNum, &Si1, Spb, Sia);				// S�Ȑ����Ԑ���_�}��
		}
		if(Si1>0) {														// 1�_�ȏ�w��̏ꍇ
			i = Si1;													// ���ʑO����_���ۑ�
			j = CopySymmetryArea(Si5, Sbase, Si12, Si15, Si16, &Si1,
                                 &Si3, Spb, Sia, &f1, &f2);				// �Ώ̗̈敡��
			if(j>0) {													// <2���Ώ̂�1���̂ݗL��>
				Si5 = 1;												// ����Ӱ�ނ�1���Ώ̂�
				Si14 = (Si2==1) ? ((j==1)?0:1) : ((j==1)?1:0);			// �������w���׸ސݒ�
			}
		}
		if(Si1>2) {														// <<<����ȋȐ�>>>
			if(Si3) {													// <<�Ȑ��̏ꍇ>>
				th = (m_CurveRFlg) ? m_CurveRadius : 0.0;
				dd = (th>1.0) ? th : Sd3;
				if(dd>1.0) {
					CalcRPoint(2, Si2, Si12, dd, &Si1, Spb, Sia);		// �I�_R���_�ʒu�Z�o
				}
				if(Sd4>1.0) {
					CalcRPoint(1, Si2, Si12, Sd4, &Si1, Spb, Sia);		// �n�_R���_�ʒu�Z�o
				}
			}
			SetCurveMatrix1(Si4, m_CurveWeight, Si3, Si1,
                            Sia, Sda, Sdb);								// �ʉߓ_=>����_�ϊ���د��
			CalcCurvePoint(Si4, Si1, Spb, Spa, Sdb);					// �ʉߓ_=>����_�ϊ�
			RedoCrtCurve(Si1, Si3, Spa, Sia);							// �Ȑ�����(�{)
			GetObjOif(Sop1, &oif);										// ���̍\������߲���擾
			if(Si3) {
				SetOifName(0, oif, OBJ_NAME01);							// ���̍\������(�Ȑ�[��])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//             (�Ȑ�[�J])
			}
			OnEdtFlg(1, Sop1);											// �Ȑ�edtflg:ON
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		} else if(Sphase==2) {											// <�ُ�ȋȐ�>
			AfxMessageBox(IDS_FEW_CTRLP);								// �װү����
			FreeObj(Sop1, 1);											// �Ȑ��폜
			BackLevel();												// ���ٖ�
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Line.Reset();													// ײ݉���
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		EndJMenu1();													// �ƭ��I������
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		ClearCurveData();												// �Ȗʐ����ް��ر
		HideCurveCtrl();												// �Ȑ������޲�۸ޏ���
		SetDataUse();													// �ް��g�p���ݒ�
		if(endf==2&&Si1>2) {											// <���ƭ����ŏI��>
			Wi0 = m_NumInp;												// ���l����Ӱ�ތp��
			Wi4 = 0;													// �Ώ̈ȊO��0
			m_FChar = (Si4) ? m_FChar|0x10 : m_FChar;					// [F9]:�ʉߓ_Ӱ�ތp��
			if(Si5>0&&Si5<4) {											// 1���2�����]�̏ꍇ
				Wi4 = i;												// ���ʑO����_��
				if(Si5==1) {											// [F7]:1���Ώ̌p��
					m_FChar = m_FChar|0x4;
				} else if(Si5==2) {										// [F8]:2���Ώ̌p��
					m_FChar = m_FChar|0x8;
				} else if(Si5==3) {										// [F6]:��]�Ώ̌p��
					m_FChar = m_FChar|0x2;
				}
				Wi1 = Si12, Wi2 = Si2, Wi3 = Si14, Wi5 = f1, Wi6 = f2;	// �Ώ̎����p��
				SpntA = Sbase;
			} else {
				Wi1 = 0, Wi2 = 0, Wi3 = 0, Wi5 = 0, Wi6 = 0;
			}
			SetActMenu(M_EDIT_CURVE);									// �����I�ɢ�Ȑ��ҏW��N��
			InitActMenu(0);
		}
	}
}

/**************************************************************/
void CJcad3GlbView::MouseECurve(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< �Ȑ��ҏW
/**************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&(Sphase==1||Sphase==3)&&plane!=3) {			// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			GetMoveValue(plane, Sbase, &pnt);							// �ׯ���ړ��ʎ擾
			MoveSymmetryArea(Sbase, pnt, Si6, Spnt1, Si7, Si11, Si12,
                             Si14, Si15, Si9, Si13, Si1, Sic, Spy, Spw);// �Ώ̗̈�ړ��ʐݒ�
			MoveECurve(Si5, Sdb, Spw, Sop1, Si1, Si2, Spa, Spb,
                       Sic, Spc, Tens);									// ����_/�ʉߓ_�ړ�
			RedoChgCurve(Si1, Si2, Spc, Sia);							// �Ȑ�����
			TrackerECurve(Si5, Sda, Si2, Si1, Spc, Sia, Sic,
                          &Si3, Spd, Sib);								// �Ȑ��ҏW�ׯ���擾
			RedoSetCurve(Si3, Si2, Spd);								// �Ȑ�����_����
			for(i=0; i<Si3; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			i = (Si13>0) ? Si13*2-1 : Si3;								// �p���͕\�����ύX
			DispParsTracker(0, Si21, Spd, Sib, i, Spx, Six, Si10);		// �����}�ׯ���\��
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// �ړ��O�_�Ƃ̋���
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Si4 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				i = (Si13>0) ? Si13*2-1 : Si3;							// �p���͕\�����ύX
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
			}
		} else if(Sphase==1||Sphase==3) {								// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor;
				box.bp[1] = pnt;
				i = (Si13>0) ? Si13*2-1 : Si3;							// �p���͕\�����ύX
				DragCtrlTracker1(plane, box, Spb, i, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// �ׯ����ׯ��1
				if(Si13>0) {											// �p���̏ꍇ��
					PickSymmetryArea(Si6, Si7, Si13*2-1, Si3,
                                     Si14, Si15, &i1, Siy);				// �Ώ̗̈��ׯ���I��
				}
				if(i1>0) {												// <����_�I���̏ꍇ>
					for(i=0, i2=0; i<i1; i++) {
						if(Siy[i]%2!=0) {
							i2++;										// ����_�}��������
						} else {
							i2 = 0; break;								// �����̑I���͑}��NG
						}
					}
					if(Si1+i2>m_MaxCCtl) {								// <����_����ȏ�>
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
						Sphase = 1;
					} else {											// <����_�������>
						if(Si13>0) {									// �����p��Ӱ�ނ̏ꍇ
							for(i=0, i2=0; i<i1; i++) {
								if(Siy[i]>=Si13*2-1) {
									break;								// �����̈�O�͏I��
								}
								if(Siy[i]%2!=0) {
									i2++;								// �����̈�}���Ώ۶���
								} else {
									i2 = 0; break;						// �����̑I���͑}��NG
								}
							}
							Si13 += i2;									// �����̈搧��_�����Z
						}
						PickECurve(i1, Siy, Si5, Sda, Sdb, &Si1, Si2,
                                   m_ObjCRpt, m_ObjCWgt, &Si3, Sop2,
                                   m_CurInp, Spa, Spb, Sia, Sib, Sic);	// �Ȑ��ҏW �_�I��
						i = (Si13>0) ? Si13*2-1 : Si3;					// �p���͕\�����ύX
						SetParsTracker(0, &Si21, 1, Sic, Spb, Sib, i,
                                       Spx, Six, &Si10);				// �����}�ׯ���ݒ�
						Sphase = 3;
						Si23 = 1;
					}
				}
			}
		} else if(Sphase==2) {											// <����_�ړ��̏ꍇ>
			if(Si4==1) {												// �ړ��ς݂̏ꍇ��
				for(i=0; i<Si1; i++) {									// �������ޯ̧�ύX
					Spa[i] = Spc[i], Spy[i] = Spd[i*2];
				}
				for(i=0; i<Si3; i++) {
					Spb[i] = Spd[i];
				}
			}
			Sphase = 3;
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				CalcScreenWorld(0, &Spnt1, point, plane);				// �Ώ̎����S�_�擾
				Si8 = plane;											// �Ώ̎��w�蕽��
				i = (flg&MK_CONTROL) ? 1 : 0;							// �������w��
				Si9 = (flg&MK_SHIFT) ? 1 : 0;							// ���������]
				SetSymmetryLine(1, Si6, Spnt1, Si8, i, Si7,
                                &Si11, &Si12);							// �Ώ̎��ݒ�
			}
		} else {														// <����_�w��̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			i = (Si13>0) ? Si13*2-1 : Si3;								// �p���͕\�����ύX
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, i,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Siy[0]%2==0&&
               Sic[Siy[0]/2]==1&&plane!=3) {							// <����_�ړ��̏ꍇ>
				Sbase = Spb[Siy[0]];									// �ړ����_�ۑ�
				Sphase = 2;
				Si4 = 0;
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Sphase = 3;
					CInputBtnDlg6 dlg(1);
					dlg.SetParam1(Sop1, Sop2, Si5, Si21, Si1, Si2, Si10,
                                  &Si4, Si13, Si6, Si7, Si11, Si12, Si14,
                                  Si15, Si9, Sbase, Spnt1, Spa, Spb, Spc,
                                  Spd, Spw, Spx, Spy, Sia, Sib, Sic, Six,
                                  Tens, Sda, Sdb, m_ECurveAdd);         // ���Ұ��ݒ�1
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						if(Si4==1) {									// �ړ��ς݂̏ꍇ��
							for(i=0; i<Si1; i++) {						// �������ޯ̧�ύX
								Spa[i] = Spc[i], Spy[i] = Spd[i*2];
							}
							for(i=0; i<Si3; i++) {
								Spb[i] = Spd[i];
							}
						}
					}
					dlg.GetParam(&m_ECurveAdd);							// ���Ұ��擾
					RedoChgCurve(Si1, Si2, Spa, Sia);					// �Ȑ�����
					TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia,
                                  Sic, &Si3, Spb, Sib);					// �Ȑ��ҏW�ׯ���擾
					RedoSetCurve(Si3, Si2, Spb);						// �Ȑ�����_����
					for(i=0; i<Si3; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					i = (Si13>0) ? Si13*2-1 : Si3;						// �p���͕\�����ύX
					DispParsTracker(0, Si21, Spb, Sib, i,
                                    Spx, Six, Si10);					// �����}�ׯ���\��
				}
			} else if(i1>0) {											// <����_�I���̏ꍇ>
				if(!m_CtlInp) {
					for(i=0; i<Si1; i++) {
						Sic[i] = 0;										// �S����_�I������
					}
				}
				if(Si13>0) {											// �p���̏ꍇ��
					PickSymmetryArea(Si6, Si7, Si13*2-1, Si3,
                                     Si14, Si15, &i1, Siy);				// �Ώ̗̈��ׯ���I��
				}
				for(i=0, i2=0; i<i1; i++) {
					if(Siy[i]%2!=0) {
						i2++;											// ����_�}��������
					} else {
						i2 = 0; break;									// �����̑I���͑}��NG
					}
				}
				if(Si1+i2>m_MaxCCtl) {									// <����_����ȏ�>
					AfxMessageBox(IDS_MANY_CTRLP);						// �װү����
					Sphase = 1;
				} else {												// <����_�������>
					if(Si13>0) {										// �����p��Ӱ�ނ̏ꍇ
						for(i=0, i2=0; i<i1; i++) {
							if(Siy[i]>=Si13*2-1) {
								break;									// �����̈�O�͏I��
							}
							if(Siy[i]%2!=0) {
								i2++;									// �����̈�}���Ώ۶���
							} else {
								i2 = 0; break;							// �����̑I���͑}��NG
							}
						}
						Si13 += i2;										// �����̈搧��_�����Z
					}
					PickECurve(i1, Siy, Si5, Sda, Sdb, &Si1, Si2,
                               m_ObjCRpt, m_ObjCWgt, &Si3, Sop2,
                               m_CurInp, Spa, Spb, Sia, Sib, Sic);		// �Ȑ��ҏW �_�I��
					for(i=0; i<Si1; i++) {
						Spy[i] = Spb[i*2];								// ���̕ҏW�㐧��_�擾
					}
					i = (Si13>0) ? Si13*2-1 : Si3;						// �p���͕\�����ύX
					SetParsTracker(0, &Si21, 1, Sic, Spb, Sib, i,
                                   Spx, Six, &Si10);					// �����}�ׯ���ݒ�
					Sphase = 3, Si23 = 1;
				}
			} else if(!m_CtlInp) {										// <�ׯ���w�薳�Œǉ���>
				for(i=0; i<Si1; i++) {
					Sic[i] = 0;											// �S����_�I������
				}
				RedoChgCurve(Si1, Si2, Spa, Sia);						// �Ȑ�����
				TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia, Sic,
                              &Si3, Spb, Sib);							// �Ȑ��ҏW�ׯ���擾
				RedoSetCurve(Si3, Si2, Spb);							// �Ȑ�����_����
				for(i=0; i<Si3; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si1; i++) {
					Spy[i] = Spb[i*2];									// ���̕ҏW�㐧��_�擾
				}
				i = (Si13>0) ? Si13*2-1 : Si3;							// �p���͕\�����ύX
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Sphase = 1, Si21 = 0, Si10 = 0;							// �����}�I������
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==11) {													// <<<DEL(�폜)>>>
		if(Sphase==2||Sphase==3) {
			for(i=0, i1=0; i<Si1; i++) {
				i1 = (Sic[i]) ? i1 + 1 : i1;							// �폜�Ώ۶���
			}
			if(Si1-i1<3) {												// <����_�����ȉ�>
				AfxMessageBox(IDS_FEW_CTRLP);							// �װү����
			} else {													// <����_��������>
				if(Si13>0) {											// �����p��Ӱ�ނ̏ꍇ
					for(i=0, i2=0; i<Si13; i++) {
						i2 = (Sic[i]) ? i2 + 1 : i2;					// �����̈�폜�Ώ۶���
					}
					Si13 -= i2;											// �����̈搧��_�����Z
				}
				DeleteCurvePoint(Si5, Sda, Sdb, Si2, m_ObjCWgt, Sic,
                                 Si1, Spa, Sia, &i1);					// ����_��ʉߓ_�폜
				Si1 = i1;												// ����_���Đݒ�
				RedoChgCurve(Si1, Si2, Spa, Sia);						// �Ȑ�����
				TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia, Sic,
                              &Si3, Spb, Sib);							// �Ȑ��ҏW�ׯ���擾
				RedoSetCurve(Si3, Si2, Spb);							// �Ȑ�����_����
				for(i=0; i<Si3; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si1; i++) {
					Spy[i] = Spb[i*2];									// ���̕ҏW�㐧��_�擾
				}
				i = (Si13>0) ? Si13*2-1 : Si3;							// �p���͕\�����ύX
				Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 1;
			}
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			m_NumInp = (m_NumInp==0&&Wi0==1) ? 1 : m_NumInp;			// ��Ȗʐ��������p��
			Sop1 = NULL, Sop2 = NULL;
			Si10 = 0, Si13 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si5 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
			Si6 = (m_FChar&0x2) ? 3 : 0;								// [F6]:����Ӱ��:��]�Ώ�
			Si6 = (m_FChar&0x8) ? 2 : Si6;								// [F8]:����Ӱ��:2���Ώ�
			Si6 = (m_FChar&0x4) ? 1 : Si6;								// [F7]:����Ӱ��:1���Ώ�
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==1&&cmp!=NULL) {									// <<�Ȑ��̏ꍇ>>
				if(Wi4>0) {
					Si7 = Wi1;											// �Ȗʐ����̌p���ް�
				} else {
					if(Si6==3) {										// ��]�Ώ̕������擾
						Si7 = (InputRevolveDivi(&m_SymRDivi))?m_SymRDivi:0;
					} else if(Si6==1) {
						Si7 = 2;										// 1���Ώ̴ر������(2)
					} else if(Si6==2) {
						Si7 = 4;										// 2���Ώ̴ر������(4)
					} else {
						Si7 = 1;										// �ʏ�ر������(1)
					}
				}
				if(Si7>0) {												// <�ر������OK>
					InitTRLimit();										// �ׯ���͈͌��菉����
					OffEdtOtherObject(Sop1);							// �w��O�ҏW�Ώۉ���
					Si1 = GetParaInt(cmp, 0);							// ����_���擾
					Si2 = GetParaInt(cmp, 1);							// ���EӰ�ގ擾
					m_ObjCRpt = GetParaInt(cmp, 2);						// �Ȑ��������擾
					m_ObjCWgt = (double)GetParaInt(cmp, 3)/1000.0;		// �Ȑ��d�݌W���擾
					for(i=0; i<Si1; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// ײ�Ӱ�ގ擾
						GetParaPnt(cmp, i, &Spa[i]);					// ����_�擾
						Sic[i] = 0;										// �Ώ��׸�:OFF
					}
					SetCurveMatrix3(Si5, m_ObjCWgt, Si2, Si1,
                                    Sia, Sda, Sdb);						// ����_<=>�ʉߓ_�ϊ���د��
					StartMenu();										// �ƭ��J�n����
					InitLevel();										// Undo/Redo������
					ChgCurve(NULL, Si1, Si2, m_ObjCRpt, m_ObjCWgt,
                             Spa, Sia);									// �Ȑ��ύX(����)
					TrackerECurve(Si5, Sda, Si2, Si1, Spa, Sia,
                                  Sic, &Si3, Spb, Sib);					// �Ȑ��ҏW�ׯ���擾
					SetCurve(Si3, Si2, Spb);							// �Ȑ�����_����(����)
					GetObjPtr(&Sop2);									// ����_OP�ۑ�
					SetSelNo(Sop2, 2);									// ���F�\��OP�ݒ�
					BaseCmdPtr(1, Sop1, &cmp);							// �L���擪����ގ擾
					NextCmdPtr(&cmp);									// �Ȑ��ҏW������
					while(cmp!=NULL) {
						RedoCommandCopy(Sop2, cmp);						// ����ޕ��ʎ��s
						NextCmdPtr(&cmp);								// ������ނ�
					}
					SetDispList(Sop2);									// �ި���ڲ�Eؽēo�^
					for(i=0; i<Si3; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					for(i=0; i<Si1; i++) {
						Spy[i] = Spb[i*2];								// ���̕ҏW�㐧��_�擾
					}
					OffUndoRedo();										// Undo/Redo�s��
					SetNumDisp(2);										// ���l�\����������
					if(Si6!=0&&Wi4>0) {									// <�ʏ�Ӱ�ވȊO�Ő����p��>
						Si13 = Wi4;										// ���ʑO����_��
						Si14 = Wi5;										// �n�_�ڎ��׸�
						Si15 = Wi6;										// �I�_�ڎ��׸�
						Si8 = Wi2;										// �Ώ̎��w�蕽��
						Spnt1 = SpntA;									// �Ώ̎����S�_
						i = Wi3;										// �������w��
						Si9 = 0;										// ���������]�Ȃ�
						SetSymmetryLine(1, Si6, Spnt1, Si8, i,
                                        Si7, &Si11, &Si12);				// �Ώ̎��ݒ�
						Sphase = 1;
					}
					Sphase = (Si6==0) ? 1 : Sphase;						// <�ʏ�Ӱ��>
					if(Sphase==1) {
						i = (Si13>0) ? Si13*2-1 : Si3;					// �p���͕\�����ύX
						Tracker1.Setup(Spb, Sib, NULL, i, 4, 0);		// �ׯ��1�ݒ�
					}
					Display(0);											// �ĕ`��
				}
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL2);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
			Wi0 = 0, Wi1 = 0, Wi2 = 0, Wi3 = 0, Wi4 = 0, Wi5 = 0, Wi6 = 0;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// ���̍폜
		}
		if(ms==4) {														// <ϳ��E�t����>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			i = (Si13>0) ? Si13*2-1 : Si3;								// �p���͕\�����ύX
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, i,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(i1>0) {													// <����_�I���̏ꍇ>
				i1 = (Siy[0]%2!=0) ? (Siy[0]+1)/2 : 0;					// �ǉ��ʒu�I���̂ݗL��
			}
			if(i1>0) {													// <<�L���Ȑؒf�ʒu�̏ꍇ>>
				if(AfxMessageBox(IDS_DIVIDE_CTRLP, MB_YESNO)!=IDYES) {	// �m�F:NG
					i1 = 0;
				}
			}
			if(i1>0) {													// <<�L���Ȑؒf�ʒu�̏ꍇ>>
				i2 = 0, i3 = 0;;
				if(Si2) {												// <<���E:�̏ꍇ>>
					for(i=i1; i<Si1; i++) {
						Spb[i2] = Spa[i], Sib[i2++] = Sia[i];			// �ؒf�ʒu�ȍ~�𕡎�
					}
					for(i=0; i<i1; i++) {
						Spb[i2] = Spa[i], Sib[i2++] = Sia[i];			// �A�����Đؒf�ʒu���O�𕡎�
					}
				} else {												// <<���E:�J�̏ꍇ>>
					if(i1<3&&Si1-i1<3) {								// <�ؒf�ʒu�O�オ����_���s��>
						AfxMessageBox(IDS_FEW_CTRLP);					// �װү����
					} else if(i1>=3&&Si1-i1<3) {						// <�ؒf�ʒu�O���L��>
						for(i=0; i<i1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// �ؒf�ʒu���O�𕡎�
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else if(i1<3&&Si1-i1>=3) {						// <�ؒf�ʒu�オ�L��>
						for(i=i1; i<Si1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// �ؒf�ʒu�ȍ~�𕡎�
						}
						AfxMessageBox(IDS_ERR_CTRLNUM);					// �װү����
					} else {											// <�ؒf�ʒu�O�オ�L��>
						for(i=0; i<i1; i++) {
							Spb[i2] = Spa[i], Sib[i2++] = Sia[i];		// �ؒf�ʒu���O�𕡎�
						}
						for(i=i1; i<Si1; i++) {
							Spc[i3] = Spa[i], Sic[i3++] = Sia[i];		// �ؒf�ʒu�ȍ~�𕡎�
						}
					}
				}
				if(i2>0) {												// <Redo:OK>
					RedoChgCurve(i2, 0, Spb, Sib);						// �Ȑ��ҏW(�J)
					GetObjOif(Sop1, &oif);								// ���̍\������߲���擾
					SetOifName(1, oif, OBJ_NAME02);						// ���̍\������(�Ȑ�[�J])
					Si23 = 1;
				}
				if(i3>0) {												// <Create:OK>
					CrtCurve(i3, 0, m_ObjCRpt, m_ObjCWgt, Spc, Sic);	// �Ȑ�����(�J)
					GetObjPtr(&Sop2);									// ����_OP�ۑ�
					BaseCmdPtr(1, Sop1, &cmp);							// �L���擪����ގ擾
					NextCmdPtr(&cmp);									// �Ȑ��ҏW������
					while(cmp!=NULL) {
						RedoCommandCopy(Sop2, cmp);						// ����ޕ��ʎ��s
						NextCmdPtr(&cmp);								// ������ނ�
					}
					SetDispList(Sop2);									// �ި���ڲ�Eؽēo�^
					OnEdtFlg(1, Sop2);									// edtflg:ON
					GetObjOif(Sop2, &oif);								// ���̍\������߲���擾
					SetOifName(0, oif, OBJ_NAME02);						// ���̍\������(�Ȑ�[�J])
					Si23 = 1;
				}
			}
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Line.Reset();													// ײ݉���
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
		if(!Si23) {
			Undo(gno);													// �ҏW�Ȃ���Undo
		}
		Display(0);														// �ĕ`��
	}
}

/*****************************************************************/
void CJcad3GlbView::MouseExtrCurve(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< �Ȑ����o
/*****************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spw, Si6, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// �ׯ����ׯ��1
				if(i1>0) {												// <����_�I���̏ꍇ>
					for(i=0; i<i1; i++) {
						Siw[Siy[i]] = 1;								// �I�����
					}
					SetLineNo2(Si2, Si6, Si7, Si8, Siw);				// ײݔԍ��ݒ�2
					TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                                 &Si6, Spw, Sic);						// �_���ׯ���擾
					for(i=0; i<Si6; i++) {
						Sic[i] = (Siw[i]) ? 4 : Sic[i];					// �w��_�F�ύX
					}
					SetParsTracker(7, &Si21, 0, Siw, Spw, Sic,
                                   Si6, Spx, Six, &Si10);				// �����}�ׯ���ݒ�
					if(i1==1) {											// <<1����_�I��>>
						Display(0);										// �ĕ`��
						if(AfxMessageBox(IDS_OUTPUT_CURVE,
                                         MB_YESNO)==IDYES) {			// <���oOK?>
							if(ExtractionCurve(Si1, Si2, Si3, Si4, Si7,
                                               Si8, Siy[0], Spb, Sia,
                                               &Si11, &Si12, Sps, Sis)){// �Ȑ����o
								CrtCurve(Si11, Si12, m_ObjCRpt,
                                         m_ObjCWgt, Sps, Sis);			// �Ȑ�����
								GetObjPtr(&Sop3);						// �Ȑ�����OP�ۑ�
								GetObjOif(Sop3, &oif);					// ���̍\������߲���擾
								if(Si12) {
									SetOifName(0, oif, OBJ_NAME01);		// ���̍\������(�Ȑ�[��])
								} else {
									SetOifName(0, oif, OBJ_NAME02);		//             (�Ȑ�[�J])
								}
								Si23 = 1;
							}
						}
					}
				}
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {													// <����Ӱ��:�ʏ�>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si6,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(Si22) {
				return;
			}
			for(i=0; i<Si6; i++) {
				Siw[i] = 0;												// �w���׸޸ر
			}
			if(i1>0) {													// <����_�I���̏ꍇ>
				for(i=0; i<i1; i++) {
					Siw[Siy[i]] = 1;									// �I�����
				}
				SetLineNo2(Si2, Si6, Si7, Si8, Siw);					// ײݔԍ��ݒ�2
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				for(i=0; i<Si6; i++) {
					Sic[i] = (Siw[i]) ? 4 : Sic[i];						// �w��_�F�ύX
				}
				SetParsTracker(7, &Si21, 0, Siw, Spw, Sic, Si6,
                               Spx, Six, &Si10);						// �����}�ׯ���ݒ�
				Display(0);												// �ĕ`��
				if(i1==1) {												// <<1����_�I��>>
					if(AfxMessageBox(IDS_OUTPUT_CURVE,
                                     MB_YESNO)==IDYES) {				// <���oOK?>
						if(ExtractionCurve(Si1, Si2, Si3, Si4, Si7,
                                           Si8, Siy[0], Spb, Sia,
                                           &Si11, &Si12, Sps, Sis)) {	// �Ȑ����o
							CrtCurve(Si11, Si12, m_ObjCRpt, m_ObjCWgt,
                                     Sps, Sis);							// �Ȑ�����
							GetObjPtr(&Sop3);							// �Ȑ�����OP�ۑ�
							GetObjOif(Sop3, &oif);						// ���̍\������߲���擾
							if(Si12) {
								SetOifName(0, oif, OBJ_NAME01);			// ���̍\������(�Ȑ�[��])
							} else {
								SetOifName(0, oif, OBJ_NAME02);			//             (�Ȑ�[�J])
							}
							Si23 = 1;
						}
					}
				}
			} else {													// <�ׯ���w�薳>
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ײݔԍ�������
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <�ʏ�Ȗʂ̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// �Ȗʺ���ގ擾1
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				SetSurface(Si1, Si2, Si3, Si4, Spa);					// �Ȗʐ���_����(����)
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				SetSelNo(Sop2, 2);										// ���F�\��OP�ݒ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �ȖʕҏW������
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)!=THICKNESS) {						// <���݂Â��ȊO�̏ꍇ>
						RedoCommandCopy(Sop2, cmp);						// ����ޕ��ʎ��s
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				SetDispList(Sop2);										// �ި���ڲؽēo�^
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// ����_��m�F
				TrackerCLine(0, 1, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				for(i=0; i<Si6; i++) {
					Siw[i] = 0;											// �w���׸޸ر
				}
				OffUndoRedo();											// Undo/Redo�s��
				if(Si24) {												// <�͈͌��肠��>
					if(SetTRLimit(Sop2)) {								// �ׯ���͈͌���ݒ�
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// �ׯ��1�͈͌���
						Tracker2.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// �ׯ��2�͈͌���
					} else {
						Si24 = 0;										// ��ݾق͔͈͌��蒆�~
					}
				} else {												// <�͈͌���Ȃ�>
					InitTRLimit();										// �ׯ���͈͌��菉����
				}
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
				Display(0);												// �ĕ`��
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Si20) {
			AttachObj(Sop1);											// ����ލĐڑ�/�Ď��s
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// ���̍폜
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Line.Reset();													// ײ݉���
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		InitLineNo(NULL);												// ײݔԍ�������
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
		if(!Si23) {
			Undo(gno);													// �ҏW�Ȃ���Undo
		}
		Display(0);														// �ĕ`��
	}
}

/***************************************/
void CJcad3GlbView::MenuCentCurve(void)									//<<< ���̒��S�_�Ȑ�����
/***************************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i1, i2, i3=0, i4=0, i5=0, i6=0, i7=0;
	CString  str;

	if(GetSelectNo()==0) {												// <<2D�Ȑ�OP��>>
		Si1 = (m_FChar&0x1) ? 1 : 0;									// [F5]:���S�_Z�ŏ�
		Si2 = (m_FChar&0x2) ? 1 : 0;									// [F6]:���S�_Z�ő�
		Si3 = (m_FChar&0x4) ? 1 : 0;									// [F7]:�����I��
		Si4 = (m_FChar&0x8) ? 1 : 0;									// [F8]:�Ȑ�
		Si5 = (m_FChar&0x10) ? 0 : 1;									// [F9]:����_
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			i3 = (GetObjAtr(op)/10==1) ? 1 : i3;						// �Ȑ��ʹװ
			if(i3==1) {
				break;													// �װ�͒��~
			}
			if(GetObjPrt(op)!=2) {										// <�Ư�:�q�ȊO�̏ꍇ>
				Sop1 = op;												// ����OP�ۑ�
				i4++;													// ���̐�����UP
			}
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(i3==0&&i4==1) {												// <<�L���ȏꍇ>>
			if(Si3==1) {												// <�����I��>
				AfxMessageBox(IDS_FEW_CTRLP);							// �װү����
			} else {													// <�����p��>
				GetObjBox(Sop1, &box);									// ����BOX�擾
				BoxCenter(box, &pnt);									// BOX�����擾
				if(Si1==1) {
					pnt.p[2] = box.bp[0].p[2];							// ���S�_Z�ŏ��w��
				} else if(Si2==1) {
					pnt.p[2] = box.bp[1].p[2];							// ���S�_Z�ő�w��
				}
				Spa[0] = pnt;											// ���S�_�ۑ�
				Sia[0] = 0;												// �Ȑ��_
				Si6 = 1;												// �ۑ���
				SetSelNo(Sop1, 1);										// ���̓o�^
				OffEdtAllObject();										// �ҏW���̑S����
				CountEdtObj();											// �ҏW�Ώۗ��̶���
				Si7 = 1;												// ���S�_�o�^��
			}
		} else {														// <�װ�̏ꍇ>
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
	} else if(Si7==1) {													// <<2D�Ȑ�OP�L�œo�^��>>
		Si1 = (m_FChar&0x1) ? 1 : 0;									// [F5]:���S�_Z�ŏ�
		Si2 = (m_FChar&0x2) ? 1 : 0;									// [F6]:���S�_Z�ő�
		Si3 = (m_FChar&0x4) ? 1 : 0;									// [F7]:�����I��
		Si4 = (m_FChar&0x8) ? 1 : Si4;									// [F8]:�Ȑ�
		Si5 = (m_FChar&0x10) ? 0 : Si5;									// [F9]:����_
		BaseObjPtr(1, &op, &i1, &i2);									// �\���Ώۗ��̎擾
		while(op!=NULL) {
			if(GetSelNo(op)==1) {										// <<�o�^�ςݗ���>>
				i3 = (GetObjAtr(op)/10==1) ? 1 : i3;					// �Ȑ��ʹװ
				i4++;													// �o�^�ϗ��̐�����UP
			} else if(i2==1) {											// <<�ҏW�Ώۗ���>>
				i7++;
				i5 = (GetObjAtr(op)/10==1) ? 1 : i5;					// �Ȑ��ʹװ
				if(i5==1) {
					break;												// �װ�͒��~
				}
				if(GetObjPrt(op)!=2) {									// <�Ư�:�q�ȊO�̏ꍇ>
					Sop1 = op;											// ����OP�ۑ�
					i6++;												// ���̐�����UP
				}
			}
			NextObjPtr(1, &op, &i1, &i2);								// ���\�����̂�
		}
		if(i3==1||Si6!=i4) {											// <�o�^�ςɈُ�̏ꍇ>
			Si6 = 0, Si7 = 0;											// �ް�ؾ��
			ResetSelNo();												// ���̑I��ؾ��
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		} else if(i5==0&&i6==1) {										// <�L���ȏꍇ>
			GetObjBox(Sop1, &box);										// ����BOX�擾
			BoxCenter(box, &pnt);										// BOX�����擾
			if(Si1==1) {
				pnt.p[2] = box.bp[0].p[2];								// ���S�_Z�ŏ��w��
			} else if(Si2==1) {
				pnt.p[2] = box.bp[1].p[2];								// ���S�_Z�ő�w��
			}
			if(Si6>m_MaxCCtl) {											// <����_����ȏ�>
				AfxMessageBox(IDS_MANY_CTRLP);							// �װү����
			} else {
				Spa[Si6] = pnt;											// ���S�_�ۑ�
				Sia[Si6++] = 0;											// �Ȑ��_
				SetSelNo(Sop1, 1);										// ���̓o�^
			}
			OffEdtAllObject();											// �ҏW���̑S����
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		} else if(i7==0) {												// <�o�^�ςݗ��̂̂�>
			Si3 = 1;													// �����I��
		} else {														// <����̂ݴװ�̏ꍇ>
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
		if(Si3==1) {													// <<<�����I��>>>
			str.Format(IDS_CURVE_CONFIRM, Si6);
			if(AfxMessageBox(str, MB_YESNO)==IDYES) {					// <<����OK?>>
				if(Si6>2) {												// <�����\�ȓ_��>
					OffEdtAllObject();									// �ҏW���̑S����
					Display(0);											// �ĕ`��
					if(Si4==0) {										// �J�Ȑ���
						Sia[0] = 1, Sia[Si6-1] = 1;						// �n�_/�I�_�͐ܐ��_
					}
					StartMenu();										// �ƭ��J�n����
					InitLevel();										// Undo/Redo������
					SetCurveMatrix1(Si5, m_CurveWeight, Si4, Si6,
                                    Sia, Sda, Sdb);						// �ʉߓ_=>����_�ϊ���د��
					CalcCurvePoint(Si5, Si6, Spa, Spb, Sdb);			// �ʉߓ_=>����_�ϊ�
					CrtCurve(Si6, Si4, m_CurveRepeat, m_CurveWeight,
                             Spb, Sia);									// �Ȑ�����
					GetObjPtr(&Sop1);									// �Ȑ�����OP�ۑ�
					GetObjOif(Sop1, &oif);								// ���̍\������߲���擾
					if(Si4==1) {
						SetOifName(0, oif, OBJ_NAME01);					// ���̍\������(�Ȑ�[��])
					} else {
						SetOifName(0, oif, OBJ_NAME02);					//             (�Ȑ�[�J])
					}
					OnEdtFlg(1, Sop1);									// �Ȑ�edtflg:ON
					CountEdtObj();										// �ҏW�Ώۗ��̶���
					EndJMenu1();										// �ƭ��I������
					SetDataUse();										// �ް��g�p���ݒ�
					ResetSelNo();										// ���̑I��ؾ��
				} else {												// <����_�s��>
					Si6 = 0, Si7 = 0;									// �ް�ؾ��
					ResetSelNo();										// ���̑I��ؾ��
					AfxMessageBox(IDS_FEW_CTRLP);						// �װү����
				}
			}
		}
	}
	ExitMenu(0, 0);														// �ƭ��I��
	Display(0);															// �ĕ`��
}
