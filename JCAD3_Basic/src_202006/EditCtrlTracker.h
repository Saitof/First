/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW�g���b�J�[���암�v���g�^�C�v�錾
// EditCtrlTracker.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void GetMoveValue(int, PNTTYPE, PNTTYPE*);								// �ׯ���ړ��ʎ擾
void SetParsTracker(int, int*, int, int*, PNTTYPE*, int*, int,
                    PNTTYPE*, int*, int*);								// �����}�ׯ���ݒ�
void DispParsTracker(int, int, PNTTYPE*, int*, int, PNTTYPE*,
                     int*, int);										// �����}�ׯ���\��
void HitCtrlTracker1(UINT, const CPoint&, int, int, PNTTYPE, PNTTYPE*,
                     int, PNTTYPE*, int*, int, int*, int*, int*, int*);	// �S�ׯ������1
void HitCtrlTracker2(UINT, const CPoint&, int, int, PNTTYPE, PNTTYPE*,
                     int, int, int*, int*, int*);						// �S�ׯ������2
void DragCtrlTracker1(int, BOXTYPE, PNTTYPE*, int, PNTTYPE*, int*,
                      int, int*, int*, int*);							// �ׯ����ׯ��1
void DragCtrlTracker2(int, BOXTYPE, PNTTYPE*, int, int*, int*);			// �ׯ����ׯ��2
void SetLineNo1(int, int, int, int, int*);								// ײݔԍ��ݒ�1
void SetLineNo2(int, int, int, int, int*);								// ײݔԍ��ݒ�2
void SetLineNo3(int, int, int*, OBJTYPE*);								// ײݔԍ��ݒ�3
void PickCPoint(PNTTYPE, int, int, PNTTYPE*, int, int, int, int, int,
                int, int, int*, int*);									// �Ώې���_�I��
void CheckCLine(int, int, int, int, PNTTYPE*, int*, int*);				// ����_��m�F
void TrackerCLine(int, int, int, int, PNTTYPE*, int, int, int*,
                  PNTTYPE*, int*);										// ����_���ׯ���擾
void PickCLine(int, int, int, int, int, int, int, int, PNTTYPE*,
               int*, int*);												// �Ώې���_��I��
void TrackerCInsert(int, int, int, int, int, int, PNTTYPE*, int*,
                    PNTTYPE*, int*);									// ����_���ׯ���擾
void TrackerULine(int, int, int, PNTTYPE*, int, int, int*, PNTTYPE*,
                  int*);												// ����_���ׯ���擾
void TrackerVLine(int, int, PNTTYPE*, int, int, int, int*, PNTTYPE*,
                  int*);												// ����_���ׯ���擾
void SelAreaCtrl(int, BOXTYPE, int, PNTTYPE*, int*, int);				// ����_�I��(�ر)
void GetBasePoint(int, int, PNTTYPE*, int, PNTTYPE*, VECTYPE*);			// ��_�擾

//==========================<EOF:EditCtrlTracker.h>==========================