#include "Constant.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// �R���X�g���N�^
Constant::Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : win(win)
{
	this->dev = dev;
	this->list = list;
	this->swap = swap;
	wvp = {};
	data = nullptr;


	SetWorldViewProjection();
	CreateResource();
	CreateView();
}

// �f�X�g���N�^
Constant::~Constant()
{
	Liberation(true);
}

// WVP�̃Z�b�g
void Constant::SetWorldViewProjection(void)
{
	//�_�~�[�錾
	FLOAT pos = 0.0f;
	DirectX::XMMATRIX view   = DirectX::XMMatrixIdentity();
	//�J�����̈ʒu
	DirectX::XMVECTOR eye    = { 0, pos,  -1 };
	//�J�����̏œ_
	DirectX::XMVECTOR target = { 0, pos,   0 };
	//�J�����̏����
	DirectX::XMVECTOR upper  = { 0, 1,     0 };

	view = DirectX::XMMatrixLookAtLH(eye, target, upper);

	//�_�~�[�錾
	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();

	projection = DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), ((static_cast<FLOAT>(win.lock()->GetX()) / static_cast<FLOAT>(win.lock()->GetY()))), 0.5f, 500.0f);

	//�X�V
	wvp.world = DirectX::XMMatrixIdentity();
	wvp.viewProjection = view * projection;
}

// ���\�[�X�̐���
HRESULT Constant::CreateResource(void)
{
	result = CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	if (FAILED(result))
	{
		return result;
	}

	//�v���p�e�B�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = (sizeof(WVP) + 0xff) &~0xff;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resource.resize(sizeof(BYTE));

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource[0]));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�萔�o�b�t�@�̃��\�[�X�̐����F���s\n"));
	}

	return result;
}

// ���\�[�X�r���[�̐���
HRESULT Constant::CreateView(void)
{
	//�萔�o�b�t�@�r���[�ݒ�p�\���̂̐ݒ�
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = resource[0]->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	//�萔�o�b�t�@�r���[����
	dev.lock()->Get()->CreateConstantBufferView(&desc, GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//���M�͈�
	D3D12_RANGE range = { 0, 0 };

	//�}�b�s���O
	result = resource[0]->Map(0, &range, (void**)(&data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�萔�o�b�t�@�̃}�b�v�F���s\n"));
	}

	//�R�s�[
	memcpy(data, &wvp, sizeof(DirectX::XMMATRIX));

	return result;
}

// WVP�̍X�V
void Constant::UpDataWVP(void)
{
	static float angle = 0.0f;
	//��]
	angle++;
	//�s��X�V
	wvp.world = DirectX::XMMatrixRotationY(RAD(angle));

	//�s��f�[�^�X�V
	memcpy(data, &wvp, sizeof(WVP));
}

// �萔�o�b�t�@�̃Z�b�g
void Constant::SetConstant(UINT index, UINT table)
{
	//�萔�o�b�t�@�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap[index]->GetGPUDescriptorHandleForHeapStart();

	//�萔�o�b�t�@�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap[index]);

	//�萔�o�b�t�@�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(table, handle);
}
