/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW�R�}���h�t���s(�߂�)���v���g�^�C�v�錾
// BackCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void BackMove(int, PNTTYPE*, PNTTYPE*, double, double, double);			// �t���s�ړ�
void BackRotate(int, PNTTYPE*, PNTTYPE*, PNTTYPE, PNTTYPE, double);		// �t��]�ړ�
void BackMirror(int, PNTTYPE*, PNTTYPE*, PNTTYPE, PNTTYPE, int);		// �t�����ړ�
void BackScale(int, PNTTYPE*, PNTTYPE*, PNTTYPE, double, double,
               double);													// �t���ޕύX
void BackResize1(int, PNTTYPE*, PNTTYPE*, int, int, double, double,
                 double, PNTTYPE, PNTTYPE, PNTTYPE);					// �t�����w��ػ���1
void BackResize2(int, PNTTYPE*, PNTTYPE*, int, int, int, int, double,
                 PNTTYPE, int, PNTTYPE*, int*);							// �t�Ȑ��w��ػ���1
void BackBend(int, PNTTYPE*, PNTTYPE*, int, int, double, PNTTYPE,
              PNTTYPE);													// �t�~���Ȃ�
void BackTwist(int, PNTTYPE*, PNTTYPE*, int, double, double, PNTTYPE);	// �t�Ђ˂�
void BackShear(int, PNTTYPE*, PNTTYPE*, int, int, double, PNTTYPE);		// �t����f
void BackResize3(int, PNTTYPE*, PNTTYPE*, int, int,
                 double, double, double, int, double,
                 double, PNTTYPE, PNTTYPE, PNTTYPE);					// �t�����w��ػ���2
void BackResize4(int, PNTTYPE*, PNTTYPE*, int, int, int,
                 int, int, double, int, double, double,
                 PNTTYPE, int, PNTTYPE*, int*);							// �t�Ȑ��w��ػ���2

//============================<EOF:BackCommand.h>============================