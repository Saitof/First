/////////////////////////////////////////////////////////////////////////////
// �X�e�[�^�X�o�[�N���X(CStatusCtrl)�錾
// StatusCtrl.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
class CStatusCtrl : public CStatusBar
{
public:
	CStatusCtrl(void);													// �׽�̍\�z
	virtual ~CStatusCtrl(void) { ; }									// �׽�̏���

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	void MessageOut(UINT nID);											// ү���ޕ\��
	void MessageOut(void);												// ү���ޕ\��

// �I�[�o�[���C�h

protected:

	DECLARE_MESSAGE_MAP()

private:
	CString m_sMess;
};

//=============================<EOF:StatusCtrl.h>============================
