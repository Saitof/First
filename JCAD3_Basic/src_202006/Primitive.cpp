/////////////////////////////////////////////////////////////////////////////
// �v���~�e�B�u���͕�
// Primitive.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "ECommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"

/************************************************************/
static BOOL InputBoxSize(double* xd, double* yd, double* zd)			//<<< �ޯ�����ޓ���
/************************************************************/
{
    CInputDlg dlg(2);
    dlg.SetValue(*xd, *yd, *zd);                                        // �����l�\��
    if(dlg.DoModal()==IDOK) {                                           // ���l�����޲�۸ޕ\��
        dlg.GetValue(xd, yd, zd);                                       // �ޯ�����ގ擾
        return TRUE;
    } else {
        return FALSE;
    }
}

/**************************/
int ReadPrimitive(int pno)												//<<< ����è�ޓ���
/**************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  oct, dct;
	BOXTYPE  box;
	VECTYPE  vec;
	CString  fname;
	int      id;
	double   dd, dx, dy, dz=0.0;

    View->CheckCtrlKey1();                                              // ���䷰�m�F(�N����)
    if(pno<2) {                                                         // <<2D����è��>>
        dx = View->GetDispHeight()/2.0;                                 // ��ʒ������擾
        dy = View->GetDispHeight()/2.0;
        if(View->GetNumInp()!=0) {                                      // <���l���͂���>
            dx = View->GetPrimXSize2D();                                // �����l�擾
            dy = View->GetPrimYSize2D();
            dz = View->GetPrimZSize2D();
            if(InputBoxSize(&dx, &dy, &dz)) {                           // �ޯ�����ޓ���
                View->SetPrimXSize2D(dx);                               // �����l�ݒ�
                View->SetPrimYSize2D(dy);
                View->SetPrimZSize2D(dz);
            } else {
                dx = 0.0, dy = 0.0, dz = 0.0;
            }
        }
        if(dx>EPSILON&&dy>EPSILON) {                                    // X,Y���L����Z�𖳌�
            dz = 0.0;
        }
        if(dx*dy<EPSILON&&dy*dz<EPSILON&&dz*dx<EPSILON) {               // �L����2�����͖���
            dx = 0.0, dy = 0.0, dz = 0.0;
        }
    } else {                                                            // <<3D����è��>>
        dx = View->GetDispHeight()/2.0;                                 // ��ʒ��̔����擾
        dy = View->GetDispHeight()/2.0;
        dz = View->GetDispHeight()/2.0;
        if(View->GetNumInp()!=0) {                                      // <���l���͂���>
            dx = View->GetPrimXSize3D();                                // �����l�擾
            dy = View->GetPrimYSize3D();
            dz = View->GetPrimZSize3D();
            if(InputBoxSize(&dx, &dy, &dz)) {                           // �ޯ�����ޓ���
                View->SetPrimXSize3D(dx);                               // �����l�ݒ�
                View->SetPrimYSize3D(dy);
                View->SetPrimZSize3D(dz);
            } else {
                dx = 0.0, dy = 0.0, dz = 0.0;
            }
        }
        if(dx<EPSILON||dy<EPSILON||dz<EPSILON) {                        // 1���ł�0�͑S�Ė���
            dx = 0.0, dy = 0.0, dz = 0.0;
        }
    }
	if(dx>EPSILON||dy>EPSILON||dz>EPSILON) {							// <�w�跬ݾوȊO>
		if(pno==0) {
			fname = GetAppPath(0)+CIR_NAME;								// ̧���߽�擾(�~)
		} else if(pno==1) {
			fname = GetAppPath(0)+SQU_NAME;								//            (�����`)
		} else if(pno==2) {
			fname = GetAppPath(0)+SPH_NAME;								//            (����)
		} else if(pno==3) {
			fname = GetAppPath(0)+CUB_NAME;								//            (������)
		} else if(pno==4) {
			fname = GetAppPath(0)+CYL_NAME;								//            (�~��)
		} else {
			fname = GetAppPath(0)+CON_NAME;								//            (�~��)
		}
		StartMenu();													// �ƭ��J�n����
		InitLevel();													// Undo/Redo������
		if((id=ReadJsdFile(fname, 0, 0))!=0) {							// ����è�ޓ���
			return id;
		}
		OffEdtAllObject();												// �ҏW���̑S����
		GetObjPtr(&op);													// �����߲���擾
		GetObjOif(op, &oif);											// ���̍\������߲���擾
		if(pno==0) {
			SetOifName(1, oif, OBJ_NAME13);								// ���̍\������(�~)
		} else if(pno==1) {
			SetOifName(1, oif, OBJ_NAME14);								//             (�����`)
		} else if(pno==2) {
			SetOifName(1, oif, OBJ_NAME15);								//             (����)
		} else if(pno==3) {
			SetOifName(1, oif, OBJ_NAME16);								//             (������)
		} else if(pno==4) {
			SetOifName(1, oif, OBJ_NAME17);								//             (�~��)
		} else {
			SetOifName(1, oif, OBJ_NAME18);								//             (�~��)
		}
		OnEdtFlg(1, op);												// �ҏW���̐ݒ�
		GetObjBox(op, &box);											// �ޯ�����ގ擾
		dd = BoxCenter(box, &oct);										// �ޯ�����S&�����擾
		View->GetCentPnt(&dct);											// ��ʒ��S�擾
		OnCmdFlgStp();													// ���ۑ��׸ލX�V���~ON
		if(dx<EPSILON) {												// <2D��Y-Z�w��̏ꍇ>
			vec.p[0] = 0.0, vec.p[1] = 1.0, vec.p[2] = 0.0;				// ��]����Y��
			Rotate(oct, vec, 90.0);										// 90�x��]
			dx = dd;
		} else if(dy<EPSILON) {											// <2D��Z-X�w��̏ꍇ>
			vec.p[0] = 1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;				// ��]����X��
			Rotate(oct, vec, 90.0);										// 90�x��]
			dy = dd;
		} else if(dz<EPSILON) {											// <2D��X-Y�w��̏ꍇ>
			dz = dd;
		}
		dx = dx/dd, dy = dy/dd, dz = dz/dd;								// X/Y/Z�����{���Z�o
		if(fabs(dx-1.0)>EPSILON||
           fabs(dy-1.0)>EPSILON||
           fabs(dz-1.0)>EPSILON) {										// <�g��/�k�����K�v�ȏꍇ>
			Scale(oct, dx, dy, dz);										// ��ʻ��ޔ�����ػ���
		}
		dx = dct.p[0] - oct.p[0];										// X�����ړ��ʎZ�o
		dy = dct.p[1] - oct.p[1];										// Y�����ړ��ʎZ�o
		dz = dct.p[2] - oct.p[2];										// Z�����ړ��ʎZ�o
		if(fabs(dx)>EPSILON||fabs(dy)>EPSILON||fabs(dz)>EPSILON) {		// <XYZ�̈ړ����K�v�ȏꍇ>
			Move(dx, dy, dz);											// ��ʒ��S�ֈړ�
		}
		OffCmdFlgStp();													// ���ۑ��׸ލX�V���~OFF
		EndJMenu1();													// �ƭ��I������
		View->MenuChgSurfParam(1);										// �Ȑ����Ұ��ύX
	}
	return 0;
}
