#include "Point.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Point::Point(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) : 
	un(Union::Get()), dev(dev), list(list), con(con), root(root), pipe(pipe), rsc(nullptr), data(nullptr)
{
	vertex.clear();

	Create();
	Map();
}

// �f�X�g���N�^
Point::~Point()
{
	Reset();
	UnMap(rsc);
	Release(rsc);
}

// ���\�[�X�̐���
long Point::Create(void)
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
	desc.Width              = sizeof(draw::Vertex) * un.GetWinX() * un.GetWinY();
	
	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�|�C���g�̒��_���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �}�b�v
long Point::Map(void)
{
	//�}�b�s���O
	auto hr = rsc->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�|�C���g�̒��_�}�b�s���O�F���s\n"));
		return hr;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(data, vertex.data(), sizeof(draw::Vertex) * vertex.size());

	return hr;
}

// ���_�̒ǉ�
void Point::AddVertexPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	if (vertex.size() >= un.GetWinX() * un.GetWinY())
	{
		return;
	}

	vertex.push_back({ { x, y, 0.0f },{ r, g, b, alpha } });
}

// �`��
void Point::Draw(void)
{
	if (vertex.size() <= 0)
	{
		return;
	}

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	//���_�f�[�^�̍X�V
	memcpy(data, vertex.data(), sizeof(draw::Vertex) * vertex.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(draw::Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(draw::Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);
}

// ���_�̃��Z�b�g
void Point::Reset(void)
{
	vertex.clear();
	vertex.shrink_to_fit();
}
