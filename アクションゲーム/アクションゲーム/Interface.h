#pragma once
#include "Player.h"
#include <memory>

class Interface
{
public:
	// �R���X�g���N�^
	Interface(std::weak_ptr<Player>pl);
	// �f�X�g���N�^
	virtual ~Interface();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// ���UI
	int top;

	// ����UI
	int bottom;

	// �v���C���[�̗̑�
	int hp;
};

