/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW���암�v���g�^�C�v�錾
// EditObject.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/08 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputCopyNum(int*, PNTTYPE*, int*, SCRTYPE*);						// ���ʏ�����
void TrackerScale1(BOXTYPE, PNTTYPE, PNTTYPE*);							// ػ��ޗp�ׯ���擾1
void TrackerScale2(int, BOXTYPE, PNTTYPE, PNTTYPE*, int*, int*);		// ػ��ޗp�ׯ���擾2
void TrackerBend(BOXTYPE, PNTTYPE, PNTTYPE*, int*);						// �~���Ȃ��p�ׯ���擾
void TrackerBox3D(BOXTYPE, PNTTYPE*);									// �ޯ��3D�p�ׯ���擾
void TrackerBox2D(int, BOXTYPE, PNTTYPE*, int*);						// �ޯ��2D�p�ׯ���擾
void TrackerBoxCrs(BOXTYPE, PNTTYPE, PNTTYPE*);							// �ޯ���\���p�ׯ���擾
void TrackerBoxCrs2(BOXTYPE, PNTTYPE, PNTTYPE*);						// �ޯ���\���p�ׯ���擾2
BOOL GetObjCrossPnt(int, PNTTYPE, PNTTYPE*, VECTYPE*, double*);			// ���̌�_�擾

//=============================<EOF:EditObject.h>============================
