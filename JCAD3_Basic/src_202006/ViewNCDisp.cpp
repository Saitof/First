/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
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
static int DisplayNCData(CString tname)									//<<< NCﾃﾞｰﾀ表示
/***************************************/
{
	char*   bf = new char[100];
	MSG     msg;
	FILE*   fp;
	PNTTYPE pts, pte;
	DWORD   wt=0;
	int     ret=0, i, x, y, z, dm, flg=0, col=0;

	if(fopen_s(&fp, tname, "r")==0) {									// 作業ﾌｧｲﾙｵｰﾌﾟﾝ
		fscanf_s(fp, "%s", bf, 100);									// 作業ﾃﾞｰﾀ読込み
		if(!strcmp(bf, "A")) {
			fscanf_s(fp, "%d%d%d%s", &dm, &dm, &dm, bf, 100);			// ﾎﾞｯｸｽｻｲｽﾞ取得
			for(i=0; i<DIM; i++) {										// 作業領域初期化
				pts.p[i] = 0.0, pte.p[i] = 0.0;
			}
			if(!strcmp(bf,"B")) {
				fscanf_s(fp, "%d%s", &dm, bf, 100);						// 積層間隔取得
			}
			while(!feof(fp)){
				if(!strcmp(bf,"E")) {									// <造形移動>
					fscanf_s(fp, "%d%d%s", &x, &y, bf, 100);			// 座標読込み
					pte.p[0] = x/1000.0, pte.p[1] = y/1000.0;
					View->SetNCDisp(pts, pte);							// NCﾃﾞｰﾀ登録
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;			// 次ﾗｲﾝ用保存
					flg = 1;
				} else if(!strcmp(bf, "G")) {							// <高速移動[Z無]>
					fscanf_s(fp, "%d%d%s", &x, &y, bf, 100);			// 座標読込み
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;			// 次ﾗｲﾝ用保存
				} else if(!strcmp(bf, "F")) {							// <高速移動[Z有]>
					fscanf_s(fp, "%d%d%d%s", &x, &y, &z, bf, 100);		// 座標読込み
					pts.p[0] = x/1000.0, pts.p[1] = y/1000.0;
					pts.p[2] = z/1000.0, pte.p[2] = z/1000.0;
					if((!feof(fp))&&flg) {								// <中間層の終了>
						View->DispNCDisp(col);							// NCﾃﾞｰﾀ表示
						Sleep(wt);										// 待ち
						col = (!col) ? 1 : 0;							// 表示色変換
						flg = 0;
						if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
							GetMessage(&msg, NULL, 0, 0);
							if(msg.message==WM_RBUTTONUP) {				// 強制終了
								ret = 1; break;
							} else if(msg.message==WM_LBUTTONUP) {		// 待ち時間設定
								if(wt<100) {
									wt = 100;							// 1回目:0.1秒待ち
								} else if(wt<300) {
									wt = 300;							// 2回目:0.3秒待ち
								} else if(wt<500) {
									wt = 500;							// 3回目:0.5秒待ち
								} else {
									wt = 0;								// 4回目:0.0秒待ち
								}
							}
						}
						View->RestoreWaitCursor();						// ｳｪｲﾄ･ｶｰｿﾙ再表示
					}
				} else {
					fscanf_s(fp, "%s", bf, 100);						// 作業ﾃﾞｰﾀ読込み
				}
			}
			if(flg) {
				View->DispNCDisp(col);									// NCﾃﾞｰﾀ表示
			}
		}
		fclose(fp);														// 作業ﾌｧｲﾙｸﾛｰｽﾞ
	}
	delete[] bf;
	return ret;
}

/**********************************************************************/
int CJcad3GlbView::DisplayNCProc(int mode, CString path, BOXTYPE* box)	//<<< NCﾃﾞｰﾀ表示&造形時間予測
/**********************************************************************/
{
	char*   bf = new char[100];
	FILE*   fp;
	PNTTYPE cpt;
	BOXTYPE bx;
	CString pname;
	int     ret1, ret2, i, x, y, z;

	if(mode==0) {														// <表示画面調整あり>
		if(fopen_s(&fp, path, "r")==0) {								// 作業ﾌｧｲﾙｵｰﾌﾟﾝ
			for(i=0; i<DIM; i++) {										// ﾎﾞｯｸｽｻｲｽﾞ設定
				bx.bp[0].p[i] = 0.0;
			}
			fscanf_s(fp, "%s%d%d%d", bf, 100, &x, &y, &z);
			bx.bp[1].p[0] = x/1000.0;
			bx.bp[1].p[1] = y/1000.0;
			bx.bp[1].p[2] = z/1000.0;
			m_Fovy = (int)((BoxCenter(bx, &cpt)*1000.0)/0.8+0.5);		// ﾎﾞｯｸｽｾﾝﾀｰ&最大幅取得
			ScrToPnt(1, &m_Cent3D, &cpt);
			m_pState->SetCent3D(cpt);
			SetSeishaei(m_Cent3D, m_Fovy);								// 正射影行列再構築
			ChangeTexPixels();											// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
			if(m_GridDisp) {
				SetGrid();												// ｸﾞﾘｯﾄﾞ設定
			}
			CentPars();													// 透視図参照点変更
			fclose(fp);													// 作業ﾌｧｲﾙｸﾛｰｽﾞ
		}
	} else if(mode==2) {												// <表示画面調整あり(SLC)>
		bx = *box;
		m_Fovy = (int)((BoxCenter(bx, &cpt)*1000.0)/0.8+0.5);			// ﾎﾞｯｸｽｾﾝﾀｰ&最大幅取得
		ScrToPnt(1, &m_Cent3D, &cpt);
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// 正射影行列再構築
		ChangeTexPixels();												// ﾃｸｽﾁｬﾋﾟｸｾﾙ更新
		if(m_GridDisp) {
			SetGrid();													// ｸﾞﾘｯﾄﾞ設定
		}
		CentPars();														// 透視図参照点変更
	}
	CConfirmDlg  dlg;
	while(TRUE) {
		Display(0);														// 再描画
		BeginWaitCursor();												// ｳｪｲﾄ･ｶｰｿﾙ表示
		InitNCDisp();													// NCﾃﾞｰﾀ表示初期処理
		ret2 = DisplayNCData(path);										// NCﾃﾞｰﾀ表示
		EndNCDisp();													// NCﾃﾞｰﾀ表示終了処理
		EndWaitCursor();												// ｳｪｲﾄ･ｶｰｿﾙ消去
		if(ret2) {														// 中止
			break;
		}
		if(dlg.DoModal()==IDOK) {										// ﾀﾞｲｱﾛｸﾞ表示
			break;
		}
	}
	ret1 = dlg.m_EndMode;												// 表示終了ﾓｰﾄﾞ
	if(ret2) {															// 中止
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(!m_NNuInp&&!ret1) {										// <ﾀﾞｲｱﾛｸﾞ表示あり>
		pname = GetAppPath(0)+NCET_NAME;								// ﾃﾞｰﾀ保存ﾌｧｲﾙPATH取得
		ret2 = EstimateTime(path, pname);								// 造形時間予測
		if(ret2>0) {													// ﾃﾞｰﾀｴﾗｰ
			AfxMessageBox(IDS_ERR_FILE);
		} else if(ret2<0) {												// 中止
			AfxMessageBox(IDS_STOP_PROCESS);
		}
	}
	delete[] bf;
	return ret1;
}

/***************************************/
void CJcad3GlbView::MenuNCDisplay(void)									//<<< NCﾃﾞｰﾀ表示
/***************************************/
{
	CString fname, tname, ext="nc", filter;
	DWORD   ffg;
	BOXTYPE box;
	int     i, mode, ret1, ret2=0;

	CheckCtrlKey1();													// 制御ｷｰ確認(起動時)
	tname = GetAppPath(0)+NCT_NAME;										// NC作業ﾌｧｲﾙ名
	if(!CheckFilePath(1, tname)) {										// <NC作業ﾌｧｲﾙ無の場合>
		if(!CheckFilePath(0, m_NCFilePath)) {							// ﾊﾟｽ確認
			m_NCFilePath = GetAppPath(0);
		}
		ffg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;						// ｢NCﾌｧｲﾙを開く｣
		filter.LoadString(IDS_FILE_FILTER106);
		CFileDlg dlg(TRUE, ext, fname, ffg, filter);					// ﾀﾞｲｱﾛｸﾞ表示
		dlg.m_ofn.lpstrInitialDir = m_NCFilePath;
		if(dlg.DoModal()==IDOK) {
			ext = dlg.GetFileExt();										// ﾌｧｲﾙ拡張子取得
			fname = dlg.GetPathName();									// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			i = fname.ReverseFind('\\');
			m_NCFilePath = fname.Left(i+1);								// ﾊﾟｽ保存
		} else {
			return;
		}
		if(!ext.CompareNoCase("slc")) {									// <SLC形式>
			ret1 = ChgSLCFile(fname, tname, &box);						// SLCﾌｧｲﾙ変換
			mode = 2;													// 表示画面調整有(SLC)
		} else {														// <NC形式>
			ret1 = ChgNCFile(fname, tname);								// NCﾌｧｲﾙ変換
			mode = 0;													// 表示画面調整有(NC)
		}
		if(ret1==0) {													// <NCﾌｧｲﾙ変換正常終了>
			ret2 = DisplayNCProc(mode, tname, &box);					// NCﾃﾞｰﾀ表示&造形時間予測
		} else {
			if(ret1>0) {												// ﾃﾞｰﾀｴﾗｰ
				AfxMessageBox(IDS_ERR_FILE);
			} else {													// 中止
				AfxMessageBox(IDS_STOP_PROCESS);
			}
		}
	} else {															// <NC作業ﾌｧｲﾙ無の場合>
		ret2 = DisplayNCProc(1, tname, NULL);							// NCﾃﾞｰﾀ表示&造形時間予測
	}
	if(!ret2) {
		remove(tname);													// 作業ﾌｧｲﾙ削除
	}
}

/****************************************/
void CJcad3GlbView::MenuSimulation(void)								//<<< ｼﾐｭﾚｰｼｮﾝ
/****************************************/
{
	OBJTYPE* op;
	PNTTYPE* vtx;
	int*     flp;
	int      i1, i2, rvr, smr, dsr=0;
	CString  path, pname;

	CheckCtrlKey1();													// 制御ｷｰ確認(起動時)
	BaseObjPtr(2, &op, &i1, &i2);										// 表示立体ﾎﾟｲﾝﾀ取得
	if(op==NULL) {														// <立体無中止>
		return;
	}
	GetDataBasePtr(&op, &vtx, &flp);									// ﾃﾞｰﾀﾍﾞｰｽ先頭ﾎﾟｲﾝﾀ取得
	pname = GetAppPath(0)+NCRV_NAME;									// ﾃﾞｰﾀ保存ﾌｧｲﾙPATH取得
	rvr = ReviseObject(op, vtx, flp, pname);							// 立体寸法補正
	if(rvr==-2) {														// 処理途中で中止
		AfxMessageBox(IDS_STOP_PROCESS);
	} else if(rvr==2) {													// ﾒﾓﾘｴﾗｰでｼｽﾃﾑ停止
		MemErr(IDS_MEMERR5);
	}
	if(rvr==1||rvr==0) {
		path = GetAppPath(0)+NCT_NAME;									// NC作業ﾌｧｲﾙ名
		pname = GetAppPath(0)+NCSM_NAME;								// ﾃﾞｰﾀ保存ﾌｧｲﾙPATH取得
		smr = SimulateMakeNC(op, vtx, flp, path, pname);				// NC作成ｼﾐｭﾚｰｼｮﾝ
		if(smr==-2) {													// 処理途中で中止
			AfxMessageBox(IDS_STOP_PROCESS);
		} else if(smr==0) {												// ﾃﾞｰﾀｴﾗｰ
			AfxMessageBox(IDS_ERR_FILE);
		} else if(smr==2) {												// ﾒﾓﾘｴﾗｰでｼｽﾃﾑ停止
			MemErr(IDS_MEMERR5);
		}
		if(smr==1) {
			dsr = DisplayNCProc(1, path, NULL);							// NCﾃﾞｰﾀ表示&造形時間予測
		}
		if(smr!=-1&&!dsr) {
			remove(path);												// 作業ﾌｧｲﾙ削除
		}
	}
	if(rvr==1||rvr==-2) {												// <寸法補正あり>
		BeginWaitCursor();												// ｳｪｲﾄ･ｶｰｿﾙ表示
		BaseObjPtr(2, &op, &i1, &i2);									// 編集立体ﾎﾟｲﾝﾀ取得
		while(op!=NULL) {
			RedoCommand(op);											// ｺﾏﾝﾄﾞ再実行
			SetDispList(op);											// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
			NextObjPtr(2, &op, &i1, &i2);								// 次編集立体へ
		}
		EndWaitCursor();												// ｳｪｲﾄ･ｶｰｿﾙ消去
	}
}
