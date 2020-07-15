/////////////////////////////////////////////////////////////////////////////
// Document�N���X(CJcad3GlbDoc)��`
// Jcad3GlbDoc.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbDoc.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "FileDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataFile1.h"
#include "DataFile2.h"
#include "DataFile3.h"
#include "LCheck.h"
#include "ExportStl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc

IMPLEMENT_DYNCREATE(CJcad3GlbDoc, CDocument)

BEGIN_MESSAGE_MAP(CJcad3GlbDoc, CDocument)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc �R���X�g���N�V����/�f�X�g���N�V����

/****************************/
CJcad3GlbDoc::CJcad3GlbDoc()
/****************************/
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

/*****************************/
CJcad3GlbDoc::~CJcad3GlbDoc()
/*****************************/
{
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc �V���A����

/******************************************/
void CJcad3GlbDoc::Serialize(CArchive& ar)
/******************************************/
{
	if(ar.IsStoring()) {
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	} else {
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc �f�f

#ifdef _DEBUG
void CJcad3GlbDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CJcad3GlbDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc �R�}���h

/**************************************************************/
BOOL CJcad3GlbDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)			// ̧�ق�ۑ�
/**************************************************************/		// TRUE->�ۑ�����
{
	CString  prompt, fname, ext;
	OBJTYPE* op;
	int      i1, i2, ret;

	BaseObjPtr(1, &op, &i1, &i2);
	if(!op) {
		return FALSE;													// ���̂�������ΏI��
	}
	if(!CheckUndoRedo()) {												// Undo�׸ފm�F
		AfxMessageBox(IDS_CANT_SAVE);									// ��ȖʕҏW����,�ۑ�NG�
		return FALSE;													// �ۑ����s
	}
	if(LicenseCheck()!=0) {												// ײ�ݽ����
		SetDemoFlg(1);													// �̌���
		return FALSE;
	}
	CString newName = lpszPathName;
	if(newName.IsEmpty()) {
		GetFileTitle(m_strPathName, fname.GetBuffer(_MAX_PATH), _MAX_PATH);
		fname.ReleaseBuffer();
		if(!MainWnd->DoPromptFName(fname, ext, FALSE)) {
			return FALSE;
		}
		newName = fname;
	} else {
		ext = newName.Right(3);
		if(!ext.CompareNoCase("ggd")||
           !ext.CompareNoCase("glf")||
           !ext.CompareNoCase("gld")) {									// �GGD/GLF/GLD�`��,�㏑��NG�
			AfxMessageBox(IDS_ERR_GLD);
			return FALSE;
		}
		prompt.Format(IDS_FILE_REWRITE, m_strPathName);					// �̧�ٍX�VOK�
		if(AfxMessageBox(prompt, MB_OKCANCEL)==IDCANCEL) {
			return FALSE;
		}
	}
	View->Display(0);													// �ĕ`��
	CWaitCursor	wait;													// ���ĥ���ق�\��
	ext = newName.Right(3);
	ret = 2;							//202006 change ret default for write file illegal 
	if (!ext.CompareNoCase("djf")) {									//202006 djf�ǉ�		
		ret = WriteJgdFile(newName, 0);									// djg save special
	}

#ifdef STL_EXPORT //202006 STL export�ł̂ݗL��  WriteCommonFile()
	else if (!ext.CompareNoCase("stl")) {
			ret = WriteCommonFile(1, newName);							// STL�`��̧�ٕۑ�
	}
#endif

	//if (!ext.CompareNoCase("jsd")) {
	//	ret = WriteJsdFile(newName, 1, 0);								// JSD�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("jsf")) {
	//	ret = WriteJsdFile(newName, 0, 0);								// JSF�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("gld")) {
	//	ret = WriteJsdFile(newName, 1, 1);								// GLD�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("glf")) {
	//	ret = WriteJsdFile(newName, 0, 1);								// GLF�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("jgd")) {
	//	ret = WriteJgdFile(newName, 0);									// JGD�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("ggd")) {
	//	ret = WriteJgdFile(newName, 1);									// GGD�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("dxf")) {
	//	ret = WriteCommonFile(0, newName);								// DXF�`��̧�ٕۑ�
	//} else if(!ext.CompareNoCase("stl")) {
	//	ret = WriteCommonFile(1, newName);								// STL�`��̧�ٕۑ�
	//}

	if(ret==1) {														// <�װ�����̏ꍇ>
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�:NG�
		remove(newName);												// �ۑ�̧�ٍ폜
		return FALSE;
	} else if(ret==2) {													// <�`���Ⴂ�̏ꍇ>
		AfxMessageBox(IDS_NOT_SAVE);									// �̧�ٕۑ�:�s�£
		return FALSE;
	} else if(ret<0) {													// <���~�̏ꍇ>
		AfxMessageBox(IDS_STOP_PROCESS);								// ��������~�
		remove(newName);												// �ۑ�̧�ٍ폜
		return FALSE;
	}
	if(bReplace) {
		SetPathName(newName);
	}
	return TRUE;														// �ۑ�����
}

/*************************************/
BOOL CJcad3GlbDoc::SaveModified(void)									// ̧�ق�ۑ����邩�̖⍇��
/*************************************/									// TRUE->���ع����:�I��OK
{
	OBJTYPE* op;
	int      dsp, edt, id;
	CString  prompt, name;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// dspflg:ON ���̖�=>���~
		return TRUE;
	}
	if(!IsModified()) {													// �ύX���͒��~
		return TRUE;
	}
	if(m_strPathName.IsEmpty()) {										// ̧�ٖ���
		name = m_strTitle;												// get name based on caption
		if(name.IsEmpty()) {
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
		}
	} else {
		name = m_strPathName;
	}
	prompt.Format(IDS_FILE_CHANGE, name);								// ��ۑ�OK?�
	id = AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
	if(id==IDYES&&!DoFileSave()) {										// �ۑ����s
		return FALSE;
	}
	if(id==IDCANCEL) {													// �ۑ���ݾ�
		return FALSE;
	}
	return TRUE;														// �ۑ�����/�ۑ���
}
