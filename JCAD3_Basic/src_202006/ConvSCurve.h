/////////////////////////////////////////////////////////////////////////////
// �r�Ȑ� ����_<->�ʉߓ_ �ϊ����v���g�^�C�v�錾
// ConvSCurve.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void SetCurveMatrix1(int, double, int, int, int*, double*, double*);	// S�Ȗʕϊ���د���ݒ�1
void SetCurveMatrix2(int, double, int, int, int*, double*, double*);	// S�Ȗʕϊ���د���ݒ�2
void SetCurveMatrix3(int, double, int, int, int*, double*, double*);	// S�Ȗʕϊ���د���ݒ�3
void CalcCurvePoint(int, int, PNTTYPE*, PNTTYPE*, double*);				// ����_<=>�ʉߓ_�v�Z
void CalcSurfacePoint(int, int, int, int, int, int, int, double, int*,
                      PNTTYPE*, PNTTYPE*);								// ����_<=>�ʉߓ_�v�Z

//=============================<EOF:ConvSCurve.h>============================
