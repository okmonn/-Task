#pragma once
#include "Character.h"

class Player :
	public Character
{
public:
	// �R���X�g���N�^
	Player(std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:

};
