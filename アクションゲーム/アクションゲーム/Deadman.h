#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>

class Deadman :
	public Enemy
{
public:
	// �R���X�g���N�^
	Deadman();
	Deadman(Positionf pos, std::weak_ptr<Player>pl);
	// �f�X�g���N�^
	~Deadman();

	// ��Ԃ̐ݒ�
	void SetMode(std::string m, bool r);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// �����蔻��
	bool CheackHit(void);
	// �����̏���
	void Walk(void);
	// ���S�̏���
	void Die(void);

	// �֐��|�C���^
	void(Deadman::*func)(void);
};

