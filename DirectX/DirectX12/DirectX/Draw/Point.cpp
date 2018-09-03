#include "Point.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../PipeLine/Pipe.h"
#include <tchar.h>

//�R���X�g���N�^
Point::Point(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe) :
	win(win), pipe(pipe), resource(nullptr), data(nullptr)
{
	this->dev = dev;
	this->list = list;
	vertex.clear();
	view = {};
	vertexMax = this->win.lock()->GetX() * this->win.lock()->GetY();

	CreateResource();
}

// �f�X�g���N�^
Point::~Point()
{
	if (resource != nullptr)
	{
		resource->Unmap(0, nullptr);
	}
	Release(resource);
	Release(heap);
}

// ���\�[�X�̐���
HRESULT Point::CreateResource(void)
{
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
	desc.Width            = sizeof(Vertex) * vertexMax;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�|�C���g�p���\�[�X�̐����F���s\n"));
		return result;
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	result = resource->Map(0, &range, reinterpret_cast<void**>(&data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�|�C���g�p���\�[�X�̃}�b�s���O�F���s\n"));
		return result;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(data, vertex.data(), sizeof(Vertex) * vertex.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	view.BufferLocation = resource->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// ���_�f�[�^�̒ǉ�
void Point::AddList(const Vec2f& pos, const Vec3f& color)
{
	vertex.push_back({ { pos.x, pos.y, 0.0f }, { 0.0f, 0.0f }, { color.x, color.y, color.z, 1.0f } });
}

// �`��
void Point::Draw(void)
{
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	//���_�f�[�^�̍X�V
	memcpy(data, vertex.data(), sizeof(Vertex) * vertex.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	view.BufferLocation = resource->GetGPUVirtualAddress();
	view.SizeInBytes = sizeof(Vertex) * vertex.size();
	view.StrideInBytes = sizeof(Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//�`��
	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);
}

// �z��̃��Z�b�g
void Point::Reset(void)
{
	vertex.clear();
}