/////////////////////////////////////////////////////////////////////////////
// �t�@�C�� �_�C�A���O�N���X(CFileDlg)�錾
// FileDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/06 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
class CFileDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(BOOL bOpenFileDialog,	// TRUE �Ȃ�� FileOpen�A FALSE �Ȃ�� FileSaveAs
             LPCTSTR lpszDefExt = NULL,
             LPCTSTR lpszFileName = NULL,
             DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
             LPCTSTR lpszFilter = NULL,
             CWnd* pParentWnd = NULL,
             DWORD dwSize = 0);
//             BOOL bVistaStyle = FALSE);

protected:
	DECLARE_MESSAGE_MAP()
};

//==============================<EOF:FileDlg.h>==============================
