/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォントインストラクション部プロトタイプ宣言
// TrueTypeInst.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void SetCodeRange(ExecContext exc, INT rno, void* bas, LONG len);
ExecContext NewContext(FontFileInfo *ffile);
INT RunContext(ExecContext exc);
void DelContext(ExecContext exc);

//============================<EOF:TrueTypeInst.h>===========================
