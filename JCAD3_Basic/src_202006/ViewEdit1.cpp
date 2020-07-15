/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewEdit1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputBtnDlg.h"
#include "InputBtnDlg2.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "EditObject.h"
#include "NumFunc.h"

/*************************************************************/
void CJcad3GlbView::MouseSpace(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ��߰�[���_]
/*************************************************************/
{
	PNTTYPE pnt, pt1, pt2;
	BOXTYPE box;
	int     hit, i;
	double  th, l11, l12, l21, l22;

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
			ObjectSpace(Si1, Si4, Si3, Si2, 0, Sda[0]+Sdb[0],
                        Sda[1]+Sdb[1], Sda[2]+Sdb[2], Sda[3]+Sdb[3],
                        Sda[4]+Sdb[4], Sda[5]+Sdb[5], Sda[6]+Sdb[6],
                        Sda[7]+Sdb[7], Sda[8]+Sdb[8]);					// ��߰�
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
			Si3 = (m_FChar&0x4) ? 0 : 2;								// [F7]:��_-���̒��S�w��
			Si3 = (m_FChar&0x8) ? 1 : Si3;								// [F8]:��_-�ƯĒ��S�w��
			Si4 = (m_FChar&0x4) ? 0 : 2;								// [F7]:���S�_-���̒��S�w��
			Si4 = (m_FChar&0x8) ? 1 : Si4;								// [F8]:���S�_-�ƯĒ��S�w��
		}
		for(i=0; i<9; i++) {
			Sda[i] = 0.0, Sdb[i] = 0.0;
		}
		Sda[6] = 1.0, Sda[7] = 1.0, Sda[8] = 1.0;						// �{���̂ݏ����l��1.0
		if(m_NumInp!=0) {												// <���l���͂̏ꍇ>
			CInputBtnDlg7 dlg(1);
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

/************************************************************/
void CJcad3GlbView::MouseMove(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< �ړ�[���_]
/************************************************************/
{
	PNTTYPE pnt;
	int     i;
	double  d[3], dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ����t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			for(i=0; i<DIM; i++) {
				d[i] = pnt.p[i]-Sbase.p[i];								// ���̈ړ��ʎZ�o
			}
			if(flg&m_LimKey) {
				LimitValue(plane, d);									// ϳ����͐����l
			}
			ObjectMove(Sphase, m_CopyNum, Spnt1, Sscr1,
                       d[0], d[1], d[2]);								// ���̕��s�ړ�
			Sphase = (Sphase==0) ? 1 : Sphase;
			dd = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
			m_pNumDisp->SetParam(d[0], d[1], d[2], dd);					// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Sd1 = d[0], Sd2 = d[1], Sd3 = d[2];							// �ړ��ʕۑ�
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(0, &Sbase, point, plane);						// �ړ����_�̍��W�擾
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <2��ڈȍ~>
			EndMove(m_CopyNum, Spnt1, Si1, Sscr1, Sd1, Sd2, Sd3);		// ���s�ړ��I������
			Display(0);													// �ĕ`��
			m_CopyNum = 0, Sphase = 0;									// ���ʐ�/̪��޸ر
		}
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			if(m_CopyInp==2) {											// <<����Ӱ��:ON(�޲�۸ޕ\��)>>
				m_CopyNum  = m_ObjCopyNum;								// �����l�ݒ�
				Spnt1.p[0] = m_ObjXScale;
				Spnt1.p[1] = m_ObjYScale;
				Spnt1.p[2] = m_ObjZScale;
				Si1        = m_ObjSynFlg;
				Sscr1.p[0] = m_ObjXPoint;
				Sscr1.p[1] = m_ObjYPoint;
				Sscr1.p[2] = m_ObjZPoint;
				if(InputCopyNum(&m_CopyNum, &Spnt1, &Si1, &Sscr1)) {	// ���ʏ�����
					m_ObjCopyNum = m_CopyNum;							// �����l�ۑ�
					m_ObjXScale  = Spnt1.p[0];
					m_ObjYScale  = Spnt1.p[1];
					m_ObjZScale  = Spnt1.p[2];
					m_ObjSynFlg  = Si1;
					m_ObjXPoint  = Sscr1.p[0];
					m_ObjYPoint  = Sscr1.p[1];
					m_ObjZPoint  = Sscr1.p[2];
				} else {
					m_ObjCopyNum = 0;
				}
				if(m_CopyNum==0) {
					ExitMenu(1, 1);										// ���ʐ������ƭ��I��
				}
			} else if(m_CopyInp==1) {									//<<����Ӱ��:ON(�޲�۸ނȂ�)>>
				m_CopyNum = 1;											// ���ʐ�:1
				Si1 = 0;												// �ʒu���[�h:���[�h1
				for(i=0; i<DIM; i++) {
					Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;					// �{��:1.0�{/��_:����
				}
			}
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				CInputBtnDlg1 dlg(1);
				dlg.SetParam1(m_CopyNum, Spnt1, Si1, Sscr1, m_MoveAdd);	// ���Ұ��ݒ�
				dlg.DoModal();											// �����޲�۸ޕ\��
				dlg.GetParam(&m_MoveAdd);								// ���Ұ��擾
				m_CopyNum = 0;											// ���ʐ��ر
				ExitMenu(1, 1);											// �ƭ��I��
				EndJMenu1();											// �ƭ��I������
			}
		}
		Display(0);														// �ĕ`��
		SetNumDisp(2);													// ���l�\����������
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		ExitMenu(1, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/**************************************************************/
void CJcad3GlbView::MouseRotate(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< ��][���_]
/**************************************************************/
{
	int    i, col=0;
	double th;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&!m_NumInp) {									// ϳ����t�������̂ݗL��
			if(Sphase==1) {
				if(abs(Si2-point.x)>DRAGMIN||abs(Si3-point.y)>DRAGMIN) {// �p�x�Z�o��_�p�x
					th = atan2((double)(point.y-Si3),
                               (double)(point.x-Si2));
					th = (th<-10||th>10) ? 0.0 : th;
					Sd1 = th*180.0/PI;
					Sphase = 2;
				}
			} else if(Sphase>=2) {										// ���ݶ��وʒu�p�x
				th = atan2((double)(point.y-Si3),
                           (double)(point.x-Si2));
				th = (th<-10||th>10) ? 0.0 : th;
				th = th*180.0/PI;
				Sd2 = th - Sd1;											// ��]�p�x
				Sd2 = (Sd2>180.0)?-(360.0-Sd2):((Sd2<-180.0)?360.0+Sd2:Sd2);
				ObjectRotate(Sphase-2, m_CopyNum, Spnt1,
                             Sscr1, Sbase, plane, Sd2, 0.0);			// ���̉�]�ړ�
				m_pNumDisp->SetParam(Sd2);								// ��]�p�x�\��
				Display(0);												// �ĕ`��
				Sphase = 3;
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {
			CalcScreenWorld(0, &Sbase, BasePoint, plane);				// ��]���S�_���W�擾
			CalcWorldScreen(&BasePoint, Sbase, plane);
			Si2 = BasePoint.x, Si3 = BasePoint.y;						// ���S�_��ذݍ��W�ۑ�
			Tracker1.Setup(&Sbase, &col, NULL, 1, plane, 2);			// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
			Sphase = 1;
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(m_NumInp!=0) {												// <���l���͂̏ꍇ>
			CInputBtnDlg2 dlg;
			dlg.SetParam(m_CopyNum, Spnt1, Si1, Sscr1,  Sbase,
                         plane, m_RotateAdd1, m_RotateAdd2);			// ���Ұ��ݒ�
			dlg.DoModal();												// �����޲�۸ޕ\��
			dlg.GetParam(&m_RotateAdd1, &m_RotateAdd2);                 // ���Ұ��擾
			Tracker1.Reset(0);											// �ׯ��1����
			m_CopyNum = 0;												// ���ʐ��ر
			ExitMenu(1, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(Sphase==3) {											// <ϳ����͂�2��ڈȍ~>
			EndRotate(m_CopyNum, Spnt1, Si1, Sscr1, Sbase, plane,
                      Sd2, 0.0);										// ��]�ړ��I������
			m_CopyNum = 0;												// ���ʐ��ر
		}
		Display(0);														// �ĕ`��
		Sphase = (m_ActMenu!=NONE) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL>>>
		Tracker1.Reset(0);												// �ׯ��1����
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			if(m_CopyInp==2) {											// <<����Ӱ��:ON(�޲�۸ޕ\��)>>
				m_CopyNum  = m_ObjCopyNum;								// �����l�ݒ�
				Spnt1.p[0] = m_ObjXScale;
				Spnt1.p[1] = m_ObjYScale;
				Spnt1.p[2] = m_ObjZScale;
				Si1        = m_ObjSynFlg;
				Sscr1.p[0] = m_ObjXPoint;
				Sscr1.p[1] = m_ObjYPoint;
				Sscr1.p[2] = m_ObjZPoint;
				if(InputCopyNum(&m_CopyNum, &Spnt1, &Si1, &Sscr1)) {	// ���ʏ�����
					m_ObjCopyNum = m_CopyNum;							// �����l�ۑ�
					m_ObjXScale  = Spnt1.p[0];
					m_ObjYScale  = Spnt1.p[1];
					m_ObjZScale  = Spnt1.p[2];
					m_ObjSynFlg  = Si1;
					m_ObjXPoint  = Sscr1.p[0];
					m_ObjYPoint  = Sscr1.p[1];
					m_ObjZPoint  = Sscr1.p[2];
				} else {
					m_ObjCopyNum = 0;
				}
				if(m_CopyNum==0) {
					ExitMenu(1, 1);										// ���ʐ������ƭ��I��
				}
			} else if(m_CopyInp==1) {									//<<����Ӱ��:ON(�޲�۸ނȂ�)>>
				m_CopyNum = 1;											// ���ʐ�:1
				Si1 = 0;												// �ʒu���[�h:���[�h1
				for(i=0; i<DIM; i++) {
					Spnt1.p[i] = 1.0, Sscr1.p[i] = 1;					// �{��:1.0�{/��_:����
				}
			}
		}
		SetNumDisp(2);													// ���l�\����������
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(1, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/**************************************************************/
void CJcad3GlbView::MouseMirror(UINT flg, const CPoint& point,
                                int plane, int ms)						//<<< ����[���_]
/**************************************************************/
{
	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ����t�������̂ݗL��
			CalcScreenWorld(0, &Spb[0], point, plane);					// ������(ײ�)�I�_
			if(fabs(Spa[0].p[0]-Spb[0].p[0])>EPSILON||
               fabs(Spa[0].p[1]-Spb[0].p[1])>EPSILON||
               fabs(Spa[0].p[2]-Spb[0].p[2])>EPSILON) {
				if(flg&m_LimKey) {
					LimitPoint(plane, &Spb[0], &Spa[0]);				// ϳ����͐������W
				}
				Line.Setup(Spa, Spb, Sia, 1, plane);					// ������(ײ�)�ݒ�
				ObjectMirror(Sphase, m_CopyNum, Spa[0], Spb[0], plane);	// ���̋����ړ�
				Sphase = (Sphase==0) ? 1 : Sphase;
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(0, &Spa[0], point, plane);						// ������(ײ�)�n�_
	} else if(ms==3) {													// <<ϳ��̍��t���>>
		Line.Reset();													// ײ݉���
		Display(0);														// �ĕ`��
		m_CopyNum = 0, Sphase = 0;										// ���ʐ�/̪��޸ر
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			m_CopyNum = (m_CopyInp) ? 1 : 0;							// ����Ӱ��:ON
			Sia[0] = 0;													// ײݶװNo�o�^
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		ExitMenu(1, 0);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*************************************************************/
void CJcad3GlbView::MouseScale(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ���ޕύX[���_]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, i, esw=0, col=0, smd;
	double  dd[3], l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&!m_NumInp&&Sphase>1) {						// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			smd = (flg&m_LimKey) ? LimitPoint(plane, &pnt, &Spnt1) : 0;	// ϳ����͐������W
			smd = (flg&m_NLiKey) ? 2 : smd;								// Ӱ��:3D�ϓ�
			if(Si7==-1) {
				esw = (pnt.p[Si3]>=Sbase.p[Si3]) ? 1 : esw;
			} else if(Si7==1) {
				esw = (pnt.p[Si3]<=Sbase.p[Si3]) ? 1 : esw;
			} else {
				pnt.p[Si3] = Sbase.p[Si3]+1.0;
				Spnt1.p[Si3] = Sbase.p[Si3]+1.0;
			}
			if(Si8==-1) {
				esw = (pnt.p[Si4]>=Sbase.p[Si4]) ? 1 : esw;
			} else if(Si8==1) {
				esw = (pnt.p[Si4]<=Sbase.p[Si4]) ? 1 : esw;
			} else {
				pnt.p[Si4] = Sbase.p[Si4]+1.0;
				Spnt1.p[Si4] = Sbase.p[Si4]+1.0;
			}
			if(esw==0) {												// <����ȏꍇ>
				l1 = Spnt1.p[Si3]-Sbase.p[Si3];							// �{���v�Z
				l2 = pnt.p[Si3]-Sbase.p[Si3];
				dd[Si3] = l2/l1;
				l1 = Spnt1.p[Si4]-Sbase.p[Si4];
				l2 = pnt.p[Si4]-Sbase.p[Si4];
				dd[Si4] = l2/l1;
				dd[Si5] = 1.0;
				if(smd==1) {											// <Ӱ��:2D�ϓ�>
					if(dd[Si3]>dd[Si4]) {								// ��{���̗p
						dd[Si4] = dd[Si3];
					} else {
						dd[Si3] = dd[Si4];
					}
				} else if(smd==2) {										// <Ӱ��:3D�ϓ�>
					if(dd[Si3]>dd[Si4]) {								// ��{���̗p
						dd[Si4] = dd[Si3], dd[Si5] = dd[Si3];
					} else {
						dd[Si3] = dd[Si4], dd[Si5] = dd[Si4];
					}
				}
				ObjectScale(Sphase-2, Sbase, dd[0], dd[1], dd[2]);		// ���̊g��/�k��
				Sphase = (Sphase==2) ? 3 : Sphase;

				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                     box.bp[1].p[1]-box.bp[0].p[1],
                                     box.bp[1].p[2]-box.bp[0].p[2]);
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 13))!=-1) {	// �ׯ��1����
				Si2 = plane;											// ���ʕۑ�
				Si3 = Si2;												// ���W���ݒ�
				Si4 = (Si3+1>2) ? 0 : Si3+1;
				Si5 = (Si4+1>2) ? 0 : Si4+1;
				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				BoxCenter(box, &cent);									// �ޯ�������擾
				Sbase = Spa[hit];										// ػ��ފ�_�ݒ�
				Sbase.p[Si5] = cent.p[Si5];
				if(m_NumInp==0) {										// <ϳ����͂̏ꍇ>
					TrackerScale2(Si2, box, Sbase, Spa, Tens, &Si6);	// �ׯ���擾2
					Tracker1.Setup(Spa, Sib, Tens, Si6, Si2, 1);		// �ׯ��1�ݒ�
				} else {												// <���l���͂̏ꍇ>
					Tracker1.Setup(&Sbase, &col, NULL, 1, Si2, 2);		// �ׯ��1�ݒ�
				}
				Display(0);												// �ĕ`��
				Sphase = 1;
			}
		} else if(Sphase==1&&plane==Si2) {								// <�X�V�_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, Si2, pnt, Spa, Si6))!=-1) {	// �ׯ��1����
				Spnt1 = Spa[hit];										// ػ��ލX�V�_�ݒ�
				Si7 = (fabs(Spnt1.p[Si3]-Sbase.p[Si3])<EPSILON) ? 0		// ��_&�X�V�_�ʒu�֌W
                                  : ((Spnt1.p[Si3]<Sbase.p[Si3])?-1:1);
				Si8 = (fabs(Spnt1.p[Si4]-Sbase.p[Si4])<EPSILON) ? 0
                                  : ((Spnt1.p[Si4]<Sbase.p[Si4])?-1:1);
				Tracker1.Setup(&Sbase, &col, NULL, 1, Si2, 2);			// �ׯ��1�ݒ�
				Display(0);												// �ĕ`��
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(m_NumInp!=0&&Sphase==1) {									// <���l���͂̏ꍇ>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			for(i=0; i<DIM; i++) {
				dd[i] = box.bp[1].p[i]-box.bp[0].p[i];
			}
			CInputBtnDlg1 dlg(Si1);
			if(Si1==2) {												// <���ގw��>
				dlg.SetParam2(Sbase, dd[0], dd[1], dd[2], m_ScaleAdd1);	// ���Ұ��ݒ�
			} else {													// <�{���w��>
				dlg.SetParam2(Sbase, dd[0], dd[1], dd[2], m_ScaleAdd2);	// ���Ұ��ݒ�
			}
			dlg.DoModal();												// �����޲�۸ޕ\��
			if(Si1==2) {												// <���ގw��>
				dlg.GetParam(&m_ScaleAdd1);								// ���Ұ��擾
			} else {													// <�{���w��>
				dlg.GetParam(&m_ScaleAdd2);								// ���Ұ��擾
			}
			Tracker1.Reset(0);											// �ׯ��1����
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(m_NumInp==0&&(Sphase==2||Sphase==3)) {				// <ϳ����͂̏ꍇ>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &cent);										// �ޯ�������擾
			TrackerScale1(box, cent, Spa);								// �ׯ���擾
			Tracker1.Setup(Spa, Sia, NULL, 13, 5, 7);					// �ׯ��1�ݒ�
			Sphase = 0;
		}
		Display(0);														// �ĕ`��
	} else if(ms==0||ms==11) {											// <<<��������/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si1 = (m_FChar&0x1) ? 3 : 2;								// [F5]:�{���w��
		}
		for(i=0; i<13; i++) {
			Sia[i] = 0;													// �ׯ���F�ݒ�
		}
		Sib[0] = 0, Sib[1] = 2, Sib[2] = 2, Sib[3] = 2, Sib[4] = 2;
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &cent);											// �ޯ�������擾
		TrackerScale1(box, cent, Spa);									// �ׯ���擾1
		Tracker1.Setup(Spa, Sia, NULL, 13, 5, 7);						// �ׯ��1�ݒ�
		Display(0);														// �ĕ`��
		SetNumDisp(2);													// ���l�\����������
		m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                             box.bp[1].p[1]-box.bp[0].p[1],
                             box.bp[1].p[2]-box.bp[0].p[2]);
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/***************************************************************/
void CJcad3GlbView::MouseResize3(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< �����w��ػ���2[���_]
/***************************************************************/
{
	PNTTYPE pnt, p, p1, p2, p3;
	BOXTYPE box;
	int     hit, i, i0, i1, i2, i3, i4, ok=1, esw=0, col=0;
	double  d1, d2, d3, d4, d5;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&plane==Si1&&(Sphase==1||Sphase==3)) {		// <<�͈͎w��>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(Sphase==1) {												// ����̂ݗL���ړ��m�F
				ok = (abs(BasePoint.x-point.x)<=DRAGMIN&&
                      abs(BasePoint.y-point.y)<=DRAGMIN) ? 0 : ok;
			}
			if(ok==1) {
				if(Sphase==1) {
					d4 = pnt.p[Si1]-Sbase.p[Si1];
					d5 = pnt.p[Si2]-Sbase.p[Si2];						// ����݈̂ړ������m�F
					Si5 = (fabs(d4)>fabs(d5)) ? Si1 : Si2;				// Si1/Si2�����ړ��L��
				}
				esw = (pnt.p[Si5]<Spa[0].p[Si5]) ? 1 : esw;
				esw = (pnt.p[Si5]>Spa[2].p[Si5]) ? 1 : esw;
				if(Sphase==3) {											// 2��ڈȍ~�ړ��ϓ_�m�F
					if(Si4==0||(Si5==Si1&&Si4==3)||(Si5==Si2&&Si4==1)) {
						esw = (pnt.p[Si5]>Spc[2].p[Si5]) ? 1 : esw;
					} else {
						esw = (pnt.p[Si5]<Spc[0].p[Si5]) ? 1 : esw;
					}
				}
				if(esw==0) {											// <<�L���ړ��͈�>>
					if(Sphase==1) {
						for(i=0; i<4; i++) {
							Spc[i] = Spa[i];
						}
					}
					if(Si5==Si1) {										// <Si1�����ړ��L��>
						if(Si4==0||Si4==3) {
							i0 = 0, i1 = 0, i2 = 3, i3 = 1, i4 = 2;
							Tens[i3] = 2, Tens[i4] = 2;
						} else {
							i0 = 1, i1 = 1, i2 = 2, i3 = 0, i4 = 3;
							Tens[i3] = 1, Tens[i4] = 1;
						}
						Tens[i1] = 5, Tens[i2] = 5;
						TensC[0] = 6, TensC[1] = 6, TensC[2] = 6;
					} else {											// <Si2�����ړ��L��>
						if(Si4==0||Si4==1) {
							i0 = 0, i1 = 0, i2 = 1, i3 = 3, i4 = 2;
							Tens[i3] = 4, Tens[i4] = 4;
						} else {
							i0 = 1, i1 = 3, i2 = 2, i3 = 0, i4 = 1;
							Tens[i3] = 3, Tens[i4] = 3;
						}
						Tens[i1] = 6, Tens[i2] = 6;
						TensC[0] = 5, TensC[1] = 5, TensC[2] = 5;
					}
					Spc[i1].p[Si5] = pnt.p[Si5];
					Spc[i2].p[Si5] = pnt.p[Si5];
					if(Si6&&Si7) {
						for(i=0; i<DIM; i++) {							// �ׯ��2�ݒ�
							Spb[0].p[i] = (Spc[i1].p[i]+Spc[i3].p[i])/2.0;
							Spb[1].p[i] = (Spc[i2].p[i]+Spc[i4].p[i])/2.0;
						}
						Sib[2] = 2;
						Tens[i3] = Tens[i1], Tens[i4] = Tens[i1];
					} else {
						Spb[0] = Spa[i1], Spb[1] = Spa[i2];
						Sib[2] = (Si9) ? 2 : 0;
						TensC[0] = 7, TensC[1] = 7, TensC[2] = 7;
					}
					for(i=0; i<DIM; i++) {
						Spb[2].p[i] = (Spb[0].p[i]+Spb[1].p[i])/2.0;
					}
					if(i0==0) {
						Si6 = 1;										// <�����ݒ�>
						Si12 = (Si12<0) ? Si11++ : Si12;				// ������ײ�No�擾
						Sps[Si12] = Spc[i1], Spt[Si12] = Spc[i2];		// ���������ݒ�
						Sis[Si12] = 0;
					} else {
						Si7 = 1;										// <����ݒ�>
						Si13 = (Si13<0) ? Si11++ : Si13;				// ������ײ�No�擾
						Sps[Si13] = Spc[i1], Spt[Si13] = Spc[i2];		// ��������ݒ�
						Sis[Si13] = 0;
					}
					if(Si9) {
						if(Si6&&Si7) {
							p1 = Spc[0], p2 = Spc[2];
						} else if(Si6) {
							p1 = Spa[0], p2 = Spc[0];
						} else {
							p1 = Spc[2], p2 = Spa[2];
						}
						if(Si5==Si1) {
							p1.p[Si2] = Spb[2].p[Si2];
							p2.p[Si2] = Spb[2].p[Si2];
						} else {
							p1.p[Si1] = Spb[2].p[Si1];
							p2.p[Si1] = Spb[2].p[Si1];
						}
						Si14 = (Si14<0) ? Si11++ : Si14;				// ������ײ�No�擾
						Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;	// ������ݒ�
					}
					Sphase = (Sphase==1) ? 3 : Sphase;
					Tracker1.Setup(Spc, Sia, Tens, 4, Si1, 7);			// �ׯ��1�ݒ�
					Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);			// �ׯ��2�ݒ�
					Line.Setup(Sps, Spt, Sis, Si11, Si1);				// ײݕ\��
				}
			}
			Display(0);													// �ĕ`��
		} else if(flg&MK_LBUTTON&&plane==Si1&&(Sphase==4||Sphase==5)){	// <���ގw��>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(!Si10) {													// ����̂ݗL���ړ��m�F
				ok = (abs(BasePoint.x-point.x)<=DRAGMIN&&
                      abs(BasePoint.y-point.y)<=DRAGMIN) ? 0 : ok;
			}
			if(ok==1) {
				if(!Si10) {
					d4 = pnt.p[Si1]-Sbase.p[Si1];
					d5 = pnt.p[Si2]-Sbase.p[Si2];						// ����݈̂ړ������m�F
					Si8 = (fabs(d4)>fabs(d5)) ? Si1 : Si2;				// Si1/Si2�����ړ��L��
				}
				if(Si6&&Si7) {
					if(Si8==Si5) {										// �L���͈͊m�F
						esw = 1;
					} else {
						if(Si4==0) {
							esw = (pnt.p[Si8]>Spc[2].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]>Spb[2].p[Si8]) ? 1 : esw;
						} else if(Si4==1) {
							esw = (pnt.p[Si8]<Spc[0].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]<Spb[2].p[Si8]) ? 1 : esw;
						} else {
							if(pnt.p[Si8]<Spc[0].p[Si8]||pnt.p[Si8]>Spc[2].p[Si8]) {
								esw = 1;
							} else {
								Si9 = 1;
							}
						}
					}
				} else {
					if(Si8==Si5) {
						if(Si6) {
							esw = (pnt.p[Si8]>Spc[0].p[Si8]) ? 1 : esw;
						} else {
							esw = (pnt.p[Si8]<Spc[2].p[Si8]) ? 1 : esw;
						}
						esw = (Si9&&Si4==2) ? 1 : esw;
					} else {
						if(Si4==0) {
							esw = (pnt.p[Si8]>Spc[2].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]>Spb[2].p[Si8]) ? 1 : esw;
						} else if(Si4==1) {
							esw = (pnt.p[Si8]<Spc[0].p[Si8]) ? 1 : esw;
							esw = (Si9&&pnt.p[Si8]<Spb[2].p[Si8]) ? 1 : esw;
						} else {
							if(pnt.p[Si8]<Spc[0].p[Si8]||pnt.p[Si8]>Spc[2].p[Si8]) {
								esw = 1;
							} else {
								Si9 = 1;
							}
						}
					}
				}
				if(esw==0&&Si4==2&&Si9) {
					Spb[2].p[Si8] = pnt.p[Si8];
					TensC[2] = (Si8==Si1) ? 5 : 6;						// �ړ��������m�F
					Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);			// �ׯ��2�\��
					if(Si6&&Si7) {
						p1 = Spc[0], p2 = Spc[2];
					} else if(Si6) {
						p1 = Spa[0], p2 = Spc[0];
					} else {
						p1 = Spc[2], p2 = Spa[2];
					}
					if(Si5==Si1) {
						p1.p[Si2] = Spb[2].p[Si2];
						p2.p[Si2] = Spb[2].p[Si2];
					} else {
						p1.p[Si1] = Spb[2].p[Si1];
						p2.p[Si1] = Spb[2].p[Si1];
					}
					Si14 = (Si14<0) ? Si11++ : Si14;					// ������ײ�No�擾
					Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;		// ������ݒ�
					Line.Setup(Sps, Spt, Sis, Si11, Si1);				// ײݕ\��
					Si10 = 1;
				} else if(esw==0) {										// <�L���ړ��͈�>
					Spnt1 = Sbase;
					Spnt1.p[Si8] = pnt.p[Si8];
					d1 = 1.0, d3 = 1.0, d2 = -0.001;					// ��_���ݒ�
					for(i=0; i<DIM; i++) {
						p1.p[i] = 0.0, p2.p[i] = 0.0, p3.p[i] = 0.0;
					}
					if(Si6&&Si7) {
						if(Si4==0) {
							i0 = 0, p3 = Spc[2];
							p1 = (Si5==Si1) ? Spc[3] : Spc[1];
						} else {
							i0 = 1, p1 = Spc[0];
							p3 = (Si5==Si1) ? Spc[1] : Spc[3];
						}
						if(Si9) {
							if(Si5==Si1) {
								p1.p[Si2] = Spb[2].p[Si2];
								p3.p[Si2] = Spb[2].p[Si2];
							} else {
								p1.p[Si1] = Spb[2].p[Si1];
								p3.p[Si1] = Spb[2].p[Si1];
							}
							i0 = (flg&m_NLiKey) ? 2 : i0;
						}
						for(i=0; i<DIM; i++) {
							p2.p[i] = (p1.p[i]+p3.p[i])/2.0;
						}
						d4 = Spnt1.p[Si8]-p1.p[Si8];
						d5 = Sbase.p[Si8]-p1.p[Si8];
						d2 = d4/d5;
					} else if(Si6) {
						if(Si8==Si5) {
							i0 = 0, p1 = Spc[0];
							if(Si5==Si1) {
								p3 = Spc[3];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si2] = Spb[2].p[Si2];
									} else {
										p = p1, p1.p[Si2] = Spb[2].p[Si2];
									}
								}
							} else {
								p3 = Spc[1];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si1] = Spb[2].p[Si1];
									} else {
										p = p1, p1.p[Si1] = Spb[2].p[Si1];
									}
								}
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							if(Si4==0) {
								d1 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p3, p3 = p, d2 = 1.0, d3 = d1;
								}
							} else if(Si4==1) {
								d3 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p1, p1 = p, d2 = 1.0, d1 = d3;
								}
							} else {
								d1 = d4/d5, d3 = d4/d5;
							}
						} else {
							if(Si4==0) {
								i0 = 0;
								if(Si5==Si1) {
									p1 = Spa[3], p3 = Spc[3];
								} else {
									p1 = Spa[1], p3 = Spc[1];
								}
							} else {
								i0 = 1, p1 = Spa[0], p3 = Spc[0];
							}
							if(Si9) {
								if(Si5==Si1) {
									p1.p[Si2] = Spb[2].p[Si2];
									p3.p[Si2] = Spb[2].p[Si2];
								} else {
									p1.p[Si1] = Spb[2].p[Si1];
									p3.p[Si1] = Spb[2].p[Si1];
								}
								i0 = (flg&m_NLiKey) ? 2 : i0;
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							d1 = d4/d5;
						}
					} else {
						if(Si8==Si5) {
							i0 = 1, p3 = Spc[2];
							if(Si5==Si1) {
								p1 = Spc[1];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si2] = Spb[2].p[Si2];
									} else {
										p = p1, p1.p[Si2] = Spb[2].p[Si2];
									}
								}
							} else {
								p1 = Spc[3];
								if(Si9) {
									if(Si4==0) {
										p = p3, p3.p[Si1] = Spb[2].p[Si1];
									} else {
										p = p1, p1.p[Si1] = Spb[2].p[Si1];
									}
								}
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							if(Si4==0) {
								d1 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p3, p3 = p, d2 = 1.0, d3 = d1;
								}
							} else if(Si4==1) {
								d3 = d4/d5;
								if((flg&m_NLiKey)&&Si9) {
									p2 = p1, p1 = p, d2 = 1.0, d1 = d3;
								}
							} else {
								d1 = d4/d5, d3 = d4/d5;
							}
						} else {
							if(Si4==0) {
								i0 = 0, p1 = Spc[2], p3 = Spa[2];
							} else if(Si5==Si1) {
								i0 = 1, p1 = Spc[1], p3 = Spa[1];
							} else {
								i0 = 1, p1 = Spc[3], p3 = Spa[3];
							}
							if(Si9) {
								if(Si5==Si1) {
									p1.p[Si2] = Spb[2].p[Si2];
									p3.p[Si2] = Spb[2].p[Si2];
								} else {
									p1.p[Si1] = Spb[2].p[Si1];
									p3.p[Si1] = Spb[2].p[Si1];
								}
								i0 = (flg&m_NLiKey) ? 2 : i0;
							}
							d4 = Spnt1.p[Si8]-p1.p[Si8];
							d5 = Sbase.p[Si8]-p1.p[Si8];
							d3 = d4/d5;
						}
					}
					d4 = Spw[0].p[Si3];									// ������������
					d5 = Spw[1].p[Si3];									// �����������
					i1 = Si16+(2*Si17);									// �������������
					ObjectResize3(Sphase-4, Si1, i0, d1, d2, d3,
                                  i1, d4, d5, p1, p2, p3);				// �����w��ػ���2
					Tracker2.Reset(0);									// �ׯ��2����
					Sphase = (Sphase = 4) ? 5 : Sphase;
					Si10 = 1;
				}
			}
			Display(0);													// �ĕ`��
		} else if(flg&MK_LBUTTON&&plane==Si2&&Sphase==6) {				// <<�����͈͌���>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			d1 = pnt.p[Si3];											// �����������W
			if(Si4==0) {												// <����>
				if(d1>=Spd[0].p[Si3]&&d1<Spw[1].p[Si3]) {				// ���̍ŏ��`
					Spw[0].p[Si3] = d1;									// �w�����͍X�V
					Si16 = 1;											// �����ݒ�ς�
				}
			} else {													// <���>
				if(d1>Spw[0].p[Si3]&&d1<=Spd[1].p[Si3]) {				// �w�艺���`
					Spw[1].p[Si3] = d1;									// ���̍ő�͍X�V
					Si17 = 1;											// ����ݒ�ς�
				}
			}
			Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);					// �ׯ��3�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		Si10 = 0;
		CalcScreenWorld(1, &pnt, point, plane);							// ���W�_�擾
		if(Sphase==0) {													// <�͈͎w��1���>
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 4))!=-1) {	// �ׯ��1����
				Sbase = Spa[hit];										// ��_�ۑ�
				Si1 = plane;											// ���W���ݒ�
				Si2 = (Si1+1>2) ? 0 : Si1+1;
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				BoxCenter(box, &pnt);									// BOX�����擾
				Spd[0] = pnt, Spd[0].p[Si3] = box.bp[0].p[Si3];			// ���������̗��[
				Spd[1] = pnt, Spd[1].p[Si3] = box.bp[1].p[Si3];
				Spw[0] = Spd[0], Spw[1] = Spd[1];
				Si16 = 0, Si17 = 0;										// ������������ς��׸�
				TrackerBox2D(Si1, box, Spa, Tens);						// �ׯ���擾
				for(Si4=0; Si4<4; Si4++) {
					if(fabs(Sbase.p[Si1]-Spa[Si4].p[Si1])<EPSILON&&
                       fabs(Sbase.p[Si2]-Spa[Si4].p[Si2])<EPSILON) {	// �����_�ׯ������
						break;
					}
				}
				Tracker1.Setup(Spa, Sia, Tens, 4, Si1, 7);				// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				if(Si15) {												// <��������L>
					Sic[0] = 6, Sic[1] = 6;
					Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);			// �ׯ��3�ݒ�
				} else {												// <�������薳>
					Tracker3.Reset(0);									// �ׯ��3����
				}
				Display(0);												// �ĕ`��
				Sphase = 1, Si6 = 0, Si7 = 0;
			}
		} else if(Sphase==2) {											// <<�͈͎w��2��ڈȍ~>>
			if((Si4=Tracker1.HitTracker(1, Si1, pnt, Spc, 4))!=-1) {	// �ׯ��1����
				Sbase = Spc[Si4], Sphase = 3;							// ��_�ۑ�
			} else if((Si4=
                       Tracker2.HitTracker(1, Si1, pnt, Spb, 3))!=-1) {	// �ׯ��2����
				Sbase = Spb[Si4], Sphase = 4;							// ��_�ۑ�
			} else if(Si15) {											// <�����͈͌���L>
				if((Si4=Tracker3.HitTracker(1, Si2, pnt, Spw, 2))!=-1) {// �ׯ��3����
					Sbase = Spw[Si4], Sphase = 6;						// ��_�ۑ�
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==3||Sphase==4||Sphase==6) {
			if(Sphase==4&&Si4==2) {
				Si9 = 1, Sib[2] = 2;
				if(Si6&&Si7) {
					p1 = Spc[0], p2 = Spc[2];
				} else if(Si6) {
					p1 = Spa[0], p2 = Spc[0];
				} else {
					p1 = Spc[2], p2 = Spa[2];
				}
				if(Si5==Si1) {
					p1.p[Si2] = Spb[2].p[Si2];
					p2.p[Si2] = Spb[2].p[Si2];
				} else {
					p1.p[Si1] = Spb[2].p[Si1];
					p2.p[Si1] = Spb[2].p[Si1];
				}
				Si14 = (Si14<0) ? Si11++ : Si14;						// ������ײ�No�擾
				Sps[Si14] = p1, Spt[Si14] = p2, Sis[Si14] = 1;			// ������ݒ�
				Line.Setup(Sps, Spt, Sis, Si11, Si1);					// ײݕ\��
			}
			Tracker1.Setup(Spc, Sia, Tens, 4, Si1, 7);					// �ׯ��1�ݒ�
			Tracker2.Setup(Spb, Sib, TensC, 3, Si1, 7);					// �ׯ��2�ݒ�
			if(Si15) {													// <��������L>
				Tracker3.Setup(Spw, Siw, Sic, 2, Si2, 7);				// �ׯ��3�ݒ�
			} else {													// <�������薳>
				Tracker3.Reset(0);										// �ׯ��3����
			}
			Display(0);													// �ĕ`��
			Sphase = 2;
		} else if(Sphase==1||Sphase==5) {
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			TrackerBox3D(box, Spa);										// �ׯ���擾
			Tens[0] = 9, Tens[1] = 10, Tens[2] = 11, Tens[3] = 12;		// �ׯ��1��߼�ݐݒ�
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// �ׯ��1�ݒ�
			Tens[0] = 9, Tens[1] = 12, Tens[2] = 10, Tens[3] = 11;		// �ׯ��2��߼�ݐݒ�
			Tracker2.Setup(Spa, Sia, Tens, 4, 1, 7);					// �ׯ��2�ݒ�
			Tens[0] = 9, Tens[1] = 11, Tens[2] = 12, Tens[3] = 10;		// �ׯ��3��߼�ݐݒ�
			Tracker3.Setup(Spa, Sia, Tens, 4, 2, 7);					// �ׯ��3�ݒ�
			Line.Reset();												// ײ݉���
			Si11 = 0, Si12 = -1, Si13 = -1, Si14 = -1;					// ײݏ�񏉊���
			Display(0);													// �ĕ`��
			Sphase = 0, Si9 = 0;
		}
	} else if(ms==0||ms==11) {											// <<<��������/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si15 = (m_FChar&0x1) ? 1 : 0;								// [F5]:��������L
		}
		Sia[0] = 2, Sia[1] = 2, Sia[2] = 2, Sia[3] = 2;					// �ׯ��1�F�ݒ�
		Sib[0] = 0, Sib[1] = 0, Sib[2] = 0;								// �ׯ��2�F�ݒ�
		Siw[0] = 5, Siw[1] = 5;											// �ׯ��3�F�ݒ�
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		TrackerBox3D(box, Spa);											// �ׯ���擾
		Tens[0] = 9, Tens[1] = 10, Tens[2] = 11, Tens[3] = 12;			// �ׯ��1��߼�ݐݒ�
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// �ׯ��1�ݒ�
		Tens[0] = 9, Tens[1] = 12, Tens[2] = 10, Tens[3] = 11;			// �ׯ��2��߼�ݐݒ�
		Tracker2.Setup(Spa, Sia, Tens, 4, 1, 7);						// �ׯ��2�ݒ�
		Tens[0] = 9, Tens[1] = 11, Tens[2] = 12, Tens[3] = 10;			// �ׯ��3��߼�ݐݒ�
		Tracker3.Setup(Spa, Sia, Tens, 4, 2, 7);						// �ׯ��3�ݒ�
		Line.Reset();													// ײ݉���
		Si11 = 0, Si12 = -1, Si13 = -1, Si14 = -1;						// ײݏ�񏉊���
		Display(0);														// �ĕ`��
		Sphase = 0, Si9 = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Line.Reset();													// ײ݉���
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/***************************************************************/
void CJcad3GlbView::MouseResize4(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< �Ȑ��w��ػ���2[���_]
/***************************************************************/
{
	PNTTYPE pnt, wk[3];
	BOXTYPE box;
	int     hit, sft, i, j;
	double  d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2&&plane==Si4&&(flg&MK_LBUTTON)) {					// <<����w��>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			pnt.p[Si8] = (pnt.p[Si8]<Sd1) ? Sd1 : pnt.p[Si8];			// ���ގw������ʒu
			pnt.p[Si8] = (pnt.p[Si8]>Sd2) ? Sd2 : pnt.p[Si8];			// ���ގw������ʒu
			Spnt1.p[Si8] = pnt.p[Si8], Spnt2.p[Si8] = pnt.p[Si8];		// ������W�X�V
			Spb[0] = Spnt1, Spc[0] = Spnt2;
			Line.Setup(Spb, Spc, Sib, 1, Si4);							// ���(ײ�)�ݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase==4&&plane==Si4&&(flg&MK_LBUTTON)) {			// <<����w��>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if((Si2&&pnt.p[Si8]<Spnt1.p[Si8])||							// �����Ŋ�_�ȉ���
               (!Si2&&pnt.p[Si8]>Spnt1.p[Si8])) {						// �����Ŋ�_�ȏ�̏ꍇ��
				pnt.p[Si8] = Spnt1.p[Si8];								// ��_�̻��ގw������l
			}
			Spy[Si12].p[Si8] = pnt.p[Si8];								// ���ގw������ύX
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// �Ȑ���񕡎�
			}
			d1 = Spw[0].p[Si6];											// ������������
			d2 = Spw[1].p[Si6];											// �����������
			i = Si16+(2*Si17);											// �������������
			ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// �Ȑ��w��ػ���2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S�Ȑ�����
			SetBaseData(1, i, 0, Spz, wk, -1);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// �ׯ��1�ݒ�
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Six[i] = 5;												// �ǉ��p�ׯ���F
			}
			Si9 = Si1-1;												// �ǉ��p�ׯ����
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// �ׯ��2�ݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase==5&&plane==Si5&&(flg&MK_LBUTTON)) {			// <<�����͈͌���>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			d1 = pnt.p[Si6];											// �����������W
			if(Si12==0) {												// <����>
				if(d1>=Spd[0].p[Si6]&&d1<Spw[1].p[Si6]) {				// ���̍ŏ��`
					Spw[0].p[Si6] = d1;									// �w�����͍X�V
					Si16 = 1;											// �����ݒ�ς�
				}
			} else {													// <���>
				if(d1>Spw[0].p[Si6]&&d1<=Spd[1].p[Si6]) {				// �w�艺���`
					Spw[1].p[Si6] = d1;									// ���̍ő�͍X�V
					Si17 = 1;											// ����ݒ�ς�
				}
			}
			Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);					// �ׯ��3�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {													// <<��_�w��>>
			Si2 = (flg&MK_SHIFT) ? 0 : 1;								// �����׸�
			Si3 = (flg&MK_CONTROL) ? 1 : 0;								// �����׸�
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 6))!=-1) {	// �ׯ��1����
				Si4 = plane;											// ���ʕۑ�
				Si5 = (Si4+1>2) ? 0 : Si4+1;							// ���W���ݒ�
				Si6 = (Si5+1>2) ? 0 : Si5+1;
				i = hit/2;												// ��No�擾
				if(i!=Si6) {											// <�L�����ׯ��>
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					BoxCenter(box, &pnt);								// BOX�����擾
					Spd[0] = pnt, Spd[0].p[Si6] = box.bp[0].p[Si6];		// ���������̗��[
					Spd[1] = pnt, Spd[1].p[Si6] = box.bp[1].p[Si6];
					Spw[0] = Spd[0], Spw[1] = Spd[1];
					Si16 = 0, Si17 = 0, Si18 = 0;						// ������������ς��׸�
					if(i==Si4) {										// ������������ގw������ݒ�
						Si7 = Si4, Si8 = Si5;
						for(j=0; j<Si1; j++) {
							Tens[j] = 6;
						}
					} else {
						Si7 = Si5, Si8 = Si4;
						for(j=0; j<Si1; j++) {
							Tens[j] = 5;
						}
					}
					Spnt1 = Spa[Si7*2];									// ���������̊�_
					Spnt2 = Spa[Si7*2+1];
					Sd1 = Spa[Si8*2].p[Si8];							// ���ގw������̈ړ��͈�
					Sd2 = Spa[Si8*2+1].p[Si8];
					Tracker1.Reset(0);									// �ׯ��1����
					Tracker2.Reset(0);									// �ׯ��2����
					if(Si15) {											// <��������L>
						Siw[0] = 5, Siw[1] = 5, Sic[0] = 6, Sic[1] = 6;	// �ׯ��3�F�ݒ�
						Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);		// �ׯ��3�ݒ�
					} else {											// <�������薳>
						Tracker3.Reset(0);								// �ׯ��3����
					}
					Spb[0] = Spnt1, Spc[0] = Spnt2;
					Line.Setup(Spb, Spc, Sib, 1, Si4);					// ���(ײ�)�ݒ�
					Display(0);											// �ĕ`��
					Sphase = 2;
				}
			}
		} else if(Sphase==3&&plane==Si4) {								// <<����_�w��>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shift�����
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			Si12 = Tracker1.HitTracker(1, plane, pnt, Spy, Si1);		// �ׯ��1����
			if(Si12!=-1) {
				if(sft) {
					Siy[Si12] = (Siy[Si12]) ? 0 : 1;					// Shift:ON�͋Ȑ�Ӱ�ޔ��]
				}
				Siy[0] = 1, Siy[Si1-1] = 1;								// �n�_��I�_�͋Ȑ�Ӱ��:1
				Sphase = 4;
				Si18 = 1;												// ���ޕύX�ς�
			}
			if(Sphase!=4) {												// <�ׯ���I�𖳂̏ꍇ>
				Si12 = Tracker2.HitTracker(1, plane, pnt, Spx, Si9);	// �ׯ��2����
				if(Si12!=-1) {
					pnt = Spx[Si12];
					for(i=Si1-1; i>Si12; i--) {
						Spy[i+1] = Spy[i];								// ���ּ��
						Siy[i+1] = Siy[i];
						Tens[i+1] = Tens[i];
					}
					Spy[Si12+1] = pnt;									// �w��ʒu��ǉ�
					Siy[Si12+1] = (sft) ? 1 : 0;
					Tens[Si12+1] = Tens[0];
					Si1++;												// �ύX�㐧��_��
					for(i=0; i<Si1; i++) {
						Spz[i] = Spy[i], Siz[i] = Siy[i];				// �Ȑ���񕡎�
					}
					d1 = Spw[0].p[Si6];									// ������������
					d2 = Spw[1].p[Si6];									// �����������
					i = Si16+(2*Si17);									// �������������
					ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                                  i, d1, d2, Spnt1, Si1, Spy, Siy);		// �Ȑ��w��ػ���2
					i = Si1;
					MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);				// S�Ȑ�����
					SetBaseData(1, i, 0, Spz, wk, -1);					// ����ް��o�^
					Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);		// �ׯ��1�ݒ�
					for(i=0; i<Si1-1; i++) {
						for(j=0; j<3; j++) {
							Spx[i].p[j]=(Spy[i].p[j]+Spy[i+1].p[j])/2.0;// �ǉ��p�ׯ��
						}
						Six[i] = 5;										// �ǉ��p�ׯ���F
					}
					Si9 = Si1-1;										// �ǉ��p�ׯ����
					Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);		// �ׯ��2�ݒ�
					Si18 = 1;											// ���ޕύX�ς�
					Display(0);											// �ĕ`��
				}
			}
		} else if(Sphase==3&&plane==Si5) {								// <<�����͈͌���>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(Si15&&!Si18) {											// <���ޕύX���̏ꍇ>
				Si12 = Tracker3.HitTracker(1, plane, pnt, Spw, 2);		// �ׯ��3����
				if(Si12!=-1) {
					Sphase = 5;
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==2) {													// <<����w��>>
			d1 = Spnt2.p[Si7]-Spnt1.p[Si7];								// �������[��
			j = Si1-1, d1 /= j;											// ���Ԑ���_�v�Z
			Spy[0] = Spnt1, Spy[j] = Spnt2;
			Siy[0] = 1, Siy[j] = 1;
			for(i=1; i<j; i++) {
				Spy[i] = Spnt1, Siy[i] = 0;
				Spy[i].p[Si7] = Spnt1.p[Si7]+d1*i;
			}
			Si2 = (Spnt1.p[Si8]<=Sd1) ? 1 : Si2;						// �����׸ޕ␳
			Si2 = (Spnt1.p[Si8]>=Sd2) ? 0 : Si2;
			if(Si2) {													// <�����̏ꍇ>
				Sd3 = Sd2 - Spnt1.p[Si8];								// �����
				for(i=0; i<Si1; i++) {
					Spy[i].p[Si8] = Sd2;								// �ő�l�ݒ�
				}
			} else {													// <�����̏ꍇ>
				Sd3 = Spnt1.p[Si8] - Sd1;								// �����
				for(i=0; i<Si1; i++) {
					Spy[i].p[Si8] = Sd1;								// �ŏ��l�ݒ�
				}
			}
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// �Ȑ���񕡎�
			}
			d1 = Spw[0].p[Si6];											// ������������
			d2 = Spw[1].p[Si6];											// �����������
			i = Si16+(2*Si17);											// �������������
			ObjectResize4(0, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// �Ȑ��w��ػ���2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S�Ȑ�����
			SetBaseData(1, i, 0, Spz, wk, Si4);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// �ׯ��1�ݒ�
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Six[i] = 5;												// �ǉ��p�ׯ���F
			}
			Si9 = Si1-1;												// �ǉ��p�ׯ����
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// �ׯ��2�ݒ�
			Sphase = 3;
			Display(0);													// �ĕ`��
		} else if(Sphase==4||Sphase==5) {								// <<����_�w��>>
			for(i=0; i<Si1; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// �Ȑ���񕡎�
			}
			d1 = Spw[0].p[Si6];											// ������������
			d2 = Spw[1].p[Si6];											// �����������
			i = Si16+(2*Si17);											// �������������
			ObjectResize4(1, Si2, Si3, Si7, Si8, Si6, Sd3,
                          i, d1, d2, Spnt1, Si1, Spy, Siy);				// �Ȑ��w��ػ���2
			i = Si1;
			MakeSCurve(&i, 0, 3, 2.0, Spz, Siz);						// S�Ȑ�����
			SetBaseData(1, i, 0, Spz, wk, -1);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, Tens, Si1, Si4, 7);				// �ׯ��1�ݒ�
			for(i=0; i<Si1-1; i++) {
				for(j=0; j<3; j++) {
					Spx[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Six[i] = 5;												// �ǉ��p�ׯ���F
			}
			Si9 = Si1-1;												// �ǉ��p�ׯ����
			Tracker2.Setup(Spx, Six, NULL, Si9, Si4, 7);				// �ׯ��2�ݒ�
			if(Si15) {													// <��������L>
				Tracker3.Setup(Spw, Siw, Sic, 2, Si5, 7);				// �ׯ��3�ݒ�
			} else {													// <�������薳>
				Tracker3.Reset(0);										// �ׯ��3����
			}
			Sphase = 3;
			Display(0);													// �ĕ`��
		}
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			OffUndoRedo();												// Undo/Redo�s��
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
			Si15 = (m_FChar&0x1) ? 1 : 0;								// [F5]:��������L
		}
		if(m_NumInp!=0) {												// <���l���͂̏ꍇ>
			CInputDlg dlg(13);
			dlg.SetValue(m_LinePNum);									// �����l�\��
			if(dlg.DoModal()==IDOK) {									// �����޲�۸ޕ\��
				dlg.GetValue(&m_LinePNum);								// �������_���擾
				Si1 = m_LinePNum;
			} else {
				Si1 = 0;
			}
		} else {														// <���l���͖��̏ꍇ>
			Si1 = 2;
		}
		if(Si1>0) {
			Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;				// �ׯ���F�ݒ�
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &pnt);										// �ޯ�������擾
			TrackerBoxCrs(box, pnt, Spa);								// �ׯ���擾
			Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// �ׯ��1�ݒ�
			Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);				// �ׯ��2�ݒ�
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);					// �ׯ��3�ݒ�
			Display(0);													// �ĕ`��
			Sphase = 1;
			m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;			// ����No�ݒ�
		} else {
			ExitMenu(0, 1);												// �ƭ��I��
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		DeleteBaseData();												// ����ް��폜
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Line.Reset();													// ײ݉���
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}
