#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Bat :
	public Enemy
{
public:
	// �R���X�g���N�^
	Bat(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Bat();

	// ��Ԃ̐ݒ�
	void SetMode(std::string m, bool r);

	// �`��
	void Draw(void);
	// ����
	void UpData(void);

	void ChangeDir(void);

private:

	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// �J�����N���X
	std::weak_ptr<Camera>cam;

	// �e���g���[
	float range;

	// �J�E���g
	int cnt;

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

