///////////////////////////////////////////////////////////////////////////// 
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewEditSurf1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputBtnDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "EditCtrlTracker.h"
#include "EditObject.h"
#include "Symmetry.h"
#include "ConvSCurve.h"
#include "NumFunc.h"

/*******************************************************************/
void CJcad3GlbView::MouseCPointSpace(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< �b�o�r
/*******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt1, pt2;
	BOXTYPE  box;
	int      hit, i, i1, i2, i3, gno[32];
	double   th, l11, l12, l21, l22;

	if((Sphase!=0&&(ms==2||ms==3)&&Si22)||
       (Sphase>=2&&ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX)) {			// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase==1&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==3&&plane==Si11&&(flg&MK_LBUTTON)) {			// ϳ����t�������̂ݗL��
			if(Si14==0) {												// <�ړ�����>
				CalcScreenWorld(0, &pnt, point, plane);					// ���W�_�擾
				for(i=0; i<DIM; i++) {
					Sdb[i] = pnt.p[i]-Spnt4.p[i];						// ���̈ړ��ʎZ�o
				}
				if(flg&m_LimKey) {
					LimitValue(plane, Sdb);								// ϳ����͐����l
				}
				Sdb[Si13] = 0.0;										// �w�蕽�ʊO�̍��W��0
			} else if(Si14==1) {										// <��]����>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				l11 = Spnt4.p[Si11]-Sbase.p[Si11];						// �p�x�Z�o
				l12 = Spnt4.p[Si12]-Sbase.p[Si12];
				l21 = pnt.p[Si11]-Sbase.p[Si11];
				l22 = pnt.p[Si12]-Sbase.p[Si12];
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
				Sdb[Si13+3] = th;
			} else {													// <���ޕύX����>
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				pt1 = Spnt4, pt1.p[Si13] = 0.0;							// �w�蕽�ʈȊO�̍��W��0
				pt2 = Sbase, pt2.p[Si13] = 0.0;
				pnt.p[Si13] = 0.0;
				l11 = PntD(&pt1, &pt2);									// ���S�_���ׯ���ʒu�܂ł̋���
				l12 = PntD(&pnt, &pt2);									// ���S�_�ƶ��وʒu�܂ł̋���
				if(Si15==0) {											// <���ޕύX�̏ꍇ>
					l21 = (l12/l11) - 1.0;								// �������̔{��
					Sdb[6] = l21, Sdb[7] = l21, Sdb[8] = l21;
				} else {												// <���ˏ�ړ��̏ꍇ>
					Sdb[9] = l12 - l11;									// �ړ�����
				}
			}
			pnt = (Si9==0) ? Spnt1 : ((Si9==1) ? Spnt2 : Spnt3);
			CtrlPointSpace(Spa, Spw, Sib, Si5, Spnt2, pnt, Si15, Si11,
                           Sda[0]+Sdb[0], Sda[1]+Sdb[1], Sda[2]+Sdb[2],
                           Sda[3]+Sdb[3], Sda[4]+Sdb[4], Sda[5]+Sdb[5],
                           Sda[6]+Sdb[6], Sda[7]+Sdb[7], Sda[8]+Sdb[8],
                           Sda[9]+Sdb[9]);								// ����_��߰�(CPS)
			MoveCPoint(0, Spw, Sop1, Si1, Si2, Si3, Si4, m_ObjCWgt,
                       Sia, Sib, Spa, Spb, Spc, Tens);					// ����_�ړ�
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ����ʉߓ_�ϊ�
			for(i=0; i<Si5; i++) {
				Spd[i] = Spc[i];
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			Tracker4.Setup(Spd, Sis, NULL, Si5, 4, 3);					// �ׯ��4�ݒ�(����_�ʒu�\��)
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			Si6 = 1;													// ���s�ς�
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spb, Si5, Spx,
                                 Six, Si10, &Si21, &i1, Siy);			// �ׯ����ׯ��1
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickPointMove(0, i1, Siy, Si1, Si2, Si3, Si4,
                                  m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                                  Sop2, Spa, Spb, Sia, Sib, Sic);		// ����_�ړ� �_�I��
					SetParsTracker(0, &Si21, 0, Sib, Spb, Sic,
                                   Si5, Spx, Six, &Si10);				// �����}�ׯ���ݒ�
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
					Si23 = 1;
				}
			}
		} else if(Sphase==3) {											// <�ҏW�̏ꍇ>
			for(i=0; i<10; i++) {
				Sda[i] += Sdb[i], Sdb[i] = 0.0;							// �����Z�l�����Z
			}
			Sda[6] = (Sda[6]<0.01) ? 0.01 : Sda[6];						// �L���{������
			Sda[7] = (Sda[7]<0.01) ? 0.01 : Sda[7];
			Sda[8] = (Sda[8]<0.01) ? 0.01 : Sda[8];
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &Sbase);										// �ޯ�������擾
			TrackerBoxCrs2(box, Sbase, Spy);							// �ׯ���擾
			Spz[0] = Spy[0], Spz[1] = Spy[1], Spz[2] = Spy[2];
			Spz[3] = Spy[3], Spz[4] = Spy[4], Spz[5] = Spy[7];
			Tracker1.Setup(Spz, Siz, TensC, 6, 0, 7);					// �ׯ��1�ݒ�
			Spz[0] = Spy[0], Spz[1] = Spy[3], Spz[2] = Spy[4];
			Spz[3] = Spy[5], Spz[4] = Spy[6], Spz[5] = Spy[7];
			Tracker2.Setup(Spz, Siz, TensC, 6, 1, 7);					// �ׯ��2�ݒ�
			Spz[0] = Spy[0], Spz[1] = Spy[5], Spz[2] = Spy[6];
			Spz[3] = Spy[1], Spz[4] = Spy[2], Spz[5] = Spy[7];
			Tracker3.Setup(Spz, Siz, TensC, 6, 2, 7);					// �ׯ��3�ݒ�
			Tracker4.Setup(Spd, Sis, NULL, Si5, 4, 3);					// �ׯ��4�ݒ�(����_�ʒu�\��)
			Sphase = 2;
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		if(Sphase==1) {													// <<����_�I��>>
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, Si5,
                            Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <����_�ړ��̏ꍇ>
				Sphase = 2, Si6 = 0;
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Sphase = 1;
					if(!Si15) {											// <�{���w��>
						CInputBtnDlg7 dlg(3);
						dlg.SetParam2(Sop1, Sop2, Si21, Si1, Si2, Si3,
                                      Si4, Si5, Si10, &Si6, Spnt1,
                                      Spnt2, Spnt3, Spa, Spb, Spc, Spd,
                                      Spw, Spx, Sia, Sib, Sic, Six,
                                      Tens, m_EarthAdd1, m_EarthAdd2,
                                      m_EarthAdd3, m_ScaleDir,
                                      m_ScalePnt);						// ���Ұ��ݒ�2
						if(dlg.DoModal()==IDOK) {						// �����޲�۸ޕ\��
							if(Si6==1) {								// �ړ��ς݂̏ꍇ��
								for(i=0; i<Si5; i++) {					// �����ޯ̧�ύX
									Spa[i] = Spc[i], Spb[i] = Spd[i];
								}
							}
						}
						dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2,
                                     &m_EarthAdd3, &m_ScaleDir,
                                     &m_ScalePnt);						// ���Ұ��擾
					} else {											// <���ˏ�ړ�>
						CInputBtnDlg8 dlg;
						dlg.SetParam(Sop1, Sop2, Si21, Si1, Si2, Si3,
                                     Si4, Si5, Si10, plane, &Si6,
                                     Spnt2, Spa, Spb, Spc, Spd, Spw,
                                     Spx, Sia, Sib, Sic, Six, Tens,
                                     m_EarthAdd1, m_EarthAdd2);			// ���Ұ��ݒ�
						if(dlg.DoModal()==IDOK) {						// �����޲�۸ޕ\��
							if(Si6==1) {								// �ړ��ς݂̏ꍇ��
								for(i=0; i<Si5; i++) {					// �����ޯ̧�ύX
									Spa[i] = Spc[i], Spb[i] = Spd[i];
								}
							}
						}
						dlg.GetParam(&m_EarthAdd1, &m_EarthAdd2);		// ���Ұ��擾
					}
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// �Ȗʐ���
					for(i=0; i<Si5; i++) {
						Spb[i] = Spa[i];
					}
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// �Ȗʐ���_����
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					DispParsTracker(0, Si21, Spb, Sic, Si5,
                                    Spx, Six, Si10);					// �����}�ׯ���\��
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
				} else {												// <ϳ����͂̏ꍇ>
					Siz[0] = 0, Siz[1] = 1, Siz[2] = 1;					// �ׯ���F�ݒ�
					Siz[3] = 1, Siz[4] = 1, Siz[5] = 2;
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					BoxCenter(box, &Sbase);								// �ޯ�������擾
					TrackerBoxCrs2(box, Sbase, Spy);					// �ׯ���擾
					TensC[0] = 7, TensC[1] = 5, TensC[2] = 5;
					TensC[3] = 6, TensC[4] = 6, TensC[5] = 8;
					Spz[0] = Spy[0], Spz[1] = Spy[1], Spz[2] = Spy[2];
					Spz[3] = Spy[3], Spz[4] = Spy[4], Spz[5] = Spy[7];
					Tracker1.Setup(Spz, Siz, TensC, 6, 0, 7);			// �ׯ��1�ݒ�
					Spz[0] = Spy[0], Spz[1] = Spy[3], Spz[2] = Spy[4];
					Spz[3] = Spy[5], Spz[4] = Spy[6], Spz[5] = Spy[7];
					Tracker2.Setup(Spz, Siz, TensC, 6, 1, 7);			// �ׯ��2�ݒ�
					Spz[0] = Spy[0], Spz[1] = Spy[5], Spz[2] = Spy[6];
					Spz[3] = Spy[1], Spz[4] = Spy[2], Spz[5] = Spy[7];
					Tracker3.Setup(Spz, Siz, TensC, 6, 2, 7);			// �ׯ��3�ݒ�
					Tracker4.Setup(Spb, Sis, NULL, Si5, 4, 3);			// �ׯ��4�ݒ�(����_�ʒu�\��)
				}
			} else if(i1>0) {											// <����_�I���̏ꍇ>
				if(!m_CtlInp) {
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// �S����_�I������
					}
				}
				PickPointMove(0, i1, Siy, Si1, Si2, Si3, Si4,
                              m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                              Sop2, Spa, Spb, Sia, Sib, Sic);			// ����_�ړ� �_�I��
				SetParsTracker(0, &Si21, 0, Sib, Spb, Sic, Si5, Spx,
                               Six, &Si10);								// �����}�ׯ���ݒ�
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				Si23 = 1;
			} else if(!m_CtlInp) {										// <�ׯ���w�薳�Œǉ���>
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// �S����_�I������
				}
				Si21 = 0, Si10 = 0;										// �����}�I������
				InitLineNo(Sop2);										// ײݔԍ�������
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Tracker4.Reset(0);										// �ׯ��3����
			}
		} else if(Sphase==2) {											// <<�ҏW�I��>>
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spy, 8))!=-1) {	// �ׯ��1����
				Si11 = plane;											// ���ʕۑ�
				Si12 = (Si11+1>2) ? 0 : Si11+1;							// ���W���ݒ�
				Si13 = (Si12+1>2) ? 0 : Si12+1;
				Si14 = -1;												// ��������
				if(hit==0) {											// <�ړ�>
					Si14 = 0;											// �ړ�����
					Spnt4 = Sbase;
				} else if(hit==7) {										// <��]>
					Si14 = 1;											// ��]����
					Spnt4 = Spy[7];
				} else {												// <���ޕύX>
					i = (hit-1)/2;										// ��No�擾
					if(i!=Si13) {										// <�L�����ׯ��>
						Si14 = 2;										// ���ޕύX����
						Spnt4 = Spy[hit];
					}
				}
				if(Si14!=-1) {											// <�ׯ���I��OK>
					Tracker1.Reset(0);									// �ׯ��1����
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
					Sphase = 3, Sd1 = 0.0, Sd2 = 0.0;
				}
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
			Si9 = (m_FChar&0x4) ? 0 : 1;								// [F7]:��_-Z�ŏ��w��
			Si9 = (m_FChar&0x8) ? 2 : Si9;								// [F8]:��_-Z�ő�w��
			Si15 = (m_FChar&0x10) ? 1 : 0;								// [F9]:���ˏ�ړ�
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
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// ����_�Ώ��׸޸ر
					Sis[i] = 4;											// �ړ����ׯ���F�ݒ�
				}
				for(i=0; i<10; i++) {
					Sda[i] = 0.0, Sdb[i] = 0.0;
				}
				Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;				// �{���̂ݏ����l��1.0
				CalcBox(Spa, Si5, &box);								// �}�`BOX���ގ擾
				BoxCenter(box, &Spnt2);									// �ޯ�������擾
				Spnt1 = Spnt2, Spnt1.p[2] = box.bp[0].p[2];				// �ޯ������(Z�ŏ�)
				Spnt3 = Spnt2, Spnt3.p[2] = box.bp[1].p[2];				// �ޯ������(Z�ő�)
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// ����_��m�F
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
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Tracker4.Reset(0);										// �ׯ��4����
				Sphase = 1;												// ����_�w���
				Display(0);												// �ĕ`��
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL3);							// �װү����
				ExitMenu(0, 1);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==2) {													// <<�ҏW��>>
			if(Si6==1) {												// �ړ��ς݂̏ꍇ��
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// �����ޯ̧�ύX
				}
			}
			RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);				// �Ȗʐ���
			for(i=0; i<Si5; i++) {
				Spb[i] = Spa[i];
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spb);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spb[i]);								// ���̕ҏW�㐧��_�擾
			}
			for(i=0; i<Si5; i++) {
				Sib[i] = 0;												// �S����_�I������
			}
			Si21 = 0, Si10 = 0;											// �����}�I������
			InitLineNo(Sop2);											// ײݔԍ�������
			Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);					// �ׯ��1�ݒ�
			Tracker2.Reset(0);											// �ׯ��2����
			Tracker3.Reset(0);											// �ׯ��3����
			Tracker4.Reset(0);											// �ׯ��4����
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			Sphase = 1, i1 = 1;
			for(i=0; i<10; i++) {
				Sda[i] = 0.0, Sdb[i] = 0.0;
			}
			Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;					// �{���̂ݏ����l��1.0
			Display(0);													// �ĕ`��
		} else {
			i1 = 0;
		}
		if(i1==0||ms==9) {												// <<��L�ȊO/�����I��>>
			if(Si20) {
				AttachObj(Sop1);										// ����ލĐڑ�/�Ď��s
			}
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// ���̍폜
			}
			Tracker1.Reset(1);											// �ׯ��1����
			Tracker2.Reset(1);											// �ׯ��2����
			Tracker3.Reset(1);											// �ׯ��3����
			Tracker4.Reset(1);											// �ׯ��4����
			Line.Reset();												// ײ݉���
			SetRenderMode(0);											// ܲ԰�ڰ�Ӱ��
			ResetSelNo();												// ���̑I��Noؾ��
			OnUndoRedo();												// Undo/Redo��
			InitLineNo(NULL);											// ײݔԍ�������
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
			SetDataUse();												// �ް��g�p���ݒ�
			if(!Si23) {
				Undo(gno);												// �ҏW�Ȃ���Undo
			}
			Display(0);													// �ĕ`��
		}
	}
}

/*****************************************************************/
void CJcad3GlbView::MousePointMove(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< ����_�ړ�
/*****************************************************************/
{
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
			MoveSymmetryArea(Spnt1, pnt, Si11, Spnt2, 0, Si14, Si15,
                             -1, -1, Si13, 0, Si5, Sib, Spb, Spw);		// �Ώ̗̈�ړ��ʐݒ�
			MoveCPoint(Si9, Spw, Sop1, Si1, Si2, Si3, Si4, m_ObjCWgt,
                       Sia, Sib, Spa, Spb, Spc, Tens);					// ����_�ړ�
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ���
			CalcSurfacePoint(Si9, 1, 0, Si1, Si2, Si3, Si4, m_ObjCWgt,
                             Sia, Spc, Spd);							// ����_=>�ʉߓ_�ϊ�
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			DispParsTracker(0, Si21, Spd, Sic, Si5, Spx, Six, Si10);	// �����}�ׯ���\��
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			dd = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]+
                      pnt.p[2]*pnt.p[2]);								// �ړ��O�_�Ƃ̋���
			m_pNumDisp->SetParam(pnt.p[0], pnt.p[1], pnt.p[2], dd);		// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Si6 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <����Ӱ��:�ʏ�ȊO>
			if(plane<3) {												// 3�ʐ}�̏ꍇ
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
			}
		} else if(Sphase==1) {											// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spb, Si5, Spx,
                                 Six, Si10, &Si21, &i1, Siy);			// �ׯ����ׯ��1
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickPointMove(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                                  m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                                  Sop2, Spa, Spb, Sia, Sib, Sic);		// ����_�ړ� �_�I��
					SetParsTracker(0, &Si21, 0, Sib, Spb, Sic,
                                   Si5, Spx, Six, &Si10);				// �����}�ׯ���ݒ�
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
					Sphase = 1, Si23 = 1;
				}
			}
		} else if(Sphase==2) {											// <����_�ړ��̏ꍇ>
			if(Si6==1) {												// �ړ��ς݂̏ꍇ��
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i], Spb[i] = Spd[i];					// �����ޯ̧�ύX
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
				Si12 = plane;											// �Ώ̎��w�蕽��
				i = (flg&MK_CONTROL) ? 1 : 0;							// �������w��
				Si13 = (flg&MK_SHIFT) ? 1 : 0;							// ���������]
				SetSymmetryLine(1,Si11,Spnt2,Si12, i, 0, &Si14, &Si15);	// �Ώ̎��ݒ�
			}
		} else {														// <����_�w��̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spb, Si5,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Sib[Siy[0]]==1&&plane!=3) {	// <����_�ړ��̏ꍇ>
				Spnt1 = Spb[Siy[0]];									// �ړ����_�ۑ�
				Sphase = 2, Si6 = 0;
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Sphase = 1;
					CInputBtnDlg6 dlg(2);
					dlg.SetParam2(Sop1, Sop2, Si9, Si21, Si1, Si2, Si3,
                                  Si4, m_ObjCWgt, Si5, Si10, &Si6,
                                  Si11, Si14, Si15, Si13, Spnt1, Spnt2,
                                  Spa, Spb, Spc, Spd, Spw, Spx, Sia,
                                  Sib, Sic, Six, Tens, m_PMoveAdd);		// ���Ұ��ݒ�2
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						if(Si6==1) {									// �ړ��ς݂̏ꍇ��
							for(i=0; i<Si5; i++) {						// �����ޯ̧�ύX
								Spa[i] = Spc[i], Spb[i] = Spd[i];
							}
						}
					}
					dlg.GetParam(&m_PMoveAdd);							// ���Ұ��擾
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// �Ȗʐ���
					CalcSurfacePoint(Si9, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// ����_=>�ʉߓ_�ϊ�
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// �Ȗʐ���_����
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					DispParsTracker(0, Si21, Spb, Sic, Si5, Spx,
                                    Six, Si10);							// �����}�ׯ���\��
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
				}
			} else if(i1>0) {											// <����_�I���̏ꍇ>
				if(!m_CtlInp) {
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// �S����_�I������
					}
				}
				PickPointMove(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                              m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                              Sop2, Spa, Spb, Sia, Sib, Sic);			// ����_�ړ� �_�I��
				SetParsTracker(0, &Si21, 0, Sib, Spb, Sic, Si5, Spx,
                               Six, &Si10);								// �����}�ׯ���ݒ�
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				Si23 = 1;
			} else if(!m_CtlInp) {										// <�ׯ���w�薳�Œǉ���>
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// �S����_�I������
				}
				Si21 = 0, Si10 = 0;										// �����}�I������
				InitLineNo(Sop2);										// ײݔԍ�������
				Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);				// �ׯ��1�ݒ�
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
			Si11 = (m_FChar&0x8) ? 2 : 0;								// [F8]:����Ӱ��:2���Ώ�
			Si11 = (m_FChar&0x4) ? 1 : Si11;							// [F7]:����Ӱ��:1���Ώ�
			Si9 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
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
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// ����_�Ώ��׸޸ر
				}
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				ExecSurface(Si9, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);		// ����_��m�F
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
				if(Si11==0) {											// <�ʏ�Ӱ�ނ̏ꍇ>
					Tracker1.Setup(Spb, Sia, NULL, Si5, 4, 0);			// �ׯ��1�ݒ�
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

/****************************************************************/
void CJcad3GlbView::MouseLineMove(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< ����_��ړ�
/****************************************************************/
{
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
			if(Si11) {													// <�ʏ�Ȗ�>
				MoveSymmetryArea(Spnt1, pnt, Si14, Spnt2, 0, Si17, Si18,
                                 -1, -1, Si16, 0, Si5, Sib, Spb, Spy);	// �Ώ̗̈�ړ��ʐݒ�
				MoveCPoint(Si13, Spy, Sop1, Si1, Si2, Si3, Si4,
                           m_ObjCWgt, Sia, Sib, Spa, Spb, Spc, Tens);	// ����_�ړ�
				RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);			// �Ȗʐ���
				CalcSurfacePoint(Si13, 1,0, Si1, Si2, Si3, Si4,
                                 m_ObjCWgt, Sia, Spc, Spd);				// ����_=>�ʉߓ_�ϊ�
			} else {													// <�|���Ȗ�>
				MoveSymmetryArea(Spnt1, pnt, Si14, Spnt2, 0, Si17, Si18,
                                 -1, -1, Si16, 0, Si6, Siw, Spz, Spy);	// �Ώ̗̈�ړ��ʐݒ�
				MoveSweep(Spy, Si12, Si1, Si2, Sps, Si6, Siw, Spt);		// �|������_�ړ�
				RedoChgSweepSurf(Si1, Si2, Si3, Si4, Spt, Sis);			// �|���ȖʕύX
				SetSweepCtrl(Si12, Si1, Si2, Si3, Spt, Sis, Spd, Siz);	// �|���Ȗʐ���_�ϊ�
			}
			RedoSetSurface(Si1, Si2, Si3, Si4, Spd);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			TrackerCLine(Si13, Si11, Si1, Si2, Spd, Si7, Si8,
                         &Si6, Spw, Sic);								// �_���ׯ���擾
			for(i=0; i<Si6; i++) {
				Sic[i] = (Siw[i]) ? 4 : Sic[i];							// �w��_�F�ύX
			}
			DispParsTracker(0, Si21, Spw, Sic, Si6, Spx, Six, Si10);	// �����}�ׯ���\��
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
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
			}
		} else if(Sphase==1) {											// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker1(plane, box, Spw, Si6, Spx, Six,
                                 Si10, &Si21, &i1, Siy);				// �ׯ����ׯ��1
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickLineMove(Si13, i1, Siy, Si11, Si1, Si2, Si3,
                                 Si4, m_ObjCWgt, Si5, Si7, Si8,
                                 m_CurInp, Si12, &Si6, Sop2, Spa, Spb,
                                 Sps, Spw, Sia, Sib, Sic, Sis, Siw);	// ����_��ړ� ��I��
					SetParsTracker(0, &Si21, 0, Siw, Spw, Sic,
                                   Si6, Spx, Six, &Si10);				// �����}�ׯ���ݒ�
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
					Si23 = 1;
				}
			}
		} else if(Sphase==2) {											// <����_�ړ��̏ꍇ>
			if(Si9==1) {												// �ړ��ς݂̏ꍇ��
				for(i=0; i<Si5; i++) {									// �����ޯ̧�ύX
					Spa[i] = Spc[i], Spb[i] = Spd[i];
				}
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i];									// �ׯ���ۑ�
				}
				if(!Si11) {
					for(i=0; i<Si1+Si2; i++) {
						Sps[i] = Spt[i];
					}
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
				Si15 = plane;											// �Ώ̎��w�蕽��
				i = (flg&MK_CONTROL) ? 1 : 0;							// �������w��
				Si16 = (flg&MK_SHIFT) ? 1 : 0;							// ���������]
				SetSymmetryLine(1, Si14, Spnt2, Si15, i, 0,
                                &Si17, &Si18);                          // �Ώ̎��ݒ�
			}
		} else {														// <����_�w��̏ꍇ>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			BaseCursor = pnt;											// �����ʒu�ۑ�
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			HitCtrlTracker1(flg, point, plane, ms, pnt, Spw, Si6,
							Spx, Six, Si10, &Si21, &Si22, &i1, Siy);	// �S�ׯ������1
			if(Si22) {
				return;
			}
			if(!m_CurInp&&!m_CtlInp&&i1==1&&Siw[Siy[0]]==1&&plane!=3) {	// <����_�ړ��̏ꍇ>
				Spnt1 = Spw[Siy[0]];									// �ړ����_�ۑ�
				Sphase = 2, Si9 = 0;
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					Sphase = 1;
					CInputBtnDlg6 dlg(3);
					dlg.SetParam3(Sop1, Sop2, Si13, Si11, Si21, Si1,
                                  Si2, Si3, Si4, m_ObjCWgt, Si5, Si7,
                                  Si8, Si10, Si6, Si12, &Si9, Si14,
                                  Si17, Si18, Si16, Spnt1, Spnt2,
                                  Spa, Spb, Spc, Spd, Sps, Spt, Spw,
                                  Spx, Spy, Spz, Sia, Sib, Sic,
                                  Sis, Siw, Six, Tens, m_LMoveAdd);		// ���Ұ��ݒ�3
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						if(Si9==1) {									// �ړ��ς݂̏ꍇ��
							for(i=0; i<Si5; i++) {						// �����ޯ̧�ύX
								Spa[i] = Spc[i], Spb[i] = Spd[i];
							}
							if(!Si11) {
								for(i=0; i<Si1+Si2; i++) {
									Sps[i] = Spt[i];
								}
							}
						}
					}
					dlg.GetParam(&m_LMoveAdd);							// ���Ұ��擾
					if(Si11) {											// <�ʏ�Ȗ�>
						RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);	// �Ȗʐ���
						CalcSurfacePoint(Si13, 1, 0, Si1, Si2, Si3,
                                         Si4, m_ObjCWgt, Sia, Spa, Spb);// ����_=>�ʉߓ_�ϊ�
					} else {											// <�|���Ȗ�>
						RedoChgSweepSurf(Si1, Si2, Si3, Si4, Sps, Sis);	// �|���ȖʕύX
						SetSweepCtrl(Si12, Si1, Si2, Si3, Sps,
                                     Sis, Spb, Siz);					// �|���Ȗʐ���_�ϊ�
					}
					RedoSetSurface(Si1, Si2, Si3, Si4, Spb);			// �Ȗʐ���_����
					for(i=0; i<Si5; i++) {
						GetVtx(Sop2, i, &Spb[i]);						// ���̕ҏW�㐧��_�擾
					}
					TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7,
                                 Si8, &Si6, Spw, Sic);					// �_���ׯ���擾
					for(i=0; i<Si6; i++) {
						Spz[i] = Spw[i];								// �ׯ���ۑ�
						Sic[i] = (Siw[i]) ? 4 : Sic[i];					// �w��_�F�ύX
					}
					DispParsTracker(0, Si21, Spw, Sic, Si6, Spx,
                                    Six, Si10);							// �����}�ׯ���\��
					ObjectShading(Sop1);								// ����ިݸ�ؽēo�^
				}
			} else if(i1>0) {											// <����_�I���̏ꍇ>
				if(!m_CtlInp) {
					for(i=0; i<Si6; i++) {
						Siw[i] = 0;										// �w���׸޸ر
					}
					for(i=0; i<Si5; i++) {
						Sib[i] = 0;										// ����_�Ώ��׸޸ر
					}
				}
				PickLineMove(Si13, i1, Siy, Si11, Si1, Si2, Si3,
                             Si4, m_ObjCWgt, Si5, Si7, Si8, m_CurInp,
                             Si12, &Si6, Sop2, Spa, Spb, Sps, Spw,
                             Sia, Sib, Sic, Sis, Siw);					// ����_��ړ� ��I��
				SetParsTracker(0, &Si21, 0, Siw, Spw, Sic, Si6,
                               Spx, Six, &Si10);						// �����}�ׯ���ݒ�
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				Si23 = 1;
			} else if(!m_CtlInp) {										// <�ׯ���w�薳�Œǉ���>
				for(i=0; i<Si6; i++) {
					Siw[i] = 0;											// �w���׸޸ر
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// ����_�Ώ��׸޸ر
				}
				Si21 = 0, Si10 = 0;
				InitLineNo(Sop2);										// ײݔԍ�������
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==11) {													// <<<DEL(�폜)>>>
		for(i=0, i1=0; i<Si6; i++) {
			if(Siw[i]==1) {
				i1++;													// �폜�Ώ۶���
			}
		}
		if(i1>0) {
			i1 = Si2, i2 = Si2, i3 = Si1;								// U/V�����L���_��m�F
			i1 = (Si7>0) ? i1 - 1 : i1;
			i2 = (Si7>0) ? i2 - 1 : i2;
			i1 = (Si7>2) ? i1 - 1 : i1;
			i2 = (Si7>2) ? i2 - 1 : i2;
			i3 = (Si8>0) ? i3 - 1 : i3;
			i3 = (Si8>2) ? i3 - 1 : i3;
			for(i=0; i<i1; i++) {
				i2 = (Siw[i]==1) ? i2 - 1 : i2;							// V�폜�Ώ۶���
			}
			for(i=i1; i<Si6; i++) {
				i3 = (Siw[i]==1) ? i3 - 1 : i3;							// U�폜�Ώ۶���
			}
			if(i3<2||i2<3) {											// <����_�����ȉ�>
				AfxMessageBox(IDS_FEW_CTRLP);							// �װү����
			} else {													// <����_��������>
				InitLineNo(NULL);										// ײݔԍ�������
				if(Si11) {												// <�ʏ�Ȗ�>
					DeleteCLine(Si6, Siw, Si3, Si4, Si7, Si8,
                                &Si1, &Si2, Spa, Sia, Tens);			// �Ώې���_��폜
					RedoChgSurface(Si1, Si2, Si3, Si4, Spa, Sia);		// �ȖʕύX
					CalcSurfacePoint(Si13, 1, 0, Si1, Si2, Si3,
                                     Si4, m_ObjCWgt, Sia, Spa, Spb);	// ����_=>�ʉߓ_�ϊ�
				} else {												// <�|���Ȗ�>
					DeleteSweep(Si6, Siw, Si3, Si4, &Si1, &Si2,
                                Sps, Sis);								// �|������_��폜
					RedoChgSweepSurf(Si1, Si2, Si3, Si4, Sps, Sis);		// �|���ȖʕύX
					SetSweepCtrl(Si12, Si1, Si2, Si3, Sps, Sis,
                                 Spb, Siz);								// �|���Ȗʐ���_�ϊ�
				}
				Si5 = Si1*Si2;											// ������_��
				RedoSetSurface(Si1, Si2, Si3, Si4, Spb);				// �Ȗʐ���_����
				for(i=0; i<Si5; i++) {
					GetVtx(Sop2,i,&Spb[i]);								// ���̕ҏW�㐧��_�擾
				}
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i], Siw[i] = 0;						// �ׯ���ۑ�/�w���׸޸ر
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// ����_�Ώ��׸޸ر
				}
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				Display(0);												// �ĕ`��
			}
			Sphase = 1;
		}
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si14 = (m_FChar&0x8) ? 2 : 0;								// [F8]:����Ӱ��:2���Ώ�
			Si14 = (m_FChar&0x4) ? 1 : Si14;							// [F7]:����Ӱ��:1���Ώ�
			Si13 = (m_FChar&0x10) ? 1 : 0;								// [F9]:�ʉߓ_Ӱ��
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==2&&cmp!=NULL) {									// <�Ȗʂ̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				GetSurfCmd2(cmp, i3, &Si11, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si12, Sps, Sis);							// �Ȗʺ���ގ擾2
				if(!Si11&&Si13) {										// �|���Ȗ�&�ʉߓ_Ӱ�ނ�
					AfxMessageBox(IDS_ERR_SWEEP);						// ү����
					Si13 = 0;											// �|���Ȗʂ͐���_Ӱ��
				}
				if(!Si11&&!Si20) {										// �|���Ȗ�&������:OFF��
					AfxMessageBox(IDS_SWEEPSURF);						// ү����
					Si20 = 1;											// �������`��:ON
				}
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				if(Si11) {												// <�ʏ�Ȗ�>
					ExecSurface(Si13, Sop1, Si1, Si2, Si3, Si4,
                                m_ObjCRpt, m_ObjCWgt, Si5, Spa, Sia,
                                Spb, &Sop2);							// �ʏ�Ȗʎ��s
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si7, &Si8);	// ����_��m�F
				} else {												// <�|���Ȗ�>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si12, Sps, Sis, Spa,
                                  Spb, &Sop2);							// �|���Ȗʎ��s
				}
				TrackerCLine(Si13, Si11, Si1, Si2, Spb, Si7, Si8,
                             &Si6, Spw, Sic);							// �_���ׯ���擾
				for(i=0; i<Si6; i++) {
					Spz[i] = Spw[i], Siw[i] = 0;						// �ׯ���ۑ�/�w���׸޸ر
				}
				for(i=0; i<Si5; i++) {
					Sib[i] = 0;											// ����_�Ώ��׸޸ر
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
				if(Si14==0) {											// <�ʏ�Ӱ�ނ̏ꍇ>
					Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 0);			// �ׯ��1�ݒ�
					Sphase = 1;											// ����_�w���
				}
				Display(0);												// �ĕ`��
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// �װү����
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
void CJcad3GlbView::MouseLineInsert(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< ����_��}��
/******************************************************************/
{
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
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// �ׯ����ׯ��2
				if(i1>0) {												// <����_�I���̏ꍇ>
					if(!CheckLineCopy(Si9, i1, Siy, Si1, Si2, Si3,
                                      Si4, Si7, &Si11, &m_SecBase4)) {	// ����_�񕡎�����
						PickLineInsert(Si9, i1, Siy, &Si1, &Si2, Si3,
                                       Si4, &Si5, Si6, m_CurInp, Si10,
                                       &Si11, m_SecBase4, Sop2, Spnt1,
                                       Spa, Spb, Sps, Sia, Sis);		// ����_��}�� ��I��
						Si23 = 1;
						ObjectShading(Sop1);							// ����ިݸ�ؽēo�^
					}
					TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11,
                                   Spb, &Si8, Spw, Sib);				// �_���ׯ���擾
					Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);			// �ׯ��1�ݒ�
				}
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, 0,
                        &Si22, &i1, Siy);								// �S�ׯ������2
		if(Si22) {
			return;
		}
		if(i1>0) {														// <�ׯ���w�肠��>
			if(!CheckLineCopy(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                              Si7, &Si11, &m_SecBase4)) {				// ����_�񕡎�����
				PickLineInsert(Si9, i1, Siy, &Si1, &Si2, Si3, Si4, &Si5,
                               Si6, m_CurInp, Si10, &Si11, m_SecBase4,
                               Sop2, Spnt1, Spa, Spb, Sps, Sia, Sis);	// ����_��}�� ��I��
				Si23 = 1;
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
			}
			TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                           &Si8, Spw, Sib);								// �_���ׯ���擾
			Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);					// �ׯ��1�ݒ�
		}
		Display(0);														// �ĕ`��
	} else if(ms==11) {													// <<<DEL(�폜)>>>
		Si11 = -1;														// ���ʒf��Noؾ��
		TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                       &Si8, Spw, Sib);									// �_���ׯ���擾
		Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);						// �ׯ��1�ݒ�
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			Sop1 = NULL, Sop2 = NULL;
			Si11 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
			}
			Si24 = (m_FChar&0x80) ? 1 : 0;								// [F12]:�͈͌���
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
                BaseCmdPtr(1, Sop1, &cmp);                              // �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if(i3/10==2&&cmp!=NULL) {									// <�Ȗʂ̏ꍇ>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				GetSurfCmd2(cmp, i3, &Si9, &Si1, &Si2, &Si3, &Si4,
                            &m_ObjCRpt, &m_ObjCWgt, &Si5, Spa, Sia,
                            &Si10, Sps, Sis);							// �Ȗʺ���ގ擾2
				if(!Si9&&!Si20) {										// �|���Ȗ�&������:OFF��
					AfxMessageBox(IDS_SWEEPSURF);						// ү����
					Si20 = 1;											// �������`��:ON
				}
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				if(Si20) {
					DetachCmd(Sop1, cmp);								// ����ވꎞ�ؗ�
				}
				if(Si9) {												// <�ʏ�Ȗ�>
					ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);	// �ʏ�Ȗʎ��s
					CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);	// ����_��m�F
				} else {												// <�|���Ȗ�>
					ExecSweepSurf(Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                                  m_ObjCWgt, Si5, Si10, Sps, Sis, Spa,
                                  Spb, &Sop2);							// �|���Ȗʎ��s
				}
				TrackerCInsert(Si9, Si3, Si4, Si1, Si2, Si11, Spb,
                               &Si8, Spw, Sib);							// �_���ׯ���擾
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
				Tracker1.Setup(Spw, Sib, NULL, Si8, 4, 7);				// �ׯ��1�ݒ�
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				OffUndoRedo();											// Undo/Redo�s��
				Display(0);												// �ĕ`��
				if(Si6==0) {											// <U�����Ȃ�>
					CalcCent(Spa, Si5, &Spnt1);
				} else if(Si6==1) {										// <U�n�_����>
					Spnt1 = Spa[0];
				} else if(Si6==2) {										// <U�I�_����>
					Spnt1 = Spa[Si5-1];
				} else {												// <U�n�_��I�_����>
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] = (Spa[0].p[i]+Spa[Si5-1].p[i])/2.0;
					}
				}
				Sphase = 1;
			} else {													// <�ҏW����:NG>
				AfxMessageBox(IDS_ERR_SEL4);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
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
		if(ms==4) {														// <ϳ��E�t����>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, 0,
	                        &Si22, &i1, Siy);							// �S�ׯ������2
			if(i1==1) {													// <�L���Ȑؒf�ʒu�̏ꍇ>
				if(AfxMessageBox(IDS_DIVIDE_CTRLP, MB_YESNO)!=IDYES) {	// �m�F:NG
					i1 = 0;
				}
			}
			if(i1==1) {													// <�ׯ���w�肠��>
				Si23 = (PickLineDivide(Si9, i1, Siy, Si1, Si2, Si3, Si4,
                                       m_ObjCRpt, m_ObjCWgt, Si10, Sop1,
                                       Spa, Sps, Sia, Sis)) ? 1 : Si23;	// ����_�񕪊�
			}
		}
		Tracker1.Reset(1);												// �ׯ��1����
		SetRenderMode(0);												// ܲ԰�ڰ�Ӱ��
		ResetSelNo();													// ���̑I��Noؾ��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 0);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
		SetDataUse();													// �ް��g�p���ݒ�
		if(!Si23) {
			Undo(gno);													// �ҏW�Ȃ���Undo
		}
		Display(0);														// �ĕ`��
	}
}

/******************************************************************/
void CJcad3GlbView::MouseLineRotate(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< ����_���]
/******************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt, pt[2];
	VECTYPE  vec;
	BOXTYPE  box;
	CPoint   pot;
	int      i, i1, i2, i3, cl[2], ot[2], gno[32];
	double   th;

	if((ms==2||ms==3)&&Si22) {											// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==1) {											// ��_�p�x
			if(abs(Si11-point.x)>DRAGMIN||abs(Si12-point.y)>DRAGMIN) {
				th = atan2((double)(point.y-Si12),
                           (double)(point.x-Si11));
				th = (th<-10||th>10) ? 0.0 : th;
				Sd1 = th*180.0/PI, Sphase = 2;
			}
		} else if(Sphase>=2) {											// ���ݶ��وʒu�p�x
			th = atan2((double)(point.y-Si12), (double)(point.x-Si11));
			th = (th<-10||th>10) ? 0.0 : th;
			th = th*180.0/PI;											// ��]�p�x
			th = th-Sd1;
			th = (th>180.0) ? -(360.0-th) : ((th<-180.0)?360.0+th:th);
			TwistCLine(Spy, Sva, th, Si10, Sib, Si8, Sop1, Si1,
                       Si2, Si3, Si7, Spa, Spb, Spc, Tens);				// ����_�Ђ˂�
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ���
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// �_���ׯ���擾
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// �w���ς�
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);					// �ׯ��1�ݒ�
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			m_pNumDisp->SetParam(th);									// �Ђ˂�p�x�\��
			Display(0);													// �ĕ`��
			Sphase = 3;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// �ׯ����ׯ��2
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// ����_���] ��I��
					if(Si10!=-1) {										// �f�ʎw���ς݂���
						Sbase = Spy[Si10];								// �w��f�ʒ��S�_
						for(i=0; i<DIM; i++) {
							Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;	// ��]�p�ׯ��
						}
						Siw[Si8] = 0, Si9 = 1, Si23 = 1;
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[2]=Sbase.p[2];
						cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
						Tracker2.Setup(pt, cl, ot, 2, 0, 1);			// �ׯ��2�ݒ�
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[0]=Sbase.p[0];
						Tracker3.Setup(pt, cl, ot, 2, 1, 1);			// �ׯ��3�ݒ�
						pt[0]=Sbase, pt[1]=Spw[Si8], pt[1].p[1]=Sbase.p[1];
						Tracker4.Setup(pt, cl, ot, 2, 2, 1);			// �ׯ��4�ݒ�
					} else {											// �f�ʎw���ς݂Ȃ�
						Si9 = 0, Si10 = -1;
						Tracker2.Reset(0);								// �ׯ��2����
						Tracker3.Reset(0);								// �ׯ��3����
						Tracker4.Reset(0);								// �ׯ��4����
					}
					Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);		// �ׯ��1�ݒ�
				}
			}
		} else if(Sphase==3) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// �����ޯ̧�ύX
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);			// ��_�擾
			}
			Sbase = Spy[Si10];											// �w��f�ʒ��S�_
			for(i=0; i<DIM; i++) {
				Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;				// ��]�p�ׯ��
			}
			Siw[Si8] = 0, Si9 = 1;
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[2] = Sbase.p[2];
			cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
			Tracker2.Setup(pt, cl, ot, 2, 0, 1);						// �ׯ��2�ݒ�
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[0] = Sbase.p[0];
			Tracker3.Setup(pt, cl, ot, 2, 1, 1);						// �ׯ��3�ݒ�
			pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[1] = Sbase.p[1];
			Tracker4.Setup(pt, cl, ot, 2, 2, 1);						// �ׯ��4�ݒ�
		}
		Sphase = 0;
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8, Si9,
                        &Si22, &i1, Siy);								// �S�ׯ������2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <�f���ׯ���w�肠��>
			if(!m_CtlInp) {
				for(i=0;i<Si8;i++) {
					Sib[i] = 0, Siw[i] = 2;								// �w���׸޸ر
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// ����_���] ��I��
			if(Si10!=-1) {												// �f�ʎw���ς݂���
				Sbase = Spy[Si10];										// �w��f�ʒ��S�_
				for(i=0; i<DIM; i++) {
					Spw[Si8].p[i] = Sbase.p[i]+m_Fovy/14000.0;			// ��]�p�ׯ��
				}
				Siw[Si8] = 0, Si9 = 1, Si23 = 1;
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[2] = Sbase.p[2];
				cl[0] = 0, cl[1] = 0, ot[0] = 0, ot[1] = 8;
				Tracker2.Setup(pt, cl, ot, 2, 0, 1);					// �ׯ��2�ݒ�
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[0] = Sbase.p[0];
				Tracker3.Setup(pt, cl, ot, 2, 1, 1);					// �ׯ��3�ݒ�
				pt[0] = Sbase, pt[1] = Spw[Si8], pt[1].p[1] = Sbase.p[1];
				Tracker4.Setup(pt, cl, ot, 2, 2, 1);					// �ׯ��4�ݒ�
			} else {													// �f�ʎw���ς݂Ȃ�
				Si9 = 0, Si10 = -1;
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Tracker4.Reset(0);										// �ׯ��4����
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
		} else if(i1==1) {												// <��]�ׯ���w�肠��>
			i = (plane-1<0) ? 2 : plane-1;								// �@���������W
			vec.p[0]=0.0, vec.p[1]=0.0, vec.p[2]=0.0, vec.p[i]=-1.0;	// �@���޸�ِݒ�
			for(i=0; i<Si1; i++) {
				Sva[i] = vec;
			}
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				Tracker1.Reset(0);										// �ׯ��1����
				if(plane==0) {
					Tracker3.Reset(0);									// �ׯ��3����
					Tracker4.Reset(0);									// �ׯ��4����
				} else if(plane==1) {
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker4.Reset(0);									// �ׯ��4����
				} else {
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
				}
				Display(0);												// �ĕ`��
				CInputBtnDlg3 dlg(5);
				dlg.SetParam2(Si1, Si2, Si3, Si4, Si7, Si10, Sib,
                              Si8, Sop1, Spy, Sva, Spa, Sia,
                              Spb, Spc, Tens, m_LRotateAdd);            // ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_LRotateAdd);							// ���Ұ��擾
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// �����ޯ̧�ύX
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// ��_�擾
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				Si9 = 0, Si10 = -1;
				InitLineNo(Sop2);										// ײݔԍ�������
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Tracker4.Reset(0);										// �ׯ��4����
			} else {													// <ϳ����͂̏ꍇ>
				CalcWorldScreen(&pot, Sbase, plane);
				Si11 = pot.x, Si12 = pot.y;								// ���S�_��ذݍ��W�ۑ�
				Tracker1.Setup(Spw, Siw, NULL, Si8, 4, 7);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Tracker4.Reset(0);										// �ׯ��4����
				Sphase = 1;
			}
		} else if(!m_CtlInp) {											// <�f�ʎw�薳�̏ꍇ>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// �w���׸޸ر
			}
			Si9 = 0, Si10 = -1;
			InitLineNo(Sop2);											// ײݔԍ�������
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
			Tracker2.Reset(0);											// �ׯ��2����
			Tracker3.Reset(0);											// �ׯ��3����
			Tracker4.Reset(0);											// �ׯ��4����
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
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
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// ����_��m�F
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &vec);		// ��_�擾
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
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
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				OffUndoRedo();											// Undo/Redo�s��
				SetNumDisp(2);											// ���l�\����������
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
		Tracker3.Reset(1);												// �ׯ��3����
		Tracker4.Reset(1);												// �ׯ��4����
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

/*****************************************************************/
void CJcad3GlbView::MouseLineTwist(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< �Ђ˂�
/*****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   th, l11, l12, l21, l22;

	if((ms==2||ms==3)&&Si22) {											// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==1) {											// �Ђ˂�p�x�Z�o
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			i1 = plane;													// �X�V�Ώۍ��W
			i2 = (i1+1>2) ? 0 : i1+1;
			l11 = Sbase.p[i1]-Spy[Si10].p[i1];							// �p�x�Z�o
			l12 = Sbase.p[i2]-Spy[Si10].p[i2];
			l21 = pnt.p[i1]-Spy[Si10].p[i1];
			l22 = pnt.p[i2]-Spy[Si10].p[i2];
			l11 = atan2(l12, l11);
			l11 = (l11<-10||l11>10) ? 0.0 : l11;
			l11 = l11*180.0/PI;
			l21 = atan2(l22, l21);
			l21 = (l21<-10||l21>10) ? 0.0 : l21;
			l21 = l21*180.0/PI;
			th = -l21+l11;
			Sd2 = (th-Sd1<-180.0)?Sd2+360:((th-Sd1>180.0)?Sd2-360:Sd2);	// �A����]�␳�l�C��
			Sd1 = th;													// �����]�p�x�ۑ�
			th += Sd2;													// �␳�l���Z
			TwistCLine(Spy, Sva, th, Si10, Sib, Si8, Sop1, Si1,
                       Si2, Si3, Si7, Spa, Spb, Spc, Tens);				// ����_�Ђ˂�
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ���
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// �_���ׯ���擾
			i1 = (Si7%2) ? Si10-1 : Si10;
			TrackerVLine(Si1, Si2, Spd, Si7, i1, Si8, &Si9, Spw, Siw);	// �_���ׯ���擾
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// �w���ς�
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			m_pNumDisp->SetParam(th);									// �Ђ˂�p�x�\��
			Display(0);													// �ĕ`��
			Si11 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// �ׯ����ׯ��2
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// ����_���] ��I��
					if(Si10!=-1) {										// �f�ʎw���ς݂���
						i2 = (Si7%2) ? Si10-1 : Si10;
						TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                                     &Si9, Spw, Siw);					// �_���ׯ���擾
						Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);	// �ׯ��1�ݒ�
						Si23 = 1;
					} else {											// �f�ʎw���ς݂Ȃ�
						Si9 = 0, Si10 = -1;
					}
				}
			}
		} else if(Sphase==1&&Si11==1) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// �����ޯ̧�ύX
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);		// ��_�擾
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8,
                        Si9, &Si22, &i1, Siy);							// �S�ׯ������2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <�f���ׯ���w�肠��>
			if(!m_CtlInp) {
				for(i=0;i<Si8;i++) {
					Sib[i] = 0, Siw[i] = 2;								// �w���׸޸ر
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// ����_���] ��I��
			if(Si10!=-1) {												// �f�ʎw���ς݂���
				i2 = (Si7%2) ? Si10-1 : Si10;
				TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                             &Si9, Spw, Siw);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// �ׯ��1�ݒ�
				Si23 = 1;
			} else {													// �f�ʎw���ς݂Ȃ�
				Si9 = 0, Si10 = -1;
			}
		} else if(i1==1) {												// <��]�ׯ���w�肠��>
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				Tracker1.Reset(0);										// �ׯ��1����
				Display(0);												// �ĕ`��
				CInputBtnDlg3 dlg(2);
				dlg.SetParam2(Si1, Si2, Si3, Si4, Si7, Si10, Sib,
                              Si8, Sop1, Spy, Sva, Spa, Sia,
                              Spb, Spc, Tens, m_LTwistAdd);             // ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_LTwistAdd);								// ���Ұ��擾
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// �����ޯ̧�ύX
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// ��_�擾
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				Si9 = 0, Si10 = -1;
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// �ׯ��1�ݒ�
			} else {													// <ϳ����͂̏ꍇ>
				Sbase = Spw[Siy[0]];									// V���W�ʒu�ۑ�
				Sphase = 1, Si11 = 0, Sd1 = 0.0, Sd2 = 0.0;
			}
		} else if(!m_CtlInp) {											// <�f�ʎw�薳�̏ꍇ>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// �w���׸޸ر
			}
			Si9 = 0, Si10 = -1;
			InitLineNo(Sop2);											// ײݔԍ�������
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
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
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// ����_��m�F
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// ��_�擾
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
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
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				OffUndoRedo();											// Undo/Redo�s��
				SetNumDisp(2);											// ���l�\����������
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

/*****************************************************************/
void CJcad3GlbView::MouseLineScale(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< �g��/�k��
/*****************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	BOXTYPE  box;
	int      i, i1, i2, i3, gno[32];
	double   scl, l11, l12, l21, l22;

	if((ms==2||ms==3)&&Si22) {											// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		Si22 = (ms==3) ? 0 : Si22;
		return;
	}
	if(ms==2) {															// <<<ϳ����t���>>>
		if(flg&MK_LBUTTON&&Sphase==0&&plane!=3) {						// ��ׯ��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			DrawBox(BaseCursor, pnt, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==1) {											// �{���Z�o
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			i1 = plane;													// �X�V�Ώۍ��W
			i2 = (i1+1>2) ? 0 : i1+1;
			l11 = Sbase.p[i1]-Spy[Si10].p[i1];							// �{���Z�o
			l12 = Sbase.p[i2]-Spy[Si10].p[i2];
			l21 = pnt.p[i1]-Spy[Si10].p[i1];
			l22 = pnt.p[i2]-Spy[Si10].p[i2];
			l11 = sqrt(l11*l11+l12*l12);
			l21 = sqrt(l21*l21+l22*l22);
			scl = l21/l11;
            ScaleCLine(Spy, Sva, m_SecBase1a, m_SecBase1b, scl,
                       Si10, Sib, Si8, Sop1, Si1, Si2,
                       Si3, Si7, Spa, Spb, Spc, Tens);                  // ����_�g��/�k��
			RedoChgSurface(Si1, Si2, Si3, Si4, Spc, Sia);				// �Ȗʐ���
			RedoSetSurface(Si1, Si2, Si3, Si4, Spc);					// �Ȗʐ���_����
			for(i=0; i<Si5; i++) {
				GetVtx(Sop2, i, &Spd[i]);								// ���̕ҏW�㐧��_�擾
			}
			TrackerULine(0, Si1, Si2, Spd, Si6, Si7, &Si8, Spw, Siw);	// �_���ׯ���擾
			i1 = (Si7%2) ? Si10-1 : Si10;
			TrackerVLine(Si1, Si2, Spd, Si7, i1, Si8, &Si9, Spw, Siw);	// �_���ׯ���擾
			for(i=0; i<Si8; i++) {
				Siw[i] = (Sib[i]) ? 4 : Siw[i];							// �w���ς�
			}
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
			ObjectShading(Sop1);										// ����ިݸ�ؽēo�^
			m_pNumDisp->SetParam(scl);									// �g��/�k���{���\��
			Display(0);													// �ĕ`��
			Si13 = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==0) {													// <�ׯ���I���̏ꍇ>
			CheckCtrlKey2(flg);											// ���䷰�m�F(����_�I��)
			if(abs(BasePoint.x-point.x)>=DRAGMIN||
               abs(BasePoint.y-point.y)>=DRAGMIN) {
				CalcScreenWorld(1, &pnt, point, plane);					// ���W�_�擾
				box.bp[0] = BaseCursor, box.bp[1] = pnt;
				DragCtrlTracker2(plane, box, Spw, Si8, &i1, Siy);		// �ׯ����ׯ��2
				if(i1>0) {												// <����_�I���̏ꍇ>
					PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib,
                                 Siw, &Si10);							// ����_���] ��I��
					if(Si10!=-1) {										// �f�ʎw���ς݂���
						if(!Si14) {
                            Si14 = InitCScale(&m_SecBase1a,
                                              &m_SecBase1b);            // �g��/�k����������
						}
						if(Si14) {										// <���������L��>
							Si23 = 1;									// ������
							i2 = (Si7%2) ? Si10-1 : Si10;
							TrackerVLine(Si1, Si2, Spb, Si7, i2,
                                         Si8, &Si9, Spw, Siw);			// �_���ׯ���擾
							Tracker1.Setup(Spw, Siw, NULL,
                                           Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
							Display(0);									// �ĕ`��
						} else {										// �����͒f�ʖ��w��
							Si9 = 0, Si10 = -1;
						}
					} else {											// �f�ʎw���ςȂ�
						Si9 = 0, Si10 = -1, Si14 = 0;
					}
				}
			}
		} else if(Sphase==1&&Si13==1) {
			for(i=0; i<Si5; i++) {
				Spa[i] = Spc[i], Spb[i] = Spd[i];						// �����ޯ̧�ύX
			}
			for(i=0; i<Si1; i++) {
				GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);		// ��_�擾
			}
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		BaseCursor = pnt;												// �����ʒu�ۑ�
		CheckCtrlKey2(flg);												// ���䷰�m�F(����_�I��)
		HitCtrlTracker2(flg, point, plane, ms, pnt, Spw, Si8,
                        Si9, &Si22, &i1, Siy);							// �S�ׯ������2
		if(Si22) {
			return;
		}
		if(i1>1||(i1==1&&Siy[0]<Si8)) {									// <�f���ׯ���w�肠��>
			if(!m_CtlInp) {
				for(i=0; i<Si8; i++) {
					Sib[i] = 0, Siw[i] = 2;								// �w���׸޸ر
				}
				Si10 = -1;
			}
			PickSurfLine(i1, Siy, Si7, Si8, Sop2, Sib, Siw, &Si10);		// ����_���] ��I��
			if(Si10!=-1) {												// �f�ʎw���ς݂���
				if(!Si14) {
                    Si14 = InitCScale(&m_SecBase1a, &m_SecBase1b);      // �g��/�k����������
				}
				if(Si14) {												// <���������L��>
					Si23 = 1;											// ������
					i2 = (Si7%2) ? Si10-1 : Si10;
					TrackerVLine(Si1, Si2, Spb, Si7, i2, Si8,
                                 &Si9, Spw, Siw);						// �_���ׯ���擾
					Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);		// �ׯ��1�ݒ�
					Display(0);											// �ĕ`��
				} else {												// �����͒f�ʖ��w��
					Si9 = 0, Si10 = -1;
				}
			} else {													// �f�ʎw���ςȂ�
				Si9 = 0, Si10 = -1, Si14 = 0;
			}
		} else if(i1==1) {												// <��]�ׯ���w�肠��>
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				Tracker1.Reset(0);										// �ׯ��1����
				Display(0);												// �ĕ`��
                CInputBtnDlg3 dlg(3);
                dlg.SetParam3(Si1, Si2, Si3, Si4, Si7, m_SecBase1a,
                              m_SecBase1b, Si10, Sib, Si8, Sop1,
                              Spy, Sva, Spa, Sia, Spb,
                              Spc, Tens, m_LScaleAdd);                  // ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_LScaleAdd);								// ���Ұ��擾
				for(i=0; i<Si5; i++) {
					Spa[i] = Spc[i];									// �����ޯ̧�ύX
					GetVtx(Sop2, i, &Spb[i]);							// ���̕ҏW�㐧��_�擾
				}
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// ��_�擾
				}
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				Si9 = 0, Si10 = -1, Si14 = 0;
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
                             &Si8, Spw, Siw);							// �_���ׯ���擾
				Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);			// �ׯ��1�ݒ�
			} else {													// <ϳ����͂̏ꍇ>
				Sbase = Spw[Siy[0]];									// V���W�ʒu�ۑ�
				Sphase = 1, Si13 = 0;
			}
		} else if(!m_CtlInp) {											// <�f�ʎw�薳�̏ꍇ>
			for(i=0; i<Si8; i++) {
				Sib[i] = 0, Siw[i] = 2;									// �w���׸޸ر
			}
			Si9 = 0, Si10 = -1, Si14 = 0;
			InitLineNo(Sop2);											// ײݔԍ�������
			Tracker1.Setup(Spw, Siw, NULL, Si8+Si9, 4, 7);				// �ׯ��1�ݒ�
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Sop1 = NULL, Sop2 = NULL;
			Si9 = 0, Si10 = -1, Si14 = 0, Si22 = 0, Si23 = 0;
			Si20 = m_ShpEnv;											// �������`����
			m_Shading = m_ShdEnv;										// ���ިݸޕ\�����
			if(m_FChar&0x1) {											// [F5]:�������`�󔽓]
				Si20 = (Si20) ? 0 : 1;
			}
			if(m_FChar&0x2) {											// [F6]:���ިݸޕ\�����]
				m_Shading = (m_Shading) ? 0 : 1;
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
				ExecSurface(0, Sop1, Si1, Si2, Si3, Si4, m_ObjCRpt,
                            m_ObjCWgt, Si5, Spa, Sia, Spb, &Sop2);		// �ʏ�Ȗʎ��s
				CheckCLine(Si3, Si4, Si1, Si2, Spa, &Si6, &Si7);		// ����_��m�F
				for(i=0; i<Si1; i++) {
					GetBasePoint(Si1, Si2, Spb, i, &Spy[i], &Sva[i]);	// ��_�擾
				}
				TrackerULine(0, Si1, Si2, Spb, Si6, Si7,
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
				for(i=0; i<Si8; i++) {
					Sib[i] = 0;											// �w���׸޸ر
				}
				ObjectShading(Sop1);									// ����ިݸ�ؽēo�^
				OffUndoRedo();											// Undo/Redo�s��
				SetNumDisp(2);											// ���l�\����������
				Display(0);												// �ĕ`��
				m_pNumDisp->SetParam(1.0);								// �g��/�k���{���\��
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
