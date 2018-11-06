#pragma once
#include "MultiRender.h"
#include <vector>

class FirstRender :
	public MultiRender
{
public:
	// �R���X�g���N�^
	FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~FirstRender();

	// �`��
	void Draw(void);

private:
	// ������
	void Init(void);

	// �����_�[�^�[�Q�b�g�r���[�̐���
	void CreateRtv(void);

	// �V�F�[�_�[���\�[�X�r���[�̐���
	void CreateSrv(void);

	// ���_���\�[�X�̐���
	long CreateVertex(void);

	// ���_�̃}�b�v
	long Map(void);


	// ���_���\�[�X
	ID3D12Resource* vertex;

	// ���_
	std::vector<multi::Vertex>v;

	// ���M�f�[�^
	unsigned int* data;
};
