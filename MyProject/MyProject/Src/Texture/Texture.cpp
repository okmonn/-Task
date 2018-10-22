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
	}
}

// ���\�[�X�r���[�̐���
void Texture::CreateView(int * i)
{
	//�V�F�[�_���\�[�X�r���[�ݒ�p�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = tex[i].c_rsc->GetDesc().Format;
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
		OutputDebugString(_T("\n�e�N�X�`���p���_���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �}�b�v
long Texture::Map(int * i)
{
	auto hr = tex[i].v_rsc->Map(0, nullptr, (void**)(&tex[i].data));
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
	if (FAILED(loader.lock()->Load(fileName)))
	{
		return S_FALSE;
	}

	//�A�h���X�Q��
	tex[&i].heap   = loader.lock()->GetHeap(fileName);
	tex[&i].c_rsc  = loader.lock()->GetConRsc(fileName);
	tex[&i].decode = loader.lock()->GetDecode(fileName);
	tex[&i].sub    = loader.lock()->GetSub(fileName);
	
	CreateView(&i);
	auto hr = CreateRsc(&i);
	hr = Map(&i);

	return hr;
}

// �`�揀��
long Texture::SetDraw(int & i, const unsigned int & rootNum)
{
	//�{�b�N�X�ݒ�p�\���̂̐ݒ�
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = tex[&i].c_rsc->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[&i].c_rsc->GetDesc().Width);
	box.top    = 0;

	//�T�u���\�[�X�ɏ�������
	auto hr = tex[&i].c_rsc->WriteToSubresource(0, &box, &tex[&i].decode[0], tex[&i].sub->RowPitch, tex[&i].sub->SlicePitch);
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

	auto desc = tex[n].c_rsc->GetDesc();

	//UV���W
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };

	//����
	tex[n].vertex[0] = { { x,                                  y,                                   0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x + static_cast<FLOAT>(desc.Width), y,                                   0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x,                                  y + static_cast<FLOAT>(desc.Height), 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x + static_cast<FLOAT>(desc.Width), y + static_cast<FLOAT>(desc.Height), 0.0f }, rightDown, alpha };

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

// �`��E�T�C�Y�w��
void Texture::DrawSize(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY,
	const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	auto desc = tex[n].c_rsc->GetDesc();

	//UV���W
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };

	//����
	tex[n].vertex[0] = { { x,         y,         0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x + sizeX, y,         0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x,         y + sizeY, 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x + sizeX, y + sizeY, 0.0f }, rightDown, alpha };

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

// �`��E�T�C�Y�E�͈͎w��
void Texture::DrawRectSize(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, 
	const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	auto desc = tex[n].c_rsc->GetDesc();

	//UV���W
	//UV���W
	DirectX::XMFLOAT2 leftUp    = { rectX + (rectSizeX * turnX),               rectY + (rectSizeY * turnY) };
	DirectX::XMFLOAT2 rightUp   = { rectX + (rectSizeX - (rectSizeX * turnX)), rectY + (rectSizeY * turnY) };
	DirectX::XMFLOAT2 leftDown  = { rectX + (rectSizeX * turnX),               rectY + (rectSizeY - (rectSizeY * turnY)) };
	DirectX::XMFLOAT2 rightDown = { rectX + (rectSizeX - (rectSizeX * turnX)), rectY + (rectSizeY - (rectSizeY * turnY)) };

	//����
	tex[n].vertex[0] = { { x,         y,         0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x + sizeX, y,         0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x,         y + sizeY, 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x + sizeX, y + sizeY, 0.0f }, rightDown, alpha };

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

// �`��E���W4�_�w��
void Texture::FreelyDraw(int & i, const float & x1, const float & y1, const float & x2, const float & y2, 
	const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	auto desc = tex[n].c_rsc->GetDesc();

	//UV���W
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };

	//����
	tex[n].vertex[0] = { { x1, y1, 0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x2, y2, 0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x3, y3, 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x4, y4, 0.0f }, rightDown, alpha };

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

// �`��E���W4�_�E�͈͎w��
void Texture::FreelyDrawRect(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4, 
	const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//�R�}���h���X�g�ւ̃Z�b�g
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	auto desc = tex[n].c_rsc->GetDesc();

	//UV���W
	//UV���W
	DirectX::XMFLOAT2 leftUp    = { rectX + (rectSizeX * turnX),               rectY + (rectSizeY * turnY) };
	DirectX::XMFLOAT2 rightUp   = { rectX + (rectSizeX - (rectSizeX * turnX)), rectY + (rectSizeY * turnY) };
	DirectX::XMFLOAT2 leftDown  = { rectX + (rectSizeX * turnX),               rectY + (rectSizeY - (rectSizeY * turnY)) };
	DirectX::XMFLOAT2 rightDown = { rectX + (rectSizeX - (rectSizeX * turnX)), rectY + (rectSizeY - (rectSizeY * turnY)) };

	//����
	tex[n].vertex[0] = { { x1, y1, 0.0f }, leftUp,    alpha };
	//�E��
	tex[n].vertex[1] = { { x2, y2, 0.0f }, rightUp,   alpha };
	//����
	tex[n].vertex[2] = { { x3, y3, 0.0f }, leftDown,  alpha };
	//�E��
	tex[n].vertex[3] = { { x4, y4, 0.0f }, rightDown, alpha };

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
