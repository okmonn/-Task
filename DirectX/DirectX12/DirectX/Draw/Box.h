#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <vector>
#include <map>

class Pipe;

class Box :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Box(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, UINT max = 4 * 100);
	// �f�X�g���N�^
	~Box();

	// ���_�f�[�^�̒ǉ�
	void AddList(const Vec2f& pos, const Vec2f& size, const Vec3f& color, float alpha = 1.0f);

	// �`��
	void Draw(void);

	// �z��̃��Z�b�g
	void Reset(void);

private:
	// ���\�[�X�̐���
	HRESULT CreateResource(void);


	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// ���\�[�X
	ID3D12Resource * resource;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW view;

	//���M�f�[�^
	UINT8* data;

	// ���_�f�[�^
	std::vector<Vertex>vertex;

	// ���_�ő吔
	UINT vertexMax;
};

