/////////////////////////////////////////////////////////////////////////////
// Documentクラス(CJcad3GlbDoc)宣言
// Jcad3GlbDoc.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___JCAD3GLBDOC_H
#define ___JCAD3GLBDOC_H

#pragma once

class CJcad3GlbDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CJcad3GlbDoc();														// ｸﾗｽの構築
	DECLARE_DYNCREATE(CJcad3GlbDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace=TRUE);
	virtual BOOL SaveModified(void);

// 実装
public:
	virtual ~CJcad3GlbDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___JCAD3GLBDOC_H

//============================<EOF:Jcad3GlbDoc.h>============================