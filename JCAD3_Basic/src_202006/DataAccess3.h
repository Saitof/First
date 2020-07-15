/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암�v���g�^�C�v�錾
// DataAccess3.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/24 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS3_H
#define ___DATAACCESS3_H

#include "DataType.h"

double ChangeFltToDbl(float);											// float => double
void GetObjBottom(double*);												// �\���������ю擾
void SetRayTraceEnv(COLORREF, int, int, int, int, double*);				// ڲ��ڰ����ݒ�
void GetRayTraceEnv(COLORREF*, int*, int*, int*, int*, double*);		// ڲ��ڰ����擾
void InitRayTraceEnvFile(void);											// ����ڲ��ڰ���̧�ٓǍ�
void GetObjCent(double*);												// �\�����̾����擾
void SetRayTraceLight(int, LIGHT);										// ڲ��ڰ������ݒ�
void GetRayTraceLight(int, LIGHT*);										// ڲ��ڰ������擾
void InitRayTraceLightFile(void);										// ����ڲ��ڰ�����̧�ٓǍ�
void SetRayTraceColor(int, ATTRIBT);									// ڲ��ڰ��\�ʑ����ݒ�
void GetRayTraceColor(int, ATTRIBT*);									// ڲ��ڰ��\�ʑ����擾
void InitRayTraceColorFile(void);										// ����ڲ��ڰ��װ̧�ٓǍ�
void ReadRayTraceFile(void);											// ڲ��ڰ��ۑ�̧�ٓǍ�
void WriteRayTraceFile(void);											// ڲ��ڰ��ۑ�̧�ُ���
void CreateObjWork(void);												// ���̍���ް��̈�m��
void SetPovData1(CString, CString, CString, int, double, int,
                 double, double, double, int, double, double,
                 double, int, double, double, double, int, int, int);	// POV-Ray�ް��ݒ�1
void GetPovData1(CString*, CString*, CString*, int*, double*, int*,
                 double*, double*, double*, int*, double*, double*,
                 double*, int*, double*, double*, double*,
                 int*, int*, int*);										// POV-Ray�ް��擾1
void SetPovData2(CString, CString, CString, int, int, int, int);		// POV-Ray�ް��ݒ�2
void GetPovData2(CString*, CString*, CString*, int*, int*, int*, int*);	// POV-Ray�ް��擾2
void SetPovData3(int*, int*, int*, int*, int*, int*);					// POV-Ray�ް��ݒ�3
void GetPovData3(int*, int*, int*, int*, int*, int*);					// POV-Ray�ް��擾3
void SetWDefFile1(CString);												// POV-Rayհ�ޒ�`WĶ�ِݒ�1
void SetWDefFile2(CString);												// POV-Rayհ�ޒ�`WĶ�ِݒ�2
void GetWDefFile(CString*, CString*);									// POV-Rayհ�ޒ�`WĶ�َ擾
void ReadPovDefFile(CString, int*, CString*);							// POV-Rayհ�ޒ�`̧�ٓǍ�
void ReadPovRayFile(void);												// POV-Ray�ۑ�̧�ٓǍ�
void WritePovRayFile(void);												// POV-Ray�ۑ�̧�ُ���
void GetColorTex(int*, int*, int*, int*, int*, char*, char*,
                 CString*, CString*, CString*, CString*);				// �װ��ø����擾
void InitCNVtx(void);													// ���_��ď�������
void SetCNVtx(PNTTYPE, VECTYPE);										// ��Ē��_�ݒ�
BOOL SortCNVtx(int, double);											// ���_����&�ݒ�
void EndCNVtx(void);													// ���_��ďI������
void CreateCmdWork(int, CMDTYPE*);										// ����ލ���ް��̈�m��
void SearchCmdWork(int, CWKTYPE**, CMDTYPE**, OBJTYPE**);				// ����ލ���ް�����
void SetOwkObjPtr(CWKTYPE*, OBJTYPE*);									// ���̍���ް������߲���ݒ�
void ChangeCwkFlg(CWKTYPE*);											// ����ލ���ް��׸ލX�V
void LastObjWorkProc(void);												// ���̍���ް��ŏI����
void FreeObjCmdWork(int);												// ����/����ލ���ް����
void OnCurveReptSW(int);												// �Ȗʍו���SW�I��
void OffCurveReptSW(void);												// �Ȗʍו���SW�I�t
BOOL GetCurveReptSW(void);												// �Ȗʍו���SW�擾
int  GetCurveReptNum(void);												// �Ȗʕ������擾

#endif ___DATAACCESS3_H

//============================<EOF:DataAccess3.h>============================