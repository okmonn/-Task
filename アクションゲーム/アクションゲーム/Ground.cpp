#include "Ground.h"
#include "Typedef.h"
#include "DxLib.h"

// �R���X�g���N�^
Ground::Ground(std::weak_ptr<Player>pl) : pl(pl)
{
	//�n�ʍ��W
	y = 340;
}

// �f�X�g���N�^
Ground::~Ground()
{
}

// �`��
void Ground::Draw(void)
{
#ifdef _DEBUG
	DrawLine(0, y, WINDOW_X, y, GetColor(255, 0, 0), false);
#endif
}

// ����
void Ground::UpData(void)
{
	if (pl.lock()->GetMode() == "Jump")
	{
		if (pl.lock()->GetPos().y >= y)
		{
			pl.lock()->SetPos(330.0f);
			pl.lock()->SetMode("Ground", pl.lock()->GetReverse());
		}
	}
}

// �n�ʍ��W�̎擾
int Ground::GetHeight(void)
{
	return y;
}
