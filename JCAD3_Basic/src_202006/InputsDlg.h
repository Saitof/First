/////////////////////////////////////////////////////////////////////////////
// �������ڐݒ�_�C�A���O�N���X�錾
// InputsDlg.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

///////////////////////////////////////////////////////////////////////////////
// CPrtParamDlg �_�C�A���O

#ifndef ___PRTPARAMDLG_H
#define ___PRTPARAMDLG_H

class CPrtParamDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CPrtParamDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PRTPARAM };
	int     m_PMode;
	int     m_PColor;
	int     m_POut;
	int     m_POri;
	CStatic m_Static1;
	CButton m_RButton1;
	CButton m_RButton2;
	int     m_PPos;
	CStatic m_Static2;
	CButton m_RButton3;
	CButton m_RButton4;
	CButton m_CCheck1;
	BOOL    m_Check1;
	CButton m_CCheck2;
	BOOL    m_Check2;
	CStatic m_Static3;
	CEdit   m_Edit1;
	int     m_PSize;
	CSpinButtonCtrl m_Spin;
	CStatic m_Static4;
	CButton m_Button1;

// �I�y���[�V����
	void SetValue(int p1, int p2, int p3, int p4, int p5, int p6, CString p7);
	void GetValue(int* p1, int* p2, int* p3, int* p4, int* p5, int* p6,
                  CString* p7, int* p8, int* p9, CString* p10);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPrintMode(UINT nID);
	afx_msg BOOL OnPrintOut(UINT nID);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnFileSave(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sPicFile, m_sPicPath;
};

#endif ___PRTPARAMDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPrtParam2Dlg �_�C�A���O

#ifndef ___PRTPARAM2DLG_H
#define ___PRTPARAM2DLG_H

class CPrtParam2Dlg : public CDialog
{
// �R���X�g���N�V����
public:
	CPrtParam2Dlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PRTPARAM2 };
	int     m_POut;
	int     m_POri;
	CStatic m_Static1;
	CButton m_RButton1;
	CButton m_RButton2;
	int     m_PPos;
	CStatic m_Static2;
	CButton m_RButton3;
	CButton m_RButton4;
	CButton m_CCheck1;
	BOOL    m_Check1;
	CStatic m_Static3;
	CEdit   m_Edit1;
	int     m_PSize;
	CSpinButtonCtrl m_Spin;
	CStatic m_Static4;
	CButton m_Button1;

// �I�y���[�V����
	void SetValue(int p1, int p2, int p3, int p4, CString p5);
	void GetValue(int* p1, int* p2, int* p3, int* p4,
                  CString* p5, int* p6, CString* p7);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPrintOut(UINT nID);
	afx_msg void OnCheck1(void);
	afx_msg void OnFileSave(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sPicFile, m_sPicPath;
};

#endif ___PRTPARAM2DLG_H

///////////////////////////////////////////////////////////////////////////////
// CCopyInfoDlg �_�C�A���O

#ifndef ___COPYINFODLG_H
#define ___COPYINFODLG_H

class CCopyInfoDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCopyInfoDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_COPY_INFO };
	int    m_CopyNum;
	double m_XScale;
	double m_YScale;
	double m_ZScale;
	int    m_SynFlg;
	int    m_XPoint;
	int    m_YPoint;
	int    m_ZPoint;

// �I�y���[�V����
	void SetValue(int p1, double p2, double p3, double p4,
                  int p5, int p6, int p7, int p8);
	void GetValue(int* p1, double* p2, double* p3, double* p4,
                  int* p5, int* p6, int* p7, int* p8);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___COPYINFODLG_H

///////////////////////////////////////////////////////////////////////////////
// CSpiralDlg �_�C�A���O

#ifndef ___SPIRALDLG_H
#define ___SPIRALDLG_H

class CSpiralDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSpiralDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SPIRAL };
	double m_Diameter;
	double m_Height;
	int    m_CtrlNum;
	int    m_SprlNum;

// �I�y���[�V����
	void SetValue(double p1, double p2, int p3, int p4);
	void GetValue(double* p1, double* p2, int* p3, int* p4);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___SPIRALDLG_H

///////////////////////////////////////////////////////////////////////////////
// CTensionDlg �_�C�A���O

#ifndef ___TENSIONDLG_H
#define ___TENSIONDLG_H

class CTensionDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CTensionDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_TENSION };
	int m_Tension;
	int m_Check1;

// �I�y���[�V����
	void SetValue(int p1);
	void GetValue(int* p1, int* p2);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___TENSIONDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTmpDlg �_�C�A���O

#ifndef ___SETTMPDLG_H
#define ___SETTMPDLG_H

class CSetTmpDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSetTmpDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_TEMPLATE };
	int     m_Knd;
	int     m_Pln;
	CButton m_Button1;
	CButton m_Button2;
	CString m_sTmpName[8][3];
	CEdit   m_Edit[8][3];

// �I�y���[�V����
	void SetValue1(CString p1);
	void SetValue2(int no, CString p1, CString p2, CString p3);
	void GetValue1(CString* p1);
	void GetValue2(int no, CString* p1, CString* p2, CString* p3);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn1(UINT nID);
	afx_msg BOOL OnCheckBtn2(UINT nID);
	afx_msg void OnFileOpen(void);
	afx_msg void OnFileClear(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sTmpPath, m_sTmpFile[8][3];
};

#endif ___SETTMPDLG_H

///////////////////////////////////////////////////////////////////////////////
// CCaptureDlg �_�C�A���O

#ifndef ___CAPTUREDLG_H
#define ___CAPTUREDLG_H

class CCaptureDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCaptureDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_CAPTURE };
	int     m_CPlane;
	int     m_CPos;
	int     m_PKind;
	int     m_PWidth;
	CSpinButtonCtrl m_Spin1;
	int     m_PHeight;
	CSpinButtonCtrl m_Spin2;
	CStatic m_Static1;
	CEdit   m_Edit3;
	int     m_JComp;
	CSpinButtonCtrl m_Spin3;
	CStatic m_Static2;

// �I�y���[�V����
	void SetValue(int p1, int p2, int p3, int p4, int p5, int p6);
	void GetValue(int* p1, int* p2, int* p3, int* p4, int* p5, int* p6);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPictureKind(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___CAPTUREDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTextDlg �_�C�A���O

#ifndef ___SETTEXTDLG_H
#define ___SETTEXTDLG_H

class CSetTextDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSetTextDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_TEXT };
	CComboBox m_Combo1;
	CString m_FontName;
	CString m_Text;
	double  m_Height;
	double  m_Depth;
	double  m_Intval;
	BOOL    m_Check1;

// �I�y���[�V����
	void GetFontInfo(void);
	void SetValue(int p1, int p2, double p3, double p4, double p5);
	void GetValue(int* p1, int* p2, double* p3, double* p4, double* p5);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	DECLARE_MESSAGE_MAP()

private:
	int  m_FontNo, m_FontLeng, m_FontNum;
	char m_FontPath[_MAX_PATH], m_FNameAra[512][128], m_FFileAra[512][128];
};

#endif ___SETTEXTDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTexDlg �_�C�A���O

#ifndef ___SETTEXDLG_H
#define ___SETTEXDLG_H

class CSetTexDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSetTexDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_TEXTURE };
	int     m_Plane;
	CButton	m_Button1;
	CButton	m_Button2;
	CString	m_sTexName[3];
	double  m_Move1, m_Move2;
	int     m_Rotate;
	float   m_Scale;

protected:
	CBitmapButton Left;
	CBitmapButton Right;
	CBitmapButton Down;
	CBitmapButton Up;
	CBitmapButton Left2;
	CBitmapButton Right2;
	CBitmapButton Up2;
	CBitmapButton Down2;

// �I�y���[�V����

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn1(UINT nID);
	afx_msg BOOL OnCheckBtn2(UINT nID);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	afx_msg void OnFileOpen(void);
	afx_msg void OnFileClear(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sTexPath, m_sTexFile[3];
	int     m_nID, m_Mode, m_Flg;
};

#endif ___SETTEXDLG_H

///////////////////////////////////////////////////////////////////////////////
// CProxyDlg �_�C�A���O

#ifndef ___PROXYDLG_H
#define ___PROXYDLG_H

class CProxyDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CProxyDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROXY_DLG };
	BOOL    m_Check1;
	BOOL    m_Check2;
	CButton m_CCheck;
	CStatic m_Static1;
	CEdit   m_Edit1;
	CString m_ProxyAdress;
	CStatic m_Static2;
	CEdit   m_Edit2;
	int     m_ProxyPort;

// �I�y���[�V����
	void GetValue(BOOL* p1, BOOL* p2, CString* p3, int* p4);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PROXYDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg �_�C�A���O

#ifndef ___PASSWDDLG_H
#define ___PASSWDDLG_H

class CPasswdDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CPasswdDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PASSWD_DLG };
	CString m_WPwdData1;
	CString m_WPwdData2;
	BOOL    m_Check1;
	CStatic m_Static1;
	CStatic m_Static2;
	CEdit   m_Edit3;
	CEdit   m_Edit4;
	CString m_RPwdData1;
	CString m_RPwdData2;

// �I�y���[�V����
	void SetValue(CString p1, CString p2, int p3);
	void GetValue(CString* p1, CString* p2, int* p3);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnCheck1(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PASSWDDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg2 �_�C�A���O

#ifndef ___PASSWDDLG2_H
#define ___PASSWDDLG2_H

class CPasswdDlg2 : public CDialog
{
// �R���X�g���N�V����
public:
	CPasswdDlg2(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PASSWD_DLG2 };
	CString m_WPwdData;

// �I�y���[�V����
	void GetValue(CString* p1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual void OnOK(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PASSWDDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CCurveDiviDlg �_�C�A���O

#ifndef ___CURVEDIVIDLG_H
#define ___CURVEDIVIDLG_H

class CCurveDiviDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCurveDiviDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CURVE_DIVI };
	int m_CurveDiv;
	int m_Polygon;
	int m_FileBUp;

// �I�y���[�V����
	void SetValue(int p1, int p2, int p3);
	void GetValue(int* p1, int* p2, int* p3);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___CURVEDIVIDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetProngDlg �_�C�A���O

#ifndef ___SETPRONGDLG_H
#define ___SETPRONGDLG_H

class CSetProngDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSetProngDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_PRONG };
	int    m_ProngN;
	double m_ProngL;
	double m_ProngD;
	double m_ProngR;

// �I�y���[�V����
	void SetValue(int p1, double p2, double p3, double p4);
	void GetValue(int* p1, double* p2, double* p3, double* p4);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()

};

#endif ___SETPRONGDLG_H

///////////////////////////////////////////////////////////////////////////////
// CLimitTrackerDlg �_�C�A���O

#ifndef ___LIMITTRACKERDLG_H
#define ___LIMITTRACKERDLG_H

class CLimitTrackerDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CLimitTrackerDlg(CWnd* pParent=NULL);								// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_LIMIT_TRACKER };
	double m_XBase;
	double m_YBase;
	double m_ZBase;
	BOOL   m_XCheck1;
	BOOL   m_XCheck2;
	BOOL   m_YCheck1;
	BOOL   m_YCheck2;
	BOOL   m_ZCheck1;
	BOOL   m_ZCheck2;

// �I�y���[�V����
	void SetValue(double p1, double p2, double p3,
                  int p4, int p5, int p6, int p7, int p8, int p9);
	void GetValue(double* p1, double* p2, double* p3,
                  int* p4, int* p5, int* p6, int* p7, int* p8, int* p9);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___LIMITTRACKERDLG_H

//=============================<EOF:InputsDlg.h>=============================