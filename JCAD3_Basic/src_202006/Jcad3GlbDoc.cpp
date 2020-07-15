/////////////////////////////////////////////////////////////////////////////
// Documentクラス(CJcad3GlbDoc)定義
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
// CJcad3GlbDoc コンストラクション/デストラクション

/****************************/
CJcad3GlbDoc::CJcad3GlbDoc()
/****************************/
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。
}

/*****************************/
CJcad3GlbDoc::~CJcad3GlbDoc()
/*****************************/
{
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc シリアル化

/******************************************/
void CJcad3GlbDoc::Serialize(CArchive& ar)
/******************************************/
{
	if(ar.IsStoring()) {
		// TODO: 格納するコードをここに追加してください。
	} else {
		// TODO: 読み込むコードをここに追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbDoc 診断

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
// CJcad3GlbDoc コマンド

/**************************************************************/
BOOL CJcad3GlbDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)			// ﾌｧｲﾙを保存
/**************************************************************/		// TRUE->保存成功
{
	CString  prompt, fname, ext;
	OBJTYPE* op;
	int      i1, i2, ret;

	BaseObjPtr(1, &op, &i1, &i2);
	if(!op) {
		return FALSE;													// 立体が無ければ終了
	}
	if(!CheckUndoRedo()) {												// Undoﾌﾗｸﾞ確認
		AfxMessageBox(IDS_CANT_SAVE);									// ｢曲面編集中は,保存NG｣
		return FALSE;													// 保存失敗
	}
	if(LicenseCheck()!=0) {												// ﾗｲｾﾝｽﾁｪｯｸ
		SetDemoFlg(1);													// 体験版
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
           !ext.CompareNoCase("gld")) {									// ｢GGD/GLF/GLD形式,上書きNG｣
			AfxMessageBox(IDS_ERR_GLD);
			return FALSE;
		}
		prompt.Format(IDS_FILE_REWRITE, m_strPathName);					// ｢ﾌｧｲﾙ更新OK｣
		if(AfxMessageBox(prompt, MB_OKCANCEL)==IDCANCEL) {
			return FALSE;
		}
	}
	View->Display(0);													// 再描画
	CWaitCursor	wait;													// ｳｪｲﾄ･ｶｰｿﾙを表示
	ext = newName.Right(3);
	ret = 2;							//202006 change ret default for write file illegal 
	if (!ext.CompareNoCase("djf")) {									//202006 djf追加		
		ret = WriteJgdFile(newName, 0);									// djg save special
	}

#ifdef STL_EXPORT //202006 STL export版のみ有効  WriteCommonFile()
	else if (!ext.CompareNoCase("stl")) {
			ret = WriteCommonFile(1, newName);							// STL形式ﾌｧｲﾙ保存
	}
#endif

	//if (!ext.CompareNoCase("jsd")) {
	//	ret = WriteJsdFile(newName, 1, 0);								// JSD形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("jsf")) {
	//	ret = WriteJsdFile(newName, 0, 0);								// JSF形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("gld")) {
	//	ret = WriteJsdFile(newName, 1, 1);								// GLD形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("glf")) {
	//	ret = WriteJsdFile(newName, 0, 1);								// GLF形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("jgd")) {
	//	ret = WriteJgdFile(newName, 0);									// JGD形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("ggd")) {
	//	ret = WriteJgdFile(newName, 1);									// GGD形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("dxf")) {
	//	ret = WriteCommonFile(0, newName);								// DXF形式ﾌｧｲﾙ保存
	//} else if(!ext.CompareNoCase("stl")) {
	//	ret = WriteCommonFile(1, newName);								// STL形式ﾌｧｲﾙ保存
	//}

	if(ret==1) {														// <ｴﾗｰ発生の場合>
		AfxMessageBox(IDS_ERR_FILEWRITE);								// ｢ﾌｧｲﾙ保存:NG｣
		remove(newName);												// 保存ﾌｧｲﾙ削除
		return FALSE;
	} else if(ret==2) {													// <形式違いの場合>
		AfxMessageBox(IDS_NOT_SAVE);									// ｢ﾌｧｲﾙ保存:不可｣
		return FALSE;
	} else if(ret<0) {													// <中止の場合>
		AfxMessageBox(IDS_STOP_PROCESS);								// ｢処理中止｣
		remove(newName);												// 保存ﾌｧｲﾙ削除
		return FALSE;
	}
	if(bReplace) {
		SetPathName(newName);
	}
	return TRUE;														// 保存成功
}

/*************************************/
BOOL CJcad3GlbDoc::SaveModified(void)									// ﾌｧｲﾙを保存するかの問合せ
/*************************************/									// TRUE->ｱﾌﾟﾘｹｰｼｮﾝ:終了OK
{
	OBJTYPE* op;
	int      dsp, edt, id;
	CString  prompt, name;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// dspflg:ON 立体無=>中止
		return TRUE;
	}
	if(!IsModified()) {													// 変更無は中止
		return TRUE;
	}
	if(m_strPathName.IsEmpty()) {										// ﾌｧｲﾙ名無
		name = m_strTitle;												// get name based on caption
		if(name.IsEmpty()) {
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
		}
	} else {
		name = m_strPathName;
	}
	prompt.Format(IDS_FILE_CHANGE, name);								// ｢保存OK?｣
	id = AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
	if(id==IDYES&&!DoFileSave()) {										// 保存失敗
		return FALSE;
	}
	if(id==IDCANCEL) {													// 保存ｷｬﾝｾﾙ
		return FALSE;
	}
	return TRUE;														// 保存成功/保存無
}
