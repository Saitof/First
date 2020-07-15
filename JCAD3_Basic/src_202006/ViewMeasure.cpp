/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewMeasure.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "Select.h"
#include "EditSurface.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "NCVL32_Api.h"

/***********************************************************************/
void CJcad3GlbView::MouseMeasureDistance(UINT flg, const CPoint& point,
                                         int plane, int ms)				//<<< ��������
/***********************************************************************/
{
	PNTTYPE pnt;
	int     i, i1, i2;
	double  dd[3], ln;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==3) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�l�ۑ�
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i1 = plane;													// �L�����W���ݒ�
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si1].p[i1] = pnt.p[i1], Spa[Si1].p[i2] = pnt.p[i2];		// ���W�l�ύX
			for(i=0; i<DIM; i++) {
				dd[i] = fabs(Spa[1].p[i]-Spa[0].p[i]);					// �덷�擾
			}
			ln = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);				// �����擾
			Tracker1.Setup(Spa, Sia, NULL, 2, 4, 3);					// �ׯ��1�ݒ�
			m_pNumDisp->SetParam(dd[0], dd[1], dd[2], ln);				// ϳ��ړ��ʕ\��
			Display(0);													// �ĕ`��
			Si2 = 1;													// ϳ��ړ��ς�
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {													// <<1���>>
			CalcScreenWorld(0, &Spa[0], point, plane);					// ���W�l�ۑ�
			Spa[1] = Spa[0];
			Si1 = 1;													// �ړ��Ώ۔ԍ�
			Sphase = 3;
			Si2 = 0, Sbase = Spa[Si1];									// ϳ��ړ��׸ޏ�����
		} else if(Sphase==2) {											// <<2��ڈȍ~>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�l�ۑ�
			if((Si1=Tracker1.HitTracker(1, plane, pnt, Spa, 2))!=-1) {	// �ׯ��1����
				Sphase = 3, Sbase = Spa[Si1];
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		Sphase = (Sphase==3) ? 2 : Sphase;
		Sphase = (!Si2) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL(���)>>>
		m_pNumDisp->SetParam(0.0, 0.0, 0.0, 0.0);						// �\���l������
		for(i=0; i<DIM; i++) {
			Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0;						// ����_������
		}
		Tracker1.Reset(0);												// �ׯ��1����
		Display(0);														// �ĕ`��
		Sphase = 1;
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			SetNumDisp(1);												// �޲�۸ޕ\��
			m_pNumDisp->SetParam(0.0, 0.0, 0.0, 0.0);					// �\���l������
			for(i=0; i<DIM; i++) {
				Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0;					// ����_������
			}
			Sia[0] = 4, Sia[1] = 4;										// �ׯ���F������
			Sphase = 1;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		SetNumDisp(0);													// �޲�۸ޏ���
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureAngle(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< �p�x����
/********************************************************************/
{
	PNTTYPE pnt;
	VECTYPE v1, v2;
	int     i, i1, i2;
	double	dd[3], th, ln;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2||Sphase==4) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�l�ۑ�
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i1 = plane;													// �L�����W���ݒ�
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si1].p[i1] = pnt.p[i1];									// ���W�l�ύX
			Spa[Si1].p[i2] = pnt.p[i2];
			if(Sphase==2) {
				for(i=0; i<DIM; i++) {
					dd[i] = fabs(Spa[1].p[i]-Spa[0].p[i]);				// 3�����̋���
					v1.p[i] = Spa[1].p[i]-Spa[0].p[i];					// �޸�َ擾
				}
				ln = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);			// �����擾
				VecN(&v1);												// �P���޸�ى�
				th = atan2(v1.p[i2], v1.p[i1]);							// i1������̊p�x
				th = (th<-10||th>10) ? 0.0 : th;
				th += PI/18.0;											// 10�x�����Z
				Spa[2] = Spa[0];										// ���S�_����
				Spa[2].p[i1] += ln*cos(th);								// i1���̍��W
				Spa[2].p[i2] += ln*sin(th);								// i2���̍��W
				Si2 = 1;												// ϳ��ړ��ς�
			}
			for(i=0; i<DIM; i++) {
				v1.p[i] = Spa[1].p[i]-Spa[0].p[i];						// ���S�_�����
				v2.p[i] = Spa[2].p[i]-Spa[0].p[i];						// �޸�َ擾
			}
			VecN(&v1);													// �P���޸�ى�
			VecN(&v2);
			if(fabs(v1.p[0]-v2.p[0])>EPSILON||							// 2�޸�يԂ̊p�x
               fabs(v1.p[1]-v2.p[1])>EPSILON||
               fabs(v1.p[2]-v2.p[2])>EPSILON) {
				th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			th = th/PI*180.0;											// �p�x
			Tracker1.Setup(Spa, Sia, NULL, 3, 4, 3);					// �ׯ��1�ݒ�
			m_pNumDisp->SetParam(th);									// ϳ��p�x�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {													// <<1���>>
			CalcScreenWorld(0, &Spa[0], point, plane);					// ���W�l�ۑ�
			Spa[1] = Spa[0], Spa[2] = Spa[0];
			Si1 = 1;													// �ړ��Ώ۔ԍ�
			Sphase = 2;
			Si2 = 0, Sbase = Spa[Si1];									// ϳ��ړ��׸ޏ�����
		} else if(Sphase==3) {											// <<2��ڈȍ~>>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�l�ۑ�
			if((Si1=Tracker1.HitTracker(1, plane, pnt, Spa, 3))!=-1) {	// �ׯ��1����
				Sphase = 4, Sbase = Spa[Si1];
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		Sphase = (Sphase==2||Sphase==4) ? 3 : Sphase;
		Sphase = (!Si2) ? 1 : Sphase;
	} else if(ms==11) {													// <<<DEL(���)>>>
		m_pNumDisp->SetParam(0.0);										// �\���l������
		for(i=0; i<DIM; i++) {
			Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0, Spa[2].p[i] = 0.0;	// ����_������
		}
		Tracker1.Reset(0);												// �ׯ��1����
		Display(0);														// �ĕ`��
		Sphase = 1;
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			SetNumDisp(1);												// �޲�۸ޕ\��
			m_pNumDisp->SetParam(0.0);									// �\���l������
			for(i=0; i<DIM; i++) {
				Spa[0].p[i] = 0.0, Spa[1].p[i] = 0.0, Spa[2].p[i] = 0.0;// ����_������
			}
			Sia[0] = 5, Sia[1] = 4, Sia[2] = 4;							// �ׯ���F������
			Sphase = 1;
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		SetNumDisp(0);													// �޲�۸ޏ���
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureCurve(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< �Ȑ���������
/********************************************************************/
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
						if(GetSurfaceCurve(Si1, Si2, Si3, Si4, Si7, Si8,
                                           m_ObjCRpt, Siy[0], Sop1,
                                           &Si11, &Si12, Spw)) {		// �ȖʋȐ��擾
							for(dd=0.0, i=1; i<Si11; i++) {
								dd += PntD(&Spw[i-1], &Spw[i]);			// ���_�ԋ����W�v
							}
							if(Si12) {									// <�Ȑ�(��)�̏ꍇ>
								dd += PntD(&Spw[Si11-1], &Spw[0]);		// �I�_-�n�_�ԋ����W�v
							}
							m_pNumDisp->SetParam(dd);					// �Ȑ�����
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
					if(GetSurfaceCurve(Si1, Si2, Si3, Si4, Si7, Si8,
                                       m_ObjCRpt, Siy[0], Sop1,
                                       &Si11, &Si12, Spw)) {			// �ȖʋȐ��擾
						for(dd=0.0, i=1; i<Si11; i++) {
							dd += PntD(&Spw[i-1], &Spw[i]);				// ���_�ԋ����W�v
						}
						if(Si12) {										// <�Ȑ�(��)�̏ꍇ>
							dd += PntD(&Spw[Si11-1], &Spw[0]);			// �I�_-�n�_�ԋ����W�v
						}
						m_pNumDisp->SetParam(dd);						// �Ȑ�����
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
			Si7 = 0, Si8 = 0, Si10 = 0, Si21 = 0, Si22 = 0;
			BaseObjPtr(2, &Sop1, &i1, &i2);								// �ҏW�Ώۗ��̎擾
			if(Sop1!=NULL) {											// <�ҏW���̗L�̏ꍇ>
				BaseCmdPtr(1, Sop1, &cmp);								// �L���擪����ގ擾
				i3 = GetObjAtr(Sop1);									// ���̑����擾
			}
			if((i3==20||i3==21)&&cmp!=NULL) {							// <<�ʏ�Ȗʂ̏ꍇ>>
				InitTRLimit();											// �ׯ���͈͌��菉����
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				GetSurfCmd1(cmp, &Si1, &Si2, &Si3, &Si4, &m_ObjCRpt,
                            &m_ObjCWgt, &Si5, Spa, Sia);				// �Ȗʺ���ގ擾1
				StartMenu();											// �ƭ��J�n����
				InitLevel();											// Undo/Redo������
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
				Tracker1.Setup(Spw, Sic, NULL, Si6, 4, 7);				// �ׯ��1�ݒ�
				Sphase = 1;												// ����_�w���
				SetNumDisp(1);											// �޲�۸ޕ\��
				m_pNumDisp->SetParam(0.0);								// �\���l������
				Display(0);												// �ĕ`��
			} else if(i3/10==1&&cmp!=NULL) {							// <<�Ȑ��̏ꍇ>>
				OffEdtOtherObject(Sop1);								// �w��O�ҏW�Ώۉ���
				Si1 = GetParaInt(cmp, 0);								// ����_���擾
				Si2 = GetParaInt(cmp, 1);								// ���EӰ�ގ擾
				Si3 = GetVtxNum(Sop1);									// ���̒��_���擾
				for(i=0; i<Si3; i++) {
					GetVtx(Sop1, i, &Spw[i]);							// �w�蒸�_�擾
				}
				for(dd=0.0, i=1; i<Si3; i++) {
					dd += PntD(&Spw[i-1], &Spw[i]);						// ���_�ԋ����W�v
				}
				if(Si2) {												// <�Ȑ�(��)�̏ꍇ>
					dd += PntD(&Spw[Si3-1], &Spw[0]);					// �I�_-�n�_�ԋ����W�v
				}
				SetNumDisp(1);											// �޲�۸ޕ\��
				m_pNumDisp->SetParam(dd);								// �\���l������
				Display(0);												// �ĕ`��
			} else {													// <<�ҏW����:NG>>
				AfxMessageBox(IDS_ERR_SEL5);							// �װү����
				ExitMenu(0, 0);											// �ƭ��I��
			}
		}
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Sphase==1) {													// <�Ȗʎw��̏ꍇ>
			if(Sop2!=NULL) {
				FreeObj(Sop2, 1);										// ���̍폜
			}
			Tracker1.Reset(1);											// �ׯ��1����
			Tracker2.Reset(1);											// �ׯ��2����
			Line.Reset();												// ײ݉���
			ResetSelNo();												// ���̑I��Noؾ��
			OnUndoRedo();												// Undo/Redo��
			InitLineNo(NULL);											// ײݔԍ�������
			Undo(gno);													// �ҏW�Ȃ���Undo
		}
		SetNumDisp(0);													// �޲�۸ޏ���
		ExitMenu(0, 0);													// �ƭ��I��
		Display(0);														// �ĕ`��
	}
}

/********************************************************************/
void CJcad3GlbView::MouseMeasureScale(UINT flg, const CPoint& point,
                                      int plane, int ms)				//<<< ��K
/********************************************************************/
{
	int    i1, i2;
	double d1, d2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Sbase, point, plane);					// ���W�l�ۑ�
			if(flg&m_LimKey) {
				LimitPoint(plane, &Sbase, &BaseCursor);					// ϳ����͐������W
			}
			DrawBox(BaseCursor, Sbase, plane);							// BOX�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		i1 = plane;														// �L�����W���擾
		i2 = (plane+1>2) ? 0 : plane+1;
		d1 = fabs(Sbase.p[i1]-BaseCursor.p[i1]);						// ���W�������̒���
		d2 = fabs(Sbase.p[i2]-BaseCursor.p[i2]);
		if(d1>=1.0&&d2>=1.0) {											// <�L���Ȼ���>
			m_ScalePlane = plane;										// �\�����ʓo�^
			MeasureSCALE(BaseCursor, Sbase);							// ��K�o�^
		}
		DeleteBox();													// ��`�j��
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==1) {													// <<<ϳ����t����>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<��������>>>
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==8) {													// <<<���ƭ��N��>>>
		ExitMenu(0, 0);													// �ƭ��I��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		DeleteSCALE();													// ��K�p��
		Display(0);														// �ĕ`��
		m_ActMenu = m_SaveMenu;											// �N�����ƭ��֖߂�
		m_SaveMenu = NONE;												// �ۑ��ƭ��ԍ��ر
		m_CursorNo = m_CursorNo_S;										// ����No����
		m_CursorNo_S = 0;												// �ۑ�����No�ر
		InitChar();														// �����͏�����
	}
}

/*******************************************************************/
void CJcad3GlbView::MouseMeasureProt(UINT flg, const CPoint& point,
                                     int plane, int ms)					//<<< ���x��
/*******************************************************************/
{
	int    i1, i2;
	double ln, l1, l2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==1) {
			CalcScreenWorld(0, &Sbase, point, plane);					// ���W�l�ۑ�(���a�I�_)
			if(flg&m_LimKey) {
				LimitPoint(plane, &Sbase, &BaseCursor);					// ϳ����͐������W
			}
			DrawCircle(BaseCursor, Sbase, plane);						// �~�\��
			Display(0);													// �ĕ`��
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		i1 = plane;														// �L�����W���擾
        i2 = (plane+1>2) ? 0 : plane+1;
		l1 = Sbase.p[i1]-BaseCursor.p[i1];								// ���W�������̒���
		l2 = Sbase.p[i2]-BaseCursor.p[i2];
		ln = sqrt(l1*l1+l2*l2);											// �����̒���
		if(ln>=2.0) {													// <�L���Ȼ���>
			m_ProtPlane = plane;										// �\�����ʓo�^
			MeasurePROT(BaseCursor, Sbase);								// ���x��o�^
		}
		DeleteCircle();													// �~�j��
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==1) {													// <<<ϳ����t����>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<��������>>>
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==8) {													// <<<���ƭ��N��>>>
		ExitMenu(0, 0);													// �ƭ��I��
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		DeletePROT();													// ���x��p��
		Display(0);														// �ĕ`��
		m_ActMenu = m_SaveMenu;											// �N�����ƭ��֖߂�
		m_SaveMenu = NONE;												// �ۑ��ƭ��ԍ��ر
		m_CursorNo = m_CursorNo_S;										// ����No����
		m_CursorNo_S = 0;												// �ۑ�����No�ر
		InitChar();														// �����͏�����
	}
}

/*******************************************/
void CJcad3GlbView::MenuMeasureVolume(void)								//<<< �̐ϑ���
/*******************************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      ret, i1, i2;

	BaseObjPtr(2, &op, &i1, &i2);										// �\�������߲���擾
	if(op==NULL) {														// <���̖��͒��~>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// �ް��ް��擪�߲���擾
	ret = VolumeObject(op, vtx, flp);									// ���̑̐ϑ���
	if(ret==-2) {														// ���~
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(ret==2) {													// ��شװ�ż��ђ�~
		MemErr(IDS_MEMERR5);
	}
}
