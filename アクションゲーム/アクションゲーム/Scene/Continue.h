#pragma once
#include "Scene.h"
class Continue :
	public Scene
{
public:
	// �R���X�g���N�^
	Continue(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	~Continue();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �摜�f�[�^
	int image;

	// �t���[��
	int flam;
};
