/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewEdit2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "InputBtnDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "DataFile2.h"
#include "DataFile3.h"
#include "Command.h"
#include "Select.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "RCommand.h"
#include "EditObject.h"
#include "FaceEdit.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/************************************************************/
void CJcad3GlbView::MouseBend(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< �~���Ȃ�[���_]
/************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, hw, esw=0;
	double  th, l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON&&plane==Si1) {								// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(Sphase==1) {												// <�Ȃ��͈͕ύX1���>
				if(abs(BasePoint.x-point.x)>DRAGMIN||
                   abs(BasePoint.y-point.y)>DRAGMIN) {
					l1 = pnt.p[Si2]-Sbase.p[Si2];
					l2 = pnt.p[Si3]-Sbase.p[Si3];
					if(fabs(l1)>fabs(l2)) {								// <i1�����ړ����L��>
						esw = (pnt.p[Si2]<Spa[Si1*4].p[Si2]) ? 1 : esw;
						esw = (pnt.p[Si2]>Spa[Si1*4+2].p[Si2]) ? 1 : esw;
					} else {											// <i2�����ړ����L��>
						esw = (pnt.p[Si3]<Spa[Si1*4].p[Si3]) ? 1 : esw;
						esw = (pnt.p[Si3]>Spa[Si1*4+2].p[Si3]) ? 1 : esw;
					}
					if(esw==0) {										// <�L���ړ��͈�>
						if(fabs(l1)>fabs(l2)) {							// <i1�����ړ����L��>
							if(Si4==0||Si4==1) {						// �ΏەӍ��W�ݒ�
								Spb[0]=Spa[Si1*4], Spb[2]=Spa[Si1*4+1];
								Si7 = 1;
							} else {
								Spb[0]=Spa[Si1*4+3], Spb[2]=Spa[Si1*4+2];
								Si7 = 0;
							}
							Tens[0] = 5, Tens[1] = 6, Tens[2] = 5;
							Si4 = (Si4==0||Si4==3) ? 0 : 2;				// �ړ��Ώے��_�ʒu
							Spb[Si4].p[Si2] += l1;						// �ړ����␳
							Si5 = Si2, Si6 = Si3;
						} else {										// <i2�����ړ����L��>
							if(Si4==0||Si4==3) {						// �ΏەӍ��W�ݒ�
								Spb[0]=Spa[Si1*4], Spb[2]=Spa[Si1*4+3];
								Si7 = 1;
							} else {
								Spb[0]=Spa[Si1*4+1], Spb[2]=Spa[Si1*4+2];
								Si7 = 0;
							}
							Tens[0] = 6, Tens[1] = 5, Tens[2] = 6;
							Si4 = (Si4==0||Si4==1) ? 0 : 2;				// �ړ��Ώے��_�ʒu
							Spb[Si4].p[Si3] += l2;						// �ړ����␳
							Si5 = Si3, Si6 = Si2;
						}
						Spb[1].p[0] = (Spb[0].p[0]+Spb[2].p[0])/2.0;	// ���ԓ_���W
						Spb[1].p[1] = (Spb[0].p[1]+Spb[2].p[1])/2.0;
						Spb[1].p[2] = (Spb[0].p[2]+Spb[2].p[2])/2.0;
						Tracker1.Setup(Spb, Sib, Tens, 3, Si1, 7);		// �ׯ��1�ݒ�
						Tracker2.Reset(0);								// �ׯ��2����
						Tracker3.Reset(0);								// �ׯ��3����
						Sphase = 2;
					}
				}
			} else if(Sphase==2&&Si4!=-1) {								// <�Ȃ��͈͕ύX2��ڈȍ~>
				if(Si4==0) {											// �ړ��͈�����
					esw = (pnt.p[Si5]<Spa[Si1*4].p[Si5]) ? 1 : esw;
					esw = (pnt.p[Si5]>=Spb[2].p[Si5]) ? 1 : esw;
				} else {
					esw = (pnt.p[Si5]<=Spb[0].p[Si5]) ? 1 : esw;
					esw = (pnt.p[Si5]>Spa[Si1*4+2].p[Si5]) ? 1 : esw;
				}
				if(esw==0) {											// <�L���ړ��͈�>
					Spb[Si4].p[Si5] = pnt.p[Si5];						// �ړ����␳
					Spb[1].p[0] = (Spb[0].p[0]+Spb[2].p[0])/2.0;		// ���ԓ_���W
					Spb[1].p[1] = (Spb[0].p[1]+Spb[2].p[1])/2.0;
					Spb[1].p[2] = (Spb[0].p[2]+Spb[2].p[2])/2.0;
					Tracker1.Setup(Spb, Sib, Tens, 3, Si1, 7);			// �ׯ��1�ݒ�
				}
			} else if(Sphase>2&&(!m_NumInp)) {							// <�p�x�w��>
				l1 = m_Fovy/4000.0;										// �p�x�v�Z
				l2 = Sbase.p[Si6]-pnt.p[Si6];
				th = 360.0*l2/l1;
				ObjectBend(Sphase-3, Si5, Si6, th, Spnt1, Spnt2);		// ���̉~���Ȃ�
				Sphase = (Sphase==3) ? 4 : Sphase;
				m_pNumDisp->SetParam(th);								// �Ȃ���p�x�\��
			}
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		Si4 = -1;
		if(Sphase==0) {													// <��_�擾1>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 12))!=-1) {	// �ׯ��1����
				Sbase = Spa[hit];										// ��_�ۑ�
				Si1 = plane;											// ���ʕۑ�
				Si2 = Si1;												// ���W���ݒ�
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				hw = hit/4;												// �w���ׯ������
				Si4 = hit%4;											// �w���ׯ���ʒu
				if(hw==Si1) {
					Sphase = 1;											// <�Ȃ���͈͕ύX>
				} else {												// <�Ȃ���p�x�w��>
					if(hw==Si3) {										// �Ȃ�����ۑ�
						if(Si4==0) {
							Spnt1 = Spa[Si1*4], Spnt2 = Spa[Si1*4+1];
							Si7 = 1;
						} else {
							Spnt1 = Spa[Si1*4+3], Spnt2 = Spa[Si1*4+2];
							Si7 = 0;
						}
						Si5 = Si2, Si6 = Si3;
					} else {
						if(Si4==0) {
							Spnt1 = Spa[Si1*4], Spnt2 = Spa[Si1*4+3];
							Si7 = 1;
						} else {
							Spnt1 = Spa[Si1*4+1], Spnt2 = Spa[Si1*4+2];
							Si7 = 0;
						}
						Si5 = Si3, Si6 = Si2;
					}
					Tracker1.Reset(0);									// �ׯ��1����
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
					Display(0);											// �ĕ`��
					Sphase = 3;
				}
			}
		} else if(Sphase==2&&plane==Si1) {								// <��_�擾2>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spb, 3))!=-1) {	// �ׯ��1����
				Sbase = Spb[hit];										// ��_�ۑ�
				Si4 = hit;
				if(hit==1) {											// <�Ȃ���p�x�w��>
					Spnt1 = Spb[0], Spnt2 = Spb[2];						// �Ȃ���͈�
					Tracker1.Reset(0);									// �ׯ��1����
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
					Display(0);											// �ĕ`��
					Sphase = 3;
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(m_NumInp!=0&&Sphase==3) {									// <���l���͂̏ꍇ>
			CInputBtnDlg3 dlg(1);
			dlg.SetParam1(Si7, Si5, Si6, Spnt1, Spnt2, m_BendAdd);		// ���Ұ��ݒ�
			dlg.DoModal();												// �����޲�۸ޕ\��
			dlg.GetParam(&m_BendAdd);									// ���Ұ��擾
			Display(0);													// �ĕ`��
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(m_NumInp==0&&(Sphase==1||Sphase==3||Sphase==4)) {		// <ϳ����͂̏ꍇ>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &cent);										// �ޯ�������擾
			TrackerBend(box, cent, Spa, Tens);							// �ׯ���擾
			Spd[0]=Spa[0], Spd[1]=Spa[1], Spd[2]=Spa[2], Spd[3]=Spa[3];
			Spd[4]=Spa[8], Spd[5]=Spa[11], Spd[6]=Spa[4], Spd[7]=Spa[5];
			Tracker1.Setup(Spd, Sia, Tens, 8, 0, 7);					// �ׯ��1�ݒ�
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[6], Spd[3]=Spa[7];
			Spd[4]=Spa[0], Spd[5]=Spa[3], Spd[6]=Spa[8], Spd[7]=Spa[9];
			Tracker2.Setup(Spd, Sia, Tens, 8, 1, 7);					// �ׯ��2�ݒ�
			Spd[0]=Spa[8], Spd[1]=Spa[9], Spd[2]=Spa[10], Spd[3]=Spa[11];
			Spd[4]=Spa[4], Spd[5]=Spa[7], Spd[6]=Spa[0], Spd[7]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 8, 2, 7);					// �ׯ��3�ݒ�
			Display(0);													// �ĕ`��
			Sphase = 0;
		}
	} else if(ms==0||ms==11) {											// <<<��������/DEL>>>
		if(ms==0&&(!m_InitMenu)) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;					// �ׯ���F�ݒ�
		Sia[4] = 0, Sia[5] = 0, Sia[6] = 0, Sia[7] = 0;
		Sib[0] = 2, Sib[1] = 0, Sib[2] = 2;
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &cent);											// �ޯ�������擾
		TrackerBend(box, cent, Spa, Tens);								// �ׯ���擾
		Spd[0]=Spa[0], Spd[1]=Spa[1], Spd[2]=Spa[2], Spd[3]=Spa[3];
		Spd[4]=Spa[8], Spd[5]=Spa[11], Spd[6]=Spa[4], Spd[7]=Spa[5];
		Tracker1.Setup(Spd, Sia, Tens, 8, 0, 7);						// �ׯ��1�ݒ�
		Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[6], Spd[3]=Spa[7];
		Spd[4]=Spa[0], Spd[5]=Spa[3], Spd[6]=Spa[8], Spd[7]=Spa[9];
		Tracker2.Setup(Spd, Sia, Tens, 8, 1, 7);						// �ׯ��2�ݒ�
		Spd[0]=Spa[8], Spd[1]=Spa[9], Spd[2]=Spa[10], Spd[3]=Spa[11];
		Spd[4]=Spa[4], Spd[5]=Spa[7], Spd[6]=Spa[0], Spd[7]=Spa[1];
		Tracker3.Setup(Spd, Sia, Tens, 8, 2, 7);						// �ׯ��3�ݒ�
		Display(0);														// �ĕ`��
		SetNumDisp(2);													// ���l�\����������
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*************************************************************/
void CJcad3GlbView::MouseTwist(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< �Ђ˂�[���_]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, col=0;
	double  th, l11, l12, l21, l22;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(m_NumInp==0&&Sphase>=1&&plane==Si1&&(flg&MK_LBUTTON)) {		// ϳ����t�������̂ݗL��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			l11 = Spnt2.p[Si1]-Spnt1.p[Si1];							// �p�x�Z�o
			l12 = Spnt2.p[Si2]-Spnt1.p[Si2];
			l21 = pnt.p[Si1]-Spnt1.p[Si1];
			l22 = pnt.p[Si2]-Spnt1.p[Si2];
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
			ObjectTwist(Sphase-1, Si3, th, Sd3, Spnt1);					// ���̂Ђ˂�
			Sphase = (Sphase==1) ? 2 : Sphase;
			m_pNumDisp->SetParam(th);									// �Ђ˂�p�x�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 4))!=-1) {	// �ׯ��1����
				Si1 = plane;											// ���ʕۑ�
				Si2 = (Si1+1>2) ? 0 : Si1+1;							// ���W���ݒ�
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				BoxCenter(box, &Spnt1);									// �ޯ�������擾
				Spnt1.p[Si3] = box.bp[0].p[Si3];						// �������ŏ��l�ݒ�
				Sd3 = box.bp[1].p[Si3]-box.bp[0].p[Si3];				// �������ޯ����
				Spnt2 = Spa[hit];										// �p�x��_�ݒ�
				Tracker1.Setup(&Spnt1, &col, NULL, 1, Si1, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Tracker3.Reset(0);										// �ׯ��3����
				Display(0);												// �ĕ`��
				Sphase = 1, Sd1 = 0.0, Sd2 = 0.0;
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(m_NumInp!=0&&Sphase==1) {									// <���l���͂̏ꍇ>
			CInputBtnDlg3 dlg(4);
			dlg.SetParam4(Si3, Sd3, Spnt1, m_TwistAdd);					// ���Ұ��ݒ�
			dlg.DoModal();												// �����޲�۸ޕ\��
			dlg.GetParam(&m_TwistAdd);									// ���Ұ��擾
			Tracker1.Reset(0);											// �ׯ��1����
			Tracker2.Reset(0);											// �ׯ��2����
			Tracker3.Reset(0);											// �ׯ��3����
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(m_NumInp==0) {										// <ϳ����͂̏ꍇ>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &cent);										// �ޯ�������擾
			TrackerBox3D(box, Spa);										// �ׯ���擾
			Spd[0]=cent, Spd[0].p[2]=Spa[1].p[2];						// �ׯ��1���W�ݒ�
			Spd[1]=Spa[0], Spd[2]=Spa[1], Spd[3]=Spa[2], Spd[4]=Spa[3];
			Tens[0]=0, Tens[1]=0, Tens[2]=8, Tens[3]=0, Tens[4]=0;		// �ׯ��1��߼�ݐݒ�
			Tracker1.Setup(Spd, Sia, Tens, 5, 0, 1);					// �ׯ��1�ݒ�
			Spd[0]=cent, Spd[0].p[0]=Spa[2].p[0];						// �ׯ��2���W�ݒ�
			Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=8, Tens[4]=0;		// �ׯ��2��߼�ݐݒ�
			Tracker2.Setup(Spd, Sia, Tens, 5, 1, 1);					// �ׯ��2�ݒ�
			Spd[0]=cent, Spd[0].p[1]=Spa[3].p[1];						// �ׯ��3���W�ݒ�
			Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=0, Tens[4]=8;		// �ׯ��3��߼�ݐݒ�
			Tracker3.Setup(Spd, Sia, Tens, 5, 2, 1);					// �ׯ��3�ݒ�
			Sphase = 0;
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0, Sia[4] = 0;		// �ׯ���F�ݒ�
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &cent);											// �ޯ�������擾
		TrackerBox3D(box, Spa);											// �ׯ���擾
		Spd[0]=cent, Spd[0].p[2]=Spa[1].p[2];							// �ׯ��1���W�ݒ�
		Spd[1]=Spa[0], Spd[2]=Spa[1], Spd[3]=Spa[2], Spd[4]=Spa[3];
		Tens[0]=0, Tens[1]=0, Tens[2]=8, Tens[3]=0, Tens[4]=0;			// �ׯ��1��߼�ݐݒ�
		Tracker1.Setup(Spd, Sia, Tens, 5, 0, 1);						// �ׯ��1�ݒ�
		Spd[0]=cent, Spd[0].p[0]=Spa[2].p[0];							// �ׯ��2���W�ݒ�
		Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=8, Tens[4]=0;			// �ׯ��2��߼�ݐݒ�
		Tracker2.Setup(Spd, Sia, Tens, 5, 1, 1);						// �ׯ��2�ݒ�
		Spd[0]=cent, Spd[0].p[1]=Spa[3].p[1];							// �ׯ��3���W�ݒ�
		Tens[0]=0, Tens[1]=0, Tens[2]=0, Tens[3]=0, Tens[4]=8;			// �ׯ��3��߼�ݐݒ�
		Tracker3.Setup(Spd, Sia, Tens, 5, 2, 1);						// �ׯ��3�ݒ�
		Display(0);														// �ĕ`��
		SetNumDisp(2);													// ���l�\����������
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*************************************************************/
void CJcad3GlbView::MouseShear(UINT flg, const CPoint& point,
                               int plane, int ms)						//<<< ����f[���_]
/*************************************************************/
{
	PNTTYPE pnt, cent;
	BOXTYPE box;
	int     hit, i, j, col=4;
	double  l1, l2, dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(m_NumInp==0&&Sphase>=1&&plane==Si1&&(flg&MK_LBUTTON)) {		// ϳ����t�������̂ݗL��
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			l1 = Spnt2.p[Si4]-Spnt1.p[Si4];								// ����f������������
			l2 = pnt.p[Si5]-Spnt1.p[Si5];								// ����f������������
			dd = l2/l1;													// ����f�X��
			ObjectShear(Sphase-1, Si4, Si5, dd, Spnt1);					// ����f
			Sphase = (Sphase==1) ? 2 : Sphase;
			Spc[0].p[Si5] = pnt.p[Si5];									// ����f�������ʒu�ύX
			Line.Setup(Spb, Spc, Sib, 1, Si1);							// ����f��(ײ�)�ݒ�
			m_pNumDisp->SetParam(l2);									// ����f���l�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((hit=Tracker1.HitTracker(1, plane, pnt, Spa, 6))!=-1) {	// �ׯ��1����
				Si1 = plane;											// ���ʕۑ�
				Si2 = (Si1+1>2) ? 0 : Si1+1;							// ���W���ݒ�
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				i = hit/2;												// ��No�擾
				if(i!=Si3) {											// <�L�����ׯ��>
					if(i==Si1) {										// ����f��������������ݒ�
						Si4 = Si1, Si5 = Si2;
					} else {
						Si4 = Si2, Si5 = Si1;
					}
					j = hit%2;											// �召No�擾
					if(j==0) {
						Spnt1 = Spa[i*2+1], Spnt2 = Spa[i*2];			// ����f��_��ړ����_�ݒ�
					} else {
						Spnt1 = Spa[i*2], Spnt2 = Spa[i*2+1];
					}
					Tracker1.Setup(&Spnt1, &col, NULL, 1, Si1, 2);		// �ׯ��1�ݒ�
					Tracker2.Reset(0);									// �ׯ��2����
					Tracker3.Reset(0);									// �ׯ��3����
					Spb[0] = Spnt1, Spc[0] = Spnt2;
					Line.Setup(Spb, Spc, Sib, 1, Si1);					// ����f��(ײ�)�ݒ�
					Display(0);											// �ĕ`��
					Sphase = 1;
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(m_NumInp!=0&&Sphase==1) {									// <���l���͂̏ꍇ>
			CInputBtnDlg3 dlg(6);
			dlg.SetParam6(Si4, Si5, Si1, Spnt1, Spnt2, m_ShearAdd);		// ���Ұ��ݒ�
			dlg.DoModal();												// �����޲�۸ޕ\��
			dlg.GetParam(&m_ShearAdd);									// ���Ұ��擾
			Tracker1.Reset(0);											// �ׯ��1����
			Tracker2.Reset(0);											// �ׯ��2����
			Tracker3.Reset(0);											// �ׯ��3����
			Line.Reset();												// ײ݉���
			ExitMenu(0, 1);												// �ƭ��I��
			EndJMenu1();												// �ƭ��I������
		} else if(m_NumInp==0) {										// <ϳ����͂̏ꍇ>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			BoxCenter(box, &cent);										// �ޯ�������擾
			TrackerBoxCrs(box, cent, Spa);								// �ׯ���擾
			Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
			Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);					// �ׯ��1�ݒ�
			Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);				// �ׯ��2�ݒ�
			Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
			Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);					// �ׯ��3�ݒ�
			Line.Reset();												// ײ݉���
			Sphase = 0;
		}
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(!m_InitMenu) {
			StartMenu();												// �ƭ��J�n����
			CheckCtrlKey1();											// ���䷰�m�F(�N����)
		}
		Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;					// �ׯ���F�ݒ�
		Sib[0] = 0;														// ����f���װNo
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		BoxCenter(box, &cent);											// �ޯ�������擾
		TrackerBoxCrs(box, cent, Spa);									// �ׯ���擾
		Tens[0] = 6, Tens[1] = 6, Tens[2] = 5, Tens[3] = 5;
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// �ׯ��1�ݒ�
		Tracker2.Setup(&Spa[2], Sia, Tens, 4, 1, 7);					// �ׯ��2�ݒ�
		Spd[0]=Spa[4], Spd[1]=Spa[5], Spd[2]=Spa[0], Spd[3]=Spa[1];
		Tracker3.Setup(Spd, Sia, Tens, 4, 2, 7);						// �ׯ��3�ݒ�
		Display(0);														// �ĕ`��
		SetNumDisp(2);													// ���l�\����������
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*****************************************************************/
void CJcad3GlbView::MouseLinesEdit(UINT flg, const CPoint& point,
                                   int plane, int ms)					//<<< �Ȑ��w��ړ�[���_]
/*****************************************************************/
{
	OBJTYPE* op, * op1;
	PNTTYPE  pnt, ps[3], pe[3];
	int      i, i1, i2, cl[3];
	double   d1, d2, d3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1) {													// <<���2���>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ϳ����͐������W
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 1;
			Line.Setup(ps, pe, cl, 1, 5);								// ײݐݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase==3) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i1 = plane;													// �L�����W
			i2 = (i1+1>2) ? 0 : i1+1;
			if(plane==Si3) {											// <�n�_��I�_�ύX�̏ꍇ>
				if(Si6==0) {
					Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];	// �n�_���W�l�ύX
				} else {
					Spnt2.p[i1] = pnt.p[i1], Spnt2.p[i2] = pnt.p[i2];	// �I�_���W�l�ύX
				}
			} else {													// <����ړ��̏ꍇ>
				if(Si6==0) {
					for(i=0; i<DIM; i++) {
						Spnt2.p[i] -= Spnt1.p[i];						// �n�_��I�_�ړ�
					}
					Spnt1.p[i1] = pnt.p[i1], Spnt1.p[i2] = pnt.p[i2];
					for(i=0; i<DIM; i++) {
						Spnt2.p[i] += Spnt1.p[i];
					}
				} else {
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] -= Spnt2.p[i];						// �I�_��n�_�ړ�
					}
					Spnt2.p[i1] = pnt.p[i1], Spnt2.p[i2] = pnt.p[i2];
					for(i=0; i<DIM; i++) {
						Spnt1.p[i] += Spnt2.p[i];
					}
				}
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 1;
			Line.Setup(ps, pe, cl, 1, 5);								// ײݐݒ�
			Spa[0] = Spnt1, Spa[1] = Spnt2;								// �I�_��n�_�ׯ��
			Tracker1.Setup(Spa, Sia, NULL, 2, 5, 7);					// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <<���1���>>
			if(plane==0) {												// <�w���XY���ʂ̂ݗL��>
				CalcScreenWorld(0, &Spnt1, point, plane);				// ���W�l�ۑ�
				Si3 = 0, Si4 = 1, Si5 = 2;								// �Ώە��ʕۑ�
				Spnt2 = Spnt1, Sphase = 1;
			}
		} else if(Sphase==2) {											// <<����ύX>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((Si6=Tracker1.HitTracker(1, plane, pnt, Spa, 2))!=-1) {	// �ׯ��1����
				Sphase = 3, Sbase = Spa[Si6];
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase>0) {													// <<�������>>
			d1 = fabs(Spnt1.p[Si3]-Spnt2.p[Si3]);						// �������[��
			d2 = fabs(Spnt1.p[Si4]-Spnt2.p[Si4]);
			d3 = m_Fovy/6000.0;
			if(d1>d3||d2>d3) {											// <<�L�������̏ꍇ>>
				Spa[0] = Spnt1, Sia[0] = 4;								// �n�_�ׯ��
				Spa[1] = Spnt2, Sia[1] = 4;								// �I�_�ׯ��
				Tracker1.Setup(Spa, Sia, NULL, 2, 5, 7);				// �ׯ��1�ݒ�
				Sphase = 2;
			} else {													// <���������̏ꍇ>
				Line.Reset();											// ײ݉���
				Sphase = 0;
			}
			Display(0);													// �ĕ`��
		}
	} else if(ms==11) {													// <<<DEL>>>
		Tracker1.Reset(0);												// �ׯ��1����
		Line.Reset();													// ײ݉���
		Sphase = 0;
		Display(0);														// �ĕ`��
	} else if(ms==0) {													// <<<��������>>>
		if(GetSelectNo()==0) {											// <<2D�Ȑ�OP��>>
			Si7 = (m_FChar&0x1) ? 0 : 1;								// [F5]:�폜�Ȃ�
			Si8 = (m_FChar&0x2) ? 1 : 0;								// [F6]:�����ύX����
			Si1 = 0, i = 0;
			BaseObjPtr(2, &op, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			while(op!=NULL) {
				i = (GetObjAtr(op)/10!=1) ? 1 : i;						// �Ȑ��ȊO�ʹװ
				if(i) {
					break;												// �װ�͒��~
				}
				SetSelNo(op, 1);										// ��Ȑ����̓o�^
				Si1++;													// �Ȑ�������UP
				NextObjPtr(2, &op, &i1, &i2);							// ���ҏW���̂�
			}
			if(!i&&Si1>0&&Si1<11) {										// <�L���ȏꍇ>
				OffEdtAllObject();										// �ҏW���̑S����
				CountEdtObj();											// �ҏW�Ώۗ��̶���
			} else {													// <�װ�̏ꍇ>
				ResetSelNo();											// ���̑I��ؾ��
				AfxMessageBox(IDS_ERR_SEL8);							// �װү����
			}
			ExitMenu(0, 0);												// �ƭ��I��
		} else {														// <<2D�Ȑ�OP�L>>
			Si7 = (m_FChar&0x1) ? 0 : Si7;								// [F5]:�폜�Ȃ�
			Si8 = (m_FChar&0x2) ? 1 : Si8;								// [F6]:�����ύX����
			Si1 = 0, Si2 = 0, i = 0;
			BaseObjPtr(1, &op, &i1, &i2);								// �\���Ώۗ��̎擾
			while(op!=NULL) {
				if(GetSelNo(op)==1) {
					i = (GetObjAtr(op)/10!=1) ? 1 : i;					// �Ȑ��ȊO�ʹװ
					Si1++;												// ��Ȑ�������UP
				} else if(i2==1) {
					Si2++;												// �ҏW�Ώۗ��̐�����UP
				}
				if(i) {
					break;												// �װ�͒��~
				}
				NextObjPtr(1, &op, &i1, &i2);							// ���\�����̂�
			}
			if(!i&&Si1>0&&Si1<11&&Si2>0) {								// <�L���ȏꍇ>
				OffUndoRedo();											// Undo/Redo�s��
				CheckCtrlKey1();										// ���䷰�m�F(�N����)
				Sphase=0;
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ����No�ݒ�
			} else {													// <�װ�̏ꍇ>
				ResetSelNo();											// ���̑I��ؾ��
				AfxMessageBox(IDS_ERR_SEL8);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==2) {													// <����ݒ�ς�>
			BaseObjPtr(2, &op, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			while(op!=NULL) {
				if(GetSelNo(op)!=1) {									// ����Ȑ��ȊO��
					SetSelNo(op, 2);									// �ҏW�Ώۗ��̓o�^
				}
				NextObjPtr(2, &op, &i1, &i2);							// ���ҏW���̂�
			}
			OffEdtAllObject();											// �ҏW���̑S����
			ObjectLinesEdit(Si3, Si4, Si5, Si8, Spnt1, Spnt2);			// �Ȑ��w��ړ�����
			if(Si7) {													// <��Ȑ��폜����>
				SelBaseObjPtr(1, &op);									// ��Ȑ����̎擾
				while(op!=NULL) {
					op1 = op;											// �폜�����߲���ۑ�
					SelNextObjPtr(1, &op);								// ����Ȑ����̂�
					LevelObj(1, op1);									// �������ٍ폜���̐ڑ�
					TempObj(op1);										// ���̈ꎞ�폜
				}
			}
			SetDataUse();												// �ް��g�p���ݒ�
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		}
		ResetSelNo();													// ���̑I��ؾ��
		Tracker1.Reset(1);												// �ׯ��1����
		Line.Reset();													// ײ݉���
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/*****************************************/
void CJcad3GlbView::MenuSurfaceEdit(void)								//<<< �Ȗʎw��ړ�[���_]
/*****************************************/
{
	OBJTYPE* op;
	int      i1, i2, cnt=0, err=0;

	if(GetSelectNo()==0) {												// <<�Ȗ�OP��>>
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// �Ȑ��ʹװ
			if(err) {
				break;													// �װ�͒��~
			}
			if(++cnt>1) {
				break;													// �Ȗʐ�����UP
			}
			SetSelNo(op, 1);											// ��Ȗʗ��̓o�^
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(err==0&&cnt==1) {											// <�L���ȏꍇ>
			OffEdtAllObject();											// �ҏW���̑S����
			CountEdtObj();												// �ҏW�Ώۗ��̶���
		} else {														// <�װ�̏ꍇ>
			ResetSelNo();												// ���̑I��ؾ��
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
		ExitMenu(0, 0);													// �ƭ��I��
	} else {															// <<�Ȗ�OP�L>>
		BaseObjPtr(1, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// �Ȑ��ʹװ
			} else if(i2==1) {
				cnt++;													// �ҏW�Ώۗ��̐�����UP
				SetSelNo(op, 2);										// �ҏW�Ώۗ��̓o�^
			}
			if(err) {
				break;													// �װ�͒��~
			}
			NextObjPtr(1, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(err==0&&cnt>0) {
			ObjectSurfaceEdit();										// �Ȗʎw��ړ�
		} else {
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
		ResetSelNo();													// ���̑I��ؾ��
		ExitMenu(0, 1);													// �ƭ��I��
	}
	Display(0);															// �ĕ`��
}

/****************************************************************/
void CJcad3GlbView::MouseLineEdit(UINT flg, const CPoint& point,
                                  int plane, int ms)					//<<< ����w��ړ�[���_]
/****************************************************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt, ps[3], pe[3];
	VECTYPE  vec;
	int      i, j, k, l, m, sft, cl[3];
	double   d1, d2, d3;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1) {													// <<���2�_��>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// ���W�_�擾(Spnt2=2�_��)
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ϳ����͐������W
			}
			ps[0] = Spnt1, pe[0] = Spnt2, cl[0] = 0;
			Line.Setup(ps, pe, cl, 1, 5);								// ײݐݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase==3) {											// <<����ύX(�_��S�̈ړ�)>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i = plane;													// ���W�ύX�L�����W
			j = (i+1>2) ? 0 : i+1;
			d1 = pnt.p[i]-Sbase.p[i];									// ���W�ύX�l
			d2 = pnt.p[j]-Sbase.p[j];
			for(k=0; k<Si4; k++) {
				Spy[k] = Spt[k];										// �ׯ�����W����
				Spy[k].p[i] += d1, Spy[k].p[j] += d2;					// �ύX�l���Z
				Spz[k] = Spy[k], Siz[k] = Siy[k];						// ����_����
			}
			Si9 = Si4;
			MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);					// S�Ȑ�����
			for(i=0; i<Si9; i++) {
				Spx[i] = Spz[i];										// ��Ȑ��ۑ�
			}
			InitBasePos(Si14, 0, Si1, Si9, Spx, &Si6, Sia, Sib, Spa);	// ��ړ���������
			SetBaseData(1, Si9, 0, Spz, ps, 3);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);					// �ׯ��1�ݒ�
			for(i=0; i<3; i++) {
				Spw[0].p[i] = (Spy[0].p[i]+Spy[1].p[i])/2.0;			// �_��S�̈ړ��p�ׯ��
				Spw[1].p[i] = (Spy[Si4-2].p[i]+Spy[Si4-1].p[i])/2.0;
			}
			Siw[0] = 4, Siw[1] = 4;										// �_��S�̈ړ��p�ׯ���F
			Tracker2.Setup(Spw, Siw, NULL, 2, 5, 7);					// �ׯ��2�ݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase==4) {											// <<����ύX(����_�ړ�)>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i = plane;													// ���W�ύX�L�����W
			j = (i+1>2) ? 0 : i+1;
			Spy[Si5].p[i] = pnt.p[i], Spy[Si5].p[j] = pnt.p[j];			// ���W�l�ύX
			for(i=0; i<Si10; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// ����_����
			}
			j = Si10;
			MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);						// S�Ȑ�����
			if(Si13) {													// <�Ȑ�:�̏ꍇ>
				Spz[j++] = Spz[0];										// �n�_����_�Ÿ۰��
			}
			EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                        Si6, Sia, Sib, Spa);							// ��ړ��ҏW����
			SetBaseData(1, j, 0, Spz, ps, -1);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);					// �ׯ��1�ݒ�
			for(i=0; i<Si10-1; i++) {
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
			}
			if(Si13) {													// <�Ȑ�:�̏ꍇ>
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
				Si11 = Si10;											// �ǉ��p�ׯ����
			} else {													// <�Ȑ�:�J�̏ꍇ>
				Si11 = Si10-1;											// �ǉ��p�ׯ����
			}
			Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);					// �ׯ��2�ݒ�
			Si8 = 0;													// �S�ړ��ׯ����
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <<���1�_��>>
			CalcScreenWorld(0, &Spnt1, point, plane);					// ���W�l�ۑ�(Spnt1=1�_��)
			Si1 = plane;												// �Ώە��ʕۑ�
			Si2 = (Si1+1>2) ? 0 : Si1+1;
			Si3 = (Si2+1>2) ? 0 : Si2+1;
			Spnt2 = Spnt1;
			Sphase = 1;
		} else if(Sphase==2) {											// <<����ύX>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shift�����
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(Si8) {													// <�S�ړ��ׯ���L�̏ꍇ>
				Si5 = Tracker2.HitTracker(1, plane, pnt, Spw, 2);		// �ׯ��2����
				if(Si5!=-1) {
					Sphase = 3, Sbase = Spw[Si5];
					for(i=0; i<Si4; i++) {
						Spt[i] = Spy[i];								// �ׯ�����W�ۑ�
					}
				}
			}
			if(Sphase==2) {												// <�ׯ���I�𖳂̏ꍇ>
				Si5 = Tracker1.HitTracker(1, plane, pnt, Spy, Si10);	// �ׯ��1����
				if(Si5!=-1) {
					Sphase = 4, Sbase = Spy[Si5];
					if(sft) {
						Siy[Si5] = (Siy[Si5]) ? 0 : 1;					// Shift:ON�͋Ȑ�Ӱ�ޔ��]
					}
					if(!Si13) {											// <�Ȑ�:�J�̏ꍇ>
						Siy[0] = 1, Siy[Si10-1] = 1;					// �n�_��I�_�͋Ȑ�Ӱ��:1
					}
				}
			}
			if(Sphase==2&&Si11>0) {										// <�ׯ���I�𖳂̏ꍇ>
				Si5 = Tracker2.HitTracker(1, plane, pnt, Spw, Si11);	// �ׯ��2����
				if(Si5!=-1) {
					Sbase = Spw[Si5];
					for(i=Si10-1; i>Si5; i--) {
						Spy[i+1] = Spy[i], Siy[i+1] = Siy[i];			// ���ּ��
					}
					Spy[Si5+1] = Sbase;									// �w��ʒu��ǉ�
					Siy[Si5+1] = (sft) ? 1 : 0;
					Si10++;												// �ύX�㐧��_��
					for(i=0; i<Si10; i++) {
						Spz[i] = Spy[i], Siz[i] = Siy[i];				// ����_����
					}
					j = Si10;
					MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);				// S�Ȑ�����
					if(Si13) {											// <�Ȑ�:�̏ꍇ>
						Spz[j++] = Spz[0];								// �n�_����_�Ÿ۰��
					}
					EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                                Si6, Sia, Sib, Spa);					// ��ړ��ҏW����
					SetBaseData(1, j, 0, Spz, ps, -1);					// ����ް��o�^
					Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);			// �ׯ��1�ݒ�
					for(i=0; i<Si10-1; i++) {
						for(j=0; j<3; j++) {
							Spw[i].p[j]=(Spy[i].p[j]+Spy[i+1].p[j])/2.0;// �ǉ��p�ׯ��
						}
						Siw[i] = 5;										// �ǉ��p�ׯ���F
					}
					if(Si13) {											// <�Ȑ�:�̏ꍇ>
						for(j=0; j<3; j++) {
							Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;// �ǉ��p�ׯ��
						}
						Siw[i] = 5;										// �ǉ��p�ׯ���F
						Si11 = Si10;									// �ǉ��p�ׯ����
					} else {											// <�Ȑ�:�J�̏ꍇ>
						Si11 = Si10-1;									// �ǉ��p�ׯ����
					}
					Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);			// �ׯ��2�ݒ�
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <<�������>>
			d1 = Spnt2.p[Si1]-Spnt1.p[Si1];								// �������[��(2�_��-1�_��)
			d2 = Spnt2.p[Si2]-Spnt1.p[Si2];
			d3 = m_Fovy/6000.0;
			if(fabs(d1)>d3||fabs(d2)>d3){								// <<�L�������̏ꍇ>>
				if(m_NumInp!=0) {										// <���l���͂̏ꍇ>
					CInputDlg dlg(13);
					dlg.SetValue(m_LinePNum);							// �����l�\��
					if(dlg.DoModal()==IDOK) {							// �����޲�۸ޕ\��
						dlg.GetValue(&m_LinePNum);						// �������_���擾
						Si4 = m_LinePNum;
					} else {
						Si4 = 0;
						OnUndoRedo();									// Undo/Redo��
						ExitMenu(0, 1);									// �ƭ��I��
					}
				} else {												// <���l���͖��̏ꍇ>
					Si4 = 5;
				}
				if(Si4>0) {												// <��ݾوȊO�̏ꍇ>
					Si13 = 0;											// ���E:�J
					j = Si4-1;
					d1 /= j, d2 /= j;									// ���Ԑ���_�v�Z
					Spy[0] = Spnt1, Spy[j] = Spnt2;
					for(i=1; i<j; i++) {
						Spy[i].p[Si1] = Spnt1.p[Si1]+d1*i;				// ����_�ݒ�
						Spy[i].p[Si2] = Spnt1.p[Si2]+d2*i;
						Spy[i].p[Si3] = Spnt1.p[Si3];
					}
					for(i=0; i<Si4; i++) {
						Spz[i] = Spy[i];								// �ׯ�����W����
						Siz[i] = 0, Siy[i] = 0;							// �Ȑ�Ӱ�ޥ�ׯ���F�ݒ�
					}
					Siy[0] = 1, Siy[Si4-1] = 1;
					Siz[0] = 1, Siz[Si4-1] = 1;
					Si9 = Si4;											// ����_��
					MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);			// S�Ȑ�����
					for(i=0; i<Si9; i++) {
						Spx[i] = Spz[i];								// ��Ȑ��ۑ�
					}
					InitLevel();										// Undo/Redo������
					Si14 = 0;											// ����ړ�Ӱ��
					InitBasePos(Si14, 1, Si1, Si9, Spx,
                                &Si6, Sia, Sib, Spa);					// ��ړ���������
					SetBaseData(1, Si9, 0, Spz, ps, 3);					// ����ް��o�^
					Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);			// �ׯ��1�ݒ�
					Si8 = 1;											// �_��S�̈ړ��p�ׯ���L
					for(i=0; i<3; i++) {
						Spw[0].p[i] = (Spy[0].p[i]+Spy[1].p[i])/2.0;	// �_��S�̈ړ��p�ׯ��
						Spw[1].p[i] = (Spy[Si4-2].p[i]+Spy[Si4-1].p[i])/2.0;
					}
					Siw[0] = 4, Siw[1] = 4;								// �_��S�̈ړ��p�ׯ���F
					Tracker2.Setup(Spw, Siw, NULL, 2, 5, 7);			// �ׯ��2�ݒ�
					Si10 = Si4;											// �ύX�㐧��_��(��)
					Si11 = 0;											// �ǉ��ׯ����
					Sphase = 2;
				}
			} else {													// <���������̏ꍇ>
				Sphase = 0;
			}
			Line.Reset();												// ײ݉���
			Display(0);													// �ĕ`��
		} else if(Sphase==3) {											// <<�_��S�̈ړ�>>
			Sphase = 2;
		} else if(Sphase==4) {											// <<����_�ړ�>>
			for(i=0; i<Si10; i++) {
				Spz[i] = Spy[i], Siz[i] = Siy[i];						// ����_����
			}
			j = Si10;
			MakeSCurve(&j, Si13, 3, 2.0, Spz, Siz);						// S�Ȑ�����
			if(Si13) {													// <�Ȑ�:�̏ꍇ>
				Spz[j++] = Spz[0];										// �n�_����_�Ÿ۰��
			}
			EditBasePos(Si14, Si1, Si7, Si9, Spx, j, Spz,
                        Si6, Sia, Sib, Spa);							// ��ړ��ҏW����
			SetBaseData(1, j, 0, Spz, ps, -1);							// ����ް��o�^
			Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);					// �ׯ��1�ݒ�
			for(i=0; i<Si10-1; i++) {
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
			}
			if(Si13) {													// <�Ȑ�:�̏ꍇ>
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
				Si11 = Si10;											// �ǉ��p�ׯ����
			} else {													// <�Ȑ�:�J�̏ꍇ>
				Si11 = Si10-1;											// �ǉ��p�ׯ����
			}
			Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);					// �ׯ��2�ݒ�
			Si8 = 0;													// �S�ړ��ׯ����
			Display(0);													// �ĕ`��
			Sphase = 2;
		}
	} else if(ms==0) {													// <<<��������>>>
		if(GetSelectNo()==0) {											// <<����OP��>>
			Si7 = (m_FChar&0x1) ? 1 : 0;								// [F5]:�����ύX���׸�
			Si12 = (m_FChar&0x2) ? 1 : 0;								// [F6]:�Ȑ��g�p�׸�
			if(!Si12) {													// <�Ȑ��g�p�Ȃ�>
				OffUndoRedo();											// Undo/Redo�s��
				StartMenu();											// �ƭ��J�n����
				CheckCtrlKey1();										// ���䷰�m�F(�N����)
				Sphase=0;
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ����No�ݒ�
			} else {													// <�Ȑ��g�p����>
				k = 0;
				BaseObjPtr(2, &Sop1, &i, &j);							// �ҏW�Ώۗ��̎擾
				while(Sop1!=NULL) {
					SetSelNo(Sop1, 1);									// �ҏW�Ώۗ��̓o�^
					k++;												// �ҏW�Ώۗ��̐�����UP
					NextObjPtr(2, &Sop1, &i, &j);						// ���ҏW���̂�
				}
				if(k>0) {												// <�L���ȏꍇ>
					OffEdtAllObject();									// �ҏW���̑S����
					CountEdtObj();										// �ҏW�Ώۗ��̶���
				}
				ExitMenu(0, 0);											// �ƭ��I��
			}
		} else {														// <<����OP�L>>
			Si7 = (m_FChar&0x1) ? 1 : Si7;								// [F5]:�����ύX���׸�
			k = 0, l = 0, m = 0;
			BaseObjPtr(1, &Sop2, &i, &j);								// �\���Ώۗ��̎擾
			while(Sop2!=NULL) {
				if(GetSelNo(Sop2)==1) {
					k++;												// �ҏW�Ώۗ��̐�����UP
					OffEdtFlg(1, Sop2);									// edtflg:OFF
				} else if(j==1) {
					Sop1 = Sop2;										// ��Ȑ�OP�ۑ�
					l++;												// ��Ȑ�������UP
					m = (GetObjAtr(Sop2)/10!=1) ? 1 : m;				// �Ȑ��ȊO�ʹװ
				}
				if(m) {
					break;												// �װ�͒��~
				}
				NextObjPtr(1, &Sop2, &i, &j);							// ���\�����̂�
			}
			if(Si12!=1||k==0) {											// <�Ȑ��g�p�ŕҏW���̗L�łȂ��ꍇ>
				ResetSelNo();											// ���̑I��ؾ��
				AfxMessageBox(IDS_ERR_SEL1);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
			} else if(m!=0||l!=1) {										// <��Ȑ���1�{�łȂ��ꍇ>
				ResetSelNo();											// ���̑I��ؾ��
				AfxMessageBox(IDS_ERR_SEL8);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
			} else {													// <�L���ȏꍇ>
				CheckCtrlKey1();										// ���䷰�m�F(�N����)
				m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;		// ����No�ݒ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				Si4 = GetParaInt(cmp, 0);								// ����_���擾
				Si13 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				for(i=0; i<Si4; i++) {
					Siy[i] = GetParaInt(cmp, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp, i, &Spy[i]);						// ����_�擾
				}
				OffUndoRedo();											// Undo/Redo�s��
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
				SetCurve(Si4, Si13, Spy);								// �Ȑ�����_����
				GetObjPtr(&Sop2);										// ����_OP�ۑ�
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				NextCmdPtr(&cmp);										// �Ȑ��ҏW������
				while(cmp!=NULL) {
					RedoCommandCopy(Sop2, cmp);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp);									// ������ނ�
				}
				for(i=0; i<Si4; i++) {
					GetVtx(Sop2, i, &Spy[i]);							// ���̕ҏW�㐧��_�擾
					Spz[i] = Spy[i];									// �ׯ�����W����
					Siz[i] = Siy[i];									// �Ȑ�Ӱ�ނ��ׯ���F��
				}
				FreeObj(Sop2, 1);										// �ҏW�㗧�̍폜
				Delete();												// �Ȑ��폜
				OffEdtAllObject();										// �ҏW���̑S����
				BaseObjPtr(1, &Sop1, &i, &j);							// �\���Ώۗ��̎擾
				while(Sop1!=NULL) {
					if(GetSelNo(Sop1)==1) {								// �ŏ��̑I�𗧑̂�
						OnEdtFlg(1, Sop1);								// edtflg:ON
					}
					NextObjPtr(1, &Sop1, &i, &j);						// ���\�����̂�
				}
				CountEdtObj();											// �ҏW�Ώۗ��̶���
				ResetSelNo();											// ���̑I��ؾ��
				j = 0, k = 0, l = 0, m = 0, d1 = 0.0;
				for(i=1; i<Si4; i++) {
					k = (fabs(Spy[0].p[0]-Spy[i].p[0])>EPSILON) ? 1 : k;
					l = (fabs(Spy[0].p[1]-Spy[i].p[1])>EPSILON) ? 1 : l;
					m = (fabs(Spy[0].p[2]-Spy[i].p[2])>EPSILON) ? 1 : m;
				}
				CalcNVec(Spy, Si4, &vec);								// �@���޸�َ擾
				if(!k&&l&&m) {											// 2D���ʎ擾
					Si1 = 1;
				} else if(!l&&m&&k) {
					Si1 = 2;
				} else if(!m&&k&&l) {
					Si1 = 0;
				} else if(!k&&!l&&m) {
					Si1 = 1;
				} else if(!m&&!k&&l) {
					Si1 = 0;
				} else if(!l&&!m&&k) {
					Si1 = 0;
				} else {
					for(i=0; i<DIM; i++) {
						if(fabs(vec.p[i])>d1) {							// �@���޸�ٍő�����擾
							d1 = fabs(vec.p[i]), j = i;
						}
					}
			        Si1 = (j+1>2) ? 0 : j+1;
				}
				Si2 = (Si1+1>2) ? 0 : Si1+1;
				Si3 = (Si2+1>2) ? 0 : Si2+1;
				Si9 = Si4;												// ����_��
				MakeSCurve(&Si9, Si13, 3, 2.0, Spz, Siz);				// S�Ȑ�����
				if(Si13) {												// <�Ȑ�:�̏ꍇ>
					Spz[Si9++] = Spz[0];								// �n�_����_�Ÿ۰��
				}
				for(i=0; i<Si9; i++) {
					Spx[i] = Spz[i];									// ��Ȑ��ۑ�
				}
				Si14 = 2;												// ����ړ�Ӱ��
				InitBasePos(Si14, 1, Si1, Si9, Spx,
                            &Si6, Sia, Sib, Spa);						// ��ړ���������
				SetBaseData(1, Si9, 0, Spz, ps, 3);						// ����ް��o�^
				Tracker1.Setup(Spy, Siy, NULL, Si4, 5, 7);				// �ׯ��1�ݒ�
				Si8 = 0;												// �_��S�̈ړ��p�ׯ����
				for(i=0; i<Si4-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// �ǉ��p�ׯ��
					}
					Siw[i] = 5;											// �ǉ��p�ׯ���F
				}
				if(Si13) {												// <�Ȑ�:�̏ꍇ>
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;	// �ǉ��p�ׯ��
					}
					Siw[i] = 5;											// �ǉ��p�ׯ���F
					Si11 = Si4;											// �ǉ��p�ׯ����
				} else {												// <�Ȑ�:�J�̏ꍇ>
					Si11 = Si4-1;										// �ǉ��p�ׯ����
				}
				Tracker2.Setup(Spw, Siw, NULL, Si11, 5, 7);				// �ׯ��2�ݒ�
				Si10 = Si4;												// �ύX�㐧��_��(��)
				Sphase = 2;
			}
		}
		Display(0);														// �ĕ`��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==2) {
			EndBasePos();												// ��ړ��I������
		}
		if(Si12) {														// <�Ȑ��g�p��>
			CrtCurve(Si10, Si13, 3, 2.0, Spy, Siy);						// �Ȑ�����(�ҏW��)
			GetObjPtr(&Sop1);											// �Ȑ�����OP�ۑ�
			GetObjOif(Sop1, &oif);										// ���̍\������߲���擾
			if(Si13) {
				SetOifName(0, oif, OBJ_NAME01);							// ���̍\������(�Ȑ�[��])
			} else {
				SetOifName(0, oif, OBJ_NAME02);							//             (�Ȑ�[�J])
			}
		}
		DeleteBaseData();												// ����ް��폜
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/*****************************************************************/
static void SetCirclePoint(int fg, PNTTYPE pt, int i1, int i2,
                           int i3, double d1, double d2,
                           PNTTYPE* py, PNTTYPE* pz, PNTTYPE* px)		//<<< ��~[�O�p�֐�]�ݒ�
/*****************************************************************/
{
	int    i;
	double th, d11, d12, d21, d22;

	d11 = pt.p[i1]-d1, d12 = pt.p[i1]+d1;								// i1���ŏ���ő�
	d21 = pt.p[i2]-d2, d22 = pt.p[i2]+d2;								// i2���ŏ���ő�
	if(fg) {
		for(i=0; i<4; i++) {
			py[i].p[i3] = pt.p[i3];										// �ׯ���ݒ�
		}
	}
	py[0].p[i1] = d11, py[0].p[i2] = d21;
	py[1].p[i1] = d12, py[1].p[i2] = d21;
	py[2].p[i1] = d11, py[2].p[i2] = d22;
	py[3].p[i1] = d12, py[3].p[i2] = d22;
	th = PI/36.0;														// 5�x
	for(i=0; i<72; i++) {
		pz[i].p[i1] = pt.p[i1]+d1*cos(th*i);							// ��~���W�ݒ�
		pz[i].p[i2] = pt.p[i2]+d2*sin(th*i);
		pz[i].p[i3] = pt.p[i3];
	}
	if(fg) {
		pz[i] = pz[0];													// �n�_����_�Ÿ۰��
		pz[i+1] = pt;													// ���S�_�����ǉ�
		for(i=0; i<74; i++) {
			px[i] = pz[i];												// ��Ȑ��ۑ�
		}
	}
}

/**********************************************************************/
static void SetCircleCtrlP(PNTTYPE pt, int i1, int i2, int i3,
                           int* pn, double d1, double d2, double* da,
                           double* db, PNTTYPE* pw, PNTTYPE* py,
                           PNTTYPE* pz, PNTTYPE* px, int* iy, int* iz)	//<<< ��~[�Ȑ�]�ݒ�
/**********************************************************************/
{
	int    i, cn;
	double th;

	cn = *pn;															// ����_��
	th = 2.0*PI/(double)cn;												// ����_���̊p�x
	for(i=0; i<cn; i++) {
		pw[i].p[i1] = pt.p[i1]+d1*cos(th*i);							// ��~���W�ݒ�
		pw[i].p[i2] = pt.p[i2]+d2*sin(th*i);
		pw[i].p[i3] = pt.p[i3];
		iy[i] = 0;														// �Ȑ�Ӱ��
	}
	SetCurveMatrix1(1, 2.0, 1, cn, iy, da, db);							// �ʉߓ_=>����_�ϊ���د��
	CalcCurvePoint(1, cn, pw, py, db);									// �ʉߓ_=>����_�ϊ�
	for(i=0; i<cn; i++) {
		pz[i] = py[i], iz[i] = iy[i];
	}
	MakeSCurve(&cn, 1, 3, 2.0, pz, iz);									// S�Ȑ�����
	pz[cn] = pz[0];														// �n�_����_�Ÿ۰��
	pz[cn+1] = pt;														// ���S�_�����ǉ�
	for(i=0; i<cn+2; i++) {
		px[i] = pz[i];													// ��Ȑ��ۑ�
	}
	*pn = cn+1;															// ���_��
}

/******************************************************************/
void CJcad3GlbView::MouseCircleEdit(UINT flg, const CPoint& point,
                                    int plane, int ms)					//<<< ��~�w��ړ�[���_]
/******************************************************************/
{
	PNTTYPE pnt, wk[3];
	int     i, j, k, sft;
	double  th, d1, d2, dd[4];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1) {													// <<��~2���>>
			CalcScreenWorld(0, &Spnt2, point, plane);					// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spnt2, &Spnt1);						// ϳ����͐������W
			}
			DrawCircle(Spnt1, Spnt2, Si1);								// �~�\��
			Display(0);													// �ĕ`��
		} else if(Sphase==3) {											// <<�S�ړ�>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i = plane;													// �Ώە���
			j = (i+1>2) ? 0 : i+1;
			Spnt1.p[i] = pnt.p[i], Spnt1.p[j] = pnt.p[j];				// �L�����W��ύX
			if(Si7==0) {												// <��~[�O�p�֐�]�w��>
				SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                               Sd1, Sd2, Spy, Spz, Spx);				// ��~[�O�p�֐�]�ݒ�
				InitBasePos(1, 0, Si1, Si4, Spx, &Si6, Sia, Sib, Spa);	// ��ړ���������
				SetBaseData(1, Si4, 0, Spz, wk, 3);						// ����ް��o�^
				Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);				// �ׯ��1�ݒ�
				Sps[0] = Spz[18], Sps[1] = Spz[54];
				Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);				// �ׯ��2�ݒ�
				Spt[0] = Spz[0], Spt[1] = Spz[36];
				Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);				// �ׯ��3�ݒ�
			} else {													// <�Ȑ�[��]�w��>
				d1 = pnt.p[i]-Sbase.p[i];								// ���W�ύX�l
				d2 = pnt.p[j]-Sbase.p[j];
				for(k=0; k<Si8; k++) {
					Spy[k] = Spt[k];									// �ׯ�����W����
					Spy[k].p[i] += d1, Spy[k].p[j] += d2;				// �ύX�l���Z
					Spz[k] = Spy[k], Siz[k] = Siy[k];					// ����_����
				}
				Si4 = Si8;
				MakeSCurve(&Si4, 1, 3, 2.0, Spz, Siz);					// S�Ȑ�����
				Spz[Si4] = Spz[0];										// �n�_����_�Ÿ۰��
				Spz[Si4+1] = Spnt1;										// ���S�_�����ǉ�
				for(k=0; k<Si4+2; k++) {
					Spx[k] = Spz[k];									// ��Ȑ��ۑ�
				}
				Si4++;													// ���_��
				InitBasePos(2, 0, Si1, Si4, Spx, &Si6, Sia, Sib, Spa);	// ��ړ���������
				SetBaseData(1, Si4, 0, Spz, wk, 3);						// ����ް��o�^
				Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);				// �ׯ��1�ݒ�
			}
			Spw[0] = Spnt1;
			Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);					// �ׯ��4�ݒ�
			Display(0);													// �ĕ`��
		} else if(Sphase>3) {											// <<�ό`>>
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			if(Si7==0) {												// <��~[�O�p�֐�]�w��>
				if(Sphase==4) {
					Sd1 = fabs(pnt.p[Si1]-Spnt1.p[Si1]);				// Si1�����a
					Sd2 = fabs(pnt.p[Si2]-Spnt1.p[Si2]);				// Si2�����a
				} else if(Sphase==5) {
					if(Si5==0) {
						Sd3 = pnt.p[Si3]-Spnt1.p[Si3];					// Si2���ʂ�Si3���ړ���1
						Sd4 = (flg&m_NLiKey) ? Sd3 : Sd4;				// �ϓ�
					} else {
						Sd4 = pnt.p[Si3]-Spnt1.p[Si3];					// Si2���ʂ�Si3���ړ���2
						Sd3 = (flg&m_NLiKey) ? Sd4 : Sd3;				// �ϓ�
					}
				} else if(Sphase==6) {
					if(Si5==0) {
						Sd5 = pnt.p[Si3]-Spnt1.p[Si3];					// Si3���ʂ�Si3���ړ���1
						Sd6 = (flg&m_NLiKey) ? Sd5 : Sd6;				// �ϓ�
					} else {
						Sd6 = pnt.p[Si3]-Spnt1.p[Si3];					// Si3���ʂ�Si3���ړ���2
						Sd5 = (flg&m_NLiKey) ? Sd6 : Sd5;				// �ϓ�
					}
				}
				SetCirclePoint(0, Spnt1, Si1, Si2, Si3,
                               Sd1, Sd2, Spy, Spz, Spx);				// ��~[�O�p�֐�]�ݒ�
				dd[0] = Sd3, dd[1] = Sd4, dd[2] = Sd5, dd[3] = Sd6;		// �ő�ړ��ʌ���
				for(d1=0.0, j=0, i=0; i<4; i++) {
					if(fabs(dd[i])>d1) {
						d1 = fabs(dd[i]), j = i;
					}
				}
				th = PI/36.0;											// 5�x
				if(j>1) {												// <Si1�������ψʗʂŒ���>
					d1 = Sd5-Sd3;										// �ψʗ�
					for(i=0; i<18; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd5-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd6-Sd3;										// �ψʗ�
					for(i=18; i<36; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd6-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd6-Sd4;										// �ψʗ�
					for(i=36; i<54; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd6-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd5-Sd4;										// �ψʗ�
					for(i=54; i<72; i++) {
						d2 = (1.0-fabs(cos(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd5-d1*sin(d2);	// ��~���W(Si3��)
					}
				} else {												// <Si2�������ψʗʂŒ���>
					d1 = Sd3-Sd6;										// �ψʗ�
					for(i=18; i<36; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd3-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd4-Sd6;										// �ψʗ�
					for(i=36; i<54; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd4-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd4-Sd5;										// �ψʗ�
					for(i=54; i<72; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd4-d1*sin(d2);	// ��~���W(Si3��)
					}
					d1 = Sd3-Sd5;										// �ψʗ�
					for(i=0; i<18; i++) {
						d2 = (1.0-fabs(sin(th*i)))*PI/2.0;				// Si1�������̔䗦
						Spz[i].p[Si3] = Spnt1.p[Si3]+Sd3-d1*sin(d2);	// ��~���W(Si3��)
					}
				}
				Spz[72] = Spz[0];
				EditBasePos(1, Si1, 0, Si4, Spx, Si4, Spz,
                            Si6, Sia, Sib, Spa);						// ��ړ��ҏW����
				SetBaseData(1, Si4, 0, Spz, wk, -1);					// ����ް��o�^
				Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);				// �ׯ��1�ݒ�
				Sps[0] = Spz[18], Sps[1] = Spz[54];
				Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);				// �ׯ��2�ݒ�
				Spt[0] = Spz[0], Spt[1] = Spz[36];
				Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);				// �ׯ��3�ݒ�
				Tracker4.Reset(0);										// �ׯ��4����
			} else {													// <�Ȑ�[��]�w��>
				i = plane;												// ���W�ύX�L�����W
				j = (i+1>2) ? 0 : i+1;
				Spy[Si5].p[i] = pnt.p[i], Spy[Si5].p[j] = pnt.p[j];		// ���W�l�ύX
				for(i=0; i<Si10; i++) {
					Spz[i] = Spy[i], Siz[i] = Siy[i];					// ����_����
				}
				k = Si10;
				MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);					// S�Ȑ�����
				Spz[k++] = Spz[0];										// �n�_����_�Ÿ۰��
				EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                            Si6, Sia, Sib, Spa);						// ��ړ��ҏW����
				SetBaseData(1, k, 0, Spz, wk, -1);						// ����ް��o�^
				Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);				// �ׯ��1�ݒ�
				for(i=0; i<Si10-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// �ǉ��p�ׯ��
					}
					Siw[i] = 5;											// �ǉ��p�ׯ���F
				}
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
				Si11 = Si10;											// �ǉ��p�ׯ����
				Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);				// �ׯ��4�ݒ�
			}
			Si9 = 0;													// �S�ړ��ׯ����
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==0) {													// <<��~1���>>
			CalcScreenWorld(0, &Spnt1, point, plane);					// ��]���S�_���W�擾
			Si1 = plane;												// �Ώە��ʕۑ�
			Si2 = (Si1+1>2) ? 0 : Si1+1;
			Si3 = (Si2+1>2) ? 0 : Si2+1;
			Spnt2 = Spnt1;
			if(m_NumInp!=0) {											// <���l���͂̏ꍇ>
				CInputDlg dlg(14+Si1);
				dlg.SetValue(m_CircleSize1, m_CircleSize2);				// �����l�\��
				if(dlg.DoModal()==IDOK) {								// �����޲�۸ޕ\��
					dlg.GetValue(&m_CircleSize1, &m_CircleSize2);		// ��~���ގ擾
					Sd1 = m_CircleSize1*0.5;							// Si1�����a
					Sd2 = m_CircleSize2*0.5;							// Si2�����a
					Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Sd6 = 0.0;			// Si3���ړ���
					if(Si7==0) {										// <��~[�O�p�֐�]�w��>
						SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                                       Sd1, Sd2, Spy, Spz, Spx);		// ��~[�O�p�֐�]�ݒ�
						Si4 = 73;
						InitLevel();									// Undo/Redo������
						InitBasePos(1, 1, Si1, Si4,
                                    Spx, &Si6, Sia, Sib, Spa);			// ��ړ���������
						SetBaseData(1, Si4, 0, Spz, wk, 3);				// ����ް��o�^
						Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);		// �ׯ��1�ݒ�
						Sps[0] = Spz[18], Sps[1] = Spz[54];
						Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);		// �ׯ��2�ݒ�
						Spt[0] = Spz[0], Spt[1] = Spz[36];
						Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);		// �ׯ��3�ݒ�
					} else {											// <�Ȑ�[��]�w��>
						Si4 = Si8;										// �w�萧��_��
						SetCircleCtrlP(Spnt1, Si1, Si2, Si3, &Si4,
                                       Sd1, Sd2, Sda, Sdb, Spw,
                                       Spy, Spz, Spx, Siy, Siz);		// ��~[�Ȑ�]�ݒ�
						InitLevel();									// Undo/Redo������
						InitBasePos(2, 1, Si1, Si4,
                                    Spx, &Si6, Sia, Sib, Spa);			// ��ړ���������
						SetBaseData(1, Si4, 0, Spz, wk, 3);				// ����ް��o�^
						Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);		// �ׯ��1�ݒ�
						Si10 = Si8;										// �ύX�㐧��_��(��)
						Si11 = 0;										// �ǉ��ׯ����
					}
					Si9 = 1;											// �S�ړ��ׯ���L
					Spw[0] = Spnt1;
					Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);			// �ׯ��4�ݒ�
					Display(0);											// �ĕ`��
					Sphase = 2;
				} else {
					OnUndoRedo();										// Undo/Redo��
					ExitMenu(0, 1);										// �ƭ��I��
				}
			} else {													// <ϳ����͂̏ꍇ>
				Sphase = 1;
			}
		} else if(Sphase==2) {											// <<<����ύX>>>
			sft = (flg&MK_SHIFT) ? 1 : 0;								// Shift�����
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if(Si7==0) {												// <<��~[�O�p�֐�]�w��>>
				if(Si9) {												// <�S�ړ��ׯ���L�̏ꍇ>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, 1);	// �ׯ��4����
					if(Si5!=-1) {
						Sphase = 3, Sbase = Spw[Si5];
					}
				}
				if(Sphase==2) {											// <�S�ړ��ׯ���I�𖳂̏ꍇ>
					if(plane==Si1) {
						Si5=Tracker1.HitTracker(1, plane, pnt, Spy, 4);	// �ׯ��1����
						if(Si5!=-1) {
							Sphase = 4, Sbase = Spy[Si5];
						}
					} else if(plane==Si2) {
						Si5=Tracker2.HitTracker(1, plane, pnt, Sps, 2);	// �ׯ��2����
						if(Si5!=-1) {
							Sphase = 5, Sbase = Sps[Si5];
						}
					} else if(plane==Si3) {
						Si5=Tracker3.HitTracker(1, plane, pnt, Spt, 2);	// �ׯ��3����
						if(Si5!=-1) {
							Sphase = 6, Sbase = Spt[Si5];
						}
					}
				}
			} else {													// <<�Ȑ�[��]�w��>>
				if(Si9) {												// <�S�ړ��ׯ���L�̏ꍇ>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, 1);	// �ׯ��4����
					if(Si5!=-1) {
						Sphase = 3, Sbase = Spw[Si5];
						for(i=0; i<Si8; i++) {
							Spt[i] = Spy[i];							// �ׯ�����W�ۑ�
						}
					}
				}
				if(Sphase==2) {											// <�ׯ���I�𖳂̏ꍇ>
					Si5 = Tracker1.HitTracker(1, plane, pnt, Spy, Si10);// �ׯ��1����
					if(Si5!=-1) {
						Sphase = 4, Sbase = Spy[Si5];
						if(sft) {
							Siy[Si5] = (Siy[Si5]) ? 0 : 1;				// Shift:ON�͋Ȑ�Ӱ�ޔ��]
						}
					}
				}
				if(Sphase==2&&Si11>0) {									// <�ׯ���I�𖳂̏ꍇ>
					Si5 = Tracker4.HitTracker(1, plane, pnt, Spw, Si11);// �ׯ��4����
					if(Si5!=-1) {
						Sbase = Spw[Si5];
						for(i=Si10-1; i>Si5; i--) {
							Spy[i+1] = Spy[i], Siy[i+1] = Siy[i];		// ���ּ��
						}
						Spy[Si5+1] = Sbase;								// �w��ʒu��ǉ�
						Siy[Si5+1] = (sft) ? 1 : 0;
						Si10++;											// �ύX�㐧��_��
						for(i=0; i<Si10; i++) {
							Spz[i] = Spy[i], Siz[i] = Siy[i];			// ����_����
						}
						k = Si10;
						MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);			// S�Ȑ�����
						Spz[k++] = Spz[0];								// �n�_����_�Ÿ۰��
						EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                                    Si6, Sia, Sib, Spa);				// ��ړ��ҏW����
						SetBaseData(1, k, 0, Spz, wk, -1);				// ����ް��o�^
						Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);		// �ׯ��1�ݒ�
						for(i=0; i<Si10-1; i++) {
							for(j=0; j<3; j++) {
								Spw[i].p[j] = 
                                      (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// �ǉ��p�ׯ��
							}
							Siw[i] = 5;									// �ǉ��p�ׯ���F
						}
						for(j=0; j<3; j++) {
							Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;// �ǉ��p�ׯ��
						}
						Siw[i] = 5;										// �ǉ��p�ׯ���F
						Si11 = Si10;									// �ǉ��p�ׯ����
						Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);		// �ׯ��4�ݒ�
						Display(0);										// �ĕ`��
					}
				}
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		if(Sphase==1) {													// <<��~����>>
			Sd1 = Spnt2.p[Si1]-Spnt1.p[Si1];							// ���a�̕ψʗ�
			Sd2 = Spnt2.p[Si2]-Spnt1.p[Si2];
			Sd3 = m_Fovy/12000.0;
			if(fabs(Sd1)>Sd3||fabs(Sd2)>Sd3){							// <�L�������̏ꍇ>
				Sd1 = sqrt(Sd1*Sd1+Sd2*Sd2);							// ���a
				Sd2 = Sd1;
				Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Sd6 = 0.0;				// Si3���ړ���
				if(Si7==0) {											// <��~[�O�p�֐�]�w��>
					SetCirclePoint(1, Spnt1, Si1, Si2, Si3,
                                   Sd1, Sd2, Spy, Spz, Spx);			// ��~[�O�p�֐�]�ݒ�
					Si4 = 73;
					InitLevel();										// Undo/Redo������
					InitBasePos(1, 1, Si1, Si4,
                                Spx, &Si6, Sia, Sib, Spa);				// ��ړ���������
					SetBaseData(1, Si4, 0, Spz, wk, 3);					// ����ް��o�^
					Tracker1.Setup(Spy, Siy, Tens, 4, Si1, 7);			// �ׯ��1�ݒ�
					Sps[0] = Spz[18], Sps[1] = Spz[54];
					Tracker2.Setup(Sps, Sis, TensC, 2, Si2, 7);			// �ׯ��2�ݒ�
					Spt[0] = Spz[0], Spt[1] = Spz[36];
					Tracker3.Setup(Spt, Sis, Six, 2, Si3, 7);			// �ׯ��3�ݒ�
				} else {												// <�Ȑ�[��]�w��>
					Si4 = Si8;											// �w�萧��_��
					SetCircleCtrlP(Spnt1, Si1, Si2, Si3, &Si4,
                                   Sd1, Sd2, Sda, Sdb, Spw,
                                   Spy, Spz, Spx, Siy, Siz);			// ��~[�Ȑ�]�ݒ�
					InitLevel();										// Undo/Redo������
					InitBasePos(2, 1, Si1, Si4,
                                Spx, &Si6, Sia, Sib, Spa);				// ��ړ���������
					SetBaseData(1, Si4, 0, Spz, wk, 3);					// ����ް��o�^
					Tracker1.Setup(Spy, Siy, NULL, Si8, 5, 7);			// �ׯ��1�ݒ�
					Si10 = Si8;											// �ύX�㐧��_��(��)
					Si11 = 0;											// �ǉ��ׯ����
				}
				Si9 = 1;												// �S�ړ��ׯ���L
				Spw[0] = Spnt1;
				Tracker4.Setup(Spw, Siw, NULL, 1, 5, 7);				// �ׯ��4�ݒ�
				Sphase = 2;
			} else {													// <���������̏ꍇ>
				Sphase = 0;
			}
			DeleteCircle();												// �~�j��
			Display(0);													// �ĕ`��
		} else if(Sphase>2) {
			if(Si7==1&&Sphase==4) {										// <�Ȑ�[��]�w��Ő���_�ړ�>
				for(i=0; i<Si10; i++) {
					Spz[i] = Spy[i], Siz[i] = Siy[i];					// ����_����
				}
				k = Si10;
				MakeSCurve(&k, 1, 3, 2.0, Spz, Siz);					// S�Ȑ�����
				Spz[k++] = Spz[0];										// �n�_����_�Ÿ۰��
				EditBasePos(2, Si1, 0, Si4, Spx, k, Spz,
                            Si6, Sia, Sib, Spa);						// ��ړ��ҏW����
				SetBaseData(1, k, 0, Spz, wk, -1);						// ����ް��o�^
				Tracker1.Setup(Spy, Siy, NULL, Si10, 5, 7);				// �ׯ��1�ݒ�
				for(i=0; i<Si10-1; i++) {
					for(j=0; j<3; j++) {
						Spw[i].p[j] = (Spy[i].p[j]+Spy[i+1].p[j])/2.0;	// �ǉ��p�ׯ��
					}
					Siw[i] = 5;											// �ǉ��p�ׯ���F
				}
				for(j=0; j<3; j++) {
					Spw[i].p[j] = (Spy[i].p[j]+Spy[0].p[j])/2.0;		// �ǉ��p�ׯ��
				}
				Siw[i] = 5;												// �ǉ��p�ׯ���F
				Si11 = Si10;											// �ǉ��p�ׯ����
				Tracker4.Setup(Spw, Siw, NULL, Si11, 5, 7);				// �ׯ��4�ݒ�
				Si9 = 0;												// �S�ړ��ׯ����
				Display(0);												// �ĕ`��
			}
			Sphase = 2;
		}
	} else if(ms==0) {													// <<<��������>>>
		Si7 = (m_FChar&0x1) ? 1 : 0;									// [F5]:�Ȑ�[����_]�׸�
		OffUndoRedo();													// Undo/Redo�s��
		StartMenu();													// �ƭ��J�n����
		CheckCtrlKey1();												// ���䷰�m�F(�N����)
		Sphase=0;
		Siy[0] = 0, Siy[1] = 0, Siy[2] = 0, Siy[3] = 0;					// �ׯ���F�ݒ�
		Sis[0] = 0, Sis[1] = 0, Siw[0] = 4;
		Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;				// �ׯ����߼�ݐݒ�
		TensC[0] = 6, TensC[1] = 6, Six[0] = 5, Six[1] = 5;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
		if(Si7!=0) {													// <�Ȑ�[����_]�̏ꍇ>
			CInputDlg dlg(28);
			dlg.SetValue(m_CirclePNum);									// �����l�\��
			if(dlg.DoModal()==IDOK) {									// �����޲�۸ޕ\��
				dlg.GetValue(&m_CirclePNum);							// ��~����_���擾
				Si8 = m_CirclePNum;
			} else {
				OnUndoRedo();											// Undo/Redo��
				ExitMenu(0, 1);											// �ƭ��I��
			}
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==2) {
			EndBasePos();												// ��ړ��I������
		}
		DeleteBaseData();												// ����ް��폜
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Tracker3.Reset(1);												// �ׯ��3����
		Tracker4.Reset(1);												// �ׯ��4����
		Display(0);														// �ĕ`��
		OnUndoRedo();													// Undo/Redo��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/************************************************************/
void CJcad3GlbView::MousePCut(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< ���ʕ���
/************************************************************/
{
	OBJTYPE *op;
	CString fname, path;
	int     i1, i2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(0, &Spb[0], point, plane);					// ������(ײ�)�I�_
			if(fabs(Spa[0].p[0]-Spb[0].p[0])>EPSILON||
               fabs(Spa[0].p[1]-Spb[0].p[1])>EPSILON||
               fabs(Spa[0].p[2]-Spb[0].p[2])>EPSILON) {
				if(flg&m_LimKey) {
					LimitPoint(plane, &Spb[0], &Spa[0]);				// ϳ����͐������W
				}
				Line.Setup(Spa, Spb, Sia, 1, plane);					// ������(ײ�)�ݒ�
				Sphase = 1;
				Display(0);												// �ĕ`��
			}
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		CalcScreenWorld(0, &Spa[0], point, plane);						// ������(ײ�)�n�_
	} else if(ms==3) {													// <<ϳ��̍��t���>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Spb[0], point, plane);					// ������(ײ�)�I�_
			if(flg&m_LimKey) {
				LimitPoint(plane, &Spb[0], &Spa[0]);					// ϳ����͐������W
			}
			Line.Setup(Spa, Spb, Sia, 1, plane);						// ������(ײ�)�ݒ�
			Display(0);													// �ĕ`��
			if(AfxMessageBox(IDS_RUN_CONFIRM, MB_YESNO)==IDYES) {		// <�m�F:OK>
				CCurveDiviDlg dlg;
				dlg.SetValue(m_CurveDvd, m_PolygonF, m_FileBUpF);
				if(dlg.DoModal()==IDOK) {								// �Ȗʍו����ݒ��޲�۸ޕ\��
					dlg.GetValue(&m_CurveDvd, &m_PolygonF, &m_FileBUpF);
					if(m_FileBUpF>0&&GetDemoFlg()==0) {					// <̧���ޯ����ߗL�Ő��i��>
						fname.Format(PCB_NAME, m_PCutCnt);				// �ޯ�����̧�ٖ�
						path = m_SaveFilePath+fname;					// ���߽
						if(WriteJgdFile(path, 2)!=0) {					// JGD�`��̧�ٕۑ�
							AfxMessageBox(IDS_ERR_FILEWRITE);			// �̧�ٕۑ�:NG�
						}
						if(++m_PCutCnt>=m_SaveFileMax) {
							m_PCutCnt = 0;
						}
					}
					if(m_CurveDvd>0) {									// <�Ȗʍו����L>
						OnCurveReptSW(m_CurveDvd);						// �Ȗʍו���SW��
						BaseObjPtr(2, &op, &i1, &i2);					// �ҏW�����߲���擾
						while(op!=NULL) {
							RedoCommand(op);							// ����ލĎ��s
							SetDispList(op);							// �ި���ڲ�ؽēo�^
							NextObjPtr(2, &op, &i1, &i2);				// ���ҏW���̂�
						}
						Display(0);										// �ĕ`��
					}
					if(m_PolygonF>0) {									// <�O�p�߯����L>
						if(MakePolygonData(1)!=0) {						// ��غ���ް��쐬
							AfxMessageBox(IDS_ERR_POLYGON);				// ���غ�݉�:NG�
						}
					}
					StartMenu();										// �ƭ��J�n����
					InitLevel();										// Undo/Redo������
					PCutObject(Spa[0], Spb[0], plane);					// ���̂̕��ʕ���
					ExitMenu(0, 0);										// �ƭ��I��
					EndJMenu1();										// �ƭ��I������
					if(m_CurveDvd>0) {									// <�Ȗʍו����L>
						OffCurveReptSW();								// �Ȗʍו���SW��
					}
				}
			}
		}
		Line.Reset();													// ײ݉���
		Display(0);														// �ĕ`��
		Sphase = 0;														// ̪��޸ر
	} else if(ms==0) {													// <<<��������>>>
		Sia[0] = 0;														// ײݶװNo
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/*************************************/
void CJcad3GlbView::MenuBoolean(void)									//<<< �}�`���Z
/*************************************/
{
	OBJTYPE* op;
	CString  fname, path;
	int      i1, i2, err=0, cnt1=0, cnt2=0, mode=-1, delb=0;

	if(GetSelectNo()==0) {												// <<�}�`���ZOP��>>
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			err = (GetObjAtr(op)/10==1) ? 1 : err;						// �Ȑ��ʹװ
			if(err) {
				break;													// �װ�͒��~
			}
			SetSelNo(op, 1);											// �}�`���Z���̓o�^
			cnt1++;														// ����UP
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		if(!err&&cnt1>0) {												// <�L���ȏꍇ>
			OffEdtAllObject();											// �ҏW���̑S����
		} else {														// <�װ�̏ꍇ>
			ResetSelNo();												// ���̑I��ؾ��
			AfxMessageBox(IDS_ERR_SEL1);								// �װү����
		}
	} else {															// <<�}�`���ZOP�L>>
		BaseObjPtr(1, &op, &i1, &i2);									// �\���Ώۗ��̎擾
		while(op!=NULL) {
			if(GetSelNo(op)==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// �Ȑ��ʹװ
				cnt1++;													// �f�ʋȐ�����UP
			} else if(i2==1) {
				err = (GetObjAtr(op)/10==1) ? 1 : err;					// �Ȑ��ʹװ
				cnt2++;													// �|��������UP
			}
			if(err) {
				break;													// �װ�͒��~
			}
			NextObjPtr(1, &op, &i1, &i2);								// ���\�����̂�
		}
		if(!err&&cnt1>0&&cnt2>0) {										// <�L���ȏꍇ>
			if(m_Char=='1') {											// 1�������͉��Z1
				mode = 0;
			} else if(m_Char=='2') {									// 2�������͉��Z2
				mode = 1;
			} else if(m_Char=='3') {									// 3�������͉��Z3
				mode = 2;
			} else {
				CBooleanDlg dlg;
				if(dlg.DoModal()==IDOK) {								// �޲�۸ޕ\��
					dlg.GetValue(&mode, &delb);							// �}�`���Z��ގ擾
				}
			}
			if(mode>=0) {												// <���Z�w��OK>
				CCurveDiviDlg dlg;
				dlg.SetValue(m_CurveDvd, m_PolygonF, m_FileBUpF);
				if(dlg.DoModal()==IDOK) {								// �Ȗʍו����ݒ��޲�۸ޕ\��
					dlg.GetValue(&m_CurveDvd, &m_PolygonF, &m_FileBUpF);
					if(m_FileBUpF>0&&GetDemoFlg()==0) {					// <̧���ޯ����ߗL�Ő��i��>
						fname.Format(BLB_NAME, m_BoolCnt);				// �ޯ�����̧�ٖ�
						path = m_SaveFilePath+fname;					// ���߽
						if(WriteJgdFile(path, 2)!=0) {					// JGD�`��̧�ٕۑ�
							AfxMessageBox(IDS_ERR_FILEWRITE);			// �̧�ٕۑ�:NG�
						}
						if(++m_BoolCnt>=m_SaveFileMax) {
							m_BoolCnt = 0;
						}
					}
					if(m_CurveDvd>0) {									// <�Ȗʍו����L>
						OnCurveReptSW(m_CurveDvd);						// �Ȗʍו���SW��
						SelBaseObjPtr(1, &op);							// A�����̎擾
						while(op!=NULL) {
							RedoCommand(op);							// ����ލĎ��s
							SetDispList(op);							// �ި���ڲ�ؽēo�^
							SelNextObjPtr(1, &op);						// �����̂�
						}
						BaseObjPtr(2, &op, &i1, &i2);					// B�������߲���擾
						while(op!=NULL) {
							if(GetSelNo(op)!=1) {
								RedoCommand(op);						// ����ލĎ��s
								SetDispList(op);						// �ި���ڲ�ؽēo�^
							}
							NextObjPtr(2, &op, &i1, &i2);				// ���ҏW���̂�
						}
						Display(0);										// �ĕ`��
					}
					if(m_PolygonF>0) {									// <�O�p�߯����L>
						if(MakePolygonData(0)!=0) {						// A����غ���ް��쐬
							AfxMessageBox(IDS_ERR_POLYGON);				// ���غ�݉�:NG�
						}
						if(MakePolygonData(1)!=0) {						// B����غ���ް��쐬
							AfxMessageBox(IDS_ERR_POLYGON);				// ���غ�݉�:NG�
						}
					}
					StartMenu();										// �ƭ��J�n����
					InitLevel();										// Undo/Redo������
					BooleanObject(mode, delb);							// ���̐}�`���Z
					EndJMenu1();										// �ƭ��I������
					if(m_CurveDvd>0) {									// <�Ȗʍו����L>
						OffCurveReptSW();								// �Ȗʍו���SW��
					}
				}
			}
		} else {
			AfxMessageBox(IDS_ERR_SEL1);								// �����ȏꍇ,�װү����
		}
		ResetSelNo();													// ���̑I��ؾ��
	}
	CountEdtObj();														// �ҏW�Ώۗ��̶���
	Display(0);															// �ĕ`��
}
