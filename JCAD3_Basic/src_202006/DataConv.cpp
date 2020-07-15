/////////////////////////////////////////////////////////////////////////////
// JCAD3標準ファイル変換部(JSD形式)
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
static void ConvertJsd(unsigned char* rb, unsigned char* wb)			//<<< JSD変換
/************************************************************/
{
	int i, j, bny;

	for(i=0; i<JSD_LENG; i++) {
		TextToBinary(rb[2*i], &j);										// TEXT->BINARY
		bny = 16 * j;													// 上位4bit
		TextToBinary(rb[(2*i)+1], &j);									// TEXT->BINARY
		bny += j;														// 下位4bit
		wb[i] = (char)bny;												// BINARY設定
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
static void ConvertCmd(unsigned char* rb, unsigned char* wb)			//<<< CMD変換
/************************************************************/
{
	int i, j, bny;

	for(i=0; i<JSD_LENG; i++) {
		bny = (int)rb[i];												// BINARY変換
		j = bny / 16;													// 上位4bit
		BinaryToText(j, &wb[2*i]);										// BINARY->TEXT
		j = bny % 16;													// 下位4bit
		BinaryToText(j, &wb[(2*i)+1]);									// BINARY->TEXT
	}
}

/************************************/
BOOL ConvCmdToJsd(const char* fname)									//<<< CMD->JSD変換
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

	memset(rb, ' ', CMD_LENG+2);										// 入力ﾊﾞｯﾌｧ初期化
	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&rfp, tname, "r")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] bf;
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	if(fopen_s(&wfp, fname, "wb")!=0) {	    							// 出力ﾌｧｲﾙOPEN
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
		memset(bf, NULL, 100);											// ﾊﾞｯﾌｧ初期化
		if(fscanf_s(rfp, "%s", bf, 100)!=1) {							// ﾃﾞｰﾀ入力
			break;
		} else {
			ret = TRUE;
		}
		if(!memcmp(bf, OIF_TITLE, 7)) {									// 立体構成情報へ切替え
			osw = 1; break;
		}
		for(cnt=0; cnt<100; cnt++) {
			if(bf[cnt]==0x0) {											// 有効桁数ｶｳﾝﾄ
				break;
			}
		}
		if(rcnt+cnt+1>512) {
			ConvertJsd(rb, wb);											// JSD変換
			fwrite(wb, JSD_LENG, 1, wfp);								// JSD書込
			memset(rb, ' ', CMD_LENG+2);								// 入力ﾊﾞｯﾌｧ初期化
			rcnt=0;
		}
		memcpy(&rb[rcnt], bf, cnt);
		rcnt += (cnt+1);
	}
	if(rcnt>0) {
		ConvertJsd(rb, wb);												// JSD変換
		fwrite(wb, JSD_LENG, 1, wfp);									// JSD書込
	}
	if(osw) {															// <立体構成情報>
		memcpy(wb, bf, 7);												// ﾀｲﾄﾙ複写
		if(fread(&wb[7], JSD_LENG-7, 1, rfp)!=1) {						// ﾃﾞｰﾀ入力
			osw = 0;
		}
		while(osw) {
			fwrite(wb, JSD_LENG, 1, wfp);								// JSD書込
			if(fread(wb, JSD_LENG, 1, rfp)!=1) {						// ﾃﾞｰﾀ入力
				break;
			}
		}
	}
	fclose(wfp);														// 出力ﾌｧｲﾙCLOSE
	fclose(rfp);														// 作業ﾌｧｲﾙCLOSE
	delete[] bf;
	delete[] rb;
	delete[] wb;
	return ret;
}

/************************************/
BOOL ConvJsdToCmd(const char* fname)									//<<< JSD->CMD変換
/************************************/
{
	unsigned char* rb = new unsigned char[JSD_LENG+2];
	unsigned char* wb = new unsigned char[CMD_LENG+2];
	FILE*   rfp;
	FILE*   wfp;
	BOOL    ret=FALSE;
	CString tname;

	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&rfp, fname, "rb")!=0) {		    						// 入力ﾌｧｲﾙOPEN
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	if(fopen_s(&wfp, tname, "w")!=0) {	    							// 作業ﾌｧｲﾙOPEN
		delete[] rb;
		delete[] wb;
		return FALSE;
	}
	while(TRUE) {
		if(fread(rb, JSD_LENG, 1, rfp)!=1) {							// ﾃﾞｰﾀ入力
			break;
		} else {
			ret = TRUE;
		}
		if(memcmp(rb, OIF_TITLE, 7)) {									// <ｺﾏﾝﾄﾞ情報>
			ConvertCmd(rb, wb);											// CMD変換
			fwrite(wb, CMD_LENG, 1, wfp);								// CMD書込
		} else {														// <立体構成情報>
			fwrite(rb, JSD_LENG, 1, wfp);								// CMD書込
		}
	}
	fclose(wfp);														// 作業ﾌｧｲﾙCLOSE
	fclose(rfp);														// 出力ﾌｧｲﾙCLOSE
	delete[] rb;
	delete[] wb;
	return ret;
}

/*********************************************************/
int ConvJgdToCmd(const char* fname, char* wpwd, int* flg)				//<<< JGD->CMD変換
/*********************************************************/
{
	unsigned char* rb = new unsigned char[JSD_LENG+2];
	unsigned char* wb = new unsigned char[CMD_LENG+2];
	FILE*   rfp;
	FILE*   wfp;
	int     ret=1, stf=1, cno, ix, pw[10];
	CString tname;

	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&rfp, fname, "rb")!=0) {		    						// 入力ﾌｧｲﾙOPEN
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

	if(fopen_s(&wfp, tname, "w")!=0) {	    							// 作業ﾌｧｲﾙOPEN
		delete[] rb;
		delete[] wb;
		return 1;
	}
	while(TRUE) {
		if(fread(rb, JSD_LENG, 1, rfp)!=1) {							// ﾃﾞｰﾀ入力
			break;
		} else {
			ret = 0;													// 正常へ
		}
		if(memcmp(rb, OIF_TITLE, 7)) {									// <<ｺﾏﾝﾄﾞ情報>>
			ConvertCmd(rb, wb);											// CMD変換
			if(stf) {													// <先頭のみﾁｪｯｸ>
				if(wb[0]!='C') {										// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
					ret = 1; break;
				}
				if(sscanf_s((char*)&wb[1], "%d%d%d%d%d%d%d%d%d%d%d",
                            &cno, &pw[0], &pw[1], &pw[2],
                            &pw[3], &pw[4], &pw[5], &pw[6],
                            &pw[7], &pw[8], &pw[9])!=11) {				// 書込ﾊﾟｽﾜｰﾄﾞ数値変換
					ret = 1; break;
				}
				if(cno!=WPWDCMD) {										// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
					ret = 1; break;
				} else {
					*flg = 0;											// 書込ﾊﾟｽﾜｰﾄﾞ:OK
					if(pw[0]!=0||pw[1]!=0||pw[2]!=0||pw[3]!=0||
                       pw[4]!=0||pw[5]!=0||pw[6]!=0||
                       pw[7]!=0||pw[8]!=0||pw[9]!=0) {					// <ﾊﾟｽﾜｰﾄﾞ指定済み>
						if(wpwd[0]!=pw[0]||wpwd[1]!=pw[1]||
                           wpwd[2]!=pw[2]||wpwd[3]!=pw[3]||
                           wpwd[4]!=pw[4]||wpwd[5]!=pw[5]||
                           wpwd[6]!=pw[6]||wpwd[7]!=pw[7]||
                           wpwd[8]!=pw[8]||wpwd[9]!=pw[9]) {			// ﾊﾟｽﾜｰﾄﾞが異なるは終了
							ret = -1; break;
						}
					}
				}
				for(ix=1; ix<CMD_LENG; ix++) {							// 読込ﾊﾟｽﾜｰﾄﾞを検索
					if(wb[ix]=='C') {
						break;
					}
				}
				if(ix==CMD_LENG) {										// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
					ret = 1; break;
				}
				if(sscanf_s((char*)&wb[ix+1], "%d", &cno)!=1) {
					ret = 1; break;
				}
				if(cno==RPWDCMD) {										// <読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ:有>
					*flg = 1;											// 読込ﾊﾟｽﾜｰﾄﾞ:OK
					if(sscanf_s((char*)&wb[ix+1], "%d%d%d%d%d%d%d%d%d%d%d",
                                &cno, &pw[0], &pw[1], &pw[2],
                                &pw[3], &pw[4], &pw[5], &pw[6],
                                &pw[7], &pw[8], &pw[9])!=11) {			// 書込ﾊﾟｽﾜｰﾄﾞ数値変換
						ret = 1; break;
					}
				}
				stf = 0;
			}
			fwrite(wb, CMD_LENG, 1, wfp);								// CMD書込
		} else {														// <<立体構成情報>>
			fwrite(rb, JSD_LENG, 1, wfp);								// CMD書込
		}
	}
	fclose(wfp);														// 作業ﾌｧｲﾙCLOSE
	fclose(rfp);														// 出力ﾌｧｲﾙCLOSE
	delete[] rb;
	delete[] wb;
	return ret;
}
