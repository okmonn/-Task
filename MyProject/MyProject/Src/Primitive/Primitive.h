#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class List;
class Depth;
class Constant;
class Root;
class Pipe;

class Primitive
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

public:
	// �R���X�g���N�^
	Primitive();
	// �f�X�g���N�^
	virtual ~Primitive();

	// �`��
	void Draw(void);

protected:
	// ���_�Z�b�g
	void SetVertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& size);

	// ���\�[�X�̐���
	long CreateRsc(void);

	// �}�b�v
	long Map(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���X�g
	std::weak_ptr<List>list;

	// �萔
	std::weak_ptr<Constant>con;

	// �[�x�o�b�t�@
	std::weak_ptr<Depth>depth;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// ���\�[�X
	ID3D12Resource* rsc;

	//���_���
	std::vector<VERTEX>vertex;

	// ���M�f�[�^
	void* data;
};
