/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW�R�}���h���v���g�^�C�v�錾
// ECommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void Move(double, double, double);										// ���s�ړ�
void ObjMove(OBJTYPE*, double, double, double);							// �w�藧�̂̈ړ�
void ObjectMove(int, int, PNTTYPE, SCRTYPE, double, double, double);	// ���̕��s�ړ�
void EndMove(int, PNTTYPE, int, SCRTYPE, double, double, double);		// ���s�ړ��I������
void Rotate(PNTTYPE, VECTYPE, double);									// ��]�ړ�
void ObjRotate(OBJTYPE*, PNTTYPE, VECTYPE, double);						// �w�藧�̂̉�]
void ObjectRotate(int, int, PNTTYPE, SCRTYPE, PNTTYPE, int,
                  double, double);										// ���̉�]�ړ�
void EndRotate(int, PNTTYPE, int, SCRTYPE, PNTTYPE, int,
               double, double);											// ��]�ړ��I������
void ObjectMirror(int, int, PNTTYPE, PNTTYPE, int);						// ���̋����ړ�
void Scale(PNTTYPE, double, double, double);							// ���ޕύX
void ObjectScale(int, PNTTYPE, double, double, double);					// ���̻��ޕύX
void Resize1(int, int, double, double, double, PNTTYPE,
             PNTTYPE, PNTTYPE);											// �����w��ػ���1
void ObjectResize1(int, int, int, double, double, double,
                   PNTTYPE, PNTTYPE, PNTTYPE);							// ���̒����w��ػ���1
void Resize2(int, int, int, int, double, PNTTYPE, int,
             PNTTYPE*, int*);											// �Ȑ��w��ػ���2
void ObjectResize2(int, int, int, int, int, double, PNTTYPE,
                   int, PNTTYPE*, int*);								// ���̋Ȑ��w��ػ���2
void Bend(int, int, double, PNTTYPE, PNTTYPE);							// �~���Ȃ�
void ObjectBend(int, int, int, double, PNTTYPE, PNTTYPE);				// ���̉~���Ȃ�
void Twist(int, double, double, PNTTYPE);								// �Ђ˂�
void ObjectTwist(int, int, double, double, PNTTYPE);					// ���̂Ђ˂�
void Shear(int, int, double, PNTTYPE);									// ����f
void ObjectShear(int, int, int, double, PNTTYPE);						// ���̂���f
void Ring(PNTTYPE, double, double, double, int, int, double,
          PNTTYPE, PNTTYPE);											// �ݸ޻��ލ��킹
void Earth(int, int, int, double, double, double, double,
           double, double, double, double, double);						// ���
void ObjectEarth(int, int, int, int, int, double, double, double,
                 double, double, double, double, double, double);		// ���̱��
void Space(int, int, int, double, double, double, double,
           double, double, double, double, double);						// ��߰�
void ObjectSpace(int, int, int, int, int, double, double, double,
                 double, double, double, double, double, double);		// ���̽�߰�
void Resize3(int, int, double, double, double, int,
             double, double, PNTTYPE, PNTTYPE, PNTTYPE);				// �����w��ػ���2
void ObjectResize3(int, int, int, double, double, double, int,
                   double, double, PNTTYPE, PNTTYPE, PNTTYPE);			// ���̒����w��ػ���2
void Resize4(int, int, int, int, int, double, int, double,
			 double, PNTTYPE, int, PNTTYPE*, int*);						// �Ȑ��w��ػ���2
void ObjectResize4(int, int, int, int, int, int, double, int,
                   double, double, PNTTYPE, int, PNTTYPE*, int*);		// ���̋Ȑ��w��ػ���2

//==============================<EOF:ECommand.h>=============================