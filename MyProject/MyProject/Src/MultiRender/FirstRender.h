#pragma once
#include "MultiRender.h"

class FirstRender :
	public MultiRender
{
public:
	// �R���X�g���N�^
	FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<Render>render);
	// �f�X�g���N�^
	~FirstRender();

private:
	// ������
	void Init(void);

	// �����_�[�^�[�Q�b�g�r���[�̐���
	void CreateRtv(void);

	// �V�F�[�_�[���\�[�X�r���[�̐���
	void CreateSrv(void);
};
