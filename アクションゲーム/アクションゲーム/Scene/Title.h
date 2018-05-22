#pragma once
#include "Scene.h"
#include <vector>
class Title :
	public Scene
{
public:
	// �R���X�g���N�^
	Title(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	~Title();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �t�F�[�h�C��
	void FadeIn(void);
	// �t�F�[�h�A�E�g
	void FadeOut(void);

	// �摜�n���h��
	int image;

	// �t���[��
	int flam;

	int cnt;

	// �֐��|�C���^
	void (Title::*func)(void);
};

