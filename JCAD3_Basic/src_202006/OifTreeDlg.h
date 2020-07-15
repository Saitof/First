/////////////////////////////////////////////////////////////////////////////
// 立体情報ツリーダイアログクラス(COifTreeDlg)宣言
// OifTreeDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/11 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ
#include "DataType.h"
#include "OifTreeCtrl.h"

class COifTreeDlg : public CDialog
{
// コンストラクション
public:
	COifTreeDlg(CWnd* pParent=NULL);

// ダイアログ データ
	enum { IDD = IDD_OBJ_INFO };
	COifTreeCtrl m_OifTreeCtrl;
	CButton      m_Button2;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV support
	virtual BOOL OnInitDialog();

// インプリメンテーション
public:
	virtual void OnCancel(void);
	virtual void OnOK(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy(void);
	afx_msg void OnSelChanged(LPNMHDR pnmhdr, LRESULT* pLResult);
	afx_msg void OnCreateUnit(void);
	afx_msg void OnDeleteUnit(void);
	DECLARE_MESSAGE_MAP()

// オペレーション
public:
	BOOL Create(void);
	void InitOifItem(void);
	void SetOifItem(OIFTYPE* oif);
	void DelOifItem(OIFTYPE* oif);
	void UpdateOifItem(OIFTYPE* oif);
	void SelectOifItem(OIFTYPE* oif);
	void UpdateDspFlg(int flg, OBJTYPE* op);
	void UpdateEdtFlg(int flg, OBJTYPE* op);

private:
	CWnd* m_pParent;
};

//============================<EOF:OifTreeDlg.h>=============================
