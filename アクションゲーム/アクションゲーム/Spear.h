#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Spear :
	public Enemy
{
public:
	// �R���X�g���N�^
	Spear(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Spear();

	// ��Ԃ̃Z�b�g
	void SetMode(std::string m, bool r);
	// �`��
	void Draw(void);
	// ����
	void UpData(void);

private:
	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// �J����
	std::weak_ptr<Camera>cam;

	Positionf camPos;

	// �֐��|�C���^
	void(Spear::*func)();

	// �U���̏���
	void Attacker(void);
};

