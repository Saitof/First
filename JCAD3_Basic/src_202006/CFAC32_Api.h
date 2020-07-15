///////////////////////////////////////////////////////////////////////////////
// JCAD3���ʃt�@�C���A�N�Z�X�v���g�^�C�v�錾
// ICFAC32_Api.h
//-----------------------------------------------------------------------------
// LastEdit: 2009/11/03 by M.Fukushima
///////////////////////////////////////////////////////////////////////////////
#ifndef ___CFAC_H
#define ___CFAC_H

int WINAPI InpDXFParam(int* ver, int* out, int* fac);					// DXF���Ұ�����
int WINAPI InpSTLParam(int* typ, int* fac);								// STL���Ұ�����
int WINAPI ChgWDXFFile(const char* fname, const char* tname,
                       int ver, int out);								// DXF̧�ٕϊ�(Write)
int WINAPI ChgWSTLFile(const char* fname, const char* tname,
                       int typ);										// STĻ�ٕϊ�(Write)
int WINAPI ChgRDXFFile(const char* fname, const char* tname);			// DXF̧�ٕϊ�(Read)
int WINAPI ChgRSTLFile(const char* fname, const char* tname);			// STĻ�ٕϊ�(Read)

#endif ___CFAC_H

//============================<EOF:CFAC32_Api.h>=============================