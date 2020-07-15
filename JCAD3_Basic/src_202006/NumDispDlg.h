/////////////////////////////////////////////////////////////////////////////
// ���l�\���޲�۸޸׽(CNumDispDlg)�錾
// NumDispDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/17 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

class CNumDispDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CNumDispDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_NUMDISP_DLG };
	CString	m_Param1;
	CString	m_Param2;
	CString	m_Param3;
	CString	m_Param4;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void Init(void);
	void SetParam(double pr1, double pr2=0.0, double pr3=0.0, double pr4=0.0);
	void SetNumDispWnd(int dp);
	void SetLayout(int kd, int dp);

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   Width, Height;
};

//============================<EOF:NumDispDlg.h>=============================