/////////////////////////////////////////////////////////////////////////////
// ���l���̓_�C�A���O�N���X(CInputDlg)�錾
// InputDlg.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

class CInputDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CInputDlg(int kind=0, CWnd* pParent=NULL);							// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUT_DLG };
	double m_Param1;
	double m_Param2;
	double m_Param3;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	void SetValue(double prm1, double prm2=0, double prm3=0);
	void SetValue(int prm1, double prm2=0, double prm3=0);
	void SetValue(double prm1, int prm2);
	void SetValue(int prm1, int prm2);
	void GetValue(double* prm1, double* prm2, double* prm3);
	void GetValue(double* prm1, double* prm2);
	void GetValue(int* prm1, double* prm2);
	void GetValue(double* prm1, int* prm2);
	void GetValue(int* prm1, int* prm2);
	void GetValue(double* prm1);
	void GetValue(int* prm1);

// �C���v�������e�[�V����
protected:
	void CInputDlg::SetInputDlg(void);
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()

private:
	int m_ParamI1, m_ParamI2, m_Kind, Width, Height;
};

//==============================<EOF:InputDlg.h>=============================