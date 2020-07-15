/////////////////////////////////////////////////////////////////////////////
// ﾌｧｲﾙ入出力部1
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

CProgressDlg* m_pProgressF1=NULL;										// ﾌﾟﾛｸﾞﾚｽｲﾝｼﾞｹｰﾀｰ

/*******************************************************/
int WriteJsdFile(const char* fname, int dflg, int sflg)					//<<< JSF/JSD/GLF/GLDﾌｧｲﾙ出力
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

	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&fp, tname, "w")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] ccn;
		delete[] name;
		return 1;
	}
	m_pProgressF1 = new CProgressDlg(3);
	m_pProgressF1->Create();											// ﾀﾞｲｱﾛｸﾞ表示
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	BaseOifPtr(1, 0, &oif);												// 表示中立体先頭取得
	while(oif) {
		GetOifObj(oif, &op);											// 構成情報立体ﾎﾟｲﾝﾀ取得
		BaseCmdPtr(1, op, &cmp);										// 有効ｺﾏﾝﾄﾞ先頭取得
		while(cmp) {
			scc++;														// ｺﾏﾝﾄﾞ数加算
			if(GetCmdNo(cmp)>=EARTH) {									// ｺﾏﾝﾄﾞNo取得
				vchk = 1;												// 新ｺﾏﾝﾄﾞ有(ｱｰｽｺﾏﾝﾄﾞ以降)
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		NextOifPtr(1, 0, &oif);											// 次表示中立体へ
	}
	if(vchk) {															// <新ｺﾏﾝﾄﾞ有の場合>
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
		remove(tname);													// 作業ﾌｧｲﾙ削除
		setlocale(LC_ALL, "");											// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
		m_pProgressF1->DestroyWindow();									// ﾀﾞｲｱﾛｸﾞ廃棄
		delete m_pProgressF1;
		delete[] ccn;
		delete[] name;
		return 2;
	}
	m_pProgressF1->SetProgress(5);										// 進捗表示(5%)
	InitOifNo();														// 立体構成立体No初期化
	if(sflg) {															// <ﾃﾞｰﾀ集(*.gld)>
		sprintf_s(ccn, 100, "C%02d", GLBDCMD);							// ﾃﾞｰﾀ集先頭ｺﾏﾝﾄﾞ
		fprintf(fp, "%s\n", ccn);										// ｺﾏﾝﾄﾞ情報書込
	}
	mflg = FALSE;														// 移動ﾌﾗｸﾞ初期化(OFF)
	oct = 0;
	BaseOifPtr(1, 0, &oif);												// 表示中立体先頭取得
	while(oif) {
		oflg = FALSE;													// 出力ﾌﾗｸﾞ初期化(OFF)
		SetOifNo(oif, oct++);											// 立体構成立体No設定
		GetOifObj(oif, &op);											// 構成情報立体ﾎﾟｲﾝﾀ取得
		BaseCmdPtr(1, op, &cmp);										// 有効ｺﾏﾝﾄﾞ先頭取得
		while(cmp) {
			oflg = TRUE;												// 出力ﾌﾗｸﾞON
			cno = GetCmdNo(cmp);										// ｺﾏﾝﾄﾞNo取得
			if(cno==CHGSURF) {											// ｺﾏﾝﾄﾞNo変更
				cno = CRTSURF;
			} else if(cno==CHGCURVE) {
				cno = CRTCURVE;
			} else if(cno==CHGSWEEP) {
				cno = CRTSWEEP;
			}
			inum = GetIntNum(cmp);										// 整数ﾊﾟﾗﾒｰﾀ数取得
			pnum = GetPntNum(cmp);										// 座標ﾊﾟﾗﾒｰﾀ数取得
			if(cno==MOVE) {												// <移動ｺﾏﾝﾄﾞの場合>
				if(mflg) {												// <前ｺﾏﾝﾄﾞが移動>
					dx += GetParaInt(cmp, 0);							// 移動量加算
					dy += GetParaInt(cmp, 1);
					dz += GetParaInt(cmp, 2);
				} else {												// <前ｺﾏﾝﾄﾞが移動でない>
					dx = GetParaInt(cmp, 0);							// 移動量取得
					dy = GetParaInt(cmp, 1);
					dz = GetParaInt(cmp, 2);
					mflg = TRUE;										// 移動ﾌﾗｸﾞON
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
				continue;												// 先頭へ戻る
			} else {													// <!=移動ｺﾏﾝﾄﾞの場合>
				if(mflg&&(dx!=0||dy!=0||dz!=0)) {						// <前ｺﾏﾝﾄﾞが有効な移動>
					fprintf(fp, "C08 3 0\n");							// 移動ｺﾏﾝﾄﾞ書込
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// 移動ﾌﾗｸﾞOFF
				}
			}
			if(cno==RING) {												// <<新ﾘﾝｸﾞｻｲｽﾞ合わせｺﾏﾝﾄﾞの場合>>
				sprintf_s(ccn, 100, "C%02d", SCALE);					// [ｻｲｽﾞ変更ｺﾏﾝﾄﾞ]
				fprintf(fp, "%s %d %d\n", ccn, 3, 1);					// ｺﾏﾝﾄﾞ情報書込
				for(i=0; i<3; i++) {
					fprintf(fp, "%d ", GetParaInt(cmp, i));				// 整数ﾊﾟﾗﾒｰﾀ書込
				}
				fprintf(fp, "\n");										// 改行
				if(dflg) {												// <JSD/GLDの場合>
					GetParaPnt(cmp, 0, &pp);							// 座標ﾊﾟﾗﾒｰﾀ取得
					fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000.0),
                                              (int)(pp.p[1]*1000.0),
                                              (int)(pp.p[2]*1000.0));	// 座標ﾊﾟﾗﾒｰﾀ書込
				} else {												// <JSF/GLFの場合>
					GetParaPnt(cmp, 0, &pp);							// 座標ﾊﾟﾗﾒｰﾀ取得
					fprintf(fp, "%f %f %f\n",
                                pp.p[0], pp.p[1], pp.p[2]);				// 座標ﾊﾟﾗﾒｰﾀ書込
				}
				sprintf_s(ccn, 100, "C%02d", BEND);						// [円筒曲げｺﾏﾝﾄﾞ]
				fprintf(fp, "%s %d %d\n", ccn, 3, 2);					// ｺﾏﾝﾄﾞ情報書込
				for(i=3; i<6; i++) {
					fprintf(fp, "%d ", GetParaInt(cmp, i));				// 整数ﾊﾟﾗﾒｰﾀ書込
				}
				fprintf(fp, "\n");										// 改行
				if(dflg) {												// <JSD/GLDの場合>
					for(i=1; i<3; i++) {
						GetParaPnt(cmp, i, &pp);						// 座標ﾊﾟﾗﾒｰﾀ取得
						fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000),
                                                  (int)(pp.p[1]*1000),
                                                  (int)(pp.p[2]*1000));	// 座標ﾊﾟﾗﾒｰﾀ書込
					}
				} else {												// <JSF/GLFの場合>
					for(i=1; i<3; i++) {
						GetParaPnt(cmp, i, &pp);						// 座標ﾊﾟﾗﾒｰﾀ取得
						fprintf(fp, "%f %f %f\n",
                                    pp.p[0], pp.p[1], pp.p[2]);			// 座標ﾊﾟﾗﾒｰﾀ書込
					}
				}
			} else {													// <<旧ｺﾏﾝﾄﾞの場合>>
				sprintf_s(ccn, 100, "C%02d", cno);
				fprintf(fp, "%s %d %d\n", ccn, inum, pnum);				// ｺﾏﾝﾄﾞ情報書込
				for(i=0, cnt=1; i<inum; i++) {
					fprintf(fp, "%d", GetParaInt(cmp, i));				// 整数ﾊﾟﾗﾒｰﾀ書込
					if(cnt%10) {										// 10件以内はｽﾍﾟｰｽ
						fprintf(fp, " ");
						cnt++;
					} else {											// 10件で改行
						fprintf(fp, "\n");
						cnt = 1;
					}
				}
				if(cnt!=1) {
					fprintf(fp, "\n");									// 改行
				}
				if(dflg) {												// <JSD/GLDの場合>
					for(i=0; i<pnum; i++) {
						GetParaPnt(cmp, i, &pp);						// 座標ﾊﾟﾗﾒｰﾀ取得
						fprintf(fp, "%d %d %d\n", (int)(pp.p[0]*1000),
                                                  (int)(pp.p[1]*1000),
                                                  (int)(pp.p[2]*1000));	// 座標ﾊﾟﾗﾒｰﾀ書込
					}
				} else {												// <JSF/GLFの場合>
					for(i=0; i<pnum; i++) {
						GetParaPnt(cmp, i, &pp);						// 座標ﾊﾟﾗﾒｰﾀ取得
						fprintf(fp, "%f %f %f\n",
                                    pp.p[0], pp.p[1], pp.p[2]);			// 座標ﾊﾟﾗﾒｰﾀ書込
					}
				}
			}
			ccc++;
			dpr = 70.0*((double)ccc/(double)scc)+5.0;					// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF1->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		if(ret) {														// 強制終了時は中止
			break;
		}
		if(mflg&&(dx!=0||dy!=0||dz!=0)) {								// <前ｺﾏﾝﾄﾞが有効な移動の場合>
			fprintf(fp, "C08 3 0\n");									// 移動ｺﾏﾝﾄﾞ書込
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// 移動ｺﾏﾝﾄﾞOFF
		}
		if(oflg) {														// <通常ｺﾏﾝﾄﾞ有の場合>
			WriteGroupFile(fp, op);										// ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存
			WriteColorFile(fp, op);										// ｶﾗｰNoﾌｧｲﾙ保存
			WritePartsFile(fp, op);										// ﾊﾟｰﾂNoﾌｧｲﾙ保存
			WriteAttributeFile(fp, op);									// 属性Noﾌｧｲﾙ保存
		}
		NextOifPtr(1, 0, &oif);											// 次表示中立体へ
	}
	if(!ret) {															// <正常処理中>
		cnt = 0, ccc = 0;												// ﾃﾞｰﾀ初期化
		memset(&buf, ' ', sizeof(OIFDATA));
		memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
		BaseOifPtr(1, 1, &oif);											// 表示中立体先頭取得
		while(oif) {
			lvl = GetOifLevel(oif);										// 立体構成ﾚﾍﾞﾙ取得
			num = GetOifNo(oif);										// 立体構成立体No取得
			GetOifName(oif, name);										// 立体構成名称取得
			GetOifObj(oif, &op);										// 構成情報立体ﾎﾟｲﾝﾀ取得
			sprintf_s(ccn, 100, "%03d", lvl);							// 立体構成ﾚﾍﾞﾙ設定
			memcpy(buf.dat[cnt].level, ccn, 3);
			sprintf_s(ccn, 100, "%04d", num);							// 立体構成立体No設定
			memcpy(buf.dat[cnt].ono, ccn, 4);
			memcpy(buf.dat[cnt].name, name, ONM_LENG);					// 立体構成名称設定
			cnt++, ccc++;												// ｶｳﾝﾀUP
			dpr = 15.0*((double)ccc/(double)oct)+75.0;					// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF1->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
			if(cnt>=8) {												// <ﾃﾞｰﾀ設定完了>
				fwrite(&buf, sizeof(OIFDATA), 1, fp);					// ﾌｧｲﾙ出力
				cnt = 0;												// ﾃﾞｰﾀ初期化
				memset(&buf, ' ', sizeof(OIFDATA));
				memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
			}
			NextOifPtr(1, 1, &oif);										// 次表示中立体へ
		}
		if(!ret&&cnt>0) {												// 未出力有はﾌｧｲﾙ出力
			fwrite(&buf, sizeof(OIFDATA), 1, fp);
		}
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	if(!ret) {															// <正常処理中>
		m_pProgressF1->SetProgress(95);									// 進捗表示(95%)
		if(!ConvCmdToJsd(fname)) {										// CMD->JSD変換
			ret = 1;
		}
		m_pProgressF1->SetProgress(100);								// 進捗表示(100%)
	}
	remove(tname);														// 作業ﾌｧｲﾙ削除
	if(!ret) {
		OffCmdFlg();													// ｺﾏﾝﾄﾞ未保存OFF
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	m_pProgressF1->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressF1;
	delete[] ccn;
	delete[] name;
	return ret;
}

/******************************************************/
static void ReadStop(int pflg, char* ccn, char* pb1,
                     char* pb2, char* pb3, char* rbuf)					//<<< ﾌｧｲﾙ入力中止処理
/******************************************************/
{
	if(pflg) {															// <ﾌﾟﾛｸﾞﾚｽ表示:有>
		m_pProgressF1->DestroyWindow();									// ﾀﾞｲｱﾛｸﾞ廃棄
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
                        char* pb1, char* pb2, char* pb3)				//<<< ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
/********************************************************/
{
	int i, j, inum, pnum, cnt=0;

	if(sflg) {
		fscanf_s(fp, "%s", ccn, 100);									// ﾃﾞｰﾀ集(ｺﾏﾝﾄﾞ情報取得)
	}
	while(TRUE) {
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ｺﾏﾝﾄﾞ情報取得
			break;
		}
		if(!memcmp(ccn, OIF_TITLE, 7)) {								// 立体構成情報へ
			break;
		}
		if(fscanf_s(fp, "%d%d", &inum, &pnum)!=2) {						// ﾊﾟﾗﾒｰﾀ数取得
			break;
		}
		if(ccn[0]!='C') {												// ﾃﾞｰﾀ異常で終了
			break;
		}
		cnt++;															// ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// 整数ﾊﾟﾗﾒｰﾀ取得
				break;
			}
		}
		for(i=0; i<pnum; i++) {											// 座標ﾊﾟﾗﾒｰﾀ取得
			if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
				break;
			}
		}
	}
	return cnt;
}

/*************************************************************/
static BOOL CheckPntTypeData(FILE* fp, int* fg, PNTTYPE* pp,
                             char* pb1, char* pb2, char* pb3)			//<<< PNTTYPEﾃﾞｰﾀﾁｪｯｸ
/*************************************************************/
{
	BOOL ret=TRUE;
	int  i, ia[3];

	memset(pb1, 0x00, 100);												// ﾊﾞｯﾌｧ初期化
	if(fscanf_s(fp, "%s", pb1, 100)!=1) {								// 先頭ﾃﾞｰﾀ読込み
		ret = FALSE;
	}
	if(strstr(pb1, ".")!=NULL) {										// <先頭ﾃﾞｰﾀが実数>
		pp->p[0] = atof(pb1);											// 数値変換
		if(fscanf_s(fp, "%s%s", pb2, 100, pb3, 100)!=2) {				// 残りの実数を読込み
			ret = FALSE;
		}
		pp->p[1] = atof(pb2), pp->p[2] = atof(pb3);
		*fg = 1;
	} else {															// <先頭ﾃﾞｰﾀが整数>
		sscanf_s(pb1, "%d", &ia[0]);									// 数値変換
		if(fscanf_s(fp, "%d%d", &ia[1], &ia[2])!=2) {					// 残りの整数を読込み
			ret = FALSE;
		}
		for(i=0; i<3; i++) {
			pp->p[i] = ia[i]/1000.0;									// 整数(μm)→実数(mm)
		}
		*fg = 0;
	}
	return ret;
}

/***********************************************************/
static BOOL GetPntTypeData(FILE* fp, int fg, PNTTYPE* pp,
                           char* pb1, char* pb2, char* pb3)				//<<< PNTTYPEﾃﾞｰﾀ取得
/***********************************************************/
{
	BOOL ret=TRUE;
	int  i, ia[3];

	if(fg) {															// <実数>
		if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
			ret = FALSE;
		}
		pp->p[0] = atof(pb1);											// 数値変換
		pp->p[1] = atof(pb2);
		pp->p[2] = atof(pb3);
	} else {															// <整数>
		if(fscanf_s(fp, "%d%d%d", &ia[0], &ia[1], &ia[2])!=3) {
			ret = FALSE;
		}
		for(i=0; i<3; i++) {
			pp->p[i] = ia[i]/1000.0;									// 整数(μm)→実数(mm)
		}
	}
	return ret;
}

/******************************************************/
int ReadJsdFile(const char* fname, int sflg, int pflg)					//<<< JSF/JSD/GLF/GLDﾌｧｲﾙ入力
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

	if(pflg) {															// <ﾌﾟﾛｸﾞﾚｽ表示:有>
		m_pProgressF1 = new CProgressDlg(4);
		m_pProgressF1->Create();										// ﾀﾞｲｱﾛｸﾞ表示
	}
	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(!ConvJsdToCmd(fname)) {											// JSD->CMD変換
		ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);						// ﾌｧｲﾙ入力中止処理
		return 1;
	}
	if(pflg) {															// <ﾌﾟﾛｸﾞﾚｽ表示:有>
		m_pProgressF1->SetProgress(5);									// 進捗表示(5%)
		if(fopen_s(&fp, tname, "r")!=0) {		    					// 作業ﾌｧｲﾙOPEN
			ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);					// ﾌｧｲﾙ入力中止処理
			return 1;
		}
		scc = CountCommand(fp, sflg, ccn, pb1, pb2, pb3);				// ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
		fclose(fp);														// 作業ﾌｧｲﾙCLOSE
		m_pProgressF1->SetProgress(10);									// 進捗表示(10%)
	}
	if(fopen_s(&fp, tname, "r")!=0) {									// 作業ﾌｧｲﾙOPEN
		ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);						// ﾌｧｲﾙ入力中止処理
		return 1;
	}
	if(sflg) {															// <ﾃﾞｰﾀ集(*.gld)>
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ｺﾏﾝﾄﾞ情報取得
			ok = FALSE;
		}
		if(ccn[0]!='C') {												// ﾃﾞｰﾀ異常で終了
			ok = FALSE;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// 数値変換
		if(cno!=GLBDCMD) {												// ﾃﾞｰﾀ異常で終了
			ok = FALSE;
		}
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	OnCmdFlgStp();														// 未保存ﾌﾗｸﾞ更新中止ON
	InitOifNo();														// 立体構成立体No初期化
	while(ok) {
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// ｺﾏﾝﾄﾞ情報取得
			break;
		} else {														// 1ﾃﾞｰﾀ読込みで正常終了へ
			ret = 0;
		}
		if(!memcmp(ccn, OIF_TITLE, 7)) {								// 立体構成情報へ切替え
			osw = 1; break;
		}
		if(fscanf_s(fp, "%d%d", &inum, &pnum)!=2) {						// ﾊﾟﾗﾒｰﾀ数取得
			break;
		}
		if(ccn[0]!='C') {												// ﾃﾞｰﾀ異常で終了
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// ｺﾏﾝﾄﾞNo数値変換
		if(cno==BEND) {													// <円筒曲げｺﾏﾝﾄﾞ>
			svf = (svf==1) ? 2 : svf;									// 保存ﾌﾗｸﾞ:2(円筒曲げ)
		} else if(cno==MOVE) {											// <移動ｺﾏﾝﾄﾞ>
			mvf = 1;
		} else {														// <円筒曲げ･移動ｺﾏﾝﾄﾞ以外>
			if(svf==1) {												// <ﾘﾝｸﾞｻｲｽﾞ合せ用でないｻｲｽﾞ変更>
				CreatCmd(SCALE, 3, 1, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				PutParaPnt(cmp, 0, psv[0]);								// 座標ﾊﾟﾗﾒｰﾀ設定
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
				svf = 0;												// ｺﾏﾝﾄﾞ保存なしへ変更
			}
			svf = (cno==SCALE) ? 1 : svf;								// ｻｲｽﾞ変更ｺﾏﾝﾄﾞは,保存ﾌﾗｸﾞ:1
		}
		if(svf==0&&mvf==0) {											// <ｺﾏﾝﾄﾞ保存無&移動以外>
			CreatCmd(cno, inum, pnum, &cmp);							// ｺﾏﾝﾄﾞ領域確保
		}
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// 整数ﾊﾟﾗﾒｰﾀ取得
				ret = 1; break;
			}
			if(mvf==1) {												// <移動ｺﾏﾝﾄﾞ>
				msv[i] = j;												// 整数ﾊﾟﾗﾒｰﾀ保存
			} else if(svf==0) {											// <ｺﾏﾝﾄﾞ保存なし>
				PutParaInt(cmp, i, j);									// 整数ﾊﾟﾗﾒｰﾀ設定
			} else if(svf==1) {											// <ｺﾏﾝﾄﾞ保存(ｻｲｽﾞ変更)>
				isv[i] = j;												// 整数ﾊﾟﾗﾒｰﾀ保存
			} else {													// <ｺﾏﾝﾄﾞ保存(円筒曲げ)>
				isv[i+3] = j;											// 整数ﾊﾟﾗﾒｰﾀ保存
			}
		}
		if(pnum>0) {
			if(!CheckPntTypeData(fp, &j, &pp, pb1, pb2, pb3)) {			// PNTTYPEﾃﾞｰﾀﾁｪｯｸ
				ret = 1; break;
			}
			if(svf==0) {												// <ｺﾏﾝﾄﾞ保存なし>
				PutParaPnt(cmp, 0, pp);									// 座標ﾊﾟﾗﾒｰﾀ設定
			} else if(svf==1) {											// <ｺﾏﾝﾄﾞ保存(ｻｲｽﾞ変更)>
				psv[0] = pp;											// 座標ﾊﾟﾗﾒｰﾀ保存
			} else {													// <ｺﾏﾝﾄﾞ保存(円筒曲げ)>
				psv[1] = pp;											// 座標ﾊﾟﾗﾒｰﾀ保存
			}
			for(i=1; i<pnum; i++) {										// 2点目以降
				if(!GetPntTypeData(fp, j, &pp, pb1, pb2, pb3)) {		// PNTTYPEﾃﾞｰﾀ取得
					ret = 1; break;
				}
				if(svf==0) {											// <ｺﾏﾝﾄﾞ保存なし>
					PutParaPnt(cmp, i, pp);								// 座標ﾊﾟﾗﾒｰﾀ設定
				} else {												// <ｺﾏﾝﾄﾞ保存(円筒曲げ)>
					psv[i+1] = pp;										// 座標ﾊﾟﾗﾒｰﾀ保存
				}
			}
		}
		if(mvf==1) {													// <移動ｺﾏﾝﾄﾞ>
			if(msv[0]!=0||msv[1]!=0||msv[2]!=0) {						// <移動量あり>
				if(svf==1) {											// <ﾘﾝｸﾞｻｲｽﾞ合せ用でないｻｲｽﾞ変更>
					CreatCmd(SCALE, 3, 1, &cmp);						// ｺﾏﾝﾄﾞ領域確保
					for(i=0; i<3; i++) {
						PutParaInt(cmp, i, isv[i]);						// 整数ﾊﾟﾗﾒｰﾀ設定
					}
					PutParaPnt(cmp, 0, psv[0]);							// 座標ﾊﾟﾗﾒｰﾀ設定
					SetCmdPtr(cmp);										// 処理対象ｺﾏﾝﾄﾞ設定
					ExecCommand(TRUE);									// ｺﾏﾝﾄﾞ実行
				}
				svf = 0;												// ｺﾏﾝﾄﾞ保存なしへ変更
				CreatCmd(MOVE, 3, 0, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, msv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
			}
			mvf = 0;
		} else if(svf==0) {												// <ｺﾏﾝﾄﾞ保存なし>
			SetCmdPtr(cmp);												// 処理対象ｺﾏﾝﾄﾞ設定
			ExecCommand(TRUE);											// ｺﾏﾝﾄﾞ実行
		} else if(svf==1) {												// <ｺﾏﾝﾄﾞ保存(ｻｲｽﾞ変更)>
			if(isv[0]!=1000||isv[2]!=1000) {							// <ﾘﾝｸﾞｻｲｽﾞ合せ用でない場合>
				CreatCmd(SCALE, 3, 1, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				PutParaPnt(cmp, 0, psv[0]);								// 座標ﾊﾟﾗﾒｰﾀ設定
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
				svf = 0;												// ｺﾏﾝﾄﾞ保存なしへ変更
			}
		} else {														// <ｺﾏﾝﾄﾞ保存(円筒曲げ)>
			if(isv[3]!=1||isv[4]!=2) {									// <ﾘﾝｸﾞｻｲｽﾞ合せ用でない場合>
				CreatCmd(SCALE, 3, 1, &cmp);							// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				PutParaPnt(cmp, 0, psv[0]);								// 座標ﾊﾟﾗﾒｰﾀ設定
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
				CreatCmd(BEND, 3, 2, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, isv[i+3]);						// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				PutParaPnt(cmp, 0, psv[1]);								// 座標ﾊﾟﾗﾒｰﾀ設定
				PutParaPnt(cmp, 1, psv[2]);
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
			} else {													// <ﾘﾝｸﾞｻｲｽﾞ合せ用の場合>
				CreatCmd(RING, 6, 3, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				for(i=0; i<6; i++) {
					PutParaInt(cmp, i, isv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
				for(i=0; i<3; i++) {
					PutParaPnt(cmp, i, psv[i]);							// 座標ﾊﾟﾗﾒｰﾀ設定
				}
				SetCmdPtr(cmp);											// 処理対象ｺﾏﾝﾄﾞ設定
				ExecCommand(TRUE);										// ｺﾏﾝﾄﾞ実行
			}
			svf = 0;													// ｺﾏﾝﾄﾞ保存なしへ変更
		}
		if(cno==ENDCMD) {												// <最終ｺﾏﾝﾄﾞの場合>
			GetObjPtr(&op);												// 立体のﾎﾟｲﾝﾀ取得
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			GetObjOif(op, &oif);										// 立体構成情報ﾎﾟｲﾝﾀ取得
			SetOifNo(oif, oct++);										// 立体構成立体No設定
			View->SetDispList(op);										// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
		}
		if(pflg) {														// <ﾌﾟﾛｸﾞﾚｽ表示:有>
			ccc++;
			dpr = 85.0*((double)ccc/(double)scc)+10.0;					// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF1->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF1->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF1->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
		}
	}
	if(!ret) {															// <正常処理中>
		bfo = NULL, sto = NULL;											// 体構成ﾎﾟｲﾝﾀ初期化
		BaseOifPtr(0, 0, &oif);											// 先頭立体構成ﾎﾟｲﾝﾀ取得
		while(oif!=NULL) {
			if(GetOifNo(oif)>=0) {										// 今回読込立体で終了
				break;
			}
			bfo = oif;													// 前立体構成ﾎﾟｲﾝﾀ保存
			NextOifPtr(0, 0, &oif);										// 次立体構成ﾎﾟｲﾝﾀ取得
		}
		if(oif==NULL) {													// 有効立体構成無は処理中止
			osw = 0;
		}
		if(osw) {														// <<立体構成情報>>
			memcpy(rbuf, ccn, 7);										// ﾀｲﾄﾙ複写
			if(fread(&rbuf[7], JSD_LENG-7, 1, fp)!=1) {					// ﾃﾞｰﾀ入力(249byte)
				osw = 0;
			}
			if(osw) {
				memcpy(&buf, rbuf, JSD_LENG);							// 入力ﾃﾞｰﾀを構造体へｺﾋﾟｰ
			}
		}
		if(osw) {														// <<立体構成情報更新有>>
			blv = -1, bpf = -1;											// 前ﾚﾍﾞﾙ,前出力ﾌﾗｸﾞ初期化
			while(osw) {
				for(i=0; i<8; i++) {
					if(!memcmp(buf.dat[i].level, "   ", 3)) {			// ﾃﾞｰﾀ無は終了
						osw = 0; break;
					}
					memset(ccn, ' ', 100);
					memcpy(ccn, buf.dat[i].level, 3);
					sscanf_s(ccn, "%d", &lvl);							// 立体構成ﾚﾍﾞﾙNo取得
					memset(ccn, ' ', 100);
					memcpy(ccn, buf.dat[i].ono, 4);
					sscanf_s(ccn, "%d", &num);							// 立体構成立体No取得
					if(oif!=NULL&&GetOifNo(oif)==num) {					// <<立体Noが同じ場合>>
						SetOifLevel(oif, lvl);							// 立体構成ﾚﾍﾞﾙ設定
						SetOifName(0, oif, buf.dat[i].name);			// 立体構成名称設定
						bfo = oif;										// 前立体構成ﾎﾟｲﾝﾀ保存
						if(!st) {										// 先頭ﾃﾞｰﾀ保存
							sto = oif, st = 1;
						}
						NextOifPtr(0, 0, &oif);							// 次立体構成ﾎﾟｲﾝﾀ取得
						bpf = 0;										// 前出力は立体
					} else {											// <<立体Noが異なる場合>>
						if(bpf==1&&blv==lvl) {							// <直前がﾕﾆｯﾄでﾚﾍﾞﾙが一致>
							SetOifName(1, bfo, buf.dat[i].name);		// 立体構成名称設定(上書き)
						} else {										// <直前がﾕﾆｯﾄ以外かﾚﾍﾞﾙが不一致>
							if(bfo==NULL) {								// 前立体構成なしは
								CreatOif(1, NULL, OBJ_NAME00,
                                                  NULL, &nwo);			// 構成情報領域確保(先頭接続)
							} else {									// 前立体構成ありは
								CreatOif(2, NULL, OBJ_NAME00,
                                                  bfo, &nwo);			// 構成情報領域確保(中間接続)
							}
							SetOifLevel(nwo, lvl);						// 立体構成ﾚﾍﾞﾙ設定
							SetOifName(0, nwo, buf.dat[i].name);		// 立体構成名称設定
							if(!st) {									// 先頭ﾃﾞｰﾀ保存
								sto = nwo, st = 1;
							}
							bfo = nwo;									// 前立体構成ﾎﾟｲﾝﾀ置換
						}
						bpf = 1;										// 前出力はﾕﾆｯﾄ
					}
					blv = lvl;											// 前ﾚﾍﾞﾙ保存
				}
				if(fread(&buf, sizeof(OIFDATA), 1, fp)!=1) {			// ﾃﾞｰﾀ入力(256byte)
					break;
				}
			}
		} else {														// <<立体構成情報更新無>>
			while(oif!=NULL) {
				View->SetOifItem(oif);									// 立体構成情報ｱｲﾃﾑ設定
				if(!st) {												// 先頭ﾃﾞｰﾀ保存
					sto = oif, st = 1;
				}
				NextOifPtr(0, 0, &oif);									// 次立体構成ﾎﾟｲﾝﾀ取得
			}
		}
		if(pflg) {
			m_pProgressF1->SetProgress(100);							// 進捗表示(100%)
		}
		if(sto!=NULL) {
			View->SelectOifItem(sto);									// 先頭ﾃﾞｰﾀｱｲﾃﾑ選択
		}
	}
	OffCmdFlgStp();														// 未保存ﾌﾗｸﾞ更新中止OFF
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	remove(tname);														// 作業ﾌｧｲﾙ削除
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	ReadStop(pflg, ccn, pb1, pb2, pb3, rbuf);							// ﾌｧｲﾙ入力中止処理
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	return ret;
}

/***************************/
int WriteCopyFile(UINT chr)												//<<< ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ出力
/***************************/
{
	char*    ccn = new char[50];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pp;
	FILE*    fp;
	CString  fpath, tpath;
	int      cno, dsp, edt, inum, pnum, i, cnt, mflg, oflg, dx, dy, dz;

	if(chr==0) {														// <Key押下なし>
		fpath = GetAppPath(0)+COP_NAME1;								// ｺﾋﾟｰﾌｧｲﾙPATH取得
	} else {															// <Key押下あり>
		fpath.Format(COP_NAME2, chr);									// Key付ｺﾋﾟｰﾌｧｲﾙ
		fpath = GetAppPath(0)+fpath;									// ｺﾋﾟｰﾌｧｲﾙPATH取得
	}
	tpath = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&fp, tpath, "w")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] ccn;
		return 1;
	}
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	BaseObjPtr(2, &op, &dsp, &edt);										// 有効立体先頭取得
	mflg = FALSE;														// 移動ﾌﾗｸﾞ初期化(OFF)
	while(op) {
		oflg = FALSE;													// 出力ﾌﾗｸﾞ初期化(OFF)
		BaseCmdPtr(1, op, &cmp);										// 有効ｺﾏﾝﾄﾞ先頭取得
		while(cmp) {
			oflg = TRUE;												// 出力ﾌﾗｸﾞON
			cno = GetCmdNo(cmp);										// ｺﾏﾝﾄﾞNo取得
			if(cno==CHGSURF) {											// ｺﾏﾝﾄﾞNo変更
				cno = CRTSURF;
			}
			if(cno==CHGCURVE) {
				cno = CRTCURVE;
			}
			if(cno==CHGSWEEP) {
				cno = CRTSWEEP;
			}
			inum = GetIntNum(cmp);										// 整数ﾊﾟﾗﾒｰﾀ数取得
			pnum = GetPntNum(cmp);										// 座標ﾊﾟﾗﾒｰﾀ数取得
			if(cno==MOVE) {												// <移動ｺﾏﾝﾄﾞの場合>
				if(mflg) {												// <前ｺﾏﾝﾄﾞが移動>
					dx += GetParaInt(cmp, 0);							// 移動量加算
					dy += GetParaInt(cmp, 1);
					dz += GetParaInt(cmp, 2);
				} else {												// <前ｺﾏﾝﾄﾞが移動でない>
					dx = GetParaInt(cmp, 0);							// 移動量取得
					dy = GetParaInt(cmp, 1);
					dz = GetParaInt(cmp, 2);
					mflg = TRUE;										// 移動ﾌﾗｸﾞON
				}
				NextCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
				continue;												// 先頭へ戻る
			} else {													// <!=移動ｺﾏﾝﾄﾞの場合>
				if(mflg) {												// <前ｺﾏﾝﾄﾞが移動>
					fprintf(fp, "C08 3 0\n");							// 移動ｺﾏﾝﾄﾞ書込
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// 移動ﾌﾗｸﾞOFF
				}
			}
			sprintf_s(ccn, 50, "C%02d", cno);
			fprintf(fp, "%s %d %d\n", ccn, inum, pnum);					// ｺﾏﾝﾄﾞ情報書込
			for(i=0, cnt=1; i<inum; i++) {
				fprintf(fp, "%d", GetParaInt(cmp, i));					// 整数ﾊﾟﾗﾒｰﾀ書込
				if(cnt%10) {											// 10件以内はｽﾍﾟｰｽ
					fprintf(fp, " ");
					cnt++;
				} else {												// 10件で改行
					fprintf(fp, "\n");
					cnt = 1;
				}
			}
			if(cnt!=1) {
				fprintf(fp, "\n");										// 改行
			}
			for(i=0; i<pnum; i++) {
				GetParaPnt(cmp, i, &pp);								// 座標ﾊﾟﾗﾒｰﾀ取得
				fprintf(fp, "%f %f %f\n", pp.p[0], pp.p[1], pp.p[2]);	// 座標ﾊﾟﾗﾒｰﾀ書込
			}
			NextCmdPtr(&cmp);											// 次ｺﾏﾝﾄﾞへ
		}
		if(mflg) {														// <前ｺﾏﾝﾄﾞが移動の場合>
			fprintf(fp, "C08 3 0\n");									// 移動ｺﾏﾝﾄﾞ書込
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// 移動ｺﾏﾝﾄﾞOFF
		}
		if(oflg) {														// <通常ｺﾏﾝﾄﾞ有の場合>
			WriteGroupFile(fp, op);										// ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存
			WriteColorFile(fp, op);										// ｶﾗｰNoﾌｧｲﾙ保存
			WritePartsFile(fp, op);										// ﾊﾟｰﾂNoﾌｧｲﾙ保存
			WriteAttributeFile(fp, op);									// 属性Noﾌｧｲﾙ保存
		}
		NextObjPtr(2, &op, &dsp, &edt);									// 次有効立体へ
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	if(!ConvCmdToJsd(fpath)) {											// CMD->JSD変換
		delete[] ccn;
		return 1;
	}
	remove(tpath);														// 作業ﾌｧｲﾙ削除
	delete[] ccn;
	return 0;
}

/**************************/
int ReadCopyFile(UINT chr)												//<<< ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ入力
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

	if(chr==0) {														// <Key押下なし>
		fpath = GetAppPath(0)+COP_NAME1;								// ｺﾋﾟｰﾌｧｲﾙPATH取得
	} else {															// <Key押下あり>
		fpath.Format(COP_NAME2, chr);									// Key付ｺﾋﾟｰﾌｧｲﾙ
		fpath = GetAppPath(0)+fpath;									// ｺﾋﾟｰﾌｧｲﾙPATH取得
	}
	tpath = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(!ConvJsdToCmd(fpath)) {											// JSD->CMD変換
		delete[] ccn;
		delete[] pb1;
		delete[] pb2;
		delete[] pb3;
		return 1;
	}
	if(fopen_s(&fp, tpath, "r")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] ccn;
		delete[] pb1;
		delete[] pb2;
		delete[] pb3;
		return 1;
	}
	FreeCopyCmd();														// ｺﾋﾟｰｺﾏﾝﾄﾞ領域解放
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	while(1) {
		if(fscanf_s(fp, "%s%d%d", ccn, 100, &inum, &pnum)!=3) {			// ｺﾏﾝﾄﾞ情報取得
			break;
		} else {
			ret = 0;
		}
		if(ccn[0]!='C') {												// ﾃﾞｰﾀ異常で終了
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// 数値変換
		CreatCopyCmd(cno, inum, pnum, &cmp);							// ｺﾋﾟｰｺﾏﾝﾄﾞ領域確保
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &pi)!=1) {							// 整数ﾊﾟﾗﾒｰﾀ取得
				ret = 1; break;
			}
			PutCopyParaInt(cmp, i, pi);									// 整数ﾊﾟﾗﾒｰﾀ設定
		}
		for(i=0; i<pnum; i++) {											// 座標ﾊﾟﾗﾒｰﾀ取得
			if(fscanf_s(fp, "%s%s%s", pb1, 100, pb2, 100, pb3, 100)!=3) {
				ret = 1; break;
			}
			pp.p[0] = atof(pb1);										// 実数変換
			pp.p[1] = atof(pb2);
			pp.p[2] = atof(pb3);
			PutCopyParaPnt(cmp, i, pp);									// 座標ﾊﾟﾗﾒｰﾀ設定
		}
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	remove(tpath);														// 作業ﾌｧｲﾙ削除
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] ccn;
	delete[] pb1;
	delete[] pb2;
	delete[] pb3;
	return ret;
}
