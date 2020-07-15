/////////////////////////////////////////////////////////////////////////////
// ﾌｧｲﾙ入出力部2
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

CProgressDlg* m_pProgressF2=NULL;										// ﾌﾟﾛｸﾞﾚｽｲﾝｼﾞｹｰﾀｰ

/*********************************************/
int WriteJgdFile(const char* fname, int sflg)							//<<< JGD/GGDﾌｧｲﾙ出力
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

	if(sflg==2) {														// <<平面分割/図形演算ﾊﾞｯｸｱｯﾌﾟ(JGD)>>
		oifmode = 3;													// 編集立体/selno=1立体
		memset(wpwd, NULL, sizeof(wpwd));								// 書込ﾊﾟｽﾜｰﾄﾞ初期化
		memset(rpwd, NULL, sizeof(rpwd));								// 読込ﾊﾟｽﾜｰﾄﾞ初期化
	} else {															// <<通常(JGD/GGD)ﾌｧｲﾙ出力>>
		oifmode = 1;													// 表示立体
		MainWnd->GetPasswordData(wpwd, rpwd, &savf);					// ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
		if(!savf) {														// ﾊﾟｽﾜｰﾄﾞ保存:無は
			memset(wpwd, NULL, sizeof(wpwd));							// 書込ﾊﾟｽﾜｰﾄﾞ初期化
			memset(rpwd, NULL, sizeof(rpwd));							// 読込ﾊﾟｽﾜｰﾄﾞ初期化
		}
	}
	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	if(fopen_s(&fp, tname, "w")!=0) {									// 作業ﾌｧｲﾙOPEN
		delete[] ccn;
		delete[] name;
		return 1;
	}

	m_pProgressF2 = new CProgressDlg(3);
	m_pProgressF2->Create();											// ﾀﾞｲｱﾛｸﾞ表示
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	BaseOifPtr(oifmode, 0, &oif);										// 表示中立体先頭取得
	while(oif) {
		GetOifObj(oif, &op);											// 構成情報立体ﾎﾟｲﾝﾀ取得
		obno = GetObjNo(op);											// 立体No取得
		min_obno = (obno<min_obno) ? obno : min_obno;					// 最小の立体No検索
		BaseCmdPtr(0, op, &cmp);										// ﾍﾞｰｽｺﾏﾝﾄﾞ先頭取得
		while(cmp) {
			clvl = GetCmdLvl(cmp);										// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo取得
			min_clvl = (clvl<min_clvl) ? clvl : min_clvl;				// 最小のｺﾏﾝﾄﾞﾚﾍﾞﾙNo検索
			scc++;														// ｺﾏﾝﾄﾞ数加算
			NextAllCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
		NextOifPtr(oifmode, 0, &oif);									// 次表示中立体へ
	}
	m_pProgressF2->SetProgress(5);										// 進捗表示(5%)
	sprintf_s(ccn, 100, "C%02d", WPWDCMD);								// 書込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ
	fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d\n",
                ccn, wpwd[0], wpwd[1], wpwd[2], wpwd[3], wpwd[4],
                wpwd[5], wpwd[6], wpwd[7], wpwd[8], wpwd[9]);			// ｺﾏﾝﾄﾞ情報書込
	sprintf_s(ccn, 100, "C%02d", RPWDCMD);								// 読込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ
	fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d\n",
                ccn, rpwd[0], rpwd[1], rpwd[2], rpwd[3], rpwd[4],
                rpwd[5], rpwd[6], rpwd[7], rpwd[8], rpwd[9]);			// ｺﾏﾝﾄﾞ情報書込
	InitOifNo();														// 立体構成立体No初期化
	if(sflg==1) {														// <ﾃﾞｰﾀ集(*.gld)>
		sprintf_s(ccn, 100, "C%02d", GLBDCMD);							// ﾃﾞｰﾀ集先頭ｺﾏﾝﾄﾞ
		fprintf(fp, "%s\n", ccn);										// ｺﾏﾝﾄﾞ情報書込
	}
	mflg = FALSE;														// 移動ﾌﾗｸﾞ初期化(OFF)
	oct = 0;
	BaseOifPtr(oifmode, 0, &oif);										// 表示中立体先頭取得
	while(oif) {
		oflg = FALSE;													// 出力ﾌﾗｸﾞ初期化(OFF)
		SetOifNo(oif, oct++);											// 立体構成立体No設定
		GetOifObj(oif, &op);											// 構成情報立体ﾎﾟｲﾝﾀ取得
		obno = GetObjNo(op);											// 立体No取得
		BaseCmdPtr(0, op, &cmp);										// ﾍﾞｰｽｺﾏﾝﾄﾞ先頭取得
		while(cmp) {
			clvl = GetCmdLvl(cmp);										// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo取得
			oflg = TRUE;												// 出力ﾌﾗｸﾞON
			cno = GetCmdNo(cmp);										// ｺﾏﾝﾄﾞNo取得
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
				mono = obno, mlvl = clvl;								// 移動ｺﾏﾝﾄﾞ立体No/ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
				NextAllCmdPtr(&cmp);									// 次ｺﾏﾝﾄﾞへ
				continue;												// 先頭へ戻る
			} else {													// <!=移動ｺﾏﾝﾄﾞの場合>
				if(mflg&&(dx!=0||dy!=0||dz!=0)) {						// <前ｺﾏﾝﾄﾞが有効な移動>
					i = mono - min_obno, j = mlvl - min_clvl;			// 最小値からの番号
					fprintf(fp, "C08 %d %d 3 0\n", i, j);				// 移動ｺﾏﾝﾄﾞ書込
					fprintf(fp, "%d %d %d\n", dx ,dy, dz);
					mflg = FALSE;										// 移動ﾌﾗｸﾞOFF
				}
			}
			sprintf_s(ccn, 100, "C%02d", cno);
			i = obno - min_obno, j = clvl - min_clvl;					// 最小値からの番号
			fprintf(fp, "%s %d %d %d %d\n", ccn, i, j, inum, pnum);		// ｺﾏﾝﾄﾞ情報書込
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
			ccc++;
			dpr = 70.0*((double)ccc/(double)scc)+5.0;					// 現在ﾊﾟｰｾﾝﾄ
			if(spr<(int)dpr) {											// <ﾊﾟｰｾﾝﾄUP>
				spr = (int)dpr;
				m_pProgressF2->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
				if(msg.message==WM_LBUTTONUP) {							// 強制終了
					ret = -1; break;
				}
			}
			NextAllCmdPtr(&cmp);										// 次ｺﾏﾝﾄﾞへ
		}
		if(ret) {														// 強制終了時は中止
			break;
		}
		if(mflg&&(dx!=0||dy!=0||dz!=0)) {								// <前ｺﾏﾝﾄﾞが有効な移動の場合>
			i = mono - min_obno, j = mlvl - min_clvl;					// 最小値からの番号
			fprintf(fp, "C08 %d %d 3 0\n", i ,j);						// 移動ｺﾏﾝﾄﾞ書込
			fprintf(fp, "%d %d %d\n", dx ,dy, dz);
			mflg = FALSE;												// 移動ｺﾏﾝﾄﾞOFF
		}
		if(oflg) {														// <通常ｺﾏﾝﾄﾞ有の場合>
			WriteGroupFileJgd(fp, op, obno-min_obno);					// ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存(JGD用)
			WriteColorFileJgd(fp, op, obno-min_obno);					// ｶﾗｰNoﾌｧｲﾙ保存(JGD用)
			WritePartsFileJgd(fp, op, obno-min_obno);					// ﾊﾟｰﾂNoﾌｧｲﾙ保存(JGD用)
			WriteAttributeFileJgd(fp, op, obno-min_obno);				// 属性Noﾌｧｲﾙ保存(JGD用)
		}
		NextOifPtr(oifmode, 0, &oif);									// 次表示中立体へ
	}
	if(!ret) {															// <正常処理中>
		cnt = 0, ccc = 0;												// ﾃﾞｰﾀ初期化
		memset(&buf, ' ', sizeof(OIFDATA));
		memcpy(buf.title, OIF_TITLE, sizeof(buf.title));
		BaseOifPtr(oifmode, 1, &oif);									// 表示中立体先頭取得
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
				m_pProgressF2->SetProgress(spr);						// 進捗表示
			}
			if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                           0, 0, PM_NOREMOVE)) {
				GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
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
			NextOifPtr(oifmode, 1, &oif);								// 次表示中立体へ
		}
		if(!ret&&cnt>0) {												// 未出力有はﾌｧｲﾙ出力
			fwrite(&buf, sizeof(OIFDATA), 1, fp);
		}
	}
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	if(!ret) {															// <正常処理中>
		m_pProgressF2->SetProgress(95);									// 進捗表示(95%)
		if(!ConvCmdToJsd(fname)) {										// CMD->JSD変換
			ret = 1;
		}
		m_pProgressF2->SetProgress(100);								// 進捗表示(100%)
	}
	remove(tname);														// 作業ﾌｧｲﾙ削除
	if(sflg!=2) {														// <<通常(JGD/GGD)ﾌｧｲﾙ出力>>
		if(!ret) {
			OffCmdFlg();												// ｺﾏﾝﾄﾞ未保存OFF
		}
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	m_pProgressF2->DestroyWindow();										// ﾀﾞｲｱﾛｸﾞ廃棄
	delete m_pProgressF2;
	delete[] ccn;
	delete[] name;
	return ret;
}

/*****************************************************/
static void ReadStop(char* ccn, char* pb1, char* pb2,
                     char* pb3, char* rbuf)								//<<< ﾌｧｲﾙ入力中止処理
/*****************************************************/
{
	if(m_pProgressF2!=NULL) {
		m_pProgressF2->DestroyWindow();									// ﾀﾞｲｱﾛｸﾞ廃棄
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
int ReadJgdFile(const char* fname, int sflg, int uflg)					//<<< JGD/GGDﾌｧｲﾙ入力
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

	m_pProgressF2 = NULL;												// ﾌﾟﾛｸﾞﾚｽ初期化
	MainWnd->GetPasswordData(wpwd, rpwd, &savf);						// ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
	tname = GetAppPath(0)+TMP_NAME;										// 作業ﾌｧｲﾙPATH取得
	while(TRUE) {
		i = ConvJgdToCmd(fname, wpwd, &flg);							// JGD->CMD変換
		if(i==1) {														// 入出力ｴﾗｰは
			ReadStop(ccn, pb1, pb2, pb3, rbuf);							// ﾌｧｲﾙ入力中止処理
			return 1;
		} else if(i==-1) {												// ﾊﾟｽﾜｰﾄﾞｴﾗｰは
			CPasswdDlg2 dlg;
			if(dlg.DoModal()==IDOK) {									// ﾊﾟｽﾜｰﾄﾞ設定ﾀﾞｲｱﾛｸﾞ表示
				dlg.GetValue(&str);
				len = str.GetLength();
				memset(wpwd, NULL, sizeof(wpwd));
				memcpy(wpwd, (const char*)str, len);
			} else {													// ﾀﾞｲｱﾛｸﾞｷｬﾝｾﾙは
				ReadStop(ccn, pb1, pb2, pb3, rbuf);						// ﾌｧｲﾙ入力中止処理
				remove(tname);											// 作業ﾌｧｲﾙ削除
				return 2;
			}
		} else {
			break;														// 変換正常終了は次処理へ
		}
	}
	m_pProgressF2 = new CProgressDlg(4);
	m_pProgressF2->Create();											// ﾀﾞｲｱﾛｸﾞ表示
	m_pProgressF2->SetProgress(5);										// 進捗表示(5%)
	if(fopen_s(&fp, tname, "r")!=0) {									// 作業ﾌｧｲﾙOPEN
		ReadStop(ccn, pb1, pb2, pb3, rbuf);								// ﾌｧｲﾙ入力中止処理
		return 1;
	}
	if(fscanf_s(fp, "%s", ccn, 100)!=1) {								// 書込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ情報取得
		ok = FALSE;
	}
	if(ccn[0]!='C') {													// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
		ok = FALSE;
	}
	sscanf_s(&ccn[1], "%d", &cno);										// 数値変換
	if(cno!=WPWDCMD) {													// 書込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
		ok = FALSE;
	}
	if(fscanf_s(fp, "%d%d%d%d%d%d%d%d%d%d",
                    &i, &i, &i, &i, &i, &i, &i, &i, &i, &i)!=10) {		// 書込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ読み飛ばし
		ok = FALSE;
	}
	if(flg) {															// <読込ﾊﾟｽﾜｰﾄﾞ設定有>
		if(fscanf_s(fp, "%s", ccn, 100)!=1) {							// 読込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ情報取得
			ok = FALSE;
		}
		if(ccn[0]!='C') {												// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
			ok = FALSE;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// 数値変換
		if(cno!=RPWDCMD) {												// 読込ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ異常で終了
			ok = FALSE;
		}
		if(fscanf_s(fp, "%d%d%d%d%d%d%d%d%d%d",
                        &i, &i, &i, &i, &i, &i, &i, &i, &i, &i)!=10) {	// 読込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞ読み飛ばし
			ok = FALSE;
		}
	}
	if(sflg) {															// <ﾃﾞｰﾀ集(*.ggd)>
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
		if(fscanf_s(fp, "%d%d%d%d", &obno, &clvl, &inum, &pnum)!=4) {	// ﾊﾟﾗﾒｰﾀ数取得
			break;
		}
		if(obno!=obno_sv) {												// <立体Noが異なる場合>
			CreateObjWork();											// 立体作業ﾃﾞｰﾀ領域確保
			obno_sv = obno;
		}
		max_clvl = (clvl>max_clvl) ? clvl : max_clvl;					// 最大ｺﾏﾝﾄﾞﾚﾍﾞﾙ検索
		if(ccn[0]!='C') {												// ﾃﾞｰﾀ異常で終了
			ret = 1; break;
		}
		sscanf_s(&ccn[1], "%d", &cno);									// ｺﾏﾝﾄﾞNo数値変換
		scc++;															// ｺﾏﾝﾄﾞ数ｶｳﾝﾄ
		mvf = (cno==MOVE) ? 1 : 0;										// 移動ｺﾏﾝﾄﾞ
		if(mvf==0) {													// <移動ｺﾏﾝﾄﾞ以外>
			CreatCmd(cno, inum, pnum, &cmp);							// ｺﾏﾝﾄﾞ領域確保
			CreateCmdWork(clvl, cmp);									// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ領域確保
		}
		for(i=0; i<inum; i++) {
			if(fscanf_s(fp, "%d", &j)!=1) {								// 整数ﾊﾟﾗﾒｰﾀ取得
				ret = 1; break;
			}
			if(mvf==1) {												// <移動ｺﾏﾝﾄﾞ>
				msv[i] = j;												// 整数ﾊﾟﾗﾒｰﾀ保存
			} else  {													// <以外>
				PutParaInt(cmp, i, j);									// 整数ﾊﾟﾗﾒｰﾀ設定
			}
		}
		if(pnum>0) {
			if(!CheckPntTypeData(fp, &j, &pp, pb1, pb2, pb3)) {			// PNTTYPEﾃﾞｰﾀﾁｪｯｸ
				ret = 1; break;
			}
			PutParaPnt(cmp, 0, pp);										// 座標ﾊﾟﾗﾒｰﾀ設定
			for(i=1; i<pnum; i++) {										// 2点目以降
				if(!GetPntTypeData(fp, j, &pp, pb1, pb2, pb3)) {		// PNTTYPEﾃﾞｰﾀ取得
					ret = 1; break;
				}
				PutParaPnt(cmp, i, pp);									// 座標ﾊﾟﾗﾒｰﾀ設定
			}
		}
		if(mvf==1) {													// <移動ｺﾏﾝﾄﾞ>
			if(msv[0]!=0||msv[1]!=0||msv[2]!=0) {						// <移動量あり>
				CreatCmd(MOVE, 3, 0, &cmp);								// ｺﾏﾝﾄﾞ領域確保
				CreateCmdWork(clvl, cmp);								// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ領域確保
				for(i=0; i<3; i++) {
					PutParaInt(cmp, i, msv[i]);							// 整数ﾊﾟﾗﾒｰﾀ設定
				}
			}
		}
	}
	m_pProgressF2->SetProgress(10);										// 進捗表示(10%)
	if(!ret) {															// <<正常の場合>>
		for(i=0; i<=max_clvl; i++) {									// 開始ｺﾏﾝﾄﾞﾚﾍﾞﾙ～最終ｺﾏﾝﾄﾞﾚﾍﾞﾙ
			chk = 0;
			while(TRUE) {
				SearchCmdWork(i, &cwk, &cmp, &op);						// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ検索
				if(cmp!=NULL) {											// <ｺﾏﾝﾄﾞ有の場合>
					if(!chk) {											// 各ｺﾏﾝﾄﾞﾚﾍﾞﾙ初回のみ
						if(uflg) {										// <個別Undo/Redoあり>
							InitLevel();								// Undo/Redo初期化
						}
						chk = 1;
					}
					if(uflg) {											// <個別Undo/Redoあり>
						InitCmdLvl(cmp);								// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo初期化
					}
					SetCmdPtr(cmp);										// 処理対象ｺﾏﾝﾄﾞ設定
					if(op!=NULL) {
						SetObjPtr(op);									// 処理対象立体設定
					}
					ExecCommand(TRUE);									// ｺﾏﾝﾄﾞ実行
					if(op==NULL) {
						GetObjPtr(&op);									// 処理対象立体取得
						SetOwkObjPtr(cwk, op);							// 立体作業ﾃﾞｰﾀ立体ﾎﾟｲﾝﾀ設定
					}
					ChangeCwkFlg(cwk);									// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀﾌﾗｸﾞ更新
					ccc++;
					dpr = 85.0*((double)ccc/(double)scc)+10.0;			// 現在ﾊﾟｰｾﾝﾄ
					if(spr<(int)dpr) {									// <ﾊﾟｰｾﾝﾄUP>
						spr = (int)dpr;
						m_pProgressF2->SetProgress(spr);				// 進捗表示
					}
					if(PeekMessage(&msg, m_pProgressF2->m_Button.m_hWnd,
                                   0, 0, PM_NOREMOVE)) {
						GetMessage(&msg, m_pProgressF2->m_Button.m_hWnd, 0, 0);
						if(msg.message==WM_LBUTTONUP) {					// 強制終了
							ret = -1; break;
						}
					}
				} else {												// <ｺﾏﾝﾄﾞ無の場合>
					break;												// 次ｺﾏﾝﾄﾞﾚﾍﾞﾙへ
				}
			}
			if(ret==-1) {
				break;
			}
		}
		if(!ret) {														// <<正常処理中>>
			while(TRUE) {												// <ｸﾞﾙｰﾌﾟ､ｶﾗｰ､ﾊﾟｰﾂ､属性ｺﾏﾝﾄﾞ>
				SearchCmdWork(-1, &cwk, &cmp, &op);						// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ検索
				if(cmp!=NULL) {											// <ｺﾏﾝﾄﾞ有の場合>
					if(uflg) {											// <個別Undo/Redoあり>
						InitCmdLvl(cmp);								// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo初期化
					}
					SetCmdPtr(cmp);										// 処理対象ｺﾏﾝﾄﾞ設定
					SetObjPtr(op);										// 処理対象立体設定
					ExecCommand(TRUE);									// ｺﾏﾝﾄﾞ実行
				} else {												// <ｺﾏﾝﾄﾞ無の場合>
					break;												// 次ｺﾏﾝﾄﾞﾚﾍﾞﾙへ
				}
			}
			LastObjWorkProc();											// 立体作業ﾃﾞｰﾀ最終処理
			if(uflg) {													// <個別Undo/Redoあり>
				FileBackUp2();											// ﾌｧｲﾙﾊﾞｯｸｱｯﾌﾟ処理2
			}
		}
		FreeObjCmdWork(0);												// 立体/ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ解放
	} else {															// <<異常の場合>>
		FreeObjCmdWork(1);												// 立体/ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ解放
	}
	if(!ret) {															// <正常処理中>
		SortOifData();													// 立体構成ﾃﾞｰﾀｿｰﾄ
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
		m_pProgressF2->SetProgress(100);								// 進捗表示(100%)
		if(sto!=NULL) {
			View->SelectOifItem(sto);									// 先頭ﾃﾞｰﾀｱｲﾃﾑ選択
		}
	}
	OffCmdFlgStp();														// 未保存ﾌﾗｸﾞ更新中止OFF
	fclose(fp);															// 作業ﾌｧｲﾙCLOSE
	remove(tname);														// 作業ﾌｧｲﾙ削除
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	ReadStop(ccn, pb1, pb2, pb3, rbuf);									// ﾌｧｲﾙ入力中止処理
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	return ret;
}
