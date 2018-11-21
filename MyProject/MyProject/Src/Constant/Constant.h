#pragma once
#include <DirectXMath.h>
#include <memory>

struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Union;
class Device;
class List;

class Constant
{
	// WVP
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 light;
		DirectX::XMFLOAT4 eye;
		DirectX::XMFLOAT2   winSize;
	};

public:
	// �R���X�g���N�^
	Constant(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~Constant();

	// WVP�̕ύX
	void ChangeWvp(const float& eyeX, const float& eyeY, const float& eyeZ,
		const float& targetX, const float& targetY, const float& targetZ, const float& upX, const float& upY, const float& upZ);

	// WVP�̍X�V
	void UpDataWvp(const float& angle);

	// �萔�o�b�t�@�̃Z�b�g
	void SetConstant(const unsigned int& index = 0);

private:
	// WVP�̏�����
	void InitWvp(void);

	// �q�[�v�̐���
	long CreateHeap(void);

	// ���\�[�X�̐���
	long CreateRsc(void);

	// ���\�[�X�r���[�̐���
	void CreateView(void);

	// �}�b�v
	long Map(void);

	// ������
	void Init(void);


	// ���j�I��
	Union& un;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	ID3D12Resource* rsc;

	// ���M�f�[�^
	unsigned int* data;

	// WVP
	WVP wvp;
};
