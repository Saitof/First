/////////////////////////////////////////////////////////////////////////////
// ̧�ٓ��o�͕�1
// DataFile1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "Attribute.h"
#include "DataConv.h"

CProgressDlg* m_pProgressF1=NULL;										// ��۸�ڽ�ݼ޹���

/*******************************************************/
int WriteJsdFile(const char* fname, int dflg, int sflg)					//<<< JSF/JSD/GLF/GLḐ�ُo��
/*******************************************************/
{
	char*    ccn = new char[100];
	char*    name = new char[ONM_LENG+2];
	MSG      msg;
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pp;
	OIFDATA  buf;
	FILE*    fp;
	CString  tname;
	int      ret=0, cno, inum, pnum, i, cnt, mflg, oflg, vchk=0;
	int      dx, dy, dz, oct, lvl, num, scc=0, ccc=0, spr=0;
	double   dpr;

	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&fp, tname, "w")!=0) {									// ���̧��OPEN
		delete[] ccn;
		delete[] name;
		return 1;
	}
	m_pProgressF1 = new CProgressDlg(3);
	m_pProgressF1->Create();											// �޲�۸ޕ\��
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	BaseOifPtr(1, 0, &oif);												// �\�������̐擪�擾
	while(oif) {
		GetOifObj(oif, &op);											// �\����񗧑��߲���擾
		BaseCmdPtr(1, op, &cmp);										// �L������ސ擪�擾
		while(cmp) {
			scc++;														// ����ސ����Z
			if(GetCmdNo(cmp)>=EARTH) {									// �����No�擾
				vchk = 1;												// �V����ޗL(�������ވȍ~)
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		NextOifPtr(1, 0, &oif);											// ���\�������̂�
	}
	if(vchk) {															// <�V����ޗL�̏ꍇ>
		fclose(fp);														// ���̧��CLOSE
		remove(tname);													// ���̧�ٍ폜
		setlocale(LC_ALL, "");											// ۹�ق���̫�ĂɕύX
		m_pProgressF1->DestroyWindow();									// �޲�۸ޔp��
		delete m_pProgressF1;
		delete[] ccn;
		delete[] name;
		return 2;
	}
	m_pProgressF1->SetProgress(5);										// �i���\��(5%)
	InitOifNo();														// ���̍\������No������
	if(sflg) {															// <�ް��W(*.gld)>
		sprintf_s(ccn, 100, "C%02d", GLBDCMD);							// �ް��W�擪�����
		fprintf(fp, "%s\n", ccn);										// ����ޏ�񏑍�
	}
	mflg = FALSE;														// �ړ��׸ޏ�����(OFF)
	oct = 0;
	BaseOifPtr(1, 0, &oif);												// �\�������̐擪�擾
	while(oif) {
		oflg = FALSE;													// �o���׸ޏ�����(OFF)
		SetOifNo(oif, oct++);											// ���̍\������No�ݒ�
		GetOifObj(oif, &op);											// �\����񗧑��߲���擾
		BaseCmdPtr(1, op, &cmp);										// �L������ސ擪�擾
		while(cmp) {
			oflg = TRUE;												// �o���׸�ON
			cno = GetCmdNo(cmp);										// �����No�擾
			if(cno==CHGSURF) {											// �����No�ύX
				cno = CRTSURF;
			} else if(cno==CHGCURVE) {
				cno = CRTCURVE;
			} else if(cno==CHGSWEEP) {
				cno = CRTSWEEP;
			}
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
				NextCmdPtr(&cmp);										// ������ނ�
				continue;												// �擪�֖߂�
			} else {													// <!=�ړ�����ނ̏ꍇ>
				if(mflg&&(dx!=0||dy!=0||dz!=0)) {						// <�O����ނ��L���Ȉړ�>
					fprintf(fp, "C08 3 0\n");							// �ړ�����ޏ���
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// �ړ��׸�OFF
				}
			}
			if(cno==RING) {												// <<�V�ݸ޻��ލ��킹����ނ̏ꍇ>>
				sprintf_s(ccn, 100, "C%02d", SCALE);					// [���ޕύX�����]
				fprintf(fp, "%s %d %d\n", ccn, 3, 1);					// ����ޏ�񏑍�
				for(i=0; i<3; i++) {
					fprintf(fp, "%d ", GetParaInt(cmp, i));				// �������Ұ�����
				}
				fprintf(fp, "\n");										// ���s
				if(dflg) {												// <JSD/GLD�̏ꍇ>
					GetParaPnt(cmp, 0, &pp);							// ���W���Ұ��擾
					fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000.0),
                                              (int)(pp.p[1]*1000.0),
                                              (int)(pp.p[2]*1000.0));	// ���W���Ұ�����
				} else {												// <JSF/GLF�̏ꍇ>
					GetParaPnt(cmp, 0, &pp);							// ���W���Ұ��擾
					fprintf(fp, "%f %f %f\n",
                                pp.p[0], pp.p[1], pp.p[2]);				// ���W���Ұ�����
				}
				sprintf_s(ccn, 100, "C%02d", BEND);						// [�~���Ȃ������]
				fprintf(fp, "%s %d %d\n", ccn, 3, 2);					// ����ޏ�񏑍�
				for(i=3; i<6; i++) {
					fprintf(fp, "%d ", GetParaInt(cmp, i));				// �������Ұ�����
				}
				fprintf(fp, "\n");										// ���s
				if(dflg) {												// <JSD/GLD�̏ꍇ>
					for(i=1; i<3; i++) {
						GetParaPnt(cmp, i, &pp);						// ���W���Ұ��擾
						fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000),
                                                  (int)(pp.p[1]*1000),
                                                  (int)(pp.p[2]*1000));	// ���W���Ұ�����
					}
				} else {												// <JSF/GLF�̏ꍇ>
					for(i=1; i<3; i++) {
						GetParaPnt(cmp, i, &pp);						// ���W���Ұ��擾
						fprintf(fp, "%f %f %f\n",
                                    pp.p[0], pp.p[1], pp.p[2]);			// ���W���Ұ�����
					}
				}
			} else {													// <<������ނ̏ꍇ>>
				sprintf_s(ccn, 100, "C%02d", cno);
				fprintf(fp, "%s %d %d\n", ccn, inum, pnum);				// ����ޏ�񏑍�
				for(i=0, cnt=1; i<inum; i++) {
					fprintf(fp, "%d", GetParaInt(cmp, i));				// �������Ұ�����
					if(cnt%10) {										// 10���ȓ��ͽ�߰�
						fprintf(fp, " ");
						cnt++;
					} else {											// 10���ŉ��s
						fprintf(fp, "\n");
						cnt = 1;
					}
				}
				if(cnt!=1) {
					fprintf(fp, "\n");									// ���s
				}
				if(dflg) {												// <JSD/GLD�̏ꍇ>
					for(i=0; i<pnum; i++) {
						GetParaPnt(cmp, i, &pp);						// ���W���Ұ��擾
						fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000),
                                                  (int)(pp.p[1]*1000),
                                                  (int)(pp.p[2]*1000));	// ���W���Ұ�����
					}
				} else {												// <JSF/GLF�̏ꍇ>
					for(i=0; i<pnum; i++) {
						GetParaPnt(cmp, i, &pp);						// ���W���Ұ��擾
						fprintf(fp, "%f %f %f\n",
                                    pp.p[0], pp.p[1], pp.p[2]);			// ���W���Ұ�����
					}
				}
			}
			ccc++;
			dpr = 70.0*((double)ccc/(double)scc)+5.0;					// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF1->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		if(ret) {														// �����I�����͒��~
			break;
		}
		if(mflg&&(dx!=0||dy!=0||dz!=0)) {								// <�O����ނ��L���Ȉړ��̏ꍇ>
			fprintf(fp, "C08 3 0\n");									// �ړ�����ޏ���
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// �ړ������OFF
		}
		if(oflg) {														// <�ʏ����ޗL�̏ꍇ>
			WriteGroupFile(fp, op);										// ��ٰ�ߏ��̧�ٕۑ�
			WriteColorFile(fp, op);										// �װNo̧�ٕۑ�
			WritePartsFile(fp, op);										// �߰�No̧�ٕۑ�
			WriteAttributeFile(fp, op);									// ����No̧�ٕۑ�
		}
		NextOifPtr(1, 0, &oif);											// ���\�������̂�
	}
	if(!ret) {															// <���폈����>
		cnt = 0, ccc = 0;												// �ް�������
		memset(&buf, ' ', sizeof(OIFDATA));
		memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
		BaseOifPtr(1, 1, &oif);											// �\�������̐擪�擾
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
				m_pProgressF1->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
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
			NextOifPtr(1, 1, &oif);										// ���\�������̂�
		}
		if(!ret&&cnt>0) {												// ���o�͗L��̧�ُo��
			fwrite(&buf, sizeof(OIFDATA), 1, fp);
		}
	}
	fclose(fp);															// ���̧��CLOSE
	if(!ret) {															// <���폈����>
		m_pProgressF1->SetProgress(95);									// �i���\��(95%)
		if(!ConvCmdToJsd(fname)) {										// CMD->JSD�ϊ�
			ret = 1;
		}
		m_pProgressF1->SetProgress(100);								// �i���\��(100%)
	}
	remove(tname);														// ���̧�ٍ폜
	if(!ret) {
		OffCmdFlg();													// ����ޖ��ۑ�OFF
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	m_pProgressF1->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressF1;
	delete[] ccn;
	delete[] name;
	return ret;
}

/******************************************************/
static void ReadStop(int pflg, char* ccn, char* pb1,
                     char* pb2, char* pb3, char* rbuf)					//<<< ̧�ٓ��͒��~����
/******************************************************/
{
	if(pflg) {															// <��۸�ڽ�\��:�L>
		m_pProgressF1->DestroyWindow();									// �޲�۸ޔp��
		delete m_pProgressF1;
	}
	delete[] ccn;
	delete[] pb1;
	delete[] pb2;
	delete[] pb3;
	delete[] rbuf;
}

/********************************************************/
static int CountCommand(FILE* fp, int sflg, char* ccn,
                        char* pb1, char* pb2, char* pb3)				//<<< ����ސ�����
/********************************************************/
{
	int i, j, inum, pnum, cnt=0;

	if(sflg) {
		fscanf_s(fp, "%s", ccn, 100);									// �ް��W(����ޏ��擾)
	}
	while(TRUE) {
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ����ޏ��擾
			break;
		}
		if(!memcmp(ccn, OIF_TITLE, 7)) {								// ���̍\������
			break;
		}
		if(fscanf_s(fp, "%d%d", &inum, &pnum)!=2) {						// ���Ұ����擾
			break;
		}
		if(ccn[0]!='C') {												// �ް��ُ�ŏI��
			break;
		}
		cnt++;															// ����ސ�����
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// �������Ұ��擾
				break;
			}
		}
		for(i=0; i<pnum; i++) {											// ���W���Ұ��擾
			if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
				break;
			}
		}
	}
	return cnt;
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
int ReadJsdFile(const char* fname, int sflg, int pflg)					//<<< JSF/JSD/GLF/GLḐ�ٓ���
/******************************************************/
{
	char*    ccn = new char[100];
	char*    pb1 = new char[100];
	char*    pb2 = new char[100];
	char*    pb3 = new char[100];
	char*    rbuf = new char[JSD_LENG];
	MSG      msg;
	OBJTYPE* op;
	CMDTYPE* cmp;
	OIFTYPE* oif, * bfo, * nwo, * sto;
	PNTTYPE  pp, psv[3];
	OIFDATA  buf;
	FILE*    fp;
	CString  tname;
	BOOL     ok=TRUE;
	int      ret=1, i, j, cno, inum, pnum, svf=0, mvf=0, isv[6], msv[3];
	int      oct=0, osw=0, lvl, num, blv, bpf, st=0, scc=0, ccc=0, spr=0;
	double   dpr;

	if(pflg) {															// <��۸�ڽ�\��:�L>
		m_pProgressF1 = new CProgressDlg(4);
		m_pProgressF1->Create();										// �޲�۸ޕ\��
	}
	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(!ConvJsdToCmd(fname)) {											// JSD->CMD�ϊ�
		ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);						// ̧�ٓ��͒��~����
		return 1;
	}
	if(pflg) {															// <��۸�ڽ�\��:�L>
		m_pProgressF1->SetProgress(5);									// �i���\��(5%)
		if(fopen_s(&fp, tname, "r")!=0) {		    					// ���̧��OPEN
			ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);					// ̧�ٓ��͒��~����
			return 1;
		}
		scc = CountCommand(fp, sflg, ccn, pb1, pb2, pb3);				// ����ސ�����
		fclose(fp);														// ���̧��CLOSE
		m_pProgressF1->SetProgress(10);									// �i���\��(10%)
	}
	if(fopen_s(&fp, tname, "r")!=0) {									// ���̧��OPEN
		ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);						// ̧�ٓ��͒��~����
		return 1;
	}
	if(sflg) {															// <�ް��W(*.gld)>
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
		if(fscanf_s(fp, "%d%d", &inum, &pnum)!=2) {						// ���Ұ����擾
			break;
		}
		if(ccn[0]!='C') {												// �ް��ُ�ŏI��
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// �����No���l�ϊ�
		if(cno==BEND) {													// <�~���Ȃ������>
			svf = (svf==1) ? 2 : svf;									// �ۑ��׸�:2(�~���Ȃ�)
		} else if(cno==MOVE) {											// <�ړ������>
			mvf = 1;
		} else {														// <�~���Ȃ���ړ�����ވȊO>
			if(svf==1) {												// <�ݸ޻��ލ����p�łȂ����ޕύX>
				CreatCmd(SCALE, 3, 1, &cmp);							// ����ޗ̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// �������Ұ��ݒ�
				}
				PutParaPnt(cmp, 0, psv[0]);								// ���W���Ұ��ݒ�
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
				svf = 0;												// ����ޕۑ��Ȃ��֕ύX
			}
			svf = (cno==SCALE) ? 1 : svf;								// ���ޕύX����ނ�,�ۑ��׸�:1
		}
		if(svf==0&&mvf==0) {											// <����ޕۑ���&�ړ��ȊO>
			CreatCmd(cno, inum, pnum, &cmp);							// ����ޗ̈�m��
		}
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// �������Ұ��擾
				ret = 1; break;
			}
			if(mvf==1) {												// <�ړ������>
				msv[i] = j;												// �������Ұ��ۑ�
			} else if(svf==0) {											// <����ޕۑ��Ȃ�>
				PutParaInt(cmp, i, j);									// �������Ұ��ݒ�
			} else if(svf==1) {											// <����ޕۑ�(���ޕύX)>
				isv[i] = j;												// �������Ұ��ۑ�
			} else {													// <����ޕۑ�(�~���Ȃ�)>
				isv[i+3] = j;											// �������Ұ��ۑ�
			}
		}
		if(pnum>0) {
			if(!CheckPntTypeData(fp, &j, &pp, pb1, pb2, pb3)) {			// PNTTYPE�ް�����
				ret = 1; break;
			}
			if(svf==0) {												// <����ޕۑ��Ȃ�>
				PutParaPnt(cmp, 0, pp);									// ���W���Ұ��ݒ�
			} else if(svf==1) {											// <����ޕۑ�(���ޕύX)>
				psv[0] = pp;											// ���W���Ұ��ۑ�
			} else {													// <����ޕۑ�(�~���Ȃ�)>
				psv[1] = pp;											// ���W���Ұ��ۑ�
			}
			for(i=1; i<pnum; i++) {										// 2�_�ڈȍ~
				if(!GetPntTypeData(fp, j, &pp, pb1, pb2, pb3)) {		// PNTTYPE�ް��擾
					ret = 1; break;
				}
				if(svf==0) {											// <����ޕۑ��Ȃ�>
					PutParaPnt(cmp, i, pp);								// ���W���Ұ��ݒ�
				} else {												// <����ޕۑ�(�~���Ȃ�)>
					psv[i+1] = pp;										// ���W���Ұ��ۑ�
				}
			}
		}
		if(mvf==1) {													// <�ړ������>
			if(msv[0]!=0||msv[1]!=0||msv[2]!=0) {						// <�ړ��ʂ���>
				if(svf==1) {											// <�ݸ޻��ލ����p�łȂ����ޕύX>
					CreatCmd(SCALE, 3, 1, &cmp);						// ����ޗ̈�m��
					for(i=0; i<3; i++) {
						PutParaInt(cmp, i, isv[i]);						// �������Ұ��ݒ�
					}
					PutParaPnt(cmp, 0, psv[0]);							// ���W���Ұ��ݒ�
					SetCmdPtr(cmp);										// �����Ώۺ���ސݒ�
					ExecCommand(TRUE);									// ����ގ��s
				}
				svf = 0;												// ����ޕۑ��Ȃ��֕ύX
				CreatCmd(MOVE, 3, 0, &cmp);								// ����ޗ̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, msv[i]);							// �������Ұ��ݒ�
				}
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
			}
			mvf = 0;
		} else if(svf==0) {												// <����ޕۑ��Ȃ�>
			SetCmdPtr(cmp);												// �����Ώۺ���ސݒ�
			ExecCommand(TRUE);											// ����ގ��s
		} else if(svf==1) {												// <����ޕۑ�(���ޕύX)>
			if(isv[0]!=1000||isv[2]!=1000) {							// <�ݸ޻��ލ����p�łȂ��ꍇ>
				CreatCmd(SCALE, 3, 1, &cmp);							// ����ޗ̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// �������Ұ��ݒ�
				}
				PutParaPnt(cmp, 0, psv[0]);								// ���W���Ұ��ݒ�
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
				svf = 0;												// ����ޕۑ��Ȃ��֕ύX
			}
		} else {														// <����ޕۑ�(�~���Ȃ�)>
			if(isv[3]!=1||isv[4]!=2) {									// <�ݸ޻��ލ����p�łȂ��ꍇ>
				CreatCmd(SCALE, 3, 1, &cmp);							// ����ޗ̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// �������Ұ��ݒ�
				}
				PutParaPnt(cmp, 0, psv[0]);								// ���W���Ұ��ݒ�
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
				CreatCmd(BEND, 3, 2, &cmp);								// ����ޗ̈�m��
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i+3]);						// �������Ұ��ݒ�
				}
				PutParaPnt(cmp, 0, psv[1]);								// ���W���Ұ��ݒ�
				PutParaPnt(cmp, 1, psv[2]);
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
			} else {													// <�ݸ޻��ލ����p�̏ꍇ>
				CreatCmd(RING, 6, 3, &cmp);								// ����ޗ̈�m��
				for(i=0; i<6; i++) {
					PutParaInt(cmp, i, isv[i]);							// �������Ұ��ݒ�
				}
				for(i=0; i<3; i++) {
					PutParaPnt(cmp, i, psv[i]);							// ���W���Ұ��ݒ�
				}
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
			}
			svf = 0;													// ����ޕۑ��Ȃ��֕ύX
		}
		if(cno==ENDCMD) {												// <�ŏI����ނ̏ꍇ>
			GetObjPtr(&op);												// ���̂��߲���擾
			RedoCommand(op);											// ����ލĎ��s
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			SetOifNo(oif, oct++);										// ���̍\������No�ݒ�
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		if(pflg) {														// <��۸�ڽ�\��:�L>
			ccc++;
			dpr = 85.0*((double)ccc/(double)scc)+10.0;					// �����߰���
			if(spr<(int)dpr) {											// <�߰���UP>
				spr = (int)dpr;
				m_pProgressF1->SetProgress(spr);						// �i���\��
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// �����I��
					ret = -1; break;
				}
			}
		}
	}
	if(!ret) {															// <���폈����>
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
		if(pflg) {
			m_pProgressF1->SetProgress(100);							// �i���\��(100%)
		}
		if(sto!=NULL) {
			View->SelectOifItem(sto);									// �擪�ް����ёI��
		}
	}
	OffCmdFlgStp();														// ���ۑ��׸ލX�V���~OFF
	fclose(fp);															// ���̧��CLOSE
	remove(tname);														// ���̧�ٍ폜
	View->SetDataUse();													// �ް��g�p���ݒ�
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);							// ̧�ٓ��͒��~����
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	return ret;
}

/***************************/
int WriteCopyFile(UINT chr)												//<<< ��߰�ް�̧�ُo��
/***************************/
{
	char*    ccn = new char[50];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pp;
	FILE*    fp;
	CString  fpath, tpath;
	int      cno, dsp, edt, inum, pnum, i, cnt, mflg, oflg, dx, dy, dz;

	if(chr==0) {														// <Key�����Ȃ�>
		fpath = GetAppPath(0)+COP_NAME1;								// ��̧߰��PATH�擾
	} else {															// <Key��������>
		fpath.Format(COP_NAME2, chr);									// Key�t��̧߰��
		fpath = GetAppPath(0)+fpath;									// ��̧߰��PATH�擾
	}
	tpath = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&fp, tpath, "w")!=0) {									// ���̧��OPEN
		delete[] ccn;
		return 1;
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	BaseObjPtr(2, &op, &dsp, &edt);										// �L�����̐擪�擾
	mflg = FALSE;														// �ړ��׸ޏ�����(OFF)
	while(op) {
		oflg = FALSE;													// �o���׸ޏ�����(OFF)
		BaseCmdPtr(1, op, &cmp);										// �L������ސ擪�擾
		while(cmp) {
			oflg = TRUE;												// �o���׸�ON
			cno = GetCmdNo(cmp);										// �����No�擾
			if(cno==CHGSURF) {											// �����No�ύX
				cno = CRTSURF;
			}
			if(cno==CHGCURVE) {
				cno = CRTCURVE;
			}
			if(cno==CHGSWEEP) {
				cno = CRTSWEEP;
			}
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
				NextCmdPtr(&cmp);										// ������ނ�
				continue;												// �擪�֖߂�
			} else {													// <!=�ړ�����ނ̏ꍇ>
				if(mflg) {												// <�O����ނ��ړ�>
					fprintf(fp, "C08 3 0\n");							// �ړ�����ޏ���
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// �ړ��׸�OFF
				}
			}
			sprintf_s(ccn, 50, "C%02d", cno);
			fprintf(fp, "%s %d %d\n", ccn, inum, pnum);					// ����ޏ�񏑍�
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
			NextCmdPtr(&cmp);											// ������ނ�
		}
		if(mflg) {														// <�O����ނ��ړ��̏ꍇ>
			fprintf(fp, "C08 3 0\n");									// �ړ�����ޏ���
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// �ړ������OFF
		}
		if(oflg) {														// <�ʏ����ޗL�̏ꍇ>
			WriteGroupFile(fp, op);										// ��ٰ�ߏ��̧�ٕۑ�
			WriteColorFile(fp, op);										// �װNo̧�ٕۑ�
			WritePartsFile(fp, op);										// �߰�No̧�ٕۑ�
			WriteAttributeFile(fp, op);									// ����No̧�ٕۑ�
		}
		NextObjPtr(2, &op, &dsp, &edt);									// ���L�����̂�
	}
	fclose(fp);															// ���̧��CLOSE
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	if(!ConvCmdToJsd(fpath)) {											// CMD->JSD�ϊ�
		delete[] ccn;
		return 1;
	}
	remove(tpath);														// ���̧�ٍ폜
	delete[] ccn;
	return 0;
}

/**************************/
int ReadCopyFile(UINT chr)												//<<< ��߰�ް�̧�ٓ���
/**************************/
{
	char*    ccn = new char[100];
	char*    pb1 = new char[100];
	char*    pb2 = new char[100];
	char*    pb3 = new char[100];
	CPCTYPE* cmp;
	PNTTYPE  pp;
	FILE*    fp;
	CString  fpath, tpath;
	int      ret=1, cno, inum, pnum, pi, i;

	if(chr==0) {														// <Key�����Ȃ�>
		fpath = GetAppPath(0)+COP_NAME1;								// ��̧߰��PATH�擾
	} else {															// <Key��������>
		fpath.Format(COP_NAME2, chr);									// Key�t��̧߰��
		fpath = GetAppPath(0)+fpath;									// ��̧߰��PATH�擾
	}
	tpath = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(!ConvJsdToCmd(fpath)) {											// JSD->CMD�ϊ�
		delete[] ccn;
		delete[] pb1;
		delete[] pb2;
		delete[] pb3;
		return 1;
	}
	if(fopen_s(&fp, tpath, "r")!=0) {									// ���̧��OPEN
		delete[] ccn;
		delete[] pb1;
		delete[] pb2;
		delete[] pb3;
		return 1;
	}
	FreeCopyCmd();														// ��߰����ޗ̈���
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	while(1) {
		if(fscanf_s(fp, "%s%d%d", ccn, 100, &inum, &pnum)!=3) {			// ����ޏ��擾
			break;
		} else {
			ret = 0;
		}
		if(ccn[0]!='C') {												// �ް��ُ�ŏI��
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// ���l�ϊ�
		CreatCopyCmd(cno, inum, pnum, &cmp);							// ��߰����ޗ̈�m��
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &pi)!=1) {							// �������Ұ��擾
				ret = 1; break;
			}
			PutCopyParaInt(cmp, i, pi);									// �������Ұ��ݒ�
		}
		for(i=0; i<pnum; i++) {											// ���W���Ұ��擾
			if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
				ret = 1; break;
			}
			pp.p[0] = atof(pb1);										// �����ϊ�
			pp.p[1] = atof(pb2);
			pp.p[2] = atof(pb3);
			PutCopyParaPnt(cmp, i, pp);									// ���W���Ұ��ݒ�
		}
	}
	fclose(fp);															// ���̧��CLOSE
	remove(tpath);														// ���̧�ٍ폜
	View->SetDataUse();													// �ް��g�p���ݒ�
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] ccn;
	delete[] pb1;
	delete[] pb2;
	delete[] pb3;
	return ret;
}
