/////////////////////////////////////////////////////////////////////////////
// �v�b�V���{�^���N���X��`
// CheckButton.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/10 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "CheckButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckButton �_�C�A���O

BEGIN_MESSAGE_MAP(CCheckButton, CBitmapButton)
END_MESSAGE_MAP()

/********************************/
CCheckButton::CCheckButton(void)										// �׽�̍\�z
/********************************/
{
	m_BitmapName = "";
	m_Check = -1;
}

/*********************************/
CCheckButton::~CCheckButton(void)										// �׽�̏���
/*********************************/
{
}

/*******************************************************/
BOOL CCheckButton::LoadBitmap(const CString& strBitmap)					// �ޯ�ϯ�߲Ұ�ސݒ�
/*******************************************************/
{
	BOOL ret;

	m_BitmapName = strBitmap;
	ret = SetCheckBtn(0);												// OFF����ޯ�ϯ�߲Ұ�ޓǍ�
	return ret;
}

/*****************************************/
BOOL CCheckButton::SetCheckBtn(int check)								// �t��ON/OFF��Ԑݒ�
/*****************************************/
{
	BOOL ret;

	if(m_Check!=check) {
		if(check) {														// ON����ޯ�ϯ�߲Ұ�ޓǍ�
			ret = LoadBitmaps(m_BitmapName+"D", m_BitmapName+"D", m_BitmapName+"DF");
		} else {														// OFF����ޯ�ϯ�߲Ұ�ޓǍ�
			ret = LoadBitmaps(m_BitmapName+"U", m_BitmapName+"D", m_BitmapName+"F");
		}
		SizeToContent();												// �t�����ޯ�ϯ�߂ɍ��킹��
		Invalidate();													// �ĕ\���̂���
		m_Check = check;
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CCheckBox �_�C�A���O

BEGIN_MESSAGE_MAP(CCheckBox, CBitmapButton)
END_MESSAGE_MAP()

/**************************/
CCheckBox::CCheckBox(void)												// �׽�̍\�z
/**************************/
{
	m_BitmapName = "CHECK";
	m_Check = -1;
}

/***************************/
CCheckBox::~CCheckBox(void)												// �׽�̏���
/***************************/
{
}

/****************************************************/
BOOL CCheckBox::LoadBitmap(const CString& strBitmap)					// �ޯ�ϯ�߲Ұ�ސݒ�
/****************************************************/
{
	BOOL ret;

	if(strBitmap!="") {
		m_BitmapName = strBitmap;
	}
	ret = SetCheckBtn(0);												// OFF����ޯ�ϯ�߲Ұ�ޓǍ�
	return ret;
}

/**************************************/
BOOL CCheckBox::SetCheckBtn(int check)									// �t��ON/OFF��Ԑݒ�
/**************************************/
{
	BOOL ret;

	if(m_Check!=check) {
		if(check) {														// ON����ޯ�ϯ�߲Ұ�ޓǍ�
			ret = LoadBitmaps(m_BitmapName+"CU", m_BitmapName+"CD", m_BitmapName+"CF");
		} else {														// OFF����ޯ�ϯ�߲Ұ�ޓǍ�
			ret = LoadBitmaps(m_BitmapName+"U", m_BitmapName+"D", m_BitmapName+"F");
		}
		SizeToContent();												// �t�����ޯ�ϯ�߂ɍ��킹��
		Invalidate();													// �ĕ\���̂���
		m_Check = check;
	}
	return ret;
}
