#include "FirstRender.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Depth/Depth.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// ���_�ő吔
#define MAX 4  

// �R���X�g���N�^
FirstRender::FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Depth>dep, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	vertex(nullptr), data(nullptr), dep(dep)
{
	this->dev = dev;
	this->list = list;
	this->root = root;
	this->pipe = pipe;

	v.clear();

	Init();
}

// �f�X�g���N�^
FirstRender::~FirstRender()
{
	Release(vertex);
	Release(rsc);
	Release(srv);
	Release(rtv);
}

// ������
void FirstRender::Init(void)
{
	CreateRsc();
	CreateRtv();
	CreateSrv();

	CreateVertex();
	Map();
}

// �����_�[�^�[�Q�b�g�r���[�̐���
void FirstRender::CreateRtv(void)
{
	if (FAILED(CreateHeap(&rtv, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE)))
	{
		return;
	}

	//�����_�[�^�[�Q�b�g�ݒ�p�\����
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	dev.lock()->Get()->CreateRenderTargetView(rsc, &desc, rtv->GetCPUDescriptorHandleForHeapStart());
}

// �V�F�[�_�[���\�[�X�r���[�̐���
void FirstRender::CreateSrv(void)
{
	if (FAILED(CreateHeap(&srv, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)))
	{
		return;
	}

	//�V�F�[�_���\�[�X�r���[�ݒ�p�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(rsc, &desc, srv->GetCPUDescriptorHandleForHeapStart());
}

// ���_���\�[�X�̐���
long FirstRender::CreateVertex(void)
{
	v.resize(MAX);

	//�v���p�e�B�ݒ�p�\����
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\����
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
	desc.Width              = sizeof(multi::Vertex) * MAX;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertex));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�t�@�[�X�g�p�X�p���_���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// ���_�̃}�b�v
long FirstRender::Map(void)
{
	auto hr = vertex->Map(0, nullptr, (void**)&data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�t�@�[�X�g�p�X�p���_�̃}�b�v�F���s\n"));
	}

	return hr;
}

// �`��
void FirstRender::Draw(void)
{
	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	auto desc = rsc->GetDesc();

	//����
	v[0] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } };
	//�E��
	v[1] = { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } };
	//����
	v[2] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };
	//�E��
	v[3] = { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } };

	//���_�f�[�^�̍X�V
	memcpy(data, v.data(), sizeof(multi::Vertex) * v.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = vertex->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(multi::Vertex) * v.size();
	view.StrideInBytes  = sizeof(multi::Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�{�b�N�X�ݒ�p�\���̂̐ݒ�
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = vertex->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(vertex->GetDesc().Width);
	box.top = 0;

	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &srv);

	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(0, srv->GetGPUDescriptorHandleForHeapStart());

	/*auto d = dep.lock()->GetSrvHeap();
	list.lock()->GetList()->SetDescriptorHeaps(1, &d);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(1, d->GetGPUDescriptorHandleForHeapStart());*/

	//�`��
	list.lock()->GetList()->DrawInstanced(v.size(), 1, 0, 0);
}
