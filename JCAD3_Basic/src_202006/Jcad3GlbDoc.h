/////////////////////////////////////////////////////////////////////////////
// Document�N���X(CJcad3GlbDoc)�錾
// Jcad3GlbDoc.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___JCAD3GLBDOC_H
#define ___JCAD3GLBDOC_H

#pragma once

class CJcad3GlbDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CJcad3GlbDoc();														// �׽�̍\�z
	DECLARE_DYNCREATE(CJcad3GlbDoc)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace=TRUE);
	virtual BOOL SaveModified(void);

// ����
public:
	virtual ~CJcad3GlbDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___JCAD3GLBDOC_H

//============================<EOF:Jcad3GlbDoc.h>============================