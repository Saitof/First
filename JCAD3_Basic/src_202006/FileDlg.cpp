/////////////////////////////////////////////////////////////////////////////
// ファイル ダイアログクラス(CFileDlg)定義
// FileDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/06 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg

IMPLEMENT_DYNAMIC(CFileDlg, CFileDialog)

CFileDlg::CFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize)
//                   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize, BOOL bVistaStyle)
		: CFileDialog(bOpenFileDialog, (LPCTSTR)lpszDefExt, (LPCTSTR)lpszFileName, dwFlags,
                      (LPCTSTR)lpszFilter, pParentWnd, dwSize)
//                      (LPCTSTR)lpszFilter, pParentWnd, dwSize, bVistaStyle)
{
}

BEGIN_MESSAGE_MAP(CFileDlg, CFileDialog)
END_MESSAGE_MAP()
