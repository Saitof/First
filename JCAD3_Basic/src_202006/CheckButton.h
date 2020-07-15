/////////////////////////////////////////////////////////////////////////////
// �v�b�V���{�^���N���X�錾
// CheckButton.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/10 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCheckButton �_�C�A���O

#ifndef ___CHECKBUTTON_H
#define ___CHECKBUTTON_H

class CCheckButton : public CBitmapButton
{
// �R���X�g���N�V����
public:
	CCheckButton(void);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	BOOL LoadBitmap(const CString& strBitmapName);
	BOOL SetCheckBtn(int check);

// �I�[�o�[���C�h

// �C���v�������e�[�V����
public:
	virtual ~CCheckButton(void);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CString m_BitmapName;
	int     m_Check;
};

#endif ___CHECKBUTTON_H

/////////////////////////////////////////////////////////////////////////////
// CCheckBox �_�C�A���O

#ifndef ___CHECKBOX_H
#define ___CHECKBOX_H

class CCheckBox : public CBitmapButton
{
// �R���X�g���N�V����
public:
	CCheckBox(void);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	BOOL LoadBitmap(const CString& strBitmap="");
	BOOL SetCheckBtn(int check);

// �I�[�o�[���C�h

// �C���v�������e�[�V����
public:
	virtual ~CCheckBox(void);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CString m_BitmapName;
	int     m_Check;
};

#endif ___CHECKBOX_H

//=================================<EOF:CheckButton.h>=================================