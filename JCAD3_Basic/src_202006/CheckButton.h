/////////////////////////////////////////////////////////////////////////////
// プッシュボタンクラス宣言
// CheckButton.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/10 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCheckButton ダイアログ

#ifndef ___CHECKBUTTON_H
#define ___CHECKBUTTON_H

class CCheckButton : public CBitmapButton
{
// コンストラクション
public:
	CCheckButton(void);

// アトリビュート
public:

// オペレーション
public:
	BOOL LoadBitmap(const CString& strBitmapName);
	BOOL SetCheckBtn(int check);

// オーバーライド

// インプリメンテーション
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
// CCheckBox ダイアログ

#ifndef ___CHECKBOX_H
#define ___CHECKBOX_H

class CCheckBox : public CBitmapButton
{
// コンストラクション
public:
	CCheckBox(void);

// アトリビュート
public:

// オペレーション
public:
	BOOL LoadBitmap(const CString& strBitmap="");
	BOOL SetCheckBtn(int check);

// オーバーライド

// インプリメンテーション
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