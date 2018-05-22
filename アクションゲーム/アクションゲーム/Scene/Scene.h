#pragma once
#include "../Input.h"
#include <memory>

class Scene
{
public:
	// �R���X�g���N�^
	Scene();
	// �f�X�g���N�^
	virtual ~Scene();

	// �`��
	virtual void Draw(void) = 0;
	// ����
	virtual void UpData(void) = 0;

protected:
	// �C���v�b�g�N���X
	std::weak_ptr<Input>in;

	// �����x
	int blend;

	// �t�H���g�T�C�Y
	int fSize;

	// �����J�n�t���O
	bool alpha;
};

