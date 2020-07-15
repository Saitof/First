/////////////////////////////////////////////////////////////////////////////
// ���C���`��N���X(CLine)�錾
// Line.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "DataType.h"

class CLine
{
// �R���X�g���N�V����
public:
	CLine(void);

// �I�y���[�V����
public:
	void Setup(PNTTYPE* ps, PNTTYPE* pe, int* cl, int pn, int md);
	void Draw(int plane);
	void Reset(void);
	BOOL EnableDraw(void) { return SetFlg; }

// �C���v�������e�[�V����
public:
	virtual ~CLine(void) { Reset(); }

private:
	BOOL SetFlg;
	UINT Lsno;
	int  Mode;
};

//===============================<EOF:Line.h>================================
