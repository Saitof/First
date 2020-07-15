/////////////////////////////////////////////////////////////////////////////
// �V�X�e�����ݒ�v���p�e�B�V�[�g�N���X��`
// SetSysEnv.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/17 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "SetSysEnv.h"
#include "DataAccess1.h"
#include "DataAccess3.h"

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg1, CPropertyPage)							// ү���ޥϯ��
	ON_BN_CLICKED(IDC_BUTTON1, OnInitData)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreData)
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg1::CSysEnvDlg1() : CPropertyPage(CSysEnvDlg1::IDD)
/************************************************************/
{
	m_Key1 = 0;
	m_Key2 = 0;
	m_Key3 = 1;
	m_Key4 = 0;
	m_Key5 = 1;
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Key1);
	DDX_Radio(pDX, IDC_RADIO3, m_Key2);
	DDX_Radio(pDX, IDC_RADIO5, m_Key3);
	DDX_Radio(pDX, IDC_RADIO7, m_Key4);
	DDX_Radio(pDX, IDC_RADIO9, m_Key5);
}

/**********************************/
void CSysEnvDlg1::OnInitData(void)										//<<< �����l���݉�������
/**********************************/
{
	m_Key1 = 0;
	m_Key2 = 0;
	m_Key3 = 1;
	m_Key4 = 0;
	m_Key5 = 1;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CSysEnvDlg1::OnSaveData(void)										//<<< �ۑ����݉�������
/**********************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+SY1_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�1
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		if(fprintf(fp, "%d %d %d %d %d\n", m_Key1, m_Key2, m_Key3,
                                           m_Key4, m_Key5)<0) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*************************************/
void CSysEnvDlg1::OnRestoreData(void)									//<<< �������݉�������
/*************************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+SY1_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�1
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		if(fscanf_s(fp, "%d%d%d%d%d", &m_Key1, &m_Key2, &m_Key3,
                                      &m_Key4, &m_Key5)!=5) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ��ۑ�̧�قȂ��
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg2, CPropertyPage)							// ү���ޥϯ��
	ON_BN_CLICKED(IDC_BUTTON1, OnInitData)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreData)
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg2::CSysEnvDlg2() : CPropertyPage(CSysEnvDlg2::IDD)
/************************************************************/
{
	m_Init1 = 0;
	m_Init2 = 0;
	m_Init3 = 0;
	m_Init4 = 0;
	m_Init5 = 0;
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Init1);
	DDX_Radio(pDX, IDC_RADIO4, m_Init2);
	DDX_Radio(pDX, IDC_RADIO7, m_Init3);
	DDX_Radio(pDX, IDC_RADIO10, m_Init4);
	DDX_Radio(pDX, IDC_RADIO13, m_Init5);
}

/**********************************/
void CSysEnvDlg2::OnInitData(void)										//<<< �����l���݉�������
/**********************************/
{
	m_Init1 = 0;
	m_Init2 = 0;
	m_Init3 = 0;
	m_Init4 = 0;
	m_Init5 = 0;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CSysEnvDlg2::OnSaveData(void)										//<<< �ۑ����݉�������
/**********************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+SY2_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�2
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		if(fprintf(fp, "%d %d %d %d %d\n", m_Init1, m_Init2, m_Init3,
                                           m_Init4, m_Init5)<0) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*************************************/
void CSysEnvDlg2::OnRestoreData(void)									//<<< �������݉�������
/*************************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+SY2_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�2
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		if(fscanf_s(fp, "%d%d%d%d%d", &m_Init1, &m_Init2, &m_Init3,
                                      &m_Init4, &m_Init5)!=5) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ��ۑ�̧�قȂ��
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg3, CPropertyPage)							// ү���ޥϯ��
	ON_BN_CLICKED(IDC_BUTTON1, OnInitData)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreData)
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg3::CSysEnvDlg3() : CPropertyPage(CSysEnvDlg3::IDD)
/************************************************************/
{
	m_Btn1 = 0;
	m_Btn2 = 0;
	m_Btn3 = 0;
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Btn1);
	DDX_Radio(pDX, IDC_RADIO3, m_Btn2);
	DDX_Radio(pDX, IDC_RADIO5, m_Btn3);
}

/**********************************/
void CSysEnvDlg3::OnInitData(void)										//<<< �����l���݉�������
/**********************************/
{
	m_Btn1 = 0;
	m_Btn2 = 0;
	m_Btn3 = 0;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CSysEnvDlg3::OnSaveData(void)										//<<< �ۑ����݉�������
/**********************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+SY3_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�3
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		if(fprintf(fp, "%d %d %d\n", m_Btn1, m_Btn2, m_Btn3)<0) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*************************************/
void CSysEnvDlg3::OnRestoreData(void)									//<<< �������݉�������
/*************************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+SY3_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�3
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		if(fscanf_s(fp, "%d%d%d", &m_Btn1, &m_Btn2, &m_Btn3)!=3) {
			er = 1;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ��ۑ�̧�قȂ��
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg4 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg4, CPropertyPage)							// ү���ޥϯ��
	ON_BN_CLICKED(IDC_BUTTON1, OnInitData)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreData)
	ON_BN_CLICKED(IDC_BUTTON4, OnSavePathBtn)
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg4::CSysEnvDlg4() : CPropertyPage(CSysEnvDlg4::IDD)
/************************************************************/
{
	m_CRepeat = 4;
	m_CWeight = 2.0;
	m_Attr = 1;
	m_Sect = 1;
	m_Shape = 0;
	m_Shade = 0;
	m_SaveMax = 50;
	m_SavePath = _T("");
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg4::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CRepeat);
	DDV_MinMaxInt(pDX, m_CRepeat, 1, 5);
	DDX_Text(pDX, IDC_EDIT2, m_CWeight);
	DDV_MinMaxDouble(pDX, m_CWeight, 1.0, 3.0);
	DDX_Radio(pDX, IDC_RADIO1, m_Attr);
	DDX_Radio(pDX, IDC_RADIO3, m_Sect);
	DDX_Radio(pDX, IDC_RADIO5, m_Shape);
	DDX_Radio(pDX, IDC_RADIO7, m_Shade);
	DDX_Text(pDX, IDC_EDIT3, m_SaveMax);
	DDV_MinMaxInt(pDX, m_SaveMax, 10, 500);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Text(pDX, IDC_EDIT4, m_SavePath);
}

/**********************************/
void CSysEnvDlg4::OnInitData(void)										//<<< �����l���݉�������
/**********************************/
{
	m_CRepeat = 4;
	m_CWeight = 2.0;
	m_Attr = 1;
	m_Sect = 1;
	m_Shape = 0;
	m_Shade = 0;
	m_SaveMax = 50;
	m_SavePath = GetAppPath(0);
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CSysEnvDlg4::OnSaveData(void)										//<<< �ۑ����݉�������
/**********************************/
{
	char*   spath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+SY4_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�4
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		while (TRUE) {
			fd = (float)m_CWeight;
			if(fprintf(fp, "%d %f %d %d %d %d %d\n", m_CRepeat, fd,
                       m_Attr, m_Sect, m_Shape, m_Shade, m_SaveMax)<0) {
				er = 1; break;
			}
			memset(spath, NULL, _MAX_PATH);
			::GetShortPathName(m_SavePath, spath, _MAX_PATH);			// ����̧�ٖ�(���߽)�擾
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
void CSysEnvDlg4::OnRestoreData(void)									//<<< �������݉�������
/*************************************/
{
	char*   spath = new char[_MAX_PATH];
	char*   lpath = new char[_MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0;
	float   fd;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	path = GetAppPath(0)+SY4_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�4
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%f%d%d%d%d%d", &m_CRepeat, &fd, &m_Attr,
                          &m_Sect, &m_Shape, &m_Shade, &m_SaveMax)!=7) {
				er = 1; break;
			}
			m_CWeight = ChangeFltToDbl(fd);

			//202006  for blank in path
			fgets(spath, _MAX_PATH, fp);  //202006 preread CR/LF
			fgets(spath, _MAX_PATH, fp);  //202006
			spath[strlen(spath) - 1] = 0; //202006
			if (strlen(spath)) {  //202006
			//if(fscanf_s(fp, "%s", spath, _MAX_PATH)==1) {  //202006 delete
				memset(lpath, NULL, _MAX_PATH);
				::GetLongPathName(spath, lpath, _MAX_PATH);				// �ݸ�̧�ٖ�(���߽)�擾
				m_SavePath.Format("%s", lpath);
				if(m_SavePath=="") {
					m_SavePath = GetAppPath(0);
				}
			} else {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ��ۑ�̧�قȂ��
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] spath;
	delete[] lpath;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/*************************************/
void CSysEnvDlg4::OnSavePathBtn(void)									//<<< �ޯ����ߐ�̫��ޓ��͖t
/*************************************/
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
		bi.lpszTitle = "�o�b�N�A�b�v��t�H���_��I�����Ă�������";
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		pidl = SHBrowseForFolder(&bi);									// �t�H���_�̎Q�ƃ_�C�A���O�{�b�N�X�̕\��
		if(pidl) {
			if(SHGetPathFromIDList(pidl, szDir)) {						// PIDL ���t�@�C���V�X�e���̃p�X�ɕϊ�
				MessageBox(szDir, "�I���t�H���_", MB_OK);
			}
			m_SavePath = szDir;
			m_SavePath = m_SavePath + "\\";
			m_Edit4.SetWindowText(m_SavePath);							// �ޯ����ߐ�̫��ޖ��\��
			pMalloc->Free(pidl);										// SHBrowseForFolder�� PIDL �����
		}
		pMalloc->Release();												// Shell �̃A���P�[�^���J��
	}
	delete[] szDir;
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg5 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg5, CPropertyPage)							// ү���ޥϯ��
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile1)
	ON_BN_CLICKED(IDC_BUTTON2, OnInitFile2)
	ON_BN_CLICKED(IDC_BUTTON3, OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON4, OnRestoreData)
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO44, OnColorKind)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor)
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg5::CSysEnvDlg5() : CPropertyPage(CSysEnvDlg5::IDD)
/************************************************************/
{
	int i;

	for(i=0; i<44; i++) {
		m_Color[i] = MainWnd->GetColor(i);								// �\���F�擾
	}
	m_Kind = 0;
	m_RCol = GetRValue(m_Color[m_Kind]);								// ���݂̍X�V�Ώە\���F
	m_GCol = GetGValue(m_Color[m_Kind]);
	m_BCol = GetBValue(m_Color[m_Kind]);
	m_Color[44] = m_Color[m_Kind];
	m_Update = FALSE;
	m_Check = 0;
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg5::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Kind);
	DDX_Text(pDX, IDC_EDIT1, m_RCol);
	DDV_MinMaxInt(pDX, m_RCol, 0, 255);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT2, m_GCol);
	DDV_MinMaxInt(pDX, m_GCol, 0, 255);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_EDIT3, m_BCol);
	DDV_MinMaxInt(pDX, m_BCol, 0, 255);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_PIC1, m_Pic1);
	DDX_Control(pDX, IDC_PIC2, m_Pic2);
	DDX_Control(pDX, IDC_PIC3, m_Pic3);
	DDX_Control(pDX, IDC_PIC4, m_Pic4);
	DDX_Control(pDX, IDC_PIC5, m_Pic5);
	DDX_Control(pDX, IDC_PIC6, m_Pic6);
	DDX_Control(pDX, IDC_PIC7, m_Pic7);
	DDX_Control(pDX, IDC_PIC8, m_Pic8);
	DDX_Control(pDX, IDC_PIC9, m_Pic9);
	DDX_Control(pDX, IDC_PIC10, m_Pic10);
	DDX_Control(pDX, IDC_PIC11, m_Pic11);
	DDX_Control(pDX, IDC_PIC12, m_Pic12);
	DDX_Control(pDX, IDC_PIC13, m_Pic13);
	DDX_Control(pDX, IDC_PIC14, m_Pic14);
	DDX_Control(pDX, IDC_PIC15, m_Pic15);
	DDX_Control(pDX, IDC_PIC16, m_Pic16);
	DDX_Control(pDX, IDC_PIC17, m_Pic17);
	DDX_Control(pDX, IDC_PIC18, m_Pic18);
	DDX_Control(pDX, IDC_PIC19, m_Pic19);
	DDX_Control(pDX, IDC_PIC20, m_Pic20);
	DDX_Control(pDX, IDC_PIC21, m_Pic21);
	DDX_Control(pDX, IDC_PIC22, m_Pic22);
	DDX_Control(pDX, IDC_PIC23, m_Pic23);
	DDX_Control(pDX, IDC_PIC24, m_Pic24);
	DDX_Control(pDX, IDC_PIC25, m_Pic25);
	DDX_Control(pDX, IDC_PIC26, m_Pic26);
	DDX_Control(pDX, IDC_PIC27, m_Pic27);
	DDX_Control(pDX, IDC_PIC28, m_Pic28);
	DDX_Control(pDX, IDC_PIC29, m_Pic29);
	DDX_Control(pDX, IDC_PIC30, m_Pic30);
	DDX_Control(pDX, IDC_PIC31, m_Pic31);
	DDX_Control(pDX, IDC_PIC32, m_Pic32);
	DDX_Control(pDX, IDC_PIC33, m_Pic33);
	DDX_Control(pDX, IDC_PIC34, m_Pic34);
	DDX_Control(pDX, IDC_PIC35, m_Pic35);
	DDX_Control(pDX, IDC_PIC36, m_Pic36);
	DDX_Control(pDX, IDC_PIC37, m_Pic37);
	DDX_Control(pDX, IDC_PIC38, m_Pic38);
	DDX_Control(pDX, IDC_PIC39, m_Pic39);
	DDX_Control(pDX, IDC_PIC40, m_Pic40);
	DDX_Control(pDX, IDC_PIC41, m_Pic41);
	DDX_Control(pDX, IDC_PIC42, m_Pic42);
	DDX_Control(pDX, IDC_PIC43, m_Pic43);
	DDX_Control(pDX, IDC_PIC44, m_Pic44);
	DDX_Control(pDX, IDC_PIC45, m_Pic45);
}

/*******************************/
void CSysEnvDlg5::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CSysEnvDlg5::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	int i;

	CPropertyPage::OnInitDialog();
	m_Spin1.SetRange(0, 255);
	m_Spin2.SetRange(0, 255);
	m_Spin3.SetRange(0, 255);
	m_Pic1.GetWindowRect(m_SQ[0]);										// ��`��ذݍ��W�擾
	m_Pic2.GetWindowRect(m_SQ[1]);
	m_Pic3.GetWindowRect(m_SQ[2]);
	m_Pic4.GetWindowRect(m_SQ[3]);
	m_Pic5.GetWindowRect(m_SQ[4]);
	m_Pic6.GetWindowRect(m_SQ[5]);
	m_Pic7.GetWindowRect(m_SQ[6]);
	m_Pic8.GetWindowRect(m_SQ[7]);
	m_Pic9.GetWindowRect(m_SQ[8]);
	m_Pic10.GetWindowRect(m_SQ[9]);
	m_Pic11.GetWindowRect(m_SQ[10]);
	m_Pic12.GetWindowRect(m_SQ[11]);
	m_Pic13.GetWindowRect(m_SQ[12]);
	m_Pic14.GetWindowRect(m_SQ[13]);
	m_Pic15.GetWindowRect(m_SQ[14]);
	m_Pic16.GetWindowRect(m_SQ[15]);
	m_Pic17.GetWindowRect(m_SQ[16]);
	m_Pic18.GetWindowRect(m_SQ[17]);
	m_Pic19.GetWindowRect(m_SQ[18]);
	m_Pic20.GetWindowRect(m_SQ[19]);
	m_Pic21.GetWindowRect(m_SQ[20]);
	m_Pic22.GetWindowRect(m_SQ[21]);
	m_Pic23.GetWindowRect(m_SQ[22]);
	m_Pic24.GetWindowRect(m_SQ[23]);
	m_Pic25.GetWindowRect(m_SQ[24]);
	m_Pic26.GetWindowRect(m_SQ[25]);
	m_Pic27.GetWindowRect(m_SQ[26]);
	m_Pic28.GetWindowRect(m_SQ[27]);
	m_Pic29.GetWindowRect(m_SQ[28]);
	m_Pic30.GetWindowRect(m_SQ[29]);
	m_Pic31.GetWindowRect(m_SQ[30]);
	m_Pic32.GetWindowRect(m_SQ[31]);
	m_Pic33.GetWindowRect(m_SQ[32]);
	m_Pic34.GetWindowRect(m_SQ[33]);
	m_Pic35.GetWindowRect(m_SQ[34]);
	m_Pic36.GetWindowRect(m_SQ[35]);
	m_Pic37.GetWindowRect(m_SQ[36]);
	m_Pic38.GetWindowRect(m_SQ[37]);
	m_Pic39.GetWindowRect(m_SQ[38]);
	m_Pic40.GetWindowRect(m_SQ[39]);
	m_Pic41.GetWindowRect(m_SQ[40]);
	m_Pic42.GetWindowRect(m_SQ[41]);
	m_Pic43.GetWindowRect(m_SQ[42]);
	m_Pic44.GetWindowRect(m_SQ[43]);
	m_Pic45.GetWindowRect(m_SQ[44]);
	for(i=0; i<45; i++) {
		ScreenToClient(m_SQ[i]);										// �ײ��č��W�֕ϊ�
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/***********************************/
void CSysEnvDlg5::OnInitFile1(void)										//<<< �������l���݉�������
/***********************************/
{
	char*   buf = new char[100];
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCO_NAME;										// JCAD3�װ�ݒ�̧�ٖ�(����)
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%s%d%d%d", buf, 100, &r, &g, &b)!=4) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// �ُ��ް��͒��~
			}
			m_Color[i] = RGB(r, g, b);									// �װ�̏�������
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	m_RCol = GetRValue(m_Color[m_Kind]);								// ���݂̍X�V�Ώە\���F
	m_GCol = GetGValue(m_Color[m_Kind]);
	m_BCol = GetBValue(m_Color[m_Kind]);
	m_Color[44] = m_Color[m_Kind];
	m_Update = TRUE;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
	delete[] buf;
}

/***********************************/
void CSysEnvDlg5::OnInitFile2(void)										//<<< �V�����l���݉�������
/***********************************/
{
	char*   buf = new char[100];
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+JCN_NAME;										// JCAD3�װ�ݒ�̧�ٖ�(�V��)
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%s%d%d%d", buf, 100, &r, &g, &b)!=4) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// �ُ��ް��͒��~
			}
			m_Color[i] = RGB(r, g, b);									// �װ�̏�������
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
	}
	m_RCol = GetRValue(m_Color[m_Kind]);								// ���݂̍X�V�Ώە\���F
	m_GCol = GetGValue(m_Color[m_Kind]);
	m_BCol = GetBValue(m_Color[m_Kind]);
	m_Color[44] = m_Color[m_Kind];
	m_Update = TRUE;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
	delete[] buf;
}

/**********************************/
void CSysEnvDlg5::OnSaveData(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	path = GetAppPath(0)+SY5_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�5
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			r = GetRValue(m_Color[i]);
			g = GetGValue(m_Color[i]);
			b = GetBValue(m_Color[i]);
			if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
				er = 1; break;
			}
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*************************************/
void CSysEnvDlg5::OnRestoreData(void)                                   //<<< �������݉�������
/*************************************/
{
	FILE*   fp;
	CString path;
	int     i, r, g, b, er=0;

	path = GetAppPath(0)+SY5_NAME;										// ���ъ��ݒ�ۑ�̧�ٖ�5
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		for(i=0; i<44; i++) {
			if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
				er = 1; break;
			}
			if(r<0||r>255||g<0||g>255||b<0||b>255) {
				er = 1; break;											// �ُ��ް��͒��~
			}
			m_Color[i] = RGB(r, g, b);                                  // �װ�̏�������
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		AfxMessageBox(IDS_NON_BACKUP);									// ��ۑ�̧�قȂ��
	}
	m_RCol = GetRValue(m_Color[m_Kind]);								// ���݂̍X�V�Ώە\���F
	m_GCol = GetGValue(m_Color[m_Kind]);
	m_BCol = GetBValue(m_Color[m_Kind]);
	m_Color[44] = m_Color[m_Kind];
	m_Update = TRUE;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
}

/***************************************/
BOOL CSysEnvDlg5::OnColorKind(UINT nID)									//<<< �\���F������݉�������
/***************************************/
{
	m_Kind = nID-IDC_RADIO1;
	m_RCol = GetRValue(m_Color[m_Kind]);								// ���݂̍X�V�Ώە\���F
	m_GCol = GetGValue(m_Color[m_Kind]);
	m_BCol = GetBValue(m_Color[m_Kind]);
	m_Color[44] = m_Color[m_Kind];
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/**********************************/
void CSysEnvDlg5::OnSetColor(void)										//<<< �װ�ݒ����݉�������
/**********************************/
{
	if(++m_Check>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		m_Color[m_Kind] = RGB(m_RCol, m_GCol, m_BCol);					// �\���F���ݒ�
		m_Color[44] = m_Color[m_Kind];
		m_Update = TRUE;
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CSysEnvDlg5::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
	int i;

	for(i=0; i<45; i++) {
		CBrush brushNew(m_Color[i]);
		CBrush* pBrushOld = dc->SelectObject(&brushNew);
		CPen penNew(0, 1, m_Color[i]);
		CPen* pPenOld = dc->SelectObject(&penNew);
		dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
									m_SQ[i].right, m_SQ[i].bottom);		// �̈�`��
		dc->SelectObject(pBrushOld);
		dc->SelectObject(pPenOld);
		brushNew.DeleteObject();
		penNew.DeleteObject();
	}
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg6 �_�C�A���O

BEGIN_MESSAGE_MAP(CSysEnvDlg6, CPropertyPage)							// ү���ޥϯ��
END_MESSAGE_MAP()

/************************************************************/
CSysEnvDlg6::CSysEnvDlg6() : CPropertyPage(CSysEnvDlg6::IDD)
/************************************************************/
{
	m_Num1 = 0, m_Num2 = 0, m_Num3 = 0, m_Num4 = 0;
	m_Num5 = 0, m_Num6 = 0, m_Num7 = 0, m_Num8 = 0;
	m_Num9 = 0, m_Num10 = 0;
	m_Num11 = MAXINTAN, m_Num12 = MAXPNTAN, m_Num13 = MAXUNDON;
	m_Num14 = MAXCCTLN, m_Num15 = MAXSCTLN, m_Num16 = MAXWCTLN;
	m_Num17 = MAXCPNTN, m_Num18 = MAXSPNTN, m_Num19 = MAXOPNTN;
	m_Num20 = MAXBPNTN;
	m_Num21 = GetIntUseNum();
	m_Num22 = GetPntUseNum();
	m_Num23.Format("%.2f %%", GetIntUse());
	m_Num24.Format("%.2f %%", GetPntUse());
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CSysEnvDlg6::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Num1);
	DDV_MinMaxInt(pDX, m_Num1, 1000, 99999999);
	DDX_Text(pDX, IDC_EDIT2, m_Num2);
	DDV_MinMaxInt(pDX, m_Num2, 1000, 99999999);
	DDX_Text(pDX, IDC_EDIT3, m_Num3);
	DDV_MinMaxInt(pDX, m_Num3, 10, 999999);
	DDX_Text(pDX, IDC_EDIT4, m_Num4);
	DDV_MinMaxInt(pDX, m_Num4, 10, 999);
	DDX_Text(pDX, IDC_EDIT5, m_Num5);
	DDV_MinMaxInt(pDX, m_Num5, 100, 9999);
	DDX_Text(pDX, IDC_EDIT6, m_Num6);
	DDV_MinMaxInt(pDX, m_Num6, 100, 9999);
	DDX_Text(pDX, IDC_EDIT7, m_Num7);
	DDV_MinMaxInt(pDX, m_Num7, 100, 9999);
	DDX_Text(pDX, IDC_EDIT8, m_Num8);
	DDV_MinMaxInt(pDX, m_Num8, 1000, 9999999);
	DDX_Text(pDX, IDC_EDIT9, m_Num9);
	DDV_MinMaxInt(pDX, m_Num9, 1000, 9999999);
	DDX_Text(pDX, IDC_EDIT10, m_Num10);
	DDV_MinMaxInt(pDX, m_Num10, 100, 99999);
	DDX_Text(pDX, IDC_EDIT11, m_Num11);
	DDX_Text(pDX, IDC_EDIT12, m_Num12);
	DDX_Text(pDX, IDC_EDIT13, m_Num13);
	DDX_Text(pDX, IDC_EDIT14, m_Num14);
	DDX_Text(pDX, IDC_EDIT15, m_Num15);
	DDX_Text(pDX, IDC_EDIT16, m_Num16);
	DDX_Text(pDX, IDC_EDIT17, m_Num17);
	DDX_Text(pDX, IDC_EDIT18, m_Num18);
	DDX_Text(pDX, IDC_EDIT19, m_Num19);
	DDX_Text(pDX, IDC_EDIT20, m_Num20);
	DDX_Text(pDX, IDC_EDIT21, m_Num21);
	DDX_Text(pDX, IDC_EDIT22, m_Num22);
	DDX_Text(pDX, IDC_EDIT23, m_Num23);
	DDX_Text(pDX, IDC_EDIT24, m_Num24);
}

///////////////////////////////////////////////////////////////////////////////
// CSysEnvSheet �v���p�e�B�V�[�g

/*******************************************************************************/
CSysEnvSheet::CSysEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: ���̈ʒu�Ɏc��̃R���g���[���p�̃y�[�W��ǉ����Ă��������B
}

/**********************************************************************************/
CSysEnvSheet::CSysEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
/**********************************************************************************/
{
	AddControlPages();
}

/*****************************/
CSysEnvSheet::~CSysEnvSheet()
/*****************************/
{
}

/************************************/
void CSysEnvSheet::AddControlPages()
/************************************/
{
	m_psh.dwFlags &= ~PSH_USEHICON;
	m_psh.dwFlags &= ~PSH_HASHELP;										// �w���v �{�^�����Ȃ����܂�
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags |= PSH_USEPAGELANG;

	AddPage(&m_SysEnvPage1);
	AddPage(&m_SysEnvPage2);
	AddPage(&m_SysEnvPage3);
	AddPage(&m_SysEnvPage4);
	AddPage(&m_SysEnvPage5);
	AddPage(&m_SysEnvPage6);
}

BEGIN_MESSAGE_MAP(CSysEnvSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSysEnvSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************/
BOOL CSysEnvSheet::OnInitDialog()
/*********************************/
{
	return CPropertySheet::OnInitDialog();
}

/***************************************************************************************/
BOOL CSysEnvSheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
/***************************************************************************************/
{
	//����l�� DS_CONTEXT_HELP �X�^�C�����폜���܂��B
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/************************************************************************/
void CSysEnvSheet::SetValue(int p1, int p2, double p3, int p4, int p5,
                            int p6, int p7, int p8, CString p9, int p10,
                            int p11, int p12, int p13, int p14,
                            int p15, int p16, int p17, int p18,
                            int p19, int p20, int p21, int p22)			//<<< ���Ұ��ݒ�
/************************************************************************/
{
	m_SysEnvPage1.m_Key1 = p1&0x1 ? 1 : 0;
	m_SysEnvPage1.m_Key2 = p1&0x2 ? 1 : 0;
	m_SysEnvPage1.m_Key3 = p1&0x4 ? 1 : 0;
	m_SysEnvPage1.m_Key4 = p1&0x8 ? 1 : 0;
	m_SysEnvPage1.m_Key5 = p1&0x10 ? 1 : 0;
	m_SysEnvPage4.m_CRepeat = p2;
	m_SysEnvPage4.m_CWeight = p3;
	m_SysEnvPage4.m_Attr = p4;
	m_SysEnvPage4.m_Sect = p5;
	m_SysEnvPage4.m_Shape = p6;
	m_SysEnvPage4.m_Shade = p7;
	m_SysEnvPage4.m_SaveMax = p8;
	m_SysEnvPage4.m_SavePath = p9;
	if(p11&0x1) {
		m_SysEnvPage2.m_Init1 = 2;
	} else {
		m_SysEnvPage2.m_Init1 = p10&0x1 ? 1 : 0;
	}
	if(p11&0x2) {
		m_SysEnvPage2.m_Init2 = 2;
	} else {
		m_SysEnvPage2.m_Init2 = p10&0x2 ? 1 : 0;
	}
	if(p11&0x4) {
		m_SysEnvPage2.m_Init3 = 2;
	} else {
		m_SysEnvPage2.m_Init3 = p10&0x4 ? 1 : 0;
	}
	if(p11&0x8) {
		m_SysEnvPage2.m_Init4 = 2;
	} else {
		m_SysEnvPage2.m_Init4 = p10&0x8 ? 1 : 0;
	}
	if(p11&0x10) {
		m_SysEnvPage2.m_Init5 = 2;
	} else {
		m_SysEnvPage2.m_Init5 = p10&0x10 ? 1 : 0;
	}
	m_SysEnvPage3.m_Btn1 = p12&0x1 ? 1 : 0;
	m_SysEnvPage3.m_Btn2 = p12&0x2 ? 1 : 0;
	m_SysEnvPage3.m_Btn3 = p12&0x4 ? 1 : 0;
	m_SysEnvPage6.m_Num1 = p13;
	m_SysEnvPage6.m_Num2 = p14;
	m_SysEnvPage6.m_Num3 = p15;
	m_SysEnvPage6.m_Num4 = p16;
	m_SysEnvPage6.m_Num5 = p17;
	m_SysEnvPage6.m_Num6 = p18;
	m_SysEnvPage6.m_Num7 = p19;
	m_SysEnvPage6.m_Num8 = p20;
	m_SysEnvPage6.m_Num9 = p21;
	m_SysEnvPage6.m_Num10 = p22;
}

/*******************************************************************/
void CSysEnvSheet::GetValue(int* p1, int* p2, double* p3, int* p4,
                            int* p5, int* p6, int* p7, int* p8,
                            CString* p9, int* p10, int* p11,
                            int* p12, int* p13, int* p14, int* p15,
                            int* p16, int* p17, int* p18, int* p19,
                            int* p20, int* p21, int* p22)				//<<< ���Ұ��擾
/*******************************************************************/
{
	*p1 = 0, *p10 = 0, *p11 = 0, *p12 = 0;
	if(m_SysEnvPage1.m_Key1) {
		*p1 = (*p1)|0x1;
	}
	if(m_SysEnvPage1.m_Key2) {
		*p1 = (*p1)|0x2;
	}
	if(m_SysEnvPage1.m_Key3) {
		*p1 = (*p1)|0x4;
	}
	if(m_SysEnvPage1.m_Key4) {
		*p1 = (*p1)|0x8;
	}
	if(m_SysEnvPage1.m_Key5) {
		*p1 = (*p1)|0x10;
	}
	*p2 = m_SysEnvPage4.m_CRepeat;
	*p3 = m_SysEnvPage4.m_CWeight;
	*p4 = m_SysEnvPage4.m_Attr;
	*p5 = m_SysEnvPage4.m_Sect;
	*p6 = m_SysEnvPage4.m_Shape;
	*p7 = m_SysEnvPage4.m_Shade;
	*p8 = m_SysEnvPage4.m_SaveMax;
	*p9 = m_SysEnvPage4.m_SavePath;
	if(m_SysEnvPage2.m_Init1==1) {
		*p10 = (*p10)|0x1;
	} else if(m_SysEnvPage2.m_Init1==2) {
		*p11 = (*p11)|0x1;
	}
	if(m_SysEnvPage2.m_Init2==1) {
		*p10 = (*p10)|0x2;
	} else if(m_SysEnvPage2.m_Init2==2) {
		*p11 = (*p11)|0x2;
	}
	if(m_SysEnvPage2.m_Init3==1) {
		*p10 = (*p10)|0x4;
	} else if(m_SysEnvPage2.m_Init3==2) {
		*p11 = (*p11)|0x4;
	}
	if(m_SysEnvPage2.m_Init4==1) {
		*p10 = (*p10)|0x8;
	} else if(m_SysEnvPage2.m_Init4==2) {
		*p11 = (*p11)|0x8;
	}
	if(m_SysEnvPage2.m_Init5==1) {
		*p10 = (*p10)|0x10;
	} else if(m_SysEnvPage2.m_Init5==2) {
		*p11 = (*p11)|0x10;
	}
	if(m_SysEnvPage3.m_Btn1) {
		*p12 = (*p12)|0x1;
	}
	if(m_SysEnvPage3.m_Btn2) {
		*p12 = (*p12)|0x2;
	}
	if(m_SysEnvPage3.m_Btn3) {
		*p12 = (*p12)|0x4;
	}
	*p13 = m_SysEnvPage6.m_Num1;
	*p14 = m_SysEnvPage6.m_Num2;
	*p15 = m_SysEnvPage6.m_Num3;
	*p16 = m_SysEnvPage6.m_Num4;
	*p17 = m_SysEnvPage6.m_Num5;
	*p18 = m_SysEnvPage6.m_Num6;
	*p19 = m_SysEnvPage6.m_Num7;
	*p20 = m_SysEnvPage6.m_Num8;
	*p21 = m_SysEnvPage6.m_Num9;
	*p22 = m_SysEnvPage6.m_Num10;
}

/***********************************/
BOOL CSysEnvSheet::GetCUpdate(void)										//<<< �\���F�X�V��Ԏ擾
/***********************************/
{
	return m_SysEnvPage5.m_Update;
}

/***************************************/
COLORREF CSysEnvSheet::GetColor(int no)									//<<< �X�V�\���F�擾
/***************************************/
{
	return m_SysEnvPage5.m_Color[no];
}
