#pragma once
#include "Character.h"
#include <map>
#include <functional>

class Player :
	public Character
{
public:
	// �R���X�g���N�^
	Player(std::weak_ptr<Camera>cam, const Vec2f& pos, const Vec2f& size = { 64.0f, 64.0f });
	// �f�X�g���N�^
	~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// ������
	void Init(void);

	// �A�j���[�V�����̏I���t���O
	bool CheckAnimEnd(void);

	// �ҋ@
	void Wait(void);

	// ����
	void Walk(void);

	// �U��
	void Attack(void);

	// ���Ⴊ��
	void Crouch(void);

	// �_���[�W
	void Damage(void);


	// �֐��|�C���^
	void (Player::*updata)(void);
};
