/////////////////////////////////////////////////////////////////////////////
// �����_�����O(RayTrace)�_�C�A���O�N���X��`
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
// CRendDlg �_�C�A���O

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
	m_pDlgDC = NULL;													// �ײ���DC�߲��������
	m_SRndDC = 0;														// ���DC���쐬
	m_SRndBit = 0;														// �ޯ�ϯ�ߖ��쐬
	InitSW = 0;															// ����޳����
	RunSW = 0;															// �����s
	StpSW = 0;															// ���~���w��
	PaintSW = 0;														// �`�斢����
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
// CRendDlg ���b�Z�[�W �n���h��

/*********************************/
BOOL CRendDlg::OnInitDialog(void)										//<<< �޲�۸ޏ�����
/*********************************/
{
	CString str;

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	m_pDlgDC = new CClientDC(this);										// �ײ���DC�擾
	m_SRndDC = m_RndDC.CreateCompatibleDC(m_pDlgDC);					// ���DC�쐬
	GetClientRect(m_Rt1);												// �޲�۸ދ�`���W
	m_Pic1.GetWindowRect(m_Rt2);
	ScreenToClient(m_Rt2);												// �߸�����`���W
	m_RtD = m_Rt2, m_Rt2S = m_Rt2;										// �߸����ڰѻ���
	m_RtD.left+=1, m_RtD.top+=1, m_RtD.right-=1, m_RtD.bottom-=1;
	XSize = m_RtD.Width(), YSize = m_RtD.Height();						// ���`�滲��
	str.Format(IDS_STR72, XSize, YSize);
	SetWindowText(str);													// ���قɻ��ނ�\��
	CheckBtn();															// ���ݎg�p��
	InitSW = 1;															// ����޳��������
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/****************************/
void CRendDlg::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/****************************/
{
	int xs, ys;

	if(PaintSW||RunSW) {												// �`�抮��/���s���̏ꍇ
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// �����ݸގ��ƌ��݂̻���
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// ���珬���������g�p
		ys = (RunSW&&ys>Line) ? Line : ys;
		m_pDlgDC->BitBlt(m_RtD.left, m_RtD.top, xs, ys, &m_RndDC, 0, 0, SRCCOPY);
	}
	CDialog::OnPaint();
}

/*************************************************/
void CRendDlg::OnSize(UINT nType, int cx, int cy)						//<<< WM_SIZEү���މ�������
/*************************************************/
{
	CString str;
	CRect   rct;
	int     x, y, xs, ys;

	CDialog::OnSize(nType, cx, cy);
	if(!InitSW) {														// ����޳���������͏I��
		return;
	}
	x = cx-m_Rt1.Width();												// �������̍�
	y = cy-m_Rt1.Height();												// �c�����̍�
	GetClientRect(m_Rt1);												// �޲�۸ދ�`���W�X�V
	m_Rt2.right += x, m_Rt2.bottom += y, m_Rt2S = m_Rt2;				// �߸������ޕύX
	if(m_Rt2S.Width()>PXLMAX+2) {										// �߸����ڰѻ��ނ�
		m_Rt2S.right = m_Rt2S.left+PXLMAX+2;							// 1002x1002���L������
	}
	if(m_Rt2S.Height()>PXLMAX+2){
		m_Rt2S.bottom = m_Rt2S.top+PXLMAX+2;
	}
	m_RtD = m_Rt2S;
	if(m_RtD.Width()>3&&m_RtD.Height()>3) {								// ���`�滲�ނ�
		m_RtD.left+=1, m_RtD.top+=1, m_RtD.right-=1, m_RtD.bottom-=1;	// �L�����ނ̓���
	} else {
		m_RtD.left=0,  m_RtD.top=0,  m_RtD.right=0,  m_RtD.bottom=0;
	}
	XSize = m_RtD.Width(), YSize = m_RtD.Height();
	m_Pic1.SetWindowPos(NULL, m_Rt2S.left, m_Rt2S.top, m_Rt2S.Width(),
                        m_Rt2S.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
	str.Format(IDS_STR72, XSize, YSize);
	SetWindowText(str);													// ���قɻ��ނ�\��
	Invalidate(TRUE);
	if(PaintSW&&!RunSW) {												// �`�抮��/���s���̏ꍇ
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// �����ݸގ��ƌ��݂̻���
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// ���珬���������g�p
		rct.left = m_RtD.left, rct.top = m_RtD.top;
		rct.right = m_RtD.left+xs, rct.bottom = m_RtD.top+ys;
		ValidateRect(&rct);
	}
}

/*****************************/
void CRendDlg::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/*****************************/
{
	if(!RunSW) {														// ���s���ȊO�͒��~����
		DestroyWindow();
		View->EndRendDlg();												// �����ݸ��޲�۸ޏI��
	}
}

/*******************************************/
LRESULT CRendDlg::OnNcHitTest(CPoint point)								//<<< WM_NCHITTEST ү���މ�������
/*******************************************/							//<<< ϳ����ق̈ʒu���擾
{
	LRESULT ret = CDialog::OnNcHitTest(point);
	if(ret==HTCLIENT) {
		return HTCAPTION;												// �ײ��ė̈���ׯ�ނų���޳�ړ�
	} else {
		return ret;
	}
}

/******************************/
void CRendDlg::OnDestroy(void)											//<<< WM_DESTROYү���މ�������
/******************************/
{
	if(m_pDlgDC) {
		delete m_pDlgDC;												// �ײ���DC�폜
	}
	if(m_SRndDC) {
		m_RndDC.DeleteDC();												// ���DC�폜
	}
	if(m_SRndBit) {
		m_RndBit.DeleteObject();										// �ޯ�ϯ�ߍ폜
	}
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "RendLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "RendTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*******************************/
void CRendDlg::OnRunTrace(void)											//<<< ���s���݉�������
/*******************************/
{
	RunSW = 1;															// ���s��
	StpSW = 0;															// ���~���w��
	PaintSW = 0;														// �`�斢����
	XSizeS = XSize, YSizeS = YSize;										// �`��̈�ۑ�
	CheckBtn();															// ���ݎg�p��
	if(m_SRndBit) {
		m_RndBit.DeleteObject();										// �ޯ�ϯ�ߍ쐬�ς݂͍폜
	}
	m_SRndBit=m_RndBit.CreateCompatibleBitmap(m_pDlgDC, XSizeS, YSizeS);// ��ذ݌݊��ޯ�ϯ�ߍ쐬
	m_RndDC.SelectObject(&m_RndBit);									// ���DC���ޯ�ϯ�ߐݒ�
	m_RndDC.BitBlt(0, 0, XSizeS, YSizeS, m_pDlgDC,
                   m_RtD.left, m_RtD.top, SRCCOPY);						// ���DC���޲�۸ނ��߰
	CRendDlg* prm = this;												// ڲ��ڰ�ݸޏ����N��
	m_pThread = AfxBeginThread(RayTracingProc, (LPVOID)prm, THREAD_PRIORITY_LOWEST);
}

/*******************************/
void CRendDlg::OnStpTrace(void)											//<<< �I��/���~���݉�������
/*******************************/
{
	if(RunSW) {															// ���s���̏ꍇ
		StpSW = 1;														// ���~�w��
		CheckBtn();														// ���ݎg�p��
	} else {															// �����s�̏ꍇ
		DestroyWindow();												// �I������
		View->EndRendDlg();												// �����ݸ��޲�۸ޏI��
	}
}

/*******************************/
void CRendDlg::OnPrtTrace(void)											//<<< ������݉�������
/*******************************/
{
	View->ChangePrint();												// ������o�͐؂�ւ�
}

/*********************************/
void CRendDlg::OnChangeSize(void)										//<<< ���ޕύX���݉�������
/*********************************/
{
	int w, h, x, y;

	w = View->GetPixelSize1();											// pixel����1�擾
	h = View->GetPixelSize2();											// pixel����2�擾
	CInputDlg dlg(26);
	dlg.SetValue(w, h);													// �����l�\��
	if(dlg.DoModal()==IDOK) {											// ��د�ފԊu�ݒ��޲�۸ޕ\��
		dlg.GetValue(&w, &h);											// �޲�۸� ����
		View->SetPixelSize1(w);											// pixel����1�ݒ�
		View->SetPixelSize2(h);											// pixel����2�ݒ�
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);										// ����޳�\����Ԏ擾
		x = wp.rcNormalPosition.right - wp.rcNormalPosition.left;		// ���݂��޲�۸޻���
		y = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_SHOW;
		wp.rcNormalPosition.left = 0;
		wp.rcNormalPosition.top = 0;
		wp.rcNormalPosition.right = x+w-XSize;							// �w��̈���޲�۸޻���
		wp.rcNormalPosition.bottom = y+h-YSize;
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
}

/////////////////////////////////////////////////////////////////////////////
// �I�y���[�V����

/***************************/
BOOL CRendDlg::Create(void)
/***************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
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
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/***************************************************************/
void CRendDlg::DDBtoDIB(CDC* pDC, int out, int check, int size)			//<<< DDB=>DIB�ϊ�
/***************************************************************/
{
	CWaitCursor wait;													// ���ĥ���ٕ\��
	CDC     m_scdc;
	CBitmap m_bitmap;
	int     px, py, esw=0;
	BITMAP  bm;
	DWORD   dwLen;
	WORD    biBits;
	HDC     hdc;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;

	m_hDIB = NULL;														// DIB����ُ�����
	if(!out&&check&&size>0) {											// <<�o�ͻ��ގw��:�L>>
		px = pDC->GetDeviceCaps(LOGPIXELSX);							// X���������dpi
		py = pDC->GetDeviceCaps(LOGPIXELSY);							// Y���������dpi
		if(XSizeS>YSizeS) {												// <������>
			XSizeP = (int)((double)px*size/25.4);						// ������������(dpi)
			YSizeP = (int)(((double)YSizeS*py*XSizeP)/((double)XSizeS*px));
		} else {														// <������>
			YSizeP = (int)((double)py*size/25.4);						// ������������(dpi)
			XSizeP = (int)(((double)XSizeS*px*YSizeP)/((double)YSizeS*py));
		}
	} else {															// <<�o�ͻ��ގw��:��>>
		XSizeP = XSizeS, YSizeP = YSizeS;								// ���ʌ��Ɠ��껲��
	}
	if(!m_scdc.CreateCompatibleDC(m_pDlgDC)) {							// ��ذ����DC����
		esw = 1;
	}
	if(!m_bitmap.CreateCompatibleBitmap(m_pDlgDC, XSizeP, YSizeP)) {	// ��ذ݌݊��ޯ�ϯ�ߐ���
		esw = 1;
	}
	CBitmap* pOldBitmap = m_scdc.SelectObject(&m_bitmap);				// ���DC���ޯ�ϯ�ߐݒ�
	int pOldSMode = m_scdc.SetStretchBltMode(COLORONCOLOR);				// �L�k���[�h�ݒ�
	if(!m_scdc.StretchBlt(0, 0, XSizeP, YSizeP, &m_RndDC,
                          0, 0, XSizeS, YSizeS, SRCCOPY)) {				// ��ذ�->���DC�L�k��߰
		esw = 1;
	}
	m_scdc.SetStretchBltMode(pOldSMode);								// �L�kӰ�ޖ߂�
	m_scdc.SelectObject(pOldBitmap);									// ���DC->�ޯ�ϯ�ߐؗ�
	if(esw) {
		AfxMessageBox(IDS_PRTERR);										// �װү���ތ㒆�~
		return;
	}
	HBITMAP hbitmap = (HBITMAP)m_bitmap.GetSafeHandle();				// �ޯ�ϯ������َ擾
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = 24;	// (WORD)(bm.bmPlanes*bm.bmBitsPixel);
	bi.biSize = sizeof(BITMAPINFOHEADER);								// DIBͯ�ް�쐬
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
	if(biBits==1) {														// ��گĻ��ތv�Z
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
	m_scdc.DeleteDC();													// ���DC�폜
	m_bitmap.DeleteObject();											// ��ذ݌݊��ޯ�ϯ�ߍ폜
}

/***********************************************/
void CRendDlg::OutputPrinter(CDC* pDC, int pos)							//<<< DIB ������o��
/***********************************************/
{
	HDC   hdc;
	LPSTR lpDIBHdr;														// Pointer to BITMAPINFOHEADER
	LPSTR lpDIBBits;													// Pointer to DIB bits
	int   sx, sy, px=0, py=0, bSuccess=0;

	if(m_hDIB==NULL) {
		return;
	}
	if(!pos) {															// <�p�������ʒu����̏ꍇ>
		sx = pDC->GetDeviceCaps(HORZRES);								// X�����������
		sy = pDC->GetDeviceCaps(VERTRES);								// Y�����������
		px = (sx>XSizeP) ? (sx-XSizeP)/2 : px;							// X��������ʒu
		py = (sy>YSizeP) ? (sy-YSizeP)/2 : py;							// Y��������ʒu
	}
	lpDIBHdr  = (LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	lpDIBBits = lpDIBHdr + *(LPDWORD)lpDIBHdr + PalSize;
	HPALETTE hpal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
	hdc  = pDC->m_hDC;
	hpal = ::SelectPalette(hdc, hpal, TRUE);							// ������گĕύX
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
		AfxMessageBox(IDS_PRTERR);										// �𑜓x��������
	}
}

/***************************************/
void CRendDlg::OutputFile(CString path)									//<<< DIB ̧�ُo��
/***************************************/
{
	CString ext;
	BOOL    ret;
	int     cmp;

	if(m_hDIB!=NULL) {
		ext = path.Right(3);											// ̧�ق̊g���q
		if(!ext.CompareNoCase("jpg")) {									// <JPEG�`��>
			cmp = View->GetCapJComp();									// ������JPEG���k���擾
			ret = IKJpegFileSave(path, m_hDIB, 0, cmp, 2,
                                 "JCAD3 GLB Output Picture",
                                 0, NULL, NULL, NULL);					// JPEĢ�ُo��
		} else if(!ext.CompareNoCase("bmp")) {							// <�ޯ�ϯ�ߌ`��>
			ret = IKBmpFileSave(path, m_hDIB, FALSE,
                                0, NULL, NULL, NULL);					// �ޯ�ϯ��̧�ُo��
		}
		::GlobalFree((HGLOBAL)m_hDIB);
		if(!ret) {
			AfxMessageBox(IDS_ERR_FILEWRITE);							// ̧�ٴװ
		}
	}
}

/****************************/
void CRendDlg::FreeDIB(void)											//<<< DIB�̈���
/****************************/
{
	::GlobalFree((HGLOBAL)m_hDIB);
}

/*****************************/
void CRendDlg::CheckBtn(void)											//<<< ���ݎg�p��
/*****************************/
{
	CString btn;

	if(RunSW) {
		btn.LoadString(IDS_STR70);										// ����~�����
	} else {
		btn.LoadString(IDS_STR71);										// ��I�������
	}
	m_Button2.SetWindowText(btn);										// ���ݖ��ύX
	m_Button1.EnableWindow(!RunSW);										// ����s����ݗL��/����
	m_Button2.EnableWindow(!StpSW);										// ��I��/���~����ݗL��/����
	m_Button3.EnableWindow(!RunSW&&PaintSW);							// ��������ݗL��/����
	m_Button4.EnableWindow(!RunSW);										// ����ޕύX����ݗL��/����
}

/******************************************/
void CRendDlg::GetRendSize(int* x, int* y)								//<<< �`�滲�ގ擾
/******************************************/
{
	*x = XSizeS, *y = YSizeS;
}

/******************************/
int CRendDlg::CheckStpSW(void)											//<<< ���~SW����
/******************************/
{
	return StpSW;
}

/**************************************************************/
void CRendDlg::SetRendColor(int x, int y, int r, int g, int b)			// ��ذݕ`��
/**************************************************************/
{
	CRect rct;
	int   xs, ys;

	if(x<0) {															// ڲ��ڰ�ݸޏI���̏ꍇ
		RunSW = 0, StpSW = 0;											// �����s, ���~���w��
		PaintSW = 1;													// �`�抮��
		CheckBtn();														// ���ݎg�p��
	} else {															// ڲ��ڰ�ݸޒ��̏ꍇ
		Line = y;														// �`��ςݍs�ۑ�
		m_RndDC.SetPixelV(x, y, RGB(r, g, b));							// ���DC���߸�ق�ݒ�
		xs = (XSizeS<XSize) ? XSizeS : XSize;							// �����ݸގ��ƌ��݂̻���
		ys = (YSizeS<YSize) ? YSizeS : YSize;							// ���珬���������g�p
		rct.left = m_RtD.left;
		rct.top = m_RtD.top;
		rct.right = m_RtD.left+xs;
		rct.bottom = m_RtD.top+ys;
		RedrawWindow(&rct, NULL, RDW_INVALIDATE);						// ��ذݍĕ`��
	}
}
