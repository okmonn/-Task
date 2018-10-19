#include "Triangle.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// �`��ő吔
#define MAX 3 * 100

// �R���X�g���N�^
Triangle::Triangle(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) : 
	dev(dev), list(list), con(con), root(root), pipe(pipe), rsc(nullptr), data(nullptr)
{
	vertex.clear();

	Create();
	Map();
}

// �f�X�g���N�^
Triangle::~Triangle()
{
	Reset();
	UnMap(rsc);
	Release(rsc);
}

// ���\�[�X�̐���
long Triangle::Create(void)
{
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
	desc.Width              = sizeof(draw::Vertex) * MAX;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g���C�A���O���̒��_���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �}�b�v
long Triangle::Map(void)
{
	//�}�b�s���O
	auto hr = rsc->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g���C�A���O���̒��_�}�b�s���O�F���s\n"));
		return hr;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(data, vertex.data(), sizeof(draw::Vertex) * vertex.size());

	return hr;
}

// ���_�̒ǉ�
void Triangle::AddVertexPoint(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3,
	const float & r, const float & g, const float & b, const float & alpha)
{
	if (vertex.size() >= MAX)
	{
		return;
	}

	vertex.push_back({ { x1, y1, 0.0f },{ r, g, b, alpha } });
	vertex.push_back({ { x2, y2, 0.0f },{ r, g, b, alpha } });
	vertex.push_back({ { x3, y3, 0.0f },{ r, g, b, alpha } });
}

// �`��
void Triangle::Draw(void)
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

	//���_�o�b�t�@�ݒ�p�\����
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(draw::Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(draw::Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);
}

// ���_�̃��Z�b�g
void Triangle::Reset(void)
{
	vertex.clear();
	vertex.shrink_to_fit();
}
