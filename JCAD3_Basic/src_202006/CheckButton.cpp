/////////////////////////////////////////////////////////////////////////////
// プッシュボタンクラス定義
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
// CCheckButton ダイアログ

BEGIN_MESSAGE_MAP(CCheckButton, CBitmapButton)
END_MESSAGE_MAP()

/********************************/
CCheckButton::CCheckButton(void)										// ｸﾗｽの構築
/********************************/
{
	m_BitmapName = "";
	m_Check = -1;
}

/*********************************/
CCheckButton::~CCheckButton(void)										// ｸﾗｽの消滅
/*********************************/
{
}

/*******************************************************/
BOOL CCheckButton::LoadBitmap(const CString& strBitmap)					// ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ設定
/*******************************************************/
{
	BOOL ret;

	m_BitmapName = strBitmap;
	ret = SetCheckBtn(0);												// OFF状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
	return ret;
}

/*****************************************/
BOOL CCheckButton::SetCheckBtn(int check)								// 釦のON/OFF状態設定
/*****************************************/
{
	BOOL ret;

	if(m_Check!=check) {
		if(check) {														// ON状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
			ret = LoadBitmaps(m_BitmapName+"D", m_BitmapName+"D", m_BitmapName+"DF");
		} else {														// OFF状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
			ret = LoadBitmaps(m_BitmapName+"U", m_BitmapName+"D", m_BitmapName+"F");
		}
		SizeToContent();												// 釦幅をﾋﾞｯﾄﾏｯﾌﾟに合わせる
		Invalidate();													// 再表示のため
		m_Check = check;
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CCheckBox ダイアログ

BEGIN_MESSAGE_MAP(CCheckBox, CBitmapButton)
END_MESSAGE_MAP()

/**************************/
CCheckBox::CCheckBox(void)												// ｸﾗｽの構築
/**************************/
{
	m_BitmapName = "CHECK";
	m_Check = -1;
}

/***************************/
CCheckBox::~CCheckBox(void)												// ｸﾗｽの消滅
/***************************/
{
}

/****************************************************/
BOOL CCheckBox::LoadBitmap(const CString& strBitmap)					// ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ設定
/****************************************************/
{
	BOOL ret;

	if(strBitmap!="") {
		m_BitmapName = strBitmap;
	}
	ret = SetCheckBtn(0);												// OFF状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
	return ret;
}

/**************************************/
BOOL CCheckBox::SetCheckBtn(int check)									// 釦のON/OFF状態設定
/**************************************/
{
	BOOL ret;

	if(m_Check!=check) {
		if(check) {														// ON状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
			ret = LoadBitmaps(m_BitmapName+"CU", m_BitmapName+"CD", m_BitmapName+"CF");
		} else {														// OFF状態ﾋﾞｯﾄﾏｯﾌﾟｲﾒｰｼﾞ読込
			ret = LoadBitmaps(m_BitmapName+"U", m_BitmapName+"D", m_BitmapName+"F");
		}
		SizeToContent();												// 釦幅をﾋﾞｯﾄﾏｯﾌﾟに合わせる
		Invalidate();													// 再表示のため
		m_Check = check;
	}
	return ret;
}
