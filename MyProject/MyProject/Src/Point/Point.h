#pragma once
#include "../etc/VertexData.h"
#include <vector>
#include <memory>

struct ID3D12Resource;

class Union;
class Device;
class List;
class Constant;
class Root;
class Pipe;

class Point
{
public:
	// �R���X�g���N�^
	Point(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Point();

	// ���_�̒ǉ�
	void AddVertexPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float& alpha);
	
	// �`��
	void Draw(void);

	// ���_�̃��Z�b�g
	void Reset(void);

private:
	// ���\�[�X�̐���
	long Create(void);

	// �}�b�v
	long Map(void);


	// ���j�I��
	Union& un;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �萔�o�b�t�@
	std::weak_ptr<Constant>con;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// ���\�[�X
	ID3D12Resource* rsc;

	// ���M�f�[�^
	unsigned int* data;

	// ���_�f�[�^
	std::vector<draw::Vertex>vertex;
};
