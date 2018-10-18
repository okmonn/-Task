#include "Constant.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// ���\�[�X�ő吔
#define RSC_MAX 1

#define RAD(X) (X) * 3.14159265f / 180.0f

// �R���X�g���N�^
Constant::Constant(std::weak_ptr<Device>dev, std::weak_ptr<List>list) : 
	un(Union::Get()), dev(dev), list(list), heap(nullptr), rsc(nullptr), data(nullptr)
{
	wvp = {};

	Init();
}

// �f�X�g���N�^
Constant::~Constant()
{
	UnMap(rsc);
	Release(rsc);
	Release(heap);
}

// WVP�̏�����
void Constant::InitWvp(void)
{
	//�_�~�[�錾
	FLOAT pos = 0.0f;
	//�J�����̈ʒu
	DirectX::XMVECTOR eye    = { 0, pos,  -1 };
	//�J�����̏œ_ 
	DirectX::XMVECTOR target = { 0, pos,   0 };
	//�J�����̏����
	DirectX::XMVECTOR upper  = { 0, 1,     0 };

	DirectX::XMStoreFloat4x4(&wvp.view, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&wvp.projection, DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), (float)un.GetWinX() / (float)un.GetWinY(), 0.5f, 500.0f));
	DirectX::XMStoreFloat4x4(&wvp.world, DirectX::XMMatrixIdentity());

	wvp.winSize = { (float)un.GetWinX(), (float)un.GetWinY() };
}

// �q�[�v�̐���
long Constant::CreateHeap(void)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�萔�o�b�t�@�p�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�̐���
long Constant::CreateRsc(void)
{
	//�v���p�e�B�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height             = 1;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels          = 1;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = (sizeof(WVP) + 0xff) &~ 0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�萔�o�b�t�@�p���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�r���[�̐���
void Constant::CreateView(void)
{
	//�萔�o�b�t�@�r���[�ݒ�p�\����
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	dev.lock()->Get()->CreateConstantBufferView(&desc, heap->GetCPUDescriptorHandleForHeapStart());
}

// �}�b�v
long Constant::Map(void)
{
	auto hr = rsc->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�萔�o�b�t�@�̃}�b�v�F���s\n"));
		return hr;
	}

	//�f�[�^���R�s�[
	memcpy(data, &wvp, sizeof(WVP));

	return hr;
}

// ������
void Constant::Init(void)
{
	InitWvp();
	CreateHeap(); 
	CreateRsc();
	CreateView();
	Map();
}

// WVP�̕ύX
void Constant::ChangeWvp(const float & eyeX, const float & eyeY, const float & eyeZ, const float & targetX, const float & targetY, const float & targetZ, const float & upX, const float & upY, const float & upZ)
{
	DirectX::XMVECTOR eye = { eyeX, eyeY, eyeZ };
	DirectX::XMVECTOR tar = { targetX, targetY, targetZ };
	DirectX::XMVECTOR upr = { upX, upY, upZ };
	DirectX::XMStoreFloat4x4(&wvp.view, DirectX::XMMatrixLookAtLH(eye, tar, upr));

	//�s��f�[�^�X�V
	memcpy(data, &wvp, sizeof(WVP));
}

// WVP�̍X�V
void Constant::UpDataWvp(const float & angle)
{
	//�s��X�V
	DirectX::XMStoreFloat4x4(&wvp.world, DirectX::XMMatrixRotationY(RAD(angle)));

	//�s��f�[�^�X�V
	memcpy(data, &wvp, sizeof(WVP));
}

// �萔�o�b�t�@�̃Z�b�g
void Constant::SetConstant(const unsigned int & index)
{
	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);

	//���[�g�V�O�l�`���Ƃ̊֘A�Â�
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(index, heap->GetGPUDescriptorHandleForHeapStart());
}
