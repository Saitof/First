/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewNCDisp.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "FileDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "NumFunc.h"
#include "NCFA32_Api.h"
#include "NCRV32_Api.h"
#include "NCSM32_Api.h"
#include "NCET32_Api.h"
#include "SLCA32_Api.h"

/***************************************/
static int DisplayNCData(CString tname)									//<<< NC�ް��\��
/***************************************/
{
	char*   bf = new char[100];
	MSG     msg;
	FILE*   fp;
	PNTTYPE pts, pte;
	DWORD   wt=0;
	int     ret=0, i, x, y, z, dm, flg=0, col=0;

	if(fopen_s(&fp, tname, "r")==0) {									// ���̧�ٵ����
		fscanf_s(fp, "%s", bf, 100);									// ����ް��Ǎ���
		if(!strcmp(bf, "A")) {
			fscanf_s(fp, "%d%d%d%s", &dm, &dm, &dm, bf, 100);			// �ޯ�����ގ擾
			for(i=0; i<DIM; i++) {										// ��Ɨ̈揉����
				pts.p[i] = 0.0, pte.p[i] = 0.0;
			}
			if(!strcmp(bf,"B")) {
				fscanf_s(fp, "%d%s", &dm, bf, 100);						// �ϑw�Ԋu�擾
			}
			while(!feof(fp)){
				if(!strcmp(bf,"E")) {									// <���`�ړ�>
					fscanf_s(fp, "%d%d%s", &x, &y, bf, 100);			// ���W�Ǎ���
					pte.p[0] = x/1000.0, pte.p[1] = y/1000.0;
					View->SetNCDisp(pts, pte);							// NC�ް��o�^
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;			// ��ײݗp�ۑ�
					flg = 1;
				} else if(!strcmp(bf, "G")) {							// <�����ړ�[Z��]>
					fscanf_s(fp, "%d%d%s", &x, &y, bf, 100);			// ���W�Ǎ���
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;			// ��ײݗp�ۑ�
				} else if(!strcmp(bf, "F")) {							// <�����ړ�[Z�L]>
					fscanf_s(fp, "%d%d%d%s", &x, &y, &z, bf, 100);		// ���W�Ǎ���
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;
					pts.p[2] = z/1000.0, pte.p[2] = z/1000.0;
					if((!feof(fp))&&flg) {								// <���ԑw�̏I��>
						View->DispNCDisp(col);							// NC�ް��\��
						Sleep(wt);										// �҂�
						col = (!col) ? 1 : 0;							// �\���F�ϊ�
						flg = 0;
						if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
							GetMessage(&msg, NULL, 0, 0);
							if(msg.message==WM_RBUTTONUP) {				// �����I��
								ret = 1; break;
							} else if(msg.message==WM_LBUTTONUP) {		// �҂����Ԑݒ�
								if(wt<100) {
									wt = 100;							// 1���:0.1�b�҂�
								} else if(wt<300) {
									wt = 300;							// 2���:0.3�b�҂�
								} else if(wt<500) {
									wt = 500;							// 3���:0.5�b�҂�
								} else {
									wt = 0;								// 4���:0.0�b�҂�
								}
							}
						}
						View->RestoreWaitCursor();						// ���ĥ���ٍĕ\��
					}
				} else {
					fscanf_s(fp, "%s", bf, 100);						// ����ް��Ǎ���
				}
			}
			if(flg) {
				View->DispNCDisp(col);									// NC�ް��\��
			}
		}
		fclose(fp);														// ���̧�ٸ۰��
	}
	delete[] bf;
	return ret;
}

/**********************************************************************/
int CJcad3GlbView::DisplayNCProc(int mode, CString path, BOXTYPE* box)	//<<< NC�ް��\��&���`���ԗ\��
/**********************************************************************/
{
	char*   bf = new char[100];
	FILE*   fp;
	PNTTYPE cpt;
	BOXTYPE bx;
	CString pname;
	int     ret1, ret2, i, x, y, z;

	if(mode==0) {														// <�\����ʒ�������>
		if(fopen_s(&fp, path, "r")==0) {								// ���̧�ٵ����
			for(i=0; i<DIM; i++) {										// �ޯ�����ސݒ�
				bx.bp[0].p[i] = 0.0;
			}
			fscanf_s(fp, "%s%d%d%d", bf, 100, &x, &y, &z);
			bx.bp[1].p[0] = x/1000.0;
			bx.bp[1].p[1] = y/1000.0;
			bx.bp[1].p[2] = z/1000.0;
			m_Fovy = (int)((BoxCenter(bx, &cpt)*1000.0)/0.8+0.5);		// �ޯ������&�ő啝�擾
			ScrToPnt(1, &m_Cent3D, &cpt);
			m_pState->SetCent3D(cpt);
			SetSeishaei(m_Cent3D, m_Fovy);								// ���ˉe�s��č\�z
			ChangeTexPixels();											// ø����߸�ٍX�V
			if(m_GridDisp) {
				SetGrid();												// ��د�ސݒ�
			}
			CentPars();													// �����}�Q�Ɠ_�ύX
			fclose(fp);													// ���̧�ٸ۰��
		}
	} else if(mode==2) {												// <�\����ʒ�������(SLC)>
		bx = *box;
		m_Fovy = (int)((BoxCenter(bx, &cpt)*1000.0)/0.8+0.5);			// �ޯ������&�ő啝�擾
		ScrToPnt(1, &m_Cent3D, &cpt);
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		CentPars();														// �����}�Q�Ɠ_�ύX
	}
	CConfirmDlg  dlg;
	while(TRUE) {
		Display(0);														// �ĕ`��
		BeginWaitCursor();												// ���ĥ���ٕ\��
		InitNCDisp();													// NC�ް��\����������
		ret2 = DisplayNCData(path);										// NC�ް��\��
		EndNCDisp();													// NC�ް��\���I������
		EndWaitCursor();												// ���ĥ���ُ���
		if(ret2) {														// ���~
			break;
		}
		if(dlg.DoModal()==IDOK) {										// �޲�۸ޕ\��
			break;
		}
	}
	ret1 = dlg.m_EndMode;												// �\���I��Ӱ��
	if(ret2) {															// ���~
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(!m_NNuInp&&!ret1) {										// <�޲�۸ޕ\������>
		pname = GetAppPath(0)+NCET_NAME;								// �ް��ۑ�̧��PATH�擾
		ret2 = EstimateTime(path, pname);								// ���`���ԗ\��
		if(ret2>0) {													// �ް��װ
			AfxMessageBox(IDS_ERR_FILE);
		} else if(ret2<0) {												// ���~
			AfxMessageBox(IDS_STOP_PROCESS);
		}
	}
	delete[] bf;
	return ret1;
}

/***************************************/
void CJcad3GlbView::MenuNCDisplay(void)									//<<< NC�ް��\��
/***************************************/
{
	CString fname, tname, ext="nc", filter;
	DWORD   ffg;
	BOXTYPE box;
	int     i, mode, ret1, ret2=0;

	CheckCtrlKey1();													// ���䷰�m�F(�N����)
	tname = GetAppPath(0)+NCT_NAME;										// NC���̧�ٖ�
	if(!CheckFilePath(1, tname)) {										// <NC���̧�ٖ��̏ꍇ>
		if(!CheckFilePath(0, m_NCFilePath)) {							// �߽�m�F
			m_NCFilePath = GetAppPath(0);
		}
		ffg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;						// �NÇ�ق��J���
		filter.LoadString(IDS_FILE_FILTER106);
		CFileDlg dlg(TRUE, ext, fname, ffg, filter);					// �޲�۸ޕ\��
		dlg.m_ofn.lpstrInitialDir = m_NCFilePath;
		if(dlg.DoModal()==IDOK) {
			ext = dlg.GetFileExt();										// ̧�يg���q�擾
			fname = dlg.GetPathName();									// ̧�ٖ�(���߽)�擾
			i = fname.ReverseFind('\\');
			m_NCFilePath = fname.Left(i+1);								// �߽�ۑ�
		} else {
			return;
		}
		if(!ext.CompareNoCase("slc")) {									// <SLC�`��>
			ret1 = ChgSLCFile(fname, tname, &box);						// SLÇ�ٕϊ�
			mode = 2;													// �\����ʒ����L(SLC)
		} else {														// <NC�`��>
			ret1 = ChgNCFile(fname, tname);								// NÇ�ٕϊ�
			mode = 0;													// �\����ʒ����L(NC)
		}
		if(ret1==0) {													// <NÇ�ٕϊ�����I��>
			ret2 = DisplayNCProc(mode, tname, &box);					// NC�ް��\��&���`���ԗ\��
		} else {
			if(ret1>0) {												// �ް��װ
				AfxMessageBox(IDS_ERR_FILE);
			} else {													// ���~
				AfxMessageBox(IDS_STOP_PROCESS);
			}
		}
	} else {															// <NC���̧�ٖ��̏ꍇ>
		ret2 = DisplayNCProc(1, tname, NULL);							// NC�ް��\��&���`���ԗ\��
	}
	if(!ret2) {
		remove(tname);													// ���̧�ٍ폜
	}
}

/****************************************/
void CJcad3GlbView::MenuSimulation(void)								//<<< �Эڰ���
/****************************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      i1, i2, rvr, smr, dsr=0;
	CString  path, pname;

	CheckCtrlKey1();													// ���䷰�m�F(�N����)
	BaseObjPtr(2, &op, &i1, &i2);										// �\�������߲���擾
	if(op==NULL) {														// <���̖����~>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// �ް��ް��擪�߲���擾
	pname = GetAppPath(0)+NCRV_NAME;									// �ް��ۑ�̧��PATH�擾
	rvr = ReviseObject(op, vtx, flp, pname);							// ���̐��@�␳
	if(rvr==-2) {														// �����r���Œ��~
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(rvr==2) {													// ��شװ�ż��ђ�~
		MemErr(IDS_MEMERR5);
	}
	if(rvr==1||rvr==0) {
		path = GetAppPath(0)+NCT_NAME;									// NC���̧�ٖ�
		pname = GetAppPath(0)+NCSM_NAME;								// �ް��ۑ�̧��PATH�擾
		smr = SimulateMakeNC(op, vtx, flp, path, pname);				// NC�쐬�Эڰ���
		if(smr==-2) {													// �����r���Œ��~
			AfxMessageBox(IDS_STOP_PROCESS);
		} else if(smr==0) {												// �ް��װ
			AfxMessageBox(IDS_ERR_FILE);
		} else if(smr==2) {												// ��شװ�ż��ђ�~
			MemErr(IDS_MEMERR5);
		}
		if(smr==1) {
			dsr = DisplayNCProc(1, path, NULL);							// NC�ް��\��&���`���ԗ\��
		}
		if(smr!=-1&&!dsr) {
			remove(path);												// ���̧�ٍ폜
		}
	}
	if(rvr==1||rvr==-2) {												// <���@�␳����>
		BeginWaitCursor();												// ���ĥ���ٕ\��
		BaseObjPtr(2, &op, &i1, &i2);									// �ҏW�����߲���擾
		while(op!=NULL) {
			RedoCommand(op);											// ����ލĎ��s
			SetDispList(op);											// �ި���ڲ�ؽēo�^
			NextObjPtr(2, &op, &i1, &i2);								// ���ҏW���̂�
		}
		EndWaitCursor();												// ���ĥ���ُ���
	}
}
