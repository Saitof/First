/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW����_���암
// EditCtrlPoint1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "BackCommand.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/****************************************************************/
static void MoveCtrlPoint(OBJTYPE* op, PNTTYPE opt, PNTTYPE npt,
                          int mx, int flg, PNTTYPE* pc)					//<<< ����_�ړ�
/****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	CMDTYPE* cmp;
	PNTTYPE  wpt, spt, p1, p2, p3;
	int      i, i1, i2, i3, i4, i5, i6, i7, wf, ct, cno;
	double   d1, d2, d3, d4, d5, d6, d7, d8, d9;

	if(flg==1) {														// <�ړ��Ώۂ̏ꍇ>
		while(mx>0) {
			wpt = opt;													// ���̕ҏW�O����_
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �Ȑ��ҏW������
			ct = 0;														// ����޶���������
			while(cmp!=NULL) {
				if(ct>mx-1) {											// ����ތ��ȏ㒆�~
					break;
				}
				wf = (ct==mx-1) ? 1 : 0;								// �w����ڍX�V�L
				cno = GetCmdNo(cmp);									// �����No�擾
				if(cno==MOVE) {											// <���s�ړ��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					BackMove(wf, &wpt, &npt, d1, d2, d3);				// �t���s�ړ����s
				} else if(cno==ROTATE) {								// <��]�ړ��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// ��]�p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ��]���޸�َ擾
					BackRotate(wf, &wpt, &npt, p1, p2, d1);				// �t��]�ړ����s
				} else if(cno==MIRROR) {								// <�����ړ��̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ώە��ʎ擾
					GetParaPnt(cmp, 0, &p1);							// ��������1�_�擾
					GetParaPnt(cmp, 1, &p2);							// ��������1�_�擾
					BackMirror(wf, &wpt, &npt, p1, p2, i1);				// �t�����ړ����s
				} else if(cno==SCALE) {									// <���ޕύX�̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����{���擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����{���擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ���ޕύX���S�_�擾
					BackScale(wf, &wpt, &npt, p1, d1, d2, d3);			// �t���ޕύX���s
				} else if(cno==RESIZE1) {								// <�����w��ػ���1�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �w�蕽�ʎ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �{��1�擾
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// �{��2�擾
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// �{��3�擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					GetParaPnt(cmp, 1, &p2);							// ��_2�擾
					GetParaPnt(cmp, 2, &p3);							// ��_3�擾
					BackResize1(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                p1, p2, p3);							// �t�����w��ػ���1���s
				} else if(cno==RESIZE2) {								// <�Ȑ��w��ػ���1�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �����׸ގ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					i3 = GetParaInt(cmp, 2);							// ���������擾
					i4 = GetParaInt(cmp, 3);							// ���ގw������擾
					d1 = (double)GetParaInt(cmp, 4)/1000.0;				// ������擾
					i5 = GetParaInt(cmp, 5);							// ����_���擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					for(i=0; i<i5; i++) {
						ia[i] = GetParaInt(cmp, i+6);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i+1, &pa[i]);					// �Ȑ�����_�擾
					}
					BackResize2(wf, &wpt, &npt, i1, i2, i3, i4, d1,
                                p1, i5, pa, ia);						// �t�Ȑ��w��ػ���1���s
				} else if(cno==BEND) {									// <�~���Ȃ��̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ȃ����������擾
					i2 = GetParaInt(cmp, 1);							// �Ȃ����������擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �Ȃ��p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ����_�擾
					GetParaPnt(cmp, 1, &p2);							// ����_�擾
					BackBend(wf, &wpt, &npt, i1, i2, d1, p1, p2);		// �t�~���Ȃ����s
				} else if(cno==TWIST) {									// <�Ђ˂�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ђ˂莲�����擾
					d1 = (double)GetParaInt(cmp, 1)/1000.0;				// �Ђ˂�p�x�擾
					d2 = (double)GetParaInt(cmp, 2)/1000.0;				// �Ђ˂莲���擾
					GetParaPnt(cmp, 0, &p1);							// �Ђ˂��_�擾
					BackTwist(wf, &wpt, &npt, i1, d1, d2, p1);			// �t�Ђ˂���s
				} else if(cno==SHEAR) {									// <����f�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// ����f���������擾
					i2 = GetParaInt(cmp, 1);							// ����f���������擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// ����f�X���擾
					GetParaPnt(cmp, 0, &p1);							// ����_�擾
					BackShear(wf, &wpt, &npt, i1, i2, d1, p1);			// �t����f���s
				} else if(cno==RING) {									// <�ݸނ̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����{���擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����{���擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����{���擾
					i1 = GetParaInt(cmp, 3);							// �Ȃ����������擾
					i2 = GetParaInt(cmp, 4);							// �Ȃ����������擾
					d4 = (double)GetParaInt(cmp, 5)/1000.0;				// �Ȃ��p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ���ޕύX���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ����_�擾
					GetParaPnt(cmp, 2, &p3);							// ����_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V��)
						BackBend(0, &wpt, &npt, i1, i2, d4, p2, p3);	// �t�~���Ȃ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V��)
						BackBend(1, &wpt, &npt, i1, i2, d4, p2, p3);	// �t�~���Ȃ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==EARTH) {									// <����̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X����]�p�x�擾
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y����]�p�x�擾
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z����]�p�x�擾
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X�����{���擾
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y�����{���擾
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ���ޕύX���S�_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==SPACE) {									// <��߰��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X����]�p�x�擾
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y����]�p�x�擾
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z����]�p�x�擾
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X�����{���擾
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y�����{���擾
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ���ޕύX���S�_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==RESIZE3) {								// <�����w��ػ���2�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �w�蕽�ʎ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �{��1�擾
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// �{��2�擾
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// �{��3�擾
					i3 = GetParaInt(cmp, 5);							// ���������׸ގ擾
					d4 = (double)GetParaInt(cmp, 6)/1000.0;				// �����͈�1�擾
					d5 = (double)GetParaInt(cmp, 7)/1000.0;				// �����͈�2�擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					GetParaPnt(cmp, 1, &p2);							// ��_2�擾
					GetParaPnt(cmp, 2, &p3);							// ��_3�擾
					BackResize3(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                i3, d4, d5, p1, p2, p3);				// �t�����w��ػ���2���s
				} else if(cno==RESIZE4) {								// <�Ȑ��w��ػ���2�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �����׸ގ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					i3 = GetParaInt(cmp, 2);							// ���������擾
					i4 = GetParaInt(cmp, 3);							// ���ގw������擾
					i5 = GetParaInt(cmp, 4);							// ���������擾
					d1 = (double)GetParaInt(cmp, 5)/1000.0;				// ������擾
					i6 = GetParaInt(cmp, 6);							// ���������׸ގ擾
					d2 = (double)GetParaInt(cmp, 7)/1000.0;				// �����͈�1�擾
					d3 = (double)GetParaInt(cmp, 8)/1000.0;				// �����͈�2�擾
					i7 = GetParaInt(cmp, 9);							// ����_���擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					for(i=0; i<i7; i++) {
						ia[i] = GetParaInt(cmp, i+10);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i+1, &pa[i]);					// �Ȑ�����_�擾
					}
					BackResize4(wf, &wpt, &npt, i1, i2, i3, i4, i5,
                                d1, i6, d2, d3, p1, i7, pa, ia);		// �t�Ȑ��w��ػ���2���s
				}
				ct++;													// ����޶���UP
				NextCmdPtr(&cmp);										// ������ނ�
			}
			mx--;														// ����ތ����Z
		}
		*pc = npt;
	} else {															// <�ړ��ΏۊO�̏ꍇ>
		*pc = opt;
	}
	delete[] pa;
	delete[] ia;
}

/***************************************************************/
void MoveCPoint(int md, PNTTYPE* ma, OBJTYPE* op, int un, int vn,
                int ub, int vb, double wt, int* ia, int* ib,
                PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)			//<<< ����_�ړ�
/***************************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxSCtA];
	int*     flg = new int[View->m_MaxSCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  zro, opt, npt;
	int      i, j, k, l, ten, pn, mx=0, chk=1, p1, p2, ps, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	pn = un*vn;															// ������_��
	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ�
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	zro.p[0] = 0.0, zro.p[1] = 0.0, zro.p[2] = 0.0;						// ��Ɨp���W���_
	for(i=0; i<pn; i++) {
		if(ib[i]) {														// �ړ���/�׸ޔz�񕡎�
			mov[i] = ma[i], flg[i] = 1;
		} else {
			mov[i] = zro,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<���͐ݒ�L�̏ꍇ>>
		for(i=0; i<vn; i++) {
			for(j=0, sct=0; j<un-1; j++) {								// <U���������m�F>
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// �w��_���E����
					sct++;
				}
			}
			if(sct>2) {													// ��������NG
				chk = 0; break;
			}
		}
		if(chk) {
			for(i=0; i<un; i++) {
				for(j=0, sct=0; j<vn-1; j++) {							// <V���������m�F>
					if(flg[i+un*j]!=flg[i+un*(j+1)]) {					// �w��_���E����
						sct++;
					}
				}
				if(sct>2) {												// ��������NG
					chk = 0; break;
				}
			}
		}
	} else {															// ���͐ݒ薳NG
		chk = 0;
	}
	if(chk) {															// <<��������OK>>
		for(i=0; i<vn; i++) {											// <U�����ړ��ʍX�V>
			for(j=0, sct=0, p1=-1, p2=-1; j<un-1; j++) {
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// <�O���׸ވ�>
					sct++;												// �w��_���E����
					if(flg[un*i+j]) {									// ���E1�ʒu
						p1 = j+1;
					} else {											// ���E2�ʒu
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <�w��_�A��>
				for(j=0; j<un; j++) {									// ����_�ԋ�������
					len[j] = (j==un-1) ? PntD(&pb[un*(i+1)-1], &pb[un*i])
                                       : PntD(&pb[un*i+j], &pb[un*i+j+1]);
					wta[j] = ta[un*i+j];								// ���͌����񒊏o
				}
				if(p1==-1) {											// �w��_�O��ʒu&�_���ݒ�
					if(ub) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = un-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = un-1;
					}
				} else if(p2==-1) {
					if(ub) {
						sct = (un-p1)/2, ect = (un-p1)/2;
						sno = un-1, eno = p1-1;
					} else {
						sct = 0, ect = un-p1;
						sno = un-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(ub) {
						sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = un-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <�w��_�O����_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k-1<0) ? un-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				lst = (lst-1<0) ? un-1 : lst-1;							// �L���_�O�������܂�
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln ;						// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k-1<0) ? un-1 : k-1;
				}
				ps = (sno+1>un-1) ? 0 : sno+1;							// ��ړ��ʔԒn
				ps = un*i+ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// �w�萧��_����̋���
					l = un*i+k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
					k = (k-1<0) ? un-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <�w��_�㐧��_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k+1>un-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>un-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k+1>un-1) ? 0 : k+1;
				}
				ps = un*i+eno;											// ��ړ��ʔԒn
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// �w�萧��_����̋���
					k = (k+1>un-1) ? 0 : k+1;
					l = un*i+k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
				}
			}
		}
		for(i=0; i<un; i++) {											// <V�����ړ��ʍX�V>
			for(j=0, sct=0, p1=-1, p2=-1; j<vn-1; j++) {
				if(flg[i+un*j]!=flg[i+un*(j+1)]) {						// <�O���׸ވ�>
					sct++;												// �w��_���E����
					if(flg[i+un*j]) {									// ���E1�ʒu
						p1 = j+1;
					} else {											// ���E2�ʒu
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <�w��_�A��>
				for(j=0; j<vn; j++) {									// ����_�ԋ�������
					len[j] = (j==vn-1) ? PntD(&pb[i+un*(vn-1)], &pb[i])
                                       : PntD(&pb[i+un*j], &pb[i+un*(j+1)]);
					wta[j] = ta[i+un*j];								// ���͌����񒊏o
				}
				if(p1==-1) {											// �w��_�O��ʒu&�_���ݒ�
					if(vb) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = vn-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = vn-1;
					}
				} else if(p2==-1) {
					if(vb) {
						sct = (vn-p1)/2, ect = (vn-p1)/2;
						sno = vn-1, eno = p1-1;
					} else {
						sct = 0, ect = vn-p1;
						sno = vn-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(vb) {
						sct = (p2+1+vn-p1)/2, ect = (p2+1+vn-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = vn-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <�w��_�O����_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k-1<0) ? vn-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				lst = (lst-1<0) ? vn-1 : lst-1;							// �L���_�O�������܂�
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k-1<0) ? vn-1 : k-1;
				}
				ps = (sno+1>vn-1) ? 0 : sno+1;							// ��ړ��ʔԒn
				ps = i+un*ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// �w�萧��_����̋���
					l = i+un*k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
					k = (k-1<0) ? vn-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <�w��_�㐧��_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>vn-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ps = i+un*eno;											// ��ړ��ʔԒn
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// �w�萧��_����̋���
					k = (k+1>vn-1) ? 0 : k+1;
					l = i+un*k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = ma[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i].p[j];									// ���̕ҏW&�ړ�����W
		}
	}
	CalcSurfacePoint(md, 0, 0, un, vn, ub, vb, wt, ia, mov, cpt);		// �ʉߓ_=>����_�ϊ�
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// �ʉߓ_�͑S�Ĉړ��ς�
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// �ړ��O/�ړ��� ����_
		MoveCtrlPoint(op, opt, npt, mx, flg[i], &pc[i]);				// ����_�ړ�
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] wta;
	delete[] len;
}

/*********************************************************************/
static void TwistPoint(PNTTYPE cpt, VECTYPE vec, double th,
                       int ix, int mx, OBJTYPE* op, int un,
                       int vn, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc)	//<<< ����_�Ђ˂�
/*********************************************************************/
{
	PNTTYPE opt, npt;
	int     i, j;
	double  rot[3][3];

	if(fabs(th)>EPSILON&&												// <<�p�x�L�̏ꍇ>>
       (fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON||fabs(vec.p[2])>EPSILON)) {
		SetRotateMatrix(vec, th, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {
			opt = pa[ix+i*un];											// ���̕ҏW�O����_
			for(j=0; j<DIM; j++) {
				npt.p[j] = pb[ix+i*un].p[j]-cpt.p[j];					// ���̕ҏW&�ړ��㐧��_
			}
			VecRotateMove(&npt, rot, cpt);								// ��]�ړ�
			MoveCtrlPoint(op, opt, npt, mx, 1, &pc[ix+i*un]);			// ����_�ړ�
		}
	} else {															// <<�p�x���̏ꍇ>>
		for(i=0; i<vn; i++) {
			pc[ix+i*un] = pa[ix+i*un];									// ����_����
		}
	}
}

/***********************************************************************/
void TwistCLine(PNTTYPE* cpt, VECTYPE* vec, double th, int bn,
                int* sa, int sn, OBJTYPE* op, int un, int vn, int ub,
                int sf, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)	//<<< ����_�Ђ˂�
/***********************************************************************/
{
	int*     wta = new int[View->m_MaxCCtA];
	double*  uln = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, k, ct, ix, sx, ex, ten, mx=0, lst, ok;
	double   sln, wln, ft, wt;

	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ��擾
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	ct = un*vn;
	for(i=0; i<ct; i++) {
		pc[i] = pa[i];													// ����_������
	}
	for(i=0; i<un; i++) {
		uln[i] = (i==un-1) ? PntD(&cpt[un-1], &cpt[0])
                           : PntD(&cpt[i], &cpt[i+1]);					// ���S�_�ԋ�������
		for(ok=0, j=0; j<vn; j++) {										// ���͗L���m�F
			if(ta[i+un*j]) {											// 1�_�ł��L����OK
				ok = 1; break;
			}
		}
		wta[i] = ok;
	}
	for(i=0, j=0, wt=0.0; i<DIM; i++) {									// ��f�ʂ�
		if(wt<fabs(vec[bn].p[i])) {										// �޸�ٍő��������
			wt = fabs(vec[bn].p[i]), j = i;
		}
	}
	if(vec[bn].p[j]>0.0) {												// ��f�ʂ��޸��
		for(i=0; i<un; i++) {											// �ő���������̏ꍇ
			vec[i].p[0]*=-1.0, vec[i].p[1]*=-1.0, vec[i].p[2]*=-1.0;	// �@���޸�ٕ␳
		}
	}
	for(i=0, sx=-1; i<sn; i++) {
		if(sa[i]) {
			ix = (sf%2) ? i+1 : i;
			sx = (sx==-1) ? ix : sx;									// �J�n�f�ʕۑ�
			ex = ix;													// �I���f�ʕۑ�
			ft = th*PI/180.0;
			TwistPoint(cpt[ix], vec[ix], ft, ix, mx, op, un, vn,
                       pa, pb, pc);										// �w��f�ʐ���_�Ђ˂�
		}
	}
	if(ub) {															// <U:�̏ꍇ>
		for(i=0, ct=0; i<sn; i++) {
			if(sa[i]) {													// �擪�̎w���ς�
				ix = i;
				for(j=0, k=0; j<sn; j++) {
					ix = (ix+1>sn-1) ? 0 : ix+1;
					if(sa[ix]) {										// �Ō�̎w���ς�
						break;
					} else {											// ���Ԃ̖��w������
						k++;
					}
				}
				if(k>ct) {												// �Œ��̖��w���̈�
					ct = k, sx = ix, ex = i;
				}
			}
		}
		sx = (sf%2) ? sx+1 : sx;										// �J�n�f�ʕۑ�
		ex = (sf%2) ? ex+1 : ex;										// �I���f�ʕۑ�
		ct = ct/2;														// �L���f�ʐ�(�w��f�ʑO/�㕪)
	}
	if(ten>0) {															// <<<���͐ݒ�L�̏ꍇ>>>
		ct = (!ub) ? sx : ct;											// <<�w��f�ʑO�Ђ˂�>>
		for(i=0, j=sx, lst=-1; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			lst = (wta[j]) ? j : lst;									// �ŏI���͗L���_����
		}
		ok = (lst==-1) ? 0 : 1;											// ���͗L���_�L��
		lst = (lst-1<0) ? un-1 : lst-1;									// �L���_�O�������܂�
		for(i=0, sln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			sln = (ok) ? sln+uln[j] : sln;								// �w��ʑO���S�������v
			ok = (j==lst) ? 0 : ok;										// �ŏI���͗L���_�ȍ~�͖���
		}
		for(i=0, wln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			wln += uln[j];												// �w��f�ʂ���̒��S����
			if(wta[j]) {												// <���͗L���f��>
				wt = ft;												// �w��f�ʂЂ˂�p�x
				GetTensionFltValue(ten, sln, wln, &wt);					// ���͐ݒ�l�ϊ�
				TwistPoint(cpt[j], vec[j], wt, j, mx, op, un, vn,
                           pa, pb, pc);									// ����_�Ђ˂�
			}
		}
		ct = (!ub) ? un-ex-1 : ct;										// <<�w��f�ʌ�Ђ˂�>>
		for(i=0, j=ex, lst=-1; i<ct; i++) {
			lst = (wta[j]) ? j : lst;									// �ŏI���͗L���_����
			j = (j+1>un-1) ? 0 : j+1;
		}
		ok = (lst==-1) ? 0 : 1;											// ���͗L���_�L��
		for(i=0, sln=0.0, j=ex; i<ct; i++) {
			k = (j+1>un-1) ? 0 : j+1;
			sln = (ok||wta[k]) ? sln+uln[j] : sln;						// �w��ʌ㒆�S�������v
			ok = (j==lst) ? 0 : ok;										// �ŏI���͗L���_�ȍ~�͖���
			j = (j+1>un-1) ? 0 : j+1;
		}
		for(i=0, wln=0.0, j=ex; i<ct; i++) {
			wln += uln[j];												// �w��f�ʂ���̒��S����
			j = (j+1>un-1) ? 0 : j+1;
			if(wta[j]) {												// <���͗L���f��>
				wt = ft;												// �w��f�ʂЂ˂�p�x
				GetTensionFltValue(ten, sln, wln, &wt);					// ���͐ݒ�l�ϊ�
				TwistPoint(cpt[j], vec[j], wt, j, mx, op, un, vn,
                           pa, pb, pc);									// ����_�Ђ˂�
			}
		}
	}
	delete[] wta;
	delete[] uln;
}

/*****************************************************************/
static void ScalePoint(VECTYPE vec, int vd, int fg, PNTTYPE bpt,
                       int bn, int dn, double sc, int ix, int mx,
                       OBJTYPE* op, int un, int vn, PNTTYPE* pa,
                       PNTTYPE* pb, PNTTYPE* pc)						//<<< ����_�g��/�k��
/*****************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  cpt, opt, npt;
	VECTYPE  rvc;
	int      i, i1, i2, i3, j1, j2, chk=0;
	double   rot[3][3], fln, rth, mn1, mn2, mx1, mx2, dd;

	if(fabs(sc-1.0)>EPSILON&&											// <<�{���L�̏ꍇ>>
       (fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON||fabs(vec.p[2])>EPSILON)) {
		for(i=0, i1=0; i<vn; i++) {
			pt[i1++] = pb[ix+i*un];										// �Ώ�V�f�ʒ��o
		}
		i1 = (vd+1>2) ? 0 : vd+1;										// ���W���ݒ�
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		if(vd==0) {
			j1 = 1, j2 = 2;
		} else if(vd==1) {
			j1 = 0, j2 = 2;
		} else {
			j1 = 1, j2 = 0;
		}
		fln = sqrt(vec.p[i1]*vec.p[i1]+vec.p[i2]*vec.p[i2]);			// ��ʕ�������
		if(fln>EPSILON) {												// <��ʂɂȂ��ꍇ>
			rvc.p[i1] = -vec.p[i2], rvc.p[i2] = vec.p[i1], rvc.p[i3] = 0.0;
			VecN(&rvc);
			rth = atan2(fln, vec.p[i3]);								// ��]�p�x�Z�o
			rth = (rth<-10||rth>10) ? 0.0 : rth;
			rth = -rth;
			rth = (fg) ? rth-PI : rth;									// ��ʂƂ̌����Ⴂ
			SetRotateMatrix(rvc, rth, rot);								// ��]��د���쐬
			for(i=0; i<vn; i++) {
				VecRotate(&pt[i], rot);									// ��ʂɉ�]
			}
			chk = 1;													// ���s�׸�ON
		} else if((fg&&vec.p[vd]>0.0)||((!fg)&&vec.p[vd]<0.0)) {		// <��_�Ɣ��]�̏ꍇ>
			if(fabs(bpt.p[0])>EPSILON||
               fabs(bpt.p[1])>EPSILON||
               fabs(bpt.p[2])>EPSILON) {
				rvc.p[i1] = -bpt.p[i2];									// ��]���ݒ�
				rvc.p[i2] = bpt.p[i1];
				rvc.p[i3] = 0.0;
				VecN(&rvc);
				rth = -PI;												// ��]�p�x���]
				SetRotateMatrix(rvc, rth, rot);							// ��]��د���쐬
				for(i=0; i<vn; i++) {									// ��ʂɉ�]
					VecRotate(&pt[i], rot);
				}
				chk = 1;												// ���s�׸�ON
			}
		}
		mn1 = pt[0].p[j1], mx1 = pt[0].p[j1];
		mn2 = pt[0].p[j2], mx2 = pt[0].p[j2];
		for(i=1; i<vn; i++) {
			mn1 = (mn1>pt[i].p[j1]) ? pt[i].p[j1] : mn1;				// ���ʍŏ��ő匟��
			mx1 = (mx1<pt[i].p[j1]) ? pt[i].p[j1] : mx1;
			mn2 = (mn2>pt[i].p[j2]) ? pt[i].p[j2] : mn2;
			mx2 = (mx2<pt[i].p[j2]) ? pt[i].p[j2] : mx2;
		}
		if(vd==1) {														// �ŏ�<->�ő����
			dd = mn1, mn1 = mx1, mx1 = dd;
		} else if(vd==2) {
			dd = mn2, mn2 = mx2, mx2 = dd;
		}
		if(bn==0||bn==3||bn==6) {										// ��_�ݒ�
			cpt.p[j1] = mn1;
		} else if(bn==1||bn==4||bn==7) {
			cpt.p[j1] = (mn1+mx1)/2.0;
		} else {
			cpt.p[j1] = mx1;
		}
		if(bn==6||bn==7||bn==8) {
			cpt.p[j2] = mn2;
		} else if(bn==3||bn==4||bn==5) {
			cpt.p[j2] = (mn2+mx2)/2.0;
		} else {
			cpt.p[j2] = mx2;
		}
		for(i=0; i<vn; i++) {
			if(dn>=1) {
				pt[i].p[j1] -= cpt.p[j1];
				pt[i].p[j1] = pt[i].p[j1]*sc+cpt.p[j1];					// ��_�ړ���,�g��/�k��
			}
			if(dn<=1) {
				pt[i].p[j2] -= cpt.p[j2];
				pt[i].p[j2] = pt[i].p[j2]*sc+cpt.p[j2];
			}
		}
		if(chk) {														// <��]���s�̏ꍇ>
			SetRotateMatrix(rvc, -rth, rot);							// ��]��د���쐬
			for(i=0; i<vn; i++) {
				VecRotate(&pt[i], rot);									// ���ʒu�ɋt��]
			}
		}
		for(i=0; i<vn; i++) {
			opt = pa[ix+i*un];											// ���̕ҏW�O����_
			npt = pt[i];												// ����&�ȖʕҏW�㐧��_
			MoveCtrlPoint(op, opt, npt, mx, 1, &pc[ix+i*un]);			// ����_�ړ�
		}
	} else {															// <<�{�����̏ꍇ>>
		for(i=0; i<vn; i++) {
			pc[ix+i*un] = pa[ix+i*un];									// ����_����
		}
	}
	delete[] pt;
}

/***************************************************************/
void ScaleCLine(PNTTYPE* cpt, VECTYPE* vec, int bn, int dn,
                double sc, int bs, int* sa, int sn,
                OBJTYPE* op, int un, int vn, int ub, int sf,
                PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ta)			//<<< ����_�g��/�k��
/***************************************************************/
{
	int*     wta = new int[View->m_MaxCCtA];
	double*  uln = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  bpt;
	int      i, j, k, l, vd, fg, ct, sx, ex, ten, mx=0, lst, ok;
	double   sln, wln, wsc;

	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ��擾
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	ct = un*vn;
	for(i=0; i<ct; i++) {
		pc[i] = pa[i];													// ����_������
	}
	for(i=0; i<un; i++) {
		uln[i] = (i==un-1) ? PntD(&cpt[un-1], &cpt[0])
                           : PntD(&cpt[i], &cpt[i+1]);					// ���S�_�ԋ�������
		for(ok=0, j=0; j<vn; j++) {										// ���͗L���m�F
			if(ta[i+un*j]) {											// 1�_�ł��L����OK
				ok = 1; break;
			}
		}
		wta[i] = ok;
	}
	for(i=0, vd=0, wln=0.0; i<DIM; i++) {
		if(wln<fabs(vec[bs].p[i])) {									// ��f���޸�ٍő��������
			wln = fabs(vec[bs].p[i]), vd = i;
		}
	}
	fg = (vec[bs].p[vd]<0) ? 1 : 0;										// ��f�ʍő�������׸�
	for(i=0, sx=-1; i<sn; i++) {
		if(sa[i]) {
			j = (sf%2) ? i+1 : i;
			sx = (sx==-1) ? j : sx;										// �J�n�f�ʕۑ�
			ex = j;														// �I���f�ʕۑ�
			for(k=0; k<DIM; k++) {
				bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];					// ��f��-�Ώےf�� ���S�_�덷
			}
			ScalePoint(vec[j], vd, fg, bpt, bn, dn, sc, j, mx,
                       op, un, vn, pa, pb, pc);							// �w��f�ʐ���_�g��k��
		}
	}
	if(ub) {															// <U:�̏ꍇ>
		for(i=0, ct=0; i<sn; i++) {
			if(sa[i]) {													// �擪�̎w���ς�
				j = i;
				for(k=0, l=0; k<sn; k++) {
					j = (j+1>sn-1) ? 0 : j+1;
					if(sa[j]) {											// �Ō�̎w���ς�
						break;
					} else {											// ���Ԃ̖��w������
						l++;
					}
				}
				if(l>ct) {												// �Œ��̖��w���̈�
					ct = l, sx = j, ex = i;
				}
			}
		}
		sx = (sf%2) ? sx+1 : sx;										// �J�n�f�ʕۑ�
		ex = (sf%2) ? ex+1 : ex;										// �I���f�ʕۑ�
		ct = ct/2;														// �L���f�ʐ�(�w��f�ʑO/�㕪)
	}
	if(ten>0) {															// <<<���͐ݒ�L�̏ꍇ>>>
		ct = (!ub) ? sx : ct;											// <<�w��f�ʑO�g��k��>>
		for(i=0, j=sx, lst=-1; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			lst = (wta[j]) ? j : lst;									// �ŏI���͗L���_����
		}
		ok = (lst==-1) ? 0 : 1;											// ���͗L���_�L��
		lst = (lst-1<0) ? un-1 : lst-1;									// �L���_�O�������܂�
		for(i=0, sln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			sln = (ok) ? sln+uln[j] : sln;								// �w��ʑO���S�������v
			ok = (j==lst) ? 0 : ok;										// �ŏI���͗L���_�ȍ~�͖���
		}
		for(i=0, wln=0.0, j=sx; i<ct; i++) {
			j = (j-1<0) ? un-1 : j-1;
			wln += uln[j];												// �w��f�ʂ���̒��S����
			if(wta[j]) {												// <���͗L���f��>
				wsc = sc - 1.0;											// �w��f�ʔ{��
				GetTensionFltValue(ten, sln, wln, &wsc);				// ���͐ݒ�l�ϊ�
				wsc = wsc + 1.0;
				for(k=0; k<DIM; k++) {									// ��f�ʂƑΏےf�ʂ�
					bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];				// ���S�_�̌덷
				}
				ScalePoint(vec[j], vd, fg, bpt, bn, dn, wsc, j,
                           mx, op, un, vn, pa, pb, pc);					// ����_�g��/�k��
			}
		}
		ct = (!ub) ? un-ex-1 : ct;										// <<�w��f�ʌ�g��k��>>
		for(i=0, j=ex, lst=-1; i<ct; i++) {
			lst = (wta[j]) ? j : lst;									// �ŏI���͗L���_����
			j = (j+1>un-1) ? 0 : j+1;
		}
		ok = (lst==-1) ? 0 : 1;											// ���͗L���_�L��
		for(i=0, sln=0.0, j=ex; i<ct; i++) {
			k = (j+1>un-1) ? 0 : j+1;
			sln = (ok||wta[k]) ? sln+uln[j] : sln;						// �w��ʌ㒆�S�������v
			ok = (j==lst) ? 0 : ok;										// �ŏI���͗L���_�ȍ~�͖���
			j = (j+1>un-1) ? 0 : j+1;
		}
		for(i=0, wln=0.0, j=ex; i<ct; i++) {
			wln += uln[j];												// �w��f�ʂ���̒��S����
			j = (j+1>un-1) ? 0 : j+1;
			if(wta[j]) {												// <���͗L���f��>
				wsc = sc - 1.0;											// �w��f�ʔ{��
				GetTensionFltValue(ten, sln, wln, &wsc);				// ���͐ݒ�l�ϊ�
				wsc = wsc + 1.0;
				for(k=0; k<DIM; k++) {									// ��f�ʂƑΏےf�ʂ�
					bpt.p[k] = cpt[j].p[k]-cpt[bs].p[k];				// ���S�_�̌덷
				}
				ScalePoint(vec[j], vd, fg, bpt, bn, dn, wsc, j,
                           mx, op, un, vn, pa, pb, pc);					// ����_�g��/�k��
			}
		}
	}
	delete[] wta;
	delete[] uln;
}

/***********************************************************/
void MoveSCurve(int md, double *db, PNTTYPE* ma,
                OBJTYPE* op, int pn, int bc, PNTTYPE* pa,
                PNTTYPE* pb, int* ia, PNTTYPE* pc, int* ta)				//<<< �f�ʐ���_�ړ�
/***********************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxCCtA];
	int*     flg = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	PNTTYPE  opt, npt;
	int      i, j, k, ten, p1, p2, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ��擾
	opt.p[0] = 0.0, opt.p[1] = 0.0, opt.p[2] = 0.0;						// ���W�l:0�z��쐬
	for(i=0; i<pn; i++) {
		if(ia[i]) {														// �ړ���/�׸ޔz�񕡎�
			mov[i] = ma[i], flg[i] = 1;
		} else {
			mov[i] = opt,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<���͐ݒ�L�̏ꍇ>>
		for(i=0, j=0, p1=-1, p2=-1; i<pn-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <�O���׸ވ�>
				j++;													// �w��_���E����
				if(flg[i]) {											// ���E1�ʒu
					p1 = i+1;
				} else {												// ���E2�ʒu
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <�w��_�A��>
			for(i=0; i<pn; i++) {
				len[i] = (i==pn-1) ? PntD(&pb[i], &pb[0])
                                   : PntD(&pb[i], &pb[i+1]);			// ����_�ԋ�������
			}
			if(p1==-1) {												// �w��_�O��ʒu&�_���ݒ�
				if(bc) {
					sct = (p2+1)/2, ect = (p2+1)/2;
					sno = p2, eno = pn-1;
				} else {
					sct = p2+1, ect = 0;
					sno = p2, eno = pn-1;
				}
			} else if(p2==-1) {
				if(bc) {
					sct = (pn-p1)/2, ect = (pn-p1)/2;
					sno = pn-1, eno = p1-1;
				} else {
					sct = 0, ect = pn-p1;
					sno = pn-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(bc) {
					sct = (p2+1+pn-p1)/2, ect = (p2+1+pn-p1)/2;
					sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = pn-p1;
					sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
				sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <�w��_�O����_�ړ�>
				lst = (ta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j-1<0) ? pn-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			lst = (lst-1<0) ? pn-1 : lst-1;								// �L���_�O�������܂�
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// �w�萧��_����̋���
				if(ta[j]) {												// <���͗L���_>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// ���͐ݒ�l�ϊ�
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// �w���׸�ON
					}
				}
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <�w��_�㐧��_�ړ�>
				lst = (ta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j+1>pn-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>pn-1) ? 0 : j+1;
				sln = (ok||ta[k]) ? sln+len[j] : sln;					// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j+1>pn-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// �w�萧��_����̋���
				j = (j+1>pn-1) ? 0 : j+1;
				if(ta[j]) {												// <���͗L���_>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// ���͐ݒ�l�ϊ�
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// �w���׸�ON
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i].p[j];									// ���̕ҏW&�ړ�����W
		}
	}
	CalcCurvePoint(md, pn, mov, cpt, db);								// �ʉߓ_=>����_�ϊ�
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// �ʉߓ_�͑S�Ĉړ��ς�
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// �ړ��O/�ړ��� ����_
		MoveCtrlPoint(op, opt, npt, 0, flg[i], &pc[i]);					// ����_�ړ�
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] len;
}

/****************************************************************/
void MoveSPoint(OBJTYPE* op, int un, int vn, int ub, int vb,
                PNTTYPE* mov, PNTTYPE* pa, PNTTYPE* pb, int* ta)		//<<< �f�ʐ���_�ړ�
/****************************************************************/
{
	int*     flg = new int[View->m_MaxSCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  opt, npt;
	int      i, j, k, l, ten, pn, mx=0, chk=1, p1, p2, ps, sct, ect, sno, eno, lst, ok;
	double   sln, wln;

	pn = un*vn;															// ������_��
	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ�
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	for(i=0; i<pn; i++) {
		for(j=0, k=0; j<DIM; j++) {
			if(fabs(mov[i].p[j])>EPSILON) {								// �����׸ސݒ�
				k = 1; break;
			}
		}
		flg[i] = k;
	}
	if(ten>0) {															// <<���͐ݒ�L�̏ꍇ>>
		for(i=0; i<vn; i++) {
			for(j=0, sct=0; j<un-1; j++) {								// <U���������m�F>
				sct = (flg[un*i+j]!=flg[un*i+j+1]) ? sct+1 : sct;		// �w��_���E����
			}
			if(sct>2) {													// ��������NG
				chk = 0; break;
			}
		}
		if(chk) {
			for(i=0; i<un; i++) {
				for(j=0, sct=0; j<vn-1; j++) {							// <V���������m�F>
					sct = (flg[i+un*j]!=flg[i+un*(j+1)]) ? sct+1 : sct;	// �w��_���E����
				}
				if(sct>2) {												// ��������NG
					chk = 0; break;
				}
			}
		}
	} else {															// ���͐ݒ薳NG
		chk = 0;
	}
	if(chk) {															// <<��������OK>>
		for(i=0; i<vn; i++) {											// <U�����ړ��ʍX�V>
			for(j=0, sct=0, p1=-1, p2=-1; j<un-1; j++) {
				if(flg[un*i+j]!=flg[un*i+j+1]) {						// <�O���׸ވ�>
					sct++;												// �w��_���E����
					if(flg[un*i+j]) {									// ���E1�ʒu
						p1 = j+1;
					} else {											// ���E2�ʒu
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <�w��_�A��>
				for(j=0; j<un; j++) {									// ����_�ԋ�������
					len[j] = (j==un-1) ? PntD(&pb[un*(i+1)-1], &pb[un*i])
                                       : PntD(&pb[un*i+j], &pb[un*i+j+1]);
					wta[j] = ta[un*i+j];								// ���͌����񒊏o
				}
				if(p1==-1) {											// �w��_�O��ʒu&�_���ݒ�
					if(ub) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = un-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = un-1;
					}
				} else if(p2==-1) {
					if(ub) {
						sct = (un-p1)/2, ect = (un-p1)/2;
						sno = un-1, eno = p1-1;
					} else {
						sct = 0, ect = un-p1;
						sno = un-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(ub) {
						sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = un-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <�w��_�O����_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k-1<0) ? un-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				lst = (lst-1<0) ? un-1 : lst-1;							// �L���_�O�������܂�
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k-1<0) ? un-1 : k-1;
				}
				ps = (sno+1>un-1) ? 0 : sno+1;							// ��ړ��ʔԒn
				ps = un*i+ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// �w�萧��_����̋���
					l = un*i+k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
					k = (k-1<0) ? un-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <�w��_�㐧��_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k+1>un-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>un-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k+1>un-1) ? 0 : k+1;
				}
				ps = un*i+eno;											// ��ړ��ʔԒn
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// �w�萧��_����̋���
					k = (k+1>un-1) ? 0 : k+1;
					l = un*i+k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
				}
			}
		}
		for(i=0; i<un; i++) {											// <V�����ړ��ʍX�V>
			for(j=0, sct=0, p1=-1, p2=-1; j<vn-1; j++) {
				if(flg[i+un*j]!=flg[i+un*(j+1)]) {						// <�O���׸ވ�>
					sct++;												// �w��_���E����
					if(flg[i+un*j]) {									// ���E1�ʒu
						p1 = j+1;
					} else {											// ���E2�ʒu
						p2 = j;
					}
				}
			}
			if(sct>0&&sct<=2) {											// <�w��_�A��>
				for(j=0; j<vn; j++) {									// ����_�ԋ�������
					len[j] = (j==vn-1) ? PntD(&pb[i+un*(vn-1)], &pb[i])
                                       : PntD(&pb[i+un*j], &pb[i+un*(j+1)]);
					wta[j] = ta[i+un*j];								// ���͌����񒊏o
				}
				if(p1==-1) {											// �w��_�O��ʒu&�_���ݒ�
					if(vb) {
						sct = (p2+1)/2, ect = (p2+1)/2;
						sno = p2, eno = vn-1;
					} else {
						sct = p2+1, ect = 0;
						sno = p2, eno = vn-1;
					}
				} else if(p2==-1) {
					if(vb) {
						sct = (vn-p1)/2, ect = (vn-p1)/2;
						sno = vn-1, eno = p1-1;
					} else {
						sct = 0, ect = vn-p1;
						sno = vn-1, eno = p1-1;
					}
				} else if(p1>p2) {
					if(vb) {
						sct = (p2+1+vn-p1)/2, ect = (p2+1+vn-p1)/2;
						sno = p2, eno = p1-1;
					} else {
						sct = p2+1, ect = vn-p1;
						sno = p2, eno = p1-1;
					}
				} else {
					sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
					sno = p2, eno = p1-1;
				}
				for(j=0, k=sno, lst=-1; j<sct; j++) {					// <�w��_�O����_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k-1<0) ? vn-1 : k-1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				lst = (lst-1<0) ? vn-1 : lst-1;							// �L���_�O�������܂�
				for(j=0, sln=0.0, k=sno; j<sct; j++) {
					sln = (ok) ? sln+len[k] : sln;						// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k-1<0) ? vn-1 : k-1;
				}
				ps = (sno+1>vn-1) ? 0 : sno+1;							// ��ړ��ʔԒn
				ps = i+un*ps;
				for(j=0, wln=0.0, k=sno; j<sct; j++) {
					wln += len[k];										// �w�萧��_����̋���
					l = i+un*k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
					k = (k-1<0) ? vn-1 : k-1;
				}
				for(j=0, k=eno, lst=-1; j<ect; j++) {					// <�w��_�㐧��_�ړ�>
					lst = (wta[k]) ? k : lst;							// �ŏI���͗L���_����
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ok = (lst==-1) ? 0 : 1;									// ���͗L���_�L��
				for(j=0, sln=0.0, k=eno; j<ect; j++) {
					l = (k+1>vn-1) ? 0 : k+1;
					sln = (ok||wta[l]) ? sln+len[k] : sln;				// ����_�ԋ����W�v
					ok = (k==lst) ? 0 : ok;								// �ŏI���͗L���_�ȍ~�͖���
					k = (k+1>vn-1) ? 0 : k+1;
				}
				ps = i+un*eno;											// ��ړ��ʔԒn
				for(j=0, wln=0.0, k=eno; j<ect; j++) {
					wln += len[k];										// �w�萧��_����̋���
					k = (k+1>vn-1) ? 0 : k+1;
					l = i+un*k;											// �ړ��ʔz��Ԓn
					if(wta[k]) {										// <���͗L���_>
						mov[l] = mov[ps];
						GetTensionPntValue(ten, sln, wln, &mov[l]);		// ���͐ݒ�l�ϊ�
						if(fabs(mov[l].p[0])>EPSILON||
                           fabs(mov[l].p[1])>EPSILON||
                           fabs(mov[l].p[2])>EPSILON) {
							flg[l] = 1;									// �w���׸�ON
						}
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i];													// ���̕ҏW�O����_
		for(j=0; j<DIM; j++) {
			npt.p[j] = pb[i].p[j]+mov[i].p[j];							// ���̕ҏW&�ړ��㐧��_
		}
		MoveCtrlPoint(op, opt, npt, mx, flg[i], &pa[i]);				// ����_�ړ�
	}
	delete[] flg;
	delete[] wta;
	delete[] len;
}

/**************************************************************/
static void CalcBaseLine(PNTTYPE* pp, int un, int vn, int ub,
                         int vflg, PNTTYPE* cp1, PNTTYPE* cp2)			//<<< �ް����ړ��v�Z
/**************************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3];
	VECTYPE  rvc, ovc, nvc, vc1, vc2;
	int      i, j, k;
	double   th, rot[3][3];

	for(i=0; i<un; i++) {
		for(j=0; j<vn; j++) {
			pt[j] = pp[i+j*un];											// V�f���ް����o
		}
		if(i==0) {														// <<�J�n�f�ʂ̏ꍇ>>
			if(vflg%2) {												// <�J�n�f�ʓ���>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[2].p[k]-cp2[0].p[k];					// �V���޸�َ擾
					nvc.p[k] = cp1[2].p[k]-cp1[0].p[k];
				}
			} else {													// <�J�n�f�ʕs����>
				if(!ub) {												// <U�J�̏ꍇ>
					for(k=0; k<DIM; k++) {
						ovc.p[k] = cp2[1].p[k]-cp2[0].p[k];				// �V���޸�َ擾
						nvc.p[k] = cp1[1].p[k]-cp1[0].p[k];
					}
				} else {												// <U�̏ꍇ>
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp2[0].p[k]-cp2[un-1].p[k];			// �O����޸�َ擾
						vc2.p[k] = cp2[1].p[k]-cp2[0].p[k];
					}
					VecN(&vc1);											// �P���޸�ى�
					VecN(&vc2);
					for(k=0; k<DIM; k++) { 
						ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// ���޸�َ擾
					}
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp1[0].p[k]-cp1[un-1].p[k];			// �O����޸�َ擾
						vc2.p[k] = cp1[1].p[k]-cp1[0].p[k];
					}
					VecN(&vc1);											// �P���޸�ى�
					VecN(&vc2);
					for(k=0; k<DIM; k++) {
						nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// �V�޸�َ擾
					}
				}
			}
		} else if(i==un-1) {											// <<�I���f�ʂ̏ꍇ>>
			if(vflg/2) {												// <�I���f�ʓ���>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[un-3].p[k]-cp2[un-1].p[k];			// �V���޸�َ擾
					nvc.p[k] = cp1[un-3].p[k]-cp1[un-1].p[k];
				}
			} else {													// <�I���f�ʕs����>
				if(!ub) {												// <U�J�̏ꍇ>
					for(k=0; k<DIM; k++) {
						ovc.p[k] = cp2[un-2].p[k]-cp2[un-1].p[k];		// �V���޸�َ擾
						nvc.p[k] = cp1[un-2].p[k]-cp1[un-1].p[k];
					}
				} else {												// <U�̏ꍇ>
					for(k=0; k<DIM; k++) {
						vc1.p[k] = cp2[un-1].p[k]-cp2[un-2].p[k];		// �O����޸�َ擾
						vc2.p[k] = cp2[0].p[k]-cp2[un-1].p[k];
					}
					VecN(&vc1);											// �P���޸�ى�
					VecN(&vc2);
					for(k=0; k<DIM; k++) { 
						ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// ���޸�َ擾
					}
					for(k=0; k<DIM; k++) {								// �O����޸�َ擾
						vc1.p[k] = cp1[un-1].p[k]-cp1[un-2].p[k];
						vc2.p[k] = cp1[0].p[k]-cp1[un-1].p[k];
					}
					VecN(&vc1);											// �P���޸�ى�
					VecN(&vc2);
					for(k=0; k<DIM; k++) {
						nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;				// �V�޸�َ擾
					}
				}
			}
		} else {														// <<���Ԓf�ʂ̏ꍇ>>
			if(vflg%2&&i==1) {											// <�J�n�f�ʓ���>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[2].p[k]-cp2[1].p[k];					// �V���޸�َ擾
					nvc.p[k] = cp1[2].p[k]-cp1[1].p[k];
				}
			} else if(vflg/2&&i==un-2) {								// <�I���f�ʓ���>
				for(k=0; k<DIM; k++) {
					ovc.p[k] = cp2[un-3].p[k]-cp2[un-2].p[k];			// �V���޸�َ擾
					nvc.p[k] = cp1[un-3].p[k]-cp1[un-2].p[k];
				}
			} else {													// <�ȊO�̏ꍇ>
				for(k=0; k<DIM; k++) {
					vc1.p[k] = cp2[i].p[k]-cp2[i-1].p[k];				// �O����޸�َ擾
					vc2.p[k] = cp2[i+1].p[k]-cp2[i].p[k];
				}
				VecN(&vc1);												// �P���޸�ى�
				VecN(&vc2);
				for(k=0; k<DIM; k++) {
					ovc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;					// ���޸�َ擾
				}
				for(k=0; k<DIM; k++) {
					vc1.p[k] = cp1[i].p[k]-cp1[i-1].p[k];				// �O����޸�َ擾
					vc2.p[k] = cp1[i+1].p[k]-cp1[i].p[k];
				}
				VecN(&vc1);												// �P���޸�ى�
				VecN(&vc2);
				for(k=0; k<DIM; k++) {
					nvc.p[k] = (vc1.p[k]+vc2.p[k])/2.0;					// �V�޸�َ擾
				}
			}
		}
		VecN(&ovc);														// �P���޸�ى�
		VecN(&nvc);
		for(j=0; j<vn; j++) {
			for(k=0; k<DIM; k++) {
				pt[j].p[k] -= cp2[i].p[k];								// �f�ʌ��_�ړ�
			}
		}
		for(k=0; k<DIM; k++) {
			wp[0].p[k] = 0.0;											// ���_���޸�ق̖�
			wp[1].p[k] = ovc.p[k]*1000.0;
			wp[2].p[k] = nvc.p[k]*1000.0;
		}
		CalcNVec(wp, 3, &rvc);											// ��]���擾
		if(fabs(ovc.p[0]-nvc.p[0])>EPSILON||
           fabs(ovc.p[1]-nvc.p[1])>EPSILON||
           fabs(ovc.p[2]-nvc.p[2])>EPSILON) {							// ��]�p�擾
			th = acos(ovc.p[0]*nvc.p[0]+ovc.p[1]*nvc.p[1]+ovc.p[2]*nvc.p[2]);
			th = (th<-10||th>10) ? 0.0 : th;
		} else {
			th = 0.0;
		}
		SetRotateMatrix(rvc, th, rot);									// ��]��د���쐬
		for(j=0; j<vn; j++) {
			VecRotateMove(&pt[j], rot, cp1[i]);							// ��]�ړ�
			pp[i+j*un] = pt[j];											// �f���ް��i�[
		}
	}
	delete[] pt;
}

/****************************************************************/
void MoveBPoint(int md, PNTTYPE* cpt, PNTTYPE* ma, OBJTYPE* op,
                int un, int vn, int ub, int vb, double wt,
                int vflg, int wn, int* ia, int* ib, PNTTYPE* pa,
                PNTTYPE* pb, PNTTYPE* pc, int* ta)						//<<< �ް����ړ�
/****************************************************************/
{
	PNTTYPE* wma = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* wcp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* wpb = new PNTTYPE[View->m_MaxSCtA];
	int*     flg = new int[View->m_MaxCCtA];
	int*     wta = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  zro;
	int      i, j, k, ten, p1, p2, st, sct, ect, sno, eno, mx=0, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ��擾
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	zro.p[0] = 0.0, zro.p[1] = 0.0, zro.p[2] = 0.0;						// ���W�l:0�z��쐬
	for(i=0; i<un; i++) {												// ��Ɨ̈揉����
		wcp[i] = zro, flg[i] = 0;
	}
	st = (vflg%2) ? 1 : 0;												// V�n�_�����+1
	for(i=0; i<wn; i++) {
		wma[i+st] = ma[i];												// �ړ��ʂ�V�f�ʈʒu�ɐݒ�
	}
	wma[0] = (vflg%2) ? wma[1] : wma[0];
	wma[un-1] = (vflg/2) ? wma[un-2] : wma[un-1];
	for(i=0; i<wn; i++) {
		if(ib[i]) {														// �Ώ��׸�ON/�ړ��ʐݒ�
			flg[i+st] = 1, wcp[i+st] = ma[i];
		}
	}
	if(vflg%2) {
		flg[0] = flg[1], wcp[0] = wcp[1];
	}
	if(vflg/2) {
		flg[un-1] = flg[un-2], wcp[un-1] = wcp[un-2];
	}
	if(ten>0) {															// <<���͐ݒ�L�̏ꍇ>>
		for(i=0, j=0, p1=-1, p2=-1; i<un-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <�O���׸ވ�>
				j++;													// �w��_���E����
				if(flg[i]) {											// ���E1�ʒu
					p1 = i+1;
				} else {												// ���E2�ʒu
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <�w��_�A��>
			for(i=0; i<un; i++) {
				len[i] = (i==un-1) ? PntD(&cpt[i], &cpt[0])
                                   : PntD(&cpt[i], &cpt[i+1]);			// ����_�ԋ�������
				for(ok=0, j=0; j<vn; j++) {								// ���͗L���m�F
					if(ta[i+un*j]) {									// 1�_�ł��L����OK
						ok = 1; break;
					}
				}
				wta[i] = ok;
			}
			if(p1==-1) {												// �w��_�O��ʒu&�_���ݒ�
				if(ub) {
					sct = (p2+1)/2, ect = (p2+1)/2;
					sno = p2, eno = un-1;
				} else {
					sct = p2+1, ect = 0;
					sno = p2, eno = un-1;
				}
			} else if(p2==-1) {
				if(ub) {
					sct = (un-p1)/2, ect = (un-p1)/2;
					sno = un-1, eno = p1-1;
				} else {
					sct = 0, ect = un-p1;
					sno = un-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(ub) {
					sct = (p2+1+un-p1)/2, ect = (p2+1+un-p1)/2;
					sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = un-p1;
					sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2;
				sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <�w��_�O����_�ړ�>
				lst = (wta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j-1<0) ? un-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			lst = (lst-1<0) ? un-1 : lst-1;								// �L���_�O�������܂�
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j-1<0) ? un-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// �w�萧��_����̋���
				if(wta[j]) {											// <���͗L���_>
					wcp[j] = wma[j];
					GetTensionPntValue(ten, sln, wln, &wcp[j]);			// ���͐ݒ�l�ϊ�
				}
				j = (j-1<0) ? un-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <�w��_�㐧��_�ړ�>
				lst = (wta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j+1>un-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>un-1) ? 0 : j+1;
				sln = (ok||wta[k]) ? sln+len[j] : sln;					// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j+1>un-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// �w�萧��_����̋���
				j = (j+1>un-1) ? 0 : j+1;
				if(wta[j]) {											// <���͗L���_>
					wcp[j] = wma[j];
					GetTensionPntValue(ten, sln, wln, &wcp[j]);			// ���͐ݒ�l�ϊ�
				}
			}
		}
	}
	for(i=0; i<un; i++) {
		for(j=0; j<DIM; j++) {
			wcp[i].p[j] += cpt[i].p[j];									// �ړ���ʒu�Z�o
		}
	}
	sct = un*vn;
	for(i=0; i<sct; i++) {
		wpb[i] = pb[i];													// ���ݐ���_����
	}
	CalcBaseLine(wpb, un, vn, ub, vflg, wcp, cpt);						// �ް����ړ��v�Z
	CalcSurfacePoint(md, 0, 0, un, vn, ub, vb, wt, ia, wpb, wma);		// �ʉߓ_=>����_�ϊ�
	for(i=0; i<sct; i++) {
		MoveCtrlPoint(op, pa[i], wma[i], mx, 1, &pc[i]);				// ����_�ړ�
	}
	delete[] wma;
	delete[] wcp;
	delete[] wpb;
	delete[] flg;
	delete[] wta;
	delete[] len;
}
