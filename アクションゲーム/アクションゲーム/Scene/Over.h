#pragma once
#include "Scene.h"
class Over :
	public Scene
{
public:
	// �R���X�g���N�^
	Over(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	~Over();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �摜�f�[�^
	int image;
};

