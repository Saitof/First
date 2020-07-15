/////////////////////////////////////////////////////////////////////////////
// JCAD3�W���t�@�C���ϊ���(JSD�`��)
// DataConv.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/09/28 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "DataType.h"

/*****************************************************/
static void TextToBinary(unsigned char txt, int* bny)					//<<< TEXT->BINARY
/*****************************************************/
{
	if(txt>=0x31&&txt<=0x39) {
		*bny = (int)txt-0x30;
	} else if(txt==0x30) {
		*bny = 10;
	} else if(txt==0x20) {
		*bny = 0;
	} else if(txt==0x2d) {
		*bny = 11;
	} else if(txt==0x2e) {
		*bny = 12;
	} else if(txt==0x43) {
		*bny = 13;
	} else {
		*bny = 15;
	}
}

/************************************************************/
static void ConvertJsd(unsigned char* rb, unsigned char* wb)			//<<< JSD�ϊ�
/************************************************************/
{
	int i, j, bny;

	for(i=0; i<JSD_LENG; i++) {
		TextToBinary(rb[2*i], &j);										// TEXT->BINARY
		bny = 16 * j;													// ���4bit
		TextToBinary(rb[(2*i)+1], &j);									// TEXT->BINARY
		bny += j;														// ����4bit
		wb[i] = (char)bny;												// BINARY�ݒ�
	}
}

/*****************************************************/
static void BinaryToText(int bny, unsigned char* txt)					//<<< BINARY->TEXT
/*****************************************************/
{
	if(bny>=1&&bny<=9) {
		*txt = (char)(bny+0x30);
	} else if(bny==10) {
		*txt = '0';
	} else if(bny==0) {
		*txt = ' ';
	} else if(bny==11) {
		*txt = '-';
	} else if(bny==12) {
		*txt = '.';
	} else if(bny==13) {
		*txt = 'C';
	} else {
		*txt = '*';
	}
}

/************************************************************/
static void ConvertCmd(unsigned char* rb, unsigned char* wb)			//<<< CMD�ϊ�
/************************************************************/
{
	int i, j, bny;

	for(i=0; i<JSD_LENG; i++) {
		bny = (int)rb[i];												// BINARY�ϊ�
		j = bny / 16;													// ���4bit
		BinaryToText(j, &wb[2*i]);										// BINARY->TEXT
		j = bny % 16;													// ����4bit
		BinaryToText(j, &wb[(2*i)+1]);									// BINARY->TEXT
	}
}

/************************************/
BOOL ConvCmdToJsd(const char* fname)									//<<< CMD->JSD�ϊ�
/************************************/
{
	unsigned char* bf = new unsigned char[100];
	unsigned char* rb = new unsigned char[CMD_LENG+2];
	unsigned char* wb = new unsigned char[JSD_LENG+2];
	FILE*   rfp;
	FILE*   wfp;
	BOOL    ret=FALSE;
	CString tname;
	int     cnt, rcnt=0, osw=0;

	memset(rb, ' ', CMD_LENG+2);										// �����ޯ̧������
	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&rfp, tname, "r")!=0) {									// ���̧��OPEN
		delete[] bf;
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	if(fopen_s(&wfp, fname, "wb")!=0) {	    							// �o��̧��OPEN
		delete[] bf;
		delete[] rb;
		delete[] wb;
		return FALSE;
	}

#if 1	//202006 write djf header  ConvCmdToJgd()
	if (!strcmp(&fname[strlen(fname)-4],".djf")) {  
		unsigned char cc[4] = { 0x80, 0x80 + 'd',0x80 + 'j',0x80 + 'f' };
		fwrite(cc, 4, 1, wfp);
	}
#endif

	while(TRUE) {
		memset(bf, NULL, 100);											// �ޯ̧������
		if(fscanf_s(rfp, "%s", bf, 100)!=1) {							// �ް�����
			break;
		} else {
			ret = TRUE;
		}
		if(!memcmp(bf, OIF_TITLE, 7)) {									// ���̍\�����֐ؑւ�
			osw = 1; break;
		}
		for(cnt=0; cnt<100; cnt++) {
			if(bf[cnt]==0x0) {											// �L����������
				break;
			}
		}
		if(rcnt+cnt+1>512) {
			ConvertJsd(rb, wb);											// JSD�ϊ�
			fwrite(wb, JSD_LENG, 1, wfp);								// JSD����
			memset(rb, ' ', CMD_LENG+2);								// �����ޯ̧������
			rcnt=0;
		}
		memcpy(&rb[rcnt], bf, cnt);
		rcnt += (cnt+1);
	}
	if(rcnt>0) {
		ConvertJsd(rb, wb);												// JSD�ϊ�
		fwrite(wb, JSD_LENG, 1, wfp);									// JSD����
	}
	if(osw) {															// <���̍\�����>
		memcpy(wb, bf, 7);												// ���ٕ���
		if(fread(&wb[7], JSD_LENG-7, 1, rfp)!=1) {						// �ް�����
			osw = 0;
		}
		while(osw) {
			fwrite(wb, JSD_LENG, 1, wfp);								// JSD����
			if(fread(wb, JSD_LENG, 1, rfp)!=1) {						// �ް�����
				break;
			}
		}
	}
	fclose(wfp);														// �o��̧��CLOSE
	fclose(rfp);														// ���̧��CLOSE
	delete[] bf;
	delete[] rb;
	delete[] wb;
	return ret;
}

/************************************/
BOOL ConvJsdToCmd(const char* fname)									//<<< JSD->CMD�ϊ�
/************************************/
{
	unsigned char* rb = new unsigned char[JSD_LENG+2];
	unsigned char* wb = new unsigned char[CMD_LENG+2];
	FILE*   rfp;
	FILE*   wfp;
	BOOL    ret=FALSE;
	CString tname;

	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&rfp, fname, "rb")!=0) {		    						// ����̧��OPEN
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	if(fopen_s(&wfp, tname, "w")!=0) {	    							// ���̧��OPEN
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	while(TRUE) {
		if(fread(rb, JSD_LENG, 1, rfp)!=1) {							// �ް�����
			break;
		} else {
			ret = TRUE;
		}
		if(memcmp(rb, OIF_TITLE, 7)) {									// <����ޏ��>
			ConvertCmd(rb, wb);											// CMD�ϊ�
			fwrite(wb, CMD_LENG, 1, wfp);								// CMD����
		} else {														// <���̍\�����>
			fwrite(rb, JSD_LENG, 1, wfp);								// CMD����
		}
	}
	fclose(wfp);														// ���̧��CLOSE
	fclose(rfp);														// �o��̧��CLOSE
	delete[] rb;
	delete[] wb;
	return ret;
}

/*********************************************************/
int ConvJgdToCmd(const char* fname, char* wpwd, int* flg)				//<<< JGD->CMD�ϊ�
/*********************************************************/
{
	unsigned char* rb = new unsigned char[JSD_LENG+2];
	unsigned char* wb = new unsigned char[CMD_LENG+2];
	FILE*   rfp;
	FILE*   wfp;
	int     ret=1, stf=1, cno, ix, pw[10];
	CString tname;

	tname = GetAppPath(0)+TMP_NAME;										// ���̧��PATH�擾
	if(fopen_s(&rfp, fname, "rb")!=0) {		    						// ����̧��OPEN
		delete[] rb;
		delete[] wb;
		return 1;
	}

#if 1	//202006 read djf header in ConvJgdToCmd()
	if (!strcmp(&fname[strlen(fname) - 4], ".djf")) {  
		unsigned char cc[4];
		fread(cc, 4, 1, rfp);
		if ((cc[1] & 0x7f) != 'd' || (cc[2] & 0x7f) != 'j' || (cc[3] & 0x7f) != 'f')
			return 1;
	}
#endif

	if(fopen_s(&wfp, tname, "w")!=0) {	    							// ���̧��OPEN
		delete[] rb;
		delete[] wb;
		return 1;
	}
	while(TRUE) {
		if(fread(rb, JSD_LENG, 1, rfp)!=1) {							// �ް�����
			break;
		} else {
			ret = 0;													// �����
		}
		if(memcmp(rb, OIF_TITLE, 7)) {									// <<����ޏ��>>
			ConvertCmd(rb, wb);											// CMD�ϊ�
			if(stf) {													// <�擪�̂�����>
				if(wb[0]!='C') {										// �����߽ܰ���ް��ُ�ŏI��
					ret = 1; break;
				}
				if(sscanf_s((char*)&wb[1], "%d%d%d%d%d%d%d%d%d%d%d",
                            &cno, &pw[0], &pw[1], &pw[2],
                            &pw[3], &pw[4], &pw[5], &pw[6],
                            &pw[7], &pw[8], &pw[9])!=11) {				// �����߽ܰ�ސ��l�ϊ�
					ret = 1; break;
				}
				if(cno!=WPWDCMD) {										// �����߽ܰ���ް��ُ�ŏI��
					ret = 1; break;
				} else {
					*flg = 0;											// �����߽ܰ��:OK
					if(pw[0]!=0||pw[1]!=0||pw[2]!=0||pw[3]!=0||
                       pw[4]!=0||pw[5]!=0||pw[6]!=0||
                       pw[7]!=0||pw[8]!=0||pw[9]!=0) {					// <�߽ܰ�ގw��ς�>
						if(wpwd[0]!=pw[0]||wpwd[1]!=pw[1]||
                           wpwd[2]!=pw[2]||wpwd[3]!=pw[3]||
                           wpwd[4]!=pw[4]||wpwd[5]!=pw[5]||
                           wpwd[6]!=pw[6]||wpwd[7]!=pw[7]||
                           wpwd[8]!=pw[8]||wpwd[9]!=pw[9]) {			// �߽ܰ�ނ��قȂ�͏I��
							ret = -1; break;
						}
					}
				}
				for(ix=1; ix<CMD_LENG; ix++) {							// �Ǎ��߽ܰ�ނ�����
					if(wb[ix]=='C') {
						break;
					}
				}
				if(ix==CMD_LENG) {										// �Ǎ��߽ܰ���ް��ُ�ŏI��
					ret = 1; break;
				}
				if(sscanf_s((char*)&wb[ix+1], "%d", &cno)!=1) {
					ret = 1; break;
				}
				if(cno==RPWDCMD) {										// <�Ǎ��߽ܰ���ް�:�L>
					*flg = 1;											// �Ǎ��߽ܰ��:OK
					if(sscanf_s((char*)&wb[ix+1], "%d%d%d%d%d%d%d%d%d%d%d",
                                &cno, &pw[0], &pw[1], &pw[2],
                                &pw[3], &pw[4], &pw[5], &pw[6],
                                &pw[7], &pw[8], &pw[9])!=11) {			// �����߽ܰ�ސ��l�ϊ�
						ret = 1; break;
					}
				}
				stf = 0;
			}
			fwrite(wb, CMD_LENG, 1, wfp);								// CMD����
		} else {														// <<���̍\�����>>
			fwrite(rb, JSD_LENG, 1, wfp);								// CMD����
		}
	}
	fclose(wfp);														// ���̧��CLOSE
	fclose(rfp);														// �o��̧��CLOSE
	delete[] rb;
	delete[] wb;
	return ret;
}
