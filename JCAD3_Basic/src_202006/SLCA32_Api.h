///////////////////////////////////////////////////////////////////////////////
// エクスポート SLC プロトタイプ宣言
// SLCA32_Api.h
//-----------------------------------------------------------------------------
// LastEdit: 2010/03/26 by M.Fukushima
///////////////////////////////////////////////////////////////////////////////
#ifndef ___SLCA_H
#define ___SLCA_H

int WINAPI MakeSLCFile(OBJTYPE* op, PNTTYPE* vtx, int* flp,
                       const char* fname, const char* pname);			// エクスポートSLC
int WINAPI ChgSLCFile(const char* inf, const char* outf, BOXTYPE* box);	// SLCファイル変換

#endif ___SLCA_H

//============================<EOF:SLCA32_Api.h>=============================