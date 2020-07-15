/////////////////////////////////////////////////////////////////////////////
// ̧�ٓ��o�͕�2
// DataFile2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "Command.h"
#include "Attribute.h"
#include "DataConv.h"
#include "InputsDlg.h"

CProgressDlg* m_pProgressF2=NULL;										// ��۸�ڽ�ݼ޹���

/*********************************************/
int WriteJgdFile(const char* fname, int sflg)							//<<< JGD/GGḐ�ُo��
/*********************************************/
{
	char*    ccn = new char[100];
	char*    name = new char[ONM_LENG+2];
	char     wpwd[11], rpwd[11];
	MSG      msg;
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pp;
	OIFDATA  buf;
	FILE*    fp;
	CString  tname;
	int      ret=0, savf, cno, inum, pnum, i, j, cnt, mflg, oflg;
	int      dx, dy, dz, oct, lvl, num, scc=0, ccc=0, spr=0, oifmode;
	int      obno, clvl, mono, mlvl, min_obno=INT_MAX, min_clvl=INT_MAX;
	double   dpr;

	if(sflg==2) {														// <<���ʕ���/�}�`���Z�ޯ�����(JGD)>>
		oifmode = 3;													// �ҏW����/selno=1����
		memset(wpwd, NULL, sizeof(wpwd));								// �����߽ܰ�ޏ�����
		memset(rpwd, NULL, sizeof(rpwd));								// �Ǎ��߽ܰ�ޏ�����
	} else {															// <<�ʏ�(JGD/GGD)̧�ُo��>>
		oifmode = 1;													// �\������
		MainWnd->GetPasswordData(wpwd, rpwd, &savf);					// �߽ܰ���ް��擾
		if(!savf) {														// �߽ܰ�ޕۑ�:����
			memset(wpwd, NULL, sizeof(wpwd));							// �����߽ܰ�ޏ�����
			memset(rpwd, NULL, sizeof(rpwd));							// �Ǎ��߽ܰ�ޏ�����
		}
	}
	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&fp, tname, "w")!=0) {									// ���̧��OPEN
		delete[] ccn;
		delete[] name;
		return 1;
	}

	m_pProgressF2 = new CProgressDlg(3);
	m_pProgressF2->Create();											// �޲�۸ޕ\��
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	BaseOifPtr(oifmode, 0, &oif);										// �\�������̐擪�擾
	while(oif) {
		GetOifObj(oif, &op);											// �\����񗧑��߲���擾
		obno = GetObjNo(op);											// ����No�擾
		min_obno = (obno<min_obno) ? obno : min_obno;					// �ŏ��̗���No����
		BaseCmdPtr(0, op, &cmp);										// �ް�����ސ擪�擾
		while(cmp) {
			clvl = GetCmdLvl(cmp);										// ���������No�擾
			min_clvl = (clvl<min_clvl) ? clvl : min_clvl;				// �ŏ��̺��������No����
			scc++;														// ����ސ����Z
			NextAllCmdPtr(&cmp);										// ������ނ�
		}
		NextOifPtr(oifmode, 0, &oif);									// ���\�������̂�
	}
	m_pProgressF2->SetProgress(5);										// �i���\��(5%)
	sprintf_s(ccn, 100, "C%02d", WPWDCMD);								// �����߽ܰ�޺����
	fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d\n",
                ccn, wpwd[0], wpwd[1], wpwd[2], wpwd[3], wpwd[4],
                wpwd[5], wpwd[6], wpwd[7], wpwd[8], wpwd[9]);			// ����ޏ�񏑍�
	sprintf_s(ccn, 100, "C%02d", RPWDCMD);								// �Ǎ��߽ܰ�޺����
	fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d\n",
                ccn, rpwd[0], rpwd[1], rpwd[2], rpwd[3], rpwd[4],
                rpwd[5], rpwd[6], rpwd[7], rpwd[8], rpwd[9]);			// ����ޏ�񏑍�
	InitOifNo();														// ���̍\������No������
	if(sflg==1) {														// <�ް��W(*.gld)>
		sprintf_s(ccn, 100, "C%02d", GLBDCMD);							// �ް��W�擪�����
		fprintf(fp, "%s\n", ccn);										// ����ޏ�񏑍�
	}
	mflg = FALSE;														// �ړ��׸ޏ�����(OFF)
	oct = 0;
	BaseOifPtr(oifmode, 0, &oif);										// �\�������̐擪�擾
	while(oif) {
		oflg = FALSE;													// �o���׸ޏ�����(OFF)
		SetOifNo(oif, oct++);											// ���̍\������No�ݒ�
		GetOifObj(oif, &op);											// �\����񗧑��߲���擾
		obno = GetObjNo(op);											// ����No�擾
		BaseCmdPtr(0, op, &cmp);										// �ް�����ސ擪�擾
		while(cmp) {
			clvl = GetCmdLvl(cmp);										// ���������No�擾
			oflg = TRUE;												// �o���׸�ON
			cno = GetCmdNo(cmp);										// �����No�擾
			inum = GetIntNum(cmp);										// �������Ұ����擾
			pnum = GetPntNum(cmp);										// ���W���Ұ����擾
			if(cno==MOVE) {												// <�ړ�����ނ̏ꍇ>
				if(mflg) {												// <�O����ނ��ړ�>
					dx += GetParaInt(cmp, 0);							// �ړ��ʉ��Z
					dy += GetParaInt(cmp, 1);
					dz += GetParaInt(cmp, 2);
				} else {												// <�O����ނ��ړ��łȂ�>
					dx = GetParaInt(cmp, 0);							// �ړ��ʎ擾
					dy = GetParaInt(cmp, 1);
					dz = GetParaInt(cmp, 2);
					mflg = TRUE;										// �ړ��׸�ON
				}
				mono = obno, mlvl = clvl;								// �ړ�����ޗ���No/���������No
				NextAllCmdPtr(&cmp);									// ������ނ�
				continue;												// �擪�֖߂�
			} else {													// <!=�ړ�����ނ̏ꍇ>
				if(mflg&&(dx!=0||dy!=0||dz!=0)) {						// <�O����ނ��L���Ȉړ�>
					i = mono - min_obno, j = mlvl - min_clvl;			// �ŏ��l����̔ԍ�
					fprintf(fp, "C08 %d %d 3 0\n", i, j);				// �ړ�����ޏ���
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// �ړ��׸�OFF
				}
			}
			sprintf_s(ccn, 100, "C%02d", cno);
			i = obno - min_obno, j = clvl - min_clvl;					// �ŏ��l����̔ԍ�
			fprintf(fp, "%s %d %d %d %d\n", ccn, i, j, inum, pnum);		// ����ޏ�񏑍�
			for(i=0, cnt=1; i<inum; i++) {
				fprintf(fp, "%d", GetParaInt(cmp, i));					// �������Ұ�����
				if(cnt%10) {											// 10���ȓ��ͽ�߰�
					fprintf(fp, " ");
					cnt++;
				} else {												// 10���ŉ��s
					fprintf(fp, "\n");
					cnt = 1;
				}
			}
			if(cnt!=1) {
				fprintf(fp, "\n");										// ���s
			}
			for(i=0; i<pnum; i++) {
				GetParaPnt(cmp, i, &pp);								// ���W���Ұ��擾
				fprintf(fp, "%f %f %f\n", pp.p[0], pp.p[1], pp.p[2]);	// ���W���Ұ�����
			}
			ccc++;
			dpr = 70.0*((double)ccc/(double)scc)+5.0;					// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF2->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
			NextAllCmdPtr(&cmp);										// ������ނ�
		}
		if(ret) {														// �����I�����͒��~
			break;
		}
		if(mflg&&(dx!=0||dy!=0||dz!=0)) {								// <�O����ނ��L���Ȉړ��̏ꍇ>
			i = mono - min_obno, j = mlvl - min_clvl;					// �ŏ��l����̔ԍ�
			fprintf(fp, "C08 %d %d 3 0\n", i ,j);						// �ړ�����ޏ���
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// �ړ������OFF
		}
		if(oflg) {														// <�ʏ����ޗL�̏ꍇ>
			WriteGroupFileJgd(fp, op, obno-min_obno);					// ��ٰ�ߏ��̧�ٕۑ�(JGD�p)
			WriteColorFileJgd(fp, op, obno-min_obno);					// �װNo̧�ٕۑ�(JGD�p)
			WritePartsFileJgd(fp, op, obno-min_obno);					// �߰�No̧�ٕۑ�(JGD�p)
			WriteAttributeFileJgd(fp, op, obno-min_obno);				// ����No̧�ٕۑ�(JGD�p)
		}
		NextOifPtr(oifmode, 0, &oif);									// ���\�������̂�
	}
	if(!ret) {															// <���폈����>
		cnt = 0, ccc = 0;												// �ް�������
		memset(&buf, ' ', sizeof(OIFDATA));
		memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
		BaseOifPtr(oifmode, 1, &oif);									// �\�������̐擪�擾
		while(oif) {
			lvl = GetOifLevel(oif);										// ���̍\�����َ擾
			num = GetOifNo(oif);										// ���̍\������No�擾
			GetOifName(oif, name);										// ���̍\�����̎擾
			GetOifObj(oif, &op);										// �\����񗧑��߲���擾
			sprintf_s(ccn, 100, "%03d", lvl);							// ���̍\�����ِݒ�
			memcpy(buf.dat[cnt].level, ccn, 3);
			sprintf_s(ccn, 100, "%04d", num);							// ���̍\������No�ݒ�
			memcpy(buf.dat[cnt].ono, ccn, 4);
			memcpy(buf.dat[cnt].name, name, ONM_LENG);					// ���̍\�����̐ݒ�
			cnt++, ccc++;												// ����UP
			dpr = 15.0*((double)ccc/(double)oct)+75.0;					// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF2->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
			if(cnt>=8) {												// <�ް��ݒ芮��>
				fwrite(&buf, sizeof(OIFDATA), 1, fp);					// ̧�ُo��
				cnt = 0;												// �ް�������
				memset(&buf, ' ', sizeof(OIFDATA));
				memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
			}
			NextOifPtr(oifmode, 1, &oif);								// ���\�������̂�
		}
		if(!ret&&cnt>0) {												// ���o�͗L��̧�ُo��
			fwrite(&buf, sizeof(OIFDATA), 1, fp);
		}
	}
	fclose(fp);															// ���̧��CLOSE
	if(!ret) {															// <���폈����>
		m_pProgressF2->SetProgress(95);									// �i���\��(95%)
		if(!ConvCmdToJsd(fname)) {										// CMD->JSD�ϊ�
			ret = 1;
		}
		m_pProgressF2->SetProgress(100);								// �i���\��(100%)
	}
	remove(tname);														// ���̧�ٍ폜
	if(sflg!=2) {														// <<�ʏ�(JGD/GGD)̧�ُo��>>
		if(!ret) {
			OffCmdFlg();												// ����ޖ��ۑ�OFF
		}
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	m_pProgressF2->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressF2;
	delete[] ccn;
	delete[] name;
	return ret;
}

/*****************************************************/
static void ReadStop(char* ccn, char* pb1, char* pb2,
                     char* pb3, char* rbuf)								//<<< ̧�ٓ��͒��~����
/*****************************************************/
{
	if(m_pProgressF2!=NULL) {
		m_pProgressF2->DestroyWindow();									// �޲�۸ޔp��
		delete m_pProgressF2;
	}
	delete[] ccn;
	delete[] pb1;
	delete[] pb2;
	delete[] pb3;
	delete[] rbuf;
}

/*************************************************************/
static BOOL CheckPntTypeData(FILE* fp, int* fg, PNTTYPE* pp,
                             char* pb1, char* pb2, char* pb3)			//<<< PNTTYPE�ް�����
/*************************************************************/
{
	BOOL ret=TRUE;
	int  i, ia[3];

	memset(pb1, 0x00, 100);												// �ޯ̧������
	if(fscanf_s(fp, "%s", pb1, 100)!=1) {								// �擪�ް��Ǎ���
		ret = FALSE;
	}
	if(strstr(pb1, ".")!=NULL) {										// <�擪�ް�������>
		pp->p[0] = atof(pb1);											// ���l�ϊ�
		if(fscanf_s(fp, "%s%s", pb2, 100, pb3, 100)!=2) {				// �c��̎�����Ǎ���
			ret = FALSE;
		}
		pp->p[1] = atof(pb2), pp->p[2] = atof(pb3);
		*fg = 1;
	} else {															// <�擪�ް�������>
		sscanf_s(pb1, "%d", &ia[0]);									// ���l�ϊ�
		if(fscanf_s(fp, "%d%d", &ia[1], &ia[2])!=2) {					// �c��̐�����Ǎ���
			ret = FALSE;
		}
		for(i=0; i<3; i++) {
			pp->p[i] = ia[i]/1000.0;									// ����(��m)������(mm)
		}
		*fg = 0;
	}
	return ret;
}

/***********************************************************/
static BOOL GetPntTypeData(FILE* fp, int fg, PNTTYPE* pp,
                           char* pb1, char* pb2, char* pb3)				//<<< PNTTYPE�ް��擾
/***********************************************************/
{
	BOOL ret=TRUE;
	int  i, ia[3];

	if(fg) {															// <����>
		if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
			ret = FALSE;
		}
		pp->p[0] = atof(pb1);											// ���l�ϊ�
		pp->p[1] = atof(pb2);
		pp->p[2] = atof(pb3);
	} else {															// <����>
		if(fscanf_s(fp, "%d%d%d", &ia[0], &ia[1], &ia[2])!=3) {
			ret = FALSE;
		}
		for(i=0; i<3; i++) {
			pp->p[i] = ia[i]/1000.0;									// ����(��m)������(mm)
		}
	}
	return ret;
}

/******************************************************/
int ReadJgdFile(const char* fname, int sflg, int uflg)					//<<< JGD/GGḐ�ٓ���
/******************************************************/
{
	char*    ccn = new char[100];
	char*    pb1 = new char[100];
	char*    pb2 = new char[100];
	char*    pb3 = new char[100];
	char*    rbuf = new char[JSD_LENG];
	char     wpwd[11], rpwd[11];
	MSG      msg;
	OBJTYPE* op;
	CMDTYPE* cmp;
	CWKTYPE* cwk;
	OIFTYPE* oif, * bfo, * nwo, * sto;
	PNTTYPE  pp;
	OIFDATA  buf;
	FILE*    fp;
	CString  tname, str;
	BOOL     ok=TRUE;
	int      ret=1, flg, savf, len, cno, obno, clvl, inum, pnum;
	int      i, j, mvf, msv[3], obno_sv=-1, max_clvl=0, chk;
	int      osw=0, lvl, num, blv, bpf, st=0, scc=0, ccc=0, spr=0;
	double   dpr;

	m_pProgressF2 = NULL;												// ��۸�ڽ������
	MainWnd->GetPasswordData(wpwd, rpwd, &savf);						// �߽ܰ���ް��擾
	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	while(TRUE) {
		i = ConvJgdToCmd(fname, wpwd, &flg);							// JGD->CMD�ϊ�
		if(i==1) {														// ���o�ʹװ��
			ReadStop(ccn, pb1, pb2, pb3, rbuf);							// ̧�ٓ��͒��~����
			return 1;
		} else if(i==-1) {												// �߽ܰ�޴װ��
			CPasswdDlg2 dlg;
			if(dlg.DoModal()==IDOK) {									// �߽ܰ�ސݒ��޲�۸ޕ\��
				dlg.GetValue(&str);
				len = str.GetLength();
				memset(wpwd, NULL, sizeof(wpwd));
				memcpy(wpwd, (const char*)str, len);
			} else {													// �޲�۸޷�ݾق�
				ReadStop(ccn, pb1, pb2, pb3, rbuf);						// ̧�ٓ��͒��~����
				remove(tname);											// ���̧�ٍ폜
				return 2;
			}
		} else {
			break;														// �ϊ�����I���͎�������
		}
	}
	m_pProgressF2 = new CProgressDlg(4);
	m_pProgressF2->Create();											// �޲�۸ޕ\��
	m_pProgressF2->SetProgress(5);										// �i���\��(5%)
	if(fopen_s(&fp, tname, "r")!=0) {									// ���̧��OPEN
		ReadStop(ccn, pb1, pb2, pb3, rbuf);								// ̧�ٓ��͒��~����
		return 1;
	}
	if(fscanf_s(fp, "%s", ccn, 100)!=1) {								// �����߽ܰ�޺���ޏ��擾
		ok = FALSE;
	}
	if(ccn[0]!='C') {													// �����߽ܰ���ް��ُ�ŏI��
		ok = FALSE;
	}
	sscanf_s(&ccn[1], "%d", &cno);										// ���l�ϊ�
	if(cno!=WPWDCMD) {													// �����߽ܰ���ް��ُ�ŏI��
		ok = FALSE;
	}
	if(fscanf_s(fp, "%d%d%d%d%d%d%d%d%d%d",
                    &i, &i, &i, &i, &i, &i, &i, &i, &i, &i)!=10) {		// �����߽ܰ�޺���ޓǂݔ�΂�
		ok = FALSE;
	}
	if(flg) {															// <�Ǎ��߽ܰ�ސݒ�L>
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// �Ǎ��߽ܰ�޺���ޏ��擾
			ok = FALSE;
		}
		if(ccn[0]!='C') {												// �Ǎ��߽ܰ���ް��ُ�ŏI��
			ok = FALSE;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// ���l�ϊ�
		if(cno!=RPWDCMD) {												// �Ǎ��߽ܰ���ް��ُ�ŏI��
			ok = FALSE;
		}
		if(fscanf_s(fp, "%d%d%d%d%d%d%d%d%d%d",
                        &i, &i, &i, &i, &i, &i, &i, &i, &i, &i)!=10) {	// �Ǎ��߽ܰ�޺���ޓǂݔ�΂�
			ok = FALSE;
		}
	}
	if(sflg) {															// <�ް��W(*.ggd)>
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ����ޏ��擾
			ok = FALSE;
		}
		if(ccn[0]!='C') {												// �ް��ُ�ŏI��
			ok = FALSE;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// ���l�ϊ�
		if(cno!=GLBDCMD) {												// �ް��ُ�ŏI��
			ok = FALSE;
		}
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	OnCmdFlgStp();														// ���ۑ��׸ލX�V���~ON
	InitOifNo();														// ���̍\������No������
	while(ok) {
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ����ޏ��擾
			break;
		} else {														// 1�ް��Ǎ��݂Ő���I����
			ret = 0;
		}
		if(!memcmp(ccn, OIF_TITLE, 7)) {								// ���̍\�����֐ؑւ�
			osw = 1; break;
		}
		if(fscanf_s(fp, "%d%d%d%d", &obno, &clvl, &inum, &pnum)!=4) {	// ���Ұ����擾
			break;
		}
		if(obno!=obno_sv) {												// <����No���قȂ�ꍇ>
			CreateObjWork();											// ���̍���ް��̈�m��
			obno_sv = obno;
		}
		max_clvl = (clvl>max_clvl) ? clvl : max_clvl;					// �ő��������ٌ���
		if(ccn[0]!='C') {												// �ް��ُ�ŏI��
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// �����No���l�ϊ�
		scc++;															// ����ސ�����
		mvf = (cno==MOVE) ? 1 : 0;										// �ړ������
		if(mvf==0) {													// <�ړ�����ވȊO>
			CreatCmd(cno, inum, pnum, &cmp);							// ����ޗ̈�m��
			CreateCmdWork(clvl, cmp);									// ����ލ���ް��̈�m��
		}
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// �������Ұ��擾
				ret = 1; break;
			}
			if(mvf==1) {												// <�ړ������>
				msv[i] = j;												// �������Ұ��ۑ�
			} else  {													// <�ȊO>
				PutParaInt(cmp, i, j);									// �������Ұ��ݒ�
			}
		}
		if(pnum>0) {
			if(!CheckPntTypeData(fp, &j, &pp, pb1, pb2, pb3)) {			// PNTTYPE�ް�����
				ret = 1; break;
			}
			PutParaPnt(cmp, 0, pp);										// ���W���Ұ��ݒ�
			for(i=1; i<pnum; i++) {										// 2�_�ڈȍ~
				if(!GetPntTypeData(fp, j, &pp, pb1, pb2, pb3)) {		// PNTTYPE�ް��擾
					ret = 1; break;
				}
				PutParaPnt(cmp, i, pp);									// ���W���Ұ��ݒ�
			}
		}
		if(mvf==1) {													// <�ړ������>
			if(msv[0]!=0||msv[1]!=0||msv[2]!=0) {						// <�ړ��ʂ���>
				CreatCmd(MOVE, 3, 0, &cmp);								// ����ޗ̈�m��
				CreateCmdWork(clvl, cmp);								// ����ލ���ް��̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, msv[i]);							// �������Ұ��ݒ�
				}
			}
		}
	}
	m_pProgressF2->SetProgress(10);										// �i���\��(10%)
	if(!ret) {															// <<����̏ꍇ>>
		for(i=0; i<=max_clvl; i++) {									// �J�n��������ف`�ŏI���������
			chk = 0;
			while(TRUE) {
				SearchCmdWork(i, &cwk, &cmp, &op);						// ����ލ���ް�����
				if(cmp!=NULL) {											// <����ޗL�̏ꍇ>
					if(!chk) {											// �e��������ُ���̂�
						if(uflg) {										// <��Undo/Redo����>
							InitLevel();								// Undo/Redo������
						}
						chk = 1;
					}
					if(uflg) {											// <��Undo/Redo����>
						InitCmdLvl(cmp);								// ���������No������
					}
					SetCmdPtr(cmp);										// �����Ώۺ���ސݒ�
					if(op!=NULL) {
						SetObjPtr(op);									// �����Ώۗ��̐ݒ�
					}
					ExecCommand(TRUE);									// ����ގ��s
					if(op==NULL) {
						GetObjPtr(&op);									// �����Ώۗ��̎擾
						SetOwkObjPtr(cwk, op);							// ���̍���ް������߲���ݒ�
					}
					ChangeCwkFlg(cwk);									// ����ލ���ް��׸ލX�V
					ccc++;
					dpr = 85.0*((double)ccc/(double)scc)+10.0;			// �����߰���
					if(spr<(int)dpr) {									// <�߰���UP>
						spr = (int)dpr;
						m_pProgressF2->SetProgress(spr);				// �i���\��
					}
					if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                                   0, 0, PM_NOREMOVE)) {
						GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
						if(msg.message==WM_LBUTTONUP) {					// �����I��
							ret = -1; break;
						}
					}
				} else {												// <����ޖ��̏ꍇ>
					break;												// ����������ق�
				}
			}
			if(ret==-1) {
				break;
			}
		}
		if(!ret) {														// <<���폈����>>
			while(TRUE) {												// <��ٰ�ߤ�װ��߰¤���������>
				SearchCmdWork(-1, &cwk, &cmp, &op);						// ����ލ���ް�����
				if(cmp!=NULL) {											// <����ޗL�̏ꍇ>
					if(uflg) {											// <��Undo/Redo����>
						InitCmdLvl(cmp);								// ���������No������
					}
					SetCmdPtr(cmp);										// �����Ώۺ���ސݒ�
					SetObjPtr(op);										// �����Ώۗ��̐ݒ�
					ExecCommand(TRUE);									// ����ގ��s
				} else {												// <����ޖ��̏ꍇ>
					break;												// ����������ق�
				}
			}
			LastObjWorkProc();											// ���̍���ް��ŏI����
			if(uflg) {													// <��Undo/Redo����>
				FileBackUp2();											// ̧���ޯ����ߏ���2
			}
		}
		FreeObjCmdWork(0);												// ����/����ލ���ް����
	} else {															// <<�ُ�̏ꍇ>>
		FreeObjCmdWork(1);												// ����/����ލ���ް����
	}
	if(!ret) {															// <���폈����>
		SortOifData();													// ���̍\���ް����
		bfo = NULL, sto = NULL;											// �̍\���߲��������
		BaseOifPtr(0, 0, &oif);											// �擪���̍\���߲���擾
		while(oif!=NULL) {
			if(GetOifNo(oif)>=0) {										// ����Ǎ����̂ŏI��
				break;
			}
			bfo = oif;													// �O���̍\���߲���ۑ�
			NextOifPtr(0, 0, &oif);										// �����̍\���߲���擾
		}
		if(oif==NULL) {													// �L�����̍\�����͏������~
			osw = 0;
		}
		if(osw) {														// <<���̍\�����>>
			memcpy(rbuf, ccn, 7);										// ���ٕ���
			if(fread(&rbuf[7], JSD_LENG-7, 1, fp)!=1) {					// �ް�����(249byte)
				osw = 0;
			}
			if(osw) {
				memcpy(&buf, rbuf, JSD_LENG);							// �����ް����\���ֺ̂�߰
			}
		}
		if(osw) {														// <<���̍\�����X�V�L>>
			blv = -1, bpf = -1;											// �O����,�O�o���׸ޏ�����
			while(osw) {
				for(i=0; i<8; i++) {
					if(!memcmp(buf.dat[i].level, "   ", 3)) {			// �ް����͏I��
						osw = 0; break;
					}
					memset(ccn, ' ', 100);
					memcpy(ccn, buf.dat[i].level, 3);
					sscanf_s(ccn, "%d", &lvl);							// ���̍\������No�擾
					memset(ccn, ' ', 100);
					memcpy(ccn, buf.dat[i].ono, 4);
					sscanf_s(ccn, "%d", &num);							// ���̍\������No�擾
					if(oif!=NULL&&GetOifNo(oif)==num) {					// <<����No�������ꍇ>>
						SetOifLevel(oif, lvl);							// ���̍\�����ِݒ�
						SetOifName(0, oif, buf.dat[i].name);			// ���̍\�����̐ݒ�
						bfo = oif;										// �O���̍\���߲���ۑ�
						if(!st) {										// �擪�ް��ۑ�
							sto = oif, st = 1;
						}
						NextOifPtr(0, 0, &oif);							// �����̍\���߲���擾
						bpf = 0;										// �O�o�͂͗���
					} else {											// <<����No���قȂ�ꍇ>>
						if(bpf==1&&blv==lvl) {							// <���O���ƯĂ����ق���v>
							SetOifName(1, bfo, buf.dat[i].name);		// ���̍\�����̐ݒ�(�㏑��)
						} else {										// <���O���ƯĈȊO�����ق��s��v>
							if(bfo==NULL) {								// �O���̍\���Ȃ���
								CreatOif(1, NULL, OBJ_NAME00,
                                                  NULL, &nwo);			// �\�����̈�m��(�擪�ڑ�)
							} else {									// �O���̍\�������
								CreatOif(2, NULL, OBJ_NAME00,
                                                  bfo, &nwo);			// �\�����̈�m��(���Ԑڑ�)
							}
							SetOifLevel(nwo, lvl);						// ���̍\�����ِݒ�
							SetOifName(0, nwo, buf.dat[i].name);		// ���̍\�����̐ݒ�
							if(!st) {									// �擪�ް��ۑ�
								sto = nwo, st = 1;
							}
							bfo = nwo;									// �O���̍\���߲���u��
						}
						bpf = 1;										// �O�o�͂��Ư�
					}
					blv = lvl;											// �O���ٕۑ�
				}
				if(fread(&buf, sizeof(OIFDATA), 1, fp)!=1) {			// �ް�����(256byte)
					break;
				}
			}
		} else {														// <<���̍\�����X�V��>>
			while(oif!=NULL) {
				View->SetOifItem(oif);									// ���̍\������ѐݒ�
				if(!st) {												// �擪�ް��ۑ�
					sto = oif, st = 1;
				}
				NextOifPtr(0, 0, &oif);									// �����̍\���߲���擾
			}
		}
		m_pProgressF2->SetProgress(100);								// �i���\��(100%)
		if(sto!=NULL) {
			View->SelectOifItem(sto);									// �擪�ް����ёI��
		}
	}
	OffCmdFlgStp();														// ���ۑ��׸ލX�V���~OFF
	fclose(fp);															// ���̧��CLOSE
	remove(tname);														// ���̧�ٍ폜
	View->SetDataUse();													// �ް��g�p���ݒ�
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	ReadStop(ccn, pb1, pb2, pb3, rbuf);									// ̧�ٓ��͒��~����
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	return ret;
}
