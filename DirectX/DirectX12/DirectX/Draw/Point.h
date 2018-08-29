#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <map>

class Point :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Point(std::weak_ptr<Device>dev, std::weak_ptr<List>list, const Vec2f& pos, const Vec3f& color);
	// �f�X�g���N�^
	~Point();

	// �`��
	HRESULT Draw(void);

private:
	// ���\�[�X�̐���
	HRESULT CreateResource(void);


	// ���_�f�[�^
	Vertex vertex;

	// ���\�[�X
	ID3D12Resource * resource;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW view;

	//���M�f�[�^
	UINT8* data;

	// ���W
	Vec2f pos;

	// �F
	Vec3f color;
};

