/////////////////////////////////////////////////////////////////////////////
// ステータスバークラス(CStatusCtrl)宣言
// StatusCtrl.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
class CStatusCtrl : public CStatusBar
{
public:
	CStatusCtrl(void);													// ｸﾗｽの構築
	virtual ~CStatusCtrl(void) { ; }									// ｸﾗｽの消滅

// アトリビュート
public:

// オペレーション
public:
	void MessageOut(UINT nID);											// ﾒｯｾｰｼﾞ表示
	void MessageOut(void);												// ﾒｯｾｰｼﾞ表示

// オーバーライド

protected:

	DECLARE_MESSAGE_MAP()

private:
	CString m_sMess;
};

//=============================<EOF:StatusCtrl.h>============================
