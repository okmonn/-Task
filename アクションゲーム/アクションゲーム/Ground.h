#pragma once
#include "Player.h"
#include <memory>

class Ground
{
public:
	// �R���X�g���N�^
	Ground(std::weak_ptr<Player>pl);
	// �f�X�g���N�^
	~Ground();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// �n�ʍ��W�̎擾
	int GetHeight(void);

private:
	// �v���C���[
	std::weak_ptr<Player>pl;

	// �n�ʍ��W
	int y;
};

