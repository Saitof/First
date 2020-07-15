/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewEditSurf2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlTracker.h"
#include "FaceEdit.h"
#include "Symmetry.h"
#include "ConvSCurve.h"

/******************************************************************/
void CJcad3GlbView::MouseSPointMove(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< �f�ʐ���_�ړ�
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

	if(Sphase==1&&(ms==2||ms==3)&&Si22) {								// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	} else if(Sphase!=1&&ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {
		EyeChange(flg, point, plane, ms);
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase>0&&Sphase<3&&plane!=3) {				// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==3) {											// ����_�ړ�
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			GetMoveValue(plane, Sbase, &pnt);							// �ׯ���ړ��ʎ擾
			MoveSymmetryArea(Sbase, pnt, Si15, Spnt4, 0, Si16, Si17, -1,
                             -1, Si21, 0, Si2, Six, Spz, Spy);			// �Ώ̗̈�ړ��ʐݒ�
			MoveSCurve(Si14, Sdb, Spy, Sop3, Si2, Si4, Spz, Spw,
                       Six, Spx, TensC);								// �f�ʐ���_�ړ�
			RedoCrtCurve(Si2, Si4, Spx, Siz);							// �Ȑ�����
			CalcCurvePoint(Si14, Si2, Spx, Spd, Sda);					// ����_=>�ʉߓ_�ϊ�
			RedoSetCurve(Si2, Si4, Spd);								// �Ȑ�����_����
			for(i=0; i<Si2; i++) {
				Siw[i] = (Six[i])?4:Siz[i]*2;							// �ׯ���F�ݒ�
			}
			Tracker1.Setup(Spd, Siw, NULL, Si2, Si11, 0);				// �ׯ��1�ݒ�
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// �ړ��O�_�Ƃ̋���
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Si9 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0&&Si16>-1) {										// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);			// �ׯ��1�ݒ�
				Sphase = 2;												// ����_�w���
			}
		} else if(Sphase<3) {											// <��ׯ�ޑI���̏ꍇ>
			i1 = 0;
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt, i1 = 1;
			}
		}
		if(Sphase==1&&i1==1) {											// <�f�ʔԍ��w��>
			DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);			// �ׯ����ׯ��2
			if(i1>0) {													// �f�ʑI������
				Si23 = 1;
				Si15 = (m_FChar&0x8) ? 2 : 0;							// [F8]:����Ӱ��:2���Ώ�
				Si15 = (m_FChar&0x4) ? 1 : Si15;						// [F7]:����Ӱ��:1���Ώ�
				m_FChar = m_FChar&~(0x4);								// [F7]OFF
				m_FChar = m_FChar&~(0x8);								// [F8]OFF
				Si10 = Siy[0];											// U�f�ʈʒu�ۑ�
				PickSPointMove1(Si10, Si14, Sda, Sdb, Si12, Si19, Si1,
                                Si2, Si4, m_ObjCRpt, m_ObjCWgt, Si7,
                                Si13, &Si11, Spb, Sps, Spz, Spw, &Spnt1,
                                &Svec1, &Spnt3, Sia, Sis, Siz, Siw, Six,
                                Tens, TensC, Sop1, Sop2, &Sop3, &Sop4);	// �f�ʐ���_�ړ� ��I��
				if(Si15==0) {											// <�ʏ�Ӱ�ނ̏ꍇ>
					Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);		// �ׯ��1�ݒ�
					Sphase = 2;											// ����_�w���
				} else {												// <�Ώ�Ӱ�ނ̏ꍇ>
					Tracker1.Reset(0);									// �ׯ��1����
					Sphase = 0, Si16 = -1;								// �Ώ̎��w���
				}
			}
		} else if(Sphase==2&&plane==Si11&&i1==1) {						// <�f�ʐ���_�w��>
			DragCtrlTracker2(plane, box, Spw, Si2, &i1, Siy);			// �ׯ����ׯ��2
			if(i1>0) {													// ����_�I������
				PickSPointMove2(i1, Siy, Si14, Sda, Sdb, Si2, Si4,
                                m_ObjCWgt, m_CurInp, Spz, Spw,
                                Siz, Siw, Six);							// �f�ʐ���_�ړ� �_�I��
				Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);			// �ׯ��1�ݒ�
			}
		} else if(Sphase==3&&Si9==1) {
			for(i=0; i<Si2; i++) {
				Spz[i] = Spx[i], Spw[i] = Spd[i];						// �����ޯ̧�ύX
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
		Sphase = (Sphase==3) ? 2 : Sphase;
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				CalcScreenWorld(0, &Spnt4, point, plane);				// �Ώ̎����S�_�擾
				i1 = plane;												// �Ώ̎��w�蕽��
				i2 = (flg&MK_CONTROL) ? 1 : 0;							// �������w��
				Si21 = (flg&MK_SHIFT) ? 1 : 0;							// ���������]
				SetSymmetryLine(1, Si15, Spnt4, i1, i2, 0,
                                &Si16, &Si17);							// �Ώ̎��ݒ�
			}
		} else {														// <����_�w��̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(Sphase==1) {												// <<�f�ʔԍ��w��>>
				HitCtrlTracker2(flg, point, plane, ms, pnt, Spw,
                                Si8, 0, &Si22, &i1, Siy);				// �S�ׯ������2
				if(Si22) {
					return;
				}
				if(i1>0) {												// <�f���ׯ���w�肠��>
					Si23 = 1;
					Si15 = (m_FChar&0x8) ? 2 : 0;						// [F8]:����Ӱ��:2���Ώ�
					Si15 = (m_FChar&0x4) ? 1 : Si15;					// [F7]:����Ӱ��:1���Ώ�
					m_FChar = m_FChar&~(0x4);							// [F7]OFF
					m_FChar = m_FChar&~(0x8);							// [F8]OFF
					Si10 = Siy[0];										// U�f�ʈʒu�ۑ�
					PickSPointMove1(Si10, Si14, Sda, Sdb, Si12, Si19,
                                    Si1, Si2, Si4, m_ObjCRpt, m_ObjCWgt,
                                    Si7, Si13, &Si11, Spb, Sps, Spz,
                                    Spw, &Spnt1, &Svec1, &Spnt3, Sia,
                                    Sis, Siz, Siw, Six, Tens, TensC,
                                    Sop1, Sop2, &Sop3, &Sop4);			// �f�ʐ���_�ړ� ��I��
					if(Si15==0) {										// <�ʏ�Ӱ�ނ̏ꍇ>
						Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);	// �ׯ��1�ݒ�
						Sphase = 2;										// ����_�w���
					} else {											// <�Ώ�Ӱ�ނ̏ꍇ>
						Tracker1.Reset(0);								// �ׯ��1����
						Sphase = 0, Si16 = -1;							// �Ώ̎��w���
					}
				}
			} else if(Sphase==2&&plane==Si11) {							// <<�f�ʐ���_�w��>>
				i1 = Tracker1.HitTracker(1, Si11, pnt, Spw, Si2);		// �ׯ��1����
				if(i1!=-1) {
					if(!m_CurInp&&!m_CtlInp&&Six[i1]==1) {				// <����_�ړ��̏ꍇ>
						Sbase = Spw[i1];								// ��_�ۑ�
						Sphase = 3;
						Si9 = 0;
						if(m_NumInp!=0) {								// <���l���͂̏ꍇ>
							Sphase = 2;
							CInputBtnDlg6 dlg(4);
							dlg.SetParam4(Sop3, Si14, Si2, Si4, Si11,
                                          &Si9, Si15, Si16, Si17, Si21,
                                          Sbase, Spnt4, Spz, Spx, Spw,
                                          Spd, Spy, Siz, Six, Siw, TensC,
                                          Sda, Sdb, m_SMoveAdd);        // ���Ұ��ݒ�4
							if(dlg.DoModal()==IDOK) {					// �����޲�۸ޕ\��
								if(Si9==1) {							// �ړ��ς݂̏ꍇ��
									for(i=0; i<Si2; i++) {
										Spz[i] = Spx[i];				// �����ޯ̧�ύX
										Spw[i] = Spd[i];
									}
								}
							}
							dlg.GetParam(&m_SMoveAdd);					// ���Ұ��擾
							RedoCrtCurve(Si2, Si4, Spz, Siz);			// �Ȑ�����
							CalcCurvePoint(Si14, Si2, Spz, Spw, Sda);	// ����_=>�ʉߓ_�ϊ�
							RedoSetCurve(Si2, Si4, Spw);				// �Ȑ�����_����
							for(i=0; i<Si2; i++) {
								Siw[i] = (Six[i])?4:Siz[i]*2;			// �ׯ���F�ݒ�
							}
							Tracker1.Setup(Spw, Siw, NULL, Si2, Si11,0);// �ׯ��1�ݒ�
						}
					} else {											// <����_�I���̏ꍇ>
						if(!m_CtlInp){
							for(i=0; i<Si2; i++) {
								Six[i] = 0;								// �S����_�I������
							}
						}
						Siy[0] = i1;
						PickSPointMove2(1, Siy, Si14, Sda, Sdb, Si2,
                                        Si4, m_ObjCWgt, m_CurInp, Spz,
                                        Spw, Siz, Siw, Six);			// �f�ʐ���_�ړ� �_�I��
						Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);	// �ׯ��1�ݒ�
					}
				} else if(!m_CtlInp) {									// <<�ׯ���I��>>
					for(i=0; i<Si2; i++) {
						Siw[i] = Siz[i]*2, Six[i] = 0;					// �ׯ��/�w���׸ސݒ�
					}
					Tracker1.Setup(Spw, Siw, NULL, Si2, Si11, 0);		// �ׯ��1�ݒ�
				}
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL, Sop3 = NULL, Sop4 = NULL;
			Scmp = NULL;
			Si6 = 0, Si7 = 0, Si16 = -1, Si18 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si19 = (m_FChar&0x4) ? 1 : 0;								// [F7]:�f�ʎ����̏���
			Si14 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
			m_FChar = m_FChar&~(0x4);									// [F7]OFF
			m_FChar = m_FChar&~(0x8);									// [F8]OFF
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==2&&cmp!=NULL) {									// <�Ȗʂ̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				if(Si19) {
					EraseObjSPointMove(&Si18, Sic, Sop1);				// �����̏���
				}
				GetSurfCmd2(cmp, i3, &Si12, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si13, Sps, Sis);							// �Ȗʺ���ގ擾2
				if(!Si12&&Si14) {										// �|���Ȗ�&�ʉߓ_Ӱ�ނ�
					AfxMessageBox(IDS_ERR_SWEEP);						// ү����
					Si14 = 0;											// �|���Ȗʂ͐���_Ӱ��
				}
				if(!Si12&&!Si20) {										// �|���Ȗ�&������:OFF��
					AfxMessageBox(IDS_SWEEPSURF);						// ү����
					Si20 = 1;											// �������`��:ON
				}
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				if(Si12) {												// <�ʏ�Ȗ�>
					ExecSurface(Si14, Sop1, Si1, Si2, Si3, Si4,
                                m_ObjCRpt, m_ObjCWgt, Si5, Spa, Sia,
                                Spb, &Sop2);							// �ʏ�Ȗʎ��s
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);	// ����_��m�F
				} else {												// <�|���Ȗ�>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si13, Sps, Sis, Spa,
                                  Spb, &Sop2);							// �|���Ȗʎ��s
				}
				BaseCmdPtr(1, Sop1, &Scmp);								// �L���擪����ގ擾
				TrackerULine(Si14, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// �_���ׯ���擾
				if(Si24) {												// <�͈͌��肠��>
					if(SetTRLimit(Sop2)) {								// �ׯ���͈͌���ݒ�
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// �ׯ��1�͈͌���
					} else {
						Si24 = 0;										// ��ݾق͔͈͌��蒆�~
					}
				} else {												// <�͈͌���Ȃ�>
					InitTRLimit();										// �ׯ���͈͌��菉����
				}
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// �ׯ��1�ݒ�
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				OffUndoRedo();											// Undo/Redo�s��
				SetNumDisp(2);											// ���l�\����������
				Display(0);												// �ĕ`��
				Sphase = 1;
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==2) {													// <<�f�ʋȐ��ҏW��>>
			ChangeSPointMove(Si12, Si14, Si19, Si20, Si10, Si1, Si2,
                             Si3, Si4, m_ObjCWgt, Si5, Si7, Si13,
                             Spnt1, Svec1, Spnt3, Spa,Spb, Spc, Sps,
                             Spz, Sia, Sis, Siz, Tens, Scmp, Sop1,
                             Sop2, Sop3, Sop4);							// �f�ʐ���_�ړ� �f�ʕύX
			TrackerULine(Si14, Si1, Si2, Spb, Si6, Si7, &Si8, Spw, Siw);// �_���ׯ���擾
			Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);					// �ׯ��1�ݒ�
			Line.Reset();												// ײ݉���
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			Display(0);													// �ĕ`��
			Sphase = 1, i3 = 1;
		} else {
			i3 = 0;
		}
		if(i3==0||ms==9) {												// <<��L�ȊO/�����I��>>
			if(Si20) {
				AttachObj(Sop1);										// ����ލĐڑ�/�Ď��s
			}
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// ���̍폜
			}
			if(Sop3!=NULL) {
				FreeObj(Sop3, 1);										// ���̍폜
			}
			if(Sop4!=NULL) {
				FreeObj(Sop4, 1);										// ���̍폜
			}
			Tracker1.Reset(1);											// �ׯ��1����
			SetRenderMode(0);											// ܲ԰�ڰ�Ӱ��
			ResetSelNo();												// ���̑I��Noؾ��
			OnUndoRedo();												// Undo/Redo��
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
			SetDataUse();												// �ް��g�p���ݒ�
			if(Si19) {
				DispObjSPointMove(Si18, Sic, Sop1);						// �����̕\��
			}
			if(!Si23) {
				Undo(gno);												// �ҏW�Ȃ���Undo
			}
			Display(0);													// �ĕ`��
		}
	}
}

/******************************************************************/
void CJcad3GlbView::MouseBPointMove(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< �ް����ړ�
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	VECTYPE  vec;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   dd;

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
		} else if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Spnt1);						// ϳ����͐������W
			}
			GetMoveValue(plane, Spnt1, &pnt);							// �ׯ���ړ��ʎ擾
			MoveSymmetryArea(Spnt1, pnt, Si12, Spnt2, 0, Si15, Si16,
                             -1, -1, Si14, 0, Si8, Sib, Spz, Sps);		// �Ώ̗̈�ړ��ʐݒ�
			MoveBPoint(Si11, Spy, Sps, Sop1, Si1, Si2, Si3,
                       Si4, m_ObjCWgt, Si7, Si8, Sia,
                       Sib, Spa, Spb, Spc, Tens);						// �ް����ړ�
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ���
			CalcSurfacePoint(Si11, 1, 0, Si1, Si2, Si3, Si4, m_ObjCWgt,
                             Sia, Spc, Spd);							// ����_=>�ʉߓ_�ϊ�
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			TrackerULine(Si11, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);// �_���ׯ���擾
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// �w��_�F�ύX
			}
			DispParsTracker(0, Si21, Spw, Siw, Si8, Spx, Six, Si10);	// �����}�ׯ���\��
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// �ړ��O�_�Ƃ̋���
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Si9 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
			}
		} else if(Sphase==1) {											// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spw, Si8, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// �ׯ����ׯ��1
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickBPointMove(i1, Siy, Si7, Si8, Sop2, Sib, Siw);	// �ް����ړ� ��I��
					SetParsTracker(0, &Si21, 0, Sib, Spw, Siw, Si8,
                                   Spx, Six, &Si10);					// �����}�ׯ���ݒ�
					Sphase = 1, Si23 = 1;
				}
			}
		} else if(Sphase==2) {
			if(Si9==1) {												// <����_�ړ��̏ꍇ>
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// �����ޯ̧�ύX
				}
				for(i=0; i<Si8; i++) {
					Spz[i] = Spw[i];									// �ׯ���ۑ�
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// ��_�擾
				}
			}
			Sphase = 1;
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				CalcScreenWorld(0, &Spnt2, point, plane);				// �Ώ̎����S�_�擾
				Si13 = plane;											// �Ώ̎��w�蕽��
				i = (flg&MK_CONTROL) ? 1 : 0;							// �������w��
				Si14 = (flg&MK_SHIFT) ? 1 : 0;							// ���������]
				SetSymmetryLine(1, Si12, Spnt2, Si13, i, 0,
                                &Si15, &Si16);							// �Ώ̎��ݒ�
			}
		} else {														// <����_�w��̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si8, Spx,
                            Six, Si10, &Si21, &Si22, &i1, Siy);			// �S�ׯ������1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <����_�ړ��̏ꍇ>
				Spnt1 = Spw[Siy[0]];									// �ړ����_�ۑ�
				Sphase = 2, Si9 = 0;
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Sphase = 1;
					CInputBtnDlg6 dlg(5);
					dlg.SetParam5(Sop1, Sop2, Si11, Si21, Si1, Si2, Si3,
                                  Si4, m_ObjCWgt, Si5, Si6, Si7, Si8,
                                  Si10, &Si9, Si12, Si15, Si16, Si14,
                                  Spnt1, Spnt2, Spy, Spa, Spb, Spc, Spd,
                                  Sps, Spw, Spx, Spz, Sia, Sib, Siw,
                                  Six, Tens, m_BMoveAdd);				// ���Ұ��ݒ�5
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						if(Si9==1) {									// �ړ��ς݂̏ꍇ��
							for(i=0; i<Si5; i++) {						// �����ޯ̧�ύX
								Spa[i] = Spc[i], Spb[i] = Spd[i];
								
							}
							for(i=0; i<Si1; i++) {
								GetBasePoint(Si1, Si2, Spb, i,
                                             &Spy[i], &vec);			// ��_�擾
							}
						}
					}
					dlg.GetParam(&m_BMoveAdd);							// ���Ұ��擾
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// �Ȗʐ���
					CalcSurfacePoint(Si11, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// ����_=>�ʉߓ_�ϊ�
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// �Ȗʐ���_����
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					TrackerULine(Si11, Si1, Si2, Spb, Si6, Si7,
                                 &Si8, Spw, Siw);						// �_���ׯ���擾
					for(i=0; i<Si8; i++) {
						Spz[i] = Spw[i];								// �ׯ���ۑ�
						Siw[i] = (Sib[i]) ? 4 : Siw[i];					// �w��_�F�ύX
					}
					DispParsTracker(0, Si21, Spw, Siw, Si8,
                                    Spx, Six, Si10);					// �����}�ׯ���\��
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
				}
			} else if(i1>0) {											// <����_�I���̏ꍇ>
				if(!m_CtlInp) {
					for(i=0;i<Si8;i++) {
						Sib[i] = 0, Siw[i] = 2;							// �w���׸޸ر
					}
				}
				PickBPointMove(i1, Siy, Si7, Si8, Sop2, Sib, Siw);		// �ް����ړ� ��I��
				SetParsTracker(0, &Si21, 0, Sib, Spw, Siw, Si8,
                               Spx, Six, &Si10);						// �����}�ׯ���ݒ�
				Si23 = 1;
			} else if(!m_CtlInp) {										// <�ׯ���w�薳�Œǉ���>
				for(i=0; i<Si8; i++) {
					Sib[i] = 0, Siw[i] = 2;								// �w���׸޸ر
				}
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ײݔԍ�������
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si12 = (m_FChar&0x8) ? 2 : 0;								// [F8]:����Ӱ��:2���Ώ�
			Si12 = (m_FChar&0x4) ? 1 : Si12;							// [F7]:����Ӱ��:1���Ώ�
			Si11 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
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
				ExecSurface(Si11, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// ����_��m�F
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// ��_�擾
				}
				TrackerULine(Si11, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// �_���ׯ���擾
				for(i=0; i<Si8; i++) {
					Spz[i] = Spw[i], Sib[i] = 0;						// �ׯ���ۑ�/�w���׸޸ر
				}
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
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
				SetNumDisp(2);											// ���l�\����������
				if(Si12==0) {											// <�ʏ�Ӱ�ނ̏ꍇ>
					Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 0);			// �ׯ��1�ݒ�
					Sphase = 1;											// ����_�w���
				}
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
		SetRenderMode(0);												// ܲ԰�ڰ�Ӱ��
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

/******************************************************************/
void CJcad3GlbView::MouseLineAdjust(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< ����_�Ȑ����킹
/******************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	SCRTYPE  scr;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, ct1, ct2, er, un, vn, ub, vb;

	if((ms==2||ms==3)&&Si22) {											// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&plane!=3) {									// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(abs(BasePoint.x-point.x)>=DRAGMIN||
           abs(BasePoint.y-point.y)>=DRAGMIN) {
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			box.bp[0] = BaseCursor, box.bp[1] = pnt;
			DragCtrlTracker1(plane, box, Spx, Si6, Spy, Siy,
                             Si7, &Si21, &i1, Siz);						// �ׯ����ׯ��1
			if(i1>0) {													// <����_�I���̏ꍇ>
				for(i=0; i<i1; i++) {
					i2 = Siz[i], pnt = Spx[i2];							// �I���ׯ��No/���W
					Six[i2] = (Six[i2]==4) ? 0 : 4;						// �I���󋵔��]
					for(j=0; j<Si6; j++) {								// <�w��_������W>
						if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                           fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                           fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
							Six[j] = (Six[j]==4) ? 0 : 4;				// �I���󋵔��]
						}
					}
				}
				SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                               Spy, Siy, &Si7);							// �����}�ׯ���ݒ�
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker1(flg, point, plane, ms, pnt, Spx, Si6,
                        Spy, Siy, Si7, &Si21, &Si22, &i1, Siz);			// �S�ׯ������1
		if(Si22) {
			return;
		}
		if(i1>0) {														// <����_�I���̏ꍇ>
			if(!m_CtlInp) {
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// �S����_�I������
				}
			}
			for(i=0; i<i1; i++) {
				i2 = Siz[i], pnt = Spx[i2];								// �I���ׯ��No/���W
				Six[i2] = (Six[i2]==4) ? 0 : 4;							// �I���󋵔��]
				for(j=0; j<Si6; j++) {									// <�w��_������W>
					if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                       fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                       fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
						Six[j] = (Six[j]==4) ? 0 : 4;					// �I���󋵔��]
					}
				}
			}
			SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                           Spy, Siy, &Si7);								// �����}�ׯ���ݒ�
		} else if(!m_CtlInp) {											// <�ׯ���w�薳�Œǉ���>
			for(i=0; i<Si6; i++) {
				Six[i] = 0;												// �S����_�I������
			}
			Si21 = 0, Si7 = 0;											// �����}�I������
			Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);					// �ׯ��1�ݒ�
			Tracker2.Reset(0);											// �ׯ��2����
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(GetSelectNo()==0) {											// <<2D�Ȑ�OP��>>
			if(m_FChar&0x1) {											// [F5]:��������(X)
				Si1 = 0;
			} else if(m_FChar&0x2) {									// [F6]:   �V   (Y)
				Si1 = 1;
			} else if(m_FChar&0x4) {									// [F7]:   �V   (Z)
				Si1 = 2;
			} else {													// ���w��(Z)
				Si1 = 2;
			}
			Si2 = (m_FChar&0x8) ? 0 : 1;								// [F8]:�폜�Ȃ�
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			ct1 = 0, er = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			while(op!=NULL) {
				if(GetObjAtr(op)/10!=1) {								// �Ȑ��ȊO�ʹװ
					er = 1; break;
				}
				op1 = op;												// �Ȑ�OP�ۑ�
				if(++ct1>1) {											// �Ȑ�������UP
					er = 1; break;
				}
				NextObjPtr(2, &op, &i1, &i2);							// ���ҏW���̂�
			}
			if(er==0) {													// <����ȏꍇ>
				GetObjBox(op1, &box);									// BOX���ގ擾
				for(i=0, i1=0; i<DIM; i++) {
					scr.p[i] = 0;
					if(fabs(box.bp[0].p[i]-box.bp[1].p[i])>EPSILON) {	// �L�����ޕ�������
						scr.p[i] = 1;
						i1++;
					}
				}
				if(i1==2) {												// <�L�����ޕ�����2>
					if(scr.p[Si1]==0) {									// ���������������ʹװ
						er = 3;
					} else {
						for(i=0; i<DIM; i++) {
							Si3 = (i!=Si1&&scr.p[i]==1) ? i : Si3;		// �ʒu�m�F����
						}
					}
				} else if(i1==1) {										// <�L�����ޕ�����1>
					if(scr.p[Si1]==1) {									// �����������L���ʹװ
						er = 3;
					} else {
						for(i=0; i<DIM; i++) {
							Si3 = (scr.p[i]==1) ? i : Si3;				// �ʒu�m�F����
						}
					}
				} else {												// �L�����ނ��S/0�ʹװ
					er = 2;
				}
			}
			if(er==0) {													// <����ȏꍇ>
				BaseCmdPtr(1, op1, &cmp);								// �L���擪����ގ擾
				if(cmp!=NULL) {
					i1 = GetParaInt(cmp, 1);							// �Ȑ����EӰ�ގ擾
				} else {
					er = 1;
				}
			}
			if(er==0&&ct1==1) {											// <����ȏꍇ>
				Si4 = m_SurfTension;									// ���͌W���ۑ�
				SetSelNo(op1, 1);										// �f�ʋȐ����̓o�^
				Si5 = GetVtxNum(op1);									// �Ȑ����_��
				for(i=0; i<Si5; i++) {
					GetVtx(op1, i, &Spw[i]);							// ��Ȑ����_�擾
				}
				if(i1) {												// ��Ȑ�:�͎n�_�ǉ�
					Spw[Si5] = Spw[0], Si5++;
				}
				OffEdtAllObject();										// �ҏW���̑S����
				CountEdtObj();											// �ҏW�Ώۗ��̶���
			} else if(er==1||ct1!=1) {
				AfxMessageBox(IDS_ERR_SEL8);							// �װү����
			} else if(er==2) {
				AfxMessageBox(IDS_ERR_PLANE);
			} else {
				AfxMessageBox(IDS_ERR_DIRECTION);
			}
			ExitMenu(0, 0);												// �ƭ��I��
		} else {														// <<2D�Ȑ�OP�L>>
			Si2 = (m_FChar&0x8) ? 0 : Si2;								// [F8]:�폜�Ȃ�
			Si24 = (m_FChar&0x80) ? 1 : Si24;							// [F12]:�͈͌���
			ct1 = 0, ct2 = 0, er = 0, Si6 = 0, Si7 = 0, Si8 = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// �\���Ώۗ��̎擾
			while(op!=NULL) {											// <<�ҏW���̗L�̏ꍇ>>
				i3 = GetObjAtr(op);										// ���̑����擾
				if(GetSelNo(op)==1) {									// <��Ȑ��̏ꍇ>
					er = (i3/10!=1) ? 3 : er;							// �Ȑ��ȊO�ʹװ
					ct1++;
				} else if(i2==1) {										// <�ҏW�Ώۂ̏ꍇ>
					ct2++;
					BaseCmdPtr(1, op, &cmp);							// �L���擪����ގ擾
					if((i3==10||i3==11)&&cmp!=NULL) {					// <�Ȑ��̏ꍇ>
						vn = GetParaInt(cmp, 0);						// ����_���擾
						vb = GetParaInt(cmp, 1);						// ���EӰ�ގ擾
						if(vn+Si6>m_MaxWCtl) {							// ����_���v���ް�͒��~
							er = 2; break;
						}
						for(i=0; i<vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// ����_�擾
						}
						SetCurve(vn, vb, Spa);							// �Ȑ�����_����
						GetObjPtr(&op1);								// ����_OP�ۑ�
						BaseCmdPtr(1, op, &cmp);						// �L���擪����ގ擾
						NextCmdPtr(&cmp);								// �ȖʕҏW������
						while(cmp!=NULL) {
							RedoCommandCopy(op1, cmp);					// ����ޕ��ʎ��s
							NextCmdPtr(&cmp);							// ������ނ�
						}
						for(i=0; i<vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// ���̕ҏW�㐧��_�擾
						}
						FreeObj(op1, 1);								// �ҏW�㗧�̍폜
						Si6 += vn;										// ����_�����v
						if(++Si8==1) {									// �ҏW�ΏۋȐ���
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// ���͌W���ݒ�
							}
						}
					} else if((i3==20||i3==21)&&cmp!=NULL) {			// <�ʏ�Ȗʂ̏ꍇ>
						un = GetParaInt(cmp, 0);						// U����_���擾
						vn = GetParaInt(cmp, 1);						// V����_���擾
						ub = GetParaInt(cmp, 2);						// U���EӰ�ގ擾
						vb = GetParaInt(cmp, 3);						// V���EӰ�ގ擾
						if(un*vn+Si6>m_MaxWCtl) {						// ����_���v���ް�͒��~
							er = 2; break;
						}
						for(i=0; i<un*vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// ����_�擾
						}
						SetSurface(un, vn, ub, vb, Spa);				// �Ȗʐ���_����
						GetObjPtr(&op1);								// ����_OP�ۑ�
						BaseCmdPtr(1, op, &cmp);						// �L���擪����ގ擾
						NextCmdPtr(&cmp);								// �ȖʕҏW������
						while(cmp!=NULL) {
							if(GetCmdNo(cmp)!=THICKNESS) {				// <���݂Â��ȊO�̏ꍇ>
								RedoCommandCopy(op1, cmp);				// ����ޕ��ʎ��s
							}
							NextCmdPtr(&cmp);							// ������ނ�
						}
						for(i=0; i<un*vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// ���̕ҏW�㐧��_�擾
						}
						FreeObj(op1, 1);								// �ҏW�㗧�̍폜
						Si6 += un*vn;									// ����_�����v
						if(++Si8==1) {									// �ҏW�ΏۋȖʐ�
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// ���͌W���ݒ�
							}
						}
					} else {											// �ҏW����:NG
						er = 1; break;
					}
				}
				NextObjPtr(1, &op, &i1, &i2);							// ���\�����̂�
			}
			if(er==0&&ct1==1&&ct2>0) {
				if(Si24) {												// <�͈͌��肠��>
					if(SetTRLimit(NULL)) {								// �ׯ���͈͌���ݒ�
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
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// �ׯ���F/�w���׸ޏ�����
				}
				Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);				// �ׯ��1�ݒ�
			} else {
				if(er==3||ct1!=1) {
					AfxMessageBox(IDS_ERR_SEL8);
				} else if(er==2) {
					AfxMessageBox(IDS_MANY_ECTLP);
				} else {
					AfxMessageBox(IDS_ERR_SEL5);						// �װү����
				}
				ExitMenu(0, 0);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
		Display(0);														// �ĕ`��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		for(i=0, i1=0; i<Si6; i++) {
			i1 = (Six[i]>0) ? i1+1 : i1;								// �w���ϐ�����
		}
		if(i1>0) {														// <�w������̏ꍇ>
			StartMenu();												// �ƭ��J�n����
			InitLevel();												// Undo/Redo������
			m_SurfTension = Si4;
			AdjustCPoint(Si1, Si3, Si5, Spw, Si8, Si6, Six,
                         Spa, Spb, Spc, Spd, Sia, Sib, Sic);			// ����_�Ȑ����킹
			if(Si2) {													// <�f�ʓo�^���̍폜����>
				SelBaseObjPtr(1, &op);									// �f�ʓo�^���̎擾
				if(op!=NULL) {
					LevelObj(1, op);									// �������ٍ폜���̐ڑ�
					TempObj(op);										// ���̈ꎞ�폜
				}
			}
			EndJMenu1();												// �ƭ��I������
			SetDataUse();												// �ް��g�p���ݒ�
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		ResetSelNo();													// ���̑I��Noؾ��
		ExitMenu(0, 0);													// �ƭ��I��
		Display(0);														// �ĕ`��
	}
}

/********************************************************************/
void CJcad3GlbView::MouseObjectAdjust(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< ����_���̍��킹
/********************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, ct1, ct2, er, un, vn, ub, vb;

	if((ms==2||ms==3)&&Si22) {											// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&plane!=3) {									// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(abs(BasePoint.x-point.x)>=DRAGMIN||
           abs(BasePoint.y-point.y)>=DRAGMIN) {
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			box.bp[0] = BaseCursor, box.bp[1] = pnt;
			DragCtrlTracker1(plane, box, Spx, Si6, Spy, Siy,
                             Si7, &Si21, &i1, Siz);						// �ׯ����ׯ��1
			if(i1>0) {													// <����_�I���̏ꍇ>
				for(i=0; i<i1; i++) {
					i2 = Siz[i], pnt = Spx[i2];							// �I���ׯ��No/���W
					Six[i2] = (Six[i2]==4) ? 0 : 4;						// �I���󋵔��]
					for(j=0; j<Si6; j++) {								// <�w��_������W>
						if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                           fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                           fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
							Six[j] = (Six[j]==4) ? 0 : 4;				// �I���󋵔��]
						}
					}
				}
				SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                               Spy, Siy, &Si7);							// �����}�ׯ���ݒ�
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker1(flg, point, plane, ms, pnt, Spx, Si6,
                        Spy, Siy, Si7, &Si21, &Si22, &i1, Siz);			// �S�ׯ������1
		if(Si22) {
			return;
		}
		if(i1>0) {														// <����_�I���̏ꍇ>
			if(!m_CtlInp) {
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// �S����_�I������
				}
			}
			for(i=0; i<i1; i++) {
				i2 = Siz[i], pnt = Spx[i2];								// �I���ׯ��No/���W
				Six[i2] = (Six[i2]==4) ? 0 : 4;							// �I���󋵔��]
				for(j=0; j<Si6; j++) {									// <�w��_������W>
					if(fabs(pnt.p[0]-Spx[j].p[0])<EPSILON&&
                       fabs(pnt.p[1]-Spx[j].p[1])<EPSILON&&
                       fabs(pnt.p[2]-Spx[j].p[2])<EPSILON&&j!=i2) {
						Six[j] = (Six[j]==4) ? 0 : 4;					// �I���󋵔��]
					}
				}
			}
			SetParsTracker(7, &Si21, 0, Six, Spx, Six, Si6,
                           Spy, Siy, &Si7);								// �����}�ׯ���ݒ�
		} else if(!m_CtlInp) {											// <�ׯ���w�薳�Œǉ���>
			for(i=0; i<Si6; i++) {
				Six[i] = 0;												// �S����_�I������
			}
			Si21 = 0, Si7 = 0;											// �����}�I������
			Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);					// �ׯ��1�ݒ�
			Tracker2.Reset(0);											// �ׯ��2����
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(GetSelectNo()==0) {											// <<����OP��>>
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			ct1 = 0, er = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			while(op!=NULL) {
				if(GetObjAtr(op)/10==1) {								// �Ȑ��ʹװ
					er = 1; break;
				}
				op1 = op;												// ����OP�ۑ�
				if(++ct1>1) {											// ���̐�����UP
					er = 1; break;
				}
				NextObjPtr(2, &op, &i1, &i2);							// ���ҏW���̂�
			}
			if(er==0&&ct1==1) {											// <����ȏꍇ>
				Si4 = m_SurfTension;									// ���͌W���ۑ�
				SetSelNo(op1, 1);										// �f�ʋȐ����̓o�^
				OffEdtAllObject();										// �ҏW���̑S����
				CountEdtObj();											// �ҏW�Ώۗ��̶���
			} else {
				AfxMessageBox(IDS_ERR_SEL1);							// �װү����
			}
			ExitMenu(0, 0);												// �ƭ��I��
		} else {														// <<2D�Ȑ�OP�L>>
			Si24 = (m_FChar&0x80) ? 1 : Si24;							// [F12]:�͈͌���
			ct1 = 0, ct2 = 0, er = 0, Si6 = 0, Si7 = 0, Si8 = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// �\���Ώۗ��̎擾
			while(op!=NULL) {											// <<�\�����̗L�̏ꍇ>>
				i3 = GetObjAtr(op);										// ���̑����擾
				if(GetSelNo(op)==1) {									// <��ʂ̏ꍇ>
					er = (i3/10==1) ? 3 : er;							// �Ȑ��ʹװ
					ct1++;
				} else if(i2==1) {										// <�ҏW�Ώۂ̏ꍇ>
					ct2++;
					BaseCmdPtr(1, op, &cmp);							// �L���擪����ގ擾
					if((i3==10||i3==11)&&cmp!=NULL) {					// <�Ȑ��̏ꍇ>
						vn = GetParaInt(cmp, 0);						// ����_���擾
						vb = GetParaInt(cmp, 1);						// ���EӰ�ގ擾
						if(vn+Si6>m_MaxWCtl) {							// ����_���v���ް�͒��~
							er = 2; break;
						}
						for(i=0; i<vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// ����_�擾
						}
						SetCurve(vn, vb, Spa);							// �Ȑ�����_����
						GetObjPtr(&op1);								// ����_OP�ۑ�
						BaseCmdPtr(1, op, &cmp);						// �L���擪����ގ擾
						NextCmdPtr(&cmp);								// �ȖʕҏW������
						while(cmp!=NULL) {
							RedoCommandCopy(op1, cmp);					// ����ޕ��ʎ��s
							NextCmdPtr(&cmp);							// ������ނ�
						}
						for(i=0; i<vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// ���̕ҏW�㐧��_�擾
						}
						FreeObj(op1, 1);								// �ҏW�㗧�̍폜
						Si6 += vn;										// ����_�����v
						if(++Si8==1) {									// �ҏW�ΏۋȐ���
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// ���͌W���ݒ�
							}
						}
					} else if((i3==20||i3==21)&&cmp!=NULL) {			// <�ʏ�Ȗʂ̏ꍇ>
						un = GetParaInt(cmp, 0);						// U����_���擾
						vn = GetParaInt(cmp, 1);						// V����_���擾
						ub = GetParaInt(cmp, 2);						// U���EӰ�ގ擾
						vb = GetParaInt(cmp, 3);						// V���EӰ�ގ擾
						if(un*vn+Si6>m_MaxWCtl) {						// ����_���v���ް�͒��~
							er = 2; break;
						}
						for(i=0; i<un*vn; i++) {
							GetParaPnt(cmp, i, &Spa[i]);				// ����_�擾
						}
						SetSurface(un, vn, ub, vb, Spa);				// �Ȗʐ���_����
						GetObjPtr(&op1);								// ����_OP�ۑ�
						BaseCmdPtr(1, op, &cmp);						// �L���擪����ގ擾
						NextCmdPtr(&cmp);								// �ȖʕҏW������
						while(cmp!=NULL) {
							if(GetCmdNo(cmp)!=THICKNESS) {				// <���݂Â��ȊO�̏ꍇ>
								RedoCommandCopy(op1, cmp);				// ����ޕ��ʎ��s
							}
							NextCmdPtr(&cmp);							// ������ނ�
						}
						for(i=0; i<un*vn; i++) {
							GetVtx(op1, i, &Spx[Si6+i]);				// ���̕ҏW�㐧��_�擾
						}
						FreeObj(op1, 1);								// �ҏW�㗧�̍폜
						Si6 += un*vn;									// ����_�����v
						if(++Si8==1) {									// �ҏW�ΏۋȖʐ�
							for(i=0; i<Si6; i++) {
								Sic[i] = Tens[i];						// ���͌W���ݒ�
							}
						}
					} else {											// �ҏW����:NG
						er = 1; break;
					}
				}
				NextObjPtr(1, &op, &i1, &i2);							// ���\�����̂�
			}
			if(er==0&&ct1==1&&ct2>0) {
				if(Si24) {												// <�͈͌��肠��>
					if(SetTRLimit(NULL)) {								// �ׯ���͈͌���ݒ�
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
				for(i=0; i<Si6; i++) {
					Six[i] = 0;											// �ׯ���F/�w���׸ޏ�����
				}
				Tracker1.Setup(Spx, Six, NULL, Si6, 4, 7);				// �ׯ��1�ݒ�
			} else {
				if(er==3||ct1!=1) {
					AfxMessageBox(IDS_ERR_SEL1);
				} else if(er==2) {
					AfxMessageBox(IDS_MANY_ECTLP);
				} else {
					AfxMessageBox(IDS_ERR_SEL5);						// �װү����
				}
				ExitMenu(0, 0);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
		Display(0);														// �ĕ`��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		for(i=0, i1=0; i<Si6; i++) {
			i1 = (Six[i]>0) ? i1+1 : i1;								// �w���ϐ�����
		}
		if(i1>0) {														// <�w������̏ꍇ>
			m_SurfTension = Si4;
			ObjectAdjustCP(Si8, Si6, Spx, Six, Spa, Spb, Spc, Spd,
                           Spw, Sia, Sib, Sic);							// ����_���̍��킹
		}
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		ResetSelNo();													// ���̑I��Noؾ��
		ExitMenu(0, 1);													// �ƭ��I��
		Display(0);														// �ĕ`��
	}
}

/******************************************/
void CJcad3GlbView::MenuObjectAdjust(void)								//<<< �Ȗʗ��̍��킹
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i1, i2, atr, ct1=0, ct2=0, err=0;

	if(GetSelectNo()==0) {												// <<�Ȗ�OP��>>
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// �Ȑ��ʹװ
			if(err) {
				break;													// �װ�͒��~
			}
			if(++ct1>1) {												// �Ȗʐ�����UP
				break;
			}
			SetSelNo(op, 1);											// ��Ȗʗ��̓o�^
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(err==0&&ct1==1) {											// <�L���ȏꍇ>
			OffEdtAllObject();											// �ҏW���̑S����
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		} else {														// <�װ�̏ꍇ>
			ResetSelNo();												// ���̑I��ؾ��
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
		ExitMenu(0, 0);													// �ƭ��I��
	} else {															// <<�Ȗ�OP�L>>
		BaseObjPtr(1, &op, &i1, &i2);									// �\���Ώۗ��̎擾
		while(op!=NULL) {
			atr = GetObjAtr(op);										// ���̑����擾
			if(GetSelNo(op)==1) {										// <��ʂ̏ꍇ>
				err = (atr/10==1) ? 1 : err;							// �Ȑ��ʹװ
				ct1++;
			} else if(i2==1) {											// <�ҏW�Ώۂ̏ꍇ>
				BaseCmdPtr(1, op, &cmp);								// �L���擪����ގ擾
				while(cmp!=NULL) {
					i1 = GetCmdNo(cmp);									// �����No�擾
					if(i1!=CRTSURF&&i1!=CHGSURF) {						// �Ȗʐ����ȊO�װ
						err = 2; break;
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
				err = (atr!=20&&atr!=21) ? 3 : err;						// �ʏ�ȖʈȊO�ʹװ
				ct2++;													// �ҏW�Ώۗ��̐�����UP
				SetSelNo(op, 2);										// �ҏW�Ώۗ��̓o�^
			}
			if(err||ct1>1) {
				break;													// �װ�͒��~
			}
			NextObjPtr(1, &op, &i1, &i2);								// ���\�����̂�
		}
		if(err==0&&ct1==1&&ct2>0) {
			ObjectAdjust();												// �Ȗʎw��ړ�
		} else if(err==1||ct1!=1) {
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		} else if(err==3||ct2==0) {
			AfxMessageBox(IDS_ERR_SEL3);
		} else {
			AfxMessageBox(IDS_ERR_OADJUST);
		}
		ResetSelNo();													// ���̑I��ؾ��
		ExitMenu(0, 1);													// �ƭ��I��
	}
	Display(0);															// �ĕ`��
}

/**********************************************/
void CJcad3GlbView::MenuChgSurfParam(int mode)							//<<< �Ȑ����Ұ��ύX
/**********************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, cnt=0, cno, mx, i, stp=0;
	double   wt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �擪���̂��߲��(�ҏW�Ώ�)
	while(op) {															// �w�藧�̂����݂���
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		cno = GetCmdNo(cmp);											// �����No�擾
		if(cno==CRTCURVE||cno==CHGCURVE) {								// <�Ȑ�����/�ύX>
			mx = GetParaInt(cmp, 2);									// �Ȑ����J�Ԑ�
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// �Ȑ��d�݌W��
			cnt++; break;
		}
		if(cno==CRTSURF||cno==CHGSURF||									// <�Ȗʐ���/�ҏW>
		   cno==CRTSWEEP||cno==CHGSWEEP) {								// <�|���Ȗʐ���/�ҏW>
			mx = GetParaInt(cmp, 4);									// �Ȑ����J�Ԑ�
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// �Ȑ��d�݌W��
			cnt++; break;
		}
		NextObjPtr(2, &op, &dsp, &edt);
	}
	if(cnt==0) {														// <�ΏەҏW���̂Ȃ�>
		return;
	}
	if(!mode) {															// <�ʏ�Ӱ��>
		CInputDlg dlg(9);
		dlg.SetValue(mx, wt);											// �Ȑ�������,�Ȑ��d�݌W���擾
		if(dlg.DoModal()==IDOK) {										// �޲�۸ޕ\��
			dlg.GetValue(&mx, &wt);
		} else {
			stp = 1;													// ���~
		}
	} else {															// <���͂Ȃ�Ӱ��>
		if(mx!=m_CurveRepeat||wt!=m_CurveWeight) {						// ���ѓo�^�l�Ɨ��̓o�^�l��
			mx = m_CurveRepeat;											// �قȂ�ꍇ�ͼ��ѓo�^�l���̗p
			wt = m_CurveWeight;
		} else {														// ���ѓo�^�l�Ɨ��̓o�^�l��
			stp = 1;													// ��v�̏ꍇ�͒��~
		}
	}
	if(!stp) {															// <<���~�Ȃ�>>
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		BaseObjPtr(2, &op, &dsp, &edt);									// �擪���̂��߲��(�ҏW�Ώ�)
		while(op) {														// �w�藧�̂����݂���
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			cno = GetCmdNo(cmp);										// �����No�擾
			if(cno==CRTCURVE||cno==CHGCURVE) {							// <�Ȑ�����/�ύX>
				Si1 = GetParaInt(cmp, 0);								// ����_���擾
				Si2 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				for(i=0; i<Si1; i++) {
					Sia[i] = GetParaInt(cmp, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// ����_�擾
				}
				ChgCurve(op, Si1, Si2, mx, wt, Spa, Sia);				// �Ȑ��ύX
			} else if(cno==CRTSURF||cno==CHGSURF) {						// <�Ȗʐ���/�ҏW>
				Si1 = GetParaInt(cmp, 0);								// U����_���擾
				Si2 = GetParaInt(cmp, 1);								// V����_���擾
				Si3 = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
				Si4 = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
				Si5 = Si1*Si2;											// ������_��
				for(i=0; i<Si5; i++) {
					Sia[i] = GetParaInt(cmp, 7+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// ����_�擾
				}
				ChgSurface(op, Si1, Si2, Si3, Si4, mx, wt, Spa, Sia);	// �ȖʕύX
			} else if(cno==CRTSWEEP||cno==CHGSWEEP) {					// <�|���Ȗʐ���/�ҏW>
				Si1 = GetParaInt(cmp, 0);								// U����_���擾
				Si2 = GetParaInt(cmp, 1);								// V����_���擾
				Si3 = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
				Si4 = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
				for(i=0; i<Si1+Si2; i++) {
					Sia[i] = GetParaInt(cmp, 8+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spa[i]);						// ����_�擾
				}
				ChgSweepSurf(op, Si1, Si2, Si3, Si4, mx, wt, Spa, Sia);	// �|���ȖʕύX
			}
			NextObjPtr(2, &op, &dsp, &edt);
		}
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
		Display(0);														// �ĕ`��
		MainWnd->SetCrtPrm(mx, wt);										// �������Ұ��ݒ�
	}
}

/***************************************************************/
void CJcad3GlbView::MouseTension(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< ���͌W���ݒ�
/***************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  area;
	int      i, i1, i2, i3;

	if(Sphase!=0&&(ms==2||ms==3)&&Si22) {								// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase==2&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==2) {
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				area.bp[0] = BaseCursor, area.bp[1] = pnt;
				SelAreaCtrl(plane, area, Si5, Spa, Tens, m_CtlInp);		// ����_�I��(�ر)
			}
			for(i=0; i<Si5; i++) {
				Sia[i] = 2*Tens[i];
			}
			Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);					// �ׯ��1�ݒ�
			DeleteBox();												// ��`�j��
			Display(0);													// �ĕ`��
			Sphase = 1;
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		Sphase = 1, i1 = -1, i2 = -1;
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(plane==3) {													// <�����}�I��>
			if((i1=Tracker1.HitTracker3D(0, point, Spa, Si5))==-1) {	// �ׯ��1����
				EyeChange(flg, point, plane, ms);						// ���_�ύX
				Si22 = 1;												// ���_�ύXӰ��
				return;
			}
		} else {														// <3�ʐ}�I��>
			i2 = Tracker1.HitTrackerS(0, plane, pnt, Spa, Si5, Sia);	// �ׯ��1����
		}
		if(!m_CtlInp) {
			for(i=0; i<Si5; i++) {
				Tens[i] = 0;											// �I��S����
			}
		}
		if(i1!=-1) {													// <�����}�I��OK>
			Tens[i1] = (Tens[i1]) ? 0 : 1;
			for(i=0; i<Si5; i++) {
				if(fabs(Spa[i].p[0]-Spa[i1].p[0])<EPSILON&&
                   fabs(Spa[i].p[1]-Spa[i1].p[1])<EPSILON&&
                   fabs(Spa[i].p[2]-Spa[i1].p[2])<EPSILON&&i!=i1) {		// ������W�_�̏ꍇ
					Tens[i] = (Tens[i]) ? 0 : 1;						// �I�𔽓]
				}
			}
		} else if(i2>0) {												// <3�ʐ}�I��OK>
			for(i=0; i<i2; i++) {
				Tens[Sia[i]] = (Tens[Sia[i]]) ? 0 : 1;					// �I�𔽓]
			}
		} else {														// <��ׯ�ފJ�n>
			Sphase = 2;
		}
		for(i=0; i<Si5; i++) {
			Sia[i] = 2*Tens[i];
		}
		Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);						// �ׯ��1�ݒ�
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			Sop1 = NULL, Sop2 = NULL;
			Si22 = 0;
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if((i3==10||i3==11||i3==20||i3==21)&&cmp!=NULL) {			// <<�Ȑ���ʏ�Ȗʂ̏ꍇ>>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				Display(0);												// �ĕ`��
				if(i3==20||i3==21) {									// <�Ȗ�>
					Si1 = GetParaInt(cmp, 0);							// U����_���擾
					Si2 = GetParaInt(cmp, 1);							// V����_���擾
					Si3 = GetParaInt(cmp, 2);							// U���EӰ�ގ擾
					Si4 = GetParaInt(cmp, 3);							// V���EӰ�ގ擾
				} else {												// <�Ȑ�>
					Si2 = GetParaInt(cmp, 0);							// ����_���擾
					Si4 = GetParaInt(cmp, 1);							// ���EӰ�ގ擾
					Si1 = 1, Si3 = 0;
				}
				Si5 = Si1*Si2;											// ������_��
				CTensionDlg dlg;
				dlg.SetValue(m_TensionNo);								// �����l�\��
				if(dlg.DoModal()==IDOK) {								// �޲�۸ޕ\��
					dlg.GetValue(&m_SurfTension, &i1);					// ���͌W���ݒ�
					m_TensionNo = m_SurfTension;
					if(m_SurfTension==0) {								// <���͐ݒ薳�̏ꍇ>
						for(i=0; i<Si5; i++) {
							Tens[i] = 0;								// �͈͎w��S����
						}
						ExitMenu(0, 0);									// �ƭ��I��
					} else if(!i1) {									// <�ݒ�L&���薳�̏ꍇ>
						for(i=0; i<Si5; i++) {
							Tens[i] = 1;								// �͈͎w��S�ݒ�
						}
						ExitMenu(0, 0);									// �ƭ��I��
					} else {											// <�ݒ�L&����L�̏ꍇ>
						for(i=0; i<Si5; i++) {
							Tens[i] = 0;								// �͈͎w��S����
						}
					}
				} else {
					ExitMenu(0, 0);										// ��ݾق��ƭ��I��
				}
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL5);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
			}
			if(m_ActMenu!=NONE) {										// <�͈͌���L�̏ꍇ>
				for(i=0; i<Si5; i++) {
					GetParaPnt(cmp, i, &Spa[i]);						// ����_�擾
				}
				if(i3==20||i3==21) {									// <�Ȗ�>
					SetSurface(Si1, Si2, Si3, Si4, Spa);				// �Ȗʐ���_����
				} else {												// <�Ȑ�>
					SetCurve(Si2, Si4, Spa);							// �Ȑ�����_����
				}
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				SetSelNo(Sop2, 2);										// ���F�\��OP�ݒ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �ȖʕҏW������
				while(cmp!=NULL) {
					if(GetCmdNo(cmp)!=THICKNESS) {
						RedoCommandCopy(Sop2, cmp);						// ����ޕ��ʎ��s
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
				SetDispList(Sop2);										// �ި���ڲؽēo�^
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2, i, &Spa[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si5; i++) {
					Sia[i] = 2*Tens[i];
				}
				if(Si24) {												// <�͈͌��肠��>
					if(SetTRLimit(Sop2)) {								// �ׯ���͈͌���ݒ�
						Tracker1.Limit(m_XLmtR, m_YLmtR, m_ZLmtR,
                                       m_XLmt1, m_XLmt2, m_YLmt1,
                                       m_YLmt2, m_ZLmt1, m_ZLmt2);		// �ׯ��1�͈͌���
					} else {
						Si24 = 0;										// ��ݾق͔͈͌��蒆�~
					}
				} else {												// <�͈͌���Ȃ�>
					InitTRLimit();										// �ׯ���͈͌��菉����
				}
				Tracker1.Setup(Spa, Sia, NULL, Si5, 4, 7);				// �ׯ��1�ݒ�
				OffUndoRedo();											// Undo/Redo�s��
				Display(0);												// �ĕ`��
				m_CursorNo = 1;											// ����No�ݒ�
			}
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		for(i1=0, i=0; i<Si5; i++) {
			if(Tens[i]) {
				i1 = 1; break;
			}
		}
		if(!i1) {
			m_SurfTension = 0;											// �͈͖��͒��͐ݒ薳��
		}
		if(Sop2!=NULL) {
			FreeObj(Sop2, 1);											// ���̍폜
		}
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}
