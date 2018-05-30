#pragma once
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <memory>

class Deadman :
	public Enemy
{
public:
	// �R���X�g���N�^
	Deadman();
	Deadman(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Deadman();

	// ��Ԃ̐ݒ�
	void SetMode(std::string m, bool r);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	void ChangeDir(void);

	Positionf GetCamPos(void);
	CutData GetCut(void);

private:
	// �v���C���[�N���X
	std::weak_ptr<Player>pl;

	// �J�����N���X
	std::weak_ptr<Camera>cam;

	float down;

	int wait;

	int go;

	bool dir[2];

	Positionf camPos;

	// �����̏���
	void Walk(void);
	// ���S�̏���
	void Die(void);

	// �֐��|�C���^
	void(Deadman::*func)(void);
};

