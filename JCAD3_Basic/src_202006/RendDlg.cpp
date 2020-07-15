/////////////////////////////////////////////////////////////////////////////
// レンダリング(RayTrace)ダイアログクラス定義
// RendDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "Rendering.h"
#define  WIDTHBYTES(i) ((i+31)/32*4)

/////////////////////////////////////////////////////////////////////////////
// CRendDlg ダイアログ

BEGIN_MESSAGE_MAP(CRendDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnRunTrace)
	ON_BN_CLICKED(IDC_BUTTON2, OnStpTrace)
	ON_BN_CLICKED(IDC_BUTTON3, OnPrtTrace)
	ON_BN_CLICKED(IDC_BUTTON4, OnChangeSize)
END_MESSAGE_MAP()

/*******************************************************************/
CRendDlg::CRendDlg(CWnd* pParent) : CDialog(CRendDlg::IDD, pParent)
/*******************************************************************/
{
	m_pParent = pParent;
	m_pDlgDC = NULL;													// ｸﾗｲｱﾝﾄDCﾎﾟｲﾝﾀ初期化
	m_SRndDC = 0;														// ﾒﾓﾘDC未作成
	m_SRndBit = 0;														// ﾋﾞｯﾄﾏｯﾌﾟ未作成
	InitSW = 0;															// ｳｨﾝﾄﾞｳ初期
	RunSW = 0;															// 未実行
	StpSW = 0;															// 中止未指示
	PaintSW = 0;														// 描画未完了
}

/*************************************************/
void CRendDlg::DoDataExchange(CDataExchange* pDX)
/*************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC1, m_Pic1);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Control(pDX, IDC_BUTTON3, m_Button3);
	DDX_Control(pDX, IDC_BUTTON4, m_Button4);
}

/////////////////////////////////////////////////////////////////////////////
// CRendDlg メッセージ ハンドラ

/*********************************/
BOOL CRendDlg::OnInitDialog(void)										//<<< ﾀﾞｲｱﾛｸﾞ初期化
/*********************************/
{
	CString str;

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	m_pDlgDC = new CClientDC(this);										// ｸﾗｲｱﾝﾄDC取得
	m_SRndDC = m_RndDC.CreateCompatibleDC(m_pDlgDC);					// ﾒﾓﾘDC作成
	GetClientRect(m_Rt1);												// ﾀﾞｲｱﾛｸﾞ矩形座標
	m_Pic1.GetWindowRect(m_Rt2);
	ScreenToClient(m_Rt2);												// ﾋﾟｸﾁｬｰ矩形座標
	m_RtD = m_Rt2, m_Rt2S = m_Rt2;										// ﾋﾟｸﾁｬｰﾌﾚｰﾑｻｲｽﾞ
	m_RtD.left+=1, m_RtD.top+=1, m_RtD.right-=1, m_RtD.bottom-=1;
	XSize = m_RtD.Width(), YSize = m_RtD.Height();						// 実描画ｻｲｽﾞ
	str.Format(IDS_STR72, XSize, YSize);
	SetWindowText(str);													// ﾀｲﾄﾙにｻｲｽﾞを表示
	CheckBtn();															// ﾎﾞﾀﾝ使用可否
	InitSW = 1;															// ｳｨﾝﾄﾞｳ初期完了
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/****************************/
void CRendDlg::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/****************************/
{
	int xs, ys;

	if(PaintSW||RunSW) {												// 描画完了/実行中の場合
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// ﾚﾝﾀﾞﾘﾝｸﾞ時と現在のｻｲｽﾞ
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// から小さい方を使用
		ys = (RunSW&&ys>Line) ? Line : ys;
		m_pDlgDC->BitBlt(m_RtD.left, m_RtD.top, xs, ys, &m_RndDC, 0, 0, SRCCOPY);
	}
	CDialog::OnPaint();
}

/*************************************************/
void CRendDlg::OnSize(UINT nType, int cx, int cy)						//<<< WM_SIZEﾒｯｾｰｼﾞ応答処理
/*************************************************/
{
	CString str;
	CRect   rct;
	int     x, y, xs, ys;

	CDialog::OnSize(nType, cx, cy);
	if(!InitSW) {														// ｳｨﾝﾄﾞｳ初期未完は終了
		return;
	}
	x = cx-m_Rt1.Width();												// 横方向の差
	y = cy-m_Rt1.Height();												// 縦方向の差
	GetClientRect(m_Rt1);												// ﾀﾞｲｱﾛｸﾞ矩形座標更新
	m_Rt2.right += x, m_Rt2.bottom += y, m_Rt2S = m_Rt2;				// ﾋﾟｸﾁｬｰｻｲｽﾞ変更
	if(m_Rt2S.Width()>PXLMAX+2) {										// ﾋﾟｸﾁｬｰﾌﾚｰﾑｻｲｽﾞは
		m_Rt2S.right = m_Rt2S.left+PXLMAX+2;							// 1002x1002が有効ｻｲｽﾞ
	}
	if(m_Rt2S.Height()>PXLMAX+2){
		m_Rt2S.bottom = m_Rt2S.top+PXLMAX+2;
	}
	m_RtD = m_Rt2S;
	if(m_RtD.Width()>3&&m_RtD.Height()>3) {								// 実描画ｻｲｽﾞは
		m_RtD.left+=1, m_RtD.top+=1, m_RtD.right-=1, m_RtD.bottom-=1;	// 有効ｻｲｽﾞの内側
	} else {
		m_RtD.left=0,  m_RtD.top=0,  m_RtD.right=0,  m_RtD.bottom=0;
	}
	XSize = m_RtD.Width(), YSize = m_RtD.Height();
	m_Pic1.SetWindowPos(NULL, m_Rt2S.left, m_Rt2S.top, m_Rt2S.Width(),
                        m_Rt2S.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
	str.Format(IDS_STR72, XSize, YSize);
	SetWindowText(str);													// ﾀｲﾄﾙにｻｲｽﾞを表示
	Invalidate(TRUE);
	if(PaintSW&&!RunSW) {												// 描画完了/実行中の場合
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// ﾚﾝﾀﾞﾘﾝｸﾞ時と現在のｻｲｽﾞ
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// から小さい方を使用
		rct.left = m_RtD.left, rct.top = m_RtD.top;
		rct.right = m_RtD.left+xs, rct.bottom = m_RtD.top+ys;
		ValidateRect(&rct);
	}
}

/*****************************/
void CRendDlg::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/*****************************/
{
	if(!RunSW) {														// 実行中以外は中止処理
		DestroyWindow();
		View->EndRendDlg();												// ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ終了
	}
}

/*******************************************/
LRESULT CRendDlg::OnNcHitTest(CPoint point)								//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/*******************************************/							//<<< ﾏｳｽｶｰｿﾙの位置を取得
{
	LRESULT ret = CDialog::OnNcHitTest(point);
	if(ret==HTCLIENT) {
		return HTCAPTION;												// ｸﾗｲｱﾝﾄ領域ﾄﾞﾗｯｸﾞでｳｨﾝﾄﾞｳ移動
	} else {
		return ret;
	}
}

/******************************/
void CRendDlg::OnDestroy(void)											//<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/******************************/
{
	if(m_pDlgDC) {
		delete m_pDlgDC;												// ｸﾗｲｱﾝﾄDC削除
	}
	if(m_SRndDC) {
		m_RndDC.DeleteDC();												// ﾒﾓﾘDC削除
	}
	if(m_SRndBit) {
		m_RndBit.DeleteObject();										// ﾋﾞｯﾄﾏｯﾌﾟ削除
	}
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "RendLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "RendTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*******************************/
void CRendDlg::OnRunTrace(void)											//<<< 実行ﾎﾞﾀﾝ押下処理
/*******************************/
{
	RunSW = 1;															// 実行中
	StpSW = 0;															// 中止未指示
	PaintSW = 0;														// 描画未完了
	XSizeS = XSize, YSizeS = YSize;										// 描画領域保存
	CheckBtn();															// ﾎﾞﾀﾝ使用可否
	if(m_SRndBit) {
		m_RndBit.DeleteObject();										// ﾋﾞｯﾄﾏｯﾌﾟ作成済みは削除
	}
	m_SRndBit=m_RndBit.CreateCompatibleBitmap(m_pDlgDC, XSizeS, YSizeS);// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ作成
	m_RndDC.SelectObject(&m_RndBit);									// ﾒﾓﾘDCへﾋﾞｯﾄﾏｯﾌﾟ設定
	m_RndDC.BitBlt(0, 0, XSizeS, YSizeS, m_pDlgDC,
                   m_RtD.left, m_RtD.top, SRCCOPY);						// ﾒﾓﾘDCへﾀﾞｲｱﾛｸﾞをｺﾋﾟｰ
	CRendDlg* prm = this;												// ﾚｲ･ﾄﾚｰｼﾝｸﾞ処理起動
	m_pThread = AfxBeginThread(RayTracingProc, (LPVOID)prm, THREAD_PRIORITY_LOWEST);
}

/*******************************/
void CRendDlg::OnStpTrace(void)											//<<< 終了/中止ﾎﾞﾀﾝ押下処理
/*******************************/
{
	if(RunSW) {															// 実行中の場合
		StpSW = 1;														// 中止指示
		CheckBtn();														// ﾎﾞﾀﾝ使用可否
	} else {															// 未実行の場合
		DestroyWindow();												// 終了処理
		View->EndRendDlg();												// ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ終了
	}
}

/*******************************/
void CRendDlg::OnPrtTrace(void)											//<<< 印刷ﾎﾞﾀﾝ押下処理
/*******************************/
{
	View->ChangePrint();												// ﾌﾟﾘﾝﾀ出力切り替え
}

/*********************************/
void CRendDlg::OnChangeSize(void)										//<<< ｻｲｽﾞ変更ﾎﾞﾀﾝ押下処理
/*********************************/
{
	int w, h, x, y;

	w = View->GetPixelSize1();											// pixelｻｲｽﾞ1取得
	h = View->GetPixelSize2();											// pixelｻｲｽﾞ2取得
	CInputDlg dlg(26);
	dlg.SetValue(w, h);													// 既存値表示
	if(dlg.DoModal()==IDOK) {											// ｸﾞﾘｯﾄﾞ間隔設定ﾀﾞｲｱﾛｸﾞ表示
		dlg.GetValue(&w, &h);											// ﾀﾞｲｱﾛｸﾞ ｻｲｽﾞ
		View->SetPixelSize1(w);											// pixelｻｲｽﾞ1設定
		View->SetPixelSize2(h);											// pixelｻｲｽﾞ2設定
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);										// ｳｨﾝﾄﾞｳ表示状態取得
		x = wp.rcNormalPosition.right - wp.rcNormalPosition.left;		// 現在のﾀﾞｲｱﾛｸﾞｻｲｽﾞ
		y = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_SHOW;
		wp.rcNormalPosition.left = 0;
		wp.rcNormalPosition.top = 0;
		wp.rcNormalPosition.right = x+w-XSize;							// 指定領域のﾀﾞｲｱﾛｸﾞｻｲｽﾞ
		wp.rcNormalPosition.bottom = y+h-YSize;
		SetWindowPlacement(&wp);										// ｳｨﾝﾄﾞｳ表示状態設定
	}
}

/////////////////////////////////////////////////////////////////////////////
// オペレーション

/***************************/
BOOL CRendDlg::Create(void)
/***************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "RendLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "RendTop", -1);
	if(left>0) {
		width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_HIDE;
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = left + width;
		wp.rcNormalPosition.bottom = top + height;
		SetWindowPlacement(&wp);										// ｳｨﾝﾄﾞｳ表示状態設定
	}
	return ret;
}

/***************************************************************/
void CRendDlg::DDBtoDIB(CDC* pDC, int out, int check, int size)			//<<< DDB=>DIB変換
/***************************************************************/
{
	CWaitCursor wait;													// ｳｪｲﾄ･ｶｰｿﾙ表示
	CDC     m_scdc;
	CBitmap m_bitmap;
	int     px, py, esw=0;
	BITMAP  bm;
	DWORD   dwLen;
	WORD    biBits;
	HDC     hdc;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;

	m_hDIB = NULL;														// DIBﾊﾝﾄﾞﾙ初期化
	if(!out&&check&&size>0) {											// <<出力ｻｲｽﾞ指定:有>>
		px = pDC->GetDeviceCaps(LOGPIXELSX);							// X方向ﾌﾟﾘﾝﾀdpi
		py = pDC->GetDeviceCaps(LOGPIXELSY);							// Y方向ﾌﾟﾘﾝﾀdpi
		if(XSizeS>YSizeS) {												// <幅が大>
			XSizeP = (int)((double)px*size/25.4);						// ﾌﾟﾘﾝﾀ印刷ｻｲｽﾞ(dpi)
			YSizeP = (int)(((double)YSizeS*py*XSizeP)/((double)XSizeS*px));
		} else {														// <高が大>
			YSizeP = (int)((double)py*size/25.4);						// ﾌﾟﾘﾝﾀ印刷ｻｲｽﾞ(dpi)
			XSizeP = (int)(((double)XSizeS*px*YSizeP)/((double)YSizeS*py));
		}
	} else {															// <<出力ｻｲｽﾞ指定:無>>
		XSizeP = XSizeS, YSizeP = YSizeS;								// 複写元と同一ｻｲｽﾞ
	}
	if(!m_scdc.CreateCompatibleDC(m_pDlgDC)) {							// ｽｸﾘｰﾝﾒﾓﾘDC生成
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pDlgDC, XSizeP, YSizeP)) {	// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ生成
		esw = 1;
	}
	CBitmap* pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ﾒﾓﾘDCへﾋﾞｯﾄﾏｯﾌﾟ設定
	int pOldSMode = m_scdc.SetStretchBltMode(COLORONCOLOR);				// 伸縮モード設定
	if(!m_scdc.StretchBlt(0, 0, XSizeP, YSizeP, &m_RndDC,
                          0, 0, XSizeS, YSizeS, SRCCOPY)) {				// ｽｸﾘｰﾝ->ﾒﾓﾘDC伸縮ｺﾋﾟｰ
		esw = 1;
	}
	m_scdc.SetStretchBltMode(pOldSMode);								// 伸縮ﾓｰﾄﾞ戻し
	m_scdc.SelectObject(pOldBitmap);									// ﾒﾓﾘDC->ﾋﾞｯﾄﾏｯﾌﾟ切離
	if(esw) {
		AfxMessageBox(IDS_PRTERR);										// ｴﾗｰﾒｯｾｰｼﾞ後中止
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙ取得
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;	// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBﾍｯﾀﾞｰ作成
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth*biBits)*bm.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	if(biBits==1) {														// ﾊﾟﾚｯﾄｻｲｽﾞ計算
		PalSize = 2;
	} else if(biBits==4) {
		PalSize = 16;
	} else if(biBits==8) {
		PalSize = 256;
	} else {
		PalSize = 0;
	}
	PalSize = PalSize*sizeof(RGBQUAD);
	dwLen = bi.biSize+PalSize+bi.biSizeImage;
	hdc = m_scdc.m_hDC;
	hpal = ::SelectPalette(hdc, hpal, FALSE);
	m_hDIB = ::GlobalAlloc(GMEM_MOVEABLE, dwLen);
	lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)m_hDIB);
	::RealizePalette(hdc);
	*lpbi = bi;
	::GetDIBits(hdc, hbitmap, 0, (UINT)bi.biHeight, (LPSTR)lpbi+lpbi->biSize+PalSize,
                (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);					// DDB => DIB
	::GlobalUnlock((HGLOBAL)m_hDIB);
	::SelectPalette(hdc, hpal, FALSE);
	m_scdc.DeleteDC();													// ﾒﾓﾘDC削除
	m_bitmap.DeleteObject();											// ｽｸﾘｰﾝ互換ﾋﾞｯﾄﾏｯﾌﾟ削除
}

/***********************************************/
void CRendDlg::OutputPrinter(CDC* pDC, int pos)							//<<< DIB ﾌﾟﾘﾝﾀ出力
/***********************************************/
{
	HDC   hdc;
	LPSTR lpDIBHdr;														// Pointer to BITMAPINFOHEADER
	LPSTR lpDIBBits;													// Pointer to DIB bits
	int   sx, sy, px=0, py=0, bSuccess=0;

	if(m_hDIB==NULL) {
		return;
	}
	if(!pos) {															// <用紙中央位置印刷の場合>
		sx = pDC->GetDeviceCaps(HORZRES);								// X方向ﾌﾟﾘﾝﾀ幅
		sy = pDC->GetDeviceCaps(VERTRES);								// Y方向ﾌﾟﾘﾝﾀ高
		px = (sx>XSizeP) ? (sx-XSizeP)/2 : px;							// X方向印刷位置
		py = (sy>YSizeP) ? (sy-YSizeP)/2 : py;							// Y方向印刷位置
	}
	lpDIBHdr  = (LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	lpDIBBits = lpDIBHdr + *(LPDWORD)lpDIBHdr + PalSize;
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	hdc  = pDC->m_hDC;
	hpal = ::SelectPalette(hdc, hpal, TRUE);							// ｼｽﾃﾑﾊﾟﾚｯﾄ変更
	bSuccess = ::SetDIBitsToDevice(hdc,									// hDC
                                   px, py,								// DestX,Y
                                   XSizeP, YSizeP,						// nDestWidth,Height
                                   0, 0,								// SrcX,Y
                                   0,									// nStartScan
                                   (WORD)YSizeP,						// nNumScans
                                   lpDIBBits,							// lpBits
                                   (LPBITMAPINFO)lpDIBHdr,				// lpBitsInfo
                                   DIB_RGB_COLORS);						// wUsage
	::GlobalUnlock((HGLOBAL)m_hDIB);
	::SelectPalette(hdc, hpal, TRUE);
	if(bSuccess==0) {
		AfxMessageBox(IDS_PRTERR);										// 解像度を下げる
	}
}

/***************************************/
void CRendDlg::OutputFile(CString path)									//<<< DIB ﾌｧｲﾙ出力
/***************************************/
{
	CString ext;
	BOOL    ret;
	int     cmp;

	if(m_hDIB!=NULL) {
		ext = path.Right(3);											// ﾌｧｲﾙの拡張子
		if(!ext.CompareNoCase("jpg")) {									// <JPEG形式>
			cmp = View->GetCapJComp();									// ｷｬﾌﾟﾁｬJPEG圧縮率取得
			ret = IKJpegFileSave(path, m_hDIB, 0, cmp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEGﾌｧｲﾙ出力
		} else if(!ext.CompareNoCase("bmp")) {							// <ﾋﾞｯﾄﾏｯﾌﾟ形式>
			ret = IKBmpFileSave(path, m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// ﾋﾞｯﾄﾏｯﾌﾟﾌｧｲﾙ出力
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ﾌｧｲﾙｴﾗｰ
		}
	}
}

/****************************/
void CRendDlg::FreeDIB(void)											//<<< DIB領域解放
/****************************/
{
	::GlobalFree((HGLOBAL)m_hDIB);
}

/*****************************/
void CRendDlg::CheckBtn(void)											//<<< ﾎﾞﾀﾝ使用可否
/*****************************/
{
	CString btn;

	if(RunSW) {
		btn.LoadString(IDS_STR70);										// ｢中止｣ﾎﾞﾀﾝ
	} else {
		btn.LoadString(IDS_STR71);										// ｢終了｣ﾎﾞﾀﾝ
	}
	m_Button2.SetWindowText(btn);										// ﾎﾞﾀﾝ名変更
	m_Button1.EnableWindow(!RunSW);										// ｢実行｣ﾎﾞﾀﾝ有効/無効
	m_Button2.EnableWindow(!StpSW);										// ｢終了/中止｣ﾎﾞﾀﾝ有効/無効
	m_Button3.EnableWindow(!RunSW&&PaintSW);							// ｢印刷｣ﾎﾞﾀﾝ有効/無効
	m_Button4.EnableWindow(!RunSW);										// ｢ｻｲｽﾞ変更｣ﾎﾞﾀﾝ有効/無効
}

/******************************************/
void CRendDlg::GetRendSize(int* x, int* y)								//<<< 描画ｻｲｽﾞ取得
/******************************************/
{
	*x = XSizeS, *y = YSizeS;
}

/******************************/
int CRendDlg::CheckStpSW(void)											//<<< 中止SWﾁｪｯｸ
/******************************/
{
	return StpSW;
}

/**************************************************************/
void CRendDlg::SetRendColor(int x, int y, int r, int g, int b)			// ｽｸﾘｰﾝ描画
/**************************************************************/
{
	CRect rct;
	int   xs, ys;

	if(x<0) {															// ﾚｲ･ﾄﾚｰｼﾝｸﾞ終了の場合
		RunSW = 0, StpSW = 0;											// 未実行, 中止未指示
		PaintSW = 1;													// 描画完了
		CheckBtn();														// ﾎﾞﾀﾝ使用可否
	} else {															// ﾚｲ･ﾄﾚｰｼﾝｸﾞ中の場合
		Line = y;														// 描画済み行保存
		m_RndDC.SetPixelV(x, y, RGB(r, g, b));							// ﾒﾓﾘDCのﾋﾟｸｾﾙを設定
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// ﾚﾝﾀﾞﾘﾝｸﾞ時と現在のｻｲｽﾞ
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// から小さい方を使用
		rct.left = m_RtD.left;
		rct.top = m_RtD.top;
		rct.right = m_RtD.left+xs;
		rct.bottom = m_RtD.top+ys;
		RedrawWindow(&rct, NULL, RDW_INVALIDATE);						// ｽｸﾘｰﾝ再描画
	}
}
