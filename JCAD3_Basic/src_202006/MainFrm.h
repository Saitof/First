/////////////////////////////////////////////////////////////////////////////
// メインフレームウィンドウクラス(CMainFrame)宣言
// MainFrm.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/28 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "StatusCtrl.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // シリアル化からのみ作成します。
	CMainFrame();                                                       // ｸﾗｽの構築
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	UINT BackColor;
	UINT ObjColor;
	UINT GroupColor1;
	UINT GroupColor2;
	UINT GroupColor3;
	UINT GroupColor4;
	UINT GroupColor5;
	UINT GroupColor6;
	UINT GroupColor7;
	UINT GroupColor8;
	UINT GroupColor9;
	UINT GroupColor10;
	UINT GroupColor11;
	UINT GroupColor12;
	UINT GroupColor13;
	UINT GroupColor14;
	UINT GroupColor15;
	UINT GroupColor16;
	UINT SelectColor;
	UINT ChooseColor;
	UINT BoolColor;
	UINT GridColor;
	UINT GridBPColor;
	UINT TemplateColor;
	UINT BaseColor;
	UINT BoxColor;
	UINT CircleColor;
	UINT LineColor;
	UINT MeasureColor;
	UINT SelLineColor;
	UINT XGuideColor;
	UINT YGuideColor;
	UINT ZGuideColor;
	UINT NCDispColor1;
	UINT NCDispColor2;
	UINT TrackerColor0;
	UINT TrackerColor1;
	UINT TrackerColor2;
	UINT TrackerColor3;
	UINT TrackerColor4;
	UINT TrackerColor5;
	UINT BlackColor;
	UINT WhiteColor;
	UINT BorderColor;
	UINT BorderHColor;
	UINT BorderSColor;

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();                                              // ｸﾗｽの消滅
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusCtrl m_wndStatusBar;
	CToolBar m_wndToolBar;
	CToolBar m_wndJcadEdit;
	CToolBar m_wndView;
	CToolBar m_wndGroup;
	CToolBar m_wndScreenCtl;
	CToolBar m_wndRendering;
	CToolBar m_wndCreate;
	CToolBar m_wndEdit;
	CToolBar m_wndSurf;
	CToolBar m_wndJewelry;
	CToolBar m_wndMeasure;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose(void);
	afx_msg BOOL OnViewMenuCtrlBar(UINT nID);
	afx_msg	void OnUpdateMenuCtrlBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurPos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCrtPrm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFunc(CCmdUI* pCmdUI);
	afx_msg BOOL OnCommonMenu(UINT nID);
	afx_msg BOOL OnGroupMenu(UINT nID);
	afx_msg BOOL OnScrCtrlMenu(UINT nID);
	afx_msg BOOL OnMeasureMenu(UINT nID);
	afx_msg void OnUpdateMenuEnableOrCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuEnableOrCheck2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMeasureEnableOrCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMeasureCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMeasurePlaneCheck(CCmdUI* pCmdUI);
	afx_msg void OnFileNew(void);
	afx_msg void OnFileOpen(void);
	afx_msg void OnViewerSave(void);
	afx_msg void OnExportSLC(void);
	afx_msg void OnCaptureSet(void);
	afx_msg void OnPasswordSet(void);
	afx_msg void OnRestBackUp(void);
	afx_msg void OnUndo(void);
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnRedo(void);
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
	afx_msg void OnCopyObject(void);
	afx_msg void OnCutObject(void);
	afx_msg void OnUpdatePasteObject(CCmdUI* pCmdUI);
	afx_msg void OnDelete(void);
	afx_msg void OnAllPic(void);
	afx_msg void OnObjInfo(void);
	afx_msg void OnUpdateObjInfo(CCmdUI* pCmdUI);
	afx_msg void OnFunctionBtn(void);
	afx_msg void OnUpdateFunctionBtn(CCmdUI* pCmdUI);
	afx_msg void OnCopyInput(void);
	afx_msg void OnUpdateCopyInput(CCmdUI* pCmdUI);
	afx_msg void OnNumInput(void);
	afx_msg void OnUpdateNumInput(CCmdUI* pCmdUI);
	afx_msg void OnDispMode(void);
	afx_msg void OnDispRange(void);
	afx_msg void OnAllDisp(void);
	afx_msg void OnAllErase(void);
	afx_msg void OnScrState(void);
	afx_msg void OnUpdateScrState(CCmdUI* pCmdUI);
	afx_msg void OnDataUse(void);
	afx_msg void OnUpdateDataUse(CCmdUI* pCmdUI);
	afx_msg void OnParamDisp(void);
	afx_msg void OnUpdateParamDisp(CCmdUI* pCmdUI);
	afx_msg void OnGroupDsp(void);
	afx_msg void OnGroupSel(void);
	afx_msg void OnZoomIn(void);
	afx_msg void OnZoomOut(void);
	afx_msg void OnAutoZoom(void);
	afx_msg void OnExactZoom(void);
	afx_msg void OnScrollTool(void);
	afx_msg void OnUpdateScrollTool(CCmdUI* pCmdUI);
	afx_msg void OnHomePos(void);
	afx_msg void OnBackScreen(void);
	afx_msg void OnTruckUp(void);
	afx_msg void OnTruckDown(void);
	afx_msg void OnCentPars(void);
	afx_msg void OnEyeSet(void);
	afx_msg void OnEyeChange(void);
	afx_msg void OnUpdateEyeChange(CCmdUI* pCmdUI);
	afx_msg void OnGridDisp(void);
	afx_msg void OnUpdateGridDisp(CCmdUI* pCmdUI);
	afx_msg void OnGridBind(void);
	afx_msg void OnUpdateGridBind(CCmdUI* pCmdUI);
	afx_msg void OnGridSpace(void);
	afx_msg void OnGuideLineBox(void);
	afx_msg void OnDspTemplate(void);
	afx_msg void OnSetTemplate(void);
	afx_msg void OnTexture(void);
	afx_msg void OnSetHome(void);
	afx_msg void OnRayTrace(void);
	afx_msg void OnUpdateRayTrace(CCmdUI* pCmdUI);
	afx_msg void OnPovData(void);
	afx_msg void OnMaterial(void);
	afx_msg void OnCrtPrimitive(void);
	afx_msg void OnCrtCentCurve(void);
	afx_msg void OnCrtSetCurve(void);
	afx_msg void OnCrtSection(void);
	afx_msg void OnCrtSpiral(void);
	afx_msg void OnResetSurface(void);
	afx_msg void OnCrtConnect(void);
	afx_msg void OnCrtThick(void);
	afx_msg void OnCrtText(void);
	afx_msg void OnEditSurface(void);
	afx_msg void OnEditBool(void);
	afx_msg void OnSurfOAdjust(void);
	afx_msg void OnSurfParam(void);
	afx_msg void OnJewelryRing(void);
	afx_msg void OnSetUnit(void);
	afx_msg void OnMeasureCalc(void);
	afx_msg void OnUpdateMeasureCalc(CCmdUI* pCmdUI);
	afx_msg void OnMeasureVolume(void);
	afx_msg void OnSimulation(void);
	afx_msg void OnNcDisp(void);
	afx_msg void OnSetEnvironment(void);
	afx_msg void OnSetRender(void);
	afx_msg void OnSetRayTrace(void);
	afx_msg void OnRefManual(void);
	afx_msg void OnRegistUser(void);
	DECLARE_MESSAGE_MAP()

// オペレーション
public:
	void Initialize(void);												// 初期化
	void InitSColorFile(void);											// 初期ｼｽﾃﾑｶﾗｰﾌｧｲﾙ読込
	void ReadSColorFile(void);											// ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ読込
	void WriteSColorFile(void);											// ｼｽﾃﾑｶﾗｰ保存ﾌｧｲﾙ書込
	void ReadFilPathFile(void);											// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ読込
	void WriteFilPathFile(void);										// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ書込
	void GetPasswordData(char* wpwd, char* rpwd, int* save);			// ﾊﾟｽﾜｰﾄﾞﾃﾞｰﾀ取得
	void InitColor(void);												// 表示色初期化
	void SaveObjColor(UINT*);											// 立体表示色保存
	void RestoreObjColor(UINT*);										// 立体表示色復元
	void ChangeObjColor(UINT);											// 立体表示色変更
	void GetColor(int, double*, double*, double*);						// 表示色取得
	COLORREF GetColor(int no) { return m_Color[no]; }					// 表示色取得
	void SetColor(int no,  COLORREF col) { m_Color[no] = col; }			// 表示色設定
	void ExecuteMenu(UINT nID);											// ﾌｧﾝｸｼｮﾝ選択ﾒﾆｭｰ起動
	void SetGroupObj(void);												// ｸﾞﾙｰﾌﾟ立体設定
	void SetCurPos(double x, double y, double z);						// ｶｰｿﾙ位置設定
	void SetCrtPrm(int d, double w);									// 生成ﾊﾟﾗﾒｰﾀ設定
	BOOL DoPromptFName(CString& fname, CString& ext, BOOL bopen);		// ﾌｧｲﾙ名指定ﾀﾞｲｱﾛｸﾞ設定
	BOOL DoOpenFile(const CString& fname);								// ﾌｧｲﾙOPEN
	void MessageOut(UINT nID) { m_wndStatusBar.MessageOut(nID); }		// 出力ﾒｯｾｰｼﾞ設定
	void MessageOut(void) { m_wndStatusBar.MessageOut();}				// ﾒｯｾｰｼﾞ出力
	int  GetCopyCmdFlg(void) { return m_CopyCmdFlg; }					// ｺﾋﾟｰｺﾏﾝﾄﾞ有効ﾌﾗｸﾞ取得

protected:
	BOOL InitStatusBar(int nSeconds);									// ｽﾃｰﾀｽﾊﾞｰ作成&初期化
	BOOL InitToolBar(CToolBar* pBar, UINT nIDR, UINT titleID=0,			// ﾂｰﾙﾊﾞｰ作成&初期化
                     DWORD style=CBRS_TOP, UINT nID=AFX_IDW_TOOLBAR);
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);			// ﾂｰﾙﾊﾞｰ配置
	int  GetGroupKey(void);												// ｸﾞﾙｰﾌﾟ対応ｷｰ取得

private:
	UINT     m_nStatusPane1ID;
	UINT     m_nStatusPane1Style;
	INT      m_nStatusPane1Width;
	CString  m_sFilePath;
	char     m_cWPassword[11], m_cRPassword[11];
	INT      m_PasswordKind, m_PasswordSave;
	INT      m_OpenKind, m_SaveKind, m_ViewerKind;
	BOOL     m_bViewSave;
	INT      m_GroupObj[32], m_GroupDisp[32];
	double   m_XPos, m_YPos, m_ZPos;
	INT      m_CrtD;
	double   m_CrtW;
	COLORREF m_Color[64];
	INT      m_CopyCmdFlg;
};

extern CMainFrame* MainWnd;

//==============================<EOF:MainFrm.h>==============================