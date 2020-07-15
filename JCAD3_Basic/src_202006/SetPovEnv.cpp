/////////////////////////////////////////////////////////////////////////////
// POV-Rayデータ出力環境設定プロパティシートクラス定義
// SetPovEnv.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MainFrm.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "FileDlg.h"
#include "SetPovEnv.h"
#include "DataAccess3.h"
#include "Texture.h"

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CPovEnvDlg1, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/************************************************************/
CPovEnvDlg1::CPovEnvDlg1() : CPropertyPage(CPovEnvDlg1::IDD)
/************************************************************/
{
	m_OutName = _T("");
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CPovEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OutName);
}

/************************************/
BOOL CPovEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CPovEnvDlg2, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_BN_CLICKED(IDC_BUTTON4, OnInfileBtn)
	ON_BN_CLICKED(IDC_BUTTON5, OnOutpathBtn)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
END_MESSAGE_MAP()

/************************************************************/
CPovEnvDlg2::CPovEnvDlg2() : CPropertyPage(CPovEnvDlg2::IDD)
/************************************************************/
{
	GetPovData1(&m_sFile, &m_sPath, &m_OutPath, &m_bSmooth,
                &m_Angle, &m_bTranslate, &m_XTranslate, &m_YTranslate,
                &m_ZTranslate, &m_bRotate, &m_XRotate, &m_YRotate,
                &m_ZRotate, &m_bScale, &m_XScale, &m_YScale, &m_ZScale,
                &m_bAnima, &m_ZAnima, &m_YAnima);						// POV-Rayﾃﾞｰﾀ取得1
	int i = m_sFile.GetLength();
	int j = m_sFile.ReverseFind('\\');
	if(i>0) {
		m_InName = m_sFile.Right(i-j-1);
	}
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CPovEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_STATIC5, m_Static5);
	DDX_Control(pDX, IDC_STATIC6, m_Static6);
	DDX_Control(pDX, IDC_STATIC7, m_Static7);
	DDX_Control(pDX, IDC_STATIC8, m_Static8);
	DDX_Control(pDX, IDC_STATIC9, m_Static9);
	DDX_Control(pDX, IDC_STATIC10, m_Static10);
	DDX_Control(pDX, IDC_STATIC11, m_Static11);
	DDX_Control(pDX, IDC_STATIC12, m_Static12);
	DDX_Control(pDX, IDC_STATIC13, m_Static13);
	DDX_Control(pDX, IDC_STATIC14, m_Static14);
	DDX_Control(pDX, IDC_STATIC15, m_Static15);
	DDX_Control(pDX, IDC_STATIC16, m_Static16);
	DDX_Control(pDX, IDC_STATIC17, m_Static17);
	DDX_Control(pDX, IDC_STATIC18, m_Static18);
	DDX_Control(pDX, IDC_STATIC19, m_Static19);
	DDX_Control(pDX, IDC_STATIC20, m_Static20);
	DDX_Control(pDX, IDC_STATIC21, m_Static21);
	DDX_Control(pDX, IDC_STATIC22, m_Static22);
	DDX_Control(pDX, IDC_STATIC23, m_Static23);
	DDX_Control(pDX, IDC_STATIC24, m_Static24);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_EDIT6, m_Edit6);
	DDX_Control(pDX, IDC_EDIT7, m_Edit7);
	DDX_Control(pDX, IDC_EDIT8, m_Edit8);
	DDX_Control(pDX, IDC_EDIT9, m_Edit9);
	DDX_Control(pDX, IDC_EDIT10, m_Edit10);
	DDX_Control(pDX, IDC_EDIT11, m_Edit11);
	DDX_Control(pDX, IDC_EDIT12, m_Edit12);
	DDX_Control(pDX, IDC_EDIT13, m_Edit13);
	DDX_Control(pDX, IDC_EDIT14, m_Edit14);
	DDX_Text(pDX, IDC_EDIT1, m_InName);
	DDX_Text(pDX, IDC_EDIT2, m_OutPath);
	DDX_Check(pDX, IDC_CHECK1, m_bTranslate);
	DDX_Text(pDX, IDC_EDIT3, m_XTranslate);
	DDV_MinMaxDouble(pDX, m_XTranslate, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_YTranslate);
	DDV_MinMaxDouble(pDX, m_YTranslate, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT5, m_ZTranslate);
	DDV_MinMaxDouble(pDX, m_ZTranslate, -1000.0, 1000.0);
	DDX_Check(pDX, IDC_CHECK2, m_bRotate);
	DDX_Text(pDX, IDC_EDIT6, m_XRotate);
	DDV_MinMaxDouble(pDX, m_XRotate, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT7, m_YRotate);
	DDV_MinMaxDouble(pDX, m_YRotate, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT8, m_ZRotate);
	DDV_MinMaxDouble(pDX, m_ZRotate, -360.0, 360.0);
	DDX_Check(pDX, IDC_CHECK3, m_bScale);
	DDX_Text(pDX, IDC_EDIT9, m_XScale);
	DDV_MinMaxDouble(pDX, m_XScale, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT10, m_YScale);
	DDV_MinMaxDouble(pDX, m_YScale, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT11, m_ZScale);
	DDV_MinMaxDouble(pDX, m_ZScale, -360.0, 360.0);
	DDX_Check(pDX, IDC_CHECK4, m_bAnima);
	DDX_Text(pDX, IDC_EDIT12, m_ZAnima);
	DDV_MinMaxInt(pDX, m_ZAnima, 0, 72);
	DDX_Text(pDX, IDC_EDIT13, m_YAnima);
	DDV_MinMaxInt(pDX, m_YAnima, 0, 72);
	DDX_Check(pDX, IDC_CHECK5, m_bSmooth);
	DDX_Text(pDX, IDC_EDIT14, m_Angle);
	DDV_MinMaxDouble(pDX, m_Angle, 0.0, 180.0);
}

/************************************/
BOOL CPovEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
	PermissionCheck();													// 入力ﾎﾞｯｸｽ入力可否ﾁｪｯｸ
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CPovEnvDlg2::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_sFile      = GetAppPath(0)+"Setting_01.txt";
	m_sPath      = GetAppPath(0);
	m_OutPath    = GetAppPath(0);
	m_bSmooth    = 0,   m_Angle      = 45.0;
	m_bTranslate = 0,   m_bRotate    = 0,   m_bScale     = 0;
	m_XTranslate = 0.0, m_YTranslate = 0.0, m_ZTranslate = 0.0;
	m_XRotate    = 0.0, m_YRotate    = 0.0, m_ZRotate    = 0.0;
	m_XScale     = 0.0, m_YScale     = 0.0, m_ZScale     = 0.0;
	m_bAnima     = 0,   m_ZAnima     = 15,  m_YAnima     = 6;
	int i = m_sFile.GetLength();
	int j = m_sFile.ReverseFind('\\');
	if(i>0) {
		m_InName = m_sFile.Right(i-j-1);
	}
	PermissionCheck();													// 入力ﾎﾞｯｸｽ入力可否ﾁｪｯｸ
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/**********************************/
void CPovEnvDlg2::OnSaveFile(void)										//<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	path = GetAppPath(0)+PV1_NAME;										// POV-Ray環境設定保存ﾌｧｲﾙ名1
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			fd1 = (float)m_Angle;
			if(fprintf(fp, "%d %f\n", m_bSmooth, fd1)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_bTranslate,
                                         m_bRotate,
                                         m_bScale)<0) {
				er = 1; break;
			}
			fd1 = (float)m_XTranslate;
			fd2 = (float)m_YTranslate;
			fd3 = (float)m_ZTranslate;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_XRotate;
			fd2 = (float)m_YRotate;
			fd3 = (float)m_ZRotate;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			fd1 = (float)m_XScale;
			fd2 = (float)m_YScale;
			fd3 = (float)m_ZScale;
			if(fprintf(fp, "%f %f %f\n", fd1, fd2, fd3)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", m_bAnima,
                                         m_ZAnima,
                                         m_YAnima)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sFile, spath, _MAX_PATH);				// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sPath, spath, _MAX_PATH);				// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_OutPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
}

/*************************************/
void CPovEnvDlg2::OnRestoreFile(void)									//<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i, j;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+PV1_NAME;                                      // POV-Ray環境設定保存ﾌｧｲﾙ名1
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%f", &m_bSmooth, &fd1)!=2) {
				er = 1; break;
			}
			m_Angle = ChangeFltToDbl(fd1);
			if(fscanf_s(fp, "%d%d%d", &m_bTranslate,
                                      &m_bRotate,
                                      &m_bScale)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_XTranslate = ChangeFltToDbl(fd1);
			m_YTranslate = ChangeFltToDbl(fd2);
			m_ZTranslate = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_XRotate = ChangeFltToDbl(fd1);
			m_YRotate = ChangeFltToDbl(fd2);
			m_ZRotate = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%f%f%f", &fd1, &fd2, &fd3)!=3) {
				er = 1; break;
			}
			m_XScale = ChangeFltToDbl(fd1);
			m_YScale = ChangeFltToDbl(fd2);
			m_ZScale = ChangeFltToDbl(fd3);
			if(fscanf_s(fp, "%d%d%d", &m_bAnima,
                                      &m_ZAnima,
                                      &m_YAnima)!=3) {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sFile.Format("%s", lpath);
				if(m_sFile=="") {
					m_sFile = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sPath.Format("%s", lpath);
				if(m_sPath=="") {
					m_sPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_OutPath.Format("%s", lpath);
				if(m_OutPath=="") {
					m_OutPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			i = m_sFile.GetLength();
			j = m_sFile.ReverseFind('\\');
			if(i>0) {
				m_InName = m_sFile.Right(i-j-1);
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
	delete[] lpath;
	PermissionCheck();													// 入力ﾎﾞｯｸｽ入力可否ﾁｪｯｸ
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/***********************************/
void CPovEnvDlg2::OnInfileBtn(void)										//<<< POV-Ray定義ﾌｧｲﾙ入力釦
/***********************************/
{
	char*   buff = new char[256];
	CString fname = "*.txt", ext = "txt", filter, strCheck;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	int     len;

	filter.LoadString(IDS_FILE_FILTER107);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sPath;
	if(dlg.DoModal()==IDOK) {											// ﾌｧｲﾙ入力ﾀﾞｲｱﾛｸﾞ表示
		strCheck.LoadString(IDS_STR96);									// 正規の1行目ﾃﾞｰﾀ
		len = strCheck.GetLength();
		m_sFile = dlg.GetPathName();									// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		std::fstream m_file(m_sFile, std::ios::in);
		m_file.getline(buff, len+1);									// 1行目入力
		if(strCheck.Compare(buff)) {									// <一致しない場合>
			AfxMessageBox(IDS_ERR_DEFFILE);								// ｢定義ﾌｧｲﾙｴﾗｰ｣
			m_sFile = _T("");
		} else {														// <一致する場合>
			int i = m_sFile.GetLength();
			int j = m_sFile.ReverseFind('\\');
			if(i>0) {
				m_InName = m_sFile.Right(i-j-1);
			}
			m_Edit1.SetWindowText(m_InName);							// 定義ﾃﾞｰﾀﾌｧｲﾙ名表示
			int idx = m_sFile.ReverseFind('\\');
			m_sPath = m_sFile.Left(idx+1);								// ﾊﾟｽ保存
		}
	}
	delete[] buff;
}

/************************************/
void CPovEnvDlg2::OnOutpathBtn(void)									//<<< POV-Ray出力ﾌｧｲﾙ入力釦
/************************************/
{
	TCHAR*       szDir = new TCHAR[MAX_PATH];
	BROWSEINFO   bi;
	LPITEMIDLIST pidl;
	LPMALLOC     pMalloc;

	if(SUCCEEDED(SHGetMalloc(&pMalloc))) {								// Shell の標準のアロケータを取得
		ZeroMemory(&bi,sizeof(bi));										// BROWSEINFO 構造体を埋める
		bi.hwndOwner = m_hWnd;											// オーナーウィンドウハンドルを設定
		bi.pidlRoot = 0;
		bi.pszDisplayName = 0;
		bi.lpszTitle = "POV-Ray 出力先フォルダを選択してください";
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		pidl = SHBrowseForFolder(&bi);									// フォルダの参照ダイアログボックスの表示
		if(pidl) {
			if(SHGetPathFromIDList(pidl, szDir)) {						// PIDL をファイルシステムのパスに変換
				MessageBox(szDir, "選択フォルダ", MB_OK);
			}
			m_OutPath = szDir;
			m_OutPath = m_OutPath + "\\";
			m_Edit2.SetWindowText(m_OutPath);							// 出力ﾌｫﾙﾀﾞ名表示
			pMalloc->Free(pidl);										// SHBrowseForFolderの PIDL を解放
		}
		pMalloc->Release();												// Shell のアロケータを開放
	}
	delete[] szDir;
}

/********************************/
void CPovEnvDlg2::OnCheck1(void)										//<<< Translate釦ﾁｪｯｸ
/********************************/
{
	m_bTranslate = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// 釦ON/OFF状態取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/********************************/
void CPovEnvDlg2::OnCheck2(void)										//<<< Rotate釦ﾁｪｯｸ
/********************************/
{
	m_bRotate = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦ON/OFF状態取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/********************************/
void CPovEnvDlg2::OnCheck3(void)										//<<< Scale釦ﾁｪｯｸ
/********************************/
{
	m_bScale = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// 釦ON/OFF状態取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/********************************/
void CPovEnvDlg2::OnCheck4(void)										//<<< ｱﾆﾒｰｼｮﾝ釦ﾁｪｯｸ
/********************************/
{
	m_bAnima = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// 釦ON/OFF状態取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/********************************/
void CPovEnvDlg2::OnCheck5(void)										//<<< ｽﾑｰｼﾞﾝｸﾞなし釦ﾁｪｯｸ
/********************************/
{
	m_bSmooth = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// 釦ON/OFF状態取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/***************************************/
void CPovEnvDlg2::PermissionCheck(void)									//<<< 入力許可ﾁｪｯｸ
/***************************************/
{
	m_Edit3.EnableWindow(m_bTranslate);
	m_Edit4.EnableWindow(m_bTranslate);
	m_Edit5.EnableWindow(m_bTranslate);
	m_Static1.EnableWindow(m_bTranslate);
	m_Static2.EnableWindow(m_bTranslate);
	m_Static3.EnableWindow(m_bTranslate);
	m_Static4.EnableWindow(m_bTranslate);
	m_Static5.EnableWindow(m_bTranslate);
	m_Static6.EnableWindow(m_bTranslate);
	m_Edit6.EnableWindow(m_bRotate);
	m_Edit7.EnableWindow(m_bRotate);
	m_Edit8.EnableWindow(m_bRotate);
	m_Static7.EnableWindow(m_bRotate);
	m_Static8.EnableWindow(m_bRotate);
	m_Static9.EnableWindow(m_bRotate);
	m_Static10.EnableWindow(m_bRotate);
	m_Static11.EnableWindow(m_bRotate);
	m_Static12.EnableWindow(m_bRotate);
	m_Edit9.EnableWindow(m_bScale);
	m_Edit10.EnableWindow(m_bScale);
	m_Edit11.EnableWindow(m_bScale);
	m_Static13.EnableWindow(m_bScale);
	m_Static14.EnableWindow(m_bScale);
	m_Static15.EnableWindow(m_bScale);
	m_Static16.EnableWindow(m_bScale);
	m_Static17.EnableWindow(m_bScale);
	m_Static18.EnableWindow(m_bScale);
	m_Edit12.EnableWindow(m_bAnima);
	m_Edit13.EnableWindow(m_bAnima);
	m_Static19.EnableWindow(m_bAnima);
	m_Static20.EnableWindow(m_bAnima);
	m_Static21.EnableWindow(m_bAnima);
	m_Static22.EnableWindow(m_bAnima);
	m_Edit14.EnableWindow(m_bSmooth);
	m_Static23.EnableWindow(m_bSmooth);
	m_Static24.EnableWindow(m_bSmooth);
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg3 ダイアログ

BEGIN_MESSAGE_MAP(CPovEnvDlg3, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_BN_CLICKED(IDC_BUTTON4, OnInfileBtn1)
	ON_BN_CLICKED(IDC_BUTTON5, OnInfileBtn2)
END_MESSAGE_MAP()

/************************************************************/
CPovEnvDlg3::CPovEnvDlg3() : CPropertyPage(CPovEnvDlg3::IDD)
/************************************************************/
{
	GetPovData2(&m_sDefFile1, &m_sDefFile2, &m_sDefPath, &m_Target,
                &m_Collect, &m_Reflection, &m_Refraction);				// POV-Rayﾃﾞｰﾀ取得2
	int i = m_sDefFile1.GetLength();
	int j = m_sDefFile1.ReverseFind('\\');
	if(i>0) {
		m_DefName1 = m_sDefFile1.Right(i-j-1);
	}
	i = m_sDefFile2.GetLength();
	j = m_sDefFile2.ReverseFind('\\');
	if(i>0) {
		m_DefName2 = m_sDefFile2.Right(i-j-1);
	}
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CPovEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Text(pDX, IDC_EDIT1, m_DefName1);
	DDX_Text(pDX, IDC_EDIT2, m_DefName2);
	DDX_Radio(pDX, IDC_RADIO1, m_Target);
	DDX_Radio(pDX, IDC_RADIO3, m_Collect);
	DDX_Radio(pDX, IDC_RADIO5, m_Reflection);
	DDX_Radio(pDX, IDC_RADIO7, m_Refraction);
}

/**********************************/
void CPovEnvDlg3::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_sDefFile1  = GetAppPath(0)+"user_01.inc";
	m_sDefFile2  = GetAppPath(0)+"user_02.inc";
	m_sDefPath   = GetAppPath(0);
	m_Target     = 1, m_Collect    = 0;
	m_Reflection = 1, m_Refraction = 1;
	int i = m_sDefFile1.GetLength();
	int j = m_sDefFile1.ReverseFind('\\');
	if(i>0) {
		m_DefName1 = m_sDefFile1.Right(i-j-1);
	}
	i = m_sDefFile2.GetLength();
	j = m_sDefFile2.ReverseFind('\\');
	if(i>0) {
		m_DefName2 = m_sDefFile2.Right(i-j-1);
	}
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/**********************************/
void CPovEnvDlg3::OnSaveFile(void)										//<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	path = GetAppPath(0)+PV2_NAME;										// POV-Ray環境設定保存ﾌｧｲﾙ名2
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fprintf(fp, "%d %d\n", m_Target,
                                      m_Collect)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", m_Reflection,
                                      m_Refraction)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sDefFile1, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sDefFile2, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sDefPath, spath, _MAX_PATH);			// ｼｮｰﾄﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
}

/*************************************/
void CPovEnvDlg3::OnRestoreFile(void)									//<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i, j;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+PV2_NAME;                                      // POV-Ray環境設定保存ﾌｧｲﾙ名2
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d", &m_Target,
                                    &m_Collect)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &m_Reflection,
                                    &m_Refraction)!=2) {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sDefFile1.Format("%s", lpath);
				if(m_sDefFile1=="") {
					m_sDefFile1 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sDefFile2.Format("%s", lpath);
				if(m_sDefFile2=="") {
					m_sDefFile2 = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// ﾛﾝｸﾞﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
				m_sDefPath.Format("%s", lpath);
				if(m_sDefPath=="") {
					m_sDefPath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			i = m_sDefFile1.GetLength();
			j = m_sDefFile1.ReverseFind('\\');
			if(i>0) {
				m_DefName1 = m_sDefFile1.Right(i-j-1);
			}
			i = m_sDefFile2.GetLength();
			j = m_sDefFile2.ReverseFind('\\');
			if(i>0) {
				m_DefName2 = m_sDefFile2.Right(i-j-1);
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	delete[] spath;
	delete[] lpath;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/************************************/
void CPovEnvDlg3::OnInfileBtn1(void)									//<<< ユーザ定義ﾌｧｲﾙ入力釦1
/************************************/
{
	CString fname = "*.inc", ext = "inc", filter;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;

	filter.LoadString(IDS_FILE_FILTER108);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sDefPath;
	if(dlg.DoModal()==IDOK) {											// ﾌｧｲﾙ入力ﾀﾞｲｱﾛｸﾞ表示
		m_sDefFile1 = dlg.GetPathName();								// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		int i = m_sDefFile1.GetLength();
		int j = m_sDefFile1.ReverseFind('\\');
		if(i>0) {
			m_DefName1 = m_sDefFile1.Right(i-j-1);
		}
		m_Edit1.SetWindowText(m_DefName1);								// ﾕｰｻﾞ定義ﾌｧｲﾙ名表示
		int idx = m_sDefFile1.ReverseFind('\\');
		m_sDefPath = m_sDefFile1.Left(idx+1);							// ﾊﾟｽ保存
		SetWDefFile1(m_sDefFile1);										// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定1
	}
}

/************************************/
void CPovEnvDlg3::OnInfileBtn2(void)									//<<< ユーザ定義ﾌｧｲﾙ入力釦2
/************************************/
{
	CString fname = "*.inc", ext = "inc", filter;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;

	filter.LoadString(IDS_FILE_FILTER108);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sDefPath;
	if(dlg.DoModal()==IDOK) {											// ﾌｧｲﾙ入力ﾀﾞｲｱﾛｸﾞ表示
		m_sDefFile2 = dlg.GetPathName();								// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		int i = m_sDefFile2.GetLength();
		int j = m_sDefFile2.ReverseFind('\\');
		if(i>0) {
			m_DefName2 = m_sDefFile2.Right(i-j-1);
		}
		m_Edit2.SetWindowText(m_DefName2);								// ﾕｰｻﾞ定義ﾌｧｲﾙ名表示
		int idx = m_sDefFile2.ReverseFind('\\');
		m_sDefPath = m_sDefFile2.Left(idx+1);							// ﾊﾟｽ保存
		SetWDefFile2(m_sDefFile2);										// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ設定2
	}
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg4 ダイアログ

BEGIN_MESSAGE_MAP(CPovEnvDlg4, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO18, OnColorNo)
	ON_COMMAND_EX_RANGE(IDC_RADIO19, IDC_RADIO20, OnShapeNo)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelChange1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelChange2)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelChange3)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelChange4)
	ON_CBN_SELCHANGE(IDC_COMBO5, OnSelChange5)
END_MESSAGE_MAP()

/************************************************************/
CPovEnvDlg4::CPovEnvDlg4() : CPropertyPage(CPovEnvDlg4::IDD)
/************************************************************/
{
	m_No = 0, m_Shape = 0;
	m_sDefFile1 = _T(""), m_sDefFile2 = _T("");
	m_Texture1 = _T(""), m_Texture2 = _T(""), m_Texture3 = _T("");
	m_Texture4 = _T(""), m_Texture5 = _T("");
	m_DefCnt1 = 0, m_DefCnt2 = 0;
	GetPovData3(m_PovShape,
                m_PovTex1, m_PovTex2, m_PovTex3, m_PovTex4, m_PovTex5);	// POV-Rayﾃﾞｰﾀ取得3
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CPovEnvDlg4::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_No);
	DDX_Radio(pDX, IDC_RADIO19, m_Shape);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_CBString(pDX, IDC_COMBO1, m_Texture1);
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_CBString(pDX, IDC_COMBO2, m_Texture2);
	DDX_Control(pDX, IDC_COMBO3, m_Combo3);
	DDX_CBString(pDX, IDC_COMBO3, m_Texture3);
	DDX_Control(pDX, IDC_COMBO4, m_Combo4);
	DDX_CBString(pDX, IDC_COMBO4, m_Texture4);
	DDX_Control(pDX, IDC_COMBO5, m_Combo5);
	DDX_CBString(pDX, IDC_COMBO5, m_Texture5);
}

/************************************/
BOOL CPovEnvDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
	ChangeComboList();													// ｺﾝﾎﾞﾘｽﾄ変更
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CPovEnvDlg4::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
	int i;

	m_No = 0;
	m_PovShape[0]  = 0, m_PovTex1[0]  = 0, m_PovTex2[0]  = 0;			// ｺﾞｰﾙﾄﾞ
	m_PovShape[1]  = 0, m_PovTex1[1]  = 0, m_PovTex2[1]  = 2;			// ｼﾙﾊﾞｰ
	m_PovShape[2]  = 1, m_PovTex1[2]  = 1, m_PovTex2[2]  = 0;			// ｸﾘｽﾀﾙ
	m_PovShape[3]  = 1, m_PovTex1[3]  = 1, m_PovTex2[3]  = 2;			// ﾙﾋﾞｰ
	m_PovShape[4]  = 1, m_PovTex1[4]  = 1, m_PovTex2[4]  = 1;			// ｴﾒﾗﾙﾄﾞ
	m_PovShape[5]  = 1, m_PovTex1[5]  = 1, m_PovTex2[5]  = 3;			// ｻﾌｧｲｱ
	m_PovShape[6]  = 1, m_PovTex1[6]  = 1, m_PovTex2[6]  = 4;			// ﾕｰｻﾞｰ1
	m_PovShape[7]  = 1, m_PovTex1[7]  = 1, m_PovTex2[7]  = 5;			// ﾕｰｻﾞｰ2
	m_PovShape[8]  = 1, m_PovTex1[8]  = 1, m_PovTex2[8]  = 6;			// ﾕｰｻﾞｰ3
	m_PovShape[9]  = 1, m_PovTex1[9]  = 1, m_PovTex2[9]  = 7;			// ﾕｰｻﾞｰ4
	m_PovShape[10] = 1, m_PovTex1[10] = 1, m_PovTex2[10] = 8;			// ﾕｰｻﾞｰ5
	m_PovShape[11] = 1, m_PovTex1[11] = 1, m_PovTex2[11] = 9;			// ﾕｰｻﾞｰ6
	m_PovShape[12] = 1, m_PovTex1[12] = 1, m_PovTex2[12] = 10;			// ﾕｰｻﾞｰ7
	m_PovShape[13] = 1, m_PovTex1[13] = 1, m_PovTex2[13] = 11;			// ﾕｰｻﾞｰ8
	m_PovShape[14] = 1, m_PovTex1[14] = 1, m_PovTex2[14] = 12;			// ﾕｰｻﾞｰ9
	m_PovShape[15] = 1, m_PovTex1[15] = 1, m_PovTex2[15] = 13;			// ﾕｰｻﾞｰ10
	m_PovShape[16] = 1, m_PovTex1[16] = 1, m_PovTex2[16] = 14;			// ﾕｰｻﾞｰ11
	m_PovShape[17] = 1, m_PovTex1[17] = 1, m_PovTex2[17] = 15;			// ﾕｰｻﾞｰ12
	for(i=0; i<POVCMAX; i++) {
		m_PovTex3[i] = 0, m_PovTex4[i] = 0, m_PovTex5[i] = 0;
	}
	ChangeComboList();													// ｺﾝﾎﾞﾘｽﾄ変更
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/**********************************/
void CPovEnvDlg4::OnSaveFile(void)										//<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
	FILE*   fp;
	CString path;
	int     er=0, i;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	path = GetAppPath(0)+PV3_NAME;										// POV-Ray環境設定保存ﾌｧｲﾙ名3
	if(fopen_s(&fp, path, "w")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			for(i=0; i<POVCMAX; i++) {
				if(fprintf(fp, "%d %d %d\n", m_PovShape[i],
                                             m_PovTex1[i],
                                             m_PovTex2[i])<0) {
					er = 1; break;
				}
				if(fprintf(fp, "%d %d %d\n", m_PovTex3[i],
                                             m_PovTex4[i],
                                             m_PovTex5[i])<0) {
					er = 1; break;
				}
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存NG｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
void CPovEnvDlg4::OnRestoreFile(void)									//<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
	FILE*   fp;
	CString path;
	int     er=0, i;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+PV3_NAME;                                      // POV-Ray環境設定保存ﾌｧｲﾙ名3
	if(fopen_s(&fp, path, "r")==0) {									// ﾌｧｲﾙOPEN
		while (TRUE) {
			for(i=0; i<POVCMAX; i++) {
				if(fscanf_s(fp, "%d%d%d", &m_PovShape[i],
                                          &m_PovTex1[i],
                                          &m_PovTex2[i])!=3) {
					er = 1; break;
				}
				if(fscanf_s(fp, "%d%d%d", &m_PovTex3[i],
                                          &m_PovTex4[i],
                                          &m_PovTex5[i])!=3) {
					er = 1; break;
				}
			}
			break;
		}
		fclose(fp);														// ﾌｧｲﾙCLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
	m_No = 0;
	ChangeComboList();													// ｺﾝﾎﾞﾘｽﾄ変更
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/*************************************/
BOOL CPovEnvDlg4::OnColorNo(UINT nID)									//<<< ｶﾗｰNoﾎﾞﾀﾝ押下処理
/*************************************/
{
	m_No = nID-IDC_RADIO1;												// 対象ｶﾗｰNo
	ChangeComboList();													// ｺﾝﾎﾞﾘｽﾄ変更
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	return TRUE;
}

/*************************************/
BOOL CPovEnvDlg4::OnShapeNo(UINT nID)									//<<< 形状ﾎﾞﾀﾝ押下処理
/*************************************/
{
	m_Shape = nID-IDC_RADIO19;											// 形状No
	m_PovShape[m_No] = m_Shape;											// ｵﾌﾞｼﾞｪｸﾄ形状
	return TRUE;
}

/************************************/
void CPovEnvDlg4::OnSelChange1(void)									//<<< ｺﾝﾎﾞ1変更
/************************************/
{
	m_PovTex1[m_No] = m_Combo1.GetCurSel();								// 選択ｲﾝﾃﾞｯｸｽ
	m_Tex1 = m_Tex1_Flg[m_PovTex1[m_No]];								// 選択区分
	m_PovTex2[m_No] = 0;												// 選択ｲﾝﾃﾞｯｸｽ初期化
	m_PovTex3[m_No] = 0;
	m_PovTex4[m_No] = 0;
	m_PovTex5[m_No] = 0;
	if(m_Tex1=='H'||m_Tex1=='I') {										// <ｶﾞﾗｽ2/ｶﾞﾗｽ3>
		SetCombo4();													// ｺﾝﾎﾞﾎﾞｯｸｽ設定4
	} else {															// <以外>
		SetCombo2();													// ｺﾝﾎﾞﾎﾞｯｸｽ設定2
	}
	PermissionCheck();													// 入力ﾎﾞｯｸｽ入力可否ﾁｪｯｸ
}

/************************************/
void CPovEnvDlg4::OnSelChange2(void)									//<<< ｺﾝﾎﾞ2変更
/************************************/
{
	m_PovTex2[m_No] = m_Combo2.GetCurSel();								// 選択ｲﾝﾃﾞｯｸｽ
}

/************************************/
void CPovEnvDlg4::OnSelChange3(void)									//<<< ｺﾝﾎﾞ3変更
/************************************/
{
	m_PovTex3[m_No] = m_Combo3.GetCurSel();								// 選択ｲﾝﾃﾞｯｸｽ
}

/************************************/
void CPovEnvDlg4::OnSelChange4(void)									//<<< ｺﾝﾎﾞ4変更
/************************************/
{
	m_PovTex4[m_No] = m_Combo4.GetCurSel();								// 選択ｲﾝﾃﾞｯｸｽ
}

/************************************/
void CPovEnvDlg4::OnSelChange5(void)									//<<< ｺﾝﾎﾞ5変更
/************************************/
{
	m_PovTex5[m_No] = m_Combo5.GetCurSel();								// 選択ｲﾝﾃﾞｯｸｽ
}

/***************************************/
void CPovEnvDlg4::ChangeComboList(void)									//<<< ｺﾝﾎﾞﾘｽﾄ変更
/***************************************/
{
	m_Shape = m_PovShape[m_No];											// ｵﾌﾞｼﾞｪｸﾄ形状
	GetWDefFile(&m_sDefFile1, &m_sDefFile2);							// POV-Rayﾕｰｻﾞ定義WKﾌｧｲﾙ取得
	if(m_sDefFile1!="") {
		ReadPovDefFile(m_sDefFile1, &m_DefCnt1, m_DefName1);			// POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
		if(m_DefCnt1==0) {
			m_sDefFile1 = _T("");
		}
	}
	if(m_sDefFile2!="") {
		ReadPovDefFile(m_sDefFile2, &m_DefCnt2, m_DefName2);			// POV-Rayﾕｰｻﾞ定義ﾌｧｲﾙ読込
		if(m_DefCnt2==0) {
			m_sDefFile2 = _T("");
		}
	}
	SetCombo1();														// ｺﾝﾎﾞﾎﾞｯｸｽ設定1
	SetCombo2();														// ｺﾝﾎﾞﾎﾞｯｸｽ設定2
	SetCombo3();														// ｺﾝﾎﾞﾎﾞｯｸｽ設定3
	SetCombo4();														// ｺﾝﾎﾞﾎﾞｯｸｽ設定4
	PermissionCheck();													// 入力ﾎﾞｯｸｽ入力可否ﾁｪｯｸ
}

/*********************************/
void CPovEnvDlg4::SetCombo1(void)										//<<< ｺﾝﾎﾞﾎﾞｯｸｽ設定1
/*********************************/
{
	CString str;
	int     i, cnt=0, pos;

	m_Texture1 = _T(""), m_Tex1 = ' ';
	m_Combo1.ResetContent();											// コンボ1削除
	if(m_sDefFile1!="") {												// <ﾕｰｻﾞ定義1あり>
		str.LoadString(IDS_STR94);										// ﾕｰｻﾞ定義1
		m_Combo1.AddString(str);										// コンボ1設定
		m_Tex1_Flg[cnt] = '1';											// 区分保存
		if(m_PovTex1[m_No]==cnt) {
			m_Texture1 = str;
			m_Tex1 = m_Tex1_Flg[cnt];
		}
		cnt++;
	}
	if(m_sDefFile2!="") {												// <ﾕｰｻﾞ定義2あり>
		str.LoadString(IDS_STR95);										// ﾕｰｻﾞ定義2
		m_Combo1.AddString(str);										// コンボ1設定
		m_Tex1_Flg[cnt] = '2';											// 区分保存
		if(m_PovTex1[m_No]==cnt) {
			m_Texture1 = str;
			m_Tex1 = m_Tex1_Flg[cnt];
		}
		cnt++;
	}
	for(i=0; i<MATERIAL_CNT; i++) {										// <テクスチャ材質>
		pos = T_Material[i].Find("|");									// 区切文字(|)
		m_Combo1.AddString(T_Material[i].Left(pos));					// コンボ1設定
		m_Tex1_Flg[cnt+i] = T_Material[i].GetAt(pos+1);					// 区分保存
		if(m_PovTex1[m_No]==cnt+i) {
			m_Texture1 = T_Material[i].Left(pos);
			m_Tex1 = m_Tex1_Flg[cnt+i];
		}
	}
	m_Combo1.SelectString(-1, m_Texture1);
}

/*********************************/
void CPovEnvDlg4::SetCombo2(void)										//<<< ｺﾝﾎﾞﾎﾞｯｸｽ設定2
/*********************************/
{
	int i, pos;

	m_Texture2 = _T(""), m_Tex2 = ' ';
	m_Combo2.ResetContent();											// コンボ2削除
	if(m_Tex1=='A') {													// <金>
		for(i=0; i<GOLD_CNT; i++) {
			pos = T_Gold[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Gold[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Gold[i].GetAt(pos+1);						// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Gold[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='B') {											// <銀>
		for(i=0; i<SILVER_CNT; i++) {
			pos = T_Silver[i].Find("|");								// 区切文字(|)
			m_Combo2.AddString(T_Silver[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Silver[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Silver[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='C') {											// <銅>
		for(i=0; i<COPPER_CNT; i++) {
			pos = T_Copper[i].Find("|");								// 区切文字(|)
			m_Combo2.AddString(T_Copper[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Copper[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Copper[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='D') {											// <真鍮>
		for(i=0; i<BRASS_CNT; i++) {
			pos = T_Brass[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Brass[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Brass[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Brass[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='E') {											// <クロム>
		for(i=0; i<CHROME_CNT; i++) {
			pos = T_Chrome[i].Find("|");								// 区切文字(|)
			m_Combo2.AddString(T_Chrome[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Chrome[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Chrome[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='F') {											// <金属>
		for(i=0; i<METAL_CNT; i++) {
			pos = T_Metal[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Metal[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Metal[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Metal[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='G') {											// <ガラス１>
		for(i=0; i<GLASS1_CNT; i++) {
			pos = T_Glass1[i].Find("|");								// 区切文字(|)
			m_Combo2.AddString(T_Glass1[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Glass1[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Glass1[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='J') {											// <木>
		for(i=0; i<WOOD_CNT; i++) {
			pos = T_Wood[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Wood[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Wood[i].GetAt(pos+1);						// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Wood[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='K') {											// <石>
		for(i=0; i<STONE_CNT; i++) {
			pos = T_Stone[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Stone[i].Left(pos));					// コンボ2設定
			m_Tex2_Flg[i] = T_Stone[i].GetAt(pos+1);					// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Stone[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='L') {											// <空>
		for(i=0; i<SKY_CNT; i++) {
			pos = T_Sky[i].Find("|");									// 区切文字(|)
			m_Combo2.AddString(T_Sky[i].Left(pos));						// コンボ2設定
			m_Tex2_Flg[i] = T_Sky[i].GetAt(pos+1);						// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Sky[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='1') {											// <ﾕｰｻﾞ定義1>
		for(i=0; i<m_DefCnt1; i++) {
			m_Combo2.AddString(m_DefName1[i]);							// コンボ2設定
			m_Tex2_Flg[i] = 'D';										// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = m_DefName1[i];
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else {															// <ﾕｰｻﾞ定義2>
		for(i=0; i<m_DefCnt2; i++) {
			m_Combo2.AddString(m_DefName2[i]);							// コンボ2設定
			m_Tex2_Flg[i] = 'E';										// 区分保存
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = m_DefName2[i];
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	}
	m_Combo2.SelectString(-1, m_Texture2);
}

/*********************************/
void CPovEnvDlg4::SetCombo3(void)										//<<< ｺﾝﾎﾞﾎﾞｯｸｽ設定3
/*********************************/
{
	int i;

	m_Texture3 = _T("");
	m_Combo3.ResetContent();											// コンボ3削除
	for(i=0; i<G_OUT_CNT; i++) {										// <ガラス表面性質>
		m_Combo3.AddString(G_Out_Prpt[i]);								// コンボ3設定
		if(m_PovTex3[m_No]==i) {
			m_Texture3 = G_Out_Prpt[i];
		}
	}
	m_Combo3.SelectString(-1, m_Texture3);
}

/*********************************/
void CPovEnvDlg4::SetCombo4(void)										//<<< ｺﾝﾎﾞﾎﾞｯｸｽ設定4
/*********************************/
{
	int i;

	m_Texture4 = _T(""), m_Texture5 = _T("");
	m_Combo4.ResetContent();											// コンボ4削除
	m_Combo5.ResetContent();											// コンボ5削除
	if(m_Tex1=='I') {													// <<ガラス３>>
		for(i=0; i<G_IN2_CNT; i++) {									// <ガラス内部(透明)性質>
			m_Combo4.AddString(G_In_Prpt2[i]);							// コンボ4設定
			if(m_PovTex4[m_No]==i) {
				m_Texture4 = G_In_Prpt2[i];
			}
		}
		for(i=0; i<G_COL2_CNT; i++) {									// <ガラス色(透明)>
			m_Combo5.AddString(G_Color2[i]);							// コンボ4設定
			if(m_PovTex5[m_No]==i) {
				m_Texture5 = G_Color2[i];
			}
		}
	} else {															// <<ガラス２>>
		for(i=0; i<G_IN1_CNT; i++) {									// <ガラス内部性質>
			m_Combo4.AddString(G_In_Prpt1[i]);							// コンボ4設定
			if(m_PovTex4[m_No]==i) {
				m_Texture4 = G_In_Prpt1[i];
			}
		}
		for(i=0; i<G_COL1_CNT; i++) {									// <ガラス色>
			m_Combo5.AddString(G_Color1[i]);							// コンボ4設定
			if(m_PovTex5[m_No]==i) {
				m_Texture5 = G_Color1[i];
			}
		}
	}
	m_Combo4.SelectString(-1, m_Texture4);
	m_Combo5.SelectString(-1, m_Texture5);
}

/***************************************/
void CPovEnvDlg4::PermissionCheck(void)									//<<< 入力許可ﾁｪｯｸ
/***************************************/
{
	BOOL m_bCheck;

	if(m_Tex1=='H'||m_Tex1=='I') {										// <ガラス2/3の場合>
		m_bCheck = TRUE;
	} else {															// <以外の場合>
		m_bCheck = FALSE;
	}
	m_Static1.EnableWindow(!m_bCheck);
	m_Static2.EnableWindow(m_bCheck);
	m_Static3.EnableWindow(m_bCheck);
	m_Static4.EnableWindow(m_bCheck);
	m_Combo2.EnableWindow(!m_bCheck);
	m_Combo3.EnableWindow(m_bCheck);
	m_Combo4.EnableWindow(m_bCheck);
	m_Combo5.EnableWindow(m_bCheck);
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvSheet プロパティシート

/*******************************************************************************/
CPovEnvSheet::CPovEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: この位置に残りのコントロール用のページを追加してください。
}

/**********************************************************************************/
CPovEnvSheet::CPovEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
/**********************************************************************************/
{
	AddControlPages();
}

/*****************************/
CPovEnvSheet::~CPovEnvSheet()
/*****************************/
{
}

/************************************/
void CPovEnvSheet::AddControlPages()
/************************************/
{
	m_psh.dwFlags &= ~PSH_USEHICON;
	m_psh.dwFlags &= ~PSH_HASHELP;										// ヘルプ ボタンをなくします
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags |= PSH_USEPAGELANG;

	AddPage(&m_PovEnvPage1);
	AddPage(&m_PovEnvPage2);
	AddPage(&m_PovEnvPage3);
	AddPage(&m_PovEnvPage4);
}

BEGIN_MESSAGE_MAP(CPovEnvSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPovEnvSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************/
BOOL CPovEnvSheet::OnInitDialog()
/*********************************/
{
	return CPropertySheet::OnInitDialog();
}

/***************************************************************************************/
BOOL CPovEnvSheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
/***************************************************************************************/
{
	//既定値の DS_CONTEXT_HELP スタイルを削除します。
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**************************************/
void CPovEnvSheet::SetPovRayData(void)									//<<< POV-Rayﾃﾞｰﾀ設定
/**************************************/
{
	SetPovData1(m_PovEnvPage2.m_sFile, m_PovEnvPage2.m_sPath,
                m_PovEnvPage2.m_OutPath, m_PovEnvPage2.m_bSmooth,
                m_PovEnvPage2.m_Angle, m_PovEnvPage2.m_bTranslate,
                m_PovEnvPage2.m_XTranslate, m_PovEnvPage2.m_YTranslate,
                m_PovEnvPage2.m_ZTranslate, m_PovEnvPage2.m_bRotate,
                m_PovEnvPage2.m_XRotate, m_PovEnvPage2.m_YRotate,
                m_PovEnvPage2.m_ZRotate, m_PovEnvPage2.m_bScale,
                m_PovEnvPage2.m_XScale, m_PovEnvPage2.m_YScale,
                m_PovEnvPage2.m_ZScale, m_PovEnvPage2.m_bAnima,
                m_PovEnvPage2.m_ZAnima, m_PovEnvPage2.m_YAnima);		// POV-Rayﾃﾞｰﾀ設定1
	SetPovData2(m_PovEnvPage3.m_sDefFile1, m_PovEnvPage3.m_sDefFile2,
                m_PovEnvPage3.m_sDefPath, m_PovEnvPage3.m_Target,
                m_PovEnvPage3.m_Collect, m_PovEnvPage3.m_Reflection,
                m_PovEnvPage3.m_Refraction);							// POV-Rayﾃﾞｰﾀ設定2
	SetPovData3(m_PovEnvPage4.m_PovShape, m_PovEnvPage4.m_PovTex1,
                m_PovEnvPage4.m_PovTex2, m_PovEnvPage4.m_PovTex3,
                m_PovEnvPage4.m_PovTex4, m_PovEnvPage4.m_PovTex5);		// POV-Rayﾃﾞｰﾀ設定3
}

/****************************************/
void CPovEnvSheet::GetValue(CString* p1)								//<<< ﾊﾟﾗﾒｰﾀ取得
/****************************************/
{
	*p1 = m_PovEnvPage1.m_OutName;
}
