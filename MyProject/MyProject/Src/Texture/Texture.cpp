#include <DirectXTex.h>
#include "Texture.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "TextureLoader.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// ���_�ő吔
#define MAX 4

// �R���X�g���N�^
Texture::Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, 
	std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<TextureLoader>loader) :
	dev(dev), list(list), con(con), root(root), pipe(pipe), loader(loader)
{
	tex.clear();
}

// �f�X�g���N�^
Texture::~Texture()
{
	for (auto itr = tex.begin(); itr != tex.end(); ++itr)
	{
		UnMap(itr->second.v_rsc);
		Release(itr->second.v_rsc);
		Release(itr->second.heap);
	}
}

// �q�[�v�̐���
long Texture::CreateHeap(int * i)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&tex[i].heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("�e�N�X�`���p�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�r���[�̐���
void Texture::CreateView(int * i)
{
	//�V�F�[�_���\�[�X�r���[�ݒ�p�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(tex[i].c_rsc, &desc, tex[i].heap->GetCPUDescriptorHandleForHeapStart());
}

// ���_���\�[�X�̐���
long Texture::CreateRsc(int * i)
{
	//�z��̃������m��
	tex[i].vertex.resize(MAX);

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
	desc.Width              = sizeof(tex::Vertex) * MAX;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tex[i].v_rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�|�C���g�p���_���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �}�b�v
long Texture::Map(int * i)
{
	auto hr = tex[i].v_rsc->Map(0, nullptr, (void**)(tex[i].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���̒��_�}�b�s���O�F���s\n"));
		return hr;
	}
	//���_�f�[�^�̃R�s�[
	memcpy(tex[i].data, tex[i].vertex.data(), sizeof(tex::Vertex) * tex[i].vertex.size());

	return 0;
}

// �ǂݍ���
long Texture::Load(const std::string & fileName, int & i)
{
	auto hr = loader.lock()->Load(fileName, tex[&i].c_rsc, tex[&i].meta, tex[&i].img);
	if (FAILED(hr))
	{
		return hr;
	}

	CreateHeap(&i);
	CreateView(&i);
	CreateRsc(&i);
	Map(&i);

	return hr;
}

// �`�揀��
long Texture::SetDraw(int & i, const unsigned int & rootNum)
{
	//�{�b�N�X�ݒ�p�\���̂̐ݒ�
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = tex[&i].meta->height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[&i].meta->width);
	box.top    = 0;

	//�T�u���\�[�X�ɏ�������
	auto hr = tex[&i].c_rsc->WriteToSubresource(0, &box, tex[&i].img->GetPixels(), tex[&i].meta->width * 4, tex[&i].meta->height * 4);
	if (FAILED(hr))
	{
		OutputDebugString(_T("�e�N�X�`���̃T�u���\�[�X�����݁F���s\n"));
		return hr;
	}

	SetHeap(i, rootNum);

	return hr;
}

// �q�[�v�̃Z�b�g
void Texture::SetHeap(int & i, const unsigned int & rootNum)
{
	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &tex[&i].heap);

	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(rootNum, tex[&i].heap->GetGPUDescriptorHandleForHeapStart());
}

// �`��
void Texture::Draw(int & i, const float & x, const float & y, const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	//UV���W
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(tex[n].meta->width) * turnX),                                            (static_cast<FLOAT>(tex[n].meta->height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(tex[n].meta->width) - (static_cast<FLOAT>(tex[n].meta->width) * turnX)), (static_cast<FLOAT>(tex[n].meta->height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(tex[n].meta->width) * turnX),                                            (static_cast<FLOAT>(tex[n].meta->height) - (static_cast<FLOAT>(tex[n].meta->height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(tex[n].meta->width) - (static_cast<FLOAT>(tex[n].meta->width) * turnX)), (static_cast<FLOAT>(tex[n].meta->height) - (static_cast<FLOAT>(tex[n].meta->height) * turnY)) };

	//����
	tex[n].vertex[0] = { { x,                                          y,                                           0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x + static_cast<FLOAT>(tex[n].meta->width), y,                                           0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x,                                          y + static_cast<FLOAT>(tex[n].meta->height), 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x + static_cast<FLOAT>(tex[n].meta->width), y + static_cast<FLOAT>(tex[n].meta->height), 0.0f }, rightDown, alpha };

	//���_�f�[�^�̍X�V
	memcpy(tex[n].data, tex[n].vertex.data(), sizeof(tex::Vertex) * tex[n].vertex.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = tex[n].v_rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(tex::Vertex) * tex[n].vertex.size();
	view.StrideInBytes  = sizeof(tex::Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (FAILED(SetDraw(i)))
	{
		return;
	}

	//�`��
	list.lock()->GetList()->DrawInstanced(tex[n].vertex.size(), 1, 0, 0);
}
