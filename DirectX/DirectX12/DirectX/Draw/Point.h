#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <vector>
#include <map>

class Window;
class Pipe;

class Point :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Point(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Point();

	// ���_�f�[�^�̒ǉ�
	void AddList(const Vec2f& pos, const Vec3f& color);

	// �`��
	void Draw(void);

	// �z��̃��Z�b�g
	void Reset(void);

private:
	// ���\�[�X�̐���
	HRESULT CreateResource(void);


	// �E�B���h�E
	std::weak_ptr<Window>win; 

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

