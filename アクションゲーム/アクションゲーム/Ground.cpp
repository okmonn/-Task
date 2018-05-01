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
	DrawLine(0, y, WINDOW_X, y, GetColor(255, 0, 0), false);
}

// ����
void Ground::UpData(void)
{
	if (pl.lock()->GetMode() != "Jump")
	{
		return;
	}

	if (pl.lock()->GetPos().y >= y)
	{
		pl.lock()->SetMode("Ground", pl.lock()->GetReverse());
		pl.lock()->SetPos(330.0f);
	}
}

// �n�ʍ��W�̎擾
int Ground::GetHeight(void)
{
	return y;
}
