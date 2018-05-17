#pragma once
#include "Enemy.h"
#include "Player.h"
#include <memory>

class Bat :
	public Enemy
{
public:
	// �R���X�g���N�^
	Bat(Positionf pos, std::weak_ptr<Player>pl);
	// �f�X�g���N�^
	~Bat();

	// ��Ԃ̐ݒ�
	void SetMode(std::string m, bool r);

	// �`��
	void Draw(void);
	// ����
	void UpData(void);

private:

	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// �e���g���[
	float range;

	// �֐��|�C���^
	void(Bat::*func)(void);

	// �ҋ@�̏���
	void Wait(void);
	// ��т̏���
	void Fly(void);
	// �_���[�W�̏���
	void Damage(void);
	// ���S�̏���
	void Die(void);
};

