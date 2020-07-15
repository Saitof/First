/////////////////////////////////////////////////////////////////////////////
// POV-Ray�f�[�^�o�͊��ݒ�v���p�e�B�V�[�g�N���X��`
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
// CPovEnvDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CPovEnvDlg1, CPropertyPage)							// ү���ޥϯ��
END_MESSAGE_MAP()

/************************************************************/
CPovEnvDlg1::CPovEnvDlg1() : CPropertyPage(CPovEnvDlg1::IDD)
/************************************************************/
{
	m_OutName = _T("");
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CPovEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OutName);
}

/************************************/
BOOL CPovEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CPropertyPage::OnInitDialog();
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CPovEnvDlg2, CPropertyPage)							// ү���ޥϯ��
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
                &m_bAnima, &m_ZAnima, &m_YAnima);						// POV-Ray�ް��擾1
	int i = m_sFile.GetLength();
	int j = m_sFile.ReverseFind('\\');
	if(i>0) {
		m_InName = m_sFile.Right(i-j-1);
	}
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CPovEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
BOOL CPovEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CPropertyPage::OnInitDialog();
	PermissionCheck();													// �����ޯ�����͉�����
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CPovEnvDlg2::OnInitFile(void)										//<<< �����l���݉�������
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
	PermissionCheck();													// �����ޯ�����͉�����
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CPovEnvDlg2::OnSaveFile(void)										//<<< �ۑ����݉�������
/**********************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+PV1_NAME;										// POV-Ray���ݒ�ۑ�̧�ٖ�1
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
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
			::GetShortPathName(m_sFile, spath, _MAX_PATH);				// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sPath, spath, _MAX_PATH);				// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_OutPath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
}

/*************************************/
void CPovEnvDlg2::OnRestoreFile(void)									//<<< �������݉�������
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i, j;
	float   fd1, fd2, fd3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+PV1_NAME;                                      // POV-Ray���ݒ�ۑ�̧�ٖ�1
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
		fclose(fp);														// ̧��CLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
	delete[] lpath;
	PermissionCheck();													// �����ޯ�����͉�����
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/***********************************/
void CPovEnvDlg2::OnInfileBtn(void)										//<<< POV-Ray��`̧�ٓ��͖t
/***********************************/
{
	char*   buff = new char[256];
	CString fname = "*.txt", ext = "txt", filter, strCheck;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	int     len;

	filter.LoadString(IDS_FILE_FILTER107);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sPath;
	if(dlg.DoModal()==IDOK) {											// ̧�ٓ����޲�۸ޕ\��
		strCheck.LoadString(IDS_STR96);									// ���K��1�s���ް�
		len = strCheck.GetLength();
		m_sFile = dlg.GetPathName();									// ̧�ٖ�(���߽)�擾
		std::fstream m_file(m_sFile, std::ios::in);
		m_file.getline(buff, len+1);									// 1�s�ړ���
		if(strCheck.Compare(buff)) {									// <��v���Ȃ��ꍇ>
			AfxMessageBox(IDS_ERR_DEFFILE);								// ���`̧�ٴװ�
			m_sFile = _T("");
		} else {														// <��v����ꍇ>
			int i = m_sFile.GetLength();
			int j = m_sFile.ReverseFind('\\');
			if(i>0) {
				m_InName = m_sFile.Right(i-j-1);
			}
			m_Edit1.SetWindowText(m_InName);							// ��`�ް�̧�ٖ��\��
			int idx = m_sFile.ReverseFind('\\');
			m_sPath = m_sFile.Left(idx+1);								// �߽�ۑ�
		}
	}
	delete[] buff;
}

/************************************/
void CPovEnvDlg2::OnOutpathBtn(void)									//<<< POV-Ray�o��̧�ٓ��͖t
/************************************/
{
	TCHAR*       szDir = new TCHAR[MAX_PATH];
	BROWSEINFO   bi;
	LPITEMIDLIST pidl;
	LPMALLOC     pMalloc;

	if(SUCCEEDED(SHGetMalloc(&pMalloc))) {								// Shell �̕W���̃A���P�[�^���擾
		ZeroMemory(&bi,sizeof(bi));										// BROWSEINFO �\���̂𖄂߂�
		bi.hwndOwner = m_hWnd;											// �I�[�i�[�E�B���h�E�n���h����ݒ�
		bi.pidlRoot = 0;
		bi.pszDisplayName = 0;
		bi.lpszTitle = "POV-Ray �o�͐�t�H���_��I�����Ă�������";
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		pidl = SHBrowseForFolder(&bi);									// �t�H���_�̎Q�ƃ_�C�A���O�{�b�N�X�̕\��
		if(pidl) {
			if(SHGetPathFromIDList(pidl, szDir)) {						// PIDL ���t�@�C���V�X�e���̃p�X�ɕϊ�
				MessageBox(szDir, "�I���t�H���_", MB_OK);
			}
			m_OutPath = szDir;
			m_OutPath = m_OutPath + "\\";
			m_Edit2.SetWindowText(m_OutPath);							// �o��̫��ޖ��\��
			pMalloc->Free(pidl);										// SHBrowseForFolder�� PIDL �����
		}
		pMalloc->Release();												// Shell �̃A���P�[�^���J��
	}
	delete[] szDir;
}

/********************************/
void CPovEnvDlg2::OnCheck1(void)										//<<< Translate�t����
/********************************/
{
	m_bTranslate = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// �tON/OFF��Ԏ擾
	PermissionCheck();													// ���͋�������
}

/********************************/
void CPovEnvDlg2::OnCheck2(void)										//<<< Rotate�t����
/********************************/
{
	m_bRotate = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// �tON/OFF��Ԏ擾
	PermissionCheck();													// ���͋�������
}

/********************************/
void CPovEnvDlg2::OnCheck3(void)										//<<< Scale�t����
/********************************/
{
	m_bScale = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// �tON/OFF��Ԏ擾
	PermissionCheck();													// ���͋�������
}

/********************************/
void CPovEnvDlg2::OnCheck4(void)										//<<< ��Ұ��ݖt����
/********************************/
{
	m_bAnima = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// �tON/OFF��Ԏ擾
	PermissionCheck();													// ���͋�������
}

/********************************/
void CPovEnvDlg2::OnCheck5(void)										//<<< �Ѱ��ݸނȂ��t����
/********************************/
{
	m_bSmooth = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// �tON/OFF��Ԏ擾
	PermissionCheck();													// ���͋�������
}

/***************************************/
void CPovEnvDlg2::PermissionCheck(void)									//<<< ���͋�������
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
// CPovEnvDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CPovEnvDlg3, CPropertyPage)							// ү���ޥϯ��
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
                &m_Collect, &m_Reflection, &m_Refraction);				// POV-Ray�ް��擾2
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
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CPovEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CPovEnvDlg3::OnInitFile(void)										//<<< �����l���݉�������
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
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CPovEnvDlg3::OnSaveFile(void)										//<<< �ۑ����݉�������
/**********************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+PV2_NAME;										// POV-Ray���ݒ�ۑ�̧�ٖ�2
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
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
			::GetShortPathName(m_sDefFile1, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sDefFile2, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_sDefPath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
			if(spath[0]==0x00) {
				::GetShortPathName(GetAppPath(0), spath, _MAX_PATH);
			}
			if(fprintf(fp, "%s\n", spath)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
}

/*************************************/
void CPovEnvDlg3::OnRestoreFile(void)									//<<< �������݉�������
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, i, j;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+PV2_NAME;                                      // POV-Ray���ݒ�ۑ�̧�ٖ�2
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
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
		fclose(fp);														// ̧��CLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
	delete[] lpath;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/************************************/
void CPovEnvDlg3::OnInfileBtn1(void)									//<<< ���[�U��`̧�ٓ��͖t1
/************************************/
{
	CString fname = "*.inc", ext = "inc", filter;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;

	filter.LoadString(IDS_FILE_FILTER108);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sDefPath;
	if(dlg.DoModal()==IDOK) {											// ̧�ٓ����޲�۸ޕ\��
		m_sDefFile1 = dlg.GetPathName();								// ̧�ٖ�(���߽)�擾
		int i = m_sDefFile1.GetLength();
		int j = m_sDefFile1.ReverseFind('\\');
		if(i>0) {
			m_DefName1 = m_sDefFile1.Right(i-j-1);
		}
		m_Edit1.SetWindowText(m_DefName1);								// հ�ޒ�`̧�ٖ��\��
		int idx = m_sDefFile1.ReverseFind('\\');
		m_sDefPath = m_sDefFile1.Left(idx+1);							// �߽�ۑ�
		SetWDefFile1(m_sDefFile1);										// POV-Rayհ�ޒ�`WĶ�ِݒ�1
	}
}

/************************************/
void CPovEnvDlg3::OnInfileBtn2(void)									//<<< ���[�U��`̧�ٓ��͖t2
/************************************/
{
	CString fname = "*.inc", ext = "inc", filter;
	DWORD   flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;

	filter.LoadString(IDS_FILE_FILTER108);
	CFileDialog dlg(TRUE, ext, fname, flg, filter);
	dlg.m_ofn.lpstrInitialDir = m_sDefPath;
	if(dlg.DoModal()==IDOK) {											// ̧�ٓ����޲�۸ޕ\��
		m_sDefFile2 = dlg.GetPathName();								// ̧�ٖ�(���߽)�擾
		int i = m_sDefFile2.GetLength();
		int j = m_sDefFile2.ReverseFind('\\');
		if(i>0) {
			m_DefName2 = m_sDefFile2.Right(i-j-1);
		}
		m_Edit2.SetWindowText(m_DefName2);								// հ�ޒ�`̧�ٖ��\��
		int idx = m_sDefFile2.ReverseFind('\\');
		m_sDefPath = m_sDefFile2.Left(idx+1);							// �߽�ۑ�
		SetWDefFile2(m_sDefFile2);										// POV-Rayհ�ޒ�`WĶ�ِݒ�2
	}
}

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg4 �_�C�A���O

BEGIN_MESSAGE_MAP(CPovEnvDlg4, CPropertyPage)							// ү���ޥϯ��
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
                m_PovTex1, m_PovTex2, m_PovTex3, m_PovTex4, m_PovTex5);	// POV-Ray�ް��擾3
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CPovEnvDlg4::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
BOOL CPovEnvDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CPropertyPage::OnInitDialog();
	ChangeComboList();													// ����ؽĕύX
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CPovEnvDlg4::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
	int i;

	m_No = 0;
	m_PovShape[0]  = 0, m_PovTex1[0]  = 0, m_PovTex2[0]  = 0;			// �ް���
	m_PovShape[1]  = 0, m_PovTex1[1]  = 0, m_PovTex2[1]  = 2;			// ���ް
	m_PovShape[2]  = 1, m_PovTex1[2]  = 1, m_PovTex2[2]  = 0;			// �ؽ��
	m_PovShape[3]  = 1, m_PovTex1[3]  = 1, m_PovTex2[3]  = 2;			// ��ް
	m_PovShape[4]  = 1, m_PovTex1[4]  = 1, m_PovTex2[4]  = 1;			// ������
	m_PovShape[5]  = 1, m_PovTex1[5]  = 1, m_PovTex2[5]  = 3;			// �̧��
	m_PovShape[6]  = 1, m_PovTex1[6]  = 1, m_PovTex2[6]  = 4;			// հ�ް1
	m_PovShape[7]  = 1, m_PovTex1[7]  = 1, m_PovTex2[7]  = 5;			// հ�ް2
	m_PovShape[8]  = 1, m_PovTex1[8]  = 1, m_PovTex2[8]  = 6;			// հ�ް3
	m_PovShape[9]  = 1, m_PovTex1[9]  = 1, m_PovTex2[9]  = 7;			// հ�ް4
	m_PovShape[10] = 1, m_PovTex1[10] = 1, m_PovTex2[10] = 8;			// հ�ް5
	m_PovShape[11] = 1, m_PovTex1[11] = 1, m_PovTex2[11] = 9;			// հ�ް6
	m_PovShape[12] = 1, m_PovTex1[12] = 1, m_PovTex2[12] = 10;			// հ�ް7
	m_PovShape[13] = 1, m_PovTex1[13] = 1, m_PovTex2[13] = 11;			// հ�ް8
	m_PovShape[14] = 1, m_PovTex1[14] = 1, m_PovTex2[14] = 12;			// հ�ް9
	m_PovShape[15] = 1, m_PovTex1[15] = 1, m_PovTex2[15] = 13;			// հ�ް10
	m_PovShape[16] = 1, m_PovTex1[16] = 1, m_PovTex2[16] = 14;			// հ�ް11
	m_PovShape[17] = 1, m_PovTex1[17] = 1, m_PovTex2[17] = 15;			// հ�ް12
	for(i=0; i<POVCMAX; i++) {
		m_PovTex3[i] = 0, m_PovTex4[i] = 0, m_PovTex5[i] = 0;
	}
	ChangeComboList();													// ����ؽĕύX
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CPovEnvDlg4::OnSaveFile(void)										//<<< �ۑ����݉�������
/**********************************/
{
	FILE*   fp;
	CString path;
	int     er=0, i;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+PV3_NAME;										// POV-Ray���ݒ�ۑ�̧�ٖ�3
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
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
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
void CPovEnvDlg4::OnRestoreFile(void)									//<<< �������݉�������
/*************************************/
{
	FILE*   fp;
	CString path;
	int     er=0, i;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+PV3_NAME;                                      // POV-Ray���ݒ�ۑ�̧�ٖ�3
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
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
		fclose(fp);														// ̧��CLOSE
		if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
		}
	} else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	m_No = 0;
	ChangeComboList();													// ����ؽĕύX
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/*************************************/
BOOL CPovEnvDlg4::OnColorNo(UINT nID)									//<<< �װNo���݉�������
/*************************************/
{
	m_No = nID-IDC_RADIO1;												// �Ώ۶װNo
	ChangeComboList();													// ����ؽĕύX
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	return TRUE;
}

/*************************************/
BOOL CPovEnvDlg4::OnShapeNo(UINT nID)									//<<< �`�����݉�������
/*************************************/
{
	m_Shape = nID-IDC_RADIO19;											// �`��No
	m_PovShape[m_No] = m_Shape;											// ��޼ު�Č`��
	return TRUE;
}

/************************************/
void CPovEnvDlg4::OnSelChange1(void)									//<<< ����1�ύX
/************************************/
{
	m_PovTex1[m_No] = m_Combo1.GetCurSel();								// �I����ޯ��
	m_Tex1 = m_Tex1_Flg[m_PovTex1[m_No]];								// �I���敪
	m_PovTex2[m_No] = 0;												// �I����ޯ��������
	m_PovTex3[m_No] = 0;
	m_PovTex4[m_No] = 0;
	m_PovTex5[m_No] = 0;
	if(m_Tex1=='H'||m_Tex1=='I') {										// <��׽2/��׽3>
		SetCombo4();													// �����ޯ���ݒ�4
	} else {															// <�ȊO>
		SetCombo2();													// �����ޯ���ݒ�2
	}
	PermissionCheck();													// �����ޯ�����͉�����
}

/************************************/
void CPovEnvDlg4::OnSelChange2(void)									//<<< ����2�ύX
/************************************/
{
	m_PovTex2[m_No] = m_Combo2.GetCurSel();								// �I����ޯ��
}

/************************************/
void CPovEnvDlg4::OnSelChange3(void)									//<<< ����3�ύX
/************************************/
{
	m_PovTex3[m_No] = m_Combo3.GetCurSel();								// �I����ޯ��
}

/************************************/
void CPovEnvDlg4::OnSelChange4(void)									//<<< ����4�ύX
/************************************/
{
	m_PovTex4[m_No] = m_Combo4.GetCurSel();								// �I����ޯ��
}

/************************************/
void CPovEnvDlg4::OnSelChange5(void)									//<<< ����5�ύX
/************************************/
{
	m_PovTex5[m_No] = m_Combo5.GetCurSel();								// �I����ޯ��
}

/***************************************/
void CPovEnvDlg4::ChangeComboList(void)									//<<< ����ؽĕύX
/***************************************/
{
	m_Shape = m_PovShape[m_No];											// ��޼ު�Č`��
	GetWDefFile(&m_sDefFile1, &m_sDefFile2);							// POV-Rayհ�ޒ�`WĶ�َ擾
	if(m_sDefFile1!="") {
		ReadPovDefFile(m_sDefFile1, &m_DefCnt1, m_DefName1);			// POV-Rayհ�ޒ�`̧�ٓǍ�
		if(m_DefCnt1==0) {
			m_sDefFile1 = _T("");
		}
	}
	if(m_sDefFile2!="") {
		ReadPovDefFile(m_sDefFile2, &m_DefCnt2, m_DefName2);			// POV-Rayհ�ޒ�`̧�ٓǍ�
		if(m_DefCnt2==0) {
			m_sDefFile2 = _T("");
		}
	}
	SetCombo1();														// �����ޯ���ݒ�1
	SetCombo2();														// �����ޯ���ݒ�2
	SetCombo3();														// �����ޯ���ݒ�3
	SetCombo4();														// �����ޯ���ݒ�4
	PermissionCheck();													// �����ޯ�����͉�����
}

/*********************************/
void CPovEnvDlg4::SetCombo1(void)										//<<< �����ޯ���ݒ�1
/*********************************/
{
	CString str;
	int     i, cnt=0, pos;

	m_Texture1 = _T(""), m_Tex1 = ' ';
	m_Combo1.ResetContent();											// �R���{1�폜
	if(m_sDefFile1!="") {												// <հ�ޒ�`1����>
		str.LoadString(IDS_STR94);										// հ�ޒ�`1
		m_Combo1.AddString(str);										// �R���{1�ݒ�
		m_Tex1_Flg[cnt] = '1';											// �敪�ۑ�
		if(m_PovTex1[m_No]==cnt) {
			m_Texture1 = str;
			m_Tex1 = m_Tex1_Flg[cnt];
		}
		cnt++;
	}
	if(m_sDefFile2!="") {												// <հ�ޒ�`2����>
		str.LoadString(IDS_STR95);										// հ�ޒ�`2
		m_Combo1.AddString(str);										// �R���{1�ݒ�
		m_Tex1_Flg[cnt] = '2';											// �敪�ۑ�
		if(m_PovTex1[m_No]==cnt) {
			m_Texture1 = str;
			m_Tex1 = m_Tex1_Flg[cnt];
		}
		cnt++;
	}
	for(i=0; i<MATERIAL_CNT; i++) {										// <�e�N�X�`���ގ�>
		pos = T_Material[i].Find("|");									// ��ؕ���(|)
		m_Combo1.AddString(T_Material[i].Left(pos));					// �R���{1�ݒ�
		m_Tex1_Flg[cnt+i] = T_Material[i].GetAt(pos+1);					// �敪�ۑ�
		if(m_PovTex1[m_No]==cnt+i) {
			m_Texture1 = T_Material[i].Left(pos);
			m_Tex1 = m_Tex1_Flg[cnt+i];
		}
	}
	m_Combo1.SelectString(-1, m_Texture1);
}

/*********************************/
void CPovEnvDlg4::SetCombo2(void)										//<<< �����ޯ���ݒ�2
/*********************************/
{
	int i, pos;

	m_Texture2 = _T(""), m_Tex2 = ' ';
	m_Combo2.ResetContent();											// �R���{2�폜
	if(m_Tex1=='A') {													// <��>
		for(i=0; i<GOLD_CNT; i++) {
			pos = T_Gold[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Gold[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Gold[i].GetAt(pos+1);						// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Gold[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='B') {											// <��>
		for(i=0; i<SILVER_CNT; i++) {
			pos = T_Silver[i].Find("|");								// ��ؕ���(|)
			m_Combo2.AddString(T_Silver[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Silver[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Silver[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='C') {											// <��>
		for(i=0; i<COPPER_CNT; i++) {
			pos = T_Copper[i].Find("|");								// ��ؕ���(|)
			m_Combo2.AddString(T_Copper[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Copper[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Copper[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='D') {											// <�^�J>
		for(i=0; i<BRASS_CNT; i++) {
			pos = T_Brass[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Brass[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Brass[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Brass[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='E') {											// <�N����>
		for(i=0; i<CHROME_CNT; i++) {
			pos = T_Chrome[i].Find("|");								// ��ؕ���(|)
			m_Combo2.AddString(T_Chrome[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Chrome[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Chrome[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='F') {											// <����>
		for(i=0; i<METAL_CNT; i++) {
			pos = T_Metal[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Metal[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Metal[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Metal[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='G') {											// <�K���X�P>
		for(i=0; i<GLASS1_CNT; i++) {
			pos = T_Glass1[i].Find("|");								// ��ؕ���(|)
			m_Combo2.AddString(T_Glass1[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Glass1[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Glass1[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='J') {											// <��>
		for(i=0; i<WOOD_CNT; i++) {
			pos = T_Wood[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Wood[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Wood[i].GetAt(pos+1);						// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Wood[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='K') {											// <��>
		for(i=0; i<STONE_CNT; i++) {
			pos = T_Stone[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Stone[i].Left(pos));					// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Stone[i].GetAt(pos+1);					// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Stone[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='L') {											// <��>
		for(i=0; i<SKY_CNT; i++) {
			pos = T_Sky[i].Find("|");									// ��ؕ���(|)
			m_Combo2.AddString(T_Sky[i].Left(pos));						// �R���{2�ݒ�
			m_Tex2_Flg[i] = T_Sky[i].GetAt(pos+1);						// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = T_Sky[i].Left(pos);
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else if(m_Tex1=='1') {											// <հ�ޒ�`1>
		for(i=0; i<m_DefCnt1; i++) {
			m_Combo2.AddString(m_DefName1[i]);							// �R���{2�ݒ�
			m_Tex2_Flg[i] = 'D';										// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = m_DefName1[i];
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	} else {															// <հ�ޒ�`2>
		for(i=0; i<m_DefCnt2; i++) {
			m_Combo2.AddString(m_DefName2[i]);							// �R���{2�ݒ�
			m_Tex2_Flg[i] = 'E';										// �敪�ۑ�
			if(m_PovTex2[m_No]==i) {
				m_Texture2 = m_DefName2[i];
				m_Tex2 = m_Tex2_Flg[i];
			}
		}
	}
	m_Combo2.SelectString(-1, m_Texture2);
}

/*********************************/
void CPovEnvDlg4::SetCombo3(void)										//<<< �����ޯ���ݒ�3
/*********************************/
{
	int i;

	m_Texture3 = _T("");
	m_Combo3.ResetContent();											// �R���{3�폜
	for(i=0; i<G_OUT_CNT; i++) {										// <�K���X�\�ʐ���>
		m_Combo3.AddString(G_Out_Prpt[i]);								// �R���{3�ݒ�
		if(m_PovTex3[m_No]==i) {
			m_Texture3 = G_Out_Prpt[i];
		}
	}
	m_Combo3.SelectString(-1, m_Texture3);
}

/*********************************/
void CPovEnvDlg4::SetCombo4(void)										//<<< �����ޯ���ݒ�4
/*********************************/
{
	int i;

	m_Texture4 = _T(""), m_Texture5 = _T("");
	m_Combo4.ResetContent();											// �R���{4�폜
	m_Combo5.ResetContent();											// �R���{5�폜
	if(m_Tex1=='I') {													// <<�K���X�R>>
		for(i=0; i<G_IN2_CNT; i++) {									// <�K���X����(����)����>
			m_Combo4.AddString(G_In_Prpt2[i]);							// �R���{4�ݒ�
			if(m_PovTex4[m_No]==i) {
				m_Texture4 = G_In_Prpt2[i];
			}
		}
		for(i=0; i<G_COL2_CNT; i++) {									// <�K���X�F(����)>
			m_Combo5.AddString(G_Color2[i]);							// �R���{4�ݒ�
			if(m_PovTex5[m_No]==i) {
				m_Texture5 = G_Color2[i];
			}
		}
	} else {															// <<�K���X�Q>>
		for(i=0; i<G_IN1_CNT; i++) {									// <�K���X��������>
			m_Combo4.AddString(G_In_Prpt1[i]);							// �R���{4�ݒ�
			if(m_PovTex4[m_No]==i) {
				m_Texture4 = G_In_Prpt1[i];
			}
		}
		for(i=0; i<G_COL1_CNT; i++) {									// <�K���X�F>
			m_Combo5.AddString(G_Color1[i]);							// �R���{4�ݒ�
			if(m_PovTex5[m_No]==i) {
				m_Texture5 = G_Color1[i];
			}
		}
	}
	m_Combo4.SelectString(-1, m_Texture4);
	m_Combo5.SelectString(-1, m_Texture5);
}

/***************************************/
void CPovEnvDlg4::PermissionCheck(void)									//<<< ���͋�������
/***************************************/
{
	BOOL m_bCheck;

	if(m_Tex1=='H'||m_Tex1=='I') {										// <�K���X2/3�̏ꍇ>
		m_bCheck = TRUE;
	} else {															// <�ȊO�̏ꍇ>
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
// CPovEnvSheet �v���p�e�B�V�[�g

/*******************************************************************************/
CPovEnvSheet::CPovEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: ���̈ʒu�Ɏc��̃R���g���[���p�̃y�[�W��ǉ����Ă��������B
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
	m_psh.dwFlags &= ~PSH_HASHELP;										// �w���v �{�^�����Ȃ����܂�
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
	//����l�� DS_CONTEXT_HELP �X�^�C�����폜���܂��B
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**************************************/
void CPovEnvSheet::SetPovRayData(void)									//<<< POV-Ray�ް��ݒ�
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
                m_PovEnvPage2.m_ZAnima, m_PovEnvPage2.m_YAnima);		// POV-Ray�ް��ݒ�1
	SetPovData2(m_PovEnvPage3.m_sDefFile1, m_PovEnvPage3.m_sDefFile2,
                m_PovEnvPage3.m_sDefPath, m_PovEnvPage3.m_Target,
                m_PovEnvPage3.m_Collect, m_PovEnvPage3.m_Reflection,
                m_PovEnvPage3.m_Refraction);							// POV-Ray�ް��ݒ�2
	SetPovData3(m_PovEnvPage4.m_PovShape, m_PovEnvPage4.m_PovTex1,
                m_PovEnvPage4.m_PovTex2, m_PovEnvPage4.m_PovTex3,
                m_PovEnvPage4.m_PovTex4, m_PovEnvPage4.m_PovTex5);		// POV-Ray�ް��ݒ�3
}

/****************************************/
void CPovEnvSheet::GetValue(CString* p1)								//<<< ���Ұ��擾
/****************************************/
{
	*p1 = m_PovEnvPage1.m_OutName;
}
