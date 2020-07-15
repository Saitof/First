/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewJewelry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputBtnDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "RCommand.h"
#include "Select.h"
#include "CreatCtrlPoint1.h"
#include "EditJewelry.h"
#include "InputBtnDlg2.h"
#include "EditObject.h"
#include "NumFunc.h"

/*************************************/
void CJcad3GlbView::MenuJwlRing(void)									//<<< �ݸ޻��ލ��킹
/*************************************/
{
	OBJTYPE* op;
	PNTTYPE  cp, pa[2];
	BOXTYPE  box;
	int      kind, mode, i, i1, i2;
	double   size=0.0, space, rr, th, sy, wk, mat[3][3];

	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	if(op!=NULL) {														// <�ҏW���̗L�̏ꍇ>
		m_RingKind = (m_FChar&0x1) ? 0 : m_RingKind;					// [F5]:�����w��
		m_RingKind = (m_FChar&0x2) ? 1 : m_RingKind;					// [F6]:���a�w��
		mode = (m_FChar&0x8) ? 1 : 0;									// [F8]:�ڍ��Ԋu�p�x�w��
		kind = m_RingKind;
		size = (!kind) ? m_RingSize1 : m_RingSize2;						// ����(����/���a)
		space = (!mode) ? m_RingSpace1 : m_RingSpace2;					// �ڍ��Ԋu(mm/�x)
		if(KeyboardJwlRing(kind, mode, &size, &space)) {				// ����,�ڍ��Ԋu�̓���
			if(!kind) {
				m_RingSize1 = size;
			} else {
				m_RingSize2 = size;
			}
			if(!mode) {
				m_RingSpace1 = space;
			} else {
				m_RingSpace2 = space;
			}
		} else {
			size = 0.0, space = 0.0;
		}
	}
	if(size>0.0) {														// <<���ގw��̏ꍇ>>
		if((m_GlbDisp==1||m_GlbDisp==2)&&m_GlbChange) {					// <GLB1/GLB2�����Ұ��X�V����>
			if(ReadGLB()!=TRUE) {										// GLB�ē���
				AfxMessageBox(IDS_CANT_READ_FILE);						// ��ް�̧�ٓǍ�NG�
				DeleteGLB();											// GLB�j��
				m_GlbDisp = 0;
			}
		}
		if(m_DspTmpNo&&m_GlbChange) {									// <����ڰĕ\�������Ұ��X�V����>
			if(ReadTEMP()!=TRUE) {										// ����ڰē���
				AfxMessageBox(IDS_CANT_READ_FILE);						// ��ް�̧�ق�Ǎ�NG�
				DeleteTEMP();											// ����ڰĔj��
				m_DspTmpNo = 0;
			}
		}
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {												// <�ҏW���̗L�̏ꍇ>
			RedoRingBeforeCommand(op);									// �ݸ޻��ޑO����ލĎ��s
			NextObjPtr(2, &op, &i1, &i2);								// �����̂�
		}
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		for(i=0; i<DIM; i++) {
			cp.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;				// �ޯ�����S�擾
		}
		rr = (!kind) ? 15.0+(size-7.0)/3.0 : size;						// �ݸޒ��a�v�Z
		th = (!mode) ? asin(space/rr) : space*PI/360.0;					// �ڍ��Ԋu���̊p�x
		th = (th<-10||th>10) ? 0.0 : th;
		sy = (rr*PI)/(box.bp[1].p[1]-box.bp[0].p[1]);
		sy = (1.0-th/PI)*sy;											// �ڍ��Ԋu�����Z
		th = 360.0-180.0/PI*th*2.0;										// �~���Ȃ��̊p�x
		pa[0].p[0] = 0.0, pa[1].p[0] = 0.0;								// �~���Ȃ���_�ݒ�
		pa[0].p[1] = box.bp[0].p[1], pa[1].p[1] = box.bp[1].p[1];
		pa[0].p[2] = box.bp[0].p[2], pa[1].p[2] = box.bp[0].p[2];
		wk = (int)(sy*1000.0)/1000.0;									// 1/1000�P�ʂɕϊ�
		mat[0][0] = 1.0, mat[0][1] = 0.0, mat[0][2] = 0.0;				// ��د���ݒ�
		mat[1][0] = 0.0, mat[1][1] = wk,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
		AffineVtx(2, pa, cp, mat);										// ���_���W�̨ݕϊ�
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		Ring(cp, 1.0, sy, 1.0, 1, 2, th, pa[0], pa[1]);					// �ݸ޻��ލ��킹
		EndJMenu1();													// �ƭ��I������
		Display(0);														// �ĕ`��
		SetDataUse();													// �ް��g�p���ݒ�
	}
}

/****************************************************************/
void CJcad3GlbView::MouseJwlStone(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< �Θg����
/****************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp=NULL;
	PNTTYPE  pnt, cpt, mip, map;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, i4, ht, es=0, col=0, endf=0, ok=0, ng=0;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}�̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			if(Sphase==2) {												// �����ȊO�͓�������
				CalcScreenWorld(0, &pnt, point, plane);					// ���W�_�擾
				if(flg&m_LimKey) {
					LimitPoint(plane, &pnt, &Spnt1);					// ϳ����͐������W
				}
				if(!Si15) {												// <���ޕύX>
					if(Si16==-1) {
						es = (pnt.p[0]>=Sbase.p[0]) ? 1 : es;
					} else if(Si16==1) {
						es = (pnt.p[0]<=Sbase.p[0]) ? 1 : es;
					}
					if(Si17==-1) {
						es = (pnt.p[1]>=Sbase.p[1]) ? 1 : es;
					} else if(Si17==1) {
						es = (pnt.p[1]<=Sbase.p[1]) ? 1 : es;
					}
				} else {												// <�l���ύX>
					if(!Si17) {											// �ύX�\�͈͎擾
						if(Si16==0||Si16==1) {
							i1 = 0, i2 = 1;
						} else {
							i1 = 3, i2 = 2;
						}
					} else {
						if(Si16==0||Si16==3) {
							i1 = 0, i2 = 3;
						} else {
							i1 = 1, i2 = 2;
						}
					}
					for(i=0; i<DIM; i++) {
						cpt.p[i] = (Spc[i1].p[i]+Spc[i2].p[i])/2.0;		// ���ԓ_�擾
					}
					if(!Si17) {
						if(Si16==0||Si16==3) {
							mip = Spc[Si16], map = cpt;
						} else {
							mip = cpt, map = Spc[Si16];
						}
					} else {
						if(Si16==0||Si16==1) {
							mip = Spc[Si16], map = cpt;
						} else {
							mip = cpt, map = Spc[Si16];
						}
					}
					es = (pnt.p[Si17]<=mip.p[Si17]) ? 1 : es;
					es = (pnt.p[Si17]>=map.p[Si17]) ? 1 : es;
				}
				if(es==0) {												// <<�װ���̏ꍇ>>
					if(Si5==2) {										// <�����̏ꍇ>
						if(!Si15) {										// �ύX��̻���
							Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
							Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
						} else {										// �l���̕ύX
							if(!Si17) {
								Sd5 = fabs(pnt.p[0]-Spc[Si16].p[0]);
							} else {
								Sd6 = fabs(pnt.p[1]-Spc[Si16].p[1]);
							}
						}
					} else {											// <�����ȊO�̏ꍇ>
						if(Si5==0) {									// �ύX��̻���
							if(!Si18) {
								Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
								Sd4 = fabs(pnt.p[0]-Sbase.p[0]);
							} else {
								Sd3 = fabs(pnt.p[1]-Sbase.p[1]);
								Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
							}
						} else {
							Sd3 = fabs(pnt.p[0]-Sbase.p[0]);
							Sd4 = fabs(pnt.p[1]-Sbase.p[1]);
						}
					}
					Si2 = 0;
					for(i=0; i<Si1; i++) {								// <�f�ʕʂɌJ��Ԃ�>
						i1 = Sis[i];									// �f�ʕʐ���_��
						for(j=0; j<i1; j++) {
							Spa[j] = Spw[Si2+j];						// �f�ʐ���_���o
							Sia[j] = Siw[Si2+j];						// �f�ʋȐ�Ӱ�ޒ��o
						}
						Si2 += i1;										// ���v�f�ʐ���_��
						if(Si5==2) {									// <�����̏ꍇ>
							SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                                        i1, Spa, Sia, Spb, Sib);		// �����Θg����
						} else {										// <�����ȊO�̏ꍇ>
							RoundStone(Sbase, Sd7, Sd3, Sd4, Si3, i1,
                                       Spa, Sia, Spb, Sib);				// ׳��ސΘg����
						}
						RedoCrtSurface2(i, Si3, i1, 1, 1, Spb, Sib);	// �ȖʕύX
					}
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1]);
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spc, Si14))!=-1) {	// �ׯ��1����
				Spnt1 = Spc[ht];										// �ύX��_�ۑ�
				if(ht<4) {												// <<���ޕύX�̏ꍇ>>
					Si15 = 0;											// ���ޕύXӰ��
					Si16 = (fabs(Spnt1.p[0]-Sbase.p[0])<EPSILON) ? 0	// ���S�_�ƍX�V�_�ʒu�֌W
                                      : ((Spnt1.p[0]<Sbase.p[0])?-1:1);
					Si17 = (fabs(Spnt1.p[1]-Sbase.p[1])<EPSILON) ? 0
                                      : ((Spnt1.p[1]<Sbase.p[1])?-1:1);
					Si18 = (Si5==0) ? ht%2 : Si18;						// �ύX����
				} else {												// <<�l���ύX�̏ꍇ>>
					Si15 = 1;											// �l���ύXӰ��
					Si16 = (ht-4)/2;									// �ҏW���ۑ�
					Si17 = (ht==5||ht==6||ht==9||ht==10) ? 0 : 1;
				}
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Display(0);												// �ĕ`��
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6, &Si14,
                     Spc, Sic, Tens);									// �Θg�ׯ���쐬
		Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);						// �ׯ��1�ݒ�
		Display(0);														// �ĕ`��
		Sphase = 1;
	} else if(ms==11) {													// <<<DEL>>>
		if(Si5==2) {													// <�����̏ꍇ>
			Sd5 = 0.0, Sd6 = 0.0;
			Si2 = 0;
			for(i=0; i<Si1; i++) {										// <�f�ʕʂɌJ��Ԃ�>
				i1 = Sis[i];											// �f�ʕʐ���_��
				for(j=0; j<i1; j++) {
					Spa[j] = Spw[Si2+j];								// �f�ʐ���_���o
					Sia[j] = Siw[Si2+j];								// �f�ʋȐ�Ӱ�ޒ��o
				}
				Si2 += i1;												// ���v�f�ʐ���_��
				SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                            i1, Spa, Sia, Spb, Sib);					// �����Θg����
				RedoCrtSurface2(i, Si3, i1, 1, 1, Spb, Sib);			// �ȖʕύX
			}
			TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6,
                         &Si14, Spc, Sic, Tens);						// �Θg�ׯ���쐬
			Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);					// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
			Sphase = 1;
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Soif = NULL, Sop2 = NULL;
			Si1 = 0, Si2 = 0, Si3 = 0;
			Sd5 = 0.0, Sd6 = 0.0, Sd7 = DBL_MAX;
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si19 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�����޲�۸ޖ�
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			while(Sop1!=NULL) {
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
				GetObjBox(Sop1, &box);									// �ޯ�����ގ擾
				if(i3==10&&cmp!=NULL) {									// �Ȑ�(��)�͐���
					ok = 1;
				} else {												// �Ȑ�(�J)�ʹװ
					ok = 0, ng = 1;
					break;
				}
				if(fabs(box.bp[0].p[0]-box.bp[1].p[0])>EPSILON) {		// YZ���ʈȊO�ʹװ
					ok = 0, ng = 1;
					break;
				}
				if(Sd7>box.bp[0].p[1]) {
					Sd7 = box.bp[0].p[1];								// Y�ŏ��ʒu����
					Sop2 = Sop1;										// Y�ŏ��Ȑ�OP�ۑ�
				}
				if(++Si1>5) {											// �f�ʋȐ����̐���6�ȏ�ʹװ
					ok = 0, ng = 1;
					break;
				}
				NextObjPtr(2, &Sop1, &i1, &i2);							// ���ҏW���̂�
			}
			if(ok) {													// <<�L���ް��̏ꍇ>>
				if(m_Char=='R') {										// R��������׳���
					Si5 = 0;
				} else if(m_Char=='O') {								// O�������͵����
					Si5 = 1;
				} else if(m_Char=='S') {								// S�������ͽ���
					Si5 = 2;
				} else {
					ok = (!KeyboardStoneForm(&Si5)) ? 0 : ok;			// �Θg�`��̓���
				}
			}
			if(ok) {													// <<�L���ް��̏ꍇ>>
				if(m_NumInp!=0) {										// <���l���͎w��̏ꍇ>
					Si6 = 1;
					if(Si5==0) {										// �Θg���a�̓���
						if(KeyboardRoundSize(&m_JwlRndSize)) {
							Sd3 = m_JwlRndSize, Sd4 = m_JwlRndSize;
						} else {
							ok = 0;
						}
					} else {											// �Θg�c�����ނ̓���
						if(KeyboardSquareSize(&m_JwlSqrSize1,
                                              &m_JwlSqrSize2)) {
							Sd3 = m_JwlSqrSize1, Sd4 = m_JwlSqrSize2;
						} else {
							ok = 0;
						}
					}
					ok = (fabs(Sd3)<EPSILON) ? 0 : ok;					// ��ݾق̏ꍇ
				} else {												// <ϳ����͎w��̏ꍇ>
					Si6 = 0;
					Sd3 = m_Fovy/2000.0, Sd4 = m_Fovy/2000.0;			// ��ʂ̒����̔����擾
				}
				Sd3 /= 2.0, Sd4 /= 2.0;									// ���a�Z�o
			}
			if(ok&&Si5==0) {											// <׳��ނ̏ꍇ>
				if(!KeyboardProng(&m_ProngNum, &m_ProngLn,
                                  &m_ProngDm, &m_ProngRv)) {			// �܏��̓���
					ok = 0;
				}
			}
			if(ok) {													// <<�L���ް��̏ꍇ>>
				if(Si5==2) {
					Si3 = 8;											// �����̕�����
				} else {
					if(!Si19) {											// �޲�۸ޏȗ����̏ꍇ
						CInputDlg dlg(11);
						dlg.SetValue(m_RevolveDivi);					// �����l�\��
						if(dlg.DoModal()==IDOK) {						// �޲�۸ޕ\��
							dlg.GetValue(&m_RevolveDivi);				// ��]�������擾
							Si3 = m_RevolveDivi;
						} else {										// ��ݾق͒��~
							ok = 0;
						}
					} else {											// �޲�۸ޏȗ���8
						Si3 = 8;
					}
				}
			}
			if(ok) {													// <<�L���ް��̏ꍇ>>
				ScrToPnt(0, &m_Cent3D, &Sbase);							// ��ʂ̒��S�擾
				Si1 = 0;
				if(Sop2!=NULL) {										// <�ő啝����(�擪:�e)>
					BaseCmdPtr(1, Sop2, &cmp);							// �L���擪����ގ擾
					i3 = GetParaInt(cmp, 0);							// ����_���擾
					i4 = GetParaInt(cmp, 1);							// ���EӰ�ގ擾
					for(i=0; i<i3; i++) {
						Sia[i] = GetParaInt(cmp, 4+i);					// ײ�Ӱ�ގ擾
						GetParaPnt(cmp, i, &Spa[i]);					// �Ȑ�����_�擾
					}
					SetCurve(i3, i4, Spa);								// �Ȑ�����_����
					GetObjPtr(&Sop3);									// ����_OP�̕ۑ�
					BaseCmdPtr(1, Sop2, &cmp);							// �L���擪����ގ擾
					NextCmdPtr(&cmp);									// �Ȑ��ҏW��̺����
					while(cmp!=NULL) {
						RedoCommandCopy(Sop3, cmp);						// ����ނ̕��ʎ��s
						NextCmdPtr(&cmp);								// ������ނ�
					}
					for(i=0; i<i3; i++) {
						GetVtx(Sop3, i, &Spa[i]);						// ���̕ҏW�㐧��_�擾
					}
					FreeObj(Sop3, 1);									// �ҏW�㗧�̂̍폜
					if(i3*Si3>m_MaxSCtl) {								// ����_������ް�ʹװ
						ok = 0, ng = 2;
					}
					for(i=0; i<i3; i++) {
						Spw[Si2+i] = Spa[i];							// �f�ʐ���_�ۑ�(�ݐ�)
						Siw[Si2+i] = Sia[i];							// �f�ʋȐ�Ӱ�ޕۑ�(�ݐ�)
					}
					Si2 += i3;											// ���v�f�ʐ���_��
					Sis[Si1] = i3;										// �f�ʕʐ���_��
					Si1++;												// �f�ʐ����Z
				}
				BaseObjPtr(2, &Sop1, &i1, &i2);							// �ҏW�Ώۗ��̎擾
				while(Sop1!=NULL) {
					if(Sop1!=Sop2) {									// <�ő啝����(�e)�ȊO>
						BaseCmdPtr(1, Sop1, &cmp);						// �L���擪����ގ擾
						i3 = GetParaInt(cmp, 0);						// ����_���擾
						i4 = GetParaInt(cmp, 1);						// ���EӰ�ގ擾
						for(i=0; i<i3; i++) {
							Sia[i] = GetParaInt(cmp, 4+i);				// ײ�Ӱ�ގ擾
							GetParaPnt(cmp, i, &Spa[i]);				// �Ȑ�����_�擾
						}
						SetCurve(i3, i4, Spa);							// �Ȑ�����_����
						GetObjPtr(&Sop3);								// ����_OP�̕ۑ�
						BaseCmdPtr(1, Sop1, &cmp);						// �L���擪����ގ擾
						NextCmdPtr(&cmp);								// �Ȑ��ҏW��̺����
						while(cmp!=NULL) {
							RedoCommandCopy(Sop3, cmp);					// ����ނ̕��ʎ��s
							NextCmdPtr(&cmp);							// ������ނ�
						}
						for(i=0; i<i3; i++) {
							GetVtx(Sop3, i, &Spa[i]);					// ���̕ҏW�㐧��_�擾
						}
						FreeObj(Sop3, 1);								// �ҏW�㗧�̂̍폜
						if(i3*Si3>m_MaxSCtl) {							// ����_������ް�ʹװ
							ok = 0, ng = 2;
							break;
						}
						for(i=0; i<i3; i++) {
							Spw[Si2+i] = Spa[i];						// �f�ʐ���_�ۑ�(�ݐ�)
							Siw[Si2+i] = Sia[i];						// �f�ʋȐ�Ӱ�ޕۑ�(�ݐ�)
						}
						Si2 += i3;										// ���v�f�ʐ���_��
						Sis[Si1] = i3;									// �f�ʕʐ���_��
						Si1++;											// �f�ʐ����Z
					}
					NextObjPtr(2, &Sop1, &i1, &i2);						// ���ҏW���̂�
				}
			}
			if(ok) {													// <<�L���ް��̏ꍇ>>
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				CreatOif(0, NULL, OBJ_NAME00, NULL, &Soif);				// ���̍\�����쐬
				SetOifName(0, Soif, OBJ_NAME36);						// �\����(�Θg)
				Delete();												// 2D�}�`�폜
				Si2 = 0;
				for(i=0; i<Si1; i++) {									// <�f�ʕʂɌJ��Ԃ�>
					i1 = Sis[i];										// �f�ʕʐ���_��
					for(j=0; j<i1; j++) {
						Spa[j] = Spw[Si2+j];							// �f�ʐ���_���o
						Sia[j] = Siw[Si2+j];							// �f�ʋȐ�Ӱ�ޒ��o
					}
					InitCreat(1, 1, i1, Spa, Sia, &i2);					// ������������
					for(j=0; j<i1; j++) {
						Spw[Si2+j] = Spa[j];							// �f�ʐ���_�Đݒ�
						Siw[Si2+j] = Sia[j];							// �f�ʋȐ�Ӱ�ލĐݒ�
					}
					Si2 += i1;											// ���v�f�ʐ���_��
					if(Si5==2) {										// <�����̏ꍇ>
						SquareStone(Sbase, Sd7, Sd3, Sd4, Sd5, Sd6,
                                    i1, Spa, Sia, Spb, Sib);			// �����Θg����
					} else {											// <�����ȊO�̏ꍇ>
						RoundStone(Sbase, Sd7, Sd3, Sd4, Si3, i1,
                                   Spa, Sia, Spb, Sib);					// ׳��ސΘg����
					}
					CrtSurface(Si3, i1, 1, 1, m_CurveRepeat,
                               m_CurveWeight, 1, Spb, Sib);				// �Ȗʐ���(����)
					GetObjPtr(&Sop1);									// �Ȗ�OP�̕ۑ�
					GetObjOif(Sop1, &oif);								// ���̍\������߲���擾
					SetOifLevel(oif, 1);								// �\������(1)
					SetOifName(0, oif, OBJ_NAME30);						// �\����(�΍�)
					OnEdtFlg(1, Sop1);									// �Ȗʂ�edtflg:ON
					if(m_ProngNum>0||Si1>0) {							// <��/�����f�ʂ�����ꍇ>
						if(i==0) {										// �ő啝(�擪)�̐΍���
							PutObjPrt(Sop1, 1, 1);						// �߰�No�ݒ�(�e)
						} else {										// �ȊO�̐΍���
							PutObjPrt(Sop1, 2, 1);						// �߰�No�ݒ�(�q)
						}
					}
				}
				CountEdtObj();											// �ҏW�Ώۗ��̶���
				if((!Si6)||Si5==2) {									// <ϳ�����/����>
					OffUndoRedo();										// Undo/Redo�s��
					TrackerStone(Si5, Sbase, Sd3, Sd4, Sd5, Sd6,
                                 &Si14, Spc, Sic, Tens);				// �Θg�ׯ���쐬
					Tracker1.Setup(Spc, Sic, Tens, Si14, 0, 7);			// �ׯ��1�ݒ�
					SetNumDisp(2);										// ���l�\����������
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1]);
					Sphase = 1;
				} else {												// <���ް�ޓ���>
					endf = 1;											// �I���׸�ON
				}
				Display(0);												// �ĕ`��
			} else {													// <�ҏW����:NG>
				if(ng==1) {
					AfxMessageBox(IDS_ERR_SEL7);						// �װү����
				} else if(ng==2) {
					AfxMessageBox(IDS_MANY_CTRLP);
				}
				ExitMenu(0, 1);											// �ƭ��I��
				Sop1 = NULL;
				Sop2 = NULL;
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E����/�I��>>>
		endf = 1;														// �I���׸�ON
	}
	if(endf&&Si5==0&&m_ProngNum>0) {									// <�I���ŒܗL�̏ꍇ>
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		SetProng(m_ProngNum, m_ProngLn, m_ProngDm, m_ProngRv,
                 box, Si2, Spw);										// �Θg�ܐݒ�
		CountEdtObj();													// �ҏW�Ώۗ��̶���
	}
	if(endf) {
		if(Soif!=NULL) {
			SelectOifItem(Soif);										// ���̍\�����ёI��
		}
		Tracker1.Reset(1);												// �ׯ��1����
		OnUndoRedo();													// Undo/Redo��
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
	}
}

/******************************************************************/
void CJcad3GlbView::MouseJwlArrange(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< �Θg�����z�u
/******************************************************************/
{
	PNTTYPE pnt;
	int     i, ht, col=0;
	double  th, d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			if(Sphase==3) {												// <XY�ړ�>
				CalcScreenWorld(0, &pnt, point, plane);					// ���W�_�擾
				Sd3 = PntD(&Spnt1, &pnt);								// �ׯ���ԋ���
				d1 = Sd3 - Sd2;											// ������Ƃ̍�
				ArrangeMove(d1, Si6, Si1, Sia, Sib, Spa, Spb, Sva);		// �Θg�ړ�
			} else if(Sphase==4) {										// <������]>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				th = atan2(pnt.p[Si4]-Spnt1.p[Si4],
                           pnt.p[Si3]-Spnt1.p[Si3]);
				th = (th<-10||th>10) ? 0.0 : th;
				Sd4 = th*180.0/PI;										// ��]�p�x
				ArrangeRotate1(Sd4, Si6, Si1, Sia, Sib, Spb);			// �Θg������]
			} else if(Sphase==5) {										// <������]>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				d1 = pnt.p[Si5]-Spnt3.p[Si5];
				d2 = m_Fovy/4000.0;
				Sd5 = 180.0*d1/d2;										// ��]�p�x
				ArrangeRotate2(Sd5, Si6, Si1, Sia, Sib, Spb, Svb);		// �Θg������]
			}
			m_pNumDisp->SetParam(Sd3, Sd4, Sd5);
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==2) {
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((ht=Tracker1.HitTracker(1, Si3, pnt, Spy, 3))!=-1) {		// �ׯ��1����
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = (ht==1) ? 3 : ((ht==2)?4:Sphase);
			} else if(Tracker2.HitTracker(1, Si4, pnt, Spz, 1)!=-1) {	// �ׯ��2����
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 5;
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <��]���ʒ��S�_>
			Si3 = plane;												// ���W��1(��������)
			Si4 = (Si3+1>2) ? 0 : Si3+1;								// ���W��2(��������)
			Si5 = (Si4+1>2) ? 0 : Si4+1;								// ���W��3
			CalcScreenWorld(0, &Spnt1, point, plane);					// ��]���S�_���W�擾
			InitLevel();												// Undo/Redo������
			InitArrange(Si3, Si1, Sd1, Spnt1 ,&Si6, Sia, Sib,
                        Spa, Sva, Svb);									// �Θg�z�u��������
			for(i=0; i<Si1; i++) {
				Spb[i] = Spa[i];										// �Θg���S�_����
			}
			Spnt1.p[Si5] = Spa[0].p[Si5];								// ���S�_��Si5���W�␳
			Spnt2 = Spa[0];												// ���a���ߗp�ׯ���ʒu
			Sd2 = PntD(&Spnt1, &Spnt2);									// �ׯ���ԋ���(�)
			AutoZoom();													// 3�ʐ}�̎����{��
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si3, 2);			// �ׯ��1�ݒ�
				Display(0);												// �ĕ`��
				CInputBtnDlg4 dlg;
				dlg.SetParam(Si3, Si1, Si6, Sd2, Sia, Sib,
                             Spa, Spb, Sva, Svb, 
                             m_ArangAdd1, m_ArangAdd2, m_ArangAdd3);	// ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_ArangAdd1, &m_ArangAdd2, &m_ArangAdd3);	// ���Ұ��擾
				Tracker1.Reset(0);										// �ׯ��1����
				OnUndoRedo();											// Undo/Redo��
				ExitMenu(0, 1);											// �ƭ��I��
				EndJMenu1();											// �ƭ��I������
			} else {													// <ϳ����͂̏ꍇ>
				pnt = Spnt1;											// ������]�p�ׯ��
				pnt.p[Si3] += m_Fovy/14000.0;
				Spy[0] = Spnt1, Spy[1] = Spnt2, Spy[2] = pnt;
				Siy[0] = 0, Siy[1] = 1, Siy[2] = 2;
				Tens[0] = 0, Tens[1] = 7, Tens[2] = 8;
				Tracker1.Setup(Spy, Siy, Tens, 3, Si3, 1);				// �ׯ��1�ݒ�
				Spnt3 = Spnt1;											// ������]�p�ׯ��
				Spnt3.p[Si5] = Spnt2.p[Si5];
				Spz[0] = Spnt3, Siz[0] = 0, TensC[0] = 6;
				Tracker2.Setup(Spz, Siz, TensC, 1, Si4, 7);				// �ׯ��2�ݒ�
				Sd3 = Sd2, Sd4 = 0.0, Sd5 = 0.0;						// �����a,�p�x�ݒ�
				m_pNumDisp->SetParam(Sd3, 0, 0);
				Sphase = 2;
			}
		} else if(Sphase>=2) {											// <�ҏW�I��>
			if(Sphase==3) {
				for(i=0; i<DIM; i++) {
					Spy[1].p[i] = Spnt1.p[i]+Sva[0].p[i]*Sd3;
				}
			} else if(Sphase==4) {
				th = PI*Sd4/180.0;
				Spy[2].p[Si3] = Spnt1.p[Si3]+(m_Fovy/14000.0)*cos(th);
				Spy[2].p[Si4] = Spnt1.p[Si4]+(m_Fovy/14000.0)*sin(th);
			} else if(Sphase==5) {
				d1 = m_Fovy/4000.0;
				d2 = d1*Sd5/180.0;
				Spz[0].p[Si5] = Spnt3.p[Si5]+d2;
			}
			Tracker1.Setup(Spy, Siy, Tens, 3, Si3, 1);					// �ׯ��1�ݒ�
			Tracker2.Setup(Spz, Siz, TensC, 1, Si4, 7);					// �ׯ��2�ݒ�
			Sphase = 2;
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si1 = (KeyboardArrange(&m_ArrangeTh, &m_ArrangeNum)) ?
                                                  m_ArrangeNum : 0;		// �z�u������
			if(Si1>0) {													// <OK>
				Sd1 = (fabs((m_ArrangeTh)-360.0)<EPSILON) ?
                             360.0/Si1 : m_ArrangeTh/(Si1-1);			// 1������̊p�x
				OffUndoRedo();											// Undo/Redo�s��
				SetNumDisp(2);											// ���l�\����������
				m_pNumDisp->SetParam(0.0, 0.0, 0.0);
				Sphase = 1;
				StartMenu();											// �ƭ��J�n����
			} else {													// <��ݾ�>
				ExitMenu(0, 1);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase!=0) {
			EndArrange(Si6);											// �Θg�z�u�I������
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*************************************/
void CJcad3GlbView::MenuSetUnit(void)									//<<< �Ưĥ��޼ު��
/*************************************/
{
	OBJTYPE* op, * wop=NULL, * sop=NULL;
	BOXTYPE  box;
	int      dsp, edt;
	double   d1, d2, mx=0.0;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		PutObjPrt(op, 0, 1);											// �߰�No����
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	if(!(m_FChar&0x1)) {												// <[F5]:�����ȊO�̏ꍇ>
		ResetSelNo();													// �I��Noؾ��
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {
			SetSelNo(op, 1);											// �I��No�ݒ�
			GetObjBox(op, &box);										// �ޯ���擾
			d1 = box.bp[1].p[0]-box.bp[0].p[0];
			d2 = box.bp[1].p[1]-box.bp[0].p[1];
			d1 = (d1<d2) ? d2 : d1;										// XY�ő啝�擾
			if(d1>mx) {
				mx = d1, wop = op;										// �΍�(�ő啝)���̕ۑ�
			}
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
		if(wop==NULL) {
			return;														// ���̖��͏������~
		}
		OffEdtAllObject();												// �ҏW���̑S����
		SetSelNo(wop, 0);												// �I��No����
		OnEdtFlg(1, wop);												// ����edtflg:ON
		Copy();															// ���̕���
		GetObjPtr(&sop);												// �����߲���擾
		SetSelNo(sop, 2);												// �I��No�ݒ�
		SetDispList(sop);												// �ި���ڲ�ؽēo�^
		Delete();														// �����̍폜
		while(TRUE) {
			wop = NULL;
			SelBaseObjPtr(1, &op);										// �擪���̎擾
			while(op!=NULL) {
				if(GetObjPrt(op)==0) {
					wop = op; break;									// ���Ưė��̂�����
				}
				SelNextObjPtr(1, &op);									// �����̂�
			}
			if(wop==NULL) {												// ���Ưė��̖��I��
				break;
			} else {
				SetSelNo(wop, 0);										// �I��No����
				OnEdtFlg(1, wop);										// ����edtflg:ON
				Copy();													// ���̕���
				GetObjPtr(&op);											// �����߲���擾
				SetSelNo(op, 2);										// �I��No�ݒ�
				SetDispList(op);										// �ި���ڲ�ؽēo�^
				Delete();												// �����̍폜
			}
		}
		SelBaseObjPtr(2, &op);											// �擪���̎擾
		while(op!=NULL) {
			if(op==sop) {
				PutObjPrt(op, 1, 1);									// �߰�No�ݒ�(�e)
			} else {
				PutObjPrt(op, 2, 1);									// �߰�No�ݒ�(�q)
			}
			OnEdtFlg(1, op);											// ����edtflg:ON
			SelNextObjPtr(2, &op);										// �����̂�
		}
		ResetSelNo();													// ���̑I��ؾ��
		CountEdtObj();													// �ҏW�Ώۗ��̶���
	}
}

/*************************************************************/
void CJcad3GlbView::MouseEarth(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ���
/*************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt1, pt2;
	BOXTYPE  box;
	int      hit, i, i1, i2;
	double   th, l11, l12, l21, l22;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1&&plane==Si5&&(flg&MK_LBUTTON)) {					// ϳ����t�������̂ݗL��
			if(Si8==0) {												// <�ړ�����>
				CalcScreenWorld(0, &pnt, point, plane);					// ���W�_�擾
				for(i=0; i<DIM; i++) {
					Sdb[i] = pnt.p[i]-Spnt1.p[i];						// ���̈ړ��ʎZ�o
				}
				if(flg&m_LimKey) {
					LimitValue(plane, Sdb);								// ϳ����͐����l
				}
				Sdb[Si7] = 0.0;											// �w�蕽�ʊO�̍��W��0
			} else if(Si8==1) {											// <��]����>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				l11 = Spnt1.p[Si5]-Sbase.p[Si5];						// �p�x�Z�o
				l12 = Spnt1.p[Si6]-Sbase.p[Si6];
				l21 = pnt.p[Si5]-Sbase.p[Si5];
				l22 = pnt.p[Si6]-Sbase.p[Si6];
				l11 = atan2(l12, l11);
				l11 = (l11<-10||l11>10) ? 0.0 : l11;
				l11 = l11*180.0/PI;
				l21 = atan2(l22, l21);
				l21 = (l21<-10||l21>10) ? 0.0 : l21;
				l21 = l21*180.0/PI;
				th = -l21+l11;
				Sd2 = (th-Sd1<-180.0) ? Sd2+360 :
                      ((th-Sd1>180.0) ? Sd2-360 : Sd2);					// �A����]�␳�l�C��
				Sd1 = th;												// �����]�p�x�ۑ�
				th += Sd2;												// �␳�l���Z
				Sdb[Si7+3] = th;
			} else {													// <���ޕύX����>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				pt1 = Spnt1, pt1.p[Si7] = 0.0;							// �w�蕽�ʈȊO�̍��W��0
				pt2 = Sbase, pt2.p[Si7] = 0.0;
				pnt.p[Si7] = 0.0;
				l11 = PntD(&pt1, &pt2);									// ���S�_���ׯ���ʒu�܂ł̋���
				l12 = PntD(&pnt, &pt2);									// ���S�_�ƶ��وʒu�܂ł̋���
				l21 = (l12/l11) - 1.0;									// �������̔{��
				Sdb[6] = l21, Sdb[7] = l21, Sdb[8] = l21;
			}
			ObjectEarth(Si1, Si4, Si3, Si2, 0, Sda[0]+Sdb[0],
                        Sda[1]+Sdb[1], Sda[2]+Sdb[2], Sda[3]+Sdb[3],
                        Sda[4]+Sdb[4], Sda[5]+Sdb[5], Sda[6]+Sdb[6],
                        Sda[7]+Sdb[7], Sda[8]+Sdb[8]);					// ���
			Si1 = 1;
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 8))!=-1) {	// �ׯ��1����
				Si5 = plane;											// ���ʕۑ�
				Si6 = (Si5+1>2) ? 0 : Si5+1;							// ���W���ݒ�
				Si7 = (Si6+1>2) ? 0 : Si6+1;
				Si8 = -1;												// ��������
				if(hit==0) {											// <�ړ�>
					Si8 = 0;											// �ړ�����
					Spnt1 = Sbase;
				} else if(hit==7) {										// <��]>
					Si8 = 1;											// ��]����
					Spnt1 = Spa[7];
				} else {												// <���ޕύX>
					i = (hit-1)/2;										// ��No�擾
					if(i!=Si7) {										// <�L�����ׯ��>
						Si8 = 2;										// ���ޕύX����
						Spnt1 = Spa[hit];
					}
				}
				if(Si8!=-1) {											// <�ׯ���I��OK>
					Tracker1.Reset(0);									// �ׯ��1����
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
					Display(0);											// �ĕ`��
					Sphase = 1, Sd1 = 0.0, Sd2 = 0.0;
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		for(i=0; i<9; i++) {
			Sda[i] += Sdb[i], Sdb[i] = 0.0;								// �����Z�l�����Z
		}
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &Sbase);											// �ޯ�������擾
		TrackerBoxCrs2(box, Sbase, Spa);								// �ׯ���擾
		Spb[0]=Spa[0], Spb[1]=Spa[1], Spb[2]=Spa[2], Spb[3]=Spa[3];
		Spb[4]=Spa[4], Spb[5]=Spa[7];
		Tracker1.Setup(Spb, Sia, Tens, 6, 0, 7);						// �ׯ��1�ݒ�
		Spb[0]=Spa[0], Spb[1]=Spa[3], Spb[2]=Spa[4], Spb[3]=Spa[5];
		Spb[4]=Spa[6], Spb[5]=Spa[7];
		Tracker2.Setup(Spb, Sia, Tens, 6, 1, 7);						// �ׯ��2�ݒ�
		Spb[0]=Spa[0], Spb[1]=Spa[5], Spb[2]=Spa[6], Spb[3]=Spa[1];
		Spb[4]=Spa[2], Spb[5]=Spa[7];
		Tracker3.Setup(Spb, Sia, Tens, 6, 2, 7);						// �ׯ��3�ݒ�
		Sphase = 0;
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si1 = 0;													// ����Ӱ��
			Si2 = (m_FChar&0x1) ? 0 : 1;								// [F5]:��_-Z�ŏ��w��
			Si2 = (m_FChar&0x2) ? 2 : Si2;								// [F6]:��_-Z�ő�w��
			Si3 = (m_FChar&0x4) ? 0 : 1;								// [F7]:��_-���̒��S�w��
			Si3 = (m_FChar&0x8) ? 2 : Si3;								// [F8]:��_-�ر���S�w��
			Si4 = (m_FChar&0x4) ? 0 : 1;								// [F7]:���S�_-���̒��S�w��
			Si4 = (m_FChar&0x8) ? 2 : Si4;								// [F8]:���S�_-�ر���S�w��
		}
		for(i=0; i<9; i++) {
			Sia[i] = 0, Sda[i] = 0.0, Sdb[i] = 0.0;
		}
		BaseObjPtr(2, &Sop1, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(Sop1!=NULL) {
			EditCmdPtr(2, Sop1, &cmp);									// ���������߲���擾
			if(cmp!=NULL) {												// <�������ޗL>
				for(i=0; i<9; i++) {
					Sia[i] = GetParaInt(cmp, i);						// �������Ұ��擾
				}
				break;
			}
			NextObjPtr(2, &Sop1, &i1, &i2);								// �����̂�
		}
		i1 = (Sia[6]>Sia[7]) ? Sia[6] : Sia[7];							// �ݒ�ς̍ő�{�����g�p
		i1 = (Sia[8]>i1) ? Sia[8] : i1;
		i1 = (i1==0) ? 1000 : i1;										// 0�̏ꍇ��1000(1.0�{)��
		Sia[6] = i1, Sia[7] = i1, Sia[8] = i1;
		for(i=0; i<9; i++) {
			Sda[i] = (double)Sia[i]/1000.0;
		}
		if(m_NumInp!=0) {												// <���l���͂̏ꍇ>
			CInputBtnDlg7 dlg(2);
			dlg.SetParam(Sda[0], Sda[1], Sda[2], Sda[3], Sda[4],
                         Sda[5], Sda[6], m_EarthAdd1, m_EarthAdd2,
                         m_EarthAdd3, m_ScaleDir, m_ScalePnt, Si3, Si4);// ���Ұ��ݒ�
			dlg.DoModal();												// �����޲�۸ޕ\��
			dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2, &m_EarthAdd3,
                         &m_ScaleDir, &m_ScalePnt);						// ���Ұ��擾
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else {														// <ϳ����͂̏ꍇ>
			OffUndoRedo();												// Undo/Redo�s��
			Sia[0]=0, Sia[1]=1, Sia[2]=1, Sia[3]=1, Sia[4]=1, Sia[5]=2;	// �ׯ���F�ݒ�
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &Sbase);										// �ޯ�������擾
			TrackerBoxCrs2(box, Sbase, Spa);							// �ׯ���擾
			Tens[0]=7, Tens[1]=5, Tens[2]=5, Tens[3]=6, Tens[4]=6, Tens[5]=8;
			Spb[0]=Spa[0], Spb[1]=Spa[1], Spb[2]=Spa[2], Spb[3]=Spa[3];
			Spb[4]=Spa[4], Spb[5]=Spa[7];
			Tracker1.Setup(Spb, Sia, Tens, 6, 0, 7);					// �ׯ��1�ݒ�
			Spb[0]=Spa[0], Spb[1]=Spa[3], Spb[2]=Spa[4], Spb[3]=Spa[5];
			Spb[4]=Spa[6], Spb[5]=Spa[7];
			Tracker2.Setup(Spb, Sia, Tens, 6, 1, 7);					// �ׯ��2�ݒ�
			Spb[0]=Spa[0], Spb[1]=Spa[5], Spb[2]=Spa[6], Spb[3]=Spa[1];
			Spb[4]=Spa[2], Spb[5]=Spa[7];
			Tracker3.Setup(Spb, Sia, Tens, 6, 2, 7);					// �ׯ��3�ݒ�
		}
		Display(0);														// �ĕ`��
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}
